#ifndef HW_VESCXPRESS2G3BRIDGE_H_
#define HW_VESCXPRESS2G3BRIDGE_H_

#define HW_NAME                 "VESCXPRESS2G3Bridge"
#define HW_TARGET               "esp32s3_n8r2"
#define HW_UART_COMM

#define HW_INIT_HOOK()          hw_init()

// CAN -- confirmed for the VESC2X3Bridge PCB (ESP32-S3-WROOM-1-N8R2), see
// docs/existing-implementation-reference.md in VESCXPRESS2G3Bridge.
#define CAN_TX_GPIO_NUM			48
#define CAN_RX_GPIO_NUM			45

// UART intentionally omitted -- not confirmed for this board (no
// documented UART wiring in VESCXPRESS2G3Bridge's notes). hw.h falls back
// to HW_NO_UART automatically when UART_NUM isn't defined, so leaving
// this out is the honest choice rather than guessing pins.

// Light/indicator/button GPIOs (right/left indicator, rear light PWM,
// button) are intentionally NOT here -- they're managed entirely at
// runtime by VESCXPRESS2G3Bridge's main.lisp via the generic
// gpio-configure/gpio-write/pwm-start LBM extensions, which take plain
// pin numbers with no compile-time hwconf entry required. Nothing in
// vesc_express's C firmware itself ever touches those pins, unlike CAN
// (comm_can_start() in main.c is compiled in only #ifdef CAN_TX_GPIO_NUM).

// Functions
void hw_init(void);

#endif /* HW_VESCXPRESS2G3BRIDGE_H_ */
