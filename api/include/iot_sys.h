#ifndef __IOT_SYS_H__
#define __IOT_SYS_H__

#include "iot_os.h"
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

/**ota����new core���ļ���������֪�ײ���Ҫ���ļ���ȡ�����µĳ���
*@param		newCoreFile:		�³����ļ� 
*@return	TRUE: �ɹ�   FALSE: ʧ��
*ע��newCoreFile�ļ����뱣����/fota�ļ�����,����newCoreFile Ϊ "/fota/core.img"
**/
BOOL iot_ota_newcore(              
                    CONST char* newCoreFile
               );


/**ota����new app���ļ���������֪�ײ���Ҫ���ļ���ȡ�����µĳ���
*@param		newAPPFile:		�³����ļ� 
*@return	TRUE: �ɹ�   FALSE: ʧ��
*ע��newAPPFile�ļ����뱣����/fota�ļ�����,����newAPPFile Ϊ "/fota/app.img"
**/
BOOL iot_ota_newapp(              
                    CONST char* newAPPFile
               );


/**��char����ת��ΪWCHAR�����������Ϊiot_fs_open_file�Ƚӿڵ��ļ�������
*@param     dst:        ת��������
*@param     src:        �ȴ�ת�����ַ���
*@return    ����dst�׵�ַ
**/ 
WCHAR* iot_strtows(WCHAR* dst, const char* src);

/** @}*/

#endif

