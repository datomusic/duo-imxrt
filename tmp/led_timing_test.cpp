#include <cassert>
#include <cstdint>
#include <stdio.h>

// #define UNSIGNED_TYPE uint32_t
// #define MAX UINT32_MAX

#define UNSIGNED_TYPE uint16_t
#define MAX UINT16_MAX

void pin_lo() {}
void pin_hi() {}

struct DWT_ {
  UNSIGNED_TYPE CYCCNT;
};

struct Timings {
  const UNSIGNED_TYPE interval;
  const UNSIGNED_TYPE bit_on;
  const UNSIGNED_TYPE bit_off;
};

// Specific timings for WS2812
#define T1 300
#define T2 600
#define T3 300

// #define NS_PER_SEC (1000000000L)
// #define CYCLES_PER_SEC (SystemCoreClock)
// #define NS_PER_CYCLE (NS_PER_SEC / CYCLES_PER_SEC)
#define NS_TO_CYCLES(n) (n)

// We need a macro to invoke every frame, since timings
// depend on SystemCoreClock, which is variable.
#define GET_TIMINGS()                                                          \
  Timings{.interval = NS_TO_CYCLES(T1 + T2 + T3),                              \
          .bit_on = NS_TO_CYCLES(T2 + T3),                                     \
          .bit_off = NS_TO_CYCLES(T3)};

DWT_ dwt;
DWT_ *DWT = &dwt;

void inc_cyc() { DWT->CYCCNT++; }
bool should_overflow = false;
void overflow_cyc() {
  if (should_overflow) {
    DWT->CYCCNT = 0;
  }
};
// void overflow_cyc() {};

int on_count = 0;
int off_count = 0;

static inline void send_bit(const uint8_t bit, UNSIGNED_TYPE &next_cycle_start,
                            const Timings &timings) {
  inc_cyc();

  // Wait for next interval cutoff
  while (DWT->CYCCNT < next_cycle_start) {
    inc_cyc();
    ++off_count;
  }

  inc_cyc();

  // Set next interval cutoff.
  // It is important that this happens immediately after the previous wait.
  next_cycle_start = DWT->CYCCNT + timings.interval;
  inc_cyc();

  // Keep bit on for relevant time.
  const UNSIGNED_TYPE on_cycles = bit ? timings.bit_on : timings.bit_off;
  inc_cyc();
  const UNSIGNED_TYPE on_cutoff =
      next_cycle_start - (timings.interval - on_cycles);
  inc_cyc();
  pin_hi();

  overflow_cyc();

  while (DWT->CYCCNT < on_cutoff) {
    inc_cyc();
    ++on_count;
  }

  inc_cyc();

  inc_cyc();
  // Pin will be kept low until next time send_byte is called.
  pin_lo();
  inc_cyc();

  inc_cyc();
}

void test() {
  DWT->CYCCNT = MAX / 2;
  on_count = off_count = 0;

  const auto timings = GET_TIMINGS();
  UNSIGNED_TYPE next_cycle_start = DWT->CYCCNT + timings.interval;
  send_bit(0, next_cycle_start, timings);

#define P(x) printf(#x ": %u\n", x)

  P(on_count);
  P(off_count);
  P(DWT->CYCCNT);

  assert(on_count < 300);
  assert(off_count == 1199);
}

int main(int argc, char *argv[]) {
  printf("=== Without overflow ===\n");
  should_overflow = false;
  test();

  printf("\n=== With overflow ===\n");
  should_overflow = true;
  test();
  return 0;
}
