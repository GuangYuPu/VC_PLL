#include "Coordinate_Trans.h"
#include "math.h"
#include "main.h"

void CT_Refresh_abc(CT_data_t *CTdata,float a,float b,float c,float theta)
{
    CTdata->AlphaBeta_value.alpha 
    = sqrt(2/3) * ((1)*a - (1/2)*b - (1/2)*c);
     CTdata->AlphaBeta_value.beta 
    = sqrt(2/3) * ((0)*a + (sqrt(3)/2)*b - (sqrt(3)/2)*c);

    CTdata->dq_value.d 
    = (2/3) * ((cos(theta))*a + (cos(theta-2*PI/3))*b + (cos(theta+2*PI/3))*c);
     CTdata->dq_value.q 
    = -(2/3) * ((sin(theta))*a + (sin(theta-2*PI/3))*b - (sin(theta+2*PI/3))*c);
}

void CT_Refresh_AlphaBeta(CT_data_t *CTdata,float alpha,float beta,float theta)
{
    CTdata->abc_value.a 
    = sqrt(2/3) * ((1)*alpha - (0)*beta);
    CTdata->abc_value.b
    = sqrt(2/3) * (-(1/2)*alpha + (sqrt(3)/2)*beta);
    CTdata->abc_value.c
    = sqrt(2/3) * (-(1/2)*alpha - (sqrt(3)/2)*beta);

    CTdata->dq_value.d 
    = (cos(theta))*alpha + (sin(theta))*beta;
     CTdata->dq_value.q 
    = -(sin(theta))*alpha + (cos(theta))*beta;
}

void CT_Refresh_dq(CT_data_t *CTdata,float d,float q,float theta)
{
    CTdata->abc_value.a 
    = cos(theta)*d - sin(theta)*q;
    CTdata->abc_value.b
    = cos(theta-2*PI/3)*d - sin(theta-2*PI/3)*q;
    CTdata->abc_value.c
   = cos(theta+2*PI/3)*d - sin(theta+2*PI/3)*q;

    CTdata->AlphaBeta_value.alpha 
    = (cos(theta))*d - (sin(theta))*q;
     CTdata->AlphaBeta_value.beta 
    = (sin(theta))*d + (cos(theta))*q;
}

void CT_get_AlphaBeta(CT_data_t *CTdata,float a,float b,float c)
{
    CTdata->AlphaBeta_value.alpha 
    = sqrt(2/3) * ((1)*a - (1/2)*b - (1/2)*c);
     CTdata->AlphaBeta_value.beta 
    = sqrt(2/3) * ((0)*a + (sqrt(3)/2)*b - (sqrt(3)/2)*c);
}

void CT_get_abc(CT_data_t *CTdata,float alpha,float beta)
{
    CTdata->abc_value.a 
    = sqrt(2/3) * ((1)*alpha - (0)*beta);
    CTdata->abc_value.b
    = sqrt(2/3) * (-(1/2)*alpha + (sqrt(3)/2)*beta);
    CTdata->abc_value.c
    = sqrt(2/3) * (-(1/2)*alpha - (sqrt(3)/2)*beta);
}