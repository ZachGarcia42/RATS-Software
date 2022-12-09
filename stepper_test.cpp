/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pico/stdlib.h"
#include "Stepper.h"

const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution
// for your motor

// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 0, 1, 2, 3);

void setup() {
  // set the speed at 60 rpm:
  myStepper.setSpeed(60);
  // initialize the serial port:
  // Serial.begin(9600);
}

void loop() {
  // step one revolution  in one direction:
  // Serial.println("clockwise");
  myStepper.step(stepsPerRevolution);
  sleep_ms(500);

  // step one revolution in the other direction:
  // Serial.println("counterclockwise");
  myStepper.step(-stepsPerRevolution);
  sleep_ms(500);
}

int main() {
    setup();
    while (true) {
      loop();
    }
}
