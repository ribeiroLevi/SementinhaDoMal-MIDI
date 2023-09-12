#include "MIDIUSB.h"

const byte TOTAL_BUTTONS = 5;
// All the Arduino pins used for buttons, in order.
const byte BUTTONS_PIN[TOTAL_BUTTONS] = {2,3,4,5,6};
// Every pitch corresponding to every Arduino pin. Each note has an associated numeric pitch (frequency scale).
// See https://github.com/arduino/tutorials/blob/master/ArduinoZeroMidi/PitchToNote.h
const byte BUTTONS_PITCH[TOTAL_BUTTONS] = {60,62,64,65,67};
// Current state of the pressed buttons.
byte currentRead[TOTAL_BUTTONS];
// Temporary input reads to check against current state.
byte tempRead;

// The setup function runs once when you press reset or power the board
void setup() {
  // Initialize all the pins as a pull-up input.
  for (byte i = 0; i < TOTAL_BUTTONS; i++) {
    pinMode(BUTTONS_PIN[i], INPUT_PULLUP);
  }
}

// The loop function runs over and over again forever
void loop() {
  for (byte i = 0; i < TOTAL_BUTTONS; i++) {
    // Get the digital state from the button pin.
    // In pull-up inputs the button logic is inverted (HIGH is not pressed, LOW is pressed).
    byte buttonState = digitalRead(BUTTONS_PIN[i]);
    // Temporarily store the digital state.
    tempRead = buttonState;
    // Continue only if the last state is different to the current state.
    if (currentRead[i] != tempRead) {
      // See https://www.arduino.cc/en/pmwiki.php?n=Tutorial/Debounce
      delay(2);
      // Get the pitch mapped to the pressed button.
      byte pitch = BUTTONS_PITCH[i];
      // Save the new input state.
      currentRead[i] = tempRead;
      // Execute note on or noted off depending on the button state.
      if (buttonState == LOW) {
        noteOn(pitch);
      } else {
        noteOff(pitch);
      }
    }
  }
}

void noteOn(byte pitch) {
  MidiUSB.sendMIDI({0x09, 0x90, pitch, 127});
  MidiUSB.flush();
}

void noteOff(byte pitch) {
  MidiUSB.sendMIDI({0x08, 0x80, pitch, 0});
  MidiUSB.flush();
}