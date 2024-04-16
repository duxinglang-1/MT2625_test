#include "syslog.h"
#include "GUI.h"
#include "image_info.h"
//extern GUI_CONST_STORAGE GUI_BITMAP bmmain_AudioPlayer;
//extern GUI_CONST_STORAGE GUI_BITMAP bmmain_AudioPlayer;
//extern GUI_CONST_STORAGE GUI_BITMAP bmc;
//extern GUI_CONST_STORAGE GUI_BITMAP bm0;
//extern GUI_CONST_STORAGE GUI_BITMAP bm1;
//extern GUI_CONST_STORAGE GUI_BITMAP bm2;
//extern GUI_CONST_STORAGE GUI_BITMAP bm3;
//extern GUI_CONST_STORAGE GUI_BITMAP bm4;
//extern GUI_CONST_STORAGE GUI_BITMAP bm5;
//extern GUI_CONST_STORAGE GUI_BITMAP bm6;
//extern GUI_CONST_STORAGE GUI_BITMAP bm7;
//extern GUI_CONST_STORAGE GUI_BITMAP bm8;
//extern GUI_CONST_STORAGE GUI_BITMAP bm9;
extern GUI_CONST_STORAGE GUI_BITMAP bm00;
extern GUI_CONST_STORAGE GUI_BITMAP bm11;
extern GUI_CONST_STORAGE GUI_BITMAP bm22;
extern GUI_CONST_STORAGE GUI_BITMAP bm33;
extern GUI_CONST_STORAGE GUI_BITMAP bm44;
extern GUI_CONST_STORAGE GUI_BITMAP bm55;
extern GUI_CONST_STORAGE GUI_BITMAP bm66;
extern GUI_CONST_STORAGE GUI_BITMAP bm77;
extern GUI_CONST_STORAGE GUI_BITMAP bm88;
extern GUI_CONST_STORAGE GUI_BITMAP bm99;
//extern GUI_CONST_STORAGE GUI_BITMAP bmw1;
//extern GUI_CONST_STORAGE GUI_BITMAP bmw2;
//extern GUI_CONST_STORAGE GUI_BITMAP bmw3;
//extern GUI_CONST_STORAGE GUI_BITMAP bmw4;
//extern GUI_CONST_STORAGE GUI_BITMAP bmw5;
//extern GUI_CONST_STORAGE GUI_BITMAP bmw6;
//extern GUI_CONST_STORAGE GUI_BITMAP bmw7;
extern GUI_CONST_STORAGE GUI_BITMAP bmsos;
//extern GUI_CONST_STORAGE GUI_BITMAP bmanimation;
extern GUI_CONST_STORAGE GUI_BITMAP bmLow_battery;
extern GUI_CONST_STORAGE GUI_BITMAP bmstep;
extern GUI_CONST_STORAGE GUI_BITMAP bms0;
extern GUI_CONST_STORAGE GUI_BITMAP bms1;
extern GUI_CONST_STORAGE GUI_BITMAP bms2;
extern GUI_CONST_STORAGE GUI_BITMAP bms3;
extern GUI_CONST_STORAGE GUI_BITMAP bms4;
extern GUI_CONST_STORAGE GUI_BITMAP bms5;
extern GUI_CONST_STORAGE GUI_BITMAP bms6;
extern GUI_CONST_STORAGE GUI_BITMAP bms7;
extern GUI_CONST_STORAGE GUI_BITMAP bms8;
extern GUI_CONST_STORAGE GUI_BITMAP bms9;
extern GUI_CONST_STORAGE GUI_BITMAP bmhart;
extern GUI_CONST_STORAGE GUI_BITMAP bmx0;
extern GUI_CONST_STORAGE GUI_BITMAP bmx1;
extern GUI_CONST_STORAGE GUI_BITMAP bmx2;
extern GUI_CONST_STORAGE GUI_BITMAP bmx3;
extern GUI_CONST_STORAGE GUI_BITMAP bmx4;
extern GUI_CONST_STORAGE GUI_BITMAP bmx5;
extern GUI_CONST_STORAGE GUI_BITMAP bmx6;
extern GUI_CONST_STORAGE GUI_BITMAP bmx7;
extern GUI_CONST_STORAGE GUI_BITMAP bmx8;
extern GUI_CONST_STORAGE GUI_BITMAP bmx9;
extern GUI_CONST_STORAGE GUI_BITMAP bmcdian;

