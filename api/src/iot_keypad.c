#include "iot_keypad.h"



/**���̳�ʼ�� 
*@param		pConfig: �������ò���
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_keypad_init(                         
                        T_AMOPENAT_KEYPAD_CONFIG *pConfig
                  )
{
   return OPENAT_init_keypad(  pConfig );
}

