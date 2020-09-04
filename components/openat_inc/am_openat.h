/*********************************************************
  Copyright (C), AirM2M Tech. Co., Ltd.
  Author: lifei
  Description: AMOPENAT ����ƽ̨
  Others:
  History: 
    Version�� Date:       Author:   Modification:
    V0.1      2012.12.14  lifei     �����ļ�
	V0.2      2012.12.26  brezen    ���pmd�ӿ�
	V0.3      2012.12.29  brezen    ���spi�ӿ�
	V0.4      2013.01.08  brezen    �޸�spi�ӿ�
	V0.5      2O13.01.14  brezen    1�����Ӻڰ�����ʼ������
	                                2�����Ӻڰ��������ӿ�
									3�����ٺڰ���ָ���ʱ�䣬���㵥��ʹ��ָ��ˢ��
    V0.6      2013.01.14  brezen    �޸ĺڰ��������ӿڲ���
	V0.7      2013.01.15  brezen    �޸ĺڰ��������ӿڲ���
    V0.8      2013.01.17  brezen    1�����ϵͳ��Դ���ƽӿ� 2�����ϵͳ���ػ��ӿ�
    V0.9      2013.01.23  brezen    �޸�SPI�ı���warning  
    V1.0      2013.01.28  brezen    ���PSAM���ӿ�
    V1.1      2013.01.30  brezen    �޸�poweron_system��������
    V1.2      2013.02.06  Jack.li   �������ͷ�ӿ�
    V1.3      2013.02.07  Jack.li   �����Ƶ¼�ơ���Ƶ���Žӿ�
    V1.4      2013.02.10  Jack.li   �޸Ĳ�����ʼ���ӿ�
    V1.5      2013.02.26  brezen    ���enter_deepsleep/exit_deepsleep�ӿ�
    V1.6      2013.03.21  maliang    �ļ�ϵͳ�ӿںͲ�����Ƶ�ļ��ӿڵ��ļ�����Ϊunicode little ending����
    V1.7      2013.04.03  Jack.li    ����I2C�ӿ�
    V1.8      2013.05.15  xc        ����tts�ӿ�
	V1.9      2013.07.18  brezen    ���set_dte_at_filter�ӿ�
	V2.0      2013.07.22  brezen    ���send_data_to_dte  send_data_to_ci set_dte_device�ӿ�
	V2.1      2013.08.20  brezen    ���PSAM��˫����֤ʧ�ܣ�rw_psam��ӷֲ����Ͳ���stopClock
	V2.2      2013.09.16  brezen    ���flush_file�ӿڣ�����֮ǰǿ��д��flash
	V2.3      2013.09.24  brezen    ���NV�ӿ�
	V2.4      2013.09.26  brezen    ֧������PSAM��
	V2.5      2013.12.30  brezen    ��������ӿ�
	V2.6      2014.6.26   brezen    �������spp�ӿ�
	V2.7      2015.04.21  panjun    ����OLED����ʱ��SPI�ӿ�
	V2.8      2015.05.20  panjun    Export two values 'lua_lcd_height' and 'lua_lcd_width'.
	V2.9      2015.06.22  panjun    Optimize mechanism of LUA's timer.
	V3.0      2015.07.04  panjun    Define a macro 'OPENAT_MAX_TIMER_ID', Increase quantity of OPENAT's timer.
	V3.1      2016.03.26  panjun    Add TTSPLY's API.
	V3.2      2016.05.03  panjun    Add an API "rtos.sysdisk_free".
*********************************************************/
#ifndef AM_OPENAT_H
#define AM_OPENAT_H

//#include "utils.h"
#include "am_openat_system.h"
#include "am_openat_fs.h"
#include "am_openat_drv.h"
#include "am_openat_vat.h"
#include "am_openat_tts.h"
#include "am_openat_image.h"
/*-\NEW\zhuwangbin\2020.3.25\���jpg�ļ��Ľ������ʾ*/
/*+\NEW\WZQ\2014.11.7\����SSL RSA����*/
#ifdef AM_OPENAT_SSL_RSA_SUPPORT
#include "openat_SSLRSA.h"
#endif

#include "am_openat_zbar.h"
#include "openat_camera.h"
/*-\NEW\WZQ\2014.11.7\����SSL RSA����*/
/*+\NEW\WJ\2019.1.8\���֤����Чʱ��У��*/
typedef struct {
    int     sec;         /* seconds */
    int     min;         /* minutes */
    int     hour;        /* hours */
    int     day;      
    int     mon;         /* month */
    int     year;        /* year */
}t_time;
/*-\NEW\WJ\2019.1.8\���֤����Чʱ��У��*/

#define IVTBL(func) OPENAT_##func

#define PUB_TRACE(pFormat, ...)  IVTBL(print)(pFormat, ##__VA_ARGS__)

//#define  NUCLEUS_TIMER_MECHANISM_ENABLE
#define  OPENAT_MAX_TIMERS 50
#define  OPENAT_MAX_TIMER_ID 60000
#define  LUA_TIMER_BASE LUA_APP_TIMER_0
#define  LUA_GIF_TIMER_BASE LUA_GIF_TIMER_0

/*\+NEW\yezhishe\2018.11.23\���GPIO8,9,10,11\*/
#define GPIO8_R28 8
#define GPIO9_R27 9
#define GPIO10_R26 10
#define GPIO11_R17 11
/*\-NEW\yezhishe\2018.11.23\���GPIO8,9,10,11\*/
/*+\NEW\wangyuan\2020.05.07\BUG_1126:֧��wifi��λ����*/
typedef struct
{
	UINT32 bssid_low;  	//< mac address low
	UINT16 bssid_high; 	//< mac address high
	UINT8 channel;	 	//< channel id
	signed char rssival; 	 	//< signal strength
} OPENAT_wifiApInfo;

typedef struct
{
	UINT32 max;		 ///< set by caller, must room count for store aps
	UINT32 found; 	 ///< set by wifi, actual found ap count
	UINT32 maxtimeout; ///< max timeout in milliseconds for each channel
	OPENAT_wifiApInfo *aps;	 ///< room for aps
} OPENAT_wifiScanRequest;
/*-\NEW\wangyuan\2020.05.07\BUG_1126:֧��wifi��λ����*/
    /*******************************************
    **                 SYSTEM                 **
    *******************************************/
BOOL OPENAT_create_task(                          /* �����߳̽ӿ� */
                            HANDLE* handlePtr,
                            PTASK_MAIN pTaskEntry,  /* �߳������� */
                            PVOID pParameter,       /* ��Ϊ�������ݸ��߳������� */
                            PVOID pStackAddr,       /* �߳�ջ��ַ����ǰ��֧�֣��봫��NULL */
                            UINT32 nStackSize,      /* �߳�ջ��С */
                            UINT8 nPriority,        /* �߳����ȼ����ò���Խ���߳����ȼ�Խ�� */
                            UINT16 nCreationFlags,  /* �߳�������ǣ� ��ο�E_AMOPENAT_OS_CREATION_FLAG */
                            UINT16 nTimeSlice,      /* ��ʱ��֧�֣��봫��0 */
                            PCHAR pTaskName         /* �߳����� */
                          );
VOID OPENAT_delete_task(HANDLE         task);

HANDLE OPENAT_current_task(                         /* ��ȡ��ǰ�߳̽ӿ� */
                            VOID
                          );
/*+\BUG\wangyuan\2020.06.30\BUG_2411:֧�������Ű�, ����2g CSDK�Ľӿ����*/
BOOL OPENAT_suspend_task(                           /* �����߳̽ӿ� */
    HANDLE hTask            /* �߳̾����create_task�ӿڷ���ֵ */
);

BOOL OPENAT_resume_task(                            /* �ָ��߳̽ӿ� */
    HANDLE hTask            /* �߳̾����create_task�ӿڷ���ֵ */
);
/*-\BUG\wangyuan\2020.06.30\BUG_2411:֧�������Ű�, ����2g CSDK�Ľӿ����*/
BOOL OPENAT_get_task_info(                          /* ��ȡ��ǰ�̴߳�����Ϣ�ӿ� */
                            HANDLE hTask,           /* �߳̾����create_task�ӿڷ���ֵ */
                            T_AMOPENAT_TASK_INFO *pTaskInfo /* �߳���Ϣ�洢�ӿ� */
                         );



    /****************************** �߳���Ϣ���нӿ� ******************************/
BOOL OPENAT_wait_message(
                                     HANDLE   task,
                                     int* msg_id,
                                     void* * ppMessage,
                                     UINT32 nTimeOut
                                     );



BOOL OPENAT_free_message(
                                     void* message_body
                                     );

    
BOOL OPENAT_send_message(                           /* ������Ϣ�ӿڣ���ӵ���Ϣ����β�� */
                                      HANDLE   destTask,
                                      int msg_id,
                                      void* pMessage,          /* �洢��Ϣָ�� */
                                      int message_length);