extern GUI_CONST_STORAGE GUI_BITMAP bmdian;
extern GUI_CONST_STORAGE GUI_BITMAP bmdians;
extern GUI_CONST_STORAGE GUI_BITMAP bmhartdw;
extern GUI_CONST_STORAGE GUI_BITMAP bmhartls;
extern GUI_CONST_STORAGE GUI_BITMAP bmc0;
extern GUI_CONST_STORAGE GUI_BITMAP bmc1;
extern GUI_CONST_STORAGE GUI_BITMAP bmc2;
extern GUI_CONST_STORAGE GUI_BITMAP bmc3;
extern GUI_CONST_STORAGE GUI_BITMAP bmc4;
extern GUI_CONST_STORAGE GUI_BITMAP bmc5;
extern GUI_CONST_STORAGE GUI_BITMAP bmc6;
extern GUI_CONST_STORAGE GUI_BITMAP bmc7;
extern GUI_CONST_STORAGE GUI_BITMAP bmc8;
extern GUI_CONST_STORAGE GUI_BITMAP bmc9;
extern GUI_CONST_STORAGE GUI_BITMAP bmstepdw;
extern GUI_CONST_STORAGE GUI_BITMAP bmstepls;
extern GUI_CONST_STORAGE GUI_BITMAP bmcalorie;
extern GUI_CONST_STORAGE GUI_BITMAP bmdistance;
extern GUI_CONST_STORAGE GUI_BITMAP bmblh;
extern GUI_CONST_STORAGE GUI_BITMAP bmbll;
extern GUI_CONST_STORAGE GUI_BITMAP bmpoweroff;
extern GUI_CONST_STORAGE GUI_BITMAP bmbldw;
extern GUI_CONST_STORAGE GUI_BITMAP bmbldwt;
extern GUI_CONST_STORAGE GUI_BITMAP bmgpsopen;
extern GUI_CONST_STORAGE GUI_BITMAP bmgpssuccess;
extern GUI_CONST_STORAGE GUI_BITMAP bmtitle;
extern GUI_CONST_STORAGE GUI_BITMAP bmtips;
extern GUI_CONST_STORAGE GUI_BITMAP bmimeivs;
extern GUI_CONST_STORAGE GUI_BITMAP bmweek;
//extern GUI_CONST_STORAGE GUI_BITMAP bmSedentary_remind;
//extern GUI_CONST_STORAGE GUI_BITMAP bmSedentary_reminder_gg;
extern GUI_CONST_STORAGE GUI_BITMAP bmPower_off_bg;
extern GUI_CONST_STORAGE GUI_BITMAP bmmedian;

//extern GUI_CONST_STORAGE GUI_BITMAP bmtheme;

/* 玺拓版本 */
extern GUI_CONST_STORAGE GUI_BITMAP bmpoint;
extern GUI_CONST_STORAGE GUI_BITMAP bmtime0;
extern GUI_CONST_STORAGE GUI_BITMAP bmtime1;
extern GUI_CONST_STORAGE GUI_BITMAP bmtime2;
extern GUI_CONST_STORAGE GUI_BITMAP bmtime3;
extern GUI_CONST_STORAGE GUI_BITMAP bmtime4;
extern GUI_CONST_STORAGE GUI_BITMAP bmtime5;
extern GUI_CONST_STORAGE GUI_BITMAP bmtime6;
extern GUI_CONST_STORAGE GUI_BITMAP bmtime7;
extern GUI_CONST_STORAGE GUI_BITMAP bmtime8;
extern GUI_CONST_STORAGE GUI_BITMAP bmtime9;


extern GUI_CONST_STORAGE GUI_BITMAP bmbattery1;
extern GUI_CONST_STORAGE GUI_BITMAP bmbattery2;
extern GUI_CONST_STORAGE GUI_BITMAP bmbattery3;
extern GUI_CONST_STORAGE GUI_BITMAP bmbattery4;
extern GUI_CONST_STORAGE GUI_BITMAP bmbattery5;
extern GUI_CONST_STORAGE GUI_BITMAP bmbattery6;
extern GUI_CONST_STORAGE GUI_BITMAP bmbattery7;
extern GUI_CONST_STORAGE GUI_BITMAP bmbattery8;
extern GUI_CONST_STORAGE GUI_BITMAP bmbattery9;
extern GUI_CONST_STORAGE GUI_BITMAP bmbattery10;

extern GUI_CONST_STORAGE GUI_BITMAP bmweek0;
extern GUI_CONST_STORAGE GUI_BITMAP bmweek00;
extern GUI_CONST_STORAGE GUI_BITMAP bmweek1;
extern GUI_CONST_STORAGE GUI_BITMAP bmweek2;
extern GUI_CONST_STORAGE GUI_BITMAP bmweek3;
extern GUI_CONST_STORAGE GUI_BITMAP bmweek4;
extern GUI_CONST_STORAGE GUI_BITMAP bmweek5;
extern GUI_CONST_STORAGE GUI_BITMAP bmweek6;
extern GUI_CONST_STORAGE GUI_BITMAP bmweek7;

extern GUI_CONST_STORAGE GUI_BITMAP bmLanguagegr;
extern GUI_CONST_STORAGE GUI_BITMAP bmLanguagewr;
extern GUI_CONST_STORAGE GUI_BITMAP bmSprachegr;
extern GUI_CONST_STORAGE GUI_BITMAP bmSprachewr;
extern GUI_CONST_STORAGE GUI_BITMAP bmDEGR;
extern GUI_CONST_STORAGE GUI_BITMAP bmDEWR;
extern GUI_CONST_STORAGE GUI_BITMAP bmENGE;
extern GUI_CONST_STORAGE GUI_BITMAP bmENWR;



