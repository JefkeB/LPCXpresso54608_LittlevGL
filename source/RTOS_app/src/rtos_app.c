//
//
//
#include "touch_screen.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "event_groups.h"
#include "task.h"
#include "LPC54608.h"
#include <stdio.h>

#include "draw_util.h"
#include "lcd_tft.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define POS_READY (1 << 0)

#define RTOS_ACT 1

/*******************************************************************************
 * Globals
 ******************************************************************************/
static volatile TouchScreenPos pos;

/*******************************************************************************
 * Code
 ******************************************************************************/
#include "lvgl.h"
#include "demo.h"
#include "lv_test.h"
// define this to get the right RGB order for the lv_color_t
#define __BYTE_ORDER__=_
//
//
//
lv_res_t btn_action(lv_obj_t * btn) {
	printf("btn\r\n");

	return LV_RES_OK;
}


//
//
//
static void lvTask(void *pvParameters) {
#if 0
	lv_style_scr.body.main_color = LV_COLOR_RED;
	lv_style_scr.body.grad_color = LV_COLOR_RED;

    static lv_style_t style;
    lv_style_copy(&style, &lv_style_plain);
    style.text.color = LV_COLOR_BLUE;
    style.text.font = &lv_font_dejavu_20;   // Unicode and symbol fonts already assigned by the library

    // Create a Label on the currently active screen
    lv_obj_t * label1 =  lv_label_create(lv_scr_act(), NULL);
    //lv_obj_set_pos(label1, 20, 20);
    lv_label_set_style(label1, &style);
    lv_label_set_text(label1, "Hello world!");
    // Align the Label to the center
    // NULL means align on parent (which is the screen now)
    // 0, 0 at the end means an x, y offset after alignment
    lv_obj_align(label1, NULL, LV_ALIGN_CENTER, 0, 0);

	lv_obj_t * btn1 = lv_btn_create(lv_scr_act(), NULL);
    lv_btn_set_action(btn1, LV_BTN_ACTION_CLICK, btn_action);
    lv_obj_t * btn2 = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_align(btn2, btn1, LV_ALIGN_OUT_RIGHT_MID, 50, 0);


    // Align the Label to the center
    // NULL means align on parent (which is the screen now)
    // 0, 0 at the end means an x, y offset after alignment
    //lv_obj_align(label1, NULL, LV_ALIGN_CENTER, 0, 0);
#endif

    //demo_create();

    lv_test_theme_1(lv_theme_night_init(210, NULL));

	while(1) {
		vTaskDelay(1);
		lv_tick_inc(portTICK_RATE_MS);
		lv_task_handler();
	}
}


static void ex_disp_flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * color_p)
{
    /*The most simple case (but also the slowest) to put all pixels to the screen one-by-one*/

    int32_t x;
    int32_t y;
    for(y = y1; y <= y2; y++) {
        for(x = x1; x <= x2; x++) {
            /* Put a pixel to the display. For example: */
            /* put_px(x, y, *color_p)*/
        	Draw16BPP565Pixel(x, y, *color_p);
            color_p++;
        }
    }

    /* IMPORTANT!!!
     * Inform the graphics library that you are ready with the flushing*/
    lv_flush_ready();
}

#if 0
/* Write a pixel array (called 'map') to the a specific area on the display
 * This function is required only when LV_VDB_SIZE == 0 in lv_conf.h*/
static void ex_disp_map(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * color_p)
{
    /*The most simple case (but also the slowest) to put all pixels to the screen one-by-one*/

    int32_t x;
    int32_t y;
    for(y = y1; y <= y2; y++) {
        for(x = x1; x <= x2; x++) {
            /* Put a pixel to the display. For example: */
            /* put_px(x, y, *color_p)*/
        	DrawUtil_Draw16BPP565Pixel(x, y, *color_p);
            color_p++;
        }
    }
}


/* Write a pixel array (called 'map') to the a specific area on the display
 * This function is required only when LV_VDB_SIZE == 0 in lv_conf.h*/
static void ex_disp_fill(int32_t x1, int32_t y1, int32_t x2, int32_t y2,  lv_color_t color)
{
    /*The most simple case (but also the slowest) to put all pixels to the screen one-by-one*/

    int32_t x;
    int32_t y;
    for(y = y1; y <= y2; y++) {
        for(x = x1; x <= x2; x++) {
            /* Put a pixel to the display. For example: */
            /* put_px(x, y, *color)*/
        	DrawUtil_Draw16BPP565Pixel(x, y, color);
        }
    }
}
#endif

/* Read the touchpad and store it in 'data'
 * Return false if no more data read; true for ready again */
static bool ex_tp_read(lv_indev_data_t *data)
{
    /* Read your touchpad */
    /* data->state = LV_INDEV_STATE_REL or LV_INDEV_STATE_PR */
    /* data->point.x = tp_x; */
    /* data->point.y = tp_y; */


	if(TS_GetSingleTouch((TouchScreenPos *)&pos)) {
		data->state = LV_INDEV_STATE_PR;
		data->point.x = pos.pos_x;
		data->point.y = pos.pos_y;
	} else {
		data->state = LV_INDEV_STATE_REL;
	}

    return false;   /*false: no more data to read because we are no buffering*/
}


//
//
//
static void PrintTask(void *pvParameters) {
  while (1) {
    printf("test\n");
    vTaskDelay(configTICK_RATE_HZ / 1);
  }
}



//
//
//
void RTOS_RunApp(void) {
	LCD_Setup((uint32_t)FrameAddr(0));

	lv_init();

	lv_disp_drv_t disp_drv;                         /*Descriptor of a display driver*/
	lv_disp_drv_init(&disp_drv);                    /*Basic initialization*/

	/*Set up the functions to access to your display*/
	disp_drv.disp_flush = ex_disp_flush;            /*Used in buffered mode (LV_VDB_SIZE != 0  in lv_conf.h)*/

	//disp_drv.disp_fill = ex_disp_fill;              /*Used in unbuffered mode (LV_VDB_SIZE == 0  in lv_conf.h)*/
	//disp_drv.disp_map = ex_disp_map; 				/*Used in unbuffered mode (LV_VDB_SIZE == 0  in lv_conf.h)*/

	/*Finally register the driver*/
	lv_disp_drv_register(&disp_drv);


	/*************************
	* Input device interface
	*************************/
	/*Add a touchpad in the example*/
	TS_Init();
	lv_indev_drv_t indev_drv;                       /*Descriptor of an input device driver*/
	lv_indev_drv_init(&indev_drv);                  /*Basic initialization*/
	indev_drv.type = LV_INDEV_TYPE_POINTER;         /*The touchpad is pointer type device*/
	indev_drv.read = ex_tp_read;                 	/*Library ready your touchpad via this function*/
	lv_indev_drv_register(&indev_drv); 				/*Finally register the driver*/



	xTaskCreate(lvTask, "lvTask",
			  configMINIMAL_STACK_SIZE + 1024, NULL,
			  (tskIDLE_PRIORITY + 1UL), (xTaskHandle *)NULL);

    xTaskCreate(PrintTask, "PrintTask", configMINIMAL_STACK_SIZE, NULL,
	              (tskIDLE_PRIORITY + 1UL), (xTaskHandle *)NULL);


	/* Start the scheduler */
	vTaskStartScheduler();
}
