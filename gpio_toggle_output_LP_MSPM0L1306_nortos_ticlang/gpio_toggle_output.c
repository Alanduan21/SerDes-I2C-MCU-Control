#include <stdio.h>
#include <string.h>
#include "ti_msp_dl_config.h"



/* This results in approximately 0.5s of delay assuming 32MHz CPU_CLK */
#define DELAY (4000000)


void toggle(uint32_t pins){
    DL_GPIO_togglePins(GPIO_LEDS_PORT, pins);
}

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

// toggle the target pin

void test_target(uint32_t pins){
    DL_GPIO_togglePins(GPIO_TEST_PORT,pins);
}


int main(void)
{
    const char debug_message[] = "GPIO Toggled!\r\n";

    /* Power on GPIO, initialize pins as digital outputs */
    SYSCFG_DL_init();

    /* Default: all LEDs and test target GPIOs are OFF */
    DL_GPIO_clearPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_2_PIN |GPIO_LEDS_USER_LED_3_PIN | GPIO_LEDS_USER_LED_1_PIN );
    DL_GPIO_clearPins(GPIO_TEST_PORT, GPIO_TEST_targetPin_PIN);
  


    printf("initialised...\n");
    while (1) 
    {   
        delay_cycles(4000000);
        test_target(GPIO_TEST_targetPin_PIN);

        uart_debug(debug_message);

        toggle(GPIO_LEDS_USER_LED_1_PIN|GPIO_LEDS_USER_LED_2_PIN|GPIO_LEDS_USER_LED_3_PIN);
        
        
    }
}

