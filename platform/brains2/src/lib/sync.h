#pragma once

#include "fsl_common.h"

namespace Sync {
    void init();
    bool jackDetected();
    uint32_t read();
    void write(uint8_t value);
    bool detect();
    uint32_t syncPinState;
}
