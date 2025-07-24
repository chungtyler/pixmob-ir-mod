#include <Arduino.h>
#include <IRremote.hpp>

const uint16_t IR_led = 3;
const int button = 2;

const int PULSE_TIME = 700; // Microseconds
const int MAX_SIGNAL_LENGTH = 45; // Array Length
const double BLINK_DELAY = 0.7; // Seconds
const double FADE_DELAY = 2; // Seconds
const int NUMBER_OF_MODES = 5;
const double MODE_SWITCH_TIME_INTERVAL = 0.05; // Seconds

int flash_mode = 0;
int running_flash_mode = 0;
int previous_button_state = 0;

// Binary colour codes based on https://github.com/danielweidman/pixmob-ir-reverse-engineering/blob/main/python_tools/effect_definitions.py
String RED = "110010100010001000010101001100001100001";
String ORANGE = "101011001100001000101001010010001100001";
String YELLOW = "101000110010001001010000110010001100001";
String GREEN = "110010101010001001001001001100001100001";
String TURQUOISE = "101011001100001001011001001100001010001";
String BLUE = "101000110100001001010011010010101010001";
String MAGENTA = "101011001100001000110001010110001010001";
String PINK = "101000110100001001010000110010001010001";
String WHITE = "110010101100001000101001001100001010001";
String FADE = "001001101010100001100001";

struct Signal {
  int IR_signal[MAX_SIGNAL_LENGTH] = {};
  int length = 0;
};

void setup() {
  //Serial.begin(115200);
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
      send_IR_signal(RED, BLINK_DELAY);
      send_IR_signal(ORANGE, BLINK_DELAY);
      send_IR_signal(YELLOW, BLINK_DELAY);
      send_IR_signal(GREEN, BLINK_DELAY);
      send_IR_signal(TURQUOISE, BLINK_DELAY);
      send_IR_signal(BLUE, BLINK_DELAY);
      send_IR_signal(MAGENTA, BLINK_DELAY);
      send_IR_signal(PINK, BLINK_DELAY);
      break;
    case 1:
      //Rainbow Fade
      running_flash_mode = flash_mode;
      send_IR_signal(RED + FADE, FADE_DELAY);
      send_IR_signal(ORANGE + FADE, FADE_DELAY);
      send_IR_signal(YELLOW + FADE, FADE_DELAY);
      send_IR_signal(GREEN + FADE, FADE_DELAY);
      send_IR_signal(TURQUOISE + FADE, FADE_DELAY);
      send_IR_signal(BLUE + FADE, FADE_DELAY);
      send_IR_signal(MAGENTA + FADE, FADE_DELAY);
      send_IR_signal(PINK + FADE, FADE_DELAY);
      break;
    case 2:
      //Ice
      running_flash_mode = flash_mode;
      send_IR_signal(BLUE + FADE, FADE_DELAY);
      send_IR_signal(TURQUOISE + FADE, FADE_DELAY);
      send_IR_signal(WHITE + FADE, FADE_DELAY);
      send_IR_signal(TURQUOISE + FADE, FADE_DELAY);
      break;
    case 3:
      //Flame
      running_flash_mode = flash_mode;
      send_IR_signal(RED + FADE, FADE_DELAY);
      send_IR_signal(ORANGE + FADE, FADE_DELAY);
      send_IR_signal(YELLOW + FADE, FADE_DELAY);
      send_IR_signal(ORANGE + FADE, FADE_DELAY);
      break;
    case 4:
      //Sakura
      running_flash_mode = flash_mode;
      send_IR_signal(MAGENTA + FADE, FADE_DELAY);
      send_IR_signal(PINK + FADE, FADE_DELAY);
      send_IR_signal(WHITE + FADE, FADE_DELAY);
      send_IR_signal(PINK + FADE, FADE_DELAY);
      break;
  }

  delay(3);
}

// Checks for any button state changes to advance flashing mode
void check_mode_switch() {
  int button_state = digitalRead(button);

  if (button_state == 1 && button_state != previous_button_state) {
    flash_mode += 1;
    flash_mode = flash_mode % NUMBER_OF_MODES;
  }

  previous_button_state = button_state;
}

// Sends the infrared signal based on the colour binary code, ensures the modes can be switched during delay times
void send_IR_signal(String binary_signal, double signal_delay) {
  if (flash_mode != running_flash_mode) {
    return;
  }

  Signal IR_signal_data = binary_to_IR_signal(binary_signal);

  int resized_IR_signal[IR_signal_data.length] = {};
  for (int i = 0; i < IR_signal_data.length; i++) {
    resized_IR_signal[i] = IR_signal_data.IR_signal[i];
  }

  IrSender.sendRaw(resized_IR_signal, IR_signal_data.length, 38);

  int total_steps = signal_delay / MODE_SWITCH_TIME_INTERVAL;
  for (int i = 0; i < total_steps; i++) {
    check_mode_switch();
    delay(MODE_SWITCH_TIME_INTERVAL * 1000);
  }

}

// Converts the binary string for each colour to the IR signal pulse
Signal binary_to_IR_signal(String binary_signal) {
  Signal IR_signal_data;
  int index = 0;

  for (int i = 0; i < binary_signal.length(); i++) {
    IR_signal_data.IR_signal[index] += 1;

    String current_bit = binary_signal.substring(i, i + 1);
    String next_bit = binary_signal.substring(i + 1, i + 2);

    if (next_bit != current_bit) {
      IR_signal_data.IR_signal[index] = IR_signal_data.IR_signal[index] * PULSE_TIME;
      index += 1;
    }
  }
  IR_signal_data.length = index;
  return IR_signal_data;
}

// Print IR signal array
// void print_signal(Signal IR_signal_data) {
//   Serial.print("Signal: ");
//   Serial.print("[");
//   for (int i = 0; i < IR_signal_data.length; i++) {
//     Serial.print(IR_signal_data.IR_signal[i]);
//     Serial.print(",");
//   }
//   Serial.println("]");
// }
