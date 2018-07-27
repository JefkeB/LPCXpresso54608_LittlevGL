/*
 * Copyright (c) 2017, NXP Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include "rtos_app.h"
#include "clock_config.h"
#include "pin_mux.h"
#include "board.h"
#include <stdio.h>

/*
 * @brief   Application entry point.
 */
int main(void) {
	/* attach 12 MHz clock to FLEXCOMM0 (debug console) */
	CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

	/* Route Main clock to LCD. */
	CLOCK_AttachClk(kMCLK_to_LCD_CLK);

	/* attach 12 MHz clock to FLEXCOMM2 (I2C master for accelerometer) */
	CLOCK_AttachClk(kFRO12M_to_FLEXCOMM2);

	CLOCK_EnableClock(kCLOCK_Gpio2);

	CLOCK_SetClkDiv(kCLOCK_DivLcdClk, 1, true);

	BOARD_InitPins();
	BOARD_BootClockPLL180M();
	//BOARD_InitDebugConsole();
	BOARD_InitSDRAM();
	//BOARD_BootClockFROHF48M();

	RTOS_RunApp();

	while (1) {
		__WFI();
	}
	return 0;
}
