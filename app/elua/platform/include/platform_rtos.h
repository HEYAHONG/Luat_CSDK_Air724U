/**************************************************************************
 *              Copyright (C), AirM2M Tech. Co., Ltd.
 *
 * Name:    platform_rtos.h
 * Author:  liweiqiang
 * Version: V0.1
 * Date:    2013/3/7
 *
 * Description:
 *          luaƽ̨��rtos��ӿ�
 **************************************************************************/

#ifndef __PLATFORM_RTOS_H__
#define __PLATFORM_RTOS_H__

#include "am_openat.h"



typedef enum
{
  MSG_ID_GPS_DATA_IND,
  MSG_ID_RTOS_TOUCH,
  MSG_ID_OPENAT_GPS_OPEN_IND,
  MSG_ID_GPS_OPEN_IND,
  MSG_ID_RTOS_WAIT_MSG_TIMEOUT,
  MSG_ID_RTOS_TIMER,
  MSG_ID_RTOS_UART_RX_DATA,
  MSG_ID_RTOS_UART_TX_DONE,
  MSG_ID_RTOS_KEYPAD,
  MSG_ID_RTOS_INT,
  MSG_ID_RTOS_PMD,   //10
  MSG_ID_RTOS_AUDIO,
  MSG_ID_RTOS_WEAR_STATUS,
  MSG_ID_RTOS_RECORD,
  /*+\new\wj\2020.4.26\ʵ��¼���ӿ�*/
  MSG_ID_ROTS_STRAM_RECORD_IND, //14
  /*-\new\wj\2020.4.26\ʵ��¼���ӿ�*/
  MSG_ID_RTOS_ALARM,
  MSG_ID_APP_MTHL_ACTIVATE_PDP_CNF,
  MSG_ID_APP_MTHL_GET_HOST_BY_NAME_CNF,
  MSG_ID_APP_MTHL_CREATE_CONN_CNF,
  MSG_ID_APP_MTHL_CREATE_SOCK_IND,
  MSG_ID_APP_MTHL_SOCK_SEND_CNF,  //19
  MSG_ID_APP_MTHL_SOCK_SEND_IND,
  MSG_ID_APP_MTHL_SOCK_RECV_IND,
  MSG_ID_APP_MTHL_DEACTIVATE_PDP_CNF,
  MSG_ID_APP_MTHL_SOCK_CLOSE_CNF,
  MSG_ID_APP_MTHL_SOCK_CLOSE_IND,
  MSG_ID_APP_MTHL_DEACTIVATED_PDP_IND,
  MSG_ID_SIM_INSERT_IND,
  
  MSG_ID_TCPIP_PDP_ACTIVATE_CNF,
  MSG_ID_TCPIP_PDP_DEACTIVATE_CNF,
  MSG_ID_TCPIP_PDP_DEACTIVATE_IND,
  MSG_ID_TCPIP_SOCKET_RECV_IND, //30
  MSG_ID_TCPIP_SOCKET_SEND_CNF,  
  
  MSG_ID_TCPIP_SOCKET_CLOSE_CNF,
  
  MSG_ID_TCPIP_SOCKET_CONNECT_CNF,
  MSG_ID_TCPIP_SOCKET_SEND_IND,
  MSG_ID_TCPIP_SOCKET_CLOSE_IND,
/*+\new\wj\2019.12.27\���TTS����*/
  MSG_ID_RTOS_TTSPLY_STATUS,
  MSG_ID_RTOS_TTSPLY_ERROR,
/*-\new\wj\2019.12.27\���TTS����*/

/*+\NEW\zhuwangbin\2020.05.01\���disp camera����*/
  MSG_ID_RTOS_MSG_ZBAR,
/*-\NEW\zhuwangbin\2020.05.01\���disp camera����*/

  /*+\NEW\shenyuanyuan\2020.05.25\wifi.getinfo()�ӿڸĳ��첽������Ϣ�ķ�ʽ֪ͨLua�ű�*/
  MSG_ID_RTOS_MSG_WIFI,
  /*-\NEW\shenyuanyuan\2020.05.25\wifi.getinfo()�ӿڸĳ��첽������Ϣ�ķ�ʽ֪ͨLua�ű�*/

  MSG_ID_NULL,

  MSG_ID_MAX = 0x100,
  
}platform_msg_type;


