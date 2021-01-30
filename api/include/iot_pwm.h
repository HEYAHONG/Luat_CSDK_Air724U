#ifndef __IOT_PWM_H__
#define __IOT_PWM_H__

#include "iot_os.h"

/**
 * @ingroup iot_sdk_device ����ӿ�
 * @{
 */
/**
 * @defgroup iot_sdk_pwm pwm�ӿ�
 * @{
 */

/**��pwm���� 
*@param		port:		�˿�
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_pwm_open(E_AMOPENAT_PWM_PORT port);

/**����pwm����
*@param		pwm_cfg:		pwm_cfg
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_pwm_set(T_AMOPENAT_PWM_CFG * pwm_cfg);

/**�ر�pwm����
*@param		port:		�˿�
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_pwm_close(E_AMOPENAT_PWM_PORT port);


/** @}*/
/** @}*/





#endif

