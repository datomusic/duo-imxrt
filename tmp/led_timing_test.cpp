#include <cassert>
#include <cstdint>
#include <stdio.h>

// #define UNSIGNED_TYPE uint32_t
// #define MAX UINT32_MAX

#define UNSIGNED_TYPE uint16_t
#define MAX UINT16_MAX

// #define UNSIGNED_TYPE uint8_t
// #define MAX UINT8_MAX

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

void delay(unsigned long tick) {
  unsigned long start = DWT->CYCCNT;
  for (;;) {
    unsigned long now = DWT->CYCCNT;
    unsigned long elapsed = now - start;
    if (elapsed >= tick)
      return;
  }
}

static inline void send_bit(const uint8_t bit, UNSIGNED_TYPE &last_mark,
                            const Timings &timings) {
  overflow_cyc();

  // Wait for next interval cutoff
  while ((UNSIGNED_TYPE)(DWT->CYCCNT - last_mark) < timings.interval) {
    // printf("Delta: %u\n", (UNSIGNED_TYPE)(DWT->CYCCNT - last_mark));
    inc_cyc();
    ++off_count;
  }

  inc_cyc();
  // overflow_cyc();

  // Set next interval cutoff.
  // It is important that this happens immediately after the previous wait.
  last_mark = DWT->CYCCNT;
  inc_cyc();
  pin_hi();
  // overflow_cyc();

  // Keep bit on for relevant time.
  if (bit) {
    assert(false && "Only testing with 0 bit.");
  } else {
    while ((UNSIGNED_TYPE)(DWT->CYCCNT - last_mark) < timings.bit_off) {
      ++on_count;
      inc_cyc();
    }
    pin_lo();
  }
  inc_cyc();
}

void test() {
  DWT->CYCCNT = MAX / 2;
  on_count = off_count = 0;

  const auto timings = GET_TIMINGS();
  UNSIGNED_TYPE last_mark = DWT->CYCCNT;
  send_bit(0, last_mark, timings);

#define P(x) printf(#x ": %u\n", x)

  P(on_count);
  P(off_count);
  P(DWT->CYCCNT);

  assert(on_count < 900);
  assert(off_count <= 1200);
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
