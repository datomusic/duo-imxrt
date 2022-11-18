#ifndef DMAMEM
#define DMAMEM __attribute__ ((section(".dmabuffers"), used))
#endif

#define FLASHMEM
#define IRQ_SOFTWARE Reserved70_IRQn
