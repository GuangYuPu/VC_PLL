#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOS.h"
#include "task.h"
#include "Coordinate_Trans.h"
#include "FeedBack.h"
#include "SVPWM.h"
#include "User_VC_Thread.h"
#include "VC_Controller.h"
#include "PLL.h"
#include "high_precision_time.h"

#define W_VC (2*PI*f_ref)

void task_vc_start();
void task_pll_start();

#ifdef __cplusplus
}
#endif
