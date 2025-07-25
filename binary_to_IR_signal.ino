#include <Arduino.h>

const uint16_t PULSE_TIME = 700; // Microseconds
const uint16_t MAX_SIGNAL_LENGTH = 45; // Max Array Length

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

String colours[] = {RED, ORANGE, YELLOW, GREEN, TURQUOISE, BLUE, MAGENTA, PINK, WHITE, FADE};

void setup() {
  Serial.begin(9600);
  delay(1000);
  uint16_t length = sizeof(colours) / sizeof(colours[0]);
  for (int i = 0; i < length; i++) {
    Serial.print("Colour ");
    Serial.print(i);
    Serial.print(": ");
    binary_to_IR_signal(colours[i]);
  }
}

void loop() {
}

// Converts the binary string for each colour to the IR signal pulse
void binary_to_IR_signal(String binary_signal) {
  uint16_t IR_signal[MAX_SIGNAL_LENGTH] = {};
  uint16_t index = 0;

  // Clusters the 1's and 0's into quantity which is than multiplied by the pulse time
  for (uint16_t i = 0; i < binary_signal.length(); i++) {
    IR_signal[index] += 1;

    String current_bit = binary_signal.substring(i, i + 1);
    String next_bit = binary_signal.substring(i + 1, i + 2);

    if (next_bit != current_bit) {
      IR_signal[index] = IR_signal[index] * PULSE_TIME;
      index += 1;
    }
  }
  
  // Resized array for readability
  uint16_t resized_IR_signal[index] = {};
  for (uint16_t i = 0; i < index; i++) {
    resized_IR_signal[i] = IR_signal[i];
  }

  print_array(resized_IR_signal, index);
}

// Print IR signal array
void print_array(uint16_t IR_signal[], int length) {
  Serial.print("{");
  for (int i = 0; i < length; i++) {
    Serial.print(IR_signal[i]);
    
    if (i == length - 1) {
      break;
    }

    Serial.print(",");
  }
  Serial.println("}");
}
