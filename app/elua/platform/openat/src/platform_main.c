
#include "string.h"
#include "stdio.h"
#include "am_openat.h"

#include "assert.h"
#include "lplatform.h"
#include "platform_malloc.h"
#include "platform_rtos.h"
#include "platform_pmd.h"

#include "osi_api.h"

#define CUST_MAIN_TASK_PRIO     OPENAT_CUST_TASKS_PRIORITY_BASE
#define LUA_SHELL_TASK_PRIO     (OPENAT_CUST_TASKS_PRIORITY_BASE+10)

#define CUST_MAIN_TASK_STACK_SIZE       (2*1024)
#define LUA_SHELL_TASK_STACK_SIZE       (16*1024)/*��ջ�Ӵ���16K���������ű���ջ���*/

typedef enum CustMessageIdTag
{
    OPENAT_VAT_MSG_ID,
    
    NumOfCustMsgIds
}CustMessageId;

typedef struct {
    UINT16 len;
    PVOID data;
}CustMessageContext;

/*+\NEW\rufei\2013.9.13\����lua�ļ����ܱ��ƻ����³�����������*/
extern void LuaDeleteMainFile(void);
/*-\NEW\rufei\2013.9.13\����lua�ļ����ܱ��ƻ����³�����������*/

extern int LuaAppTask(void);
extern void RILAPI_ReceiveData(void *data, int len);
extern void platform_setup_vat_queue(void);

/*+\NEW\shenyuanyuan\2019.5.8\��lua�汾��usb AT�ڸ�Ϊlua�ű��ɿ��Ƶ���ͨ���ݴ���� */
extern u32 Get_usbdata_mode(void);
extern void USBAPI_ReceiveData(void *data, int len);
extern void platform_setup_usb_queue(void);
/*-\NEW\shenyuanyuan\2019.5.8\��lua�汾��usb AT�ڸ�Ϊlua�ű��ɿ��Ƶ���ͨ���ݴ���� */

static VOID cust_poweron_reason_init(VOID);
static void cust_pm_message(T_AMOPENAT_PM_MSG* pmMessage);
static VOID cust_at_message(UINT8 *pData, UINT16 length);

kal_bool lua_task_init ( void);

/*******************************************************
** ATTENTION: For our cust, this part must used. If you 
** have your own main entry file(instead of cust_main.c),
** pls copy this part!
**
** START .. START .. START .. START .. START .. START ..
********************************************************/
/* CUSTOM APP CODE INFOMATION */

/*******************************************************
** ATTENTION:
** END .. END .. END .. END .. END .. END .. END ..
********************************************************/

/* Function table from OpenAT platform */
HANDLE g_LuaShellTaskHandle;

struct
{
    HANDLE poweronSyncSemaphore;
    E_AMOPENAT_POWERON_REASON reason;
}g_PowronInfo = {0};


#if 0

