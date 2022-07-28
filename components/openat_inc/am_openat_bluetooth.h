/*********************************************************
  Copyright (C), AirM2M Tech. Co., Ltd.
  Author: liangjian
  Description: AMOPENAT ����ƽ̨
  Others:
  History: 
    Version�� Date:       Author:   Modification:
    V0.1      2020.09.05  liangjain     �����ļ�
*********************************************************/
#ifndef AM_OPENAT_BULETOOTH_H
#define AM_OPENAT_BULETOOTH_H

#include "am_openat_common.h"
//#include "drv_bluetooth.h"

#define BLE_MAX_DATA_COUNT  244
#define BLE_MAX_ADV_MUBER  31
#define BLE_LONG_UUID_FLAG 16
#define BLE_SHORT_UUID_FLAG 2

typedef struct 
{
    unsigned char id;         ///< event identifier
    char state;               ///< ����״̬
    UINT8 len;                ///< ���ص����ݳ���
    unsigned char * dataPtr;  ///< ���ص�����ָ��
    UINT16 uuid;   
    UINT16 handle;
    UINT8 long_uuid[16];
    UINT8 uuid_flag;
}T_OPENAT_BLE_EVENT_PARAM;

typedef struct 
{
   UINT16 AdvMin;            ///< ��С�㲥���
   UINT16 AdvMax;            ///< ���㲥���
   UINT8 AdvType;           ///< �㲥����
   UINT8 OwnAddrType;       ///< �㲥���ص�ַ����
   UINT8 DirectAddrType;    ///< �����ַ����
   char *DirectAddr;        ///< �����ַ
   UINT8 AdvChannMap;       ///< �㲥channel map,3��bit���ֱ��Ӧ37�� 38�� 39�ŵ� 
   UINT8 AdvFilter;         ///< �㲥���˲���
}T_OPENAT_BLE_ADV_PARAM;

typedef struct 
{
    UINT8 scanType;        //  ɨ������   
    UINT16 scanInterval;   //  ɨ����
    UINT16 scanWindow;     //  ɨ�贰��
    UINT8 filterPolicy;    //  ɨ����˲���
    UINT8 own_addr_type;   //  ���ص�ַ����
} T_OPENAT_BLE_SCAN_PARAM;

typedef enum
{
    BLE_SET_NAME = 0x01,    ///< ����BLE �㲥����
    BLE_SET_ADV_PARAM,		///< ����BLE �㲥�㲥����
    BLE_SET_ADV_DATA,		///< ����BLE �㲥�㲥������
    BLE_SET_SCANRSP_DATA,	///< ����BLE �㲥��Ӧ������
	BLE_SET_ADV_ENABLE,		///< �Ƿ�ʹ�ܹ㲥
	BLE_SET_SCAN_ENABLE,		///< �Ƿ�ʹ��ɨ��
	BLE_READ_STATE,			///< ��BLE �Ƿ�ʹ��
    BLE_ADD_SERVICE,        ///< ��ӷ���
    BLE_ADD_CHARACTERISTIC, ///< �������
    BLE_ADD_DESCRIPTOR,     ///< �������
    BLE_CLEAN_ALLSERVICE,   ///< ������з���
    BLE_FIND_SERVICE,       ///< ���ַ���
    BLE_FIND_CHARACTERISTIC,///< ��������
    BLE_OPEN_NOTIFICATION,  ///< ��֪ͨ
    BLE_CLOSE_NOTIFICATION,  ///< �ر�֪ͨ
    BLE_GET_ADDR,           ///< ��ȡ����MAC��ַ
    BLE_SET_BEACON_DATA,     ///< ����beacon����
    BLE_SET_SCAN_PARAM,		///< ����BLEɨ�����
    BLE_ADD_WHITE_LIST,		///< ���BLE�������豸
    BLE_REMOVE_WHITE_LIST,	///< �Ƴ�BLE�������豸
    BLE_CLEAR_WHITE_LIST,	///< ���BLE������
    BLE_READ_VALUE,	        ///< ��BLE����value
    BLE_SET_VALUE,	        ///< ����BLE����value

    BT_GET_ADDR,           ///< ��ȡ����MAC��ַ
    BT_READ_STATE,			///< ��BT �Ƿ�ʹ��
    BT_SET_NAME,           ///< ����BT����
    BT_SET_VISIBILITY,      ///< ����BT�ɼ���
    BT_SET_HFP_VOL,      ///< ����hfp������
    BT_HFP_CALL_REJECT,  ///< HFP�ܾ�����
    BT_HFP_CALL_ANSWER,  ///< HFP��������
    BT_HFP_CALL_HANGUP,  ///< HFP�Ҷ�����
    BT_HFP_CALL_REDIAL,  ///< HFP�ز�
    BT_HFP_CALL_DIAL,  ///< HFP����
    BT_SET_AVRCP_VOL,  ///< ����AVRCP������
    BT_SET_AVRCP_SONGS,    ///< AVRCP�ĸ�������
} E_OPENAT_BT_CMD;

