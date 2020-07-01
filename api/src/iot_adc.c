#include "iot_adc.h"

/**ADC��ʼ�� 
*@param		channel:		adcͨ��
*@param     mode:       adcģʽ
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_adc_init(
                        E_AMOPENAT_ADC_CHANNEL channel  /* ADC��� */,
    					E_AMOPENAT_ADC_CFG_MODE mode
                )
{
    return OPENAT_InitADC(channel,mode);
}

/**��ȡADC����
*@note ADCֵ������Ϊ��, ��ѹֵ������Ϊ��
*@param		channel:		adcͨ��
*@param		adcValue:	ADCֵ������Ϊ��
*@param		voltage:	��ѹֵ������Ϊ��
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_adc_read(
                        E_AMOPENAT_ADC_CHANNEL channel,     
                        UINT32* adcValue,                  
                        UINT32* voltage                   
                )
{
    return OPENAT_ReadADC(channel, adcValue, voltage);
}
