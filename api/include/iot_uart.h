#ifndef __IOT_UART_H__
#define __IOT_UART_H__

#include "iot_os.h"
/**
 * @ingroup iot_sdk_device ����ӿ�
 * @{
 */
/**
 * @defgroup iot_sdk_uart ���ڽӿ�
 * @{
 */
/**@example uart/demo_uart.c
* uart�ӿ�ʾ��
*/ 

/**��uart
*@param		port:		UART ���
*@param		cfg:		������Ϣ
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_uart_open(
                        E_AMOPENAT_UART_PORT port,       
                        T_AMOPENAT_UART_PARAM *cfg         
                   );

/**�ر�uart
*@param		port:		UART ���
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_uart_close(
                        E_AMOPENAT_UART_PORT port          
                   );


/**��uart
*@param		port:		UART ���
*@param		buf:		�洢���ݵ�ַ
*@param		bufLen:		�洢�ռ䳤��
*@param		timeoutMs:	��ȡ��ʱ ms
*@return	UINT32:     ʵ�ʶ�ȡ����
**/
UINT32 iot_uart_read(                                       
                        E_AMOPENAT_UART_PORT port,         
                        UINT8* buf,                       
                        UINT32 bufLen,                      
                        UINT32 timeoutMs                  
                   );

/**дuart
*@param		port:		UART ���
*@param		buf:		д�����ݵ�ַ
*@param		bufLen:		д�����ݳ���
*@return	UINT32:     ʵ�ʶ�ȡ����
**/
UINT32 iot_uart_write(                                      
                        E_AMOPENAT_UART_PORT port,        
                        UINT8* buf,                         
                        UINT32 bufLen                       
                    );

/** @}*/
/** @}*/
#endif

