
/*
	Basic test program that enables all the power rails
*/
#include <stdio.h>

#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>

#define LED0_NODE DT_ALIAS(led0)

static const struct gpio_dt_spec ldo_enable = GPIO_DT_SPEC_GET(DT_ALIAS(ldo_enable), gpios);
static const struct gpio_dt_spec pos_enable = GPIO_DT_SPEC_GET(DT_ALIAS(pos_enable), gpios);
static const struct gpio_dt_spec neg_enable = GPIO_DT_SPEC_GET(DT_ALIAS(neg_enable), gpios);

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

int main(void)
{
	int err;
	bool led_state = true;

	if (!gpio_is_ready_dt(&ldo_enable)) {
		printf("The load switch pin GPIO port is not ready.\n");
		return 0;
	}

	if (!gpio_is_ready_dt(&led)) {
		return 0;
	}

	err = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (err < 0) {
		return 0;
	}

	printf("Initializing pin with inactive level.\n");

	err += gpio_pin_configure_dt(&ldo_enable, GPIO_OUTPUT_INACTIVE);
	if (err != 0) {
		printf("Configuring LDO enable pin failed: %d\n", err);
		return 0;
	}
	err = gpio_pin_configure_dt(&pos_enable, GPIO_OUTPUT_INACTIVE);
	if (err != 0) {
		printf("Configuring POS enable pin failed: %d\n", err);
		return 0;
	}
	err = gpio_pin_configure_dt(&neg_enable, GPIO_OUTPUT_INACTIVE);
	if (err != 0) {
		printf("Configuring NEG enable pin failed: %d\n", err);
		return 0;
	}

	printf("Waiting one second.\n");

	k_sleep(K_MSEC(1000));

	printf("Setting pin to active level.\n");

	err = gpio_pin_set_dt(&pos_enable, 1);
	if (err != 0) {
		printf("Setting POS pin level failed: %d\n", err);
	}
	err = gpio_pin_set_dt(&neg_enable, 1);
	if (err != 0) {
		printf("Setting NEG pin level failed: %d\n", err);
	}

	k_sleep(K_MSEC(100));

	err = gpio_pin_set_dt(&ldo_enable, 1);
	if (err != 0) {
		printf("Setting LDO pin level failed: %d\n", err);
	}

	while (1) {
		err = gpio_pin_toggle_dt(&led);
		if (err < 0) {
			return 0;
		}

		led_state = !led_state;
		printf("LED state: %s\n", led_state ? "ON" : "OFF");
		k_msleep(1000);
	}
	return 0;
}