/*+\TASK\wangyuan\2020.06.28\task_255:֧�������Ű�, ����2g CSDK�Ľӿ����*/
BOOL OPENAT_SendHighPriorityMessage(             /* ���͸����ȼ���Ϣ�ӿڣ���ӵ���Ϣ����ͷ�� */
											    HANDLE hTask,           /* �߳̾����create_task�ӿڷ���ֵ */
											    PVOID pMessage          /* Ҫ������Ϣָ�� */
											);
/*-\TASK\wangyuan\2020.06.28\task_255:֧�������Ű�, ����2g CSDK�Ľӿ����*/


BOOL OPENAT_available_message(                      /* �����Ϣ�������Ƿ�����Ϣ */
                            HANDLE hTask            /* �߳̾����create_task�ӿڷ���ֵ */
                             );

/******************************************************
   ��OPENAT TASK����Ϣ�Ľӿ�
*******************************************************/
BOOL OPENAT_send_internal_message(                           /* ������Ϣ�ӿڣ���ӵ���Ϣ����β�� */
                                      int msg_id,
                                      void* pMessage,          /* �洢��Ϣָ�� */
                                      int message_length
                                      );

/****************************** ʱ��&��ʱ���ӿ� ******************************/
HANDLE OPENAT_create_timer(                         /* ������ʱ���ӿ� */
                            PTIMER_EXPFUNC pFunc,   /* ��ʱ����ʱ������ */
                            PVOID pParameter        /* ��Ϊ�������ݸ���ʱ����ʱ������ */
                          );

/*+\NEW\zhuwangbin\2020.2.12\��timer�Ļص����жϸĳ�task*/
HANDLE OPENAT_create_timerTask(                         /* ������ʱ���ӿ� */
                            PTIMER_EXPFUNC pFunc,   /* ��ʱ����ʱ������ */
                            PVOID pParameter        /* ��Ϊ�������ݸ���ʱ����ʱ������ */
                          );

/*-\NEW\zhuwangbin\2020.2.12\��timer�Ļص����жϸĳ�task*/
HANDLE OPENAT_create_hir_timer(						/* ������ʱ���ӿ� */
						PTIMER_EXPFUNC pFunc,	/* ��ʱ����ʱ������ */
						PVOID pParameter		/* ��Ϊ�������ݸ���ʱ����ʱ������ */
					  );


BOOL OPENAT_start_timer(                            /* ������ʱ���ӿ� */
                            HANDLE hTimer,          /* ��ʱ�������create_timer�ӿڷ���ֵ */
                            UINT32 nMillisecondes   /* ��ʱ��ʱ�� */
                       );

BOOL OPENAT_loop_start_timer(                            /* ����ѭ����ʱ���ӿ� */
                            HANDLE hTimer,          /* ��ʱ�������create_timer�ӿڷ���ֵ */
                            UINT32 nMillisecondes   /* ��ʱ��ʱ�� */
                       );

BOOL OPENAT_start_precise_timer(                            /* ������ʱ���ӿ� */
                            HANDLE hTimer,          /* ��ʱ�������create_timer�ӿڷ���ֵ */
                            UINT32 nMillisecondes   /* ��ʱ��ʱ�� */
                       );

BOOL OPENAT_stop_timer(                             /* ֹͣ��ʱ���ӿ� */
                            HANDLE hTimer           /* ��ʱ�������create_timer�ӿڷ���ֵ */
                      );

UINT64 OPENAT_timer_remaining(
							HANDLE hTimer
						);

 BOOL OPENAT_play_gif(
                                             char* buff,
                                            UINT8* gif_buf, 
                                            int length,
                                            int x, 
                                            int y, 
                                            int times
                                            );



BOOL OPENAT_stop_gif(void);

VOID OPENAT_lcd_sleep(BOOL);

BOOL OPENAT_delete_timer(                           /* ɾ����ʱ���ӿ� */
                            HANDLE hTimer           /* ��ʱ�������create_timer�ӿڷ���ֵ */
                        );



BOOL OPENAT_available_timer(                        /* ��鶨ʱ���Ƿ��Ѿ������ӿ� */
                            HANDLE hTimer           /* ��ʱ�������create_timer�ӿڷ���ֵ */
                           );



BOOL OPENAT_get_minute_tick(                        /* minute indication infterface */
                            PMINUTE_TICKFUNC pFunc  /* if pFunc != NULL, one MINUTE interval timer will be started. else the timer will be stop */
                           );

UINT32 OPENAT_get_timestamp(void);
BOOL OPENAT_gmtime(UINT32 timestamp, T_AMOPENAT_SYSTEM_DATETIME* pDatetime);


BOOL OPENAT_get_system_datetime(                    /* ��ȡϵͳʱ��ӿ� */
                            T_AMOPENAT_SYSTEM_DATETIME* pDatetime/* �洢ʱ��ָ�� */
                           );



BOOL OPENAT_set_system_datetime(                    /* ����ϵͳʱ��ӿ� */
                            T_AMOPENAT_SYSTEM_DATETIME* pDatetime/* �洢ʱ��ָ�� */
                           );
/*+\NEW\wangyuan\2020.05.08\������úͻ�ȡ��ǰʱ���Ľӿ�*/
void OPENAT_Set_TimeZone(INT32 timeZone);

INT8 OPENAT_get_TimeZone(VOID);
/*-\NEW\wangyuan\2020.05.08\������úͻ�ȡ��ǰʱ���Ľӿ�*/

/****************************** ALARM�ӿ� ******************************/
/*+\BUG\wangyuan\2020.04.30\BUG_1757:Air724Ŀǰû��alarm demo*/
BOOL OPENAT_InitAlarm(                                        /* ���ӳ�ʼ���ӿ� */
                            T_AMOPENAT_ALARM_CONFIG *pConfig   /* �������ò��� */
                       ); 



BOOL OPENAT_SetAlarm(                                        /* ��������/ɾ���ӿ� */
                            T_AMOPENAT_ALARM_PARAM *pAlarmSet    /* �������ò��� */
                       );

/*-\BUG\wangyuan\2020.04.30\BUG_1757:Air724Ŀǰû��alarm demo*/

/****************************** �ٽ���Դ�ӿ� ******************************/
HANDLE OPENAT_enter_critical_section(               /* �����ٽ���Դ���ӿڣ��ر������ж� */
                            VOID
                                    );


VOID OPENAT_exit_critical_section(                  /* �˳��ٽ���Դ���ӿڣ������ж� */
                            HANDLE hSection         /* �ٽ���Դ�������enter_critical_section�ӿڷ���ֵ */
                                 );



/****************************** �ź����ӿ� ******************************/
HANDLE OPENAT_create_semaphore(                     /* �����ź����ӿ� */
                            UINT32 nInitCount       /* �ź������� */
                              );



BOOL OPENAT_delete_semaphore(                       /* ɾ���ź����ӿ� */
                            HANDLE hSem             /* �ź��������create_semaphore�ӿڷ���ֵ */
                            );



BOOL OPENAT_wait_semaphore(                         /* ��ȡ�ź����ӿ� */
                            HANDLE hSem,            /* �ź��������create_semaphore�ӿڷ���ֵ */
                            UINT32 nTimeOut         /* Ŀǰ��֧�� */
                          );



BOOL OPENAT_release_semaphore(
                            HANDLE hSem             /* �ź��������create_semaphore�ӿڷ���ֵ */
                             );



UINT32 OPENAT_get_semaphore_value(                   /* ��ȡ������ֵ*/
                            HANDLE hSem             /* �ź��������create_semaphore�ӿڷ���ֵ */  
                            );



/****************************** �ڴ�ӿ� ******************************/

#define OPENAT_malloc(size) OPENAT_malloc1(size, (char*)__FUNCTION__,(UINT32) __LINE__)
#define OPENAT_calloc(cnt,size) OPENAT_malloc1(cnt*size, (char*)__FUNCTION__,(UINT32) __LINE__)
PVOID OPENAT_malloc1(                                /* �ڴ�����ӿ� */
                            UINT32 nSize,            /* ������ڴ��С */
							/*+\NEW \zhuwangbin\2020.02.3\�޸�warning*/
                            const char*  func,
							/*-\NEW \zhuwangbin\2020.02.3\�޸�warning*/
                            UINT32 line
                   );



PVOID OPENAT_realloc(                               /**/
                            PVOID pMemory,          /* �ڴ�ָ�룬malloc�ӿڷ���ֵ */
                            UINT32 nSize            /* ������ڴ��С */
                    );



VOID OPENAT_free(                                   /* �ڴ��ͷŽӿ� */
                            PVOID pMemory           /* �ڴ�ָ�룬malloc�ӿڷ���ֵ */
                );


/*+\bug2307\zhuwangbin\2020.06.20\���OPENAT_MemoryUsed�ӿ�*/
VOID OPENAT_MemoryUsed(UINT32* total, UINT32* used); /* ��ȡ�����ڴ�ʹ�����*/
/*-\bug2307\zhuwangbin\2020.06.20\���OPENAT_MemoryUsed�ӿ�*/
/****************************** ����ӿ� ******************************/
BOOL OPENAT_sleep(                                  /* ϵͳ˯�߽ӿ� */
                            UINT32 nMillisecondes   /* ˯��ʱ�� */
                 );

