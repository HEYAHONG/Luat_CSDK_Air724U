#ifndef __IOT_DEBUG_H__
#define __IOT_DEBUG_H__

#include "iot_os.h"
#include "stdarg.h"

/**
 * @defgroup iot_sdk_debug ���Խӿ�
 * @{
 */

/**������Ϣ��ӡ
**/
VOID iot_debug_print(     CHAR *fmt, ...);

/**assert����
*@param		condition:	��������
*@param		func:	    ���Ժ���
*@param		line:	    ����λ��
*@return	TURE: 	    �ɹ�
*           FALSE:      ʧ��
**/
VOID iot_debug_assert(                                            
                        BOOL condition,                  
                        CHAR *func,                      
                        UINT32 line                     
              );

/** @}*/

#endif
