/*
 * Debounced button for Arduino.
 *
 * Copyright (c) 2021, Kupto
 *
 * This is free software; published under the MIT license.
 * See the LICENSE file.
 */
#include <Arduino.h>
#include "Button.hpp"

void Button::begin() {
  pinMode(_pin, (_pullup) ? INPUT_PULLUP : INPUT);
  _state = (digitalRead(_pin) == ((_invert) ? HIGH : LOW));
  _debouncer.reset();
}

void Button::update() {
  bool s = (digitalRead(_pin) == ((_invert) ? HIGH : LOW));

  if (_state == s)
    return;

  if (s)
    _debouncer.on(); // == pressed == LOW
  else
    _debouncer.off();

  _state = s;
}

void Button::click() {
  // Activates the was{Pressed,Released}() functionality based on
  // current input state.
  _state = (digitalRead(_pin) == ((_invert) ? HIGH : LOW));
  _debouncer.reset();

  if (_state)
    _debouncer.on(); // == pressed == LOW
  else
    _debouncer.off();
}
