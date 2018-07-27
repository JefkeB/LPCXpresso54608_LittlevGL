#include "draw_util.h"
#include "display_config.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/

//#define BYTES_PER_LINE (IMG_WIDTH / PIXEL_PER_BYTE)
#define BYTES_PER_LINE (IMG_WIDTH * 2)

typedef uint8_t(*LineBuf)[BYTES_PER_LINE];


/*******************************************************************************
 * Variables
 ******************************************************************************/

//__attribute__((aligned(8)))
__attribute__(( section(".noinit.$RAM4"), aligned(16) )) static uint16_t s_frameBuf0[IMG_HEIGHT*IMG_WIDTH];

//__attribute__((aligned(8)))
__attribute__(( section(".noinit.$RAM4"), aligned(16) )) static uint16_t s_frameBuf1[IMG_HEIGHT*IMG_WIDTH];

const uint16_t *frame_buf_addr[] = {s_frameBuf0,
                                    s_frameBuf1};

static uint8_t inactive_buf_idx = 1;

/*******************************************************************************
 * Private
 ******************************************************************************/

/*******************************************************************************
 * Public
 ******************************************************************************/
uint16_t *FrameAddr(uint8_t idx) {
  return (uint16_t*)frame_buf_addr[idx];
}

void Draw16BPP565Pixel(uint16_t x, uint16_t y, lv_color_t color) {
	((uint16_t *)frame_buf_addr[0])[(y * IMG_WIDTH) + x] = color.full;
}
