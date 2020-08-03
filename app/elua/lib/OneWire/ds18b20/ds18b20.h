#ifndef __ONEWIRE_DS18B20_H
#define __ONEWIRE_DS18B20_H

#include "OneWire.h"


//pinȡֵ0��1��2��3��7��ӦGPIO0��GPIO1��GPIO2��GPIO3��GPIO7���⼸��������������ȡ�����ߣ�������û�в���


/**
 * @description: ��ds18b20�õ��¶�ֵ���֣���ȷ��0.0625�����������10000��
 * @param : pin{uint8}:Ҫ���������ţ���ѡ��Χ0��1��2��3��7
 *          TempNum{int *}:����¶�ֵ
 * @return  0:������ȡ
 *          2:�����pin��������Χ
 *          3:δ��⵽ds18b20
 */
uint8 DS18B20_GetTemp_Num(uint8 pin, int *TempNum);


/**
 * @description: ��ds18b20�õ��¶�ֵ�ַ���
 * @param : pin{uint8}:Ҫ���������ţ���ѡ��Χ0��1��2��3��7
 *          TempNum{char *}:����¶��ַ���
 * @return  0:������ȡ
 *          1:TempStr=NULL
 *          2:�����pin��������Χ
 *          3:δ��⵽ds18b20
 */
uint8 DS18B20_GetTemp_String(uint8 pin, char *TempStr);
#endif
