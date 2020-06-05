#include "iot_fota.h"


/**Զ��������ʼ��
*@return	0:   ��ʾ�ɹ�
*           <0��  ��ʾ�д���
**/
E_OPENAT_OTA_RESULT iot_fota_init(void)
{
    return openat_otaInit();
}

/**Զ����������
*@param		data:				���ع̼�������
*@param		len:				���ع̼�������
*@param		total:				�̼����ܴ�С
*@return	0:   ��ʾ�ɹ�
*           <0��  ��ʾ�д���
**/
E_OPENAT_OTA_RESULT iot_fota_download(const char* data, UINT32 len, UINT32 total)
{
    return openat_otaProcess(data, len,total);
}

/**Զ������
*@return	0:   ��ʾ�ɹ�
*           <0��  ��ʾ�д���
**/
E_OPENAT_OTA_RESULT iot_fota_done(void)
{
    return openat_otaDone();
}


