/**************************************************************************
 *              Copyright (C), AirM2M Tech. Co., Ltd.
 *
 * Name:    platform_disp.h
 * Author:  liweiqiang
 * Version: V0.1
 * Date:    2013/3/26
 *
 * Description:
 *          platform display �ӿ�
 **************************************************************************/

#ifndef _PLATFORM_DISP_H_
#define _PLATFORM_DISP_H_

#include <sys/queue.h>

typedef struct PlatformRectTag
{
    u16 ltx;        //left top x y
    u16 lty;
    u16 rbx;        //right bottom x y
    u16 rby;
}PlatformRect;

// ��ɫ���� RGB(5,6,5)
#define COLOR_WHITE_16 0xffff
#define COLOR_BLACK_16 0x0000
#define COLOR_WHITE_24 0xffffff
#define COLOR_BLACK_24 0x000000

#define COLOR_WHITE_1 0xff
#define COLOR_BLACK_1  0x00

#define BASIC_LAYER_ID  0
#define USER_LAYER_1_ID  1
#define USER_LAYER_2_ID  2
#define INVALID_LAYER_ID  -1


typedef enum PlatformLcdBusTag
{
    PLATFORM_LCD_BUS_SPI4LINE,
    PLATFORM_LCD_BUS_PARALLEL,

/*+\new\liweiqiang\2014.10.11\���lcd i2c spi�ӿ� */
    PLATFORM_LCD_BUS_I2C,
    PLATFORM_LCD_BUS_SPI,
/*-\new\liweiqiang\2014.10.11\���lcd i2c spi�ӿ� */
    
    PLATFORM_LCD_BUS_QTY,
}PlatformLcdBus;

/*+\new\liweiqiang\2014.10.11\���lcd i2c spi�ӿ� */
typedef union {
    struct {
        int bus_id;
        int pin_rs;
        int pin_cs;
        int freq;
    } bus_spi;
    
    struct {
        int bus_id;
        int freq;
        int slave_addr;
        int cmd_addr;
        int data_addr;
    } bus_i2c;
} lcd_itf_t;
/*-\new\liweiqiang\2014.10.11\���lcd i2c spi�ӿ� */

typedef struct PlatformDispInitParamTag
{
    u16 width;  // lcd�豸���
    u16 height; // lcd�豸�߶�
    u8  bpp; // bits per pixel lcd�豸ɫ�� 1:�ڰ� 16:16λɫ����
    u16 x_offset;
    u16 y_offset;
    u32 *pLcdCmdTable;    //lcd��ʼ��ָ���
    u16 tableSize;         //lcd��ʼ��ָ�����С
/*+\NEW\liweiqiang\2013.12.18\����lcd˯������֧�� */
    u32 *pLcdSleepCmd;  // lcd sleepָ���
    u16 sleepCmdSize;
    u32 *pLcdWakeCmd;   // lcd wakeָ���
    u16 wakeCmdSize;
/*-\NEW\liweiqiang\2013.12.18\����lcd˯������֧�� */
    PlatformLcdBus bus;
/*+\new\liweiqiang\2014.10.11\���lcd i2c�ӿ� */
    lcd_itf_t lcd_itf;
/*-\new\liweiqiang\2014.10.11\���lcd i2c�ӿ� */
    int pin_rst; //reset pin
    /*+\new\liweiqiang\2014.10.21\���Ӳ�ͬ�ڰ������ɫ���� */
    int hwfillcolor; // lcd�������ɫ
    /*-\new\liweiqiang\2014.10.21\���Ӳ�ͬ�ڰ������ɫ���� */
/*+\NEW\2013.4.10\���Ӻڰ�����ʾ֧�� */
    int pin_cs; // cs pin
/*+\new\czm\2020.9.6\bug:2964 mono_std_spi_st7571.lua �޷�����ʹ�� */
    int pin_rs; // rs pin
/*-\new\czm\2020.9.6\bug:2964 mono_std_spi_st7571.lua �޷�����ʹ�� */
	/*+\bug2406\zhuwangbin\2020.6.28\����ͷɨ��Ԥ��ʱ��Ҫ֧�������Ƿ�ˢ����ʾ���� */
    u8 camera_preview_no_update_screen;
	/*-\bug2406\zhuwangbin\2020.6.28\����ͷɨ��Ԥ��ʱ��Ҫ֧�������Ƿ�ˢ����ʾ���� */
    u8 *framebuffer;
/*-\NEW\2013.4.10\���Ӻڰ�����ʾ֧�� */

    /*+\BUG:3316\czm\2020.10.16\LCD_SPI ������������ϣ������ǿ��������*/  
    u8 Driving;//lcd_spi�������������ֵΪ15
    /*-\BUG:3316\czm\2020.10.16\LCD_SPI ������������ϣ������ǿ��������*/  
}PlatformDispInitParam;