BOOL OPENAT_Delayms(                                  /* ��ʱ�ӿ� */
                            UINT32 nMillisecondes   /* ��ʱʱ�� */
                 );

INT64 OPENAT_get_system_tick(                      /* ��ȡϵͳtick�ӿ� */
                            VOID
                             );



UINT32 OPENAT_rand(                                 /* ��ȡ������ӿ� */
                            VOID
                  );



VOID OPENAT_srand(                                  /* ������������ӽӿ� */
                            UINT32 seed             /* ��������� */
                 );



VOID OPENAT_shut_down(                              /* �ػ��ӿ� */
                            VOID
                     );



VOID OPENAT_restart(                                /* �����ӿ� */
                            VOID
                   );



/*+\NEW\liweiqiang\2013.7.1\[OpenAt]����ϵͳ��Ƶ���ýӿ�*/
VOID OPENAT_sys_request_freq(                       /* ��Ƶ���ƽӿ� */
                            E_AMOPENAT_SYS_FREQ freq/* ��Ƶֵ */
                   );



UINT16 OPENAT_unicode_to_ascii(UINT8 *pOutBuffer, WCHAR *pInBuffer);


UINT16 OPENAT_ascii_to_unicode(WCHAR *pOutBuffer, UINT8 *pInBuffer);

    
/*-\NEW\liweiqiang\2013.7.1\[OpenAt]����ϵͳ��Ƶ���ýӿ�*/
    /*******************************************
    **              FILE SYSTEM               **
    *******************************************/
INT32 OPENAT_open_file(                             /* ���ļ��ӿ� *//* �����������ֵ��0��ʼ��С��0������ */
/*+\BUG WM-719\maliang\2013.3.21\�ļ�ϵͳ�ӿںͲ�����Ƶ�ļ��ӿڵ��ļ�����Ϊunicode little ending����*/
                            char* pszFileNameUniLe,/* �ļ�ȫ·������ unicode little endian*/
                            UINT32 iFlag,           /* �򿪱�־ */
	                        UINT32 iAttr            /* �ļ����ԣ���ʱ��֧�֣�������0 */
                      );



INT32 OPENAT_close_file(                            /* �ر��ļ��ӿ� */
                            INT32 iFd               /* �ļ������open_file �� create_file ���ص���Ч���� */
                       );



INT32 OPENAT_read_file(                             /* ��ȡ�ļ��ӿ� */
                            INT32 iFd,              /* �ļ������open_file �� create_file ���ص���Ч���� */
                            UINT8 *pBuf,            /* ���ݱ���ָ�� */
                            UINT32 iLen             /* buf���� */
                      );



INT32 OPENAT_write_file(                            /* д���ļ��ӿ�*/
                            INT32 iFd,              /* �ļ������open_file �� create_file ���ص���Ч���� */
                            UINT8 *pBuf,            /* ��Ҫд�������ָ�� */
                            UINT32 iLen             /* ���ݳ��� */
                       );



INT32 OPENAT_flush_file(                            /* ����д��flash*/
                            INT32 iFd               /* �ļ������open_file �� create_file ���ص���Ч���� */
                       );    



INT32 OPENAT_seek_file(                             /* �ļ���λ�ӿ� */
                            INT32 iFd,              /* �ļ������open_file �� create_file ���ص���Ч���� */
                            INT32 iOffset,          /* ƫ���� */
                            UINT8 iOrigin           /* ƫ����ʼλ�� */
                      );

INT32 OPENAT_tell_file(                             /* �ļ���λ�ӿ� */
                            INT32 iFd              /* �ļ������open_file �� create_file ���ص���Ч���� */
                      );

INT32 OPENAT_rename_file(char* name, char* new);

INT32 OPENAT_create_file(                           /* �����ļ��ӿ� */
                            char* pszFileNameUniLe,/* �ļ�ȫ·������ unicode little endian*/
                            UINT32 iAttr            /* �ļ����ԣ���ʱ��֧�֣�������0 */
                        );
UINT32 OPENAT_get_file_size(char* pszFileNameUniLe);
UINT32 OPENAT_get_file_size_h(int handle);


INT32 OPENAT_delete_file(                           /* ɾ���ļ��ӿ� */
                            char* pszFileNameUniLe/* �ļ�ȫ·������ unicode little endian*/
                        );



INT32 OPENAT_change_dir(                            /* �л���ǰ����Ŀ¼�ӿ� */
                            char* pszDirNameUniLe  /* Ŀ¼·�� unicode little endian */
                       );



INT32 OPENAT_make_dir(                              /* ����Ŀ¼�ӿ� */
                            char* pszDirNameUniLe, /* Ŀ¼·�� unicode little endian */
                            UINT32 iMode            /* Ŀ¼���ԣ���ϸ��μ� E_AMOPENAT_FILE_ATTR */
                     );



INT32 OPENAT_remove_dir(                            /* ɾ��Ŀ¼�ӿ� *//* ��Ŀ¼����Ϊ�գ��ӿڲ��ܷ��سɹ� */
                            char* pszDirNameUniLe  /* Ŀ¼·�� unicode little endian */
                       );



INT32 OPENAT_remove_dir_rec(                        /* �ݹ�ɾ��Ŀ¼�ӿ� *//* ��Ŀ¼�������ļ���Ŀ¼���ᱻɾ�� */
                            char* pszDirNameUniLe  /* Ŀ¼·�� unicode little endian */
                           );
                           

INT32 OPENAT_remove_file_rec(                        /* �ݹ�ɾ���ļ��ӿ� *//* ��Ŀ¼�������ļ����ᱻɾ�� */
                            char* pszDirNameUniLe  /* Ŀ¼·�� unicode little endian */
                           );                          



INT32 OPENAT_get_current_dir(                       /* ��ȡ��ǰĿ¼�ӿ� */
                            char* pCurDirUniLe,    /* �洢Ŀ¼��Ϣ unicode little endian */
                            UINT32 uUnicodeSize     /* �洢Ŀ¼��Ϣ�ռ��С */
                            );



INT32 OPENAT_find_first_file(                       /* �����ļ��ӿ� */
                            char* pszFileNameUniLe,/* Ŀ¼·�����ļ�ȫ·�� unicode little endian */
/*-\BUG WM-719\maliang\2013.3.21\�ļ�ϵͳ�ӿںͲ�����Ƶ�ļ��ӿڵ��ļ�����Ϊunicode little ending����*/
                            PAMOPENAT_FS_FIND_DATA  pFindData /* ���ҽ������ */
                            );




INT32 OPENAT_find_next_file(                        /* ���������ļ��ӿ� */
                            INT32 iFd,              /* �����ļ������Ϊ find_first_file �ӿڷ��ز��� */
                            PAMOPENAT_FS_FIND_DATA  pFindData /* ���ҽ������ */
                           );




INT32 OPENAT_find_close(                            /* ���ҽ����ӿ� */
                            INT32 iFd               /* �����ļ������Ϊ find_first_file �ӿڷ��ز��� */
                       );

INT32 OPENAT_ftell(                             /* �ļ���λ�ӿ� */
                            INT32 iFd             /* �ļ������open_file �� create_file ���ص���Ч���� */
                      );

/*+\NewReq WM-743\maliang\2013.3.28\[OpenAt]���ӽӿڻ�ȡ�ļ�ϵͳ��Ϣ*/
INT32 OPENAT_get_fs_info(                            /* ��ȡ�ļ�ϵͳ��Ϣ�ӿ� */
                            E_AMOPENAT_FILE_DEVICE_NAME       devName,            /*��ȡ�Ŀ�device name����Ϣ*/
                            T_AMOPENAT_FILE_INFO               *fileInfo,                   /*�ļ�ϵͳ����Ϣ*/
                            INT32 isSD,                  /*�Ƿ��ȡSD ����Ϣ*/
							INT32 type
                       );


/*-\NewReq WM-743\maliang\2013.3.28\[OpenAt]���ӽӿڻ�ȡ�ļ�ϵͳ��Ϣ*/
    
    /*+\NewReq\Jack.li\2013.1.17\����T���ӿ�*/
INT32 OPENAT_init_tflash(                            /* ��ʼ��T���ӿ� */
                            PAMOPENAT_TFLASH_INIT_PARAM pTlashInitParam/* T����ʼ������ */
                       );



    /*-\NewReq\Jack.li\2013.1.17\����T���ӿ�*/

E_AMOPENAT_MEMD_ERR OPENAT_flash_erase(              /*flash��д 128K����*/
                            UINT32 startAddr,
                            UINT32 endAddr
                       );


    
E_AMOPENAT_MEMD_ERR OPENAT_flash_write(              /*дflash*/
                            UINT32 startAddr,
                            UINT32 size,
                            UINT32* writenSize,
                            CONST UINT8* buf
                       );



E_AMOPENAT_MEMD_ERR OPENAT_flash_read(               /*��flash*/
                            UINT32 startAddr,
                            UINT32 size,
                            UINT32* readSize,
                            UINT8* buf
                       );
                       
