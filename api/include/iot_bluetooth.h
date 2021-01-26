#ifndef __IOT_BLUETOOTH_H__
#define __IOT_BLUETOOTH_H__

#include "iot_os.h"
#include "am_openat_bluetooth.h"

void bluetooth_callback(T_OPENAT_BLE_EVENT_PARAM *result);
/**
 * @defgroup iot_sdk_bluetooth �����ӿ�
 * @{
 */
/**@example bluetooth/demo_bluetooth.c
* bluetooth�ӿ�ʾ��
*/

/**������
*@param  mode:          ������ģʽ
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_bt_open(                                        
                        E_OPENAT_BT_MODE mode
                );

/**�ر�����
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_bt_close(                                        
                        VOID
                );

/**д����
*@param  handle:      ���Ӿ��  
*@param  uuid:        д������uuid
*@param  data:      д����������
*@param  len:        д�����ݳ���

*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_ble_write(    
                        UINT16 handle,   
                        T_OPENAT_BLE_UUID uuid,                                 
                        char *data , 
                        UINT8 len         
                );

/**������������
*@param  handle:      ���Ӿ��  
*@param  cmd:        ����cmd
*@param  parm:        ����
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_ble_iotctl(      
                        UINT16 handle,                                  
                        E_OPENAT_BT_CMD cmd,
                        U_OPENAT_BT_IOTCTL_PARAM  param
         
                );

/**�Ͽ���������
* *@param  handle:      ���Ӿ��  
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_ble_disconnect(                                        
                        UINT16 handle
                );

/**��������
* *@param  addr:      ����������ַ
* *@param  addr_type: ����������ַ����
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_ble_connect(    
                        char *addr ,                                    
                        UINT8 addr_type
                        
                );

#endif