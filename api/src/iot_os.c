#include "iot_os.h"
#include "am_openat.h"


/*******************************************
**                 SYSTEM                 **
*******************************************/

/**�����߳�
*@note  nPriorityֵ�ķ�����0-20, ֵԽ�����ȼ�Խ��
*@param	pTaskEntry:		�߳�������
*@param	pParameter:		��Ϊ�������ݸ��߳�������
*@param	nStackSize: 	�߳�ջ��С
*@param	nPriority: 		�߳����ȼ����ò���Խ���߳����ȼ�Խ��
*@param nCreationFlags: �߳�������ǣ� ��ο�E_AMOPENAT_OS_CREATION_FLAG
*@param pTaskName: 		�߳�����
*@return	HANDLE: 	�����ɹ������߳̾��
**/
HANDLE iot_os_create_task(                         
                            PTASK_MAIN pTaskEntry, 
                            PVOID pParameter,     
                            UINT16 nStackSize,      
                            UINT8 nPriority,        
                            UINT16 nCreationFlags, 
                            PCHAR pTaskName         
                          )
{
    HANDLE h = NULL;
    OPENAT_create_task(&h, pTaskEntry, pParameter, NULL, 
                nStackSize, nPriority, nCreationFlags, 10, pTaskName);
    return h;
}
             
/**�����߳�
*@param	hTask:		�߳̾����create_task�ӿڷ���ֵ
*@param	pParameter:		��Ϊ�������ݸ��߳������������滻create_task�ӿڴ����pParameter����
**/
VOID iot_os_start_task(                         
                        HANDLE hTask,         
                        PVOID pParameter      
                      )
{
    //IVTBL(start_task)(hTask, pParameter);
}

/**ֹͣ�߳�
*@param	hTask:		�߳̾��
**/	
VOID iot_os_stop_task(                             
                        HANDLE hTask            
                 )
{
    //IVTBL(stop_task)(hTask);
}

/**ɾ���߳�
*@param		hTask:		�߳̾��
*@return	TURE:		ɾ���̳߳ɹ�
*			FALSE: 		ɾ���߳�ʧ��
**/	
BOOL iot_os_delete_task(                           
                        HANDLE hTask            
                   )
{
    OPENAT_delete_task(hTask);
	return TRUE;
}

/**�����߳�
*@param		hTask:		�߳̾��
*@return	TURE: 		�����̳߳ɹ�
*			FALSE  : 	�����߳�ʧ��
**/
BOOL iot_os_suspend_task(                      
                            HANDLE hTask          
                        )
{
    //return IVTBL(suspend_task)(hTask);
}

/**�ָ��߳�
*@param		hTask:		�߳̾��
*@return	TURE: 		�ָ��̳߳ɹ�
*			FALSE  : 	�ָ��߳�ʧ��
**/
BOOL iot_os_resume_task(                           
                        HANDLE hTask          
                   )
{
    //return IVTBL(resume_task)(hTask);
}

/**��ȡ��ǰ�߳�
*@return	HANDLE:		���ص�ǰ�߳̾��
*
**/		
HANDLE iot_os_current_task(                         
                            VOID
                          )
{
    return OPENAT_current_task();
}

/**��ȡ��ǰ�̴߳�����Ϣ
*@param		hTask:		�߳̾��
*@param		pTaskInfo:		�߳���Ϣ�洢�ӿ�
*@return	TURE: 		�ɹ�
*			FALSE  : 	ʧ��
**/
BOOL iot_os_get_task_info(                         
                            HANDLE hTask,         
                            T_AMOPENAT_TASK_INFO *pTaskInfo 
                         )
{
    //return IVTBL(get_task_info)(hTask, pTaskInfo);
}




/**��ȡ�߳���Ϣ
*@note ������
*@param		hTask:		�߳̾��
*@param		ppMessage:	�洢��Ϣָ��
*@return	TURE: 		�ɹ�
*			FALSE  : 	ʧ��
**/
BOOL iot_os_wait_message(                          
                        HANDLE hTask,         
                        PVOID* ppMessage    
                    )
{
    int msgId;
    return OPENAT_wait_message(hTask, &msgId, ppMessage, 0);
}