UINT32 OPENAT_flash_page(void);

E_OPENAT_OTA_RESULT OPENAT_fota_init(void);
E_OPENAT_OTA_RESULT OPENAT_fota_download(const char* data, UINT32 len, UINT32 total);
E_OPENAT_OTA_RESULT OPENAT_fota_done(void);
    
    /*******************************************
    **                 NV                     **
    *******************************************/    
    /*��Ϊ����Ľӿڻ�ֱ�Ӳ���flash��������ϵͳ��������Ҫ���жϻ���Ҫ��Ƚϸߵ�TASK������*/    
INT32 OPENAT_nv_init(                                /*NV ��ʼ���ӿ�*/
                      UINT32 addr1,                  /*NV ��ŵ�ַ1 4KByte��ַ���� ��С4KByte*/
                      UINT32 addr2                   /*NV ��ŵ�ַ2 4KByte��ַ���� ��С4KByte*/
                    );


INT32 OPENAT_nv_add(                                 /*����һ��NV�洢����*/
                      UINT32 nv_id,                  /*NV ID Ŀǰֻ֧��0-255*/
                      UINT32 nv_size                 /*NV �����С,��λByte,���512Byte*/
                    );


INT32 OPENAT_nv_delete(                              /*ɾ��NV*/
                      UINT32 nv_id
                      );                 


INT32 OPENAT_nv_read(                                /*��ȡNV����*/
                     UINT32 nv_id,                   /*NV ID Ŀǰֻ֧��0-255*/
                     UINT8* buf,                     /*buf*/
                     UINT32 bufSize,                 /*buf�Ĵ�С*/
                     UINT32* readSize                /*ʵ�ʶ�ȡ����*/
                    );

    
INT32 OPENAT_nv_write(                               /*д��NV����*/
                      UINT32 nv_id,                  /*NV ID Ŀǰֻ֧��0-255*/
                      UINT8* buf,                    /*buf*/
                      UINT32 bufSize,                /*buf�Ĵ�С*/
                      UINT32* writeSize              /*ʵ��д�볤��*/
                     );          
    /*******************************************
    **                Hardware                **
    *******************************************/
    /****************************** GPIO ******************************/
BOOL OPENAT_config_gpio(                          
                            E_AMOPENAT_GPIO_PORT port,  /* GPIO��� */
                            T_AMOPENAT_GPIO_CFG *cfg    /* ��������� */
                       );


    
BOOL OPENAT_set_gpio(                               
                            E_AMOPENAT_GPIO_PORT port,  /* GPIO��� */
                            UINT8 value                 /* 0 or 1 */
                    );



/*+:\NewReq WM-475\brezen\2012.12.14\�޸�gpio�ӿ� */				
BOOL OPENAT_read_gpio(                            
                            E_AMOPENAT_GPIO_PORT port,  /* GPIO��� */
                            UINT8* value                /* ��� 0 or 1 */
                      );
/*-:\NewReq WM-475\brezen\2012.12.14\�޸�gpio�ӿ� */

/*+\BUG WM-720\rufei\2013.3.21\ ����gpio��close�ӿ�*/
BOOL OPENAT_close_gpio(                            
                            E_AMOPENAT_GPIO_PORT port/* GPIO��� */
                      );
/*-\BUG WM-720\rufei\2013.3.21\ ����gpio��close�ӿ�*/



/****************************** PMD ******************************/
BOOL OPENAT_init_pmd(     
                            E_AMOPENAT_PM_CHR_MODE chrMode,     /* ��緽ʽ */
/*+\NEW WM-746\rufei\2013.3.30\����оƬIC���*/
                            T_AMOPENAT_PMD_CFG*    cfg,         /*�������*/
/*-\NEW WM-746\rufei\2013.3.30\����оƬIC���*/
                            PPM_MESSAGE            pPmMessage   /* ��Ϣ�ص����� */
                    );



VOID OPENAT_get_batteryStatus(
                            T_AMOPENAT_BAT_STATUS* batStatus    /* ���״̬ OUT */
                             );



VOID OPENAT_get_chargerStatus(
                            T_AMOPENAT_CHARGER_STATUS* chrStatus/* �����״̬ OUT */
                             );



/*+\NEW\RUFEI\2014.2.13\����OPENAT��ѯ�����HW״̬�ӿ�*/
E_AMOPENAT_CHR_HW_STATUS OPENAT_get_chargerHwStatus(
                            VOID
                            );


/*-\NEW\RUFEI\2014.2.13\����OPENAT��ѯ�����HW״̬�ӿ�*/
/*+\TASK\wangyuan\2020.06.28\task_255:֧�������Ű�, ����2g CSDK�Ľӿ����*/
int OPENAT_get_chg_param(BOOL *battStatus, u16 *battVolt, u8 *battLevel, BOOL *chargerStatus, u8 *chargeState);
/*-\TASK\wangyuan\2020.06.28\task_255:֧�������Ű�, ����2g CSDK�Ľӿ����*/
BOOL OPENAT_poweron_system(                                     /* �������� */  
                            E_AMOPENAT_STARTUP_MODE simStartUpMode,/* ����SIM����ʽ */
                            E_AMOPENAT_STARTUP_MODE nwStartupMode/* ����Э��ջ��ʽ */
                          );



VOID OPENAT_poweroff_system(                                    /* �����ػ��������ر�Э��ջ�͹��� */        
                            VOID
                           );



BOOL OPENAT_poweron_ldo(                                        /* ��LDO */
                            E_AMOPENAT_PM_LDO    ldo,
                            UINT8                level          /*0-7 0:�ر� 1~7��ѹ�ȼ�*/
                       );



BOOL OPENAT_gpio_disable_pull(                            /* GPIO���ýӿ� */
                            E_AMOPENAT_GPIO_PORT port  /* GPIO��� */
        );

VOID OPENAT_enter_deepsleep(VOID);                                   /* ����˯�� */
                      


VOID OPENAT_exit_deepsleep(VOID);                                     /* �˳�˯�� */

void OPENAT_deepSleepControl(E_AMOPENAT_PMD_M m, BOOL sleep, UINT32 timeout);



/*+NEW OPEANT-104\RUFEI\2014.6.17\ ���ӻ�ȡ����ԭ��ֵ�ӿ�*/
 E_AMOPENAT_POWERON_REASON OPENAT_get_poweronCause (VOID);             /*��ȡ����ԭ��ֵ*/
/*-NEW OPEANT-104\RUFEI\2014.6.17\ ���ӻ�ȡ����ԭ��ֵ�ӿ�*/


    /****************************** UART ******************************/
BOOL OPENAT_config_uart(
                            E_AMOPENAT_UART_PORT port,          /* UART ��� */
                            T_AMOPENAT_UART_PARAM *cfg          /* ��ʼ������ */
                       );



/*+\NEW\liweiqiang\2013.4.20\���ӹر�uart�ӿ�*/
BOOL OPENAT_close_uart(
                            E_AMOPENAT_UART_PORT port           /* UART ��� */
                       );
/*-\NEW\liweiqiang\2013.4.20\���ӹر�uart�ӿ�*/




UINT32 OPENAT_read_uart(                                        /* ʵ�ʶ�ȡ���� */
                            E_AMOPENAT_UART_PORT port,          /* UART ��� */
                            UINT8* buf,                         /* �洢���ݵ�ַ */
                            UINT32 bufLen,                      /* �洢�ռ䳤�� */
                            UINT32 timeoutMs                    /* ��ȡ��ʱ ms */
                       );



UINT32 OPENAT_write_uart(                                       /* ʵ��д�볤�� */
                            E_AMOPENAT_UART_PORT port,          /* UART ��� */
                            UINT8* buf,                         /* д�����ݵ�ַ */
                            UINT32 bufLen                       /* д�����ݳ��� */
                        );

UINT32 OPENAT_write_uart_sync(E_AMOPENAT_UART_PORT port,          /* UART ��� */
                           UINT8* buf,                         /* д�����ݵ�ַ */
                           UINT32 bufLen                       /* д�����ݳ��� */
                           );


/*+\NEW\liweiqiang\2014.4.12\���Ӵ��ڽ����ж�ʹ�ܽӿ� */
BOOL OPENAT_uart_enable_rx_int(
                            E_AMOPENAT_UART_PORT port,          /* UART ��� */
                            BOOL enable                         /* �Ƿ�ʹ�� */
                                );
/*-\NEW\liweiqiang\2014.4.12\���Ӵ��ڽ����ж�ʹ�ܽӿ� */




/*+\NEW\liweiqiang\2013.12.25\���host uart�������ݹ��� */
    /****************************** HOST ******************************/
BOOL OPENAT_host_init(PHOST_MESSAGE hostCallback);



BOOL OPENAT_host_send_data(uint8 *data, uint32 len);
/*-\NEW\liweiqiang\2013.12.25\���host uart�������ݹ��� */



    /******************************* SPI ******************************/
