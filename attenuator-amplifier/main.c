#include "ti_msp_dl_config.h"


uint8_t gCurrentSequence = 0;
uint8_t gIntTriggered = 0;

extern volatile uint32_t interruptVectors[];


void SPI_Controller_transmitCommand(uint8_t *cmd, uint8_t cmdLength)
{
    /* SPI must be idle prior to setting a new value of CDMODE */
    while (DL_SPI_isBusy(SPI_0_INST))
        ;

    DL_SPI_setControllerCommandDataModeConfig(SPI_0_INST, cmdLength);

    int i = 0;
    for (i = 0; i < cmdLength; i++) {
        while (DL_SPI_isBusy(SPI_0_INST))
            ;
        DL_SPI_transmitData8(SPI_0_INST, cmd[i]);
    }
}

void set_shift_register_state(uint8_t command[2]) {
    // byte order: second register, first register
    SPI_Controller_transmitCommand(command, 2);

    // latch into output
    delay_cycles(2000);

    DL_GPIO_setPins(GPIO_EXTRA_PORT, GPIO_EXTRA_R_LATCH_PIN);

    delay_cycles(2000);

    DL_GPIO_clearPins(GPIO_EXTRA_PORT, GPIO_EXTRA_R_LATCH_PIN);
}

void cycle_relays() 
{
    const uint32_t relay_actuation_time = 80000; // 3ms / 42ns (24 MHz) rounded up
    /*
        In order of MSB first:
        b0, b1: gain selection
        b2: R3+S
        b3: R3+R
        b4: R1+R
        b5: R1+S
        b6: R2+R
        b7: R2+S

        Sequence: 
        1. R1+R,   R2+S, R3+R     for 1000:1
        2. (R1+R), R2+R, R3+S     for 100:1
        3. (R1+R), R2+S, (R3+S)   for 10:1
        4. R1+S,   GS_A=0, GS_B=1 for 1:1
        5. (R1+S), GS_A=1, GS_B=0 for 1:10
        6. (R1+S), GS_A=1, GS_B=1 for 1:100
    */
    if (gCurrentSequence == 0) {
        set_shift_register_state((uint8_t[]){0b11000000, 0b01011001});
        delay_cycles(relay_actuation_time);
        set_shift_register_state((uint8_t[]){0b11000000, 0b01000000});
        gCurrentSequence = 1;
    } else if (gCurrentSequence == 1) {
        set_shift_register_state((uint8_t[]){0b11000000, 0b01100010});
        delay_cycles(relay_actuation_time);
        set_shift_register_state((uint8_t[]){0b11000000, 0b01000000});
        gCurrentSequence = 2;
    } else if (gCurrentSequence == 2) {
        set_shift_register_state((uint8_t[]){0b11000000, 0b01000001});
        delay_cycles(relay_actuation_time);
        set_shift_register_state((uint8_t[]){0b11000000, 0b01000000});
        gCurrentSequence = 3;
    } else if (gCurrentSequence == 3) {
        set_shift_register_state((uint8_t[]){0b11000000, 0b01000100});
        delay_cycles(relay_actuation_time);
        set_shift_register_state((uint8_t[]){0b11000000, 0b01000000});
        gCurrentSequence = 4;
    } else if (gCurrentSequence == 4) {
        set_shift_register_state((uint8_t[]){0b11000000, 0b10000000});
        gCurrentSequence = 5;
    }  else if (gCurrentSequence == 5) {
        set_shift_register_state((uint8_t[]){0b11000000, 0b11000000});
        gCurrentSequence = 0;
    }
}

int main(void)
{
    SYSCFG_DL_init();

    NVIC_EnableIRQ(GPIO_SWITCHES_INT_IRQN);

    cycle_relays();

    while (1) {
        __WFI();
        if (gIntTriggered == 1) {
            cycle_relays();
            gIntTriggered = 0;

            for (int i=0; i <= gCurrentSequence; i++) {
                DL_GPIO_setPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
                delay_cycles(12000000);
                DL_GPIO_clearPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
            }
        }
    }
}

void GPIOA_IRQHandler(void)
{
    /* If SW is high, turn the LED off */
    if (DL_GPIO_readPins(
            GPIO_SWITCHES_PORT, GPIO_SWITCHES_USER_SWITCH_1_PIN)) {
        //DL_GPIO_setPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
        __NOP();
    }
    /* Otherwise, turn the LED on */
    else {
        //DL_GPIO_clearPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
        gIntTriggered = 1;
    }
}
