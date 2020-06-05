#include "iot_gpio.h"

/**����gpio 
*@param		port:		GPIO���
*@param		cfg:		������Ϣ
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_gpio_open(                          
                        E_AMOPENAT_GPIO_PORT port, 
                        T_AMOPENAT_GPIO_CFG *cfg    
                   )
{
    return OPENAT_config_gpio(port, cfg);
}

/**����gpio 
*@param		port:		GPIO���
*@param		value:		0 or 1
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_gpio_set(                               
                        E_AMOPENAT_GPIO_PORT port,  
                        UINT8 value                
                )
{
    return OPENAT_set_gpio(port, value);
}

/**��ȡgpio 
*@param		port:		GPIO���
*@param		value:		0 or 1
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/				
BOOL iot_gpio_read(                            
                        E_AMOPENAT_GPIO_PORT port, 
                        UINT8* value                
                  )
{
    return OPENAT_read_gpio(port, value);
}

/**�ر�gpio 
*@param		port:		GPIO���
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/	
BOOL iot_gpio_close(                            
                        E_AMOPENAT_GPIO_PORT port
                  )
{
    return OPENAT_close_gpio(port);
}