/*+\NEW\zhuwangbin\2020.3.7\���openat spi�ӿ�*/
BOOL OPENAT_OpenSPI( E_AMOPENAT_SPI_PORT port, T_AMOPENAT_SPI_PARAM *cfg);
UINT32 OPENAT_ReadSPI(E_AMOPENAT_SPI_PORT port, CONST UINT8 * buf, UINT32 bufLen);
UINT32 OPENAT_WriteSPI(E_AMOPENAT_SPI_PORT port, CONST UINT8 * buf, UINT32 bufLen, BOOLEAN type);
UINT32 OPENAT_RwSPI(E_AMOPENAT_SPI_PORT port, CONST UINT8* txBuf, CONST UINT8* rxBuf,UINT32 bufLen);
BOOL OPENAT_CloseSPI( E_AMOPENAT_SPI_PORT port);
/*-\NEW\zhuwangbin\2020.3.7\���openat spi�ӿ�*/







/******************************* I2C ******************************/
BOOL OPENAT_open_i2c(
                            E_AMOPENAT_I2C_PORT  port,          /* I2C ��� */
                            T_AMOPENAT_I2C_PARAM *param         /* ��ʼ������ */
                      );




BOOL OPENAT_close_i2c(
                            E_AMOPENAT_I2C_PORT  port           /* I2C ��� */
                      );



UINT32 OPENAT_write_i2c(                                        /* ʵ��д�볤�� */
                            E_AMOPENAT_I2C_PORT port,          /* I2C ��� */
                            UINT8 salveAddr,
                            CONST UINT8 *pRegAddr,              /* I2C����Ĵ�����ַ */
                            CONST UINT8* buf,                   /* д�����ݵ�ַ */
                            UINT32 bufLen                       /* д�����ݳ��� */
                       );




UINT32 OPENAT_read_i2c(                                         /* ʵ�ʶ�ȡ���� */
                            E_AMOPENAT_I2C_PORT port,          /* I2C ��� */
                            UINT8 slaveAddr, 
                            CONST UINT8 *pRegAddr,              /* I2C����Ĵ�����ַ */
                            UINT8* buf,                         /* �洢���ݵ�ַ */
                            UINT32 bufLen                       /* �洢�ռ䳤�� */
                      );



BOOL  OPENAT_open_bt(
                            T_AMOPENAT_BT_PARAM* param
                     );



BOOL  OPENAT_close_bt(VOID);
                           



BOOL  OPENAT_poweron_bt(VOID);

  
BOOL  OPENAT_poweroff_bt(VOID);

BOOL  OPENAT_send_cmd_bt
                      (
                            E_AMOPENAT_BT_CMD cmd, 
                            U_AMOPENAT_BT_CMD_PARAM* param
                      );    

BOOL  OPENAT_build_rsp_bt
                      (
                            E_AMOPENAT_BT_RSP rsp,
                            U_AMOPENAT_BT_RSP_PARAM* param
                      );   

/*������ΪDevA�豸�������������ӣ����ӽ��OPENAT_BT_SPP_CONNECT_CNF
�����ΪDevB�豸�����Զ������������ӣ��ǾͲ���Ҫ��������ӿڣ�
�Զ����Ӻ���յ�OPENAT_BT_SPP_CONNECT_IND��Ϣ*/
BOOL  OPENAT_connect_spp                              
                      (
                            T_AMOPENAT_BT_ADDR* addr,
                            T_AMOPENAT_UART_PARAM* portParam    /*��ʱ��֧��,����дNULL��Ĭ������Ϊ9600,8(data),1(stop),none(parity)*/
                      );

BOOL  OPENAT_disconnect_spp                                    /*�Ͽ����ӣ���� OPENAT_BT_SPP_DISCONNECT_CNF*/
                      (
                            UINT8   port                        /*�˿ںţ�����OPENAT_BT_SPP_CONNECT_IND/OPENAT_BT_SPP_CONNECT_CNF���ϱ�*/
                      ); 

INT32  OPENAT_write_spp                                         /*���ͽ�����ڻص��������OPENAT_BT_SPP_SEND_DATA_CNF�¼����ϱ�*/
                                                                /*����ֵΪʵ��ִ��д��ĳ��ȣ����Ϊ0��ʾ����û�����ݱ����ͣ�Ҳû��
                                                                  OPENAT_BT_SPP_SEND_DATA_CNF�¼��ϱ�*/
                      (
                            UINT8   port,                       /*�˿ںţ�����OPENAT_BT_SPP_CONNECT_IND/OPENAT_BT_SPP_CONNECT_CNF���ϱ�*/
                            UINT8*  buf,                        /*���ܴ���"rls��ͷ���ַ������������Ϊ������RFCOMM��״̬������rls0*/
                            UINT32  bufLen                      /*һ����ഫ��T_AMOPENAT_BT_SPP_CONN_IND.maxFrameSize��С�ֽڵ�����*/
                      );


INT32  OPENAT_read_spp                                         /*�ص��������յ�OPENAT_BT_SPP_DATA_IND�¼��󣬵��øýӿڶ�ȡ*/
                                                                /*����ֵΪʵ�ʶ�ȡ����*/
                      (
                            UINT8   port,                       /*�˿ںţ�����OPENAT_BT_SPP_CONNECT_IND/OPENAT_BT_SPP_CONNECT_CNF���ϱ�*/
                            UINT8*  buf,
                            UINT32  bufLen
                      );   


/****************************** AUDIO ******************************/
BOOL OPENAT_open_tch(VOID);                                       /* ����������ͨ����ʼʱ���� */
                           


BOOL OPENAT_close_tch(VOID);                                         /* �ر�������ͨ������ʱ���� */

                            
BOOL OPENAT_play_tone(                                          /* ����TONE���ӿ� */
                            E_AMOPENAT_TONE_TYPE toneType,      /* TONE������ */
                            UINT16 duration,                    /* ����ʱ�� */
                            E_AMOPENAT_SPEAKER_GAIN volume      /* �������� */
                     );


BOOL OPENAT_stop_tone(VOID);                                          /* ֹͣ����TONE���ӿ� */


                            
BOOL OPENAT_play_dtmf(                                          /* ����DTMF���ӿ� */
                            E_AMOPENAT_DTMF_TYPE dtmfType,      /* DTMF���� */
                            UINT16 duration,                    /* ����ʱ�� */
                            E_AMOPENAT_SPEAKER_GAIN volume      /* �������� */
                     );


BOOL OPENAT_stop_dtmf(VOID);                                          /* ֹͣ����DTMF���ӿ� */


int OPENAT_streamplay(E_AMOPENAT_AUD_FORMAT playformat,AUD_PLAY_CALLBACK_T cb,char* data,int len);

                            
/*+\NewReq WM-584\maliang\2013.2.21\[OpenAt]֧��T������MP3*/
BOOL OPENAT_play_music(T_AMOPENAT_PLAY_PARAM*  playParam);
/*-\NewReq WM-584\maliang\2013.2.21\[OpenAt]֧��T������MP3*/


BOOL OPENAT_stop_music(VOID);                                         /* ֹͣ��Ƶ���Žӿ� */


                            
BOOL OPENAT_pause_music(VOID);                                        /* ��ͣ��Ƶ���Žӿ� */


                            
BOOL OPENAT_resume_music(VOID);                                      /* ֹͣ��Ƶ���Žӿ� */

                           
/*+\NewReq WM-710\maliang\2013.3.18\ [OpenAt]���ӽӿ�����MP3���ŵ���Ч*/
BOOL OPENAT_set_eq(                                       /* ����MP3��Ч*/
                            E_AMOPENAT_AUDIO_SET_EQ setEQ
                          );
/*-\NewReq WM-710\maliang\2013.3.18\ [OpenAt]���ӽӿ�����MP3���ŵ���Ч*/

BOOL OPENAT_open_mic(VOID);                                           /* ����MIC�ӿ� */
                            


BOOL OPENAT_close_mic(VOID);                                          /* �ر�MIC�ӿ� */

                           
BOOL OPENAT_mute_mic(VOID);                                           /* MIC�����ӿ� */


BOOL OPENAT_unmute_mic(VOID);                                         /* ���MIC�����ӿ� */

                            
BOOL OPENAT_set_mic_gain(                                       /* ����MIC����ӿ� */
                            UINT16 micGain                      /* ����MIC�����棬���Ϊ20 */
                        );



int OPENAT_audio_record( 									  /* ¼���ӿ� */
										E_AMOPENAT_RECORD_PARAM* param,
										AUD_RECORD_CALLBACK_T cb);



int OPENAT_audio_stop_record(VOID);

BOOL OPENAT_open_speaker(VOID);                                       /* ���������ӿ� */


BOOL OPENAT_close_speaker(VOID);                                      /* �ر��������ӿ� */


BOOL OPENAT_mute_speaker(VOID);                                       /* �����������ӿ� */


BOOL OPENAT_unmute_speaker(VOID);                                     /* ��������������ӿ� */


                           
BOOL OPENAT_set_speaker_gain(                                   /* ���������������� */
                            E_AMOPENAT_SPEAKER_GAIN speakerGain /* ���������������� */
                            );