//extern GUI_CONST_STORAGE GUI_BITMAP bmweathericon0;
//extern GUI_CONST_STORAGE GUI_BITMAP bmweathericon1;
//extern GUI_CONST_STORAGE GUI_BITMAP bmweathericon2;
//extern GUI_CONST_STORAGE GUI_BITMAP bmweathericon3;
//extern GUI_CONST_STORAGE GUI_BITMAP bmweathericon4;
//extern GUI_CONST_STORAGE GUI_BITMAP bmweathericon6;
//extern GUI_CONST_STORAGE GUI_BITMAP bmweathericon6;
//extern GUI_CONST_STORAGE GUI_BITMAP bmweathericon7;
//extern GUI_CONST_STORAGE GUI_BITMAP bmweathericon8;
//extern GUI_CONST_STORAGE GUI_BITMAP bmweathericon9;
//extern GUI_CONST_STORAGE GUI_BITMAP bmweathericon10;
//extern GUI_CONST_STORAGE GUI_BITMAP bmweathericon11;
//extern GUI_CONST_STORAGE GUI_BITMAP bmweathericon12;
//extern GUI_CONST_STORAGE GUI_BITMAP bmweathericon13;
//extern GUI_CONST_STORAGE GUI_BITMAP bmweathericon14;
//extern GUI_CONST_STORAGE GUI_BITMAP bmweathericon15;
//extern GUI_CONST_STORAGE GUI_BITMAP bmweathericon16;
//extern GUI_CONST_STORAGE GUI_BITMAP bmweathericon17;
//extern GUI_CONST_STORAGE GUI_BITMAP bmweathericon18;
//extern GUI_CONST_STORAGE GUI_BITMAP bmweathericon20;
//extern GUI_CONST_STORAGE GUI_BITMAP bmweathericon21;
//extern GUI_CONST_STORAGE GUI_BITMAP bmmedicine;
//extern GUI_CONST_STORAGE GUI_BITMAP bmcurr_medicine;
//extern GUI_CONST_STORAGE GUI_BITMAP bmmedicine_hour;
//extern GUI_CONST_STORAGE GUI_BITMAP bmmedicine_hour_only;
//extern GUI_CONST_STORAGE GUI_BITMAP bmmedicine_min;
//extern GUI_CONST_STORAGE GUI_BITMAP bmmedicine_next;

extern GUI_CONST_STORAGE GUI_BITMAP bmnetwork_strong;
extern GUI_CONST_STORAGE GUI_BITMAP bmnetwork_weak;
extern GUI_CONST_STORAGE GUI_BITMAP bmslash;
extern GUI_CONST_STORAGE GUI_BITMAP bmtemperature;
extern GUI_CONST_STORAGE GUI_BITMAP bmStep;
extern GUI_CONST_STORAGE GUI_BITMAP bmStepW;
//extern GUI_CONST_STORAGE GUI_BITMAP bmsedentary;
extern GUI_CONST_STORAGE GUI_BITMAP bmmovement;
extern GUI_CONST_STORAGE GUI_BITMAP bmmovement_g;
//extern GUI_CONST_STORAGE GUI_BITMAP bmsedentary_g;
extern GUI_CONST_STORAGE GUI_BITMAP bmdata_syn;
extern GUI_CONST_STORAGE GUI_BITMAP bmdata_syn_g;
extern GUI_CONST_STORAGE GUI_BITMAP bmStep_bg;
extern GUI_CONST_STORAGE GUI_BITMAP bmhr_bg;

extern GUI_CONST_STORAGE GUI_BITMAP bmsyn_fail;
extern GUI_CONST_STORAGE GUI_BITMAP bmsyn_succe;
extern GUI_CONST_STORAGE GUI_BITMAP bmsyn_ing;

extern GUI_CONST_STORAGE GUI_BITMAP bmbp_bg;
extern GUI_CONST_STORAGE GUI_BITMAP bmbp_font;
extern GUI_CONST_STORAGE GUI_BITMAP bmhr;
extern GUI_CONST_STORAGE GUI_BITMAP bmbp;
extern GUI_CONST_STORAGE GUI_BITMAP bmcharging;
extern GUI_CONST_STORAGE GUI_BITMAP bmcomplete;
extern GUI_CONST_STORAGE GUI_BITMAP bmBright_screen;
extern GUI_CONST_STORAGE GUI_BITMAP bmBright_screen_g;
//extern GUI_CONST_STORAGE GUI_BITMAP bmPower_saving;
//extern GUI_CONST_STORAGE GUI_BITMAP bmPower_saving_g;
extern GUI_CONST_STORAGE GUI_BITMAP bmoff;
extern GUI_CONST_STORAGE GUI_BITMAP bmon;
extern GUI_CONST_STORAGE GUI_BITMAP bmgps_bg;
extern GUI_CONST_STORAGE GUI_BITMAP bmgps_flag1;
extern GUI_CONST_STORAGE GUI_BITMAP bmgps_flag2;
extern GUI_CONST_STORAGE GUI_BITMAP bmgps_font;
extern GUI_CONST_STORAGE GUI_BITMAP bmgps_font2;
extern GUI_CONST_STORAGE GUI_BITMAP bmlocation_flag;
extern GUI_CONST_STORAGE GUI_BITMAP bmstep_flag;
extern GUI_CONST_STORAGE GUI_BITMAP bmFlight;
extern GUI_CONST_STORAGE GUI_BITMAP bmactivity_time;
//extern GUI_CONST_STORAGE GUI_BITMAP bmsedentary_time;
extern GUI_CONST_STORAGE GUI_BITMAP bmsleep_time;
extern GUI_CONST_STORAGE GUI_BITMAP bmbt;
extern GUI_CONST_STORAGE GUI_BITMAP bmmodel_line;

extern GUI_CONST_STORAGE GUI_BITMAP bmred_point;
extern GUI_CONST_STORAGE GUI_BITMAP bmblue_point;
extern GUI_CONST_STORAGE GUI_BITMAP bmyellow_point;

