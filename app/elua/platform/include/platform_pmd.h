/**************************************************************************
 *              Copyright (C), AirM2M Tech. Co., Ltd.
 *
 * Name:    platform_pmd.h
 * Author:  liweiqiang
 * Version: V0.1
 * Date:    2013/3/26
 *
 * Description:
 *          platform power manage �ӿ�
 **************************************************************************/

#ifndef _PLATFORM_PMD_H_
#define _PLATFORM_PMD_H_

typedef enum PlatformLdoIdTag
{
    PLATFORM_LDO_VLCD,
    PLATFORM_LDO_VMMC,
	/*+\new\wj\2020.4.14\��ӵ�ѹ��VSIM1����gpio29��30��31*/
    PLATFORM_LDO_VSIM1,
	/*-\new\wj\2020.4.14\��ӵ�ѹ��VSIM1����gpio29��30��31*/
	/*+\new\shenyuanyuan\2020.5.21\ģ����VCAM���*/
	PLATFORM_LDO_VCAMA,
	PLATFORM_LDO_VCAMD,
	/*-\new\shenyuanyuan\2020.5.21\ģ����VCAM���*/
	/*+\BUG\wangyuan\2020.08.22\BUG_2883:lua����820GPS������������*/
	PLATFORM_LDO_VIBR,
	/*-\BUG\wangyuan\2020.08.22\BUG_2883:lua����820GPS������������*/
	/*+\BUG3154\zhuwangbin\2020.10.10\���backlight����*/
	PLATFORM_LDO_VBACKLIGHT_R,
	PLATFORM_LDO_VBACKLIGHT_G,
	PLATFORM_LDO_VBACKLIGHT_B,
	PLATFORM_LDO_VBACKLIGHT_W,
	/*-\BUG3154\zhuwangbin\2020.10.10\���backlight����*/
	
	/*+\BUG3753\zhuwangbin\2020.12.4\���audio hmic bias ldo����*/
	PLATFORM_LDO_POWER_HMICBIAS,
	/*-\BUG3753\zhuwangbin\2020.12.4\���audio hmic bias ldo����*/
    PLATFORM_LDO_QTY
}PlatformLdoId;

/*+\NEW\liweiqiang\2013.9.8\����pmd.init���ó������ӿ� */
/*+\NEW\liweiqiang\2014.2.8\���Ƶ�Դ�������ýӿ� */
#define PMD_CFG_INVALID_VALUE           (0xffff)

typedef struct PlatformPmdCfgTag
{
/*+\NEW\RUFEI\2015.5.8\���Ƴ�����*/
    u16             ccLevel;/*�����׶�:4.1*/
    u16             cvLevel;/*��ѹ�׶�:4.2*/
    u16             ovLevel;/*������ƣ�4.3*/
    u16             pvLevel;/*�س�4.1*/
    u16             poweroffLevel;/*�ػ���ѹ��3.4�������ڼ�������ٷֱȣ�ʵ�����ϲ���ƹػ�*/
    u16             ccCurrent;/*�����׶ε���*/
    u16             fullCurrent;/*��ѹ����������30*/
/*-\NEW\RUFEI\2015.5.8\���Ƴ�����*/
    /*+\NEW\zhuth\2014.11.6\��Դ�������ò���������Ƿ����ص�����*/
    u16             batdetectEnable;
    /*-\NEW\zhuth\2014.11.6\��Դ�������ò���������Ƿ����ص�����*/
}PlatformPmdCfg;
/*-\NEW\liweiqiang\2014.2.8\���Ƶ�Դ�������ýӿ� */

int platform_pmd_init(PlatformPmdCfg *pmdCfg);
/*-\NEW\liweiqiang\2013.9.8\����pmd.init���ó������ӿ� */

int platform_ldo_set(PlatformLdoId id, int level);

//sleep_wake: 1 sleep 0 wakeup
int platform_pmd_powersave(int sleep_wake);

/*+\NEW\liweiqiang\2014.2.13\����pmd.charger��ѯ�����״̬�ӿ� */
int platform_pmd_get_charger(void);
/*-\NEW\liweiqiang\2014.2.13\����pmd.charger��ѯ�����״̬�ӿ� */

#endif//_PLATFORM_PMD_H_