/*+\bug\wj\2020.5.6\����ͨ���е��������ӿ�*/
BOOL OPENAT_set_sph_vol(								   
						UINT32 vol);
/*-\bug\wj\2020.5.6\����ͨ���е��������ӿ�*/
/*+\BUG\wangyuan\2020.08.10\BUG_2801: ˼��CSDK ͨ��iot_audio_set_speaker_vol()�ӿ�����ͨ��������Ч AT+CLVL�����޸�ͨ������*/
UINT32 OPENAT_get_sph_vol(void);
/*-\BUG\wangyuan\2020.08.10\BUG_2801: ˼��CSDK ͨ��iot_audio_set_speaker_vol()�ӿ�����ͨ��������Ч AT+CLVL�����޸�ͨ������*/

E_AMOPENAT_SPEAKER_GAIN OPENAT_get_speaker_gain(VOID);                /* ��ȡ������������ӿ� */



BOOL OPENAT_set_channel(                                        /* ������Ƶͨ���ӿ� */
                            E_AMOPENAT_AUDIO_CHANNEL channel    /* ͨ�� */
                       );

/*+\BUG\wangyuan\2020.06.08\BUG_2163:CSDK�ṩaudio��Ƶ���Žӿ�*/
BOOL OPENAT_set_music_volume(UINT32 vol);		/* ������Ƶ�����ӿ� */

UINT32 OPENAT_get_music_volume(void);		/* ��ȡ��Ƶ�����ӿ� */

void OPENAT_delete_record(VOID);
/*-\BUG\wangyuan\2020.06.08\BUG_2163:CSDK�ṩaudio��Ƶ���Žӿ�*/

VOID OPENAT_set_channel_with_same_mic(                          /* ���ù���ͬһ��MIC��Ƶͨ���ӿ� */
                        E_AMOPENAT_AUDIO_CHANNEL channel_1,     /* ͨ�� 1 */
                        E_AMOPENAT_AUDIO_CHANNEL channel_2      /* ͨ�� 2 */
                   );



/*+\BUG WM-882\rufei\2013.7.18\����ͨ������*/
BOOL set_hw_channel(
                          E_AMOPENAT_AUDIO_CHANNEL hfChanne,    /*�ֱ�ͨ��*/
                          E_AMOPENAT_AUDIO_CHANNEL erChanne,    /*����ͨ��*/
                          E_AMOPENAT_AUDIO_CHANNEL ldChanne    /*����ͨ��*/
                         );
/*-\BUG WM-882\rufei\2013.7.18\����ͨ������*/

/*+\NEW\zhuwangbin\2020.8.11\��Ӷ����������*/
int OPENAT_headPlug(E_OPENAT_AUD_HEADSET_TYPE type);
/*-\NEW\zhuwangbin\2020.8.11\��Ӷ����������*/

E_AMOPENAT_AUDIO_CHANNEL OPENAT_get_current_channel(VOID);



/*+\NewReq WM-711\maliang\2013.3.18\[OpenAt]���ӽӿڴ򿪻�ر���Ƶ�ػ�����*/
/*+\New\lijiaodi\2014.7.30\�޸���Ƶ�ػ����Խӿڣ�����IsSpkLevelAdjust��SpkLevel������
                           ���IsSpkLevelAdjustΪFALSE,spkLevelΪĬ�ϵ�ֵ������ΪSpkLevelָ����ֵ*/
BOOL  OPENAT_audio_loopback(BOOL  start,                    /*��ʼ��ֹͣ�ػ�����*/
                                        E_AMOPENAT_AUDIO_LOOPBACK_TYPE type,   /*�ػ����Ե�����*/
                                        BOOL IsSpkLevelAdjust,   /*SPK������С�Ƿ��ָ��*/
                                        UINT8 SpkLevel);   /*SPKָ����������СSpkLevelȡֵ��ΧAUD_SPK_MUTE--AUD_SPK_VOL_7*/
/*-\New\lijiaodi\2014.7.30\�޸���Ƶ�ػ����Խӿڣ�����IsSpkLevelAdjust��SpkLevel������
                           ���IsSpkLevelAdjustΪFALSE,spkLevelΪĬ�ϵ�ֵ������ΪSpkLevelָ����ֵ*/
/*-\NewReq WM-711\maliang\2013.3.18\[OpenAt]���ӽӿڴ򿪻�ر���Ƶ�ػ�����*/



BOOL  OPENAT_audio_inbandinfo(PINBANDINFO_CALLBACK callback); 

int OPENAT_WritePlayData(char* data, unsigned size);

    
/****************************** ADC ******************************/
/*+\BUG\wangyuan\2020.06.30\BUG_2424:CSDK-8910 ADC ��API �������*/
BOOL OPENAT_InitADC(
				    E_AMOPENAT_ADC_CHANNEL channel  /* ADC��� */,
				    E_AMOPENAT_ADC_CFG_MODE mode
				);


BOOL OPENAT_ReadADC(
				    E_AMOPENAT_ADC_CHANNEL channel,  /* ADC��� */
				    kal_uint32*               adcValue,   /* adcֵ */
				    kal_uint32*               voltage    /* ��ѹֵ*/
				);
/*-\BUG\wangyuan\2020.06.30\BUG_2424:CSDK-8910 ADC ��API �������*/


/****************************** LCD ******************************/
/* MONO */                                                  /* �ڰ���*/			
BOOL OPENAT_init_mono_lcd(                                      /* ��Ļ��ʼ���ӿ� */
                            T_AMOPENAT_MONO_LCD_PARAM*  monoLcdParamP
                    );

/*+\bug2958\czm\2020.9.1\disp.close() ֮����ִ��disp.init ����ʾֱ������*/
BOOL OPENAT_close_mono_lcd(void);/* Lcd�رսӿ� */
/*-\bug2958\czm\2020.9.1\disp.close() ֮����ִ��disp.init ����ʾֱ������*/

VOID OPENAT_send_mono_lcd_command(                              /* ��������ӿ� */
                            UINT8 cmd                           /* ���� */
                                 );


VOID OPENAT_send_mono_lcd_data(                                 /* �������ݽӿ� */
                            UINT8 data                          /* ���� */
                              );



VOID OPENAT_update_mono_lcd_screen(                             /* ������Ļ�ӿ� */
                            T_AMOPENAT_LCD_RECT_T* rect         /* ��Ҫˢ�µ����� */
                                  );


VOID OPENAT_clear_mono_lcd(                                     /* ������һ������ʵ��LCD RAM����ʾ��������� */
                            UINT16 realHeight,                  /* ʵ��LCD RAM �߶� */
                            UINT16 realWidth                    /* ʵ��LCD RAM ��ȣ�������4�ı��� */
                          );


/* COLOR */                                                 /* ��ɫ�� */
BOOL OPENAT_init_color_lcd(                                     /* ��Ļ��ʼ���ӿ� */
                            T_AMOPENAT_COLOR_LCD_PARAM *param   /* ������ʼ������ */
                          );

/*+\bug2958\czm\2020.9.1\disp.close() ֮����ִ��disp.init ����ʾֱ������*/
BOOL OPENAT_close_color_lcd(void);/* Lcd�رսӿ� */
/*-\bug2958\czm\2020.9.1\disp.close() ֮����ִ��disp.init ����ʾֱ������*/

BOOL OPENAT_spiconfig_color_lcd(                     /* ����SPI���� */
                        void                    
                        );

VOID OPENAT_send_color_lcd_command(                             /* ��������ӿ� */
                            UINT8 cmd                           /* ���� */
                                  );


VOID OPENAT_send_color_lcd_data(                                /* �������ݽӿ� */
                            UINT8 data                          /* ���� */
                               );


VOID OPENAT_update_color_lcd_screen(                            /* ������Ļ�ӿ� */
                            T_AMOPENAT_LCD_RECT_T* rect,        /* ��Ҫˢ�µ����� */
                            UINT16 *pDisplayBuffer              /* ˢ�µĻ����� */
                                   );


void OPENAT_layer_flatten(OPENAT_LAYER_INFO* layer1,
                                   OPENAT_LAYER_INFO* layer2,
                                   OPENAT_LAYER_INFO* layer3) ;



/*+\NEW\Jack.li\2013.2.9\��������ͷ��Ƶ¼�ƽӿ� */
BOOL OPENAT_camera_videorecord_start(                           /* ��ʼ¼����Ƶ */
                    INT32 iFd                               /* ¼���ļ���� */
                    );


BOOL OPENAT_camera_videorecord_pause(                           /* ��ͣ¼����Ƶ */
                    void                    
                    );


BOOL OPENAT_camera_videorecord_resume(                          /* �ָ�¼����Ƶ */
                        void                    
                        );


BOOL OPENAT_camera_videorecord_stop(                            /* ֹͣ¼����Ƶ */
                        void                    
                        );


/*-\NEW\Jack.li\2013.2.9\��������ͷ��Ƶ¼�ƽӿ� */

/*-\NEW\Jack.li\2013.1.28\��������ͷ����*/

