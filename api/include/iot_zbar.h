#ifndef __IOT_ZBAR_H__
#define __IOT_ZBAR_H__
#include "iot_os.h"

/**
 * @defgroup iot_sdk_zbar ��ά������ӿ�
 * @{
 */
 
/**@example zbar/demo_zbar.c
* ��ά��ӿ�ʾ��
*/ 

/**������ά�룬���ؾ��handle
*@param		width:		����ͼƬ�Ŀ��
*@param		height:		����ͼƬ�ĸ߶�
*@param		data:		����ͼƬ������
*@param		size:		����ͼƬ���ݴ�С
*@return	��ȡ���handle�� ������0��ʾ�����ɹ�
*           
**/
int iot_zbar_scannerOpen(int width, int height, int size, unsigned char *data);

/**��ȡ������ά�������
*@param		handle:		���,��iot_zbar_scannerOpen����
*@param		len:		��ȡ���ݵĳ���
*@return	��ȡ�����ݵ�ַ
**/
char * iot_zbar_getData(int handle, int *len);

/**�鿴�Ƿ�����һ������
*@param		handle:		���,��iot_zbar_scannerOpen����
*@return	TRUE:   ������һ������
*			FALSE:  û����һ������
**/
BOOL iot_zbar_findNextData(int handle);

/**��ȡ��ǰ���ݱ���ĸ�ʽ
*@param		handle:		���,��iot_zbar_scannerOpen����
*@return	���صĸ�ʽ����
**/
char * iot_zbar_getType(int handle);

/**�ͷž��
*@param		handle:		���,��iot_zbar_scannerOpen����
*@return    void	
**/
void iot_zbar_scannerClose(int handle);

/** @}*/
#endif
