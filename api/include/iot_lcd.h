#ifndef __IOT_LCD_H__
#define __IOT_LCD_H__

#include "iot_os.h"

/**
 * @ingroup iot_sdk_device ����ӿ�
 * @{
 */
/**
 * @defgroup iot_sdk_lcd lcd�ӿ�
 * @{
 */
 
/**@example zbar/demo_zbar_lcd.c
* LCD&���̽ӿ�ʾ��
*/ 

/**д�� lcd����
*@param		cmd: ����
**/
VOID iot_lcd_write_cmd(                          
                        UINT8 cmd 
                   );

/**lcd д��lcd���� 
*@param	 	data: ����
**/
VOID iot_lcd_write_data(                               
                        UINT8 data                
                );


/**lcd��ʼ��
*@param		param: lcd��ʼ������
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/	
BOOL iot_lcd_color_init(T_AMOPENAT_COLOR_LCD_PARAM *param );

/**  ˢ��lcd
*@param		rect: ��Ҫˢ�µ�����
*@param		pDisplayBuffer: ˢ�µĻ�����
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
VOID iot_lcd_update_color_screen(
				T_AMOPENAT_LCD_RECT_T* rect,        /* ��Ҫˢ�µ����� */
				UINT16 *pDisplayBuffer    );

/** ����jpg��ʽͼƬ
*@param		filename: �ļ�·�������ļ���
*@param		imageinfo: �ļ���ʽ
*@return	INT32: ����״̬��
**/
INT32 iot_decode_jpeg(
                    CONST char * filename,
                    T_AMOPENAT_IMAGE_INFO *imageinfo
                    );

/** �ͷ�jpg��ʽ��������
*@param		buffer: ������ʾbuffer
*@return	INT32: �ͷ�״̬��
**/
INT32 iot_free_jpeg_decodedata(
                    INT16* buffer
                    );
/** @}*/
/** @}*/
#endif









