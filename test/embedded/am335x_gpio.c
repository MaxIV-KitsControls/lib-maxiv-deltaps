#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdint.h>

#include "gpio.h"

/*
 * definitions
 *
 * reference: AM335x Technical Reference Manual (TRM)
 * http://www.ti.com/lit/ug/spruh73g/spruh73g.pdf
 */

/* base addresses */
#define AM335X_GPIO0_BASE	0x44E07000
#define AM335X_GPIO1_BASE	0x4804C000
#define AM335X_GPIO2_BASE	0x481AC000
#define AM335X_GPIO3_BASE	0x481AE000
#define AM335X_GPIO_BANKS	4

#define AM335X_RESERVE(from, to) uint8_t reserved_ ## to [to - from]

/* gpio controller register map (TRM table 25-5, page 4021) */
struct am335x_gpio_regs {
    uint32_t gpio_revision;         /* 0x0000  GPIO Revision Register */
    AM335X_RESERVE(0x0004, 0x0010);
    uint32_t gpio_sysconfig;        /* 0x0010  System Configuration Register */
    AM335X_RESERVE(0x0014, 0x0020);
    uint32_t gpio_eoi;              /* 0x0020  End of Interrupt Register */
    uint32_t gpio_irqstatus_raw[2]; /* 0x0024  Interrupt Status Raw Registers */
    uint32_t gpio_irqstatus[2];     /* 0x002C  Interrupt Status Registers */
    uint32_t gpio_irqstatus_set[2]; /* 0x0034  Interrupt Enable Set Registers */
    uint32_t gpio_irqstatus_clr[2]; /* 0x003C  Interrupt Enable Clear Registers */
    AM335X_RESERVE(0x0044, 0x0114);
    uint32_t gpio_sysstatus;        /* 0x0114  System Status Register */
    AM335X_RESERVE(0x0118, 0x0130);
    uint32_t gpio_ctrl;             /* 0x0130  Module Control Register */
    uint32_t gpio_oe;               /* 0x0134  Output Enable Register */
    uint32_t gpio_datain;           /* 0x0138  Data Input Register */
    uint32_t gpio_dataout;          /* 0x013C  Data Output Register */
    uint32_t gpio_leveldetect[2];   /* 0x0140  Low-level Detection Enable Registers */
    uint32_t gpio_risingdetect;     /* 0x0148  Rising-edge Detection Enable Register */
    uint32_t gpio_fallingdetect;    /* 0x014C  Falling-edge Detection Enable Register */
    uint32_t gpio_debouncenable;    /* 0x0150  Debounce Enable Register */
    uint32_t gpio_debouncingtime;   /* 0x0154  Debouncing Time Register */
    AM335X_RESERVE(0x0158, 0x0190);
    uint32_t gpio_cleardataout;     /* 0x0190  Clear Data Output Register */
    uint32_t gpio_setdataout;       /* 0x0194  Set Data Output Register */
};

#define BANK(pin)     ((pin) / 32)
#define BITMASK(pin)  (1 << ((pin) % 32))

/*
 *  globals
 */

static const unsigned int AM335X_GPIO_BASE[] = {
		AM335X_GPIO0_BASE,
		AM335X_GPIO1_BASE,
		AM335X_GPIO2_BASE,
		AM335X_GPIO3_BASE };

static volatile struct am335x_gpio_regs *_regmap[AM335X_GPIO_BANKS] = {0,0,0,0};

static unsigned int _users[AM335X_GPIO_BANKS] = {0,0,0,0};

/*
 * functions
 */

int gpio_init(unsigned int pin, unsigned int direction)
{
	int fd;

	if (BANK(pin) >= AM335X_GPIO_BANKS)
		return -1;

	if (!_regmap[BANK(pin)])
	{	// open register map
		fd = open("/dev/mem", O_RDWR | O_SYNC);
		if (!fd)
			return -1;

		_regmap[BANK(pin)] = (struct am335x_gpio_regs *)mmap(
				NULL,
				sizeof(struct am335x_gpio_regs),
				PROT_READ | PROT_WRITE,
				MAP_SHARED,
				fd,
				AM335X_GPIO_BASE[BANK(pin)]);

		close(fd);
	}
	if (_regmap[BANK(pin)] == MAP_FAILED)
		return -1;

	_users[BANK(pin)]++;

	if (direction > 0)
		// configure pin as output
		_regmap[BANK(pin)]->gpio_oe |= BITMASK(pin);
	else
		// configure pin as input
		_regmap[BANK(pin)]->gpio_oe &= ~BITMASK(pin);

	return 0;
}

int gpio_free(unsigned int pin)
{
	if (BANK(pin) >= AM335X_GPIO_BANKS)
		return -1;

	if (--_users[BANK(pin)] > 0)
		return 0;

	// unmap memory
	return munmap((void *)_regmap[BANK(pin)], sizeof(struct am335x_gpio_regs));
}

int gpio_set(unsigned int pin, unsigned int value)
{
	if (BANK(pin) >= AM335X_GPIO_BANKS)
		return -1;

	if (!_regmap[BANK(pin)] || _regmap[BANK(pin)] == MAP_FAILED)
		return -1;

	if (value > 0)
		_regmap[BANK(pin)]->gpio_dataout |= BITMASK(pin);
	else
		_regmap[BANK(pin)]->gpio_dataout &= ~BITMASK(pin);

	return 0;
}

int gpio_get(unsigned int pin)
{
	if (BANK(pin) >= AM335X_GPIO_BANKS)
		return -1;

	if (!_regmap[BANK(pin)] || _regmap[BANK(pin)] == MAP_FAILED)
		return -1;

	return (_regmap[BANK(pin)]->gpio_datain & BITMASK(pin)) > 0;
}