/**�����߳���Ϣ
*@note ��ӵ���Ϣ����β��
*@param		hTask:		�߳̾��
*@param		pMessage:	�洢��Ϣָ��
*@return	TURE: 		�ɹ�
*			FALSE  : 	ʧ��
**/
BOOL iot_os_send_message(                       
                        HANDLE hTask,         
                        PVOID pMessage         
                    )
{
    return OPENAT_send_message(hTask, 0, pMessage, 0);
}

/**���͸����ȼ��߳���Ϣ
*@note      ��ӵ���Ϣ����ͷ��
*@param		hTask:		�߳̾��
*@param		pMessage:	�洢��Ϣָ��
*@return	TURE: 		�ɹ�
*			FALSE  : 	ʧ��
**/
BOOL iot_os_send_high_priority_message(          
                        HANDLE hTask,          
                        PVOID pMessage         
                                  )
{
    //return IVTBL(send_high_priority_message)(hTask, pMessage);
}

/**�����Ϣ�������Ƿ�����Ϣ
*@param		hTask:		�߳̾��
*@return	TURE: 		�ɹ�
*			FALSE  : 	ʧ��
**/
BOOL iot_os_available_message(                     
                        HANDLE hTask           
                         )
{
    //return IVTBL(available_message)(hTask);
}


/**������ʱ��
*@param		pFunc:			��ʱ����ʱ������
*@param		pParameter:		��Ϊ�������ݸ���ʱ����ʱ������
*@return	HANDLE: 		���ض�ʱ�����
*			
**/	
HANDLE iot_os_create_timer(                        
                        PTIMER_EXPFUNC pFunc,   
                        PVOID pParameter        
                      )
{
    return OPENAT_create_timerTask(pFunc, pParameter);
}

/**������ʱ��
*@param		hTimer:				��ʱ�������create_timer�ӿڷ���ֵ
*@param		nMillisecondes:		��ʱ��ʱ��
*@return	TURE: 				�ɹ�
*			FALSE  : 			ʧ��
**/
BOOL iot_os_start_timer(                         
                        HANDLE hTimer,         
                        UINT32 nMillisecondes   
                   )
{
    return OPENAT_start_timer(hTimer, nMillisecondes);
}

/**ֹͣ��ʱ��
*@param		hTimer:				��ʱ�������create_timer�ӿڷ���ֵ
*@return	TURE: 				�ɹ�
*			FALSE  : 			ʧ��
**/	
BOOL iot_os_stop_timer(                          
                        HANDLE hTimer
                    )
{
    return OPENAT_stop_timer(hTimer);
}

/**ɾ����ʱ��
*@param		hTimer:				��ʱ�������create_timer�ӿڷ���ֵ
*@return	TURE: 				�ɹ�
*			FALSE  : 			ʧ��
**/	
BOOL iot_os_delete_timer(                         
                        HANDLE hTimer          
                    )
{
    return OPENAT_delete_timer(hTimer);
}

/**��鶨ʱ���Ƿ��Ѿ�����
*@param		hTimer:				��ʱ�������create_timer�ӿڷ���ֵ
*@return	TURE: 				�ɹ�
*			FALSE  : 			ʧ��
**/
BOOL iot_os_available_timer(                      
                        HANDLE hTimer         
                       )
{
    //return IVTBL(available_timer)(hTimer);
}

/**��ȡϵͳʱ��
*@param		pDatetime:		�洢ʱ��ָ��
*@return	TURE: 			�ɹ�
*			FALSE  : 		ʧ��
**/	
BOOL iot_os_get_system_datetime(                   
                        T_AMOPENAT_SYSTEM_DATETIME* pDatetime
                       )
{
    return OPENAT_get_system_datetime(pDatetime);
}

