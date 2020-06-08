#include "string.h"
#include "iot_debug.h"
#include "iot_gpio.h"
#include "iot_pmd.h"

#define gpio_print iot_debug_print
#define DEMO_GPIO_8 8
#define DEMO_GPIO_9 9

VOID demo_gpio_handle (E_OPENAT_DRV_EVT evt, 
                    E_AMOPENAT_GPIO_PORT gpioPort,
                unsigned char state)
{
    UINT8 status;

    // �ж���gpio�ж�
    if (OPENAT_DRV_EVT_GPIO_INT_IND == evt)
    {
        // �жϴ����жϵĹܽ�
        if (DEMO_GPIO_8 == gpioPort)
        {   
            // ������ƽ��״̬
            gpio_print("[gpio] input handle gpio %d, state %d", gpioPort, state);

            // ����ǰgpio״̬, 1:�ߵ�ƽ 0:�͵�ƽ
            iot_gpio_read(gpioPort, &status);
            gpio_print("[gpio] input handle gpio %d, status %d", gpioPort, state);
            
        }
    }
}


VOID demo_gpio_input(VOID)
{
    T_AMOPENAT_GPIO_CFG  input_cfg;
    BOOL err;
    
    memset(&input_cfg, 0, sizeof(T_AMOPENAT_GPIO_CFG));
    
    input_cfg.mode = OPENAT_GPIO_INPUT_INT; //���������ж�
    input_cfg.param.defaultState = FALSE;    
    input_cfg.param.intCfg.debounce = 50;  //����50ms
    input_cfg.param.intCfg.intType = OPENAT_GPIO_INT_BOTH_EDGE; //�жϴ�����ʽ˫����
    input_cfg.param.intCfg.intCb = demo_gpio_handle; //�жϴ�����
    err = iot_gpio_open(DEMO_GPIO_8, &input_cfg);

    if (!err)
        return;

    gpio_print("[gpio] set gpio8 input");
}

VOID demo_gpio_output(VOID)
{
    T_AMOPENAT_GPIO_CFG  output_cfg;
    BOOL err;
    
    memset(&output_cfg, 0, sizeof(T_AMOPENAT_GPIO_CFG));
    
    output_cfg.mode = OPENAT_GPIO_OUTPUT; //�������
    output_cfg.param.defaultState = FALSE; // Ĭ�ϵ͵�ƽ

    err = iot_gpio_open(DEMO_GPIO_9, &output_cfg);

    if (!err)
        return;
        
    iot_gpio_set(DEMO_GPIO_9, TRUE); //����Ϊ�ߵ�ƽ
    
    gpio_print("[gpio] set gpio9 output");
}

VOID demo_gpio_init(VOID)
{
    demo_gpio_output(); //����gpio8Ϊ���
    demo_gpio_input(); //����gpio9Ϊ����
}

int appimg_enter(void *param)
{    
    gpio_print("[gpio] app_main");

    demo_gpio_init();

    return 0;
}

void appimg_exit(void)
{
    gpio_print("[gpio] appimg_exit");
}