#include "iot_pwm.h"

/**��pwm���� 
*@param		port:		�˿�
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_pwm_open(E_AMOPENAT_PWM_PORT port)
{
    return OPENAT_pwm_open(port);
}

/**����pwm����
*@param		pwm_cfg:		pwm_cfg
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_pwm_set(T_AMOPENAT_PWM_CFG * pwm_cfg)
{
    return OPENAT_pwm_set(pwm_cfg);
}

/**�ر�pwm����
*@param		port:		�˿�
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_pwm_close(E_AMOPENAT_PWM_PORT port)
{
    return OPENAT_pwm_close(port);
}

