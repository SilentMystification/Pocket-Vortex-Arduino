/*
This code is heavily based off the code used in this tutorial https://sdvx-diy.github.io/

It's major changes involve redoing the interrupt based knob handling to match the djdao knob fix 
logic included on my github but added onto full controller functionality with button handling

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public 
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "./src/Encoder/Encoder.h"
#include <Mouse.h>
#include <Keyboard.h>

/* Encoder */
#define ENC_LEFT_PIN_A 3
#define ENC_LEFT_PIN_B 2
#define ENC_RIGHT_PIN_A 1
#define ENC_RIGHT_PIN_B 0
#define PULSECOUNT 24
#define MOUSE_MULTIPLIER 3

/* Encoder */
Encoder encLeft(PULSECOUNT, ENC_LEFT_PIN_B, ENC_LEFT_PIN_A);
Encoder encRight(PULSECOUNT, ENC_RIGHT_PIN_B, ENC_RIGHT_PIN_A);


// Button Keybinds
#define BIND_A    'a'
#define BIND_B    'b'
#define BIND_C    'c'
#define BIND_D    'd'
#define BIND_FX_L 'l'
#define BIND_FX_R 'r'
#define BIND_ST   's'

// Button Pinout
#define BT_A  A3
#define BT_B  A1
#define BT_C  A0
#define BT_D  15
#define FX_L  A2
#define FX_R  14
#define BT_ST 10


uint8_t bindKeys[] = { BIND_A, BIND_B, BIND_C, BIND_D, BIND_FX_L, BIND_FX_R, BIND_ST };
uint8_t buttonPins[] = { BT_A, BT_B, BT_C, BT_D, FX_L, FX_R, BT_ST };
uint8_t buttonCount = sizeof(buttonPins) / sizeof(buttonPins[0]);



/* Startup Loop */
void setup() {
  Mouse.begin();
  Keyboard.begin();
  
  encLeft.updateState();
  encRight.updateState();
  
  attachInterrupt(digitalPinToInterrupt(ENC_LEFT_PIN_A), onLeftEncoderTurns, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_LEFT_PIN_B), onLeftEncoderTurns, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_RIGHT_PIN_A), onRightEncoderTurns, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_RIGHT_PIN_B), onRightEncoderTurns, CHANGE);

    for (int i = 0; i < buttonCount; i++) {
      pinMode(buttonPins[i], INPUT_PULLUP);
  }
} 

/* Main Loop */
void loop() {
  signed char mouseX = 0, mouseY = 0;
  mouseX = encLeft.getDelta() * MOUSE_MULTIPLIER;
  mouseY = encRight.getDelta() * MOUSE_MULTIPLIER;
  Mouse.move(mouseX, mouseY, 0);
  updateButtons();
}

void updateButtons()
{
 // Read the buttons, if it's low, output a keyboard press  
  for (int i = 0; i < buttonCount; i++) {
    if (digitalRead(buttonPins[i]) == LOW) {
      Keyboard.press(bindKeys[i]);
    } else {
      Keyboard.release(bindKeys[i]);
    }
  }  
}


void onLeftEncoderTurns()
{
  bool dir = encLeft.updateEncoder();
}

void onRightEncoderTurns()
{
  bool dir = encRight.updateEncoder();
}