/*+\NEW\liweiqiang\2013.12.12\���ӳ�翪��ʱ���û����о����Ƿ�����ϵͳ */
typedef enum PlatformPoweronReasonTag
{
    PLATFORM_POWERON_KEY,
    PLATFORM_POWERON_CHARGER,
    PLATFORM_POWERON_ALARM,
    PLATFORM_POWERON_RESTART,
    PLATFORM_POWERON_OTHER,
    PLATFORM_POWERON_UNKNOWN,
    /*+\NewReq NEW\zhuth\2014.6.18\���ӿ���ԭ��ֵ�ӿ�*/
    PLATFORM_POWERON_EXCEPTION,
    PLATFORM_POWERON_HOST,
    PLATFORM_POWERON_WATCHDOG,
    /*-\NewReq NEW\zhuth\2014.6.18\���ӿ���ԭ��ֵ�ӿ�*/

}PlatformPoweronReason;    
/*-\NEW\liweiqiang\2013.12.12\���ӳ�翪��ʱ���û����о����Ƿ�����ϵͳ */

// ��ʼ����ģ��ID
typedef enum PlatformRtosModuleTag
{
    RTOS_MODULE_ID_KEYPAD,
	/*+\NEW\rufei\2015.3.13\����������Ϣ */
    RTOS_MODULE_ID_ALARM,
	/*-\NEW\rufei\2015.3.13\����������Ϣ */

    RTOS_MODULE_ID_TOUCH,
    // touch screen...

    NumOfRTOSModules
}PlatformRtosModule;

typedef struct KeypadMatrixDataTag
{
    unsigned char       row;
    unsigned char       col;
}KeypadMatrixData;

typedef struct KeypadMsgDataTag
{
    u8   type;  // keypad type
    BOOL bPressed; /* �Ƿ��ǰ�����Ϣ */
    union {
        struct {
            u8 row;
            u8 col;
        }matrix, gpio;
        u16 adc;
    }data;
}KeypadMsgData;

typedef struct TouchMsgDataTag
{
    UINT8   type;  // touch action type
    UINT16   x;
    UINT16   y;
}TouchMsgData;

/*+\NEW\liweiqiang\2013.4.5\����lua gpio �ж�����*/
typedef struct PlatformIntDataTag
{
    elua_int_id             id;
    elua_int_resnum         resnum;
}PlatformIntData;
/*-\NEW\liweiqiang\2013.4.5\����lua gpio �ж�����*/

/*+\NEW\liweiqiang\2013.7.8\����rtos.pmd��Ϣ*/
#define PLATFORM_BATT_NOT_CHARGING      0
#define PLATFORM_BATT_CHARING           1
#define PLATFORM_BATT_CHARGE_STOP       2

typedef struct PlatformPmdDataTag
{
    BOOL    battStatus;
    BOOL    chargerStatus;
    u8      chargeState;
    u8      battLevel;
    u16     battVolt;
}PlatformPmdData;
/*-\NEW\liweiqiang\2013.7.8\����rtos.pmd��Ϣ*/

/*+\NEW\liweiqiang\2013.11.4\����audio.core�ӿڿ� */
typedef struct PlatformAudioDataTag
{
    BOOL    playEndInd;
    BOOL    playErrorInd;
}PlatformAudioData;
/*-\NEW\liweiqiang\2013.11.4\����audio.core�ӿڿ� */

typedef struct PlatformRecordDataTag
{
    BOOL    recordEndInd;
    BOOL    recordErrorInd;
}PlatformRecordData;

/*+\NEW\zhuwangbin\2020.05.01\���disp camera����*/
typedef struct PlatformZbarDataTag
{
    BOOL    result;
    u8 *pType;
    u8 *pData;
}PlatformZbarData;
/*-\NEW\zhuwangbin\2020.05.01\���disp camera����*/

/*+\NEW\shenyuanyuan\2020.05.25\wifi.getinfo()�ӿڸĳ��첽������Ϣ�ķ�ʽ֪ͨLua�ű�*/
typedef struct PlatformWifiDataTag
{
    UINT32    num;
    CHAR* pData;
}PlatformWifiData;
/*-\NEW\shenyuanyuan\2020.05.25\wifi.getinfo()�ӿڸĳ��첽������Ϣ�ķ�ʽ֪ͨLua�ű�*/

typedef struct PlatformWearStatusDataTag
{
    BOOL    wearStatus;
}PlatformWearStatusData;
/*+\new\wj\2019.12.27\���TTS����*/
//#if defined(__AM_LUA_TTSPLY_SUPPORT__)
typedef struct tagPlatformTtsPlyData
{
    s32    ttsPlyStatusInd;
    s32    ttsPlyErrorInd;
}PlatformTtsPlyData;
//#endif //__AM_LUA_TTSPLY_SUPPORT__
/*-\new\wj\2019.12.27\���TTS����*/
#define PLATFORM_RTOS_WAIT_MSG_INFINITE         (OPENAT_OS_SUSPENDED)

