#ifndef _IMAGE_INFO_H_
#define _IMAGE_INFO_H_

typedef enum{
    /*start add you image id here*/
	//IMAGE_INDEX_AUDIO,
	IMAGE_INDEX_LOGO,
	IMAGE_INDEX_GIRL,
	IMAGE_INDEX_TIME,
	IMAGE_INDEX_TIME_H0,
	IMAGE_INDEX_TIME_H1,
	IMAGE_INDEX_TIME_H2,
	IMAGE_INDEX_TIME_H3,
	IMAGE_INDEX_TIME_H4,
	IMAGE_INDEX_TIME_H5,
	IMAGE_INDEX_TIME_H6,
	IMAGE_INDEX_TIME_H7,
	IMAGE_INDEX_TIME_H8,
	IMAGE_INDEX_TIME_H9,
	IMAGE_INDEX_TIME_H00,
    IMAGE_INDEX_TIME_H11,
	IMAGE_INDEX_TIME_H22,
	IMAGE_INDEX_TIME_H33,
	IMAGE_INDEX_TIME_H44,
	IMAGE_INDEX_TIME_H55,
	IMAGE_INDEX_TIME_H66,
	IMAGE_INDEX_TIME_H77,
	IMAGE_INDEX_TIME_H88,
	IMAGE_INDEX_TIME_H99,
	IMAGE_INDEX_TIME_DIAN,
	IMAGE_INDEX_TIME_DIANS,
	IMAGE_INDEX_TIME_W1,
	IMAGE_INDEX_TIME_W2,
	IMAGE_INDEX_TIME_W3,
	IMAGE_INDEX_TIME_W4,
	IMAGE_INDEX_TIME_W5,
	IMAGE_INDEX_TIME_W6,
	IMAGE_INDEX_TIME_W7,
	IMAGE_INDEX_TIME_SOS,
	//IMAGE_INDEX_animation,
	IMAGE_INDEX_Low_battery,
	//IMAGE_INDEX_STEP_ID,
	IMAGE_INDEX_STEP_S0,
	IMAGE_INDEX_STEP_S1,
	IMAGE_INDEX_STEP_S2,
	IMAGE_INDEX_STEP_S3,
	IMAGE_INDEX_STEP_S4,
	IMAGE_INDEX_STEP_S5,
	IMAGE_INDEX_STEP_S6,
	IMAGE_INDEX_STEP_S7,
	IMAGE_INDEX_STEP_S8,
	IMAGE_INDEX_STEP_S9,
	IMAGE_INDEX_GPS_ID,
	IMAGE_INDEX_HART_ID,
	IMAGE_INDEX_HART_X0,
	IMAGE_INDEX_HART_X1,
	IMAGE_INDEX_HART_X2,
	IMAGE_INDEX_HART_X3,
	IMAGE_INDEX_HART_X4,
	IMAGE_INDEX_HART_X5,
	IMAGE_INDEX_HART_X6,
	IMAGE_INDEX_HART_X7,
	IMAGE_INDEX_HART_X8,
	IMAGE_INDEX_HART_X9,
	IMAGE_INDEX_CIRCLE_DIAN,
	IMAGE_INDEX_HARTDW_ID,
	IMAGE_INDEX_HARTLS_ID,
	IMAGE_INDEX_STEP_C0,
	IMAGE_INDEX_STEP_C1,
	IMAGE_INDEX_STEP_C2,
	IMAGE_INDEX_STEP_C3,
	IMAGE_INDEX_STEP_C4,
	IMAGE_INDEX_STEP_C5,
	IMAGE_INDEX_STEP_C6,
	IMAGE_INDEX_STEP_C7,
	IMAGE_INDEX_STEP_C8,
	IMAGE_INDEX_STEP_C9,
	IMAGE_INDEX_STEPLS_ID,
	IMAGE_INDEX_STEPDW_ID,
	IMAGE_INDEX_CAL,
	IMAGE_INDEX_DIST,
	IMAGE_INDEX_BLOOD_H,
	IMAGE_INDEX_BLOOD_L,
	IMAGE_INDEX_BLOOD_DW,
	IMAGE_INDEX_BLOOD_DWT,
	IMAGE_INDEX_GPS_OPEN,
	IMAGE_INDEX_GPS_SUCCESS,
	IMAGE_INDEX_POWEROFF_IDLE,
	IMAGE_INDEX_TITLE,
	IMAGE_INDEX_TIPS,
	IMAGE_INDEX_IMEIVS,
	IMAGE_INDEX_week,
	//IMAGE_INDEX_Sedentary_remind,
	//Sedentaryreminder_gg,
	IMAGE_INDEX_theme,
	IMAGE_INDEX_TIME_TEST,
	Power_off_bg,
	IMAGE_median,
	
	/*	�Ϻ����ذ汾  */
	IMAGE_XiTuo_point,
	IMAGE_XiTuo_time0,
    IMAGE_XiTuo_time1,
    IMAGE_XiTuo_time2,
    IMAGE_XiTuo_time3,
    IMAGE_XiTuo_time4,
    IMAGE_XiTuo_time5,
    IMAGE_XiTuo_time6,
    IMAGE_XiTuo_time7,
    IMAGE_XiTuo_time8,
    IMAGE_XiTuo_time9,	
 
    IMAGE_XiTuo_battery1,
    IMAGE_XiTuo_battery2,
    IMAGE_XiTuo_battery3,
    IMAGE_XiTuo_battery4,
    IMAGE_XiTuo_battery5,
    IMAGE_XiTuo_battery6,
    IMAGE_XiTuo_battery7,
    IMAGE_XiTuo_battery8,
    IMAGE_XiTuo_battery9,
	IMAGE_XiTuo_battery10,
	IMAGE_XiTuo_week0,
	IMAGE_XiTuo_week00,
	IMAGE_XiTuo_week1,
	IMAGE_XiTuo_week2,
	IMAGE_XiTuo_week3,
	IMAGE_XiTuo_week4,
	IMAGE_XiTuo_week5,
	IMAGE_XiTuo_week6,
	IMAGE_XiTuo_week7,
	IMAGE_XiTuo_Languagegr,
	IMAGE_XiTuo_Languagewr,
	IMAGE_XiTuo_Sprachegr, 
	IMAGE_XiTuo_Sprachewr, 
	IMAGE_XiTuo_DEGR,      
	IMAGE_XiTuo_DEWR,      
	IMAGE_XiTuo_ENGE,      
	IMAGE_XiTuo_ENWR,      
	
	//IMAGE_XiTuo_bmweathericon0,
    //IMAGE_XiTuo_bmweathericon1,
    //IMAGE_XiTuo_bmweathericon2,
    //IMAGE_XiTuo_bmweathericon3,
    //IMAGE_XiTuo_bmweathericon4,
    //IMAGE_XiTuo_bmweathericon5,
    //IMAGE_XiTuo_bmweathericon6,
    //IMAGE_XiTuo_bmweathericon7,
    //IMAGE_XiTuo_bmweathericon8,
    //IMAGE_XiTuo_bmweathericon9,
	//IMAGE_XiTuo_bmweathericon10,
	//IMAGE_XiTuo_bmweathericon11,
	//IMAGE_XiTuo_bmweathericon12,
	//IMAGE_XiTuo_bmweathericon13,
	//IMAGE_XiTuo_bmweathericon14,
	//IMAGE_XiTuo_bmweathericon15,
	//IMAGE_XiTuo_bmweathericon16,
	//IMAGE_XiTuo_bmweathericon17,
	//IMAGE_XiTuo_bmweathericon18,
	//IMAGE_XiTuo_bmweathericon20,
    //IMAGE_XiTuo_bmweathericon21,
	IMAGE_XiTuo_network_strong,
	IMAGE_XiTuo_network_weak,
	IMAGE_XiTuo_temperature,
	IMAGE_XiTuo_slash,
	//IMAGE_XiTuo_medicine,
	//IMAGE_XiTuo_curr_medicine,
	//IMAGE_XiTuo_medicine_hour,
	//IMAGE_XiTuo_medicine_hour_only,
	//IMAGE_XiTuo_medicine_min,
	//IMAGE_XiTuo_medicine_next,
	IMAGE_XiTuo_Step,
	IMAGE_XiTuo_StepW,
	IMAGE_XiTuo_Step_bg,
	IMAGE_XiTuo_hr_bg,
	IMAGE_syn_fail,
	IMAGE_syn_sucess,
	IMAGE_syn_ing,
	IMAGE_XiTuo_bp_bg,
	IMAGE_XiTuo_bp_font,
	IMAGE_XiTuo_Bright_screen,
	IMAGE_XiTuo_Bright_screen_g,
	//IMAGE_XiTuo_Power_saving,
	//IMAGE_XiTuo_Power_saving_g,
	IMAGE_XiTuo_charging,
	IMAGE_XiTuo_complete,
	IMAGE_XiTuo_off,
	IMAGE_XiTuo_on,
	IMAGE_XiTuo_hr,
	IMAGE_XiTuo_bp,
	//IMAGE_XiTuo_sedentary,
	//IMAGE_XiTuo_sedentary_g,
	IMAGE_XiTuo_data_syn,
	IMAGE_XiTuo_data_syn_g,
	IMAGE_XiTuo_gps_bg,
	IMAGE_XiTuo_gps_flag1,
	IMAGE_XiTuo_gps_flag2,
	IMAGE_XiTuo_movement,
	IMAGE_XiTuo_movement_g,

	IMAGE_XiTuo_gps_font,
	IMAGE_XiTuo_gps_font2,
	IMAGE_INDEX_location_flag,
	IMAGE_INDEX_step_flag,
	IMAGE_INDEX_Flight,
	IMAGE_INDEX_activity_time,
	//IMAGE_INDEX_sedentary_time,
	IMAGE_INDEX_sleep_time,
	IMAGE_INDEX_bt,
	IMAGE_model_line,
	IMAGE_INDEX_battery,

	IMAGE_RED_POINT,
	IMAGE_BLUE_POINT,
	IMAGE_YELLOW_POINT,
	IMAGE_KCALORIE_CN,
	IMAGE_KM_CN,
	IMAGE_M_CN,
	IMAGE_HOUR_CN,
	IMAGE_MIN_CN,

	IMAGE_NUMM_0,
    IMAGE_NUMM_1,
    IMAGE_NUMM_2,
    IMAGE_NUMM_3,
    IMAGE_NUMM_4,
    IMAGE_NUMM_5,
    IMAGE_NUMM_6,
    IMAGE_NUMM_7,
    IMAGE_NUMM_8,
    IMAGE_NUMM_9,	
    Low_battery_Please_charge,
	jan_01,
	feb_02,
	mar_03,
	apr_04,
	may_05,
	jun_06,
	jul_07,
	aug_08,
	sep_09,
	oct_10,
	nov_11,
	dec_12,
	signal_0,
	signal_1,
	signal_2,
	signal_3,
	signal_4,
	step0,
	step1,
	step2,
	step3,
	step4,
	step5,
	step6,
	step7,
	step8,
	step9,
	hr_x_0,
	hr_x_1,
	hr_x_2,
	hr_x_3,
	hr_x_4,
	hr_x_5,
	hr_x_6,
	hr_x_7,
	hr_x_8,
	hr_x_9,
	hr_x_xie,
	hr_x_dian,
	dian_1,
    gps_bg_fall,
    IMAGE_Factory_Reset_g,
	IMAGE_Factory_Reset,
	#if defined(German_SUPPORT)
	//IMAGE_GE_mmHg,  
	//IMAGE_GE_KCAL,
	//IMAGE_GE_KM,
	//IMAGE_GE_M,
	IMAGE_GE_Akku_geladen,
	IMAGE_GE_Bitte_Laden ,
	IMAGE_GE_Wird_geladen ,
	//IMAGE_GE_BPM ,
	IMAGE_GE_Januar01,
	IMAGE_GE_feb02,
	IMAGE_GE_mrs03,
	IMAGE_GE_apr04,
	IMAGE_GE_mai05,
	IMAGE_GE_jun06,
	IMAGE_GE_jul07,
	IMAGE_GE_aug08,
	IMAGE_GE_sept09,
	IMAGE_GE_okt10,
	IMAGE_GE_nov11,
	IMAGE_GE_dez12,
	IMAGE_GE_mo01,
	IMAGE_GE_di02,
	IMAGE_GE_mi03,
	IMAGE_GE_do04,
	IMAGE_GE_fr05,
	IMAGE_GE_sa06,
	IMAGE_GE_so07,
	IMAGE_GE_location_nofound,
	IMAGE_GE_locationge,
    IMAGE_GE_gepower_on_logo,
	IMAGE_GE_Poweroff_logo,
	IMAGE_GE_Auto_Bildschirm_aufwachen,

	IMAGE_SyncEinstellungeng,
	IMAGE_SyncEinstellungnw,

	//IMAGE_GE_Bewegungs_erinnerung,
	//IMAGE_GE_Bewegungs_erinnerungblue,
	IMAGE_GE_s_Auto_Bildschim_aufwachen,
	//IMAGE_GE_Stromspar_modusg,
	//IMAGE_GE_Sync_Einstellungen,
	//IMAGE_GE_SyncEinstellungen,
	IMAGE_GE_Zurucksetze,
	IMAGE_GE_Zurucksetzenblue,
	IMAGE_GE_GESLEEP_MIN,
	IMAGE_GE_GESLEEP_Std,
	IMAGE_GE_STEPS,
	IMAGE_GE_N1_FW_Version,
	IMAGE_GE_IMEI_No,
	#endif
	IMAGW_INDEX_END
}image_index_enum;

#endif