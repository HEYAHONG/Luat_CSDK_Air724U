#include "iot_flash.h"
#include "string.h"
#include "iot_debug.h"
#include "iot_vat.h"
#include "iot_network.h"


BOOL  simpresent = FALSE;
static F_OPENAT_NETWORK_IND_CB g_s_OpenatNetIndCb = NULL;
E_OPENAT_NETWORK_STATE network_state = 0;
VOID networkStatusChange(VOID);

int CsqValue = 0;
static AtCmdRsp AtCmdCb_csqval(char* pRspStr)
{
	iot_debug_print("[vat]AtCmdCb_csqval");
    AtCmdRsp  rspValue = AT_RSP_WAIT;
    char *rspStrTable[] = {"+CME","+CSQ: ", "OK"};
    s16  rspType = -1;
    u8  i = 0;
    char  *p = pRspStr + 2;
    for (i = 0; i < sizeof(rspStrTable) / sizeof(rspStrTable[0]); i++)
    {
        if (!strncmp(rspStrTable[i], p, strlen(rspStrTable[i])))
        {
            rspType = i;
            if (rspType == 1){
				if(p[strlen(rspStrTable[rspType])+1] == ',')
				{
			  		CsqValue = STR_TO_INT(p[strlen(rspStrTable[rspType])]);
				}else
				{
					CsqValue = STR_TO_INT(p[strlen(rspStrTable[rspType])])*10 + STR_TO_INT(p[strlen(rspStrTable[rspType])+1]);
				}
            }
            break;
        }
    }
    switch (rspType)
    {
        case 0:  /* ERROR */
        rspValue = AT_RSP_ERROR;
        break;

        case 1:  /* +CSQ */
		rspValue  = AT_RSP_WAIT;
        break;
		
		case 2:  /* OK */
		rspValue  = AT_RSP_CONTINUE;
		break;

        default:
        break;
    }
    return rspValue;
}

static int CregValue = 0;
static AtCmdRsp AtCmdCb_creg(char* pRspStr)
{
	iot_debug_print("[vat]AtCmdCb_creg");
    AtCmdRsp  rspValue = AT_RSP_WAIT;
    char *rspStrTable[] = {"ERROR", "+CREG: ", "OK"};
    s16  rspType = -1;
    u8  i = 0;
    char  *p = pRspStr + 2;
	
    for (i = 0; i < sizeof(rspStrTable) / sizeof(rspStrTable[0]); i++)
    {
        if (!strncmp(rspStrTable[i], p, strlen(rspStrTable[i])))
        {
            rspType = i;
			if(1 == rspType)
			{
 	  	    	CregValue = STR_TO_INT(p[strlen(rspStrTable[rspType])+2]);
				iot_debug_print("[vat] CregValue %d",CregValue);
			}
            break;
        }
    }
	iot_debug_print("[vat] rspType %d",rspType);
    switch (rspType)
    {
        case 0:
        rspValue = AT_RSP_ERROR;
        break;

        case 1:
		rspValue  = AT_RSP_WAIT;
        break;

        case 2:
		if(1 == CregValue || 5 == CregValue)
		{
			network_state = OPENAT_NETWORK_READY;
			networkStatusChange();
        	rspValue  = AT_RSP_CONTINUE;
		}
		else
		{
			rspValue  = AT_RSP_STEP - 1;
		}
        break;

        default:
        break;
    }
    return rspValue;
}

static AtCmdRsp AtCmdCb_cgact_set(char* pRspStr)
{
    AtCmdRsp  rspValue = AT_RSP_WAIT;
    char *rspStrTable[] = {"ERROR", "+CME", "OK", "+CGACT"};
    s16  rspType = -1;
    u8  i = 0;
    char  *p = pRspStr + 2;
	
    for (i = 0; i < sizeof(rspStrTable) / sizeof(rspStrTable[0]); i++)
    {
        if (!strncmp(rspStrTable[i], p, strlen(rspStrTable[i])))
        {
            rspType = i;
            break;
        }
    }
	iot_debug_print("[vat] rspType %d",rspType);
    switch (rspType)
    {
        case 0:
		case 1:
        case 2:
		rspValue = AT_RSP_CONTINUE;
		break;

        break;

        default:
        break;
    }
    return rspValue;
}

static UINT8 CgactValue = 0;
static AtCmdRsp AtCmdCb_cgact_read(char* pRspStr)
{
    AtCmdRsp  rspValue = AT_RSP_WAIT;
    char *rspStrTable[] = {"\r\nERROR", "+CGACT: 6, ", "\r\nOK"};
    s16  rspType = -1;
    u8  i = 0;
    char  *p = pRspStr;
    for (i = 0; i < sizeof(rspStrTable) / sizeof(rspStrTable[0]); i++)
    {
        if (!strncmp(rspStrTable[i], p, strlen(rspStrTable[i])))
        {
            rspType = i;
			if(rspType == 1)
				CgactValue = STR_TO_INT(p[strlen(rspStrTable[rspType])]);
            break;
        }
    }
	iot_debug_print("[vat] rspType %d",rspType);
    switch (rspType)
    {
        case 0:
		rspValue = AT_RSP_STEP - 3;
        break;
		
		case 1:
        rspValue = AT_RSP_WAIT;
        break;

        case 2:
		iot_debug_print("[vat] CgactValue %d",CgactValue);
		if(CgactValue)
		{
			network_state = OPENAT_NETWORK_LINKED;
			networkStatusChange();
			rspValue = AT_RSP_FUN_OVER;
		}
		else
		{
		 	rspValue = AT_RSP_STEP - 3;
		}
        break;

        default:
        break;
    }
    return rspValue;
}