#if 0
typedef enum PlatformMsgIdTag
{
    RTOS_MSG_WAIT_MSG_TIMEOUT, // receive message timeout
    RTOS_MSG_TIMER,
    RTOS_MSG_UART_RX_DATA,
    RTOS_MSG_KEYPAD,
/*+\NEW\liweiqiang\2013.4.5\����lua gpio �ж�����*/
    RTOS_MSG_INT,             
/*-\NEW\liweiqiang\2013.4.5\����lua gpio �ж�����*/
/*+\NEW\liweiqiang\2013.7.8\����rtos.pmd��Ϣ*/
    RTOS_MSG_PMD,
/*-\NEW\liweiqiang\2013.7.8\����rtos.pmd��Ϣ*/
/*+\NEW\liweiqiang\2013.11.4\����audio.core�ӿڿ� */
    RTOS_MSG_AUDIO,
/*-\NEW\liweiqiang\2013.11.4\����audio.core�ӿڿ� */

    NumOfMsgIds
}PlatformMsgId;
#endif


typedef struct PlatformGpsDataTag
{
    UINT16 dataMode;
    UINT16 dataLen;
}PlatformGpsData;

typedef struct 
{
    UINT8 success; //0->OK 
}PlatformGpsOpenInd;

typedef struct 
{
    UINT8 inserted;
}PlatformRemSimInsertInd;

typedef struct
{
  UINT8 socket_index;
  BOOL  result;
}PlatformSocketConnectCnf;

typedef struct
{
  UINT8 socket_index;
  INT32 recv_len;
}PlatformSocketRecvInd;

typedef struct
{
  UINT8 socket_index;
  BOOL  result;
}PlatformSocketCloseCnf, PlatformSocketCloseInd;

typedef struct
{
  BOOL  result;
  UINT32 errorCause;
}PlatformPdpActiveCnf;

typedef struct
{
  BOOL  result;
  UINT8 socket_index;
  UINT32 length;
}PlatformSocketSendCnf;



typedef union PlatformMsgDataTag
{
    int                 timer_id;
    int                 uart_id;
    KeypadMsgData       keypadMsgData;
    TouchMsgData        touchMsgData;
/*+\NEW\liweiqiang\2013.4.5\����lua gpio �ж�����*/
    PlatformIntData     interruptData;
/*-\NEW\liweiqiang\2013.4.5\����lua gpio �ж�����*/
/*+\NEW\liweiqiang\2013.7.8\����rtos.pmd��Ϣ*/
    PlatformPmdData     pmdData;
/*-\NEW\liweiqiang\2013.7.8\����rtos.pmd��Ϣ*/
/*+\NEW\liweiqiang\2013.11.4\����audio.core�ӿڿ� */
    PlatformAudioData   audioData;
/*-\NEW\liweiqiang\2013.11.4\����audio.core�ӿڿ� */

    PlatformRecordData recordData;
	/*+\new\wj\2020.4.26\ʵ��¼���ӿ�*/
	int streamRecordLen;
	/*-\new\wj\2020.4.26\ʵ��¼���ӿ�*/
    PlatformWearStatusData wearStatusData;
/*+\new\wj\2019.12.27\���TTS����*/
    //#if defined(__AM_LUA_TTSPLY_SUPPORT__)
    PlatformTtsPlyData ttsPlyData;
    //#endif //__AM_LUA_TTSPLY_SUPPORT__
/*-\new\wj\2019.12.27\���TTS����*/
    PlatformGpsData    gpsData;
    PlatformGpsOpenInd gpsOpenInd;
    PlatformRemSimInsertInd remSimInsertInd;
    PlatformSocketConnectCnf socketConnectCnf;
    PlatformSocketRecvInd   socketRecvInd;
    PlatformSocketCloseCnf  socketCloseCnf;
    PlatformSocketCloseInd  socketCloseInd;
    PlatformPdpActiveCnf    pdpActiveCnf;
    PlatformSocketSendCnf   socketSendCnf;
	
	/*+\NEW\zhuwangbin\2020.05.01\���disp camera����*/
    PlatformZbarData      zbarData;
	/*-\NEW\zhuwangbin\2020.05.01\���disp camera����*/
	/*+\NEW\shenyuanyuan\2020.05.25\wifi.getinfo()�ӿڸĳ��첽������Ϣ�ķ�ʽ֪ͨLua�ű�*/
	PlatformWifiData      wifiData;
	/*-\NEW\shenyuanyuan\2020.05.25\wifi.getinfo()�ӿڸĳ��첽������Ϣ�ķ�ʽ֪ͨLua�ű�*/
}PlatformMsgData;

