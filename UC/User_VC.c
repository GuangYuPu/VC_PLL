#include "User_VC.h"

void vc_loop(void *argument)
{
    (void)argument;
    Task_VC();
    while(1) {
        Task_Vc_Loop();
        vTaskDelay(1);
    }
}

void pll_loop_start(void *argument)
{
    (void)argument;
    PLL_init(0.1,0,50);
    vTaskDelete(NULL); // 删除当前线程
}

void task_vc_start()
{
    xTaskCreate(vc_loop, "vc_loop", 1024, NULL, 4, NULL);
}

void task_pll_start()
{
    xTaskCreate(pll_loop_start, "pll_init", 128, NULL, 2, NULL);
}