extern GUI_CONST_STORAGE GUI_BITMAP bmkcalorie_cn;
extern GUI_CONST_STORAGE GUI_BITMAP bmkm_cn;
extern GUI_CONST_STORAGE GUI_BITMAP bmm_cn;
extern GUI_CONST_STORAGE GUI_BITMAP bmhour_cn;
extern GUI_CONST_STORAGE GUI_BITMAP bmmin_cn;

extern GUI_CONST_STORAGE GUI_BITMAP bmnum_0;
extern GUI_CONST_STORAGE GUI_BITMAP bmnum_1;
extern GUI_CONST_STORAGE GUI_BITMAP bmnum_2;
extern GUI_CONST_STORAGE GUI_BITMAP bmnum_3;
extern GUI_CONST_STORAGE GUI_BITMAP bmnum_4;
extern GUI_CONST_STORAGE GUI_BITMAP bmnum_5;
extern GUI_CONST_STORAGE GUI_BITMAP bmnum_6;
extern GUI_CONST_STORAGE GUI_BITMAP bmnum_7;
extern GUI_CONST_STORAGE GUI_BITMAP bmnum_8;
extern GUI_CONST_STORAGE GUI_BITMAP bmnum_9;
extern GUI_CONST_STORAGE GUI_BITMAP bmLow_charge;
extern GUI_CONST_STORAGE GUI_BITMAP bmjan_01;
extern GUI_CONST_STORAGE GUI_BITMAP bmfeb_02;
extern GUI_CONST_STORAGE GUI_BITMAP bmmar_03;                                   
extern GUI_CONST_STORAGE GUI_BITMAP bmapr_04;                                   
extern GUI_CONST_STORAGE GUI_BITMAP bmmay_05;                                   
extern GUI_CONST_STORAGE GUI_BITMAP bmjun_06;                                   
extern GUI_CONST_STORAGE GUI_BITMAP bmjul_07;                                   
extern GUI_CONST_STORAGE GUI_BITMAP bmaug_08;                                   
extern GUI_CONST_STORAGE GUI_BITMAP bmsep_09;                                   
extern GUI_CONST_STORAGE GUI_BITMAP bmoct_10;                                   
extern GUI_CONST_STORAGE GUI_BITMAP bmnov_11;                                   
extern GUI_CONST_STORAGE GUI_BITMAP bmdec_12;                                   
extern GUI_CONST_STORAGE GUI_BITMAP bmsignal_0;                                   
extern GUI_CONST_STORAGE GUI_BITMAP bmsignal_1;    
extern GUI_CONST_STORAGE GUI_BITMAP bmsignal_2;    
extern GUI_CONST_STORAGE GUI_BITMAP bmsignal_3;    
extern GUI_CONST_STORAGE GUI_BITMAP bmsignal_4;    
extern GUI_CONST_STORAGE GUI_BITMAP bmstep0;
extern GUI_CONST_STORAGE GUI_BITMAP bmstep1;	
extern GUI_CONST_STORAGE GUI_BITMAP bmstep2;	
extern GUI_CONST_STORAGE GUI_BITMAP bmstep3;	
extern GUI_CONST_STORAGE GUI_BITMAP bmstep4;	
extern GUI_CONST_STORAGE GUI_BITMAP bmstep5;	
extern GUI_CONST_STORAGE GUI_BITMAP bmstep6;	
extern GUI_CONST_STORAGE GUI_BITMAP bmstep7;	
extern GUI_CONST_STORAGE GUI_BITMAP bmstep8;	
extern GUI_CONST_STORAGE GUI_BITMAP bmstep9;
extern GUI_CONST_STORAGE GUI_BITMAP bmhr_x_0;       
extern GUI_CONST_STORAGE GUI_BITMAP bmhr_x_1;		
extern GUI_CONST_STORAGE GUI_BITMAP bmhr_x_2;		
extern GUI_CONST_STORAGE GUI_BITMAP bmhr_x_3;	 	
extern GUI_CONST_STORAGE GUI_BITMAP bmhr_x_4;		
extern GUI_CONST_STORAGE GUI_BITMAP bmhr_x_5;		
extern GUI_CONST_STORAGE GUI_BITMAP bmhr_x_6;		
extern GUI_CONST_STORAGE GUI_BITMAP bmhr_x_7;		
extern GUI_CONST_STORAGE GUI_BITMAP bmhr_x_8;		
extern GUI_CONST_STORAGE GUI_BITMAP bmhr_x_9;		
extern GUI_CONST_STORAGE GUI_BITMAP bmhr_x_xie;	
extern GUI_CONST_STORAGE GUI_BITMAP bmhr_x_dian;	
extern GUI_CONST_STORAGE GUI_BITMAP bmdian_1;
extern GUI_CONST_STORAGE GUI_BITMAP bmgps_bg_fall;
extern GUI_CONST_STORAGE GUI_BITMAP bmFactory_Reset_g;
extern GUI_CONST_STORAGE GUI_BITMAP bmFactory_Reset;  

