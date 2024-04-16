#ifndef YC_SPO2_ALG_H_
#define YC_SPO2_ALG_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
	type_unkown = 0,
	type_Hr,
	type_Spo2,	
}value_type_t;

typedef struct
{
	value_type_t type;
	float val;	
}alg_parg_t;

int yc_spo2_alg_init(float fs);
int yc_oxg_get_spo2(int red, int ir, int green, alg_parg_t *parg);

#ifdef __cplusplus
}
#endif

#endif // YC_SPO2_ALG_H_
