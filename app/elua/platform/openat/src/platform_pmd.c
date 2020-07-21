/**************************************************************************
 *              Copyright (C), AirM2M Tech. Co., Ltd.
 *
 * Name:    platform_pmd.c
 * Author:  liweiqiang
 * Version: V0.1
 * Date:    2013/3/28
 *
 * Description:
 *          lua pmd API
  * History:
 *     panjun 2015.06.09 OS don't control LCD's power, only LUA.
 **************************************************************************/

#include <stdio.h>

#if 1
#include "lplatform.h"
#include "platform_pmd.h"

static const E_AMOPENAT_PM_LDO ldo2OpenatLdo[PLATFORM_LDO_QTY] = {
   
    OPENAT_LDO_POWER_VLCD,

   
    OPENAT_LDO_POWER_MMC,
	/*+\new\wj\2020.4.14\��ӵ�ѹ��VSIM1����gpio29��30��31*/
	OPENAT_LDO_POWER_VSIM1,
	/*-\new\wj\2020.4.14\��ӵ�ѹ��VSIM1����gpio29��30��31*/

	/*+\new\shenyuanyuan\2020.5.21\ģ����VCAM���*/
	OPENAT_LDO_POWER_VCAMA,
	OPENAT_LDO_POWER_VCAMD
	/*-\new\shenyuanyuan\2020.5.21\ģ����VCAM���*/
};

/*+\NEW\liweiqiang\2013.9.8\����pmd.init���ó������ӿ� */
extern BOOL cust_pmd_init(PlatformPmdCfg *cfg);

static E_OPENAT_CHARGE_CURRENT getOpenatCurrent(u16 current)
{
/*+\NEW\RUFEI\2015.5.8\���Ƴ�����*/
    static const u16 openatCurrentVal[OPENAT_PM_CHARGE_CURRENT_QTY] =
    {
        0,
        20,
        30,
        40,
        50,
        60,
        70,
        200,
        300,
        400,
        500,
        600,
        700,
        800,
        900,
        1000,
        1100,
        1200,
        1300,
        1400,
        1500
    };
/*-\NEW\RUFEI\2015.5.8\���Ƴ�����*/
    uint16 i;

    for(i = 1/*OFF��ȥ�ж�*/; i < OPENAT_PM_CHARGE_CURRENT_QTY; i++)
    {
        if(openatCurrentVal[i] == current)
        {
/*+\BUG WM-1015\rufei\2013.11.19\ �޸�lua������*/
            return i;
/*-\BUG WM-1015\rufei\2013.11.19\ �޸�lua������*/
        }
        else if(openatCurrentVal[i] > current)
        {
            break;
        }
    }

    return OPENAT_PM_CHARGE_CURRENT_QTY;
}
/*+\NEW\RUFEI\2015.5.8\���Ƴ�����*/
static E_OPENAT_PM_VOLT getOpenatVolt(u16 volt)
{
    static const u16 openatVoltVal[OPENAT_PM_VOLT_QTY] =
    {
        0,
        1800,
        2800,
        3000,
        3200,
        3400,
        3600,
        3800,
        3850,
        3900,
        4000,
        4050,
        4100,
        4120,
        4130,
        4150,
        4160,
        4170,
        4180,
        4200,
        4210,
        4220,
        4230,
        4250,
        4260,
        4270,
        4300,
        4320,
        4350,
        4370,
        4400,
        4420
    };
    uint16 i;

    for(i = 1/*OFF��ȥ�ж�*/; i < OPENAT_PM_VOLT_QTY; i++)
    {
        if(openatVoltVal[i] >= volt)
        {
            return i;
        }
    }

    return OPENAT_PM_VOLT_QTY;
}
/*-\NEW\RUFEI\2015.5.8\���Ƴ�����*/