//extern GUI_CONST_STORAGE GUI_BITMAP bmmmHg;  
//extern GUI_CONST_STORAGE GUI_BITMAP bmKCAL;
//extern GUI_CONST_STORAGE GUI_BITMAP bmKM;
//extern GUI_CONST_STORAGE GUI_BITMAP bmM;
extern GUI_CONST_STORAGE GUI_BITMAP bmAkku_geladen;
extern GUI_CONST_STORAGE GUI_BITMAP bmBitte_Laden;
extern GUI_CONST_STORAGE GUI_BITMAP bmWird_geladen ;
//extern GUI_CONST_STORAGE GUI_BITMAP bmBPM ;
extern GUI_CONST_STORAGE GUI_BITMAP bmJanuar01;
extern GUI_CONST_STORAGE GUI_BITMAP bmfeb02;
extern GUI_CONST_STORAGE GUI_BITMAP bmmrs03;
extern GUI_CONST_STORAGE GUI_BITMAP bmapr04;
extern GUI_CONST_STORAGE GUI_BITMAP bmmai05;
extern GUI_CONST_STORAGE GUI_BITMAP bmjun06;
extern GUI_CONST_STORAGE GUI_BITMAP bmjul07;
extern GUI_CONST_STORAGE GUI_BITMAP bmaug08;
extern GUI_CONST_STORAGE GUI_BITMAP bmsept09;
extern GUI_CONST_STORAGE GUI_BITMAP bmokt10;
extern GUI_CONST_STORAGE GUI_BITMAP bmnov11;
extern GUI_CONST_STORAGE GUI_BITMAP bmdez12;
extern GUI_CONST_STORAGE GUI_BITMAP bmmo01;
extern GUI_CONST_STORAGE GUI_BITMAP bmdi02;
extern GUI_CONST_STORAGE GUI_BITMAP bmmi03;
extern GUI_CONST_STORAGE GUI_BITMAP bmdo04;
extern GUI_CONST_STORAGE GUI_BITMAP bmfr05;
extern GUI_CONST_STORAGE GUI_BITMAP bmsa06;
extern GUI_CONST_STORAGE GUI_BITMAP bmso07;
extern GUI_CONST_STORAGE GUI_BITMAP bmlocation_nofound;
extern GUI_CONST_STORAGE GUI_BITMAP bmlocationge;



extern GUI_CONST_STORAGE GUI_BITMAP bmGE_Std;


extern GUI_CONST_STORAGE GUI_BITMAP bmgepower_on_logo;
extern GUI_CONST_STORAGE GUI_BITMAP bmGE_Poweroff_logo;
extern GUI_CONST_STORAGE GUI_BITMAP bmAuto_Bildschirm_aufwachen;



extern GUI_CONST_STORAGE GUI_BITMAP bmSyncEinstellungeng;
extern GUI_CONST_STORAGE GUI_BITMAP bmSyncEinstellungnw;

//extern GUI_CONST_STORAGE GUI_BITMAP bmBewegungs_erinnerung;
//extern GUI_CONST_STORAGE GUI_BITMAP bmBewegungs_erinnerungblue;
extern GUI_CONST_STORAGE GUI_BITMAP bms_Auto_Bildschim_aufwachen;
//extern GUI_CONST_STORAGE GUI_BITMAP bmStromspar_modusg;
//extern GUI_CONST_STORAGE GUI_BITMAP bmSync_Einstellungen;
//extern GUI_CONST_STORAGE GUI_BITMAP bmSyncEinstellungen;
extern GUI_CONST_STORAGE GUI_BITMAP bmZurucksetze;
extern GUI_CONST_STORAGE GUI_BITMAP bmZurucksetzenblue;
extern GUI_CONST_STORAGE GUI_BITMAP bmGESLEEP_MIN;
extern GUI_CONST_STORAGE GUI_BITMAP bmGESLEEP_Std;
extern GUI_CONST_STORAGE GUI_BITMAP bmSTEPS;
extern GUI_CONST_STORAGE GUI_BITMAP bmN1_FW_Version;
extern GUI_CONST_STORAGE GUI_BITMAP bmIMEI_No;
extern GUI_CONST_STORAGE GUI_BITMAP bmSTEPS;



























