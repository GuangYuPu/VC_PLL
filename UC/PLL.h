#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "VC_Controller.h"

extern float Kp_pll,Ki_PLL,f_ref_PLL;
extern float id_out_sum;
extern uint8_t num_reg; //(uint8_t)(1/(4*f_ref_PLL*0.00025));
extern float Sample_reg[20];

void PLL_init(float kp,float ki,float fref);
void PLL_SinglePhase(float *amplitude_pll,float *theta_pll,float Sample_in,float theta_pll_in);
void PLL_ThreePhase(float *amplitude_pll,float *theta_pll,CT_data_t Sample_in,float theta_pll_in);

#ifdef __cplusplus
}
#endif
