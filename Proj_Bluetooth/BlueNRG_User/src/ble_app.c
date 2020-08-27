/*
 * ble_app.c
 *
 *  Created on: Aug 22, 2020
 *      Author: akash
 */

/*
 * includes
 */
#include "ble_app.h"
#include "bluenrg_conf.h"
#include "hci_le.h"
#include "hci_const.h"
#include "hci.h"
#include "hci_tl.h"
#include "bluenrg_hal_aci.h"
#include "bluenrg_gap.h"
#include "bluenrg_gap_aci.h"
#include "bluenrg_gatt_aci.h"
#include "ble_services.h"

#include <stdint.h>

/*
 * user variables
 */
extern volatile uint8_t connected;
extern volatile uint8_t set_connectable;
extern volatile uint8_t notification_enabled;
uint8_t button_init_state = FALSE;
/*
 * user defines
 */

#define BDADDR_SIZE 6
/*
 * functions prototypes
 */
static void make_connection();
static void userChat();

/*
 * Initialization of BlueNRG module
 */
void MX_BlueNRG_MS_Init() {
	const char* name = "Preis";
	uint8_t SERVER_BDADDR[] = {0x01,0x02,0x03,0x04,0x05,0x06};
	uint8_t bdaddr[BDADDR_SIZE];

	uint16_t service_handle, dev_name_char_handle, appearance_char_handle;

	hci_init(user_notify, NULL);
	hci_reset();
	HAL_Delay(100);

	BLUENRG_memcpy(bdaddr,SERVER_BDADDR, sizeof(SERVER_BDADDR));

	aci_hal_write_config_data(CONFIG_DATA_PUBADDR_OFFSET, CONFIG_DATA_PUBADDR_LEN,bdaddr);
	aci_gatt_init();

	//IDB05A1 BLE
	aci_gap_init_IDB05A1(GAP_PERIPHERAL_ROLE_IDB05A1, 0, 0x07, &service_handle, &dev_name_char_handle, &appearance_char_handle);

	aci_gatt_update_char_value(service_handle, dev_name_char_handle, 0, strlen(name), (uint8_t*)name);

	//add services
	addServices();

}

/*
 * Initialization of BlueNRG process
 */
void MX_BlueNRG_MS_Process() {
	userChat();
	hci_user_evt_proc();
}
/*
 * user chat function
 */
static void userChat() {
	if(set_connectable) {
		make_connection();
		set_connectable = FALSE;
		button_init_state = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);
	}

	if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) != button_init_state) {
		while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) != button_init_state);

		if(connected && notification_enabled) {
			uint8_t data[2] = {'a','b'};
			sendData(data, 2);
		}
	}
}

static void make_connection() {
	const char local_name[] = {AD_TYPE_COMPLETE_LOCAL_NAME,'B','L','E','-','A','X','G'};

	hci_le_set_scan_resp_data(0, NULL);

	aci_gap_set_discoverable(ADV_IND, 0, 0, PUBLIC_ADDR,NO_WHITE_LIST_USE, sizeof(local_name), local_name, 0, NULL, 0, 0);

}