/*+\NEW\Jack.li\2013.2.10\������Ƶ���Žӿ� */
BOOL OPENAT_video_open(                                         /* ����Ƶ���� */
                        T_AMOPENAT_VIDEO_PARAM *param           /* ��Ƶ���� */
                        );


BOOL OPENAT_video_close(                                        /* �ر���Ƶ���� */
                        void
                        );


BOOL OPENAT_video_get_info(                                     /* ��ȡ��Ƶ��Ϣ */
                        T_AMOPENAT_VIDEO_INFO *pInfo            /* ��Ƶ��Ϣ */
                        );


BOOL OPENAT_video_play(                                         /* ���� */
                        void
                        );


BOOL OPENAT_video_pause(                                        /* ��ͣ */
                        void
                        );


BOOL OPENAT_video_resume(                                       /* �ָ� */
                        void
                        );


BOOL OPENAT_video_stop(                                         /* ֹͣ */
                        void
                        );
    /*-\NEW\Jack.li\2013.2.10\������Ƶ���Žӿ� */

#if 0 
void OPENAT_ttsply_Error_Callback(S32 ttsResult);
void OPENAT_ttsply_State_Callback(int ttsplyState);
BOOL OPENAT_ttsply_initEngine(AMOPENAT_TTSPLY_PARAM *param);
BOOL OPENAT_ttsply_setParam(U16 playParam, S16 value);
S16 OPENAT_ttsply_getParam(U16 playParam);
BOOL OPENAT_ttsply_play(AMOPENAT_TTSPLY *param);
BOOL OPENAT_ttsply_pause(void);
BOOL OPENAT_ttsply_stop(void);
#endif //__AM_LUA_TTSPLY_SUPPORT__


void OPENAT_AW9523B_display(
                                                                u8 num1, 
                                                                u8 num2, 
                                                                u8 num3
                                                                );



void OPENAT_AW9523B_set_gpio(
                                                                    u8 pin_num, 
                                                                    u8 value
                                                                    );

void OPENAT_AW9523B_init(void);


BOOL OPENAT_register_msg_proc(
                                                                    int msg_id, 
                                                                    openat_msg_proc msg_proc
                                                                    );



void OPENAT_SLI3108_init(openat_msg_proc msg_proc);


typedef enum SLI3108_STATUS_TAG
{
    SLI3108_STATUS_INVALID,
    SLI3108_STATUS_LOW,                          /*������ͷ�ֵ����ʾδ���*/
    SLI3108_STATUS_HIGH,                         /*������߷�ֵ����ʾ�����*/
}SLI3108_STATUS; 


    /* NULL */
    /****************************** KEYPAD ******************************/
BOOL OPENAT_init_keypad(                                        /* ���̳�ʼ���ӿ� */
                            T_AMOPENAT_KEYPAD_CONFIG *pConfig   /* �������ò��� */
                       );
    

    /****************************** TOUCHSCREEN ******************************/
BOOL OPENAT_init_touchScreen(                                   /* ��������ʼ���ӿ� */
                            PTOUCHSCREEN_MESSAGE pTouchScreenMessage /* ������Ϣ�ص����� */
                            );
    


VOID OPENAT_TouchScreen_Sleep_In(VOID);



VOID OPENAT_TouchScreen_Sleep_Out(VOID);

    
/******************************** PSAM ***********************************/
/* ע��:::PSAM���ӿ��ڲ����豸ʱ�ᵼ�µ����߱�����ֱ���豸����Ӧ����2s+��ʱ */
E_AMOPENAT_PSAM_OPER_RESULT OPENAT_open_psam(                   /* ��psam */
                            E_AMOPENAT_PSAM_ID id               /* Ӳ��SIM���ӿ� */
                                            );


VOID OPENAT_close_psam(                                         /* �ر�psam */
                            E_AMOPENAT_PSAM_ID id               /* Ӳ��SIM���ӿ� */
                      );
	/*  rw_psam�ӿ�ʹ��˵��
		psam ָ���
		-------------------------
	    ����  |	ֵ
		-------------------------
		CLA     80
		INS  	82
		P1	    00
		P2	    00����Կ�汾��KID��
		Lc	    08
		DATA	��������
		-------------------------

	�����ҪDATA������PSAMָ���Ҫ�ֲ����ͣ�
	  ��һ�� ����DATA֮ǰ������֣�ͬʱ����rxLen=1��stopClock = FALSE
	  	����ֵ����ΪIns �� ~Ins 
	  		���������Ins�����������
	  		���������~Ins�������ڶ���
	  �ڶ��� ���DATA��ʣ������ݴ���1���ֽڣ�����һ���ֽڵ�DATAʣ�����ݣ�ͬʱ����rxLen=1��stopClock = FALSE��
	        ����ֱ�ӽ��������
	     ����ֵ�Ĵ���͵�һ������ֵ����һ��
		   
	  ����������DATA�е�ʣ�����ݣ�ͬʱ����stopClock=TRUE,rxLen������Ҫ����
	*/					  
E_AMOPENAT_PSAM_OPER_RESULT OPENAT_rw_psam(                     /* �������� */
                            E_AMOPENAT_PSAM_ID id,              /* Ӳ��SIM���ӿ� */
                            CONST UINT8*  txBuf,                /* д���� */
                            UINT16        txLen,                /* д���泤�� */
                            UINT8*        rxBuf,                /* ������ */
                            UINT16        rxLen,                /* �����泤�� */
                            BOOL          stopClock             /* ����ֿ���������ΪFALSE, ����һ�η��ͻ���Ϊ�ֲ����͵����һ������ΪTRUE*/  
                                          );


    
E_AMOPENAT_PSAM_OPER_RESULT OPENAT_reset_psam(                  /* ��λPSAM */
                            E_AMOPENAT_PSAM_ID id,              /* Ӳ��SIM���ӿ� */
                            UINT8*      atrBuf,                 /* ATR ���� */
                            UINT16      atrBufLen,              /* ATR ���泤�� */
                            E_AMOPENAT_PSAM_VOLT_CLASS volt     /* ������ѹ */
                                             );



E_AMOPENAT_PSAM_OPER_RESULT OPENAT_setfd_psam(                  /* ����Fֵ��Dֵ��Ĭ��F=372 D=1 */
                            E_AMOPENAT_PSAM_ID id,              /* Ӳ��SIM���ӿ� */
                            UINT16      f,                      /* Fֵ */
                            UINT8       d                       /* Dֵ */
                                             );

    /******************************** PWM ***********************************/
/*+\NEW\RUFEI\2015.9.8\Add pwm function */
BOOL OPENAT_OpenPwm(E_AMOPENAT_PWM_PORT port);

BOOL OPENAT_SetPwm(T_AMOPENAT_PWM_CFG *cfg);

BOOL OPENAT_ClosePwm(E_AMOPENAT_PWM_PORT port);
/*-\NEW\RUFEI\2015.9.8\Add pwm function */

    /****************************** FM ******************************/
BOOL OPENAT_open_fm(											/* ��FM */
                            T_AMOPENAT_FM_PARAM *fmParam        /* ��ʼ������ */
                   );


BOOL OPENAT_tune_fm(											/* ����ָ��Ƶ�� */
                            UINT32 frequency                    /* Ƶ��(KHZ) */
                   );


BOOL OPENAT_seek_fm(											/* ������һ��̨ */
                            BOOL seekDirection					/* TRUE:Ƶ�����ӵķ��� FALSE::Ƶ�ʼ�С�ķ��� */		
                   );


BOOL OPENAT_stopseek_fm(										/* ֹͣ���� */
                            void
                       );


BOOL OPENAT_setvol_fm(											/* ������Ч */
                            E_AMOPENAT_FM_VOL_LEVEL volume, 	/* �������� */
                            BOOL bassBoost, 
                            BOOL forceMono
                     );



BOOL OPENAT_getrssi_fm(											/* ��ȡFM�ź� */
                            UINT32* pRssi
                      );


BOOL OPENAT_close_fm(											/* �ر�FM */
                            void
                    );


/*******************************************
**               AT COMMAND               **
*******************************************/
BOOL OPENAT_init_at(                                            /* ����ATͨ·��ʼ���ӿ� */
                            PAT_MESSAGE pAtMessage              /* AT��Ϣ�ص����� */
                   );



BOOL OPENAT_send_at_command(                                    /* ����AT����ӿ� */
                            UINT8 *pAtCommand,                  /* AT���� */
                            UINT16 nLength                      /* AT����� */
                           );



/*+\NEW WM-733\xc\2013.04.19\�޸ļ��ܿ�����(���openat��ȡ�ӿ�) */
/*******************************************
**               ���ܿ�����               **
*******************************************/
BOOL OPENAT_set_encinfo(                         /* ������Կ��Ϣ */
                        UINT8 *encInfo,
                        UINT32 len
              );


BOOL OPENAT_get_encinfo(                         /* ��ȡ��Կ��Ϣ */
                        UINT8 *encInfo,
                        UINT32 len
              );



UINT8 OPENAT_get_encresult(                         /* ��ȡ����У���� */
                        void
              );



