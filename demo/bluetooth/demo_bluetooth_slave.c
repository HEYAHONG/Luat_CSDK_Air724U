/***************
	demo_bluetooth
****************/


#include "string.h"
#include "iot_debug.h"
#include "iot_bluetooth.h"

#define BT_8910_TP_UUID 0xFEE0
#define BT_8910_TP_UUID_CHAR 0xFEE1
#define BT_8910_FEEDBACK_CHAR 0xFEE2

//GATT Characteristic attribute
#define ATT_CHARA_PROP_BROADCAST           0x01
#define ATT_CHARA_PROP_READ         0x02
#define ATT_CHARA_PROP_WWP          0x04    // WWP short for "write without response"
#define ATT_CHARA_PROP_WRITE               0x08
#define ATT_CHARA_PROP_NOTIFY              0x10
#define ATT_CHARA_PROP_INDICATE     0x20
#define ATT_CHARA_PROP_ASW          0x40    // ASW short for "Authenticated signed write"
#define ATT_CHARA_PROP_EX_PROP      0x80

#define ATT_PM_READABLE                    0x0001
#define ATT_PM_WRITEABLE                   0x0002
#define ATT_PM_R_AUTHENT_REQUIRED          0x0004
#define ATT_PM_R_AUTHORIZE_REQUIRED        0x0008
#define ATT_PM_R_ENCRYPTION_REQUIRED       0x0010
#define ATT_PM_R_AUTHENT_MITM_REQUERED     0x0020
#define ATT_PM_W_AUTHENT_REQUIRED          0x0040
#define ATT_PM_W_AUTHORIZE_REQUIRED        0x0080
#define ATT_PM_W_ENCRYPTION_REQUIRED       0x0100
#define ATT_PM_W_AUTHENT_MITM_REQUERED     0x0200
#define ATT_PM_BR_ACCESS_ONLY              0x0400

//GATT Characteristic Descriptors
#define ATT_UUID_CHAR_EXT           0x2900
#define ATT_UUID_CHAR_USER      0x2901
#define ATT_UUID_CLIENT         0x2902
#define ATT_UUID_SERVER         0x2903
#define ATT_UUID_CHAR_FORMAT        0x2904
#define ATT_UUID_CHAR_AGGREGATE 0x2905

HANDLE ble_test_handle = NULL;

typedef struct ble_report_info
{
    unsigned char  eventid;
    char    state;
    UINT8   bleRcvBuffer[BLE_MAX_DATA_COUNT];
    UINT8    len;
    UINT16 uuid;
    UINT16 handle;
    UINT8 long_uuid[BLE_LONG_UUID_FLAG];
    UINT8 uuid_flag;
} ble_report_info_t;

typedef struct ble_add_characteristic
{
    T_OPENAT_BLE_CHARACTERISTIC_PARAM uuid_c; //����uuid  
    T_OPENAT_BLE_DESCRIPTOR_PARAM *uuid_d; //��������
    UINT8 count;//��������
} ble_add_characteristic_t;



UINT16 connect_handle = 0xff;//���Ӿ��

static void AppConvertBinToHex(
    UINT8 *bin_ptr, // in: the binary format string
    UINT16 length,  // in: the length of binary string
    UINT8 *hex_ptr  // out: pointer to the hexadecimal format string
)
{
    UINT8 semi_octet = 0;
    int32 i = 0;

    for (i = 0; i < length; i++)
    {
        // get the high 4 bits
        semi_octet = (UINT8)((bin_ptr[i] & 0xF0) >> 4);
        if (semi_octet <= 9) //semi_octet >= 0
        {
            *hex_ptr = (UINT8)(semi_octet + '0');
        }
        else
        {
            *hex_ptr = (UINT8)(semi_octet + 'A' - 10);
        }

        hex_ptr++;

        // get the low 4 bits
        semi_octet = (UINT8)(bin_ptr[i] & 0x0f);
        if (semi_octet <= 9) // semi_octet >= 0
        {
            *hex_ptr = (UINT8)(semi_octet + '0');
        }
        else
        {
            *hex_ptr = (UINT8)(semi_octet + 'A' - 10);
        }
        hex_ptr++;
    }
}

