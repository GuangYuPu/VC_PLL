#pragma once

#ifdef __cplusplus
extern "C" {
#endif
#include "Coordinate_Trans.h"
#include "main.h"
#include "math.h"

extern float Ud,ia,ib,ic,ua,ub,uc;//直接采样值
extern CT_data_t Sample_i,Sample_u;//储存并处理采样数据
extern float amplitude_pll,theta_pll;//采自锁相环
 
extern float Ua_ref,Ub_ref,Uc_ref;//SVPWM输入
extern float f_ref;

extern float Ud_Servo,Uq_Servo,Id_Servo,Iq_Servo,theta_ref;//对外接口

extern float global_timer;
extern float Gain_u;

typedef struct{
    float KP;											//PID参数P
    float KI;											//PID参数I
    float KD;											//PID参数D
    float fdb;											//PID反馈值
    float ref;											//PID目标值
    float cur_error;									//当前误差
    float error[2];										//前两次误差
    float output;										//输出值
    float outputMax;									//最大输出值的绝对值
	float outputMin;                                    //最小输出值的绝对值用于防抖
}PID_t;

typedef struct
{
    CT_data_t Uref;
    PID_t id_pid;
    PID_t iq_pid;
}VC_t;

extern VC_t hVC;

void VoltageServo(float* Ua_out,float* Ub_out,float* Uc_out,float ud_ref,float uq_ref,float theta_pll);
void CurrentServo(VC_t* hVC_in,float ud_ref,float uq_ref,float theta_pll);

void PID_Calc(PID_t *pid);

#ifdef __cplusplus
}
#endif
