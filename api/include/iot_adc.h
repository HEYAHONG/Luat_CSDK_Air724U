#ifndef __IOT_ADC_H__
#define __IOT_ADC_H__

#include "iot_os.h"

/**
 * @ingroup iot_sdk_device ����ӿ�
 * @{
 */
/**
 * @defgroup iot_sdk_adc adc�ӿ�
 * @{
 */

/**ADC��ʼ�� 
*@param		channel:		adcͨ��
*@param     mode:       adcģʽ
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_adc_init(
                        E_AMOPENAT_ADC_CHANNEL channel  /* ADC��� */,
    					E_AMOPENAT_ADC_CFG_MODE mode
                );


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
                );

/** @}*/
/** @}*/





#endif

