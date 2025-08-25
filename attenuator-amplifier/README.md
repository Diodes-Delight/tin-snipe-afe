# Attenuator and Amplifier

This project is the attenuator and the amplifier portion of the AFE.
The attenuation is 1000:1, 100:1 and 10:1 while the amplification can be set to 1x, 10x and 100x with the 1x being the buffer
configuration which is used for signals which are either exactly in range or for attenuated signals which should also be buffered
so as to not load the input source too much.

## Building the firmware

The board itself does not contain any microcontrollers, but for testing a TI LaunchPad LP-MSPM0C1106 is used. This board was chosen for its low cost, wide availability from distributors and on board debugger.

For building and running, [Code Composer Studio](https://www.ti.com/tool/CCSTUDIO) and the [LP-MSPM0C1106](https://www.ti.com/tool/LP-MSPM0C1106) are necessary.
To build and run the firmware, simply open this folder in ccstudio and either use the Menu Run->Flash Project or Debug Project. ccstudio automatically detects the plugged in LaunchPad and can debug and flash the system from within the IDE.

The connectors J4 and J5 should be connected with cables in order to the following pins:

J4:

1 - PA25
2 - PA26
3 - PA17
4 - PA6
5 - PA11
6 - PA24
7 - PA4
8 - GND

J5:

1 - read below
2 - 3.3V
2 - unused for now
4 - GND

Warning! connect pin 1 to 5V only after the board has been flashed at least once! This is to prevent it being in an undefined state and potentially damaging the relays.