/* Main function call by OpenAT platform */
VOID cust_main(VOID)
{
    /* ��Կ�Ѿ�׼���� ��Ҫ����֪ͨ */
    IVTBL(set_enc_data_ok)();

    cust_poweron_reason_init();

    /*+\NEW\liweiqiang\2013.9.22\����vat�������ź�������*/
    platform_setup_vat_queue();
    /*-\NEW\liweiqiang\2013.9.22\����vat�������ź�������*/
    
    IVTBL(init_at)(cust_at_message);
    
    /* FOR power on reason and charging messages */
    T_AMOPENAT_PMD_CFG pmdcfg;
    E_AMOPENAT_PM_CHR_MODE pmdmode;

    memset(&pmdcfg, 0, sizeof(T_AMOPENAT_PMD_CFG));
#if 1  /*ģ�����ó�緽��*/
    pmdmode = OPENAT_PM_CHR_BY_DEFAULT;
    /*��Ӳ������й�*/
    pmdcfg.deFault.batdetectEnable = TRUE;
    
    pmdcfg.deFault.tempdetectEnable = FALSE;
    pmdcfg.deFault.templowLevel = 0;
    pmdcfg.deFault.temphighLevel = 0;

/*+\NEW\2013.8.5\A9321�����������,������ѹ4.25V ������150mA ʵ��217mA */
    pmdcfg.deFault.batLevelEnable = TRUE;
    pmdcfg.deFault.batfullLevel = 4200;
    pmdcfg.deFault.batPreChargLevel = 4050;
    pmdcfg.deFault.poweronLevel = 3450;
    pmdcfg.deFault.poweroffLevel = 3400;
    pmdcfg.deFault.batAdc = OPENAT_ADC_7;/*adc_sense  or adc_vbat:OPENAT_ADC_0*/
    pmdcfg.deFault.tempAdc = OPENAT_ADC_1;
    
    /*level:  poweron-----levelFirst-----levelSecond-----levelFull*/
    /*current:----currentFirst----currentSecond---currentThird----*/
    pmdcfg.deFault.currentControlEnable = TRUE; 
    pmdcfg.deFault.currentFirst = OPENAT_PM_CHARGER_500MA;
    pmdcfg.deFault.intervalTimeFirst = 9*60; /*9����*/
    pmdcfg.deFault.batLevelFirst = 4150;
    pmdcfg.deFault.currentSecond = OPENAT_PM_CHARGER_300MA;
    pmdcfg.deFault.intervalTimeSecond = 6*60;/*6����*/
    pmdcfg.deFault.batLevelSecond = 4190;
    pmdcfg.deFault.currentThird = OPENAT_PM_CHARGER_100MA;
    pmdcfg.deFault.intervalTimeThird = 3*60; /*3����*/  
/*-\NEW\2013.8.5\A9321�����������,������ѹ4.25V ������150mA ʵ��217mA */

    pmdcfg.deFault.chargTimeOutEnable = FALSE;
    pmdcfg.deFault.TimeOutMinutes = 240;
#endif
#if 0
    pmdmode = OPENAT_PM_CHR_BY_IC;
    /*��Ӳ������й�*/
    pmdcfg.ic.batdetectEnable = TRUE;
    pmdcfg.ic.tempdetectEnable = FALSE;
    pmdcfg.ic.templowLevel = 0;
    pmdcfg.ic.temphighLevel = 0;
    
    pmdcfg.ic.chrswitchport = OPENAT_GPIO_8;
    pmdcfg.ic.batstatusport = OPENAT_GPIO_1;
    
    pmdcfg.ic.batLevelEnable = FALSE;
    pmdcfg.ic.batfullLevel = 4200;
    pmdcfg.ic.poweronLevel = 3450;
    pmdcfg.ic.poweroffLevel = 3400;
    pmdcfg.ic.batAdc = OPENAT_ADC_7;/*adc_sense  or adc_vbat:OPENAT_ADC_0*/
    pmdcfg.ic.tempAdc = OPENAT_ADC_1;/*adc_battemp*/

    pmdcfg.ic.chargTimeOutEnable = FALSE;
    pmdcfg.ic.TimeOutMinutes = 240;
#endif
    ASSERT(IVTBL(init_pmd)(pmdmode, &pmdcfg, cust_pm_message));
    
    /* ����custom app�߳� */
    g_CustTaskHandle = IVTBL(create_task)((PTASK_MAIN)cust_task_main, 
                                            NULL, 
                                            NULL, 
                                            CUST_MAIN_TASK_STACK_SIZE, 
                                            CUST_MAIN_TASK_PRIO, 
                                            OPENAT_OS_CREATE_DEFAULT, 
                                            0, 
                                            "cust task");

    if(OPENAT_INVALID_HANDLE == g_CustTaskHandle)
    {
        ASSERT(0);
    }
    
    g_LuaShellTaskHandle = IVTBL(create_task)((PTASK_MAIN)lua_shell_main, 
                                                NULL, 
                                                NULL, 
                                                LUA_SHELL_TASK_STACK_SIZE, 
                                                LUA_SHELL_TASK_PRIO, 
                                                OPENAT_OS_CREATE_DEFAULT, 
                                                0, 
                                                "lua shell task");

    if(OPENAT_INVALID_HANDLE == g_LuaShellTaskHandle)
    {
        ASSERT(0);
    }
}
#endif

