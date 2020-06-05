#ifndef __IOT_FOTA_H__
#define __IOT_FOTA_H__

#include "openat_ota.h"

/**
 * @defgroup iot_sdk_fota Զ�������ӿ�
 * @{
 */
	/**@example fota/demo_fota.c
	* OTA�ӿ�ʾ��
	*/ 


/**Զ��������ʼ��
*@return	0:   ��ʾ�ɹ�
*           <0��  ��ʾ�д���
**/
E_OPENAT_OTA_RESULT iot_fota_init(void);

/**Զ����������
*@param		data:				���ع̼�������
*@param		len:				���ع̼�������
*@param		total:				�̼����ܴ�С
*@return	0:   ��ʾ�ɹ�
*           <0��  ��ʾ�д���
**/
E_OPENAT_OTA_RESULT iot_fota_download(const char* data, UINT32 len, UINT32 total);

/**Զ������
*@return	0:   ��ʾ�ɹ�
*           <0��  ��ʾ�д���
**/
E_OPENAT_OTA_RESULT iot_fota_done(void);


#endif

