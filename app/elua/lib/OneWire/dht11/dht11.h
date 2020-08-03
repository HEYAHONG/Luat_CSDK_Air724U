#ifndef __ONEWIRE_DHT11_H
#define __ONEWIRE_DHT11_H

#include "OneWire.h"

//pinȡֵ0��1��2��3��7��ӦGPIO0��GPIO1��GPIO2��GPIO3��GPIO7���⼸��������������ȡ�����ߣ�������û�в���

/**
 * @description: ��dht11�õ���/ʪ��ֵ����
 * @param : pin{uint8}:Ҫ���������ţ���ѡ��Χ0��1��2��3��7
 *          HumNum{uint8 *}:����¶�ֵ
 *          TemNum{uint8 *}:���ʪ��ֵ
 * @return  0:������ȡ
 *          2:�����pin��������Χ
 *          3:δ��⵽dht11
 *          4:����У�����
 */

uint8 DHT11_GetData_Num(uint8 pin, uint8 *HumNum, uint8 *TemNum);
/**
 * @description: ��dht11�õ���/ʪ��ֵ�ַ���
 * @param : pin{uint8}:Ҫ���������ţ���ѡ��Χ0��1��2��3��7
 *          HumStr{char *}:����¶�ֵ�ַ���
 *          TemStr{char *}:���ʪ��ֵ�ַ���
 * @return  0:������ȡ
 *          1:HumStr == NULL || TemStr == NULL
 *          2:�����pin��������Χ
 *          3:δ��⵽dht11
 *          4:����У�����
 */
uint8 DHT11_GetData_String(uint8 pin, char *HumStr, char *TemStr);
#endif