/**����ϵͳʱ��
*@param		pDatetime:		�洢ʱ��ָ��
*@return	TURE: 			�ɹ�
*			FALSE  : 		ʧ��
**/	
BOOL iot_os_set_system_datetime(                   
                        T_AMOPENAT_SYSTEM_DATETIME* pDatetime
                       )
{
    return OPENAT_set_system_datetime(pDatetime);
}

BOOL iot_os_get_minute_tick(                   
                        PMINUTE_TICKFUNC pFunc
                       )
{
    //return IVTBL(get_minute_tick)(pFunc);
}

BOOL iot_os_init_alarm(                                       
                        T_AMOPENAT_ALARM_CONFIG *pConfig 
                   )
{
    //return IVTBL(init_alarm)(pConfig);
    return FALSE;
}

BOOL iot_os_set_alarm(                                        
                        T_AMOPENAT_ALARM_PARAM *pAlarmSet    
                   )
{
    //return IVTBL(set_alarm)(pAlarmSet);
    return FALSE;
}

HANDLE iot_os_enter_critical_section(            
                        VOID
                                )
{
    return OPENAT_enter_critical_section(); 
}

VOID iot_os_exit_critical_section(                
                        HANDLE hSection       
                             )
{
    OPENAT_exit_critical_section(hSection); 
}

HANDLE iot_os_create_semaphore(                   
                        UINT32 nInitCount     
                          )
{
    return OPENAT_create_semaphore(nInitCount); 
}

BOOL iot_os_delete_semaphore(                      
                        HANDLE hSem            
                        )
{
    return OPENAT_delete_semaphore(hSem);  
}

BOOL iot_os_wait_semaphore(                  
                        HANDLE hSem,           
                        UINT32 nTimeOut        
                      )
{
    return OPENAT_wait_semaphore(hSem, nTimeOut);
}

BOOL iot_os_release_semaphore(
                        HANDLE hSem           
                         )
{
    return OPENAT_release_semaphore(hSem);
}

UINT32 iot_os_get_semaphore_value (                  
                        HANDLE hSem            
                        )
{
    //return IVTBL(get_semaphore_value)(hSem);
}


PVOID iot_os_malloc(                                
                        UINT32 nSize           
               )
{
    return OPENAT_malloc(nSize);
}

PVOID iot_os_realloc(                            
                        PVOID pMemory,          
                        UINT32 nSize           
                )
{
    return OPENAT_realloc(pMemory, nSize);
}

VOID iot_os_free(                                  
                        PVOID pMemory          
            )
{
    OPENAT_free(pMemory);
}


BOOL iot_os_sleep(                                 
                        UINT32 nMillisecondes  
             )
{
    return OPENAT_sleep(nMillisecondes);
}

UINT32 iot_os_get_system_tick(                   
                        VOID
                         )
{
    return OPENAT_get_system_tick();
}

UINT32 iot_os_rand(                                
                        VOID
              )
{
    return OPENAT_rand();
}

VOID iot_os_srand(                              
                        UINT32 seed           
             )
{
    OPENAT_srand(seed);
}

VOID iot_os_shut_down(                             
                        VOID
                 )
{
    OPENAT_shut_down();
}

VOID iot_os_restart(                              
                        VOID
               )
{
    OPENAT_restart();
}

VOID iot_os_sys_request_freq(                     
                        E_AMOPENAT_SYS_FREQ freq
               )
{
    //IVTBL(sys_request_freq)(freq);
}
/**����trace��ӡ��
*@param		port:		0: uart1
                        1: uart2
                        2: usb modem ��ʹ��usbץlogʱ,luatoolsͬʱҲ��ץȡ�ײ�log
*@return	TURE: 			�ɹ�
*			FALSE  : 		ʧ��
**/
BOOL iot_os_set_trace_port(UINT8 port)
{
    return OPENAT_set_trace_port(port,0);
}

VOID iot_wifi_scan(OPENAT_wifiScanRequest* wifi_info)
{
	return OPENAT_get_wifiinfo(wifi_info);
}