static void cust_pm_message(T_AMOPENAT_PM_MSG* pmMessage)
{
/*+\NEW\liweiqiang\2013.7.8\����rtos.pmd��Ϣ*/
    static PlatformPmdData pmdData = 
    {
        TRUE,
         FALSE,
        PLATFORM_BATT_NOT_CHARGING,
        0xff,
         0,
    };
    T_AMOPENAT_BAT_STATUS battStatus;
    T_AMOPENAT_CHARGER_STATUS chargerStatus;

    //PUB_TRACE("[cust_pm_message]: event %d",pmMessage->evtId);
    
    switch(pmMessage->evtId)
    {
#ifdef LUA_TODO
        case OPENAT_DRV_EVT_PM_POWERON_ON_IND:
            if(OPENAT_PM_POWERON_BY_INVALID == g_PowronInfo.reason)
            {
                IVTBL(get_batteryStatus)(&battStatus);
                IVTBL(get_chargerStatus)(&chargerStatus);

                pmdData.battVolt = battStatus.batteryVolt;
                pmdData.chargerStatus = chargerStatus.chargerStatus == OPENAT_PM_CHARGER_PLUG_IN;
                
                g_PowronInfo.reason = pmMessage->param.poweronind.powerOnReason;
                
                if(0 != g_PowronInfo.poweronSyncSemaphore)
                {
                    OPENAT_release_semaphore(g_PowronInfo.poweronSyncSemaphore);
                }
            }
            else
            {
                /* received poweron reason message again */
            }
#endif
/*+\NEW\RUFEI\2015.6.11\Modify the charging message sequence*/
            return;
/*-\NEW\RUFEI\2015.6.11\Modify the charging message sequence*/
            break;
        case OPENAT_DRV_EVT_CHR_PRESENT_IND:
            pmdData.chargerStatus = pmMessage->param.chrpresentind.present;
            if(pmdData.chargerStatus == FALSE)
                pmdData.chargeState = PLATFORM_BATT_NOT_CHARGING; // ������γ�,��Ϊ���ڳ��״̬
            break;
        case OPENAT_DRV_EVT_BAT_PRESENT_IND:
            pmdData.battStatus = pmMessage->param.batpresentind.present;
            break;
        case OPENAT_DRV_EVT_BAT_LEVEL_IND:
            if(pmdData.battLevel == pmMessage->param.batlevelind.batteryLevel)
            {
                //�������û�б仯������Ϣ��ʾ
                return;
            }
            pmdData.battLevel = pmMessage->param.batlevelind.batteryLevel;
            IVTBL(get_batteryStatus)(&battStatus);
            pmdData.battVolt = battStatus.batteryVolt;
            break;
        case OPENAT_DRV_EVT_BAT_CHARGING:
        /*+\NEW\liweiqiang\2013.7.19\�������ʱ������������µ�����*/
/*+\NEW\RUFEI\2015.6.5\�����翪�������ϱ��������λ����*/
            if(pmdData.chargerStatus == FALSE)
            {
                pmdData.chargerStatus = TRUE;
            }
/*-\NEW\RUFEI\2015.6.5\�����翪�������ϱ��������λ����*/
            if(PLATFORM_BATT_CHARING == pmdData.chargeState &&
                pmdData.battLevel == pmMessage->param.chargingind.batteryLevel)
            {
                // ���ʱһֱ���ϱ�����Ϣ ��״̬δ�ı��ҵ������δ�仯����Ƶ����ʾ
                return;
            }
            pmdData.chargeState = PLATFORM_BATT_CHARING;
            pmdData.battLevel = pmMessage->param.chargingind.batteryLevel;
            IVTBL(get_batteryStatus)(&battStatus);
            pmdData.battVolt = battStatus.batteryVolt;
        /*-\NEW\liweiqiang\2013.7.19\�������ʱ������������µ�����*/
            break;
        case OPENAT_DRV_EVT_BAT_CHR_FULL:
/*+\NEW\RUFEI\2015.6.5\�����翪�������ϱ��������λ����*/
            if(pmdData.chargerStatus == FALSE)
            {
                pmdData.chargerStatus = TRUE;
            }
/*-\NEW\RUFEI\2015.6.5\�����翪�������ϱ��������λ����*/
        /*+\NEW\liweiqiang\2013.7.19\�������ʱ������������µ�����*/
            pmdData.chargeState = PLATFORM_BATT_CHARGE_STOP;
            pmdData.battLevel = pmMessage->param.chrfullind.batteryLevel;
            IVTBL(get_batteryStatus)(&battStatus);
            pmdData.battVolt = battStatus.batteryVolt;
        /*-\NEW\liweiqiang\2013.7.19\�������ʱ������������µ�����*/
            break;
        case OPENAT_DRV_EVT_BAT_CHR_STOP:
            if(pmMessage->param.chrstopind.chrStopReason == OPENAT_PM_CHR_STOP_NO_CHARGER)
                pmdData.chargeState = PLATFORM_BATT_NOT_CHARGING;
            else
                pmdData.chargeState = PLATFORM_BATT_CHARGE_STOP;
            break;
        case OPENAT_DRV_EVT_BAT_CHR_ERR:
            pmdData.chargeState = PLATFORM_BATT_CHARGE_STOP;
            break;
        default:
            break;
    }
    
    //if(pmdData.battLevel != 0xff) //ȥ��levelδ��ʼ�����ж�,����ֱ�Ӳ���������δ�ϱ������������һֱ�޷��ϱ�pmd��Ϣ
    {
        PlatformMsgData msgData;

        msgData.pmdData = pmdData;
        
        platform_rtos_send(MSG_ID_RTOS_PMD, &msgData);
    }
/*-\NEW\liweiqiang\2013.7.8\����rtos.pmd��Ϣ*/
}

