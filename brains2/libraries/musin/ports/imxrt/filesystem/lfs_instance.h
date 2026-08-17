#ifndef MUSIN_PORTS_IMXRT_LFS_INSTANCE_H_
#define MUSIN_PORTS_IMXRT_LFS_INSTANCE_H_

#include "lfs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief The mounted littlefs instance, or NULL before Filesystem::init()
 * succeeds. Shared between the Filesystem facade and the newlib stdio
 * syscalls in lfs_stdio.c.
 */
lfs_t *musin_imxrt_lfs(void);

#ifdef __cplusplus
}
#endif

#endif
