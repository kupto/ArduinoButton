/*
 * Debouncer for Arduino.
 *
 * Copyright (c) 2021, Kupto
 *
 * When flicking a switch or pressing a real world button, several rapid input
 * voltage HIGH-LOW transitions might occur, while just one is expected.  This
 * phenomenon may be produced by (among others) ripple currents, imperfections
 * in mechanical construction of switches, or by Parkinson's disease.
 *
 * The debouncer solves the issue of registering multiple clicks, where single
 * state change should be reported. This is done by starting a cool-down timer
 * on the first input change, while ignoring all subsequent changes, until the
 * cool-down period elapses. Note that selecting a correct duration for such a
 * timer is the tricky part.
 *
 * This is free software; published under the MIT license.
 * See the LICENSE file.
 */
#include <Arduino.h>

#pragma once

class Debouncer {
  // Member functions are non-reentrant
  enum class State {
    None,
    On,
    Off,
    Acked,
  } _state;
  unsigned long _ms;
  unsigned long _cooldown;

  void start(State state) {
    const unsigned long now = millis();

    if (_state != State::None && (now - _ms) < _cooldown)
      return; // Debouncing in progress

    _state = state;
    _ms = now;
  }

  bool query(State state) {
    // Returns true once per Debouncer [de]activation
    if (_state != state)
      return false;

    _state = State::Acked;
    return true;
  }

public:
  Debouncer(unsigned long delay)
    : _state(State::None)
    , _ms(0-delay-1)
    , _cooldown(delay)
  { }

  void reset() {
    // Skip currently running debouncing and forget the on/off state
    _state = State::None;
  }
  void on() {
    start(State::On);
  }
  void off() {
    start(State::Off);
  }
  bool wasOn() {
    return query(State::On);
  }
  bool wasOff() {
    return query(State::Off);
  }

  void setCoolDown(unsigned long delay)
  { _cooldown = delay; }
};
