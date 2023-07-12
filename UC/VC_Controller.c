#include "VC_Controller.h"


//extern in .h
float Ud,ia,ib,ic,ua,ub,uc;//直接采样值
CT_data_t Sample_i,Sample_u;//储存并处理采样数据
float amplitude_pll = 0;
float theta_pll = 0;//采自锁相环

float Ua_ref,Ub_ref,Uc_ref;//SVPWM输入
float f_ref;

float theta_ref;
float Ud_Servo = 0;
float Uq_Servo = 0;
float Id_Servo = 0;
float Iq_Servo = 0;//对外输入接口

VC_t hVC;//相当于hDJI

float global_timer = 0;
float Gain_u;

void User_VC_init()
{
    hVC.id_pid.KP = 10;
    hVC.id_pid.KI = 5;
    hVC.id_pid.KD = 0;
    hVC.id_pid.outputMax = 80;
    hVC.id_pid.outputMin = 0;

    hVC.iq_pid.KP = 10;
    hVC.iq_pid.KI = 5;
    hVC.iq_pid.KD = 0;
    hVC.iq_pid.outputMax = 80;
    hVC.iq_pid.outputMin = 0;

    Gain_u = 10;
}

//增量式PID算法
void PID_Calc(PID_t *pid){
	pid->cur_error = pid->ref - pid->fdb;
	// pid->output += pid->KP * (pid->cur_error - pid->error[1]) + pid->KI * pid->cur_error + pid->KD * (pid->cur_error - 2 * pid->error[1] + pid->error[0]);
	pid->output = pid->KP * pid->cur_error;
    pid->error[0] = pid->error[1];
	pid->error[1] = pid->ref - pid->fdb;
	/*设定输出上下限*/
	if(pid->output > pid->outputMax) pid->output = pid->outputMax;
	if(pid->output < -pid->outputMax) pid->output = -pid->outputMax;
    if(fabs(pid->output)<pid->outputMin) pid->output=0;
}

void VoltageServo(float* Ua_out,float* Ub_out,float* Uc_out,float ud_ref,float uq_ref,float theta_pll)
{
    CT_data_t hVC_in;
    CT_Refresh_dq(&hVC_in,ud_ref,uq_ref,theta_pll);
    *Ua_out = hVC_in.abc_value.a;
    *Ub_out = hVC_in.abc_value.b;
    *Uc_out = hVC_in.abc_value.c;
}

void CurrentServo(VC_t* hVC_in,float id_ref,float iq_ref,float theta_pll)
{
    hVC_in->id_pid.ref = id_ref;
    hVC_in->id_pid.fdb = Sample_i.dq_value.d;
    PID_Calc(&hVC_in->id_pid);

    hVC_in->iq_pid.ref = iq_ref;
    hVC_in->iq_pid.fdb = Sample_i.dq_value.q;
    PID_Calc(&hVC_in->iq_pid);

    VoltageServo(&Ua_ref,&Ub_ref,&Uc_ref,hVC_in->id_pid.output,hVC_in->iq_pid.output,theta_pll);
}