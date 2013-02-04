#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdint.h>

#include "gpio.h"

/*
 * definitions
 *
 * reference: BCM2835 ARM Peripherals
 * http://www.raspberrypi.org/wp-content/uploads/2012/02/BCM2835-ARM-Peripherals.pdf
 */

/* base addresses */
#define BCM2835_PERI_BASE   0x20000000
#define BCM2835_GPIO_BASE   (BCM2835_PERI_BASE + 0x200000)

#define BCM2835_RESERVE(from, to) uint8_t reserved_ ## to [to - from]

/* gpio controller register map */
struct bcm2835_gpio_regs {
	uint32_t gpfsel[6];    /* 0x0000 GPIO Function Select Registers */
	BCM2835_RESERVE(0x0018, 0x001C);
	uint32_t gpset[2];     /* 0x001C GPIO Pin Output Set Registers */
	BCM2835_RESERVE(0x0024, 0x0028);
	uint32_t gpclr[2];     /* 0x0028 GPIO Pin Output Clear Registers */
	BCM2835_RESERVE(0x0030, 0x0034);
	uint32_t gplev[2];     /* 0x0034 GPIO Pin Level Registers */
	BCM2835_RESERVE(0x003C, 0x0040);
	uint32_t gpeds[2];     /* 0x0040 GPIO Pin Event Detect Status Registers */
	BCM2835_RESERVE(0x0048, 0x004C);
	uint32_t gpren[2];     /* 0x004C GPIO Pin Rising Edge Detect Enable Registers */
	BCM2835_RESERVE(0x0054, 0x0058);
	uint32_t gpfen[2];     /* 0x0058 GPIO Pin Falling Edge Detect Enable Registers */
	BCM2835_RESERVE(0x0060, 0x0064);
	uint32_t gphen[2];     /* 0x0064 GPIO Pin High Detect Enable Registers */
	BCM2835_RESERVE(0x006C, 0x0070);
	uint32_t gplen[2];     /* 0x0070 GPIO Pin Low Detect Enable Registers */
	BCM2835_RESERVE(0x0078, 0x007C);
	uint32_t gparen[2];    /* 0x007C GPIO Pin Async. Rising Edge Detect Registers */
	BCM2835_RESERVE(0x0084, 0x0088);
	uint32_t gpafen[2];    /* 0x0088 GPIO Pin Async. Falling Edge Detect Registers */
	BCM2835_RESERVE(0x0090, 0x0094);
	uint32_t gpud;         /* 0x0094 GPIO Pin Pull-up/down Enable Register */
	uint32_t gppudclk[2];  /* 0x0098 GPIO Pin Pull-up/down Enable Clock Registers */
	BCM2835_RESERVE(0x00A0, 0x00B4);
};

/*
 * globals
 */

static volatile struct bcm2835_gpio_regs *_regmap = 0;
static int _users = 0;

/*
 * functions
 */

int gpio_init(unsigned int pin, unsigned int direction)
{
	int fd;
	uint32_t mask, shift, inout;
	volatile uint32_t *reg;

	if (!_regmap)
	{	/* open register map */
		fd = open("/dev/mem", O_RDWR | O_SYNC);
		if (!fd)
			return -1;

		_regmap = (struct bcm2835_gpio_regs *)mmap(
				NULL,
				sizeof(struct bcm2835_gpio_regs),
				PROT_READ | PROT_WRITE,
				MAP_SHARED,
				fd,
				BCM2835_GPIO_BASE);

		close(fd);
	}
	if (_regmap == MAP_FAILED)
		return -1;

	_users++;

	/* there are 6 function select registers.
	 * each register controls 10 pins, 3 bits per pin:
	 * 000 = gpio input
	 * 001 = gpio output
	 * 100 = alternate function 0
	 * 101 = alternate function 1
	 * 110 = alternate function 2
	 * 111 = alternate function 3
	 * 011 = alternate function 4
	 * 010 = alternate function 5
	 */
	reg   = &(_regmap->gpfsel[pin / 10]);
	shift = 3*(pin % 10);
	mask  = 0x7;
	inout = direction > 0;

	*reg = (*reg & ~(mask << shift)) | (inout << shift);

	return 0;
}

int gpio_free(unsigned int pin)
{
	if (--_users > 0)
		return 0;

	return munmap((void *)_regmap, sizeof(struct bcm2835_gpio_regs));
}

int gpio_set(unsigned int pin, unsigned int value)
{
	volatile uint32_t *reg;

	if (!_regmap || _regmap == MAP_FAILED)
		return -1;

	if (pin > 53)
		return -1;

	/* there are two sets of GPSET/GPCLR registers
	 * one for pins 0-31 and one for pins 32-53
	 */
	if (value > 0)
		reg = &(_regmap->gpset[pin / 32]);
	else
		reg = &(_regmap->gpclr[pin / 32]);

	/* registers are write only. zeros are ignored */
	*reg = 1 << (pin % 32);
	return 0;
}

int gpio_get(unsigned int pin)
{
	volatile uint32_t *reg;

	if (!_regmap || _regmap == MAP_FAILED)
		return -1;

	if (pin > 53)
		return -1;

	/* there are two GPLEV registers
	 * one for pins 0-31 and one for pins 32-53
	 */
	reg = &(_regmap->gplev[pin / 32]);

	return (*reg & (1 << (pin % 32))) > 0;
}
