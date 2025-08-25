# Attenuator and Amplifier

This project is the attenuator and the amplifier portion of the AFE.
The attenuation is 1000:1, 100:1 and 10:1 while the amplification can be set to 1x, 10x and 100x with the 1x being the buffer
configuration which is used for signals which are either exactly in range or for attenuated signals which should also be buffered
so as to not load the input source too much.

## Building the firmware

The board itself does not contain any microcontrollers, but for testing a TI LaunchPad LP-MSPM0C1106 is used. The choice of board is irrelevant,
it's just what the test code has been developed with.
