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

#include <stdint.h>

/*
 * user defines
 */

#define BDADDR_SIZE 6
/*
 * functions prototypes
 */

/*
 * Initialization of BlueNRG module
 */
void MX_BlueNRG_MS_Init() {
	const char* name = "Preis";
	uint8_t SERVER_BDADDR[] = {0x01,0x02,0x03,0x04,0x05,0x06};
	uint8_t bdaddr[BDADDR_SIZE];

	uint16_t service_handle, dev_name_char_handle, appearance_char_handle;

	hci_init(NULL, NULL);
	hci_reset();
	HAL_Delay(100);

	BLUENRG_memcpy(bdaddr,SERVER_BDADDR, sizeof(SERVER_BDADDR));

	aci_hal_write_config_data(CONFIG_DATA_PUBADDR_OFFSET, CONFIG_DATA_PUBADDR_LEN,bdaddr);
	aci_gatt_init();

	//IDB05A1 BLE
	aci_gap_init_IDB05A1(GAP_PERIPHERAL_ROLE_IDB05A1, 0, 0x07, &service_handle, &dev_name_char_handle, &appearance_char_handle);

	aci_gatt_update_char_value(service_handle, dev_name_char_handle, 0, strlen(name), (uint8_t*)name);

}

/*
 * Initialization of BlueNRG process
 */
void MX_BlueNRG_MS_Process() {
	tBleStatus ret;
	const char local_name[] = {AD_TYPE_COMPLETE_LOCAL_NAME,'B','L','E','-','A','L','G'};

	hci_le_set_scan_resp_data(0, NULL);

	ret = aci_gap_set_discoverable(ADV_IND, 0, 0, PUBLIC_ADDR,NO_WHITE_LIST_USE, sizeof(local_name), local_name, 0, NULL, 0, 0);

}

