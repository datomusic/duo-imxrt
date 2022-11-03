set(LINKER ${CMAKE_CURRENT_LIST_DIR}/../armgcc/MIMXRT1011_uf2.ld)

set(CFLAGS_COMMON " \
    -DCPU_MIMXRT1011DAE5A \
    -DIMXRT10XX \
    -D__MIMXRT1011DAE5A__ \
    -D__IMXRT1011__ \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DMCUXPRESSO_SDK \
    -mcpu=cortex-m7 \
    -Wall \
    -mfloat-abi=hard \
    -mfpu=fpv5-sp-d16 \
    -mthumb \
    -MMD \
    -MP \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -ffreestanding \
    -fno-builtin \
    -mapcs \
    -nostdlib \
    -fshort-enums \
    -fsingle-precision-constant \
    -fno-strict-aliasing \
    -fno-inline-functions \
    -ggdb \
    -fstack-usage \
    -nostdlib \
")

set(LDFLAGS_COMMON " \
    ${CFLAGS_COMMON} -nostartfiles -Wl,-nostdlib -Wl,-Map=output.map -Wl,-cref -Wl,-gc-sections -specs=nano.specs \
    -mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-sp-d16 -mthumb -mapcs \
")

    # --specs=nano.specs \
    # --specs=nosys.specs \
    # -Xlinker \
    # --gc-sections \
    # -Xlinker \
    # -static \
    # -Xlinker \
    # -z \
    # -Xlinker \
    # muldefs \
    # -Xlinker \
    # -Map=output.map \
    # -Wl,--print-memory-usage \


set(CFLAGS_DEBUG_COMMON,
    -DDEBUG
    -O0
    -g3
)

set(ASM_COMMON, 
    -D__STARTUP_INITIALIZE_NONCACHEDATA
    -D__STARTUP_CLEAR_BSS
    -D__ARM_ARCH_7EM__
    ${CFLAGS_COMMON}
)


SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG " \
    ${CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG} \
    ${CFLAGS_DEBUG_COMMON} \
    ${CFLAGS_COMMON} \
    ${ASM_COMMON} \
")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG " \
    ${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} \
    ${CFLAGS_COMMON} \
    ${CFLAGS_DEBUG_COMMON} \
")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG " \
    ${CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG} \
    ${CFLAGS_DEBUG_COMMON} \
    ${CFLAGS_COMMON} \
")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG " \
    ${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG} \
    ${CFLAGS_COMMON} \
    ${CFLAGS_DEBUG_COMMON} \
    ${LDFLAGS_COMMON} \
    -Wl,-T${LINKER} \
")
