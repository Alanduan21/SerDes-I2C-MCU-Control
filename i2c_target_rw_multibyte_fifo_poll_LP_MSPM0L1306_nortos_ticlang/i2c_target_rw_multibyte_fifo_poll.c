#include "ti_msp_dl_config.h"
#include <stdio.h>
#include <string.h>

// expect only 2 bytes: [command, value]
#define I2C_RX_PACKET_SIZE (2)

/* Data received from Controller during a Write transfer */
uint8_t gRxPacket[I2C_RX_PACKET_SIZE];

/// UART Debug
void uart_debug(const char *str){
    while(*str)
    {
        DL_UART_transmitData(UART_0_INST,*str++);
        /// returns true when the transmit FIFO is empty, meaning all data has been sent or moved to the shift register.
        while (!DL_UART_isTXFIFOEmpty(UART_0_INST)) 
        {
              // Wait
        }
    }
}


int main(void)
{
    SYSCFG_DL_init();

    DL_GPIO_clearPins(GPIO_TEST_PORT, GPIO_TEST_targetpin_PIN);
    while(1)
    {
        uint8_t i = 0;
        while (!DL_I2C_isTargetRXFIFOEmpty(I2C_INST) && i < sizeof(gRxPacket)) {
            gRxPacket[i++] = DL_I2C_receiveTargetData(I2C_INST);
     }
    }

 /// interpret command
 if(gRxPacket[0] == 0x01){
    if(gRxPacket[1] == 0x01){
        DL_GPIO_setPins(GPIO_TEST_PORT,GPIO_TEST_targetpin_PIN);
        uart_debug("GPIO set HIGH!\r\n");
    }else if(gRxPacket[1] == 0x00){
        DL_GPIO_clearPins(GPIO_TEST_PORT,GPIO_TEST_targetpin_PIN);
        uart_debug("GPIO set LOW!\r\n");
    }else{
        uart_debug("unknown GPIO command\r\n");
    }
 }
 
    delay_cycles(100000);
}
