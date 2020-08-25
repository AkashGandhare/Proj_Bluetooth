/*
 * ble_services.h
 *
 *  Created on: 25.08.2020
 *      Author: akash
 */

#ifndef INC_BLE_SERVICES_H_
#define INC_BLE_SERVICES_H_

/*
 * user includes
 */


/*
 * user defined prototypes
 */
tBleStatus updateData(int16_t newData);
tBleStatus addServices(void);
void user_notify(void *pdata);
#endif /* INC_BLE_SERVICES_H_ */
