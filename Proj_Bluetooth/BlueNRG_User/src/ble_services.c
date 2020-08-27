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
#include "usart.h"

#include <stdint.h>
#include <stdlib.h>

/*
 * user variables
 */
const uint8_t service_uuid[16] = {0x15, 0x29, 0x07, 0x80, 0xe6, 0xa8, 0x11, 0xea, 0xad, 0xc1, 0x02, 0x42, 0xac, 0x12, 0x00, 0x02};
const uint8_t charTx_uuid[16] = {0x15, 0x29, 0x09, 0xba, 0xe6, 0xa8, 0x11, 0xea, 0xad, 0xc1, 0x02, 0x42, 0xac, 0x12, 0x00, 0x02};
const uint8_t charRx_uuid[16] = {0x15, 0x29, 0x0C, 0x9E, 0xe6, 0xa8, 0x11, 0xea, 0xad, 0xc1, 0x02, 0x42, 0xac, 0x12, 0x00, 0x02};

uint16_t myServiceHandle, myCharTxHandle, myCharRxHandle;

volatile uint8_t connected = FALSE;
volatile uint8_t set_connectable = 1;
volatile uint16_t connection_handle = 0;
volatile uint8_t notification_enabled = FALSE;
volatile uint8_t start_read_tx_char_handle = FALSE;
volatile uint8_t start_read_rx_char_handle = FALSE;
volatile uint8_t end_read_rx_char_handle = FALSE;
volatile uint8_t end_read_tx_char_handle = FALSE;


/*
 * @brief add services to the devices
 *
 */

tBleStatus addServices(void) {

	tBleStatus ret;

	aci_gatt_add_serv(UUID_TYPE_128, service_uuid, PRIMARY_SERVICE, 7, &myServiceHandle);
	ret = aci_gatt_add_char(myServiceHandle, UUID_TYPE_128, charTx_uuid, 20, CHAR_PROP_NOTIFY, ATTR_PERMISSION_NONE, GATT_DONT_NOTIFY_EVENTS, 16, 0, &myCharTxHandle);
	ret = aci_gatt_add_char(myServiceHandle, UUID_TYPE_128, charRx_uuid, 20, CHAR_PROP_WRITE |CHAR_PROP_WRITE_WITHOUT_RESP, ATTR_PERMISSION_NONE, GATT_NOTIFY_ATTRIBUTE_WRITE, 16, 1, &myCharRxHandle);

	return ret;
}

/*
 * @brief receive data
 */
void receiveData(uint8_t *dataBuffer, uint8_t dataLen) {
	uint8_t recData[30];
	memset(recData,0,sizeof(recData));

	HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);

	memcpy(recData,dataBuffer,dataLen);
	HAL_UART_Transmit(&huart1, recData, dataLen, 100);

}

/*
 * @brief send data
 */
void sendData(uint8_t *dataBuffer, uint8_t dataLen) {
	aci_gatt_update_char_value(myServiceHandle, myCharTxHandle, 0, dataLen, dataBuffer);

}

/*
 * @brief attribute modified CB
 */
void attribute_Modified_CB(uint16_t handle, uint8_t dataLen, uint8_t *attData) {

	if(handle == myCharRxHandle+1){
		receiveData(attData, dataLen);
	} else if(handle == myCharTxHandle+2) {
		if(attData[0] == 0x01) {
			notification_enabled = TRUE;
		}
	}
}
/*
 * @brief BLE connection complete CB
 */
void GAP_ConnectionComplete_CB(uint8_t addr[6], uint16_t handle) {

//#Todo: connection complete
	connected = TRUE;
	connection_handle = handle;
	printf("Connected to the Device:\r\n");

	for(int i = 5; i>=0; i--){
		printf("%02X-\r\n",addr[i]);
	}


}

/*
 * @brief BLE disconnection complete CB
 */
void GAP_DisconnectionComplete_CB(void) {

//#Todo: connection complete
	connected= FALSE;
	printf("Disconnected the device:\r\n");
	set_connectable = TRUE;

	start_read_tx_char_handle = FALSE;
	start_read_rx_char_handle = FALSE;
	end_read_rx_char_handle = FALSE;
	end_read_tx_char_handle = FALSE;

}

/*
 * @brief read request complete CB
 */
//void Read_Request_CB(uint16_t handle) {
//
//	if(handle == myCharHandle+1) {
//
//		int myData = 450;
//		myData = 450 + ((uint64_t) rand()*100)/1000;
//		updateData(myData);
//	//	updateTempData();
//
//	}
//
//	if(handle == myTempCharHandle+1) {
//
//		int16_t myTempData = 0;
//		myTempData = 50 + ((uint64_t) rand()*100)/1000;
//		updateTempData(myTempData);
//
//	}
//
//	if(connection_handle != 0) {
//		aci_gatt_allow_read(connection_handle);
//	}
//
//}

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
//				case EVT_BLUE_GATT_READ_PERMIT_REQ:
//				{
//					evt_gatt_read_permit_req *pr = (void *)blue_evt->data;
//					Read_Request_CB(pr->attr_handle);
//
//				}
//				break;
			case EVT_BLUE_GATT_ATTRIBUTE_MODIFIED:
			{
				evt_gatt_attr_modified_IDB05A1 *evt = (evt_gatt_attr_modified_IDB05A1*) blue_evt->data;
				attribute_Modified_CB(evt->attr_handle,evt->data_length,evt->att_data);
			}
			break;
			}
		}
		break;
	}
}
