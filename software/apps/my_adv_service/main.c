// BLE Service example app
//
// Creates a BLE service and blinks an LED

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

static simple_ble_service_t test_service = {{
  .uuid128 = {0x70,0x6C,0x98,0x41,0xCE,0x43,0x14,0xA9,
              0xB5,0x4D,0x22,0x2B,0x88,0x10,0xE6,0x32}
}};                           //     --  --

static simple_ble_char_t button_state_char = {.uuid16 = 0x1089}; // LED handle
static simple_ble_char_t led_control_char = {.uuid16 = 0x108A}; // Handle 2 
static simple_ble_char_t print_control_char = {.uuid16 = 0x108B}; // Handle 3

static uint8_t button_state = 0;
static bool led_blink = false;
static bool print_bool = false;

static bool led_toggle_bool = false;
static uint8_t curr_print = 0;

/*******************************************************************************
 *   State for this application
 ******************************************************************************/
// Main application state


// Story switch
static void darth_plageous(uint8_t curr_line){
  switch (curr_line) {
    case 0:
      printf("Have you ever heard of the tragedy of Darth Plageous the Wise?\n");
      printf("(Write TRUE to cont.)\n");
      break;
    case 1:
      printf("I thought not, its not a story the Jedi would tell you, its a Sith legend.\n");
      printf("(Write TRUE to cont.)\n");
      break;
    case 2:
      printf("In my time training in the Jedi temple, there was much about the Force that I was forbidden from learning, and many stories they would not tell me.\n");
      printf("(Write TRUE to cont.)\n");
      break;
    case 3:
      printf("Unlike other padawans there, I excelled in mastery and understanding of the deeper mysteries of the Force.\n");
      printf("None of my classmates could command its powers as well as I, nor could they become so deep in meditation as to see its flow and movements in the universe.\n");
      printf("(Write TRUE to cont.)\n");
      break;
    case 4:
      printf("Prodigy, they called me. They said that if I became a master, that I could rival even Yoda in skill an understanding of the Force.\n");
      printf("(Write TRUE to cont.)\n");
      break;
    case 5:
      printf("But I didn’t master the Force, for there was a whole half of its essence that was denied from me while I was in the Jedi temple.\n");
      printf("(Write TRUE to cont.)\n");
      break;
    case 6:
      printf("See, before I was brought to the temple when was not two years old, I was in the hands of a Sith Mistress, who had survived the great Sith War and had me as her only child.\n");
      printf("The Jedi hunted her down and slayed before my young eyes, and brought me to the temple.\n");
      printf("(Write TRUE to cont.)\n");
      break;
    case 7:
      printf("Though I do not remember any of this, I still have at best mixed feelings toward the Jedi for killing my mother.\n");
      printf("(Write TRUE to cont.)\n");
      break;
    case 8:
      printf("Because of my past, I had already had exposure to the Dark Side of the Force, and in the temple, it stood ever elusively, always just out of reach, and always tempting.\n");
      printf("(Write TRUE to cont.)\n");
      break;
    case 9:
      printf("Though I was told that resisting such urges was the mark of a Jedi, I began to question their beliefs. Why couldn’t the dark and the light exist together?\n");
      printf("This question would haunt me and would spark my leaving the Jedi.\n");
      printf("(Write TRUE to cont.)\n");
      break;
    case 10:
      printf("I was told that such questions were dangerous, and that I was headed down a dark and dangerous path, from which there would be no return if I continued.\n");
      printf("But despite their warnings, I still delved to understand the Dark Side.\n");
      printf("(Write TRUE to cont.)\n");
      break;
    case 11:
      printf("Those were the stories the Jedi wouldn’t tell me. The nature of the Dark Side, and what made so evil. All I was told was that I would learn in due time when I was ready,\n");
      printf("and for now all I had to was to search my feelings, and understand the goodness of the Light, and that the Dark Side was simply the Opposite.\n");
      printf("(Write TRUE to cont.)\n");
      break;
    case 12:
      printf("Though I saw through the lies of the Jedi, and now fully understand the Force. The Dark and the Light can exist together, and cooperate and compliment each other.\n");
      printf("I have accomplished more in my private studies away from the Jedi than any Jedi ever will.\n");
      printf("(Write TRUE to cont.)\n");
      break;
    case 13:
      printf("Those are the stories that the Jedi never told me, instead replaced by lies. Even my past, they kept from, simply saying I was brought to the temple at birth,\n");
      printf("like all other padawans, I had to find my mother\'s holocron to find the truth.\n");
      printf("(Write TRUE to cont.)\n");
      break;
    case 14:
      printf("Do not except the Jedi at face value, they are not all what they seem to be. And know that theirs, is not the only Way of the Force.\n");
      break;
    case 15:
      printf("Perhaps a story for another time...\n");
      break;
  }
}

