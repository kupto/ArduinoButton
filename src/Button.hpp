/*
 * Debounced button for Arduino.
 *
 * Copyright (c) 2021, Kupto
 *
 * Button (or switch) is an abstraction over a digital input pin, which allows
 * to view such a physical pin in one of two states: pressed or released. When
 * the input voltage is LOW (tied to ground) the button is considered pressed,
 * and vice versa. Note that this interpretation may be inverted in code.
 *
 * This is free software; published under the MIT license.
 * See the LICENSE file.
 */
#include <Arduino.h>
#include <inttypes.h>
#include "Debouncer.hpp"

#pragma once

class Button {
  // Member functions are non-reentrant
protected:
  Debouncer _debouncer;
  const int _pin;
  const bool _invert;
  bool _pullup;
  bool _state;

public:
  enum Polarity {
    NoInvert = 0,
    Invert,
  };
  enum Resistor {
    NoPull = 0,
    Pullup,
  };

  Button(uint8_t pin,
      Polarity invert = NoInvert,
      Resistor input_resistor = NoPull)
    : _debouncer(10)
    , _pin(pin)
    , _invert(invert)
    , _pullup(input_resistor)
  { }

  void begin();
  void update();
  void click();

  void attachInterrupt(void (*callback)(void), int mode)
  { ::attachInterrupt(digitalPinToInterrupt(_pin), callback, mode); }

  bool state() const // True iff pressed
  { return _state; }

  bool wasPressed()
  { return _debouncer.wasOn(); }

  bool wasReleased()
  { return _debouncer.wasOff(); }

  void setDebounceTimeout(unsigned long ms)
  { _debouncer.setCoolDown(ms); }
};
