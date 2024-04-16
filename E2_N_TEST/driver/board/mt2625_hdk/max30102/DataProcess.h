#ifndef __DATA_PROCESS_H__
#define __DATA_PROCESS_H__       
/**********************************************************************************
***@author: shl          **********************************************************
***@date:2017-04-17      **********************************************************
***@func: ������Ӧ�����ṹ,********************************************************
***@Warning: FEATURE_POINT�еĵ�ַƫ������Ի������ڵĵ�ĺ�����Ϊ����ƫ��*******
**********************************************************************************/
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#ifdef __cplusplus
extern "C" {
#endif  

#define FILTER_HIGH_PASS_TYPE     0X01
#define FILTER_LOW_PASS_TYPE	  0X10

typedef struct feature_point
{
	int				offSet;			 //ƫ��
	float			value;
}FEATURE_POINT;						//������
typedef struct mean_square_param_t //������
{
	float*	DataCache;
	float	DataSum; 
	float	DataMeanValue;
	float	DataMSV; //  DataMeanSquareValue
	int		DataCount;
	int		DataIndex;
	int		CacheSize;
}MEAN_SQUARE_PARAM_T;
typedef struct mean_val_filter_param_t //��ֵ�˲�
{
	double		DataSum;	//��ֵ�ܺ�
	int			DataCnt;	//���ݸ�������
	float*		DataBuf;
	int			DataIndex;
	int			CacheSize;
}MEAN_VAL_FILTER_PARAM_T;

typedef struct klm_filter_param_t
{
	float Xk_1;
	float Pk_1;

	float Q;
	float Q0;
	float R;
}KLM_FILTER_PARAM_T;
typedef struct filter_data_param_t
{
	float Data_X[4];
	float Data_Y[4];
	const float* coefs;
	short  FilterType;	//�˲�������
	short  DataInitCnt; //��Ҫ��ʼ�������ݸ���
	short  FilterOrder; //�˲�������
}FILTER_DATA_PARAM_T;

typedef struct bpfilter_data_param_t //���״�ͨ���ߴ����˲�������
{
	float Data_X[4];
	float Data_Y[4];
	const float* coefs;
}BPFILTER_DATA_PARAM_T;

typedef struct rc_filter_param_t
{
	float	a_curr_coef;
	float	a_default_coef;
	int		unequal_cnt;
	float	Yn_1;
	float	RS_Threshold;
}RC_FILTER_PARAM_T;
typedef struct fir_filter_param_t
{
	const float *coefs;
	float		*Xn;
	int			Fir_Order;
}FIR_FILTER_PARAM_T;

typedef struct _average_value_filter_param_t
{
	int 	avg_len; // ����ٸ����ݵ�ƽ��ֵ
	int 	data_count; //�Ѿ������˶��ٸ���
	float   agv_v;		//ƽ��ֵ
}AVG_VL_PARAM_T;

typedef struct _first_order_hp_filter_param_t
{
	int is_use;
	float pre_xn;
	float pre_yn;
	float radio; // ��ͨϵ����ȡֵС��1��Խ����1���˲�Ч��Խ�
}FST_ODR_HP_FIL_PARAM_T;

typedef struct scnd_filter_coef_t
{
	int Type;	//	Lp or Hp or Bp
	int order;	//�����ʼ��Ϊ2
	float x[2];
	float y[2];
	float coef[5];//��Ϊ�Ƕ���ϵ������ˣ�A0 = 1����ˣ�ֻ�����ϵ��
}SCND_FILTER_COEF_T;

int  average_value_filter_init(AVG_VL_PARAM_T* parg, int len);
float average_value_filter(AVG_VL_PARAM_T* parg, float v);
int	  ButterScndOrderLP_HP_CoefCal(SCND_FILTER_COEF_T* parg, int  itype, float fs, float f_cut); //ע�⣬������˹�����˲������ڽ�ֹƵ�ʴ����½�3db																							
float ButterScndOrderFilterProcess(SCND_FILTER_COEF_T* parg, float data);

float BandPassOrEliminateFilter(BPFILTER_DATA_PARAM_T* parg, float samp); //���״�ͨ���ߴ����˲���
float KalmanFilter(float data, float *pXk_1, float *pPk_1, float Q, float R);
float HighPassFilter(const float *pb, float *x, float *y, float samp);//һ�׸�ͨ�˲�
float ButterFilter(const float *pb, float *u, float *y, float samp);//������˹�����˲���
float ChebyFilter(const float *pb, float *x, float *y, float samp); //�б�ѩ�������˲���
float LowPassFilter(float data, float ratio, float* Yn_1);//��ͨ�˲�
float HighOrLowPassFilter(FILTER_DATA_PARAM_T* parg, float samp);//��Զ��׻������Ľ׵ĸߵ�ͨ��ʼ�����˲�
/*����ʽ��ֵ���㣬���У�xΪ��֪�ĺ����꣬fxΪ��֪�����꣬nΪ��֪�������������ĵ�����
zΪ��Ҫ��ֵ������pzΪ��������Ĳ�ֵ��ֵ��mΪ����ֵ�ĸ�����
��ֵ�ɹ�����0��ʧ�ܷ���-1��*/
int Interpol(const double* x, const double *fx, int n, double *z, double *pz, int m);
/*��fx = ax3+bx2+cx+d�����ݵ������ϣ�Ȼ�������ֵ�����ֵ��Ӧ��λ��*****
**warning: ���У�fx[2]Ϊ��֪���ֵ,x[2]Ϊ��֪���ֵ��Ӧ�ĵ�����x�����ڲ�����ĸ������ĵ��Ӧ��ƫ�ƣ��ɹ�����0��ʧ�ܷ���-1��*/
int FitCurveAndGetMax(const float x[4], const float fx[4], float *z, float *pz);
int FitCurveWithCubicCurve(const float x[4], const float fx[4], double coef[4]);//��fx = ax3+bx2+cx+d�����߽������,��a,b,c,dϵ����shl 2017-04-12
int FitCurveWithQuadraticCurve(const float x[3], const float fx[3], double coef[3]);//��fx = ax2+bx+c�����߽������,��a,b,cϵ����shl 2017-04-12
/*��fx = ax2+bx+c�����ݵ������ϣ�Ȼ������Сֵ�Լ���Сֵ��Ӧ��λ�ã�
���У�fx[1]Ϊ��֪��С�㣬x[1]Ϊ��֪��Сֵ��Ӧ�ĵ������ɹ�����0��ʧ�ܷ���-1��*/
int FitCurveAndGetMin(const float x[3], const float fx[3], float *z, float *pz);
/*����ֱ���ཻ�ķ�����ȡ���㣬fx[2]Ϊ��ֵ��*********/
int GetMaxPntWithCrsPntOfStrLine(const float x[5], const float fx[5], float *z, float *pz);
int GetIntersecOfStr_LineWithX_Axis(const float x[2], const float fx[2], float *z, float *pz);//��ȡֱ����X��Ľ���
/*��С���˹��ƣ�y= b1*x+b0,����b1 ��b0 ��ֵȥ���,������y=b1*x+b0���ɲ���x��y��ָ���ĵ㼯����������*/
void Lsqe(const float *x, const float *y, int n, float *b1, float* b0);
/*��С���˹��ƣ�y= b1/x+b0,����b1 ��b0 ��ֵȥ���,������y=b1/x+b0���ɲ���x��y��ָ���ĵ㼯����������*/
void LsqeAntyRateCurve(const float *x, const float *y, int n, float *b1, float* b0);

int	  MeanFilterInit(MEAN_VAL_FILTER_PARAM_T* parg, float* data_cache, int cache_size);//��ֵ�˲�
float MeanFilter(MEAN_VAL_FILTER_PARAM_T* parg, float samp); //
int   MedianFilterInit(MEAN_VAL_FILTER_PARAM_T* parg, float* data_cache, int cache_size); //��ֵ�˲�
float MedianFilterPutValue(MEAN_VAL_FILTER_PARAM_T* parg, float samp);//��ֵ�˲������ݸ���С��lenʱ������samp
int	  MeanSquareParamInit(MEAN_SQUARE_PARAM_T *parg, float* data_cache,int cache_size); //��������ʼ��
float MeanSquareParamPutValue(MEAN_SQUARE_PARAM_T *parg, float ps); //�������

int	  RCLowPassFilterInit(RC_FILTER_PARAM_T* parg, float fs, float cutoff_fs, float rs);
float RCLowPassFilter(RC_FILTER_PARAM_T* parg, float data);

int   FirFilterInit(FIR_FILTER_PARAM_T* parg, const float* coef, float* Xn, int Order);
float FirFilterProcess(FIR_FILTER_PARAM_T* parg, float m_data);

int	  fst_order_highpass_filter_param_init(FST_ODR_HP_FIL_PARAM_T* parg, float radio);
float fst_order_highpass_filter(FST_ODR_HP_FIL_PARAM_T* parg, float samp);

void  BubbleSort(float a[], int n);//ð������
void  ClearValueOfFtpt(FEATURE_POINT* pt1); //��������������

void  GetFitedMinValueInWave(int dots, float buf[], float *z, float *pz);//��ȡ���ζ���
void  GetFitedMaxValueInWave(int dots, float buf[], float *z, float *pz);//��ȡ���ε׵�
void  GetZeroPointOfStrLineWithX_Axis(FEATURE_POINT* fp, float* z, float *pz);//��ȡֱ����X���ཻ�ĵ�
void  GetMaxPntOfTwoStrLineCross(int dots, float buf[], float* z, float *pz);//��ȡ���ֱཻ�ߵĽ���
 
#ifdef __cplusplus
}
#endif

#endif