typedef enum
{
    BLE_SLAVE = 0,    ///< ����BLE��ģʽ
    BLE_MASTER,	      ///< ����BLE��ģʽ
    BT_CLASSIC,	          ///< ���þ�������
} E_OPENAT_BT_MODE;

typedef enum
{
    UUID_SHORT = 0,    // 16λuuid
    UUID_LONG,	       // 128λuuid
} E_OPENAT_BLE_UUID_FLAG;

typedef struct 
{
    E_OPENAT_BLE_UUID_FLAG uuid_type;
    union {
        UINT16 uuid_short;
        UINT8 uuid_long[16];
    };
}T_OPENAT_BLE_UUID;

typedef struct 
{
    T_OPENAT_BLE_UUID uuid;
    UINT8   attvalue;//����ֵ
    UINT16  permisssion;//Ȩ��
}T_OPENAT_BLE_CHARACTERISTIC_PARAM;

typedef struct 
{
    T_OPENAT_BLE_UUID uuid;
    UINT8   value[255];//����
    UINT16  configurationBits;//����
}T_OPENAT_BLE_DESCRIPTOR_PARAM;

typedef struct 
{
    UINT8 	addr_type; 
    char    *addr;
}T_OPENAT_BLE_WHITE_LIST;

typedef struct 
{
    UINT8 		data[BLE_MAX_ADV_MUBER]; 
    UINT8       len;
}T_OPENAT_BLE_ADV_DATA;

typedef struct 
{
    UINT8 uuid[16];
    UINT16 major;
    UINT16 minor; 
}T_OPENAT_BLE_BEACON_DATA;

typedef enum
{
    BT_AVRCP_STATE_PAUSE = 0x00,
    BT_AVRCP_STATE_START,
    BT_AVRCP_STATE_PREVI,
    BT_AVRCP_STATE_NEXT,
} E_OPENAT_BT_AVRCP_STATE;

typedef struct 
{
    T_OPENAT_BLE_UUID uuid;
    UINT8   data[BLE_MAX_DATA_COUNT];
    UINT8   len;
}T_OPENAT_BLE_SET_VALUE_PARAM;

typedef union {
    T_OPENAT_BLE_ADV_PARAM  *advparam;   ///< ����BLE �㲥����
    T_OPENAT_BLE_SCAN_PARAM  *scanparam;   ///< ����BLE ɨ�����
    T_OPENAT_BLE_ADV_DATA   *advdata;    ///< �㲥�����ݡ���Ӧ������
    UINT8 	*data;    ///< ����BLE �㲥���ơ���ȡ����MAC��ַ������BT���ơ�hfp���в���
    UINT8       advEnable;          ///< �Ƿ�ʹ�ܹ㲥��ʹ��ɨ�衢����BT�ɼ��ԡ�����hfp����������avrcp����
    T_OPENAT_BLE_UUID  *uuid;   ///< ��ӷ��񡢷�����������֪ͨ���ر�֪ͨ
    T_OPENAT_BLE_CHARACTERISTIC_PARAM  *characteristicparam;   ///< �������
    T_OPENAT_BLE_DESCRIPTOR_PARAM   *descriptorparam;   ///< �������
    T_OPENAT_BLE_BEACON_DATA   *beacondata;   ///< ����beacon����
    T_OPENAT_BLE_WHITE_LIST    *addrparam;  ///<���BLE�������豸���Ƴ�BLE�������豸
    T_OPENAT_BLE_SET_VALUE_PARAM  *valueparam;   ///< �������value
    E_OPENAT_BT_AVRCP_STATE state; ///< ����BT�ĸ�������״̬
}U_OPENAT_BT_IOTCTL_PARAM;

