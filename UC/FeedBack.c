#include "FeedBack.h"

void Sample_Refesh(CT_data_t *Sample_i_in,CT_data_t *Sample_u_in,float ia_in,float ib_in,float ic_in)
{
    CT_Refresh_abc(Sample_i_in,ia_in,ib_in,ic_in,theta_pll);
    CT_Refresh_abc(Sample_u_in,ia_in,ib_in,ic_in,theta_pll);
}