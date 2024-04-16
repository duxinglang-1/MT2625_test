
#ifndef LIB_BSP_ALG_H
#define LIB_BSP_ALG_H

//#define INT32 int
//#define int32_t int

/** 
 * @brief This function configures whether the algorithms should be enable
 * @param[in] config is 0 to disable all features
 * bit0: snr check, set 1 to enable
 * bit1: HR, set 1 to enable
 * bit2: SpO2, set 1 to enable
 * bit3: BP, set 1 to enable
 * bit4: HRV, set 1 to enable
 *
*/
void mt6381_config_feature(int config);

int checkQuality(int *ppg1, int ppg1_len,
                 int *ppg2, int ppg2_len,
                 int *ecg, int ecg_len);
void check_quality_init(int ecg_enable);
/** 
 * @brief This function sets the user information for the blood pressure algorithm library. Call this function right after initializing the algorithm to include user's setting for all the other calculations.
 * @param[in] age is the user's age.
 * @param[in] gender is the user's gender. If the value is 1, the gender is male, otherwise it is female.
 * @param[in] height is the user's height. The unit of height is centimeters. 
 * @param[in] weight is the user's weight. The unit of weight is kilograms.
 * @param[in] arm_length is the user's arm length. The unit of length is centimeters.
*/
void bp_alg_set_user_info(int32_t age, int32_t gender, int32_t height, int32_t weight, int32_t arm_length);
void bp_alg_set_calibration_data(int32_t* data_in,  int32_t data_in_len);
void bp_alg_get_calibration_data(int32_t* data_out, int32_t data_out_len);

/** 
 * @brief Call this function to get the pulse rate BPM value from optical heart rate monitor.
 * @return Return the HR value in BPM. 
*/
int spo2_get_bpm(void);

/** 
 * @brief Call this function to get the SpO2 value
 * @return Return the SPO2 value in percentage. 
*/
int spo2_get_spo2(void);

int32_t bp_alg_get_sbp(void);
int32_t bp_alg_get_dbp(void);
int32_t bp_alg_get_bpm(void);
int32_t bp_alg_get_hrv_sdnn(void);
int32_t bp_alg_get_hrv_lf(void);
int32_t bp_alg_get_hrv_hf(void);
int32_t bp_alg_get_hrv_lf_hf(void);
int32_t bp_alg_get_hrv_fatigue(void);
int32_t bp_alg_get_hrv_pressure(void);

#endif