simple_ble_app_t* simple_ble_app;

void ble_evt_write(ble_evt_t const* p_ble_evt) {
  
  if(simple_ble_is_char_event(p_ble_evt, &led_control_char)){  // Toggle blinking LED
    if(led_blink != 0){
      printf("Toggle LED blink ON\n");
      led_toggle_bool = true;
    } else {
      printf("Toggle LED blink OFF\n");
      led_toggle_bool = false;
    }
  }

  if(simple_ble_is_char_event(p_ble_evt, &print_control_char)){  // Toggle story
    if(print_bool != 0){ // If true, continue story
      darth_plageous(curr_print);
      curr_print++;
      if(curr_print > 14) curr_print = 0; // If end of story, reset
    } else{
      darth_plageous(15); // If false, end and restart story
      curr_print = 0;
    }
  } 
}

int main(void) {

  printf("Board started. Initializing BLE: \n");
  
  // Setup button GPIO

  nrf_gpio_cfg_input(BUTTON1, NRF_GPIO_PIN_PULLUP);
  nrf_gpio_cfg_input(BUTTON2, NRF_GPIO_PIN_PULLUP);
  nrf_gpio_cfg_input(BUTTON3, NRF_GPIO_PIN_PULLUP);
  nrf_gpio_cfg_input(BUTTON4, NRF_GPIO_PIN_PULLUP);

  // Setup LED GPIO
  nrf_gpio_cfg_output(LED1);

  // Setup BLE
  simple_ble_app = simple_ble_init(&ble_config);

  simple_ble_add_service(&test_service);

  simple_ble_add_characteristic(1, 0, 1, 0,   // FIRST HANDLE / CHARACTERISTIC
      sizeof(button_state), (uint8_t*)&button_state,
      &test_service, &button_state_char);

  simple_ble_add_characteristic(1, 1, 0, 0,   // SECOND HANDLE / CHARACTERISTIC
      sizeof(led_blink), (uint8_t*)&led_blink,
      &test_service, &led_control_char);

  simple_ble_add_characteristic(1, 1, 0, 0,   // THIRD HANDLE / CHARACTERISTIC
      sizeof(print_bool), (uint8_t*)&print_bool,
      &test_service, &print_control_char);


  // Start Advertising
  simple_ble_adv_only_name();

  // Init story
  printf("Write TRUE to 3rd characteristic for story\n");

  while(1) {
      
        // Button characteristic

        if(nrf_gpio_pin_read(BUTTON1) == false){
            printf("Toggled button 1!\n");
            button_state = 1;
            nrf_delay_ms(200);
            simple_ble_notify_char(&button_state_char);
        }

        if(nrf_gpio_pin_read(BUTTON2) == false){
            printf("Toggled button 2!\n");
            button_state = 2;
            nrf_delay_ms(200);
            simple_ble_notify_char(&button_state_char);
        }

        if(nrf_gpio_pin_read(BUTTON3) == false){
            printf("Toggled button 3!\n");
            button_state = 3;
            nrf_delay_ms(200);
            simple_ble_notify_char(&button_state_char);
        }

        if(nrf_gpio_pin_read(BUTTON4) == false){
            printf("Toggled button 4!\n");
            button_state = 4;
            nrf_delay_ms(200);
            simple_ble_notify_char(&button_state_char);
        }

        // End

        // LED blink toggle characteristic
        if(led_toggle_bool){
          nrf_gpio_pin_toggle(LED1);
          nrf_delay_ms(500);
        }

        // End
  }
}

