#include "ti_msp_dl_config.h"


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


int main(void)
{
    SYSCFG_DL_init();

    /* Set LED to indicate start of transfer */
    DL_GPIO_clearPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);

    uint8_t cmdType0Buffer[] = {0xFF, 0x0};

    SPI_Controller_transmitCommand(cmdType0Buffer, 2);


    /* If write and read were successful, toggle LED */
    while (1) {
        DL_GPIO_clearPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);

        DL_GPIO_setPins(GPIO_EXTRA_PORT, GPIO_EXTRA_R_LATCH_PIN);

        delay_cycles(1000);

        DL_GPIO_clearPins(GPIO_EXTRA_PORT, GPIO_EXTRA_R_LATCH_PIN);

        delay_cycles(12000000);
    }
}