void bluetooth_callback(T_OPENAT_BLE_EVENT_PARAM *result)
{
    ble_report_info_t *msg = NULL;
    msg = iot_os_malloc(sizeof(ble_report_info_t));
	msg->eventid = result->id;
	msg->state  =  result->state;
	switch(result->id)
	{
		case OPENAT_BLE_RECV_DATA:
			if((result->len != 0) && (result->dataPtr != NULL))
			{
                msg->len  =  result->len;
                msg->uuid_flag  =  result->uuid_flag;
                msg->uuid  =  result->uuid;
                memcpy(msg->long_uuid, result->long_uuid, sizeof(result->long_uuid));
                memset(msg->bleRcvBuffer,0,sizeof(msg->bleRcvBuffer));
				memcpy(msg->bleRcvBuffer, result->dataPtr,result->len);  
                iot_os_send_message(ble_test_handle,msg);
			}
			break;
        case OPENAT_BLE_CONNECT_IND:
            msg->handle  =  result->handle;
            iot_os_send_message(ble_test_handle,msg);
            break;
        case OPENAT_BLE_DISCONNECT_IND:
            iot_debug_print("[bluetooth]bt disconnect");
            iot_os_free(msg);
            break;
        case OPENAT_BT_ME_ON_CNF:
            iot_os_send_message(ble_test_handle,msg);
		    break;
		default:
            iot_os_free(msg);
		    break;
	}
}

BOOL ble_poweron(VOID)
{
    iot_debug_print("[bluetooth]bt poweron");
    iot_bt_open(BLE_SLAVE);//������
    return TRUE;
}
/*����ӷ�������������������������°����������������������������������*/
BOOL AddService()
{
    int i,j;
    U_OPENAT_BT_IOTCTL_PARAM  param1 = {0};
    U_OPENAT_BT_IOTCTL_PARAM  param2 = {0};
    U_OPENAT_BT_IOTCTL_PARAM  param3 = {0};
    T_OPENAT_BLE_UUID uuid = {0};
    T_OPENAT_BLE_CHARACTERISTIC_PARAM uuid_c1 = {0};
    T_OPENAT_BLE_CHARACTERISTIC_PARAM uuid_c2 = {0};
    T_OPENAT_BLE_DESCRIPTOR_PARAM uuid_d1 = {0};
    T_OPENAT_BLE_DESCRIPTOR_PARAM uuid_d2 = {0};
    uuid.uuid_short = BT_8910_TP_UUID;//����uuid
    uuid.uuid_type = UUID_SHORT;
    uuid_c1.uuid.uuid_short = BT_8910_TP_UUID_CHAR;//����uuid
    uuid_c1.uuid.uuid_type = UUID_SHORT;
    uuid_c1.attvalue = ATT_CHARA_PROP_READ | ATT_CHARA_PROP_WRITE;//��������
    uuid_c1.permisssion = ATT_PM_READABLE|ATT_PM_WRITEABLE;//����Ȩ��
    uuid_c2.uuid.uuid_short = BT_8910_FEEDBACK_CHAR;//����uuid
    uuid_c2.uuid.uuid_type = UUID_SHORT;
    uuid_c2.attvalue = ATT_CHARA_PROP_READ | ATT_CHARA_PROP_NOTIFY | ATT_CHARA_PROP_INDICATE;//��������
    uuid_c2.permisssion = ATT_PM_READABLE;//����Ȩ��
    
    T_OPENAT_BLE_DESCRIPTOR_PARAM bt_descriptor[2] = {0};//��������
    bt_descriptor[0].uuid.uuid_short = ATT_UUID_CHAR_USER;//����uuid
    bt_descriptor[0].uuid.uuid_type = UUID_SHORT;
    memcpy(bt_descriptor[0].value,"123456789",strlen("123456789"));//��������
    bt_descriptor[1].uuid.uuid_short = ATT_UUID_CLIENT;//����uuid
    bt_descriptor[1].uuid.uuid_type = UUID_SHORT;
    bt_descriptor[1].configurationBits = 1;//��������
    ble_add_characteristic_t service_param[2] = {{uuid_c1,NULL,0},
                                        {uuid_c2,bt_descriptor,2}}; 

    param1.uuid = iot_os_malloc(sizeof(T_OPENAT_BLE_UUID));
    memcpy(param1.uuid,&uuid,sizeof(T_OPENAT_BLE_UUID));
    iot_ble_iotctl(0,BLE_ADD_SERVICE,param1);//��ӷ���
    if(param1.uuid != NULL)
        iot_os_free(param1.uuid);
    param1.data = NULL;
    for(i = 0;i < sizeof(service_param)/sizeof(ble_add_characteristic_t);i ++)
    {
        param2.characteristicparam = iot_os_malloc(sizeof(T_OPENAT_BLE_CHARACTERISTIC_PARAM));
        memcpy(param2.characteristicparam,&service_param[i].uuid_c,sizeof(T_OPENAT_BLE_CHARACTERISTIC_PARAM));
        iot_ble_iotctl(0,BLE_ADD_CHARACTERISTIC,param2);//�������
        if(param2.characteristicparam != NULL)
            iot_os_free(param2.characteristicparam);
        param2.characteristicparam = NULL;
        if(service_param[i].count != 0)
        {
            for(j = 0;j < service_param[i].count;j ++)
            {
                param3.descriptorparam = iot_os_malloc(sizeof(T_OPENAT_BLE_DESCRIPTOR_PARAM));
                memcpy(param3.descriptorparam,&bt_descriptor[j],sizeof(T_OPENAT_BLE_DESCRIPTOR_PARAM));
                iot_ble_iotctl(0,BLE_ADD_DESCRIPTOR,param3);//�������
                if(param3.descriptorparam != NULL)
                    iot_os_free(param3.descriptorparam);
                param3.descriptorparam = NULL;
            }
        }
    }

    return TRUE;
}

