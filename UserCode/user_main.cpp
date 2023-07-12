#include "user_main.h"
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"

#include "User_VC.h"


void StartDefaultTask(void const *argument)
{
    (void)argument;
    
    task_vc_start();
    task_pll_start();

    while (true) {
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
        vTaskDelay(200);
    }

    vTaskDelete(nullptr); // 删除当前线程
}
