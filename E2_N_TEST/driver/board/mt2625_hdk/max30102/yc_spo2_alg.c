
#include "DataProcess.h"
#include "yc_spo2_alg.h"
#include "hrm_alg_v01.h"
//#include "YCLIB_common.h"
#define LOG(...)    //  YCLIB_PRINTF(__VA_ARGS__)

#define RADIO_BUFFER_SIZE 8
typedef struct
{
    AVG_VL_PARAM_T				avg_flt;
    FST_ODR_HP_FIL_PARAM_T		hp_flt;
    SCND_FILTER_COEF_T			lp_flt_param[2];
    float						fs;
    int							vpp_ok_cnt;//

    float						dc_val;
    float						ac_val;
    int							vpp_ok;
    float						max_val;
    float						min_val;
    float						pre_data;
    float						fst_dv[2];

} single_light_parg;

typedef struct
{
    single_light_parg light[3];
    KLM_FILTER_PARAM_T klm_flt_parg;
    float radio_klm;
    float radio[RADIO_BUFFER_SIZE]; 
    MEAN_SQUARE_PARAM_T ms_filter;
    short	  byond_thresh_cnt;
    short   spo2_out;
    int datacnt;
    float last_spo2;
    short  off_rd;
    short  klm_inited;
	short  init_cnt;
	short  is_init;
	
} yc_spo2_parg;

static yc_spo2_parg spo2_parg;

static int  singal_light_init(single_light_parg* parg, float fs)
{
    memset(parg, 0, sizeof(single_light_parg));
    parg->fs = fs;
    ButterScndOrderLP_HP_CoefCal(&parg->lp_flt_param[0], FILTER_LOW_PASS_TYPE, parg->fs, 2.2f);
    ButterScndOrderLP_HP_CoefCal(&parg->lp_flt_param[1], FILTER_LOW_PASS_TYPE, parg->fs, 2.2f);
    average_value_filter_init(&parg->avg_flt, (int)parg->fs);
    fst_order_highpass_filter_param_init(&parg->hp_flt, 0.96f);
    return 0;
}

static void singal_light_updata_data(single_light_parg* parg, int data)
{
    parg->dc_val = average_value_filter(&parg->avg_flt, data);
    float hp_data = fst_order_highpass_filter(&parg->hp_flt, data);
    float lpdata1 = ButterScndOrderFilterProcess(&parg->lp_flt_param[0], hp_data);
    float lpdata2 = ButterScndOrderFilterProcess(&parg->lp_flt_param[1], lpdata1);

    if (lpdata2 > parg->max_val)
    {
        parg->max_val = lpdata2;
    }
    else if (lpdata2 < parg->min_val)
    {
        parg->min_val = lpdata2;
    }
    parg->fst_dv[1] = parg->fst_dv[0];
    parg->fst_dv[0] = lpdata2 - parg->pre_data; // Ò»½×²î·Ö
    if (parg - spo2_parg.light == 1)
    {
        //printf("%d %.2f %.2f %.2f %.2f\n", data, parg->dc_val, hp_data, lpdata1, lpdata2);
    }
    if (parg->fst_dv[0] >= 0 && parg->fst_dv[1] < 0)
    {
        parg->ac_val = parg->max_val - parg->min_val;
        if (parg - spo2_parg.light == 1)
        {
            //	printf("acval %.2f\n", parg->ac_val);
        }
        if (parg->vpp_ok_cnt != 0)
        {
            parg->vpp_ok = 1;
        }
        parg->vpp_ok_cnt++;
        parg->min_val = 2097152;//2^21
        parg->max_val = -2097152;//2^21
    }
    parg->pre_data = lpdata2;

}

