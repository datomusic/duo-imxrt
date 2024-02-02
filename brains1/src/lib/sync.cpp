#include "sync.h"

#include "Arduino.h"
#include "pinmap.h"

namespace Sync {
void init() {
  
}

uint32_t read() {
  return digitalRead(SYNC_IN);

}

void write(const uint8_t value) {
  digitalWrite(SYNC_OUT_PIN, value);
}

bool detect(){
  return digitalRead(SYNC_DETECT);
}
} // namespace Sync
