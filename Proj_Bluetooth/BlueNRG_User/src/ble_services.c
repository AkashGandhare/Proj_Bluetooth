/*
 * ble_services.c
 *
 *  Created on: 25.08.2020
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
#include "bluenrg_aci_const.h"
#include "main.h"

#include <stdint.h>
#include <stdlib.h>

/*
 * user variables
 */
const uint8_t service_uuid[16] = {0x15, 0x29, 0x07, 0x80, 0xe6, 0xa8, 0x11, 0xea, 0xad, 0xc1, 0x02, 0x42, 0xac, 0x12, 0x00, 0x02};
const uint8_t char_uuid[16] = {0x15, 0x29, 0x09, 0xba, 0xe6, 0xa8, 0x11, 0xea, 0xad, 0xc1, 0x02, 0x42, 0xac, 0x12, 0x00, 0x02};
const uint8_t tempchar_uuid[16] = {0x15, 0x29, 0x0C, 0x9E, 0xe6, 0xa8, 0x11, 0xea, 0xad, 0xc1, 0x02, 0x42, 0xac, 0x12, 0x00, 0x02};
const uint8_t tempDesc_uuid[2] = {0x15,0x02};

uint16_t myServiceHandle, myCharHandle, myTempCharHandle, myDescHandle;

uint32_t connected = FALSE;
uint8_t set_connectable = 1;
uint16_t connection_handle = 0;
uint8_t notification_enabled = FALSE;


/*
 * @brief add services to the devices
 *
 */

tBleStatus addServices(void) {

	charactFormat charFormat;
	tBleStatus ret;

	aci_gatt_add_serv(UUID_TYPE_128, service_uuid, PRIMARY_SERVICE, 7, &myServiceHandle);
	ret = aci_gatt_add_char(myServiceHandle, UUID_TYPE_128, char_uuid, 2, CHAR_PROP_READ, ATTR_PERMISSION_NONE, GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP, 16, 0, &myCharHandle);
	ret = aci_gatt_add_char(myServiceHandle, UUID_TYPE_128, tempchar_uuid, 2, CHAR_PROP_READ, ATTR_PERMISSION_NONE, GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP, 16, 0, &myTempCharHandle);

	charFormat.format = FORMAT_SINT16;
	charFormat.exp = -1;
	charFormat.unit = UNIT_TEMP_CELSIUS;
	charFormat.name_space = 0;
	charFormat.desc = 0;

	aci_gatt_add_char_desc(myServiceHandle, myTempCharHandle, UUID_TYPE_16, (uint8_t *)tempDesc_uuid, 7, 7, (void *)&charFormat, ATTR_PERMISSION_NONE, ATTR_ACCESS_READ_ONLY,0,16,FALSE, &myDescHandle);

	return ret;
}

/*
 * @brief update general data
 *
 */

tBleStatus updateData(int16_t newData) {

	tBleStatus ret;

	ret = aci_gatt_update_char_value(myServiceHandle, myCharHandle, 0, 2, (uint8_t *)&newData);

	return ret;

}

/*
 * @brief update temperature data
 *
 */

tBleStatus updateTempData(int16_t tempData) {

	tBleStatus ret;
	ret = aci_gatt_update_char_value(myServiceHandle, myTempCharHandle, 0, 2, (uint8_t *)&tempData);

	return ret;

}
/*
 * @brief BLE connection complete CB
 */
void GAP_ConnectionComplete_CB(uint8_t addr[6], uint16_t handle) {

//#Todo: connection complete
	connected = TRUE;
	connection_handle = handle;
	HAL_GPIO_WritePin(GPIOA, LED_GREEN_Pin, GPIO_PIN_SET);
	HAL_Delay(100);

}

/*
 * @brief BLE disconnection complete CB
 */
void GAP_DisconnectionComplete_CB(void) {

//#Todo: connection complete
	HAL_GPIO_WritePin(GPIOA, LED_GREEN_Pin, GPIO_PIN_RESET);
	HAL_Delay(100);

}

/*
 * @brief read request complete CB
 */
void Read_Request_CB(uint16_t handle) {

	if(handle == myCharHandle+1) {

		int myData = 450;
		myData = 450 + ((uint64_t) rand()*100)/1000;
		updateData(myData);
	//	updateTempData();

	}

	if(handle == myTempCharHandle+1) {

		int16_t myTempData = 0;
		myTempData = 50 + ((uint64_t) rand()*100)/1000;
		updateTempData(myTempData);

	}

	if(connection_handle != 0) {
		aci_gatt_allow_read(connection_handle);
	}

}

/*
 * @brief user_notify events
 */
void user_notify(void *pdata) {

	hci_uart_pckt *hci_pckt = pdata;
	hci_event_pckt *event_pckt = (hci_event_pckt *)hci_pckt->data;

	if(hci_pckt->type != HCI_EVENT_PKT)
		return;

	switch(event_pckt->evt) {

		case EVT_DISCONN_COMPLETE:
		{
			GAP_DisconnectionComplete_CB();
		}
		break;

		case EVT_LE_META_EVENT:
		{
			evt_le_meta_event *evt = (void *)event_pckt->data;

			switch(evt->subevent)
			{
				case EVT_LE_CONN_COMPLETE:
				{
					evt_le_connection_complete *cc = (void *)evt->data;
					GAP_ConnectionComplete_CB(cc->peer_bdaddr, cc->handle);

				}
				break;
			}
		}
		break;

		case EVT_VENDOR:
		{
			evt_blue_aci *blue_evt = (void *)event_pckt->data;

			switch(blue_evt->ecode)
			{
				case EVT_BLUE_GATT_READ_PERMIT_REQ:
				{
					evt_gatt_read_permit_req *pr = (void *)blue_evt->data;
					Read_Request_CB(pr->attr_handle);

				}
				break;
			}
		}
		break;
	}
}
