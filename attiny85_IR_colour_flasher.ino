#include <Arduino.h>
#include <IRremote.hpp>

#define ARRAY_LENGTH(array) (sizeof(array) / (sizeof(array[0])))

const uint16_t IR_led = 1;
const uint16_t button = 2;

const uint16_t PULSE_TIME = 700; // Microseconds
const uint16_t MAX_SIGNAL_LENGTH = 45; // Array Length
const float BLINK_DELAY = 0.7; // Seconds
const float FADE_DELAY = 2; // Seconds
const uint16_t NUMBER_OF_MODES = 5;
const float MODE_SWITCH_TIME_INTERVAL = 0.05; // Seconds

uint16_t flash_mode = 0;
uint16_t running_flash_mode = 0;
uint16_t previous_button_state = 0;

// IR signal colour codes derived from "binary_to_IR_signal.ino"
const uint16_t RED[] PROGMEM = {1400,1400,700,700,700,2100,700,2100,700,2800,700,700,700,700,700,1400,1400,2800,1400,2800,700};
const uint16_t ORANGE[] PROGMEM = {700,700,700,700,1400,1400,1400,2800,700,2100,700,700,700,1400,700,700,700,1400,700,2100,1400,2800,700};
const uint16_t YELLOW[] PROGMEM = {700,700,700,2100,1400,1400,700,2100,700,1400,700,700,700,2800,1400,1400,700,2100,1400,2800,700};
const uint16_t GREEN[] PROGMEM = {1400,1400,700,700,700,700,700,700,700,2100,700,1400,700,1400,700,1400,700,1400,1400,2800,1400,2800,700};
const uint16_t TURQUOISE[] PROGMEM = {700,700,700,700,1400,1400,1400,2800,700,1400,700,700,1400,1400,700,1400,1400,2800,700,700,700,2100,700};
const uint16_t BLUE[] PROGMEM = {700,700,700,2100,1400,700,700,2800,700,1400,700,700,700,1400,1400,700,700,1400,700,700,700,700,700,700,700,2100,700};
const uint16_t MAGENTA[] PROGMEM = {700,700,700,700,1400,1400,1400,2800,700,2100,1400,2100,700,700,700,700,1400,2100,700,700,700,2100,700};
const uint16_t PINK[] PROGMEM = {700,700,700,2100,1400,700,700,2800,700,1400,700,700,700,2800,1400,1400,700,2100,700,700,700,2100,700};
const uint16_t WHITE[] PROGMEM = {1400,1400,700,700,700,700,1400,2800,700,2100,700,700,700,1400,700,1400,1400,2800,700,700,700,2100,700};

const uint16_t RED_FADE[] PROGMEM = {1400,1400,700,700,700,2100,700,2100,700,2800,700,700,700,700,700,1400,1400,2800,1400,2800,700,1400,700,1400,1400,700,700,700,700,700,700,2800,1400,2800,700};
const uint16_t ORANGE_FADE[] PROGMEM = {700,700,700,700,1400,1400,1400,2800,700,2100,700,700,700,1400,700,700,700,1400,700,2100,1400,2800,700,1400,700,1400,1400,700,700,700,700,700,700,2800,1400,2800,700};
const uint16_t YELLOW_FADE[] PROGMEM = {700,700,700,2100,1400,1400,700,2100,700,1400,700,700,700,2800,1400,1400,700,2100,1400,2800,700,1400,700,1400,1400,700,700,700,700,700,700,2800,1400,2800,700};
const uint16_t GREEN_FADE[] PROGMEM = {1400,1400,700,700,700,700,700,700,700,2100,700,1400,700,1400,700,1400,700,1400,1400,2800,1400,2800,700,1400,700,1400,1400,700,700,700,700,700,700,2800,1400,2800,700};
const uint16_t TURQUOISE_FADE[] PROGMEM = {700,700,700,700,1400,1400,1400,2800,700,1400,700,700,1400,1400,700,1400,1400,2800,700,700,700,2100,700,1400,700,1400,1400,700,700,700,700,700,700,2800,1400,2800,700};
const uint16_t BLUE_FADE[] PROGMEM = {700,700,700,2100,1400,700,700,2800,700,1400,700,700,700,1400,1400,700,700,1400,700,700,700,700,700,700,700,2100,700,1400,700,1400,1400,700,700,700,700,700,700,2800,1400,2800,700};
const uint16_t MAGENTA_FADE[] PROGMEM = {700,700,700,700,1400,1400,1400,2800,700,2100,1400,2100,700,700,700,700,1400,2100,700,700,700,2100,700,1400,700,1400,1400,700,700,700,700,700,700,2800,1400,2800,700};
const uint16_t PINK_FADE[] PROGMEM = {700,700,700,2100,1400,700,700,2800,700,1400,700,700,700,2800,1400,1400,700,2100,700,700,700,2100,700,1400,700,1400,1400,700,700,700,700,700,700,2800,1400,2800,700};
const uint16_t WHITE_FADE[] PROGMEM = {1400,1400,700,700,700,700,1400,2800,700,2100,700,700,700,1400,700,1400,1400,2800,700,700,700,2100,700,1400,700,1400,1400,700,700,700,700,700,700,2800,1400,2800,700};

