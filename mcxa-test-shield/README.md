# FRDM-MCXA156 Test Shield

This is a mostly Arduino header compatible shield for testing the ADC of the MCXA156 and the power supplies for the OpAmps.

## Build

The programs require a working installation of the Zephyr toolchain, see the Zehpyr [Getting Started Guide](https://docs.zephyrproject.org/latest/develop/getting_started/index.html) for how to accomplish that.

With a working setup, the application can be built and flashed as follows:

```sh
# build
west build -d ../build
west flash -d ../build

# to debug
west debug -d ../build

```

## Bringup

If the program successfully flashes and the LED on the board is blinking, the LDO output voltage can be checked on the J201 pins.
The center pin is ground and the outer pins are positive and negative voltages respectively. Between the pins there should be 30V.
