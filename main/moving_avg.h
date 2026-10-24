//compute moving averages
// One MovingAverage<N> object per quantity (lux, temperature, ...).
// Call add() once per sample; read average() whenever you need the smoothed value.
#pragma once
#include <Arduino.h>

template <uint8_t N>
class MovingAverage {
 public:
  void add(float v) {
    buf[idx] = v;                 // overwrite the oldest sample with the newest
    idx = (idx + 1) % N;          // wrap back to 0 after N samples
    if (count < N) count++;       // until the window is full, only average what we have
  }

  float average() const {
    if (count == 0) return 0.0f;
    float sum = 0.0f;
    for (uint8_t i = 0; i < count; i++) sum += buf[i];   // N is small, so re-summing is cheap
    return sum / count;           // re-summing each time means no rounding drift over days
  }

  uint8_t samples() const { return count; }   // how many samples are in the window so far

 private:
  float   buf[N] = {};
  uint8_t idx    = 0;
  uint8_t count  = 0;
};