BOOL advertising(VOID)
{
    U_OPENAT_BT_IOTCTL_PARAM  param1;
    //U_OPENAT_BT_IOTCTL_PARAM  param2;
    //U_OPENAT_BT_IOTCTL_PARAM  param3;
    //U_OPENAT_BT_IOTCTL_PARAM  param4;
    U_OPENAT_BT_IOTCTL_PARAM  param5;
    //T_OPENAT_BLE_ADV_DATA advdata;
    //T_OPENAT_BLE_ADV_DATA scanrspdata;
    //UINT8 data1[BLE_MAX_ADV_MUBER] = {0x02,0x01,0x06,0x04,0xff,0x01,0x02,0x03};//�㲥������
    //UINT8 data2[BLE_MAX_ADV_MUBER] = {0x02,0x0a,0x04};//��Ӧ������
    //memcpy(advdata.data,data1,BLE_MAX_ADV_MUBER);
    //advdata.len = strlen(data1);
    //memcpy(scanrspdata.data,data2,BLE_MAX_ADV_MUBER);
    //scanrspdata.len = strlen(data2);
    //T_OPENAT_BLE_ADV_PARAM advparam = {0x80,0xa0,0,0,0,"11:22:33:44:55:66",0x07,0};//�㲥����
    iot_debug_print("[bluetooth]bt advertising");
    param1.data = iot_os_malloc(strlen("Luat_Air724UG"));
    memcpy(param1.data,"Luat_Air724UG",strlen("Luat_Air724UG"));
    iot_ble_iotctl(0,BLE_SET_NAME,param1);//���ù㲥����
    if(param1.data != NULL)
        iot_os_free(param1.data);
    param1.data = NULL;
/*
    param2.advdata = iot_os_malloc(sizeof(T_OPENAT_BLE_ADV_DATA));
    memcpy(param2.advdata,&advdata,sizeof(T_OPENAT_BLE_ADV_DATA));
    iot_ble_iotctl(0,BLE_SET_ADV_DATA,param2);//���ù㲥������
    if(param2.advdata != NULL)
        iot_os_free(param2.advdata);
    param2.advdata = NULL;

    param3.advdata = iot_os_malloc(sizeof(T_OPENAT_BLE_ADV_DATA));
    memcpy(param3.advdata,&scanrspdata,sizeof(T_OPENAT_BLE_ADV_DATA));
    iot_ble_iotctl(0,BLE_SET_SCANRSP_DATA,param3);//������Ӧ������
    if(param3.advdata != NULL)
        iot_os_free(param3.advdata);
    param3.advdata = NULL;

    //AddService();//����Զ�����������

    param4.advparam = iot_os_malloc(sizeof(T_OPENAT_BLE_ADV_PARAM));
    memcpy(param4.advparam,&advparam,sizeof(T_OPENAT_BLE_ADV_PARAM));
    iot_ble_iotctl(0,BLE_SET_ADV_PARAM,param4);//���ù㲥����
    if(param4.advparam != NULL)
        iot_os_free(param4.advparam);
    param4.advparam = NULL;
*/
    iot_os_sleep(1000);
    param5.advEnable = 1;
    iot_ble_iotctl(0,BLE_SET_ADV_ENABLE,param5);//�򿪹㲥  
    return TRUE;
}