/* AT message from OpenAT platform */
static VOID cust_at_message(UINT8 *pData, UINT16 length)
{
#if 0
    CustMessageContext *pMessage;
    T_AMOPENAT_MSG openatMsg;
    
    if(NULL != pMessage)
    {        //copy data
        pMessage = IVTBL(malloc)(length + sizeof(CustMessageContext));
        
        if(NULL != pMessage)
        {
            pMessage->data = (PVOID)(pMessage + 1);
            memcpy(pMessage->data, pData, length);
            pMessage->len = length;
        }

        openatMsg.openat_msg_id = MSG_ID_OPENAT_VAT_MSG_ID;
        openatMsg.openat_msg_context = pMessage;
        
        IVTBL(send_message)(&openatMsg);
    }
#endif

    RILAPI_ReceiveData(pData,length);
}

static VOID cust_poweron_reason_init(VOID)
{
    if(0 == g_PowronInfo.poweronSyncSemaphore)
    {
        g_PowronInfo.poweronSyncSemaphore = OPENAT_create_semaphore(0);
        ASSERT(0 != g_PowronInfo.poweronSyncSemaphore);
    }
}

static VOID cust_wait_for_poweron_reason(BOOL bDeleteSema)
{
    ASSERT(0 != g_PowronInfo.poweronSyncSemaphore);

    OPENAT_wait_semaphore(g_PowronInfo.poweronSyncSemaphore, 0);

    if(TRUE == bDeleteSema)
    {
        OPENAT_delete_semaphore(g_PowronInfo.poweronSyncSemaphore);
        g_PowronInfo.poweronSyncSemaphore = 0;
    }
}

/*+\NEW\liweiqiang\2013.12.12\���ӳ�翪��ʱ���û����о����Ƿ�����ϵͳ */
int cust_get_poweron_reason(void)
{
    return (int)g_PowronInfo.reason;
}

void cust_poweron_system(void)
{
    static BOOL hasPoweron = FALSE;

    if(!hasPoweron)
    {
        hasPoweron = TRUE;
        IVTBL(poweron_system)(OPENAT_PM_STARTUP_MODE_DEFAULT, OPENAT_PM_STARTUP_MODE_DEFAULT);
    }
}
/*-\NEW\liweiqiang\2013.12.12\���ӳ�翪��ʱ���û����о����Ƿ�����ϵͳ */

/*+\NEW\rufei\2015.4.17\ʵ�����ӿ�������������ϵͳL4�㹦��*/
void cust_repoweron_system(void)
{
    static BOOL hasRePoweron = FALSE;

    if(!hasRePoweron)
    {
        hasRePoweron = TRUE;
        IVTBL(poweron_system)(OPENAT_PM_STARTUP_MODE_ON, OPENAT_PM_STARTUP_MODE_ON);
    }
}
/*-\NEW\rufei\2015.4.17\ʵ�����ӿ�������������ϵͳL4�㹦��*/

/*+\NEW\liweiqiang\2013.10.25\lua�ű�ͳһ����luaĿ¼��,Ԥ�õķ�lua�ļ�ͳһ����ldata�ļ��� */
#define LUA_DIR "/lua"
#define LUA_DATA_DIR "/ldata"
#define LUA_DIR_UNI     L"/lua"
#define LUA_DATA_UNI    L"/ldata"

char *getLuaPath(void)
{
    return LUA_DIR "/?.lua;" LUA_DIR "/?.luac";
}

char *getLuaDir(void)
{
    return LUA_DIR;
}

