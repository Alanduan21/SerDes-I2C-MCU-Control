#include "ti_msp_dl_config.h"
#include <stdio.h>
#include <string.h>

#define I2C_TX_PACKET_SIZE (5)

#define I2C_RX_PACKET_SIZE (8)

/* Data sent to Controller in response to Read transfer */
uint8_t gTxPacket[I2C_TX_PACKET_SIZE] = {'M', 'S', 'P', 'M', '0'};

/* Data received from Controller during a Write transfer */
uint8_t gRxPacket[I2C_RX_PACKET_SIZE];

///////////////////////////////////////////////////////
void uart_debug(const char *str){
    while(*str)
    {
        DL_UART_transmitData(UART_0_INST,*str++);
        /// returns true when the transmit FIFO is empty, meaning all data has been sent or moved to the shift register.
        while (!DL_UART_isTXFIFOEmpty(UART_0_INST)) 
        {
              // Wait }
        }
    }
}
///////////////////////////////////////////////////////

int main(void)
{
    SYSCFG_DL_init();

    /// initial debug printf
    ///////////////////////////////////////////////////////
    uart_debug("Target is ready!\r\n");

    ///////////////////////////////////////////////////////


    /* Set LED to indicate start of transfer */
    DL_GPIO_setPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);

    
    //only fills the buffer and the Target device will send this data when
    //requested by the Controller.
    DL_I2C_fillTargetTXFIFO(I2C_INST, &gTxPacket[0], I2C_TX_PACKET_SIZE);

    /*
     * Wait to receive data from the Controller.
     * This loop expects I2C_RX_PACKET_SIZE bytes.
     */
    for (uint8_t i = 0; i < I2C_RX_PACKET_SIZE; i++) {
        while (DL_I2C_isTargetRXFIFOEmpty(I2C_INST))
            ;
        gRxPacket[i] = DL_I2C_receiveTargetData(I2C_INST);
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    switch(gRxPacket[0]){
        case 0x00:
        
            uart_debug("GET 0\r\n");
            // DL_GPIO_togglePins(GPIO_TESTS_PORT,GPIO_TESTS_TESTPIN_PIN);
            break;
        case 0x01:
            uart_debug("GET 1\r\n");
            break;
        case 0x02:
            uart_debug("GET 2\r\n");
            break;

        default: 
            uart_debug("unknown command\r\n");
            break;
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////


    /*
     * Wait until all bytes written to TX FIFO are sent after a successful
     * request.
     */
    while (!DL_I2C_isTargetTXFIFOEmpty(I2C_INST))
        ;

    /* If write and read were successful, toggle LED */
    while (1) {
        DL_GPIO_togglePins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
        uart_debug("received\r\n");
        delay_cycles(16000000);
    }
}