static char pAtApn[80] = {0};
static BOOL network_connect(T_OPENAT_NETWORK_CONNECT* connectParam)
{
	BOOL result = FALSE;
	network_state = OPENAT_NETWORK_LINKING;
	networkStatusChange();
	memset(pAtApn, 0, 80);
	if(strlen(connectParam->apn))
	{
		sprintf(pAtApn,"AT+CGDCONT=6,IP,\"%s\"%s",connectParam->apn,AT_CMD_END);
	}
	else
	{
		sprintf(pAtApn,"AT+CGDCONT=6,IP,\"cmnet\"%s",AT_CMD_END);
	}
	AtCmdEntity atCmdInit[]={
		{AT_CMD_DELAY"1000",10,NULL},
		{pAtApn,strlen(pAtApn),NULL},
		{"AT+CGACT=1,6"AT_CMD_END,14,AtCmdCb_cgact_set},
		{"AT+CGACT?"AT_CMD_END,11,AtCmdCb_cgact_read},
		{AT_CMD_DELAY"1000",10,NULL},
	};
	result = iot_vat_push_cmd(atCmdInit,sizeof(atCmdInit) / sizeof(atCmdInit[0]));
    return result;
}

static BOOL network_disconnect(BOOL flymode)
{
	BOOL result = FALSE;
	network_state = OPENAT_NETWORK_GOING_DOWN;
	networkStatusChange();
	AtCmdEntity atCmdInit[]={
		{AT_CMD_DELAY"1000",10,NULL},
		{"AT+CGACT=0,6"AT_CMD_END,4,NULL},
	};
	result = iot_vat_push_cmd(atCmdInit,sizeof(atCmdInit) / sizeof(atCmdInit[0]));
    return result;
}

static BOOL network_get_status(T_OPENAT_NETWORK_STATUS* status)
{
	BOOL result = FALSE;
	AtCmdEntity atCmdInit[]={
		{AT_CMD_DELAY"500",9,NULL},
		{"AT+CSQ"AT_CMD_END,8,AtCmdCb_csqval},
	};
	result = iot_vat_push_cmd(atCmdInit,sizeof(atCmdInit) / sizeof(atCmdInit[0]));
	iot_os_sleep(1000);
	status->state = network_state;
	status->simpresent= simpresent;
	status->csq = CsqValue;
     return result;
}

static BOOL network_set_cb(F_OPENAT_NETWORK_IND_CB indCb)
{
	iot_vat_init(NULL);
	g_s_OpenatNetIndCb = indCb;
	return TRUE;
}

VOID networkStatusChange(VOID)
{
  static E_OPENAT_NETWORK_STATE orgState = OPENAT_NETWORK_DISCONNECT;
  E_OPENAT_NETWORK_STATE newState = network_state;
  if(orgState != newState)
  {
  	if(g_s_OpenatNetIndCb)
	{
	  g_s_OpenatNetIndCb(newState);
	}
	orgState = newState;
  }  
}
static int checknetflg = FALSE;
VOID network_check_status(VOID)
{
	if(checknetflg)
		return;
	checknetflg = TRUE;
	AtCmdEntity atCmdInit[]={
		{"AT"AT_CMD_END,4,NULL},
		{AT_CMD_DELAY"2000",10,NULL},
		{"AT+CREG?"AT_CMD_END,11,AtCmdCb_creg},
	};
	iot_vat_push_cmd(atCmdInit,sizeof(atCmdInit) / sizeof(atCmdInit[0]));
}


/**��ȡ����״̬
*@param     status:   ��������״̬
*@return    TRUE:    �ɹ�
            FLASE:   ʧ��            
**/                                
BOOL iot_network_get_status (
                            T_OPENAT_NETWORK_STATUS* status
                            )
{
    return network_get_status(status);
}                            
/**��������״̬�ص�����
*@param     indCb:   �ص�����
*@return    TRUE:    �ɹ�
            FLASE:   ʧ��
**/                            
BOOL iot_network_set_cb    (
                            F_OPENAT_NETWORK_IND_CB indCb
                          )
{
    return network_set_cb(indCb);
}                          
/**�����������ӣ�ʵ��Ϊpdp��������
*@param     connectParam:  �������Ӳ�������Ҫ����APN��username��passwrd��Ϣ
*@return    TRUE:    �ɹ�
            FLASE:   ʧ��
@note      �ú���Ϊ�첽���������غ󲻴����������Ӿͳɹ��ˣ�indCb��֪ͨ�ϲ�Ӧ�����������Ƿ�ɹ������ӳɹ�������OPENAT_NETWORK_LINKED״̬
           ����socket����֮ǰ����Ҫ������������
           ��������֮ǰ��״̬��ҪΪOPENAT_NETWORK_READY״̬�����������ʧ��
**/                          
BOOL iot_network_connect     (
                            T_OPENAT_NETWORK_CONNECT* connectParam
                          )
{
    return network_connect(connectParam);
}                          
/**�Ͽ��������ӣ�ʵ��Ϊpdpȥ����
*@param     flymode:   ��ʱ��֧�֣�����ΪFLASE
*@return    TRUE:    �ɹ�
            FLASE:   ʧ��
@note      �ú���Ϊ�첽���������غ󲻴����������������ͶϿ��ˣ�indCb��֪ͨ�ϲ�Ӧ��
           ���ӶϿ�������״̬��ص�OPENAT_NETWORK_READY״̬
           ��ǰ����socket����Ҳ��ʧЧ����Ҫclose��
**/                                        
BOOL iot_network_disconnect  (
                            BOOL flymode
                          )
{
    return network_disconnect(FALSE);
}                          

