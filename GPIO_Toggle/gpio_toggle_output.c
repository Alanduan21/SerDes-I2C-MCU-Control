#include <stdio.h>
#include <string.h>
#include "ti_msp_dl_config.h"


const char debug_message[] = "GPIO Toggled NOW!\r\n";
const char after_debug_msg[] = "Transmission Finished\r\n";

void toggle(uint32_t pins){
    DL_GPIO_togglePins(GPIO_LEDS_PORT, pins);
}


/// COM5
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

int main(void)
{


    /* Power on GPIO, initialize pins as digital outputs */
    SYSCFG_DL_init();

    /* Default: all LEDs and test target GPIOs are OFF */
    DL_GPIO_clearPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN );
  


    printf("initialised...\n");
    while (1) 
    {   
        
        // test_target(GPIO_TEST_targetPin_PIN);

        uart_debug(debug_message);
        // printf("transmit\r\n");
        
        toggle(GPIO_LEDS_USER_LED_1_PIN);
        delay_cycles(80000000);

        toggle(GPIO_LEDS_TESTPIN_PIN);

        uart_debug(after_debug_msg);
        delay_cycles(80000000);
        
        
    }
}