typedef enum{
    OPENAT_BT_ME_ON_CNF = 1,
    OPENAT_BT_ME_OFF_CNF,
    OPENAT_BT_VISIBILE_CNF,
    OPENAT_BT_HIDDEN_CNF,
    OPENAT_BT_SET_LOCAL_NAME_RES,
    OPENAT_BT_SET_LOCAL_ADDR_RES,
    OPENAT_BT_INQ_DEV_NAME,
    OPENAT_BT_INQ_COMP_CNF,
    OPENAT_BT_INQUIRY_CANCEL,
    OPENAT_BT_DEV_PAIR_COMPLETE,
    OPENAT_BT_DELETE_DEVICE_RES,
    OPENAT_BT_DEV_PIN_REQ,
    OPENAT_BT_SSP_NUM_IND,
    OPENAT_BT_SPP_CONNECT_IND,
    OPENAT_BT_SPP_DISCONNECT_IND,
    OPENAT_BT_SPP_DATA_RECIEVE_IND,
    OPENAT_BT_SPP_DATA_SEND_IND,
	OPENAT_BT_HFP_CONNECT_RES,
	OPENAT_BT_HFP_DISCONNECT_RES,
	OPENAT_BT_REOPEN_IND,				//stop of BT reopen
	OPENAT_BT_REOPEN_ACTION_IND, //start of BT reopen
    OPENAT_BT_HFP_CALLSETUP_OUTGOING,
    OPENAT_BT_HFP_CALLSETUP_INCOMING,
    OPENAT_BT_HFP_CALLSETUP_NONE,
    OPENAT_BT_HFP_RING_INDICATION,
    OPENAT_BT_AVRCP_CONNECT_COMPLETE,
    OPENAT_BT_AVRCP_DISCONNECT_COMPLETE,

    OPENAT_BLE_SET_PUBLIC_ADDR = 51,
    OPENAT_BLE_SET_RANDOM_ADDR,
    OPENAT_BLE_ADD_WHITE_LIST,
    OPENAT_BLE_REMOVE_WHITE_LIST,
    OPENAT_BLE_CLEAR_WHITE_LIST,
    OPENAT_BLE_CONNECT,
    OPENAT_BLE_DISCONNECT,
    OPENAT_BLE_UPDATA_CONNECT,
    OPENAT_BLE_SET_ADV_PARA,
    OPENAT_BLE_SET_ADV_DATA,
    OPENAT_BLE_SET_ADV_ENABLE,
    OPENAT_BLE_SET_ADV_SCAN_RSP,
    OPENAT_BLE_SET_SCAN_PARA,
    OPENAT_BLE_SET_SCAN_ENABLE,
    OPENAT_BLE_SET_SCAN_DISENABLE,
    OPENAT_BLE_SET_SCAN_REPORT,
    OPENAT_BLE_SET_SCAN_FINISH,
    OPENAT_BLE_CONNECT_IND,
    OPENAT_BLE_DISCONNECT_IND,
    OPENAT_BLE_FIND_CHARACTERISTIC_IND,
    OPENAT_BLE_FIND_SERVICE_IND,
    OPENAT_BLE_FIND_CHARACTERISTIC_UUID_IND,
    OPENAT_BLE_READ_VALUE,
    OPENAT_BLE_RECV_DATA = 100,
}E_OPENAT_BT_EVENT;

typedef VOID (*F_BT_CB)(VOID* param);
void OPENAT_SetBLECallback(F_BT_CB handler);
BOOL OPENAT_OpenBT(E_OPENAT_BT_MODE mode);
BOOL OPENAT_CloseBT(void);
BOOL OPENAT_IotctlBT(E_OPENAT_BT_CMD cmd,U_OPENAT_BT_IOTCTL_PARAM param);
UINT8 OPENAT_GetVisibilityBT(void);
int8 OPENAT_GetAvrcpVolBT(void);
/*+\new\cxd\2022.3.4\֧�ֻ�ȡ���ֲ���״̬*/
UINT8 OPENAT_GetAvrcpStateBT(void);
/*-\new\cxd\2022.3.4\֧�ֻ�ȡ���ֲ���״̬*/

BOOL OPENAT_WriteBLE(UINT16 handle,T_OPENAT_BLE_UUID uuid,char *data,UINT8 len);
BOOL OPENAT_IotctlBLE(UINT16 handle,E_OPENAT_BT_CMD cmd,U_OPENAT_BT_IOTCTL_PARAM param);
BOOL OPENAT_DisconnectBLE(UINT16 handle);
BOOL OPENAT_ConnectBLE(UINT8 addr_type, char *addr);


#endif /* AM_OPENAT_FS_H */