void setup() {
  IrSender.begin(IR_led);
  IrSender.enableIROut(38);
  pinMode(button, INPUT);
}

// Sends corresponding colour flashing pattern based on the flashing mode, can be appended or altered
void loop() {
  switch (flash_mode) {
    case 0:
      //Rainbow Blink
      running_flash_mode = flash_mode;
      send_IR_signal(RED, ARRAY_LENGTH(RED), BLINK_DELAY);
      send_IR_signal(ORANGE, ARRAY_LENGTH(ORANGE), BLINK_DELAY);
      send_IR_signal(YELLOW, ARRAY_LENGTH(YELLOW), BLINK_DELAY);
      send_IR_signal(GREEN, ARRAY_LENGTH(GREEN), BLINK_DELAY);
      send_IR_signal(TURQUOISE, ARRAY_LENGTH(TURQUOISE), BLINK_DELAY);
      send_IR_signal(BLUE, ARRAY_LENGTH(BLUE), BLINK_DELAY);
      send_IR_signal(MAGENTA, ARRAY_LENGTH(MAGENTA), BLINK_DELAY);
      send_IR_signal(PINK, ARRAY_LENGTH(PINK), BLINK_DELAY);
      break;
    case 1:
      //Rainbow Fade
      running_flash_mode = flash_mode;
      send_IR_signal(RED_FADE, ARRAY_LENGTH(RED_FADE), FADE_DELAY);
      send_IR_signal(ORANGE_FADE, ARRAY_LENGTH(ORANGE_FADE), FADE_DELAY);
      send_IR_signal(YELLOW_FADE, ARRAY_LENGTH(YELLOW_FADE), FADE_DELAY);
      send_IR_signal(GREEN_FADE, ARRAY_LENGTH(GREEN_FADE), FADE_DELAY);
      send_IR_signal(TURQUOISE_FADE, ARRAY_LENGTH(TURQUOISE_FADE), FADE_DELAY);
      send_IR_signal(BLUE_FADE, ARRAY_LENGTH(BLUE_FADE), FADE_DELAY);
      send_IR_signal(MAGENTA_FADE, ARRAY_LENGTH(MAGENTA_FADE), FADE_DELAY);
      send_IR_signal(PINK_FADE, ARRAY_LENGTH(PINK_FADE), FADE_DELAY);
      break;
    case 2:
      //Ice
      running_flash_mode = flash_mode;
      send_IR_signal(BLUE_FADE, ARRAY_LENGTH(BLUE_FADE), FADE_DELAY);
      send_IR_signal(TURQUOISE_FADE, ARRAY_LENGTH(TURQUOISE_FADE), FADE_DELAY);
      send_IR_signal(WHITE_FADE, ARRAY_LENGTH(WHITE_FADE), FADE_DELAY);
      send_IR_signal(TURQUOISE_FADE, ARRAY_LENGTH(TURQUOISE_FADE), FADE_DELAY);
      break;
    case 3:
      //Flame
      running_flash_mode = flash_mode;
      send_IR_signal(RED_FADE, ARRAY_LENGTH(RED_FADE), FADE_DELAY);
      send_IR_signal(ORANGE_FADE, ARRAY_LENGTH(ORANGE_FADE), FADE_DELAY);
      send_IR_signal(YELLOW_FADE, ARRAY_LENGTH(YELLOW_FADE), FADE_DELAY);
      send_IR_signal(ORANGE_FADE, ARRAY_LENGTH(ORANGE_FADE), FADE_DELAY);
      break;
    case 4:
      //Sakura
      running_flash_mode = flash_mode;
      send_IR_signal(MAGENTA_FADE, ARRAY_LENGTH(MAGENTA_FADE), FADE_DELAY);
      send_IR_signal(PINK_FADE, ARRAY_LENGTH(PINK_FADE), FADE_DELAY);
      send_IR_signal(WHITE_FADE, ARRAY_LENGTH(WHITE_FADE), FADE_DELAY);
      send_IR_signal(PINK_FADE, ARRAY_LENGTH(PINK_FADE), FADE_DELAY);
      break;
  }

  delay(3);
}

// Checks for any button state changes to advance flashing mode
void check_mode_switch() {
  uint16_t button_state = digitalRead(button);

  if (button_state == 1 && button_state != previous_button_state) {
    flash_mode += 1;
    flash_mode = flash_mode % NUMBER_OF_MODES;
  }

  previous_button_state = button_state;
}

// Sends the infrared signal based on the colour binary code, ensures the modes can be switched during delay times
void send_IR_signal(const uint16_t IR_signal[], int length, float signal_delay) {
  if (flash_mode != running_flash_mode) {
    return;
  }

  uint16_t buffer[MAX_SIGNAL_LENGTH];
  for (int i = 0; i < length; i++) {
    buffer[i] = pgm_read_word(&IR_signal[i]);
  }

  IrSender.sendRaw(buffer, length, 38);

  int total_steps = signal_delay / MODE_SWITCH_TIME_INTERVAL;
  for (uint16_t i = 0; i < total_steps; i++) {
    check_mode_switch();
    delay(MODE_SWITCH_TIME_INTERVAL * 1000);
  }

}
