#include "iot_sys.h"


/**ota����new app���ļ���������֪�ײ���Ҫ���ļ���ȡ�����µĳ���
*@param		newAPPFile:		�³����ļ� 
*@return	TRUE: �ɹ�   FALSE: ʧ��
**/
BOOL iot_ota_newapp(              
                    CONST char* newAPPFile
               )
{
    //return IVTBL(flash_set_newapp)(newAPPFile);
    return FALSE;
}


/**��char����ת��ΪWCHAR�����������Ϊiot_fs_open_file�Ƚӿڵ��ļ�������
*@param     dst:        ת��������
*@param     src:        �ȴ�ת�����ַ���
*@return    ����dst�׵�ַ
**/ 
WCHAR* iot_strtows(WCHAR* dst, const char* src)
{
   WCHAR* rlt = dst;
   while(*src)
   {
       *dst++ = *src++;
   }
   *dst = 0;
   
   return (rlt);
}


