#ifndef HW_VESCXPRESS2G3BRIDGE_H_
#define HW_VESCXPRESS2G3BRIDGE_H_

#define HW_NAME                 "VESCXPRESS2X3Bridge"
#define HW_TARGET               "esp32s3_n8r2"
#define HW_UART_COMM

#define HW_INIT_HOOK()          hw_init()

#define CAN_TX_GPIO_NUM			48
#define CAN_RX_GPIO_NUM			45

#define CONF_WIFI_MODE 0

#define CONF_BLE_NAME "X3Bridge"

// Functions
void hw_init(void);

#endif /* HW_VESCXPRESS2G3BRIDGE_H_ */