/*+\NEW WM-733\xc\2013.05.06\�޸ļ��ܿ�����5(��ӻ�ȡ�����͵Ľӿ�) */
UINT8 OPENAT_get_cardtype(                         /* ��ȡ������ 0δ֪  1���ܿ�  2��ͨ��  */
                        void
              );
/*-\NEW WM-733\xc\2013.05.06\�޸ļ��ܿ�����5(��ӻ�ȡ�����͵Ľӿ�) */


/*+\NEW WM-733\xc\2013.04.23\�޸ļ��ܿ�����2(��openat�ӿڴ���at������Կ��Ϣ������ź���) */
BOOL OPENAT_set_enc_data_ok(                         /* mmi��׼������Կ��Ϣ��Ҫ���������֪ͨ */
                        void
              );
/*-\NEW WM-733\xc\2013.04.23\�޸ļ��ܿ�����2(��openat�ӿڴ���at������Կ��Ϣ������ź���) */
/*+\NEW WM-733\xc\2013.04.19\�޸ļ��ܿ�����(���openat��ȡ�ӿ�) */



/*+\NEW\xiongjunqun\2014.04.02\����TTS�Ĵ���*/
/* delete TTS���÷���AT ָ�����ʽ */
/*-\NEW\xiongjunqun\2014.04.02\����TTS�Ĵ���*/
/*+\NEW AMOPENAT-91 \zhangyang\2013.11.19\����USB HID����*/
void OPENAT_uhid_open(
        void OPENAT_handler(uint8 *, uint32));



void OPENAT_uhid_close(
        void);


int32 OPENAT_uhid_write(
        uint8 *data_p, 
        uint32 length);
/*-\NEW AMOPENAT-91 \zhangyang\2013.11.19\����USB HID����*/


/*+\NEW\RUFEI\2014.8.20\����gps�ӿ�ʵ��*/
    /*******************************************
    **               RDAGPS                      **
    *******************************************/
BOOL OPENAT_Gps_open(
        T_AMOPENAT_RDAGPS_PARAM *cfg);

BOOL OPENAT_Gps_close(
        T_AMOPENAT_RDAGPS_PARAM *cfg);
/*-\NEW\RUFEI\2014.8.20\����gps�ӿ�ʵ��*/
    /*******************************************
    **                 DEBUG                  **
    *******************************************/
VOID OPENAT_print(                                              /* trace log����ӿ� */
                            CHAR * fmt, ...
                 );

VOID OPENAT_openat_print(CHAR * fmt, ...);

VOID OPENAT_lua_print(CHAR * fmt, ...);

VOID OPENAT_openat_dump(char* head, char* hex, UINT32 len);


VOID OPENAT_assert(                                             /* ���Խӿ� */
                            BOOL condition,                     /* ���� */
                            CHAR *func,                         /* �������� */
                            UINT32 line                         /* ���� */
                  );



VOID OPENAT_enable_watchdog(BOOL enable);                            /*�򿪿��Ź�*/

/*+\NEW\xiongjunqun\2015.06.11\����factory�ӿڿ�*/
boolean OPENAT_factory_check_calib(void);
/*-\NEW\xiongjunqun\2015.06.11\����factory�ӿڿ�*/


void OPENAT_watchdog_restart(void);
/*+\NEW\brezen\2016.03.03\����watchdogʹ�ܽӿ�*/
BOOL OPENAT_watchdog_enable(BOOL enable, UINT16 count);
/*-\NEW\brezen\2016.03.03\����watchdogʹ�ܽӿ�*/


#ifdef HRD_SENSOR_SUPPORT
/***********************************************/
VOID OPENAT_hrd_sensor_start(void);
VOID OPENAT_hrd_sensor_close(void);
int OPENAT_hrd_sensor_getrate(void);
/***********************************************/
#endif

INT64 OPENAT_disk_free(int drvtype);
INT32 OPENAT_disk_volume(int drvtype);


/*+\BUG WM-656\lifei\2013.03.07\[OpenAT] �޸�cust����������*/
#define OPENAT_CUST_VTBL_DEFUALT_MAGIC 0x87654321
/*-\BUG WM-656\lifei\2013.03.07\[OpenAT] �޸�cust����������*/

typedef enum E_AMOPENAT_CUST_INIT_RESULT_TAG
{
    OPENAT_CUST_INIT_RES_OK,        /* �ͻ������ʼ���ɹ������Ե���cust_main���� */
    OPENAT_CUST_INIT_RES_ERROR,     /* �ͻ������ʼ��ʧ�ܣ��������cust_main���� */
    OPENAT_CUST_INIT_RES_MAX
}E_AMOPENAT_CUST_INIT_RESUL;

extern u16 lua_lcd_height;
extern u16 lua_lcd_width;

#define OPENAT_TICKS_TO_MILLSEC(t) ((UINT64)(t)*5)
#define OPENAT_TICKS_TO_SEC(t) ((UINT64)(t)*5/1000)

BOOL OPENAT_set_trace_port(UINT8 port, UINT8 usb_port_diag_output);
UINT8 OPENAT_get_trace_port(void);
/*+\NEW\WJ\2018.10.10\ȥ��USES_NOR_FLASH��*/
BOOL OPENAT_is_nor_flash(void);
UINT32 OPENAT_turn_addr(UINT32 addr);
/*-\NEW\WJ\2018.10.10\ȥ��USES_NOR_FLASH��*/

/*+\NEW\shenyuanyuan\2019.4.19\����AT+TRANSDATA����*/
void OPENAT_rtos_sendok(char *src);
/*-\NEW\shenyuanyuan\2019.4.19\����AT+TRANSDATA����*/
/*+\NEW\shenyuanyuan\2019.11.01\����rtos.set_lua_info�ӿں�AT+LUAINFO������*/
void OPENAT_rtos_set_luainfo(char *src);
/*-\NEW\shenyuanyuan\2019.11.01\����rtos.set_lua_info�ӿں�AT+LUAINFO������*/
/*+\NEW\WANGJIAN\2019.4.28\��ӿ���������Ӧģ���Ӧ��Ƶ��*/
int OPENAT_set_band();
/*-\NEW\WANGJIAN\2019.4.28\��ӿ���������Ӧģ���Ӧ��Ƶ��*/
/*+\NEW\wangyuan\2020.05.07\BUG_1126:֧��wifi��λ����*/
void OPENAT_get_wifiinfo(OPENAT_wifiScanRequest* wifi_info);
/*-\NEW\wangyuan\2020.05.07\BUG_1126:֧��wifi��λ����*/
void OPENAT_get_channel_wifiinfo(OPENAT_wifiScanRequest* wifi_info, uint32 channel);

#ifdef __AM_LUA_TTSPLY_SUPPORT__
BOOL OPENAT_tts_init(TTS_PLAY_CB fCb);
BOOL OPENAT_tts_set_param(OPENAT_TTS_PARAM_FLAG flag,u32 value);
BOOL OPENAT_tts_play(char *text,u32 len);
BOOL OPENAT_tts_stop();
#endif
#endif /* AM_OPENAT_H */

/*+\NEW\zhuwangbin\2020.05.14\���openat speex�ӿ�*/
BOOL openat_speexEncoderInit(void);
int  openat_speexEncode(short decoded[], int decoded_size, char *output, int output_size);
BOOL openat_speexEncoderDestroy(void);

BOOL openat_speexDecoderInit(void);
int openat_speexDecoder(char encoded[], int encoded_size, short output[], int output_size);
BOOL openat_speexDecoderDestroy(void);
/*-\NEW\zhuwangbin\2020.05.14\���openat speex�ӿ�*/

/*+\bug2767\zhuwangbin\2020.8.5\����ⲿpa���ýӿ�*/
BOOL OPENAT_ExPASet(OPENAT_EXPA_T * exPaCtrl);
/*-\bug2767\zhuwangbin\2020.8.5\����ⲿpa���ýӿ�*/

/*+\new\zhuwangbin\2020.6.2\�����Ƶ�����������ýӿ�*/
BOOL OPENAT_setpa(OPENAT_SPKPA_TYPE_T type);
OPENAT_SPKPA_TYPE_T OPENAT_getpa(void);
/*-\new\zhuwangbin\2020.6.2\�����Ƶ�����������ýӿ�*/
/*+\BUG\wnagyuan\2020.06.10\BUG_1930:Lua��Ҫsd��Ĭ�Ϲر�,��Ҫʱ��Lua�ű���������sd������*/
BOOL OPENAT_fs_mount_sdcard(void);
BOOL OPENAT_fs_umount_sdcard(void);
/*-\BUG\wnagyuan\2020.06.10\BUG_1930:Lua��Ҫsd��Ĭ�Ϲر�,��Ҫʱ��Lua�ű���������sd������*/
/*+\BUG\wangyuan\2020.07.29\BUG_2663:��������ο�2G CSDK����iot_debug_set_fault_mode�ӿ�*/
VOID OPENAT_SetFaultMode(E_OPENAT_FAULT_MODE mode);
/*-\BUG\wangyuan\2020.07.29\BUG_2663:��������ο�2G CSDK����iot_debug_set_fault_mode�ӿ�*/