char *getLuaDataDir(void)
{
    return LUA_DATA_DIR;
}
/*-\NEW\liweiqiang\2013.10.25\lua�ű�ͳһ����luaĿ¼��,Ԥ�õķ�lua�ļ�ͳһ����ldata�ļ��� */
void removeLuaDir(void)
{
    /*+\NEW\rufei\2013.9.13\����lua�ļ����ܱ��ƻ����³�����������*/
    LuaDeleteMainFile();
/*-\NEW\rufei\2013.9.13\����lua�ļ����ܱ��ƻ����³�����������*/
/*+\NEW\liweiqiang\2013.10.25\lua�ű�ͳһ����luaĿ¼��,Ԥ�õķ�lua�ļ�ͳһ����ldata�ļ��� */
    IVTBL(remove_dir)(LUA_DIR); //�쳣�˳�ɾ�����нű�
/*-\NEW\liweiqiang\2013.10.25\lua�ű�ͳһ����luaĿ¼��,Ԥ�õķ�lua�ļ�ͳһ����ldata�ļ��� */
/*+\NEW\liweiqiang\2013.11.28\luadb��ʽԶ������֧�� */
    IVTBL(remove_dir)(LUA_DATA_DIR); //�쳣�˳�ɾ����������
    IVTBL(remove_dir)("/luazip"); //�쳣�˳�ɾ��������
/*-\NEW\liweiqiang\2013.11.28\luadb��ʽԶ������֧�� */
}

/*lua app main*/
static HANDLE luaShellSem = 0;
/*+\NEW\zhuwangbin\2020.2.26\���lua Զ������ */
#include "hal_config.h"
#define OTA_APP_MAGIN ("APPUPDATE")
#define OTA_APP_FILE_NAME "app"
#define OTA_APP_FILE_PATH CONFIG_FS_FOTA_DATA_DIR "/" OTA_APP_FILE_NAME


static void lua_update(void)
{
	INT32 size, readLen;
	INT32 appMaginLen = strlen(OTA_APP_MAGIN);
	INT32 fd = OPENAT_open_file(OTA_APP_FILE_PATH, FS_O_RDONLY, 0);
	UINT32 flash_size = LUA_SCRIPT_SIZE;
	UINT8 appMagin[20] = {0};

	IVTBL(print)("lua_update fd %d\r\n", fd);	
	if (fd < 0)
	{
		goto updateEnd;
	}
	
	size = OPENAT_get_file_size_h(fd);

	IVTBL(print)("lua_update size %d\r\n", size);
	if (size <= appMaginLen || size >= flash_size)
	{
		goto updateEnd;
	}
	
	readLen = OPENAT_seek_file(fd, size-appMaginLen, 0);

	IVTBL(print)("lua_update OPENAT_seek_file %d\r\n", readLen);
	
	readLen = OPENAT_read_file(fd, appMagin, appMaginLen);

	IVTBL(print)("lua_update readLen %d, appMagin %s\r\n", readLen, appMagin);
	if (readLen != appMaginLen)
	{
		IVTBL(print)("lua_update appMagin error %s\r\n", appMagin);
		goto updateEnd;
	}

	if (memcmp(appMagin, OTA_APP_MAGIN, appMaginLen) != 0)
	{
		IVTBL(print)("lua_update appMagin error %s\r\n", appMagin);
		goto updateEnd;
	}

	readLen = OPENAT_seek_file(fd, 0, 0);
	IVTBL(print)("lua_update OPENAT_seek_file %d\r\n", readLen);
	{
		#define blockSize (0X10000)
		UINT8 *data = OPENAT_malloc(blockSize);
		UINT32 addr = LUA_SCRIPT_ADDR;;

		if (!data)
		{
			IVTBL(print)("lua_update data %x\r\n", data);
			goto updateEnd;
		}
		
		IVTBL(print)("lua_update LUA_SCRIPT_ADDR [%x,%x]\r\n", LUA_SCRIPT_ADDR,LUA_SCRIPT_ADDR+flash_size);
		OPENAT_flash_erase(LUA_SCRIPT_ADDR, LUA_SCRIPT_ADDR+flash_size);

		while(1)
		{
			readLen = OPENAT_read_file(fd, data, blockSize);
			
			IVTBL(print)("lua_update write readLen %x, addr %x\r\n", readLen, addr);
			if (readLen > 0)
			{
				UINT32 writenSize;
				
				OPENAT_flash_write(addr, readLen, &writenSize, data);
				if (writenSize == 0)
				{
					ASSERT(0);
				}
				addr += readLen;
			}
			else
			{
				OPENAT_close_file(fd);
				OPENAT_delete_file(OTA_APP_FILE_PATH);
				OPENAT_free(data);
	
				return;
			}
		}
	}
	
updateEnd:
	if (fd > 0)
	{
		OPENAT_close_file(fd);
	}
	return;
}
/*-\NEW\zhuwangbin\2020.2.26\���lua Զ������ */

