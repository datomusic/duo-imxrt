#ifndef SYNC_H
#define SYNC_H

#include <cstdint>

namespace Sync {
    void init();
    bool jackDetected();
    uint32_t read();
    void write(uint8_t value);
    bool detect();
}


#endif /* end of include guard: SYNC_H */
