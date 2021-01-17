// Toggle buttons and print

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "app_error.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrfx_gpiote.h"
#include "nrf_gpio.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_serial.h"


// Pin definitions
#include "nrf52840dk.h"

int main(void){
    ret_code_t error_code = NRF_SUCCESS;

    // initialize RTT library
    error_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(error_code);
    NRF_LOG_DEFAULT_BACKENDS_INIT();
    printf("Log initialized!\n");

    // Initialize.
    nrf_gpio_cfg_output(LED1);
    nrf_gpio_cfg_output(LED2);
    nrf_gpio_cfg_output(LED3);
    nrf_gpio_cfg_output(LED4);
    nrf_gpio_pin_set(LED1);
    nrf_gpio_pin_set(LED2);
    nrf_gpio_pin_set(LED3);
    nrf_gpio_pin_set(LED4);
    nrf_gpio_cfg_input(BUTTON1, NRF_GPIO_PIN_PULLUP);
    nrf_gpio_cfg_input(BUTTON2, NRF_GPIO_PIN_PULLUP);
    nrf_gpio_cfg_input(BUTTON3, NRF_GPIO_PIN_PULLUP);
    nrf_gpio_cfg_input(BUTTON4, NRF_GPIO_PIN_PULLUP);

    // Enter main loop.
    while (1) {
        // Pressing a button will toggle correspoding LED
        // Board will also log which LED was toggled
        // (200 ms delay used for button debounce)

        if(nrf_gpio_pin_read(BUTTON1) == false){
            nrf_gpio_pin_toggle(LED1);
            printf("Toggled LED 1!\n");
            nrf_delay_ms(200);
        }

        if(nrf_gpio_pin_read(BUTTON2) == false){
            nrf_gpio_pin_toggle(LED2);
            printf("Toggled LED 2!\n");
            nrf_delay_ms(200);
        }

        if(nrf_gpio_pin_read(BUTTON3) == false){
            nrf_gpio_pin_toggle(LED3);
            printf("Toggled LED 3!\n");
            nrf_delay_ms(200);
        }

        if(nrf_gpio_pin_read(BUTTON4) == false){
            nrf_gpio_pin_toggle(LED4);
            printf("Toggled LED 4!\n");
            nrf_delay_ms(200);
        }
    }
}