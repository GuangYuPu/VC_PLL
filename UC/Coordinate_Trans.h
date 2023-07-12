#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define PI 3.141592653589793

typedef struct
{
    float a;
    float b;
    float c;
}abc_t;

typedef struct
{
    float alpha;
    float beta;
}AlphaBeta_t;

typedef struct
{
    float d;
    float q;
}dq_t;

typedef struct
{
    abc_t abc_value;
    AlphaBeta_t AlphaBeta_value;
    dq_t dq_value;
}CT_data_t;

void CT_Refresh_abc(CT_data_t *CTdata,float a,float b,float c,float theta);
void CT_Refresh_AlphaBeta(CT_data_t *CTdata,float alpha,float beta,float theta);
void CT_Refresh_dq(CT_data_t *CTdata,float d,float q,float theta);

void CT_get_AlphaBeta(CT_data_t *CTdata,float a,float b,float c);
void CT_get_abc(CT_data_t *CTdata,float alpha,float beta);

#ifdef __cplusplus
}
#endif