int platform_pmd_init(PlatformPmdCfg *pmdCfg)
{
    #define CHECK_FILED(fIELD) do{ \
        if(pmdCfg->fIELD != PMD_CFG_INVALID_VALUE && (pmdCfg->fIELD = getOpenatCurrent(pmdCfg->fIELD)) == OPENAT_PM_CHARGE_CURRENT_QTY) \
        { \
            PUB_TRACE("[platform_pmd_init]: error filed " #fIELD); \
            return PLATFORM_ERR; \
        } \
    }while(0)
/*+\NEW\RUFEI\2015.5.8\���Ƴ�����*/
    #define CHECK_VOLTAGE_FILED(fIELD) do{ \
        if(pmdCfg->fIELD != PMD_CFG_INVALID_VALUE && (pmdCfg->fIELD = getOpenatVolt(pmdCfg->fIELD)) == OPENAT_PM_VOLT_QTY) \
        { \
            PUB_TRACE("[platform_pmd_init]: error filed " #fIELD); \
            return PLATFORM_ERR; \
        } \
    }while(0)

    CHECK_FILED(ccCurrent);
    CHECK_FILED(fullCurrent);
    
    CHECK_VOLTAGE_FILED(ccLevel);
    CHECK_VOLTAGE_FILED(cvLevel);
    CHECK_VOLTAGE_FILED(ovLevel);
    CHECK_VOLTAGE_FILED(pvLevel);
    CHECK_VOLTAGE_FILED(poweroffLevel);
/*-\NEW\RUFEI\2015.5.8\���Ƴ�����*/

    return cust_pmd_init(pmdCfg) ? PLATFORM_OK : PLATFORM_ERR;
}
/*-\NEW\liweiqiang\2013.9.8\����pmd.init���ó������ӿ� */

int platform_ldo_set(PlatformLdoId id, int level)
{
    if(ldo2OpenatLdo[id] >= OPENAT_LDO_POWER_INVALID){
        return PLATFORM_ERR;
    }
	/*+\BUG\wangyuan\2020.04.07\����ldo���ýӿ�*/
    OPENAT_poweron_ldo(ldo2OpenatLdo[id], level);
	/*-\BUG\wangyuan\2020.04.07\����ldo���ýӿ�*/
    return PLATFORM_OK;
}


int platform_pmd_powersave(int sleep_wake)
{
    if(sleep_wake){
        /*+\NEW\liweiqiang\2013.10.19\���ù���ʱ��ƵΪ208M*/
        //IVTBL(sys_request_freq)(OPENAT_SYS_FREQ_32K);
        OPENAT_enter_deepsleep();
    } else {
        OPENAT_exit_deepsleep();
        //IVTBL(sys_request_freq)(OPENAT_SYS_FREQ_208M);
        /*-\NEW\liweiqiang\2013.10.19\���ù���ʱ��ƵΪ208M*/
    }

    return PLATFORM_OK;
}

/*+\NEW\liweiqiang\2014.2.13\����pmd.charger��ѯ�����״̬�ӿ� */
int platform_pmd_get_charger(void)
{
    UINT32 chargerStatus;
    
    //chargerStatus = IVTBL(get_chargerHwStatus)();
    return chargerStatus == OPENAT_PM_CHR_HW_STATUS_AC_ON ? 1 : 0;
}
/*-\NEW\liweiqiang\2014.2.13\����pmd.charger��ѯ�����״̬�ӿ� */



UINT32 platform_pmd_getChargingCurrent(void)
{
    return OPENAT_pmd_getChargingCurrent();
}

int platform_pmd_get_chg_param(BOOL *battStatus, u16 *battVolt, u8 *battLevel, BOOL *chargerStatus, u32 *chargeState)
{
#ifdef _LUA_TODO_
    AmGetBatInstantVolt(battVolt);
    *battLevel = AmGetBatteryPercent(*battVolt);

    //0:���ڳ�磻1:δ��磻2:������
    *chargeState = AmBatteryChargeStatus();
#endif
    return PLATFORM_OK;
}

#endif
