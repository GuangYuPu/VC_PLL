#include "SVPWM.h"
#include "math.h"
#include "main.h"

#include "Coordinate_Trans.h"

float TIM1_ARR=200-1;

Svpwm_t Duty_abc;//SVPWM输出

//在定时器向下溢出中断中更新三相桥的占空比
void Svpwm_Calculate(Svpwm_t *Svpwm,float Ud,float Ua_ref,float Ub_ref,float Uc_ref)
{
    //计算均以矢量进行
    //坐标变换
    CT_data_t svpwm_ref;
    svpwm_ref.abc_value.a = sqrt(2/3)*Ua_ref;
    svpwm_ref.abc_value.b = sqrt(2/3)*Ub_ref;
    svpwm_ref.abc_value.c = sqrt(2/3)*Uc_ref;
    CT_get_AlphaBeta(&svpwm_ref,svpwm_ref.abc_value.a,svpwm_ref.abc_value.b,svpwm_ref.abc_value.c);

    //判断矢量扇区
    uint8_t region;
    float theta = atan2f(svpwm_ref.AlphaBeta_value.beta,svpwm_ref.AlphaBeta_value.alpha);
    if((theta > -PI) && (theta <= -2*PI/3))
    {
        region = 4;
    }
    else if((theta > -2*PI/3) && (theta <= -1*PI/3))
    {
        region = 5;
    }
    else if((theta > -1*PI/3) && (theta <= 0*PI/3))
    {
        region = 6;
    }
    else if((theta > 0*PI/3) && (theta <= 1*PI/3))
    {
        region = 1;
    }
    else if((theta > 1*PI/3) && (theta <= 2*PI/3))
    {
        region = 2;
    }
    else //((theta >= 2*PI/3) && (theta < PI))
    {
        region = 3;
    }

    //计算每个扇区相邻矢量作用时间，计算值单位是毫秒,给出三相桥占空比
    float Uref_len = sqrt((svpwm_ref.AlphaBeta_value.alpha)*(svpwm_ref.AlphaBeta_value.alpha) + (svpwm_ref.AlphaBeta_value.beta)*(svpwm_ref.AlphaBeta_value.beta));
    float t1 = 0; 
    float t2 = 0;
    float theta_f = theta;
    if(region == 1)
    {
        //计算矢量作用时间
        t1 = sqrt(2)*(sin(theta_f)*Uref_len)/Ud;//(1,1,0)
        t2 = sqrt(2)*(sin(PI/3 - theta_f)*Uref_len)/Ud;//(1,0,0)
        if((t1+t2)>1)
        {
            t1 = t1/(t1+t2);
            t2 = t2/(t1+t2);
        }
        //计算每相占空比
        Svpwm->ccRA = TIM1_ARR*(t1+t2);
        Svpwm->ccRB = TIM1_ARR*(t1);
        Svpwm->ccRC = TIM1_ARR*(0);
    }
    else if(region == 2)
    {
        //计算矢量作用时间
        theta_f = theta - PI/3;
        t1 = sqrt(2)*(sin(theta_f)*Uref_len)/Ud;//(0,1,0)
        t2 = sqrt(2)*(sin(PI/3 - theta_f)*Uref_len)/Ud;//(1,1,0)
        if((t1+t2)>1)
        {
            t1 = t1/(t1+t2);
            t2 = t2/(t1+t2);
        }  
        //计算每相占空比
        Svpwm->ccRA = TIM1_ARR*(t2);
        Svpwm->ccRB = TIM1_ARR*(t1+t2);
        Svpwm->ccRC = TIM1_ARR*(0);
    }
    else if(region == 3)
    {
        //计算矢量作用时间
        theta_f = theta - 2*PI/3;
        t1 = sqrt(2)*(sin(theta_f)*Uref_len)/Ud;//(0,1,1)
        t2 = sqrt(2)*(sin(PI/3 - theta_f)*Uref_len)/Ud;//(0,1,0)
        if((t1+t2)>1)
        { 
            t1 = t1/(t1+t2);
            t2 = t2/(t1+t2);
        }  
        //计算每相占空比
        Svpwm->ccRA = TIM1_ARR*(0);
        Svpwm->ccRB = TIM1_ARR*(t1+t2);
        Svpwm->ccRC = TIM1_ARR*(t1);
    }
    else if(region == 6)
    {
        //计算矢量作用时间
        theta_f = theta + 1*PI/3;
        t1 = sqrt(2)*(sin(theta_f)*Uref_len)/Ud;//(1,0,0)
        t2 = sqrt(2)*(sin(PI/3 - theta_f)*Uref_len)/Ud;//(1,0,1)
        if((t1+t2)>1)
        {
            t1 = t1/(t1+t2);
            t2 = t2/(t1+t2);
        }  
        //计算每相占空比
        Svpwm->ccRA = TIM1_ARR*(t1+t2);
        Svpwm->ccRB = TIM1_ARR*(0);
        Svpwm->ccRC = TIM1_ARR*(t2);
    }
    else if(region == 5)
    {
        //计算矢量作用时间
        theta_f = theta + 2*PI/3;
        t1 = sqrt(2)*(sin(theta_f)*Uref_len)/Ud;//(1,0,1)
        t2 = sqrt(2)*(sin(PI/3 - theta_f)*Uref_len)/Ud;//(0,0,1)
        if((t1+t2)>1)
        {
            t1 = t1/(t1+t2);
            t2 = t2/(t1+t2);
        }  
        //计算每相占空比
        Svpwm->ccRA = TIM1_ARR*(t1);
        Svpwm->ccRB = TIM1_ARR*(0);
        Svpwm->ccRC = TIM1_ARR*(t1+t2);
    }
    else //if(region == 4)
    {
        //计算矢量作用时间
        theta_f = theta + PI;
        t1 = sqrt(2)*(sin(theta_f)*Uref_len)/Ud;//(0,0,1)
        t2 = sqrt(2)*(sin(PI/3 - theta_f)*Uref_len)/Ud;//(0,1,1)
        if((t1+t2)>1)
        {
            t1 = t1/(t1+t2);
            t2 = t2/(t1+t2);
        }  
        //计算每相占空比
        Svpwm->ccRA = TIM1_ARR*(0);
        Svpwm->ccRB = TIM1_ARR*(t2);
        Svpwm->ccRC = TIM1_ARR*(t1+t2);
    }
}

void Spwm_Calculate(Svpwm_t *Svpwm,float GAIN,float Ua_ref,float Ub_ref,float Uc_ref)
{
    Svpwm->ccRA = TIM1_ARR/2 + ((Ua_ref)*(GAIN));
    Svpwm->ccRB = TIM1_ARR/2 + ((Ub_ref)*(GAIN));
    Svpwm->ccRC = TIM1_ARR/2 + ((Uc_ref)*(GAIN));
}