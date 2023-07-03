#include "unity.h"
#include "firmware/sequencer_test.h"

void setUp(void) {}
void tearDown(void) {}
int main(void) {
  UNITY_BEGIN();

  Sequencer_run_tests();

  return UNITY_END();
}
