/**************************************************************************
 *              Copyright (C), AirM2M Tech. Co., Ltd.
 *
 * Name:    platform_lcd.h
 * Author:  liweiqiang
 * Version: V0.1
 * Date:    2013/3/26
 *
 * Description:
 *          platform lcd �ӿ�
 **************************************************************************/

#ifndef _PLATFORM_LCD_H_
#define _PLATFORM_LCD_H_

#include "platform_disp.h"

void platform_lcd_init(const PlatformDispInitParam *pParam);
/*+\bug2958\czm\2020.9.1\disp.close() ֮����ִ��disp.init ����ʾֱ������*/
void platform_lcd_close(void);
/*+\bug2958\czm\2020.9.1\disp.close() ֮����ִ��disp.init ����ʾֱ������*/
void platform_lcd_update(PlatformRect *pRect, u8 *buffer);

#endif//_PLATFORM_LCD_H_