int yc_spo2_alg_init(float fs)
{
	short init_cnt = spo2_parg.init_cnt;
    memset(&spo2_parg, 0, sizeof(yc_spo2_parg));
	spo2_parg.init_cnt = init_cnt++;
    for (int  i = 0; i < 3 ; i++)
    {
        singal_light_init(&spo2_parg.light[i], fs);
    }
    MeanSquareParamInit(&spo2_parg.ms_filter, spo2_parg.radio, RADIO_BUFFER_SIZE);
	if(spo2_parg.init_cnt < 500)
	{
		spo2_parg.is_init = 1;	
	}
	else
	{
		spo2_parg.is_init = 0;	
	}
	hrm_alg_init(fs);
    return 0;
}
int yc_oxg_get_spo2(int red, int ir, int green, alg_parg_t *parg )
{
	int ret = -1;
	if(0 == spo2_parg.is_init )
	{
		return -1;
	}
    spo2_parg.datacnt++;
    int signal[3] = {red,ir,green};
    for (int i= 0; i< 2; i++)
    {
        singal_light_updata_data(&spo2_parg.light[i], signal[i]);
    }
	float hrm = 0;
	int hr_alg_ret = hrm_alg_samp_process(ir, &hrm);
	if (0 == hr_alg_ret)
	{
		ret = 0;
		(*parg).type = type_Hr;
		(*parg).val = hrm;
	}
    float spo2_radio = 0;
    float spo2 = 0;
    float ms_val;
    if (spo2_parg.light[0].vpp_ok && spo2_parg.light[1].vpp_ok)
    {
        spo2_radio = (spo2_parg.light[0].ac_val / spo2_parg.light[0].dc_val) / (spo2_parg.light[1].ac_val / spo2_parg.light[1].dc_val);
        if (spo2_radio >0.22f && spo2_radio < 1.32f)
        {
            ms_val =  MeanSquareParamPutValue(&spo2_parg.ms_filter, spo2_radio);
            spo2_parg.off_rd++;
            
            if (spo2_parg.klm_inited == 0)
            {
                if(spo2_parg.off_rd >= RADIO_BUFFER_SIZE && ms_val < 0.07f)
                {
                    spo2_parg.radio_klm = spo2_parg.klm_flt_parg.Pk_1 = spo2_parg.klm_flt_parg.Xk_1 = spo2_parg.ms_filter.DataMeanValue; 
                    spo2_parg.klm_flt_parg.Q = spo2_parg.radio_klm * spo2_parg.radio_klm * 0.0002f;
                    spo2_parg.klm_inited = 1;
                }
                else if(spo2_parg.off_rd > 22 || spo2_parg.datacnt/ spo2_parg.light[0].fs > 18)
                {
                	spo2_parg.radio_klm = spo2_parg.klm_flt_parg.Pk_1 = spo2_parg.klm_flt_parg.Xk_1 = spo2_parg.ms_filter.DataMeanValue; 
                  spo2_parg.klm_flt_parg.Q = spo2_parg.radio_klm * spo2_parg.radio_klm * 0.0002f;
                  spo2_parg.klm_inited = 1;
                	
                        
                }
            }
            else 
            {
                if(ms_val > 0.13f)
                {
                    spo2 = spo2_parg.last_spo2;
                }
                else
                {
                    float r2 = spo2_radio - spo2_parg.radio_klm;
                    if(spo2_radio < spo2_parg.radio_klm && spo2_radio < 0.95f)
                    {
                        spo2_parg.byond_thresh_cnt++;
                    }
                    else
                    {
                        spo2_parg.byond_thresh_cnt = 0;
                    }
                      if(spo2_parg.spo2_out == 0 )
                    {
                        if(spo2_radio > 0.52f)
                        {
                            spo2_radio = 0.52f;
                        }
                    }
                    spo2_parg.spo2_out++;

                    spo2_parg.klm_flt_parg.R = r2*r2;
                    spo2_parg.radio_klm = KalmanFilter(spo2_radio, &spo2_parg.klm_flt_parg.Xk_1, &spo2_parg.klm_flt_parg.Pk_1, spo2_parg.klm_flt_parg.Q, spo2_parg.klm_flt_parg.R);
                    //LOG("spo2radio: %.2f %.2f\n",spo2_parg.radio_klm,spo2_radio);
                    if (spo2_parg.byond_thresh_cnt >= 6 )
                    {
                        //LOG("flow spo2\n");
                        spo2_parg.klm_flt_parg.Q = spo2_parg.radio_klm * spo2_parg.radio_klm * 0.0006f;// powf(spo2_parg.radio_klm * 0.03f, 2);
                    }
                    else
                    {
                        spo2_parg.klm_flt_parg.Q = spo2_parg.radio_klm * spo2_parg.radio_klm * 0.0002f;// powf(spo2_parg.radio_klm * 0.01f, 2);
                    }
                    float klm_rd = spo2_parg.radio_klm;
                    //spo2 = 0.757f* klm_rd* klm_rd* klm_rd - 7.4977f* klm_rd* klm_rd +29.02f* klm_rd+50.356f;
                    spo2 = 7.0967f* klm_rd* klm_rd* klm_rd - 17.401f* klm_rd* klm_rd -7.989f* klm_rd+105.3f;
                    
                   // spo2 = 1.2818f*klm_rd* klm_rd - 22.654f* klm_rd + 109; //add by shl 2018-11-12
                    if(spo2 > 99.9f)
                    {
                        spo2 = 99.9f;
                    }
                    else if(spo2 < 82.5f)
                    {
                        spo2 = 82.5f;
                    }
					ret = 0;
					(*parg).type = type_Spo2;
					(*parg).val = spo2;
                    LOG("spo2radio: %.2f %.2f %.1f\n",spo2_parg.radio_klm,spo2_radio,spo2 );
                }
            }
        }
        spo2_parg.light[0].vpp_ok = 0;
        spo2_parg.light[1].vpp_ok = 0;
    }
    spo2_parg.last_spo2 = spo2;
    return ret;
}