typedef SLIST_ENTRY(platform_layer) platform_layer_iter_t;
typedef SLIST_HEAD(platform_layer_head, platform_layer) platform_layer_head_t;

typedef struct platform_layer
{
    platform_layer_iter_t iter;
	int id;
	int height;
	int width;
	int bpp;
	void *buf;
	void (*update) (PlatformRect *area, struct platform_layer *layer);
    void (*destroy) (struct platform_layer *layer);
} platform_layer_t;

int platform_disp_create_layer(platform_layer_t *layer);

int platform_disp_set_act_layer(int id);

void platform_disp_destroy_layer(int id);

void platform_disp_init(PlatformDispInitParam *pParam);

void platform_disp_close(void);

void platform_disp_clear(void);

void platform_disp_update(void);

u16* platform_disp_puttext(const char *string, u16 x, u16 y);

/*+\NEW\liweiqiang\2013.12.6\����ͼƬ͸��ɫ���� */
/*+\NEW\liweiqiang\2013.11.4\����BMPͼƬ��ʾ֧�� */
/*+\NewReq NEW\zhutianhua\2013.12.24\��ʾͼƬ��ָ������*/
int platform_disp_putimage(const char *filename, u16 x, u16 y, int transcolor, u16 left, u16 top, u16 right, u16 bottom,int transtype);
/*-\NewReq NEW\zhutianhua\2013.12.24\��ʾͼƬ��ָ������*/
/*-\NEW\liweiqiang\2013.11.4\����BMPͼƬ��ʾ֧�� */
/*-\NEW\liweiqiang\2013.12.6\����ͼƬ͸��ɫ���� */

void platform_layer_start_move(int layer_id1, int layer_id2,int layer_id3,int delay_ms,int x_inc,int y_inc);

void platform_disp_playgif(const char* gif_file_name, int x, int y,  int times);


void platform_disp_stopgif(void);


int platform_disp_preload_png_foreground(const char *filename, int index);

int platform_disp_preload_png_background(const char *filename);

int platform_disp_move_foreimg(int x1, int y1, int x2, int y2);


/*+\NEW\liweiqiang\2013.12.7\���Ӿ�����ʾ֧�� */
int platform_disp_drawrect(int x1, int y1, int x2, int y2, int color);
/*-\NEW\liweiqiang\2013.12.7\���Ӿ�����ʾ֧�� */

/*+\NEW\liweiqiang\2013.12.9\����ǰ��ɫ\����ɫ���� */
int platform_disp_setcolor(int color);
int platform_disp_setbkcolor(int color);
/*-\NEW\liweiqiang\2013.12.9\����ǰ��ɫ\����ɫ���� */

/*+\NEW\liweiqiang\2013.12.9\���ӷ������������� */
int platform_disp_loadfont(const char *name);
int platform_disp_setfont(int id);
/*-\NEW\liweiqiang\2013.12.9\���ӷ������������� */
/*+NEW\brezen\2016.05.13\��������*/  
int platform_disp_setfontHeight(int height);
int platform_disp_getfontHeight(void);
/*-NEW\brezen\2016.05.13\��������*/  
/******************************************************************************
***func name---- platform_disp_preload_png_to_layer
***param    ---- filename: Ҫװ�ص�PNGͼƬ����
                 layer_id: ͼƬװ�ص�Ŀ��ͼ��ID
***desc     ---- Ԥװ��PNGͼƬ��ͼ��
***return   ---- �Ƿ�ɹ�
***note     
******************************************************************************/
int platform_disp_preload_png_to_layer(const char *filename, int layer_id);





/******************************************************************************
***func name---- platform_create_user_layer
***param    ---- layer_id:Ҫ�������û�ͼ��
            -----layer_width:ͼ����
            -----layer_height:ͼ��߶�
***desc     ---- �����û�ͼ��
***return   ---- �Ƿ�ɹ�
***note     
-----1.�ڴ���֮ǰ����ȷ����ͼ���δ�����������Ѿ�����
-----2.Ŀǰֻ֧�������û�ͼ��Ĵ���
-----3.�����㲻�ô�����ϵͳ�Զ�����
-----4.�û�ͼ���Ĭ�ϴ�СΪ��Ļ��С��Ĭ�ϸ�ʽΪ24λ
******************************************************************************/
int platform_create_user_layer(int layer_id, int start_x, int start_y, int layer_width, int layer_height);




