#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

extern uint8_t VC_TASK_ENABLE;
extern uint8_t PLL_ENABLE;

void Task_VC();
void Task_Vc_Loop();
void task_pll_loop(TIM_HandleTypeDef *htim);

#ifdef __cplusplus
}
#endif
