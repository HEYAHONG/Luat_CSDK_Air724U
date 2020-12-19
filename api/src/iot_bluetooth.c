#include "iot_bluetooth.h"
/****************************** BLUETOOTH ******************************/


/**������
*@param  mode:          ������ģʽ
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_bt_open(                                        
                        E_OPENAT_BT_MODE mode
                )
{
    IVTBL(SetBLECallback)(bluetooth_callback);
    return IVTBL(OpenBT)(mode);
}

/**�ر�����
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_bt_close(                                        
                        VOID
                )
{
    return IVTBL(CloseBT)();
}

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
                )
{
    return IVTBL(WriteBLE)(handle,uuid,data,len);
}

/**�ر�����
*@param  cmd:        ����cmd
*@param  parm:        �㲥����
*@param  len:        ���ݳ���
*@param  handle:      ���Ӿ��  
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_ble_iotctl(                                        
                        E_OPENAT_BT_CMD cmd,
                        U_OPENAT_BT_IOTCTL_PARM  *parm,
                        UINT8 len,
                        UINT16 handle
                )
{
    return IVTBL(IotctlBLE)(cmd,parm, len,handle);
}

/**�Ͽ���������
* *@param  handle:      ���Ӿ��  
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_ble_disconnect(                                        
                        UINT16 handle
                )
{
    return IVTBL(DisconnectBLE)(handle);
}

/**��������
* *@param  addr:      ����������ַ
* *@param  addr_type: ����������ַ����
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_ble_connect(    
                        char *addr ,                                    
                        UINT8 addr_type
                        
                )
{
    return IVTBL(ConnectBLE)(addr_type,addr);
}