GUI_CONST_STORAGE image_info_struct image_information[] = 
{
	//{IMAGE_INDEX_TIME_H0,&bm0},
    //{IMAGE_INDEX_TIME_H1,&bm1},
    //{IMAGE_INDEX_TIME_H2,&bm2},
    //{IMAGE_INDEX_TIME_H3,&bm3},
    //{IMAGE_INDEX_TIME_H4,&bm4},
    //{IMAGE_INDEX_TIME_H5,&bm5},
    //{IMAGE_INDEX_TIME_H6,&bm6},
    //{IMAGE_INDEX_TIME_H7,&bm7},
    //{IMAGE_INDEX_TIME_H8,&bm8},
    //{IMAGE_INDEX_TIME_H9,&bm9},
    {IMAGE_INDEX_TIME_H00,&bm00},
    {IMAGE_INDEX_TIME_H11,&bm11},
    {IMAGE_INDEX_TIME_H22,&bm22},
    {IMAGE_INDEX_TIME_H33,&bm33},
    {IMAGE_INDEX_TIME_H44,&bm44},
    {IMAGE_INDEX_TIME_H55,&bm55},
    {IMAGE_INDEX_TIME_H66,&bm66},
    {IMAGE_INDEX_TIME_H77,&bm77},
    {IMAGE_INDEX_TIME_H88,&bm88},
    {IMAGE_INDEX_TIME_H99,&bm99},
    {IMAGE_INDEX_TIME_DIAN,&bmdian},
    {IMAGE_INDEX_TIME_DIANS,&bmdians},
    {IMAGE_INDEX_TIME_SOS,&bmsos},
    //{IMAGE_INDEX_animation,&bmanimation},
	{IMAGE_INDEX_Low_battery,&bmLow_battery},
    {IMAGE_INDEX_STEP_S0,&bms0},
    {IMAGE_INDEX_STEP_S1,&bms1},
    {IMAGE_INDEX_STEP_S2,&bms2},
    {IMAGE_INDEX_STEP_S3,&bms3},
    {IMAGE_INDEX_STEP_S4,&bms4},
    {IMAGE_INDEX_STEP_S5,&bms5},
    {IMAGE_INDEX_STEP_S6,&bms6},
    {IMAGE_INDEX_STEP_S7,&bms7},
    {IMAGE_INDEX_STEP_S8,&bms8},
    {IMAGE_INDEX_STEP_S9,&bms9},
    {IMAGE_INDEX_HART_ID,&bmhart},
    {IMAGE_INDEX_HART_X0,&bmx0},
    {IMAGE_INDEX_HART_X1,&bmx1},
    {IMAGE_INDEX_HART_X2,&bmx2},
    {IMAGE_INDEX_HART_X3,&bmx3},
    {IMAGE_INDEX_HART_X4,&bmx4},
    {IMAGE_INDEX_HART_X5,&bmx5},
    {IMAGE_INDEX_HART_X6,&bmx6},
    {IMAGE_INDEX_HART_X7,&bmx7},
    {IMAGE_INDEX_HART_X8,&bmx8},
    {IMAGE_INDEX_HART_X9,&bmx9},
    {IMAGE_INDEX_HART_X9,&bmx9},
    {IMAGE_INDEX_CIRCLE_DIAN,&bmcdian},
    {IMAGE_INDEX_HARTDW_ID,&bmhartdw},
    {IMAGE_INDEX_HARTLS_ID,&bmhartls},
    {IMAGE_INDEX_STEP_C0,&bmc0},
    {IMAGE_INDEX_STEP_C1,&bmc1},
    {IMAGE_INDEX_STEP_C2,&bmc2},
    {IMAGE_INDEX_STEP_C3,&bmc3},
    {IMAGE_INDEX_STEP_C4,&bmc4},
    {IMAGE_INDEX_STEP_C5,&bmc5},
    {IMAGE_INDEX_STEP_C6,&bmc6},
    {IMAGE_INDEX_STEP_C7,&bmc7},
    {IMAGE_INDEX_STEP_C8,&bmc8},
    {IMAGE_INDEX_STEP_C9,&bmc9},
    {IMAGE_INDEX_STEPLS_ID,&bmstepls},
	{IMAGE_INDEX_STEPDW_ID,&bmstepdw},
	{IMAGE_INDEX_CAL,&bmcalorie},
	{IMAGE_INDEX_DIST,&bmdistance},
    {IMAGE_INDEX_BLOOD_H,&bmblh},
    {IMAGE_INDEX_BLOOD_L,&bmbll},
    {IMAGE_INDEX_POWEROFF_IDLE,&bmpoweroff},
    {IMAGE_INDEX_BLOOD_DW,&bmbldw},
	{IMAGE_INDEX_BLOOD_DWT,&bmbldwt},
	{IMAGE_INDEX_GPS_OPEN,&bmgpsopen},
	{IMAGE_INDEX_GPS_SUCCESS,&bmgpssuccess},
	{IMAGE_INDEX_TITLE,&bmtitle},
	{IMAGE_INDEX_TIPS,&bmtips},
	{IMAGE_INDEX_IMEIVS,&bmimeivs},
	{IMAGE_INDEX_week,&bmweek},
	//{IMAGE_INDEX_Sedentary_remind,&bmSedentary_remind},
	//{Sedentaryreminder_gg,&bmSedentary_reminder_gg},
	{Power_off_bg,&bmPower_off_bg},
	{IMAGE_median,&bmmedian},

	
	//{IMAGE_INDEX_theme,&bmtheme},

	/*  上海玺拓版本  */
	{IMAGE_XiTuo_point,&bmpoint},
	{IMAGE_XiTuo_time0,&bmtime0},
    {IMAGE_XiTuo_time1,&bmtime1},
    {IMAGE_XiTuo_time2,&bmtime2},
    {IMAGE_XiTuo_time3,&bmtime3},
    {IMAGE_XiTuo_time4,&bmtime4},
    {IMAGE_XiTuo_time5,&bmtime5},
    {IMAGE_XiTuo_time6,&bmtime6},
    {IMAGE_XiTuo_time7,&bmtime7},
    {IMAGE_XiTuo_time8,&bmtime8},
    {IMAGE_XiTuo_time9,&bmtime9},

    {IMAGE_XiTuo_battery1,&bmbattery1},
    {IMAGE_XiTuo_battery2,&bmbattery2},
    {IMAGE_XiTuo_battery3,&bmbattery3},
    {IMAGE_XiTuo_battery4,&bmbattery4},
    {IMAGE_XiTuo_battery5,&bmbattery5},
    {IMAGE_XiTuo_battery6,&bmbattery6},
    {IMAGE_XiTuo_battery7,&bmbattery7},
    {IMAGE_XiTuo_battery8,&bmbattery8},
    {IMAGE_XiTuo_battery9,&bmbattery9},
	{IMAGE_XiTuo_battery10,&bmbattery10},
	
	{IMAGE_XiTuo_week0,&bmweek0},
	{IMAGE_XiTuo_week00,&bmweek00},
	{IMAGE_XiTuo_week1,&bmweek1},
	{IMAGE_XiTuo_week2,&bmweek2},
	{IMAGE_XiTuo_week3,&bmweek3},
	{IMAGE_XiTuo_week4,&bmweek4},
	{IMAGE_XiTuo_week5,&bmweek5},
	{IMAGE_XiTuo_week6,&bmweek6},
	{IMAGE_XiTuo_week7,&bmweek7},

    {IMAGE_XiTuo_Languagegr,&bmLanguagegr},
	{IMAGE_XiTuo_Languagewr,&bmLanguagewr},
	{IMAGE_XiTuo_Sprachegr, &bmSprachegr},
	{IMAGE_XiTuo_Sprachewr, &bmSprachewr},
	{IMAGE_XiTuo_DEGR,      &bmDEGR},
	{IMAGE_XiTuo_DEWR,      &bmDEWR},
	{IMAGE_XiTuo_ENGE,      &bmENGE},
	{IMAGE_XiTuo_ENWR,      &bmENWR},
	{IMAGE_XiTuo_network_strong,&bmnetwork_strong},
	{IMAGE_XiTuo_network_weak,&bmnetwork_weak},
	{IMAGE_XiTuo_temperature,&bmtemperature},
	
	{IMAGE_XiTuo_slash,&bmslash},	
	{IMAGE_XiTuo_Step,&bmStep},
	{IMAGE_XiTuo_StepW,&bmStepW},
	{IMAGE_XiTuo_Step_bg,&bmStep_bg},
	{IMAGE_XiTuo_hr_bg,&bmhr_bg},
	{IMAGE_XiTuo_bp_bg,&bmbp_bg},
	{IMAGE_XiTuo_bp_font,&bmbp_font},
	{IMAGE_XiTuo_hr,&bmhr},
	{IMAGE_XiTuo_bp,&bmbp},
	{IMAGE_XiTuo_Bright_screen,&bmBright_screen},
	{IMAGE_XiTuo_Bright_screen_g,&bmBright_screen_g},
	//{IMAGE_XiTuo_Power_saving,&bmPower_saving},
	//{IMAGE_XiTuo_Power_saving_g,&bmPower_saving_g},
	{IMAGE_XiTuo_charging,&bmcharging},
	{IMAGE_XiTuo_complete,&bmcomplete},
	{IMAGE_XiTuo_off,&bmoff},
	{IMAGE_XiTuo_on,&bmon},
	{IMAGE_XiTuo_gps_bg,&bmgps_bg},
	{IMAGE_XiTuo_gps_flag1,&bmgps_flag1},
	{IMAGE_XiTuo_gps_flag2,&bmgps_flag2},
	{IMAGE_XiTuo_gps_font,&bmgps_font},
	{IMAGE_XiTuo_gps_font2,&bmgps_font2},
	{IMAGE_INDEX_location_flag,&bmlocation_flag},
	{IMAGE_INDEX_step_flag,&bmstep_flag},
	{IMAGE_INDEX_Flight,&bmFlight},
	{IMAGE_INDEX_activity_time,&bmactivity_time},
	//{IMAGE_INDEX_sedentary_time,&bmsedentary_time},
	{IMAGE_INDEX_sleep_time,&bmsleep_time},
	{IMAGE_INDEX_bt,&bmbt},
	{IMAGE_model_line,&bmmodel_line},

    
	{IMAGE_syn_fail,&bmsyn_fail},	
	{IMAGE_syn_sucess,&bmsyn_succe},
	{IMAGE_syn_ing,&bmsyn_ing},
#if defined(LOGO_SUPPORT_3)	
	{IMAGE_XiTuo_movement,&bmmovement},
	{IMAGE_XiTuo_movement_g,&bmmovement_g},
//#else
//	{IMAGE_XiTuo_sedentary,&bmsedentary},
//	{IMAGE_XiTuo_sedentary_g,&bmsedentary_g},
#endif  
	{IMAGE_XiTuo_data_syn,&bmdata_syn},
	{IMAGE_XiTuo_data_syn_g,&bmdata_syn_g},

    {IMAGE_RED_POINT,&bmred_point},
    {IMAGE_BLUE_POINT,&bmblue_point},
    {IMAGE_YELLOW_POINT,&bmyellow_point},

    {IMAGE_KCALORIE_CN,&bmkcalorie_cn},
    {IMAGE_KM_CN,&bmkm_cn},
    {IMAGE_M_CN,&bmm_cn},
    {IMAGE_HOUR_CN,&bmhour_cn},
    {IMAGE_MIN_CN,&bmmin_cn},

	{IMAGE_NUMM_0,&bmnum_0},
	{IMAGE_NUMM_1,&bmnum_1},
	{IMAGE_NUMM_2,&bmnum_2},
	{IMAGE_NUMM_3,&bmnum_3},
	{IMAGE_NUMM_4,&bmnum_4},
	{IMAGE_NUMM_5,&bmnum_5},
	{IMAGE_NUMM_6,&bmnum_6},
	{IMAGE_NUMM_7,&bmnum_7},
	{IMAGE_NUMM_8,&bmnum_8},
	{IMAGE_NUMM_9,&bmnum_9},
    {Low_battery_Please_charge,&bmLow_charge},
	{jan_01,&bmjan_01},
	{feb_02,&bmfeb_02},
	{mar_03,&bmmar_03},
	{apr_04,&bmapr_04},
	{may_05,&bmmay_05},
	{jun_06,&bmjun_06},
	{jul_07,&bmjul_07},
	{aug_08,&bmaug_08},
	{sep_09,&bmsep_09},
	{oct_10,&bmoct_10},
	{nov_11,&bmnov_11},
	{dec_12,&bmdec_12},
	{signal_0,&bmsignal_0},
	{signal_1,&bmsignal_1},
	{signal_2,&bmsignal_2},
	{signal_3,&bmsignal_3},
	{signal_4,&bmsignal_4},
	{step0,&bmstep0},
	{step1,&bmstep1},
	{step2,&bmstep2},
	{step3,&bmstep3},
	{step4,&bmstep4},
	{step5,&bmstep5},
	{step6,&bmstep6},
	{step7,&bmstep7},
	{step8,&bmstep8},
	{step9,&bmstep9},
	{hr_x_0,&bmhr_x_0},            
	{hr_x_1,&bmhr_x_1},			
	{hr_x_2,&bmhr_x_2},			
	{hr_x_3,&bmhr_x_3},		 	
	{hr_x_4,&bmhr_x_4},			
	{hr_x_5,&bmhr_x_5},			
	{hr_x_6,&bmhr_x_6},			
	{hr_x_7,&bmhr_x_7},			
	{hr_x_8,&bmhr_x_8},			
	{hr_x_9,&bmhr_x_9},			
	{hr_x_xie,&bmhr_x_xie},			
	{hr_x_dian,&bmhr_x_dian},			
	{dian_1,&bmdian_1},
	{gps_bg_fall,&bmgps_bg_fall},
	{IMAGE_Factory_Reset_g,&bmFactory_Reset_g},
	{IMAGE_Factory_Reset,&bmFactory_Reset},
	#if defined(German_SUPPORT)
	//{IMAGE_GE_mmHg,&bmmmHg},
	//{IMAGE_GE_KCAL,&bmKCAL},
	//{IMAGE_GE_KM,&bmKM },
	//{IMAGE_GE_M,&bmM},
	{IMAGE_GE_Akku_geladen,&bmAkku_geladen},
	{IMAGE_GE_Bitte_Laden,&bmBitte_Laden},
	{IMAGE_GE_Wird_geladen,&bmWird_geladen},
	//{IMAGE_GE_BPM,&bmBPM},
	{IMAGE_GE_Januar01,&bmJanuar01},
	{IMAGE_GE_feb02,&bmfeb02},
	{IMAGE_GE_mrs03,&bmmrs03},
	{IMAGE_GE_apr04,&bmapr04},
	{IMAGE_GE_mai05,&bmmai05},
	{IMAGE_GE_jun06,&bmjun06},
	{IMAGE_GE_jul07,&bmjul07},
	{IMAGE_GE_aug08,&bmaug08},
	{IMAGE_GE_sept09,&bmsept09},
	{IMAGE_GE_okt10,&bmokt10},
	{IMAGE_GE_nov11,&bmnov11},
	{IMAGE_GE_dez12,&bmdez12},
	{IMAGE_GE_mo01,&bmmo01},
	{IMAGE_GE_di02,&bmdi02},
	{IMAGE_GE_mi03,&bmmi03},
	{IMAGE_GE_do04,&bmdo04},
	{IMAGE_GE_fr05,&bmfr05},
	{IMAGE_GE_sa06,&bmsa06},
	{IMAGE_GE_so07,&bmso07},
	{IMAGE_GE_location_nofound,&bmlocation_nofound},
	{IMAGE_GE_locationge,&bmlocationge},
    {IMAGE_GE_gepower_on_logo,&bmgepower_on_logo},
	{IMAGE_GE_Poweroff_logo,&bmGE_Poweroff_logo},
	{IMAGE_GE_Auto_Bildschirm_aufwachen,&bmAuto_Bildschirm_aufwachen},



	{IMAGE_SyncEinstellungeng,&bmSyncEinstellungeng},
	{IMAGE_SyncEinstellungnw,&bmSyncEinstellungnw},
	
	
	//{IMAGE_GE_Bewegungs_erinnerung,&bmBewegungs_erinnerung},
	//{IMAGE_GE_Bewegungs_erinnerungblue,&bmBewegungs_erinnerungblue},
	{IMAGE_GE_s_Auto_Bildschim_aufwachen,&bms_Auto_Bildschim_aufwachen},
	//{IMAGE_GE_Stromspar_modusg,&bmStromspar_modusg},
	//{IMAGE_GE_Sync_Einstellungen,&bmSync_Einstellungen},
	//{IMAGE_GE_SyncEinstellungen,&bmSyncEinstellungen},
	{IMAGE_GE_Zurucksetze,&bmZurucksetze},
	{IMAGE_GE_Zurucksetzenblue,&bmZurucksetzenblue},
	{IMAGE_GE_GESLEEP_MIN,&bmGESLEEP_MIN },
	{IMAGE_GE_GESLEEP_Std,&bmGESLEEP_Std},
	{IMAGE_GE_STEPS,&bmSTEPS},
	{IMAGE_GE_N1_FW_Version,&bmN1_FW_Version},
	{IMAGE_GE_IMEI_No,&bmIMEI_No},
	#endif
	{IMAGW_INDEX_END,NULL}
};

uint32_t gdi_get_max_image_num(void)
{
    return IMAGW_INDEX_END;
}


