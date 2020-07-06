#ifndef  __IOT_VAT_H__
#define  __IOT_VAT_H__

#include "am_openat_vat.h"
#include "iot_network.h"

#define URC_QUEUE_COUNT 10                      /* global urc count */
#define AT_CMD_QUEUE_COUNT 50
#define AT_CMD_EXECUTE_DELAY  10                /* 10 ms */

#define AT_CMD_DELAY "DELAY:"
#define AT_CMD_END "\x0d\x0a"
#define AT_CMD_CR  '\x0d'
#define AT_CMD_LF  '\x0a'
#define STR_TO_INT(x) 	(x-'0') 					/*���ֵ�charת��Ϊint*/


typedef enum
{
    AT_RSP_ERROR = -1,
    AT_RSP_WAIT= 0, 
    AT_RSP_CONTINUE = 1,                        /* ����ִ����һ��AT����������� */
    AT_RSP_PAUSE= 2,                            /* ��ִͣ��AT�������� */
    AT_RSP_FINISH = 3,                          /* ִֹͣ��AT�������� */

    AT_RSP_FUN_OVER = 4,                        /* ����ģ�黯AT������ִ����ϣ����԰ѱ������еĹ������ */
    AT_RSP_STEP_MIN = 10,
    AT_RSP_STEP = 20,                           /* ����ִ�б���AT���� */
    AT_RSP_STEP_MAX = 30,

}AtCmdRsp;

typedef AtCmdRsp (*AtCmdRspCB)(char *pRspStr);
typedef VOID (*UrcCB)(char *pUrcStr, u16 len);
typedef VOID (*ResultNotifyCb)(BOOL result);

typedef struct AtCmdEntityTag
{
    char* p_atCmdStr;				/*AT�����ַ���*/
    u16 cmdLen;					/*AT�����*/
    AtCmdRspCB p_atCmdCallBack;	/*AT����ص�����*/
}AtCmdEntity;

typedef struct UrcEntityTag
{
    char* p_urcStr;
    UrcCB p_urcCallBack;

}UrcEntity;

typedef struct _CELL_INFO
{
	u32 CellId;  //cell ID
	u32 Lac;  //LAC
	u16 Mcc;  //MCC
	u16 Mnc;  //MNC
	u16 rssi; //rssi
}CELL_INFO;

typedef struct _gsmloc_cellinfo
{
	CELL_INFO Cellinfo[6];
}gsmloc_cellinfo;

/**
 * @defgroup iot_sdk_sys ϵͳ�ӿ�
 * @{
 */
/**@example vat/demo_vat.c
* vat�ӿ�ʾ��
*/ 
/**������������ATͨ���Ļص�����
*@param		vatHandle:  ����AT�����ϱ�����AT���������صĻص�����
*@return	TRUE: �ɹ�   FALSE: ʧ��
**/
BOOL iot_vat_init(PAT_MESSAGE vatHandle);

/**��������AT����
*@param		cmdStr:  AT�����ַ���
*@param   	cmdLen:  AT�����
*@return	TRUE: �ɹ�   FALSE: ʧ��
*@note      ע�⣬AT�����ַ���cmdStr����Ҫ����"\r\n"����"\r"��β
**/
BOOL iot_vat_send_cmd(UINT8* cmdStr, UINT16 cmdLen);

/**������������AT����
*@param		cmd:  AT�������
*@param   	cmd_count:  AT�������
*@return	TRUE: �ɹ�   FALSE: ʧ��
**/
BOOL iot_vat_push_cmd(AtCmdEntity cmd[],u8 cmd_count);


/** @}*/


#endif