VOID __lua_exit()
{
    OPENAT_sleep(2000);
	ASSERT(FALSE);
}

static VOID lua_shell_main(void *task_entry_ptr)
{
/*+\NEW\liweiqiang\2013.4.25\����lua�˳�assert���� */
    int luaExitStatus;
	/*+\BUG\wangyuan\2020.06.22\BUG_2360:coretest���ԣ�һֱ��ӡSET_PDP_4G_WAITAPN*/
    lua_task_init();
	
	/*+\NEW\zhuwangbin\2020.2.26\���lua Զ������ */
	lua_update();
	/*-\NEW\zhuwangbin\2020.2.26\���lua Զ������ */
	/*-\BUG\wangyuan\2020.06.22\BUG_2360:coretest���ԣ�һֱ��ӡSET_PDP_4G_WAITAPN*/
    IVTBL(print)("lua_shell_main - LUA_SYNC_MMI(OK).");

    #if 0
    OPENAT_print("lua_shell_main enter 111");
    if(0 == luaShellSem)
    {
        luaShellSem = OPENAT_create_semaphore(0);
        ASSERT(0 != luaShellSem);
    }

    platform_setup_vat_queue();
    OPENAT_RegisterVirtualATCallBack(cust_at_message);
    
    OPENAT_print("lua_shell_main enter 22222");

/*+\NEW\liweiqiang\2013.5.11\�����Խ�ѹluazipĿ¼���ļ�֧��,ѹ���㷨lzma*/
    IVTBL(make_dir)(L"/luazip", 0);
/*-\NEW\liweiqiang\2013.5.11\�����Խ�ѹluazipĿ¼���ļ�֧��,ѹ���㷨lzma*/
/*+\NEW\liweiqiang\2013.10.25\lua�ű�ͳһ����luaĿ¼��,Ԥ�õķ�lua�ļ�ͳһ����ldata�ļ��� */
    IVTBL(make_dir)(LUA_DIR_UNI, 0);
    IVTBL(make_dir)(LUA_DATA_UNI, 0);
/*-\NEW\liweiqiang\2013.10.25\lua�ű�ͳһ����luaĿ¼��,Ԥ�õķ�lua�ļ�ͳһ����ldata�ļ��� */
/*+\NEW\liweiqiang\2013.10.24\����¼��Ŀ¼,�Բ���¼���ļ� */
    IVTBL(make_dir)(L"/RecDir", 0);
/*-\NEW\liweiqiang\2013.10.24\����¼��Ŀ¼,�Բ���¼���ļ� */

    OPENAT_print("lua_shell_main enter 33333");
    //OPENAT_sleep(1000);
    #endif

#if 0
    OPENAT_send_at_command("ati\r", strlen("ati\r"));

    while(1)
    {
        OPENAT_wait_message(&msg, 0);
        
        switch(msg->openat_msg_id)
        {
            case MSG_ID_OPENAT_VAT_MSG_ID:
            {
                CustMessageContext* openatVatMsg = msg->openat_msg_context;
                OPENAT_print("OPENAT %d DATA11:%s", (UINT32)openatVatMsg->len, openatVatMsg->data);
                OPENAT_free(openatVatMsg);
                break;
            }
        }
    }
#endif

   
    luaExitStatus = LuaAppTask();

	osiThreadCallback(osiThreadCurrent(), __lua_exit, NULL);
    
/*+\NEW\rufei\2013.9.13\����lua�ļ����ܱ��ƻ����³�����������*/
    LuaDeleteMainFile();
/*-\NEW\rufei\2013.9.13\����lua�ļ����ܱ��ƻ����³�����������*/
    /*+\NEW\WJ\2018.10.10\ȥ��USES_NOR_FLASH��*/
    if(!OPENAT_is_nor_flash())
    {
        IVTBL(remove_file_rec)(LUA_DIR_UNI); //�쳣�˳�ɾ�����нű�
        IVTBL(remove_file_rec)(LUA_DATA_UNI); //�쳣�˳�ɾ����������
        IVTBL(remove_file_rec)(L"/luazip"); //�쳣�˳�ɾ��������
    }
    else
    {
    /*+\NEW\liweiqiang\2013.10.25\lua�ű�ͳһ����luaĿ¼��,Ԥ�õķ�lua�ļ�ͳһ����ldata�ļ��� */
        IVTBL(remove_file_rec)(LUA_DIR); //�쳣�˳�ɾ�����нű�
    /*-\NEW\liweiqiang\2013.10.25\lua�ű�ͳһ����luaĿ¼��,Ԥ�õķ�lua�ļ�ͳһ����ldata�ļ��� */
    /*+\NEW\liweiqiang\2013.11.28\luadb��ʽԶ������֧�� */
        IVTBL(remove_file_rec)(LUA_DATA_DIR); //�쳣�˳�ɾ����������
        IVTBL(remove_file_rec)("/luazip"); //�쳣�˳�ɾ��������
    /*-\NEW\liweiqiang\2013.11.28\luadb��ʽԶ������֧�� */
    }
    /*-\NEW\WJ\2018.10.10\ȥ��USES_NOR_FLASH��*/
    OPENAT_print("[lua_shell_main]: lua exit status %d", luaExitStatus);
	/*+\BUG\wangyuan\2020.04.07\BUG_1328��720U��Lua�ű���������ͱ���û��lua������Ϣ���*/	
    IVTBL(wait_semaphore)(luaShellSem, 10000); // 10�������ϵͳ
	/*-\BUG\wangyuan\2020.04.07\BUG_1328��720U��Lua�ű���������ͱ���û��lua������Ϣ���*/	

/*+\NEW\liweiqiang\2013.9.20\lua�쳣�˳�������ϵͳ,�����ļ�δд�뵽flash��*/
    // ASSERT(FALSE);
    //IVTBL(restart)();
/*-\NEW\liweiqiang\2013.9.20\lua�쳣�˳�������ϵͳ,�����ļ�δд�뵽flash��*/
/*-\NEW\liweiqiang\2013.4.25\����lua�˳�assert���� */
    osiEvent_t event;

    for (;;)
    {
        if (osiEventWait(osiThreadCurrent(), &event) == true)
        {
        }
    }
}

