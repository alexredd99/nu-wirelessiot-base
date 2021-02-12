// BLE Service example app
//
// Creates a BLE environmental sensing service

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "simple_ble.h"

#include "nrf52840dk.h"

// Intervals for advertising and connections
static simple_ble_config_t ble_config = {
  // c0:98:e5:4e:xx:xx
  .platform_id       = 0x4E,    // used as 4th octect in device BLE address
  .device_id         = 0xAABB,
  .adv_name          = "CS397/497", // used in advertisements if there is room
  .adv_interval      = MSEC_TO_UNITS(1000, UNIT_0_625_MS),
  .min_conn_interval = MSEC_TO_UNITS(500, UNIT_1_25_MS),
  .max_conn_interval = MSEC_TO_UNITS(1000, UNIT_1_25_MS),
};

static simple_ble_service_t environmental_sensing_service = {{
  .uuid128 = {0xFB,0x34,0x9B,0x5F,0x80,0x00,0x00,0x80,
              0x00,0x10,0x00,0x00,0x1A,0x18,0x00,0x00}
}};

static simple_ble_char_t Elevation = {.uuid16 = 0x2A6C}; // temperature handle
static simple_ble_char_t Pressure = {.uuid16 = 0x2A6D};
static simple_ble_char_t Temperature = {.uuid16 = 0x2A6E};
static simple_ble_char_t Humidity = {.uuid16 = 0x2A6F};
static simple_ble_char_t Pollen_Concentration = {.uuid16 = 0x2A75};

static int32_t elevation_int = 185 & 0xFFFFFF;  // sint24
static uint32_t pressure_int = 99000;    // change place?
static int16_t temperature_int = -1722; // change place?
static uint16_t humidity_int = 5300; // change place? 
static uint32_t pollen_int =  343 & 0xFFFFFF; // Change place?




/*******************************************************************************
 *   State for this application
 ******************************************************************************/
// Main application state
simple_ble_app_t* simple_ble_app;

void ble_evt_write(ble_evt_t const* p_ble_evt) {
  printf("Got write to a characteristic!\n");
}

int main(void) {

  printf("Board started. Initializing BLE: \n");

  // Setup BLE
  simple_ble_app = simple_ble_init(&ble_config);

  simple_ble_add_service(&environmental_sensing_service); // Init service
  
  simple_ble_add_characteristic(1, 0, 1, 0,   // Elevation handle
      sizeof(elevation_int), (uint8_t*)&elevation_int,  // change sizeof?
      &environmental_sensing_service, &Elevation);
  
   simple_ble_add_characteristic(1, 0, 1, 0,   // Pressure handle
      sizeof(pressure_int), (uint8_t*)&pressure_int,  // change sizeof?
      &environmental_sensing_service, &Pressure);

    simple_ble_add_characteristic(1, 0, 1, 0,   // Temperature handle
      sizeof(temperature_int), (uint8_t*)&temperature_int,  // change sizeof?
      &environmental_sensing_service, &Temperature);

    simple_ble_add_characteristic(1, 0, 1, 0,   // Humidity handle
      sizeof(humidity_int), (uint8_t*)&humidity_int,  // change sizeof?
      &environmental_sensing_service, &Humidity);
       
    simple_ble_add_characteristic(1, 0, 1, 0,   // Pollen handle
      sizeof(pollen_int), (uint8_t*)&pollen_int,  // change sizeof?
      &environmental_sensing_service, &Pollen_Concentration);

  // Start Advertising
  simple_ble_adv_only_name();

  while(1) {
    power_manage();
  }
}