BOOL ble_data_trans(VOID)
{
    ble_report_info_t *msg = NULL;
    T_OPENAT_BLE_UUID uuid = {0};
    uuid.uuid_short = BT_8910_FEEDBACK_CHAR;
    uuid.uuid_type = UUID_SHORT;
    char *bleRcvBuffer = NULL;
    /*���ӳɹ�*/
    while(1)
    {
        
        iot_os_wait_message(ble_test_handle,&msg);//�ȴ����յ�����
        if(msg->eventid == OPENAT_BLE_RECV_DATA)
        {
            bleRcvBuffer = iot_os_malloc(BLE_MAX_DATA_COUNT*2+1);                                                 
            iot_debug_print("[bluetooth]bt recv uuid %x",msg->uuid);
            iot_debug_print("[bluetooth]bt recv data len %d",msg->len);

            AppConvertBinToHex(msg->bleRcvBuffer,msg->len,bleRcvBuffer);
            bleRcvBuffer[msg->len*2] = '\0';
            iot_debug_print("[bluetooth]bt recv data %s",bleRcvBuffer);

            iot_ble_write(connect_handle,uuid,msg->bleRcvBuffer,msg->len);  
            //test �����յ�"close"ʱ�����Ͽ���������
            if(memcmp(msg->bleRcvBuffer,"close",sizeof("close")) == 0)
            {
                iot_ble_disconnect(connect_handle);
            }
            if(bleRcvBuffer != NULL)
                iot_os_free(bleRcvBuffer);
            bleRcvBuffer = NULL;
            if(msg != NULL)
                iot_os_free(msg);
            msg = NULL;
        }
    }
    return TRUE;
}

VOID ble_test(VOID)
{
    ble_report_info_t *msg = NULL;
    //1.  ������
    ble_poweron();
    iot_os_wait_message(ble_test_handle,&msg);//�ȴ�������
    if(msg->eventid == OPENAT_BT_ME_ON_CNF)
    {
        if(msg != NULL)
            iot_os_free(msg);
        msg = NULL;
        //2.�㲥����
        advertising();
        iot_os_wait_message(ble_test_handle,&msg);//�ȴ����ӳɹ�
        if(msg->eventid == OPENAT_BLE_CONNECT_IND)
        {     
            //3. ���ݴ���
            connect_handle = msg->handle;//���Ӿ��
            if(msg != NULL)
                iot_os_free(msg);
            msg = NULL;
            ble_data_trans();
        }
    }
}


int appimg_enter(void *param)
{
    iot_debug_print("[bluetooth]appimg_enter");
    ble_test_handle = iot_os_create_task(ble_test, NULL, 4096, 1, OPENAT_OS_CREATE_DEFAULT, "bluetooth");
    return 0;
}

void appimg_exit(void)
{
    iot_debug_print("[bluetooth]appimg_exit");
}