/*+\NEW\liweiqiang\2013.9.8\����pmd.init���ó������ӿ� */
BOOL cust_pmd_init(PlatformPmdCfg *cfg)
{
/*+\NEW\liweiqiang\2014.2.8\���Ƶ�Դ�������ýӿ� */
#define GET_FILED_VAL(fIELD, dEFault) (cfg->fIELD == PMD_CFG_INVALID_VALUE ? (dEFault) : (cfg->fIELD))
    T_AMOPENAT_PMD_CFG pmdcfg;
    E_AMOPENAT_PM_CHR_MODE pmdmode;

    pmdmode = OPENAT_PM_CHR_BY_DEFAULT;
    /*��Ӳ������й�*/
/*+\NEW\RUFEI\2015.5.8\���Ƴ�����*/
    pmdcfg.deFault.batdetectEnable = (GET_FILED_VAL(batdetectEnable, 0) == 1);
    
    pmdcfg.deFault.tempdetectEnable = FALSE;
    pmdcfg.deFault.templowLevel = 0;
    pmdcfg.deFault.temphighLevel = 0;

    pmdcfg.deFault.batLevelEnable = TRUE;
    pmdcfg.deFault.ccLevel = GET_FILED_VAL(ccLevel, OPENAT_PM_VOLT_04_050V);
    pmdcfg.deFault.cvLevel = GET_FILED_VAL(cvLevel, OPENAT_PM_VOLT_04_200V);
    pmdcfg.deFault.ovLevel = GET_FILED_VAL(ovLevel, OPENAT_PM_VOLT_04_250V);
    pmdcfg.deFault.pvLevel = GET_FILED_VAL(pvLevel, OPENAT_PM_VOLT_04_050V);
    pmdcfg.deFault.poweroffLevel = GET_FILED_VAL(poweroffLevel, OPENAT_PM_VOLT_03_400V);
/*+\NEW\RUFEI\2015.8.27\Add adc fuction*/
    pmdcfg.deFault.batAdc = OPENAT_ADC_0;/*adc_sense  or adc_vbat:OPENAT_ADC_0*/
    pmdcfg.deFault.tempAdc = OPENAT_ADC_1;
/*-\NEW\RUFEI\2015.8.27\Add adc fuction*/    
    pmdcfg.deFault.currentControlEnable = TRUE; 
    pmdcfg.deFault.ccCurrent = GET_FILED_VAL(ccCurrent, OPENAT_PM_CHARGER_200MA); 
    pmdcfg.deFault.fullCurrent = GET_FILED_VAL(fullCurrent, OPENAT_PM_CHARGER_30MA);
    pmdcfg.deFault.ccOnTime = 0; 
    pmdcfg.deFault.ccOnTime = 0; 
/*-\NEW\RUFEI\2015.5.8\���Ƴ�����*/
    pmdcfg.deFault.chargTimeOutEnable = FALSE;
    pmdcfg.deFault.TimeOutMinutes = 240;
/*-\NEW\liweiqiang\2014.2.8\���Ƶ�Դ�������ýӿ� */

    return OPENAT_init_pmd(pmdmode, &pmdcfg, cust_pm_message);
}
/*-\NEW\liweiqiang\2013.9.8\����pmd.init���ó������ӿ� */



