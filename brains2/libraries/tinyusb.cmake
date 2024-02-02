include_guard(GLOBAL)
message("TinyUSB component is included.")

# TinyUSB defines
add_definitions(
  -DCFG_TUSB_MCU=OPT_MCU_MIMXRT11XX
  -DCFG_TUD_MIDI_RX_BUFSIZE=512
  -DCFG_TUD_CDC_RX_BUFSIZE=512
  -DCFG_TUD_MIDI_TX_BUFSIZE=512
  -DCFG_TUD_CDC_TX_BUFSIZE=512
  -DCFG_TUD_MSC_BUFSIZE=1024
)


set(TINYUSB_SRC ${CMAKE_CURRENT_LIST_DIR}/tinyusb/src)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${TINYUSB_SRC}/class/cdc/cdc_device.c
    ${TINYUSB_SRC}/common/tusb_fifo.c
    ${TINYUSB_SRC}/device/usbd.c
    ${TINYUSB_SRC}/device/usbd_control.c
    ${TINYUSB_SRC}/tusb.c
    ${TINYUSB_SRC}/class/midi/midi_device.c
    ${TINYUSB_SRC}/portable/chipidea/ci_hs/dcd_ci_hs.c
)
