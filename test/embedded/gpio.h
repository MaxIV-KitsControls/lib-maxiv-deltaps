#ifndef GPIO_H
#define GPIO_H

#define GPIO_INPUT  0
#define GPIO_OUTPUT 1

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Configure the specified gpio pin as an input or output
 *
 * NOTE: this function must be called prior to using
 * gpio_set() or gpio_get().
 *
 * @param pin			pin number
 * @param direction		0 : input, positive : output
 * @return 				0 on success, -1 on failure
 */
int gpio_init(unsigned int pin, unsigned int direction);

/**
 * release the specified gpio pin and free allocated resources
 *
 * @param pin			pin number
 * @return 				0 on success, -1 on failure
 */
int gpio_free(unsigned int pin);

/**
 * Set the specified gpio pin high or low
 *
 * @param pin			pin number
 * @param value			0 : logic '0', positive : logic '1'
 * @return 				0 on success, -1 on failure
 */
int gpio_set(unsigned int pin, unsigned int value);

/**
 * Read the value of the specified gpio pin
 *
 * @param pin			pin number
 * @return 				0 : logic '0', positive : 'logic 1', -1 : failure
 */
int gpio_get(unsigned int pin);


#ifdef __cplusplus
}
#endif

#endif // GPIO_H
