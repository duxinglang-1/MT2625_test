#include "hrm_alg_v01.h"
#include "DataProcess.h"

//#define max_hrm_internal()  m_hrm_parg.fs/3
//#define min_hrm_internal()  3*m_hrm_parg.fs

typedef struct
{
    AVG_VL_PARAM_T				avg_flt;
    FST_ODR_HP_FIL_PARAM_T		hp_flt;
    SCND_FILTER_COEF_T			lp_flt_param[2];
    float						fs;
		KLM_FILTER_PARAM_T 			hrm_klm_flt_parg;
    float 						hrm_klm;
		MEAN_SQUARE_PARAM_T			ms_filter;
		float						ms_filter_buf[5];

    float						pre_data;
    float						fst_dv[2];
		int 						data_cnt;
		int 						data_offset;
		int							is_inited;

} hrm_parg_t;

static hrm_parg_t m_hrm_parg;

int hrm_alg_init(float fs)
{
	memset(&m_hrm_parg, 0, sizeof(hrm_parg_t));
    m_hrm_parg.fs = fs;
    ButterScndOrderLP_HP_CoefCal(&m_hrm_parg.lp_flt_param[0], FILTER_LOW_PASS_TYPE, m_hrm_parg.fs, 2.1f);
    ButterScndOrderLP_HP_CoefCal(&m_hrm_parg.lp_flt_param[1], FILTER_LOW_PASS_TYPE, m_hrm_parg.fs, 2.1f);
    average_value_filter_init(&m_hrm_parg.avg_flt, (int)m_hrm_parg.fs);
    fst_order_highpass_filter_param_init(&m_hrm_parg.hp_flt, 0.96f);
	MeanSquareParamInit(&m_hrm_parg.ms_filter, m_hrm_parg.ms_filter_buf, sizeof(m_hrm_parg.ms_filter_buf)/sizeof(float)); //求均方差初始�?
	m_hrm_parg.is_inited = 1;
    return 0;	
}
    
int hrm_alg_samp_process(int data, float* hrm)
{
	if (1 != m_hrm_parg.is_inited)
	{
		return -1;
	}
	int ret = -1;
	*hrm = 0;
	m_hrm_parg.data_offset++;
	m_hrm_parg.data_cnt++;
	float hp_data = fst_order_highpass_filter(&m_hrm_parg.hp_flt, data);
    float lpdata1 = ButterScndOrderFilterProcess(&m_hrm_parg.lp_flt_param[0], hp_data);
    float lpdata2 = ButterScndOrderFilterProcess(&m_hrm_parg.lp_flt_param[1], lpdata1);
	
	float smp_data = lpdata2;
	
	m_hrm_parg.fst_dv[1] = m_hrm_parg.fst_dv[0];
	m_hrm_parg.fst_dv[0] = smp_data - m_hrm_parg.pre_data;
    if (m_hrm_parg.fst_dv[0] >= 0 && m_hrm_parg.fst_dv[1] < 0 && m_hrm_parg.data_cnt != 0)
    {
		float tem_hrm = 60 * m_hrm_parg.fs / m_hrm_parg.data_cnt;
		if(tem_hrm > 45 && tem_hrm < 170)
		{
			float ms_val = MeanSquareParamPutValue(&m_hrm_parg.ms_filter, tem_hrm);
			if (m_hrm_parg.hrm_klm == 0.f)
			{
				if ((m_hrm_parg.ms_filter.DataCount == m_hrm_parg.ms_filter.CacheSize && ms_val < 3.0f) || (m_hrm_parg.data_offset > 14 * m_hrm_parg.fs))
				{
					m_hrm_parg.hrm_klm = m_hrm_parg.hrm_klm_flt_parg.Pk_1 = m_hrm_parg.hrm_klm_flt_parg.Xk_1 = m_hrm_parg.ms_filter.DataMeanValue;
					m_hrm_parg.hrm_klm_flt_parg.Q = m_hrm_parg.hrm_klm * m_hrm_parg.hrm_klm * 0.0003f;
				}
			}
			else
			{
				float mean_hrm = m_hrm_parg.ms_filter.DataMeanValue;
				float deta_r = mean_hrm - m_hrm_parg.hrm_klm;
				m_hrm_parg.hrm_klm_flt_parg.R = deta_r*deta_r;
				m_hrm_parg.hrm_klm = KalmanFilter(mean_hrm, &m_hrm_parg.hrm_klm_flt_parg.Xk_1, &m_hrm_parg.hrm_klm_flt_parg.Pk_1, m_hrm_parg.hrm_klm_flt_parg.Q, m_hrm_parg.hrm_klm_flt_parg.R);
				m_hrm_parg.hrm_klm_flt_parg.Q = m_hrm_parg.hrm_klm * m_hrm_parg.hrm_klm * 0.00015f;// 
				*hrm = m_hrm_parg.hrm_klm;
				ret = 0;
			}		
		}
		m_hrm_parg.data_cnt = 0;   
    }
	m_hrm_parg.pre_data = smp_data;	
	return ret;
}