/******************************************************************************
***func name---- platform_destroy_user_layer
***param    ---- layer_id:Ҫ���ٵ��û�ͼ��
***desc     ---- �����û�ͼ��
***return   ---- �Ƿ�ɹ�
***note     
-----1. layer_idֻ����USER_LAYER_1_ID����USER_LAYER_2_ID
-----2. ͼ��ʹ�����֮��Ӧ�þ����ͷţ������ռ��ϵͳ�ڴ�
******************************************************************************/
int platform_destroy_user_layer(int layer_id);



/******************************************************************************
***func name---- platform_set_active_layer
***param    ---- layer_id:Ҫ�����ͼ��ID
***desc     ---- ����ͼ�㣬��LCD�����л�ͼ���������ڼ���ͼ���Ͻ���
***return   ---- NULL
***note     
******************************************************************************/
void platform_set_active_layer(int layer_id);




/******************************************************************************
***func name---- platform_swap_user_layer
***param    ---- NULL
***desc     ---- �Ի������û�ͼ������ݡ��˺������漰ͼ������ݿ���������ٶȺܿ졣
                 �����ڶ��ٶ�Ҫ��ߵ�����¿��ٽ��������û�ͼ�������.
***return   ---- NULL
***note     
******************************************************************************/
void platform_swap_user_layer(void);





/******************************************************************************
***func name---- platform_copy_layer
***param    ---- dst_layer_id: ҪCOPY��Ŀ��ͼ���ID
            -----display_x:  Դͼ����Ŀ��ͼ���ϵ�λ��X
            -----display_y:  Դͼ����Ŀ��ͼ���ϵ�λ��Y
            ---- src_layer_id: ҪCOPY��Դͼ���ID
            -----src_rect:  ��ҪCOPY��Դͼ�������
***desc     ---- ͼ�����ݿ�����
***return   ---- NULL
***note
******************************************************************************/
void platform_copy_layer(int dst_layer_id,
                                 int display_x,
                                 int display_y,
                                 int src_layer_id,
                                 T_AMOPENAT_LCD_RECT_T* src_rect);




/******************************************************************************
***func name---- platform_copy_to_basic_layer
***param    ---- layer_id1: Ҫ��ʾ��ͼ��1��ID
            -----x1:  Դͼ��1��LCD����ʾ����X.
            -----y1:  Դͼ��1��LCD����ʾ����y.
            ---- layer_id2: Ҫ��ʾ��ͼ��2��ID
            -----x2:  Դͼ��2��LCD����ʾ����X.
            -----y2:  Դͼ��2��LCD����ʾ����y.            
            ---- layer_id3: Ҫ��ʾ��ͼ��3��ID
            -----x2:  Դͼ��3��LCD����ʾ����X.
            -----y2:  Դͼ��3��LCD����ʾ����y.            
***desc     -----��ͼ�����ʾ��
***return   ----- NULL
***note     ----- �������Ҫ��ʾĳ��ͼ�㣬��layer_id��ΪINVALID_LAYER_ID����
******************************************************************************/
void platform_layer_flatten(int layer_id1, int x1, int y1, 
                                  int layer_id2, int x2, int y2,
                                  int layer_id3, int x3, int y3);



int platform_get_png_file_resolution(const char *filename, u32* width, u32* height);

/*+\BUG2739\lijiaodi\2020.08.06\���disp.new disp.getframe�ӿ�\*/ 
void platform_disp_new(PlatformDispInitParam *pParam);

int platform_disp_get();
/*-\BUG2739\lijiaodi\2020.08.06\���disp.new disp.getframe�ӿ�\*/ 
/*+\NEW\zhuwangbin\2020.05.01\���disp camera����*/
#define MAX_LCD_WIDTH_SUPPORT 240
#define MAX_LCD_HEIGH_SUPPORT 240

#define MAX_LCD_PIXEL_BYTES   3
#define MAX_LCD_BUFF_SIZE (MAX_LCD_WIDTH_SUPPORT*MAX_LCD_HEIGH_SUPPORT* MAX_LCD_PIXEL_BYTES)
/*-\NEW\zhuwangbin\2020.05.01\���disp camera����*/

#endif//_PLATFORM_DISP_H_