/*����Ҫ������ôһ���ṹ����LOCAL_PARA_HDR����Ϊconstruct_local_paraʱ���Ǳ���Ҫ����LOCAL_PARA_HDR��*/
typedef struct  LOCAL_PARAM_STRUCT_TAG
{
    //LOCAL_PARA_HDR
    PlatformMsgData msgData;         
}LOCAL_PARAM_STRUCT;


typedef struct PlatformKeypadInitParamTag
{
    int type;
    struct{
        int inMask;         /* active key in mask */
        int outMask;        /* active key out mask */        
    }matrix;
}PlatformKeypadInitParam;

/*+\NEW\rufei\2015.3.13\����������Ϣ */
typedef struct PlatformSetAlarmParamTag
{
    BOOL alarmon;
    u16 year;
    u8 month;
    u8 day;
    u8 hour;
    u8 min;
    u8 sec;
}PlatformSetAlarmParam;
/*+\NEW\zhuwangbin\2019.12.10\lua �汾���벻��*/
#undef ASSERT
#define ASSERT(cOND)	{if (!(cOND)) {platform_assert(__FILE__, (short)__LINE__);}}
/*-\NEW\zhuwangbin\2019.12.10\lua �汾���벻��*/
/*-\NEW\rufei\2015.3.13\����������Ϣ */
int platform_rtos_init(void);

int platform_rtos_poweroff(void);

/*+\NEW\liweiqiang\2013.9.7\����rtos.restart�ӿ�*/
int platform_rtos_restart(void);
/*-\NEW\liweiqiang\2013.9.7\����rtos.restart�ӿ�*/

int platform_rtos_init_module(int module, void *pParam);

int platform_rtos_receive(platform_msg_type* msg_id, void **ppMessage, u32 timeout);

void platform_rtos_free_msg(void *msg_body);

int platform_rtos_send(platform_msg_type msg_id,  PlatformMsgData * pMsg);

int platform_rtos_start_timer(int timer_id, int milliSecond);

int platform_rtos_stop_timer(int timer_id);

/*+\NEW\liweiqiang\2013.4.5\����rtos.tick�ӿ�*/
int platform_rtos_tick(void);
/*-\NEW\liweiqiang\2013.4.5\����rtos.tick�ӿ�*/

/*+\NEW\liweiqiang\2013.12.12\���ӳ�翪��ʱ���û����о����Ƿ�����ϵͳ */
int platform_get_poweron_reason(void);

int platform_rtos_poweron(int flag);

void platform_poweron_try(void);
/*-\NEW\liweiqiang\2013.12.12\���ӳ�翪��ʱ���û����о����Ƿ�����ϵͳ */
/*+\NEW\rufei\2015.3.13\����������Ϣ */
int platform_rtos_setalarm(void *pParam);
/*-\NEW\rufei\2015.3.13\����������Ϣ */

// +panjun, 2015.08.26, Simplify MMI's frame for Video.
int platform_rtos_repoweron(void);
// -panjun, 2015.08.26, Simplify MMI's frame for Video.
char*  platform_base64_encode(const char *str, int length);

extern void LuaChargingPwnOnThanPwnOn(void);

/*+\NEW\brezen\2016.4.25\����base64�ӿ�*/  
char* platform_base64_decode (const char *src, int length,int* decodedLen);
/*-\NEW\brezen\2016.4.25\����base64�ӿ�*/
//+panjun,160503,Add an API "rtos.disk_free".
long platform_rtos_disk_free(int drvtype);
int platform_rtos_disk_volume(int drvtype);
//-panjun,160503,Add an API "rtos.disk_free".


int platform_make_dir(char *pDir, int len);
int platform_remove_dir(char *pDir, int len);
void platform_rtos_set_time(u32 timestamp);

int platform_rtos_fota_start(void);
int platform_rtos_fota_process(const char* data, int len, int total);
int platform_rtos_fota_end(void);
BOOL platform_rtos_set_trace_port(u8 port, u8 usb_port_diag_output);
u8 platform_rtos_get_trace_port(void);
/*+\NEW\shenyuanyuan\2019.4.19\����AT+TRANSDATA����*/
int platform_rtos_sendok(char *src);
/*-\NEW\shenyuanyuan\2019.4.19\����AT+TRANSDATA����*/

#endif //__PLATFORM_RTOS_H__