kal_bool lua_task_init ( void)
{
    OPENAT_print("lua_task_init enter 111");
    if(0 == luaShellSem)
    {
        luaShellSem = OPENAT_create_semaphore(0);
        ASSERT(0 != luaShellSem);
    }

    platform_setup_vat_queue();

	/*+\NEW\shenyuanyuan\2019.5.8\��lua�汾��usb AT�ڸ�Ϊlua�ű��ɿ��Ƶ���ͨ���ݴ���� */
	platform_setup_usb_queue();
	/*-\NEW\shenyuanyuan\2019.5.8\��lua�汾��usb AT�ڸ�Ϊlua�ű��ɿ��Ƶ���ͨ���ݴ���� */
	
    OPENAT_init_at(cust_at_message);
    //OPENAT_RegisterVirtualATCallBack(cust_at_message);
    
    OPENAT_print("lua_task_init enter 22222");
    /*+\NEW\WJ\2018.10.10\ȥ��USES_NOR_FLASH��*/
    if(!OPENAT_is_nor_flash())
    {
        IVTBL(make_dir)(L"/luazip", 0);
        IVTBL(make_dir)(LUA_DIR_UNI, 0);
        IVTBL(make_dir)(LUA_DATA_UNI, 0);
        IVTBL(make_dir)(L"/RecDir", 0);
    }
    else
    {
    /*+\NEW\liweiqiang\2013.5.11\�����Խ�ѹluazipĿ¼���ļ�֧��,ѹ���㷨lzma*/
        IVTBL(make_dir)("/luazip", 0);
    /*-\NEW\liweiqiang\2013.5.11\�����Խ�ѹluazipĿ¼���ļ�֧��,ѹ���㷨lzma*/
    /*+\NEW\liweiqiang\2013.10.25\lua�ű�ͳһ����luaĿ¼��,Ԥ�õķ�lua�ļ�ͳһ����ldata�ļ��� */
        IVTBL(make_dir)(LUA_DIR, 0);
        IVTBL(make_dir)(LUA_DATA_DIR, 0);
    /*-\NEW\liweiqiang\2013.10.25\lua�ű�ͳһ����luaĿ¼��,Ԥ�õķ�lua�ļ�ͳһ����ldata�ļ��� */
    /*+\NEW\liweiqiang\2013.10.24\����¼��Ŀ¼,�Բ���¼���ļ� */
        IVTBL(make_dir)("/RecDir", 0);
    /*-\NEW\liweiqiang\2013.10.24\����¼��Ŀ¼,�Բ���¼���ļ� */
    }
    /*-\NEW\WJ\2018.10.10\ȥ��USES_NOR_FLASH��*/

    OPENAT_print("lua_task_init enter 33333");

    return KAL_TRUE;
}




kal_bool lua_task_create(void)
{
    //TODO �����߳�
    OPENAT_create_task(&g_LuaShellTaskHandle, lua_shell_main, NULL, NULL, 
                  32*1024, 
                  24,
                  OPENAT_OS_CREATE_DEFAULT,
                  300,
                  "lua_task");
    OPENAT_print("lua_task create end handle %x", g_LuaShellTaskHandle);
    return KAL_TRUE;
}


void luatEluaInit()
{
  //openat_init();
  lua_task_create();
}


