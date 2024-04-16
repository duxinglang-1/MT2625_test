/*
 * Copyright 2018 All rights reserved. Goodix Inc.
 * Author: Chris-Chan
 * example code for condensed  hbd_ctrl lib. 
 */

/* includes. */
#include "stdint.h"
#include "string.h"
#include "hbd_ctrl.h"


// application mode macro
#define __APP_MODE_ADT_HB_DET__        (2)
#define __APP_MODE_HRV_DET__           (3)    
#define __APP_MODE_HSM_DET__           (4)
#define __APP_MODE_BPD_DET__           (5)
#define __APP_MODE_PFA_DET__           (6)

// hb algorithm scenario macro
#define __HBA_SCENARIO_DEFAULT__       (0)
#define __HBA_SCENARIO_ROUTINE__       (1)
#define __HBA_SCENARIO_RUN__           (2)
#define __HBA_SCENARIO_CLIBM__         (3)
#define __HBA_SCENARIO_BIKE__          (4)
#define __HBA_SCENARIO_IRREGULAR__     (5)

// application config macro
#define __APP_MODE_CONFIG__            		(__APP_MODE_ADT_HB_DET__)
#define __ENABLE_WEARING__                  (1) // wearing algo enable flag
#define __USE_GOODIX_APP__                  (1) // if need use GOODIX app debug
#define __NEED_LOAD_NEW_COFIG__             (1) // use reg_config_array val update gh30x.
#define __GH30X_IRQ_PLUSE_WIDTH_CONFIG__    (1) // gh30x default irq pluse width 20us, if need config irq pluse width, search this macro.
#define __HB_MODE_ENABLE_FIFO__           	(1) 
#define __GS_SENSITIVITY_CONFIG__          	(HBD_GSENSOR_SENSITIVITY_512_COUNTS_PER_G)
#define __HBA_SCENARIO_CONFIG__        		(__HBA_SCENARIO_DEFAULT__)
#define __RETRY_MAX_CNT_CONFIG__            (100) // retry max cnt
#define __START_ADT_AFTER_GSENSOR_MOTION__  (0) //whether need gsensor motion before start adt detect

#if (__USE_GOODIX_APP__)
#include "hbd_communicate.h"
#endif

HBD_INIT_CONFIG_DEFAULT_DEF(HbdInitStruct); 
    
#if (__ENABLE_WEARING__)
    GF32 wearing_config_array[3] = {0, 0, 0}; // 这个值由app根据结构生成，请联系GOODIX输出对应数组
#endif

#if (__NEED_LOAD_NEW_COFIG__) // 此配置数组由APP 生成，与结构相关，请联系GOODIX输出对应数组
	#if (__APP_MODE_CONFIG__ == __APP_MODE_ADT_HB_DET__)
    GU8 reg_config_array[] = 
	{
		0x00, 0x47, 0x48, 0x33, 0x2e, 
		0x46, 0x6f, 0x9a, 0x59, 
		0x65, 0x7a, 0x73, 0xd7, 
		0x55, 0x47, 0x9a, 0x5e, 
		0x67, 0x6f, 0x5d, 0xd9, 
		0x73, 0x7a, 0xbe, 0x55, 
		0x78, 0x49, 0x55, 0xfc, 
		0x64, 0x4b, 0x9e, 0x27, 
		0x68, 0x6a, 0x46, 0xc9, 
		0x4e, 0x44, 0xb2, 0x27, 
		0x6f, 0x74, 0x57, 0xe7, 
		0x40, 0x66, 0x83, 0x03, 
		0x64, 0x6f, 0x7d, 0xef, 
		0x65, 0x79, 0x46, 0x90, 
		0x6e, 0x77, 0x65, 0x87, 
		0x48, 0x6f, 0x55, 0xbc, 
		0x6e, 0x48, 0x64, 0x6b, 
		0x66, 0x69, 0x72, 0x70, 
		0x4a, 0x55, 0x7b, 0x05, 
		0x6e, 0x56, 0x67, 0x01, 
		0x6e, 0x5b, 0x6b, 0x22, 
		0x78, 0x5f, 0x4a, 0x4a, 
		0x43, 0xef, 0x6a, 0x63, 
		0x69, 0xe5, 0x40, 0xb6, 
		0x72, 0xcf, 0x65, 0x47, 
		0x42, 0x77, 0x0a, 0x0b, 
		0x46, 0x75, 0x6f, 0x07, 
		0x65, 0x56, 0x49, 0x49, 
		0x55, 0x75, 0x6f, 0x6e, 
		0x67, 0xef, 0x67, 0x46, 
		0x73, 0xf2, 0x4b, 0x28, 
		0x78, 0x69, 0x6c, 0x67, 
		0x64, 0x6b, 0x6f, 0x6c, 
		0x69, 0xa6, 0xf8, 0x49, 
		0x4f, 0x90, 0x48, 0xd7, 
		0x6e, 0xa0, 0xe9, 0x67, 
		0x41, 0xba, 0x79, 0xf3, 
		0x65, 0xb3, 0x43, 0xcf, 
		0x64, 0xa9, 0x47, 0x01, 
		0x6f, 0xa7, 0x1a, 0x8c, 
		0x49, 0x9f, 0x47, 0xcb, 
		0x6f, 0xba, 0x34, 0x61, 
		0x67, 0x97, 0x79, 0xca, 
		0x4b, 0xbd, 0x7a, 0x42, 
		0x6f, 0xbe, 0x65, 0x47, 
		0x6f, 0xb3, 0x68, 0x65, 
		0x78, 0x97, 0x4f, 0x54,
	};
	GU8 reg_config_array_wear_confirm[] = 
	{
		0x00, 0x47, 0x48, 0x33, 0x10, 
		0x47, 0xef, 0x6d, 0x6c, 
		0x64, 0xfa, 0x49, 0x0d, 
		0x54, 0xc7, 0x6f, 0x4f, 
		0x67, 0x71, 0x4d, 0x6b, 
		0x73, 0x5c, 0x4b, 0x65, 
		0x78, 0x75, 0x66, 0x44, 
		0x64, 0xc1, 0x6f, 0x6e, 
		0x68, 0xe4, 0x78, 0x64, 
		0x4f, 0x96, 0xbc, 0x90, 
		0x6e, 0xa2, 0x29, 0x13, 
		0x41, 0xb4, 0x8d, 0xb4, 
		0x65, 0xb1, 0x43, 0x6f, 
		0x64, 0xab, 0x7d, 0xf7, 
		0x6f, 0xb9, 0x64, 0x78, 
		0x49, 0x9b, 0x54, 0x43, 
		0x6f, 0xb8, 0x66, 0x69, 
	};
	#elif (__APP_MODE_CONFIG__ == __APP_MODE_HRV_DET__)
	GU8 reg_config_array[] = 
    {
        0x00, 0x3E, 0x0A, 0xD3, 0x02, 
        0x47, 0xEB, 0x6F, 0x57,
        0x64, 0x74, 0x48, 0xBF,
    };
	#elif (__APP_MODE_CONFIG__ == __APP_MODE_HSM_DET__)
	GU8 reg_config_array[] = 
    {
        0x00, 0x3E, 0x0A, 0xD3, 0x02, 
        0x47, 0xEB, 0x6F, 0x57,
        0x64, 0x74, 0x48, 0xBF,
    };
    #elif (__APP_MODE_CONFIG__ == __APP_MODE_BPD_DET__)
	GU8 reg_config_array[] = 
    {
        0x00, 0x3E, 0x0A, 0xD3, 0x02, 
        0x47, 0xEB, 0x6F, 0x57,
        0x64, 0x74, 0x48, 0xBF,
    };
    #elif (__APP_MODE_CONFIG__ == __APP_MODE_PFA_DET__)
	GU8 reg_config_array[] = 
    {
        0x00, 0x3E, 0x0A, 0xD3, 0x02, 
        0x47, 0xEB, 0x6F, 0x57,
        0x64, 0x74, 0x48, 0xBF,
    };
	#endif
#endif

#if (__USE_GOODIX_APP__)
int8_t ble_comm_type = 0xFF;
EM_COMM_CMD_TYPE gh30x_status = COMM_CMD_INVALID;
#endif

#if (__ENABLE_WEARING__)
uint8_t last_wearing_state = 1;
#endif

#define GSENSOR_SOFT_FIFO_BUFFER_MAX_LEN (256)
static uint16_t gsensor_soft_fifo_buffer_index = 0;
static ST_GS_DATA_TYPE gsensor_soft_fifo_buffer[GSENSOR_SOFT_FIFO_BUFFER_MAX_LEN];

void GH30x_adt_wear_detect_start(void);
void GH30x_Load_new_config(uint8_t *config_ptr);

/// i2c interface 
void hal_gh30x_i2c_init(void)
{
	// code implement by user
}

uint8_t hal_gh30x_i2c_write(uint8_t device_id,const uint8_t write_buffer[], uint16_t length)
{
	uint8_t ret = 1;
	// code implement by user
	return ret;
}

uint8_t hal_gh30x_i2c_read(uint8_t device_id,const uint8_t write_buffer[], uint16_t write_length, uint8_t read_buffer[], uint16_t read_length)
{
	uint8_t ret = 1;
	// code implement by user
	return ret;
}

uint8_t gsensor_drv_motion_det_mode = 0;
/// gsensor driver
int8_t gsensor_drv_init(void)
{
	int8_t ret = 1;
	gsensor_drv_motion_det_mode = 0;
	// code implement by user
	return ret;
}

void gsensor_drv_enter_normal_mode(void)
{
	// code implement by user
	gsensor_drv_motion_det_mode = 0;
}

void gsensor_drv_enter_fifo_mode(void)
{
	// code implement by user
	gsensor_drv_motion_det_mode = 0;
}

void gsensor_drv_enter_motion_det_mode(void)
{
	// code implement by user
	gsensor_drv_motion_det_mode = 1;
}

void gsensor_drv_get_fifo_data(ST_GS_DATA_TYPE gsensor_buffer[], uint16_t *gsensor_buffer_index, uint16_t gsensor_max_len)
{
	// code implement by user
}

void gsensor_drv_clear_buffer(ST_GS_DATA_TYPE gsensor_buffer[], uint16_t *gsensor_buffer_index, uint16_t gsensor_buffer_len)
{
    memset(gsensor_buffer, 0, sizeof(ST_GS_DATA_TYPE) * gsensor_buffer_len);
    *gsensor_buffer_index = 0;
}

void gsensor_drv_get_data(ST_GS_DATA_TYPE *gsensor_data_ptr)
{
	// code implement by user
}

void gsensor_drv_int1_handler(void)
{
	// code implement by user
	 if (gsensor_drv_motion_det_mode != 0)
	 {
		gsensor_drv_enter_normal_mode();    
		gsensor_drv_clear_buffer(gsensor_soft_fifo_buffer, &gsensor_soft_fifo_buffer_index, GSENSOR_SOFT_FIFO_BUFFER_MAX_LEN);   
		#if (__NEED_LOAD_NEW_COFIG__)
		GH30x_Load_new_config(reg_config_array);
		#endif
		GH30x_adt_wear_detect_start();
	 }
	 else
	 {
		/* if using gsensor fifo mode, shouold get data by fifo int 
		 * E.g gsensor_drv_get_fifo_data(gsensor_soft_fifo_buffer, &gsensor_soft_fifo_buffer_index, GSENSOR_SOFT_FIFO_BUFFER_MAX_LEN);   
		*/
	 }
}


/// int 
void hal_gh30x_int_init(void)
{
	// code implement by user
}

void hal_gsensor_int1_init(void)
{
	// code implement by user
	
	/* if using gsensor fifo mode,
	and gsensor fifo depth is not enough to store 1 second data,
	please connect gsensor fifo interrupt to the host,
	or if using gsensor motion detect mode(e.g  motion interrupt response by 0.5G * 5counts),
	and implement this function to receive gsensor interrupt.
	*/ 
}

/// ble 
void ble_module_send_heartrate(uint32_t heartrate) // send value via heartrate profile
{
	// code implement by user
}

void ble_module_add_rr_interval(uint16_t rr_interval_arr[], uint8_t cnt) // add value to heartrate profile
{
	// code implement by user
}

void ble_module_send_data_via_gdcs(uint8_t string[], uint8_t length) // send value via through profile
{
	// code implement by user
}


// gh30x ctrl with retry function
#if (__NEED_LOAD_NEW_COFIG__)
void GH30x_Load_new_config(uint8_t *config_ptr)
{
    uint8_t index = 0;
    for (index = 0; index < __RETRY_MAX_CNT_CONFIG__; index ++) // retry 
    {
        if (HBD_LoadNewConfig(config_ptr, 0) == HBD_RET_OK)
        {
            break;
        }
    }
}
#endif


void GH30x_adt_wear_detect_start(void)
{
    uint8_t index = 0;
    if (HBD_AdtWearDetectStart() != HBD_RET_OK)  // start
    {
        for (index = 0; index < __RETRY_MAX_CNT_CONFIG__; index ++) // retry 
        {
            if (HBD_SimpleInit(&HbdInitStruct) == HBD_RET_OK)
            {
                #if (__NEED_LOAD_NEW_COFIG__)
                if (HBD_LoadNewConfig(reg_config_array, 0) == HBD_RET_OK)
                #endif
                {
                    if (HBD_AdtWearDetectStart() == HBD_RET_OK)
                    {
                        break;
                    }
                }    
            }
        }
    }
}

#if ((__APP_MODE_CONFIG__ == __APP_MODE_HB_DET__) || (__APP_MODE_CONFIG__ == __APP_MODE_ADT_HB_DET__))
void GH30x_hb_start(void)
{
    uint8_t index = 0;
	//HBD_HbDetectClearReference();
    if (HBD_HbDetectStart() != HBD_RET_OK)  // start
    {
        for (index = 0; index < __RETRY_MAX_CNT_CONFIG__; index ++) // retry 
        {
            if (HBD_SimpleInit(&HbdInitStruct) == HBD_RET_OK)
            {
                #if (__NEED_LOAD_NEW_COFIG__)
                if (HBD_LoadNewConfig(reg_config_array, 0) == HBD_RET_OK)
                #endif
                {
                    if (HBD_HbDetectStart() == HBD_RET_OK)
                    {
                        break;
                    }
                }  
            }
        }
    }
}

#if (__APP_MODE_CONFIG__ == __APP_MODE_ADT_HB_DET__)
void GH30x_wear_confirm_start(void)
{
    uint8_t index = 0;
    if (HBD_WearStateConfirmStart() != HBD_RET_OK)  // start
    {
        for (index = 0; index < __RETRY_MAX_CNT_CONFIG__; index ++) // retry 
        {
            if (HBD_SimpleInit(&HbdInitStruct) == HBD_RET_OK)
            {
                #if (__NEED_LOAD_NEW_COFIG__)
                if (HBD_LoadNewConfig(reg_config_array_wear_confirm, 0) == HBD_RET_OK)
                #endif
                {
                    if (HBD_WearStateConfirmStart() == HBD_RET_OK)
                    {
                        break;
                    }
                }   
            }
        }
    }
}
#endif
#endif

#if (__APP_MODE_CONFIG__ == __APP_MODE_HRV_DET__)
void GH30x_hrv_start(void)
{
    uint8_t index = 0;
    if (HBD_HrvDetectStart() != HBD_RET_OK)  // start
    {
        for (index = 0; index < __RETRY_MAX_CNT_CONFIG__; index ++) // retry 
        {
            if (HBD_SimpleInit(&HbdInitStruct) == HBD_RET_OK)
            {
                #if (__NEED_LOAD_NEW_COFIG__)
                if (HBD_LoadNewConfig(reg_config_array, 0) == HBD_RET_OK)
                #endif
                {
                    if (HBD_HrvDetectStart() == HBD_RET_OK)
                    {
                        break;
                    }
                }  
            }
        }
    }
}
#endif

#if (__APP_MODE_CONFIG__ == __APP_MODE_HSM_DET__)
void GH30x_hsm_start(void)
{
    uint8_t index = 0;
    if (HBD_HsmDetectStart() != HBD_RET_OK)  // start
    {
        for (index = 0; index < __RETRY_MAX_CNT_CONFIG__; index ++) // retry 
        {
            if (HBD_SimpleInit(&HbdInitStruct) == HBD_RET_OK)
            {
                #if (__NEED_LOAD_NEW_COFIG__)
                if (HBD_LoadNewConfig(reg_config_array, 0) == HBD_RET_OK)
                #endif
                {
                    if (HBD_HsmDetectStart() == HBD_RET_OK)
                    {
                        break;
                    }
                }   
            }
        }
    }
}
#endif 

#if (__APP_MODE_CONFIG__ == __APP_MODE_BPD_DET__)
void GH30x_bpd_start(void)
{
    uint8_t index = 0;
    if (HBD_BpdDetectStart() != HBD_RET_OK)  // start
    {
        for (index = 0; index < __RETRY_MAX_CNT_CONFIG__; index ++) // retry 
        {
            if (HBD_SimpleInit(&HbdInitStruct) == HBD_RET_OK)
            {
                #if (__NEED_LOAD_NEW_COFIG__)
                if (HBD_LoadNewConfig(reg_config_array, 0) == HBD_RET_OK)
                #endif
                {
                    if (HBD_BpdDetectStart() == HBD_RET_OK)
                    {
                        break;
                    }
                }   
            }
        }
    }
}
#endif

#if (__APP_MODE_CONFIG__ == __APP_MODE_PFA_DET__)
void GH30x_pfa_start(void)
{
    uint8_t index = 0;
    if (HBD_PfaDetectStart() != HBD_RET_OK)  // start
    {
        for (index = 0; index < __RETRY_MAX_CNT_CONFIG__; index ++) // retry 
        {
            if (HBD_SimpleInit(&HbdInitStruct) == HBD_RET_OK)
            {
                #if (__NEED_LOAD_NEW_COFIG__)
                if (HBD_LoadNewConfig(reg_config_array, 0) == HBD_RET_OK)
                #endif
                {
                    if (HBD_PfaDetectStart() == HBD_RET_OK)
                    {
                        break;
                    }
                }  
            }
        }
    }
}
#endif

/* gh30x module init, include gsensor init. */
int GH30x_module_init(void)
{ 
    #if (!__HB_MODE_ENABLE_FIFO__)
    HbdInitStruct.stHbInitConfig.emHbModeFifoEnable = HBD_FUNCTIONAL_STATE_DISABLE; //if GH30x fifo disable 
    HbdInitStruct.stHbInitConfig.emHrvModeFifoEnable = HBD_FUNCTIONAL_STATE_DISABLE; //if GH30x fifo disable 
    HbdInitStruct.stHbInitConfig.emHsmModeFifoEnable = HBD_FUNCTIONAL_STATE_DISABLE; //if GH30x fifo disable 
    HbdInitStruct.stHbInitConfig.emBpdModeFifoEnable = HBD_FUNCTIONAL_STATE_DISABLE; //if GH30x fifo disable 
    HbdInitStruct.stHbInitConfig.emPfaModeFifoEnable = HBD_FUNCTIONAL_STATE_DISABLE; //if GH30x fifo disable 
    #endif

	HbdInitStruct.stAdtInitConfig.emLogicChannel0PDSelect = HBD_LED_PD_SEL_EXTERNAL;
	HbdInitStruct.stAdtInitConfig.emLogicChannel1PDSelect = HBD_LED_PD_SEL_EXTERNAL;
	HbdInitStruct.stAdtInitConfig.uchLogicChannel0Current = 0x28;
	HbdInitStruct.stAdtInitConfig.uchLogicChannel1Current = 0x28;	
	HbdInitStruct.stAdtInitConfig.emLogicChannel0TiaGain = HBD_TIA_GAIN_2;
    HbdInitStruct.stAdtInitConfig.emLogicChannel1TiaGain = HBD_TIA_GAIN_2;

    /*1. user i2c init. */
    hal_gh30x_i2c_init();

    /*2. register i2c RW. hal_gh30x_i2c_write and  hal_gh30x_i2c_read must be defined by user. */
    HBD_SetI2cRW(HBD_I2C_ID_SEL_1L0L, hal_gh30x_i2c_write, hal_gh30x_i2c_read);

	/*3. Init GH30x. */
    if (HBD_RET_OK != HBD_SimpleInit(&HbdInitStruct))  
	{
    	return 0;
	}
	
	/*4. gsensor init. gsensor_drv_init must be defined by user, and >40Hz. */
	gsensor_drv_init(); 
	
	/*5.1 setup EX INT for G-sensor INT pin. hal_gsensor_int1_init must be defined by user. */
    hal_gsensor_int1_init();
    /*5.2 setup EX INT for GH30x INT pin. hal_gh30x_int_init must be defined by user. */
    hal_gh30x_int_init(); 

    #if (__GH30X_IRQ_PLUSE_WIDTH_CONFIG__)
    /*5.3 (optional) GH30x INT pin irq pluse width. */
    HBD_SetIrqPluseWidth(255); // set Irq pluse width (255us)
    #endif

#if (__NEED_LOAD_NEW_COFIG__)
    /*6 (optional) load new config for GH30x, config array generate by GOODIX APP. */
	GH30x_Load_new_config(reg_config_array);
#endif

    #if (__USE_GOODIX_APP__)
    /*7. setup Ble(or other.) send data function. */
    ble_comm_type = HBD_SetSendDataFunc(ble_module_send_data_via_gdcs);
    #endif
    
#if (__ENABLE_WEARING__)
    /*8. (optional)enable wearing function, only for GH300. */
    HBD_EnableWearing(wearing_config_array);
#endif

	/*9. config Hb algorithm scenario. */
    HBD_HbAlgoScenarioConfig(__HBA_SCENARIO_CONFIG__);
	
    /*10. (optional)start GH30x. */
    GH30x_adt_wear_detect_start();
    return 1;  
}

/* GH30x Int msg handler .*/
void GH30x_Int_Msg_Handler(void)
{
	uint8_t uchChipIntStatus = HBD_GetIntStatus();
	uint8_t gh30x_adt_working_flag = HBD_IsAdtWearDetectHasStarted();

    if (uchChipIntStatus == INT_STATUS_FIFO_WATERMARK)
    {
        #if (__APP_MODE_CONFIG__ == __APP_MODE_ADT_HB_DET__)
        #if (__ENABLE_WEARING__) 
		static uint8_t hb_value_notify = 0;
        #endif
        uint8_t hb_value = 0;
        uint8_t wearing_state = 0;
        uint8_t wearing_quality = 0;
        uint8_t voice_broadcast = 0;
        uint16_t rr_value = 0;

        /*1.get gsensor fifo data, fix fifo. gsensor_drv_get_fifo_data must must be defined by user.*/
        gsensor_drv_get_fifo_data(gsensor_soft_fifo_buffer, &gsensor_soft_fifo_buffer_index, GSENSOR_SOFT_FIFO_BUFFER_MAX_LEN);

		#if (__ENABLE_WEARING__)
        if (last_wearing_state == 1)
        {
        #endif
			/*2. calc with gsensor fifo data. */
        	HBD_HbCalculateByFifoInt(gsensor_soft_fifo_buffer, gsensor_soft_fifo_buffer_index, __GS_SENSITIVITY_CONFIG__, &hb_value, &wearing_state, &wearing_quality, &voice_broadcast, &rr_value);
   
			#if (__ENABLE_WEARING__) 
            switch (wearing_state)
            {
                case 2:
					#if (__START_ADT_AFTER_GSENSOR_MOTION__)
                    gsensor_drv_enter_normal_mode();
                    HBD_Stop();
					#else
					GH30x_Load_new_config(reg_config_array);
                    GH30x_adt_wear_detect_start();
					#endif
                    hb_value_notify = 0;
                    gsensor_drv_enter_motion_det_mode();
                    break;

                case 3:
                    #if (__NEED_LOAD_NEW_COFIG__)
                    GH30x_Load_new_config(reg_config_array_wear_confirm);
                    #endif
                    GH30x_wear_confirm_start();
            
                default:
                    hb_value_notify = hb_value;
                    ble_module_send_heartrate(hb_value_notify);
                    break;
            }
            if (wearing_state != 0)
            {
                last_wearing_state = wearing_state;
            }
            #else
            ble_module_send_heartrate(hb_value);
            #endif
        #if (__ENABLE_WEARING__)
        }
        else if (last_wearing_state == 3)
        {
            wearing_state = HBD_WearStateConfirm();
            switch (wearing_state)
            {
                case 2:
					#if (__START_ADT_AFTER_GSENSOR_MOTION__)
                    gsensor_drv_enter_normal_mode();
                    HBD_Stop();
					#else
					GH30x_Load_new_config(reg_config_array);
                    GH30x_adt_wear_detect_start();
					#endif
                    hb_value_notify = 0;
                    gsensor_drv_enter_motion_det_mode();
                    break;

                case 1:
                    #if (__NEED_LOAD_NEW_COFIG__)
                    GH30x_Load_new_config(reg_config_array);
                    #endif
                    GH30x_hb_start();
            
                default:
                    ble_module_send_heartrate(hb_value_notify);
                    break;
            }
            if (wearing_state != 0)
            {
                last_wearing_state = wearing_state;
            }            
        }
        else
        {
            last_wearing_state = 1;
        }
        #endif
		/*3. clear gsensor fifo. */
        gsensor_drv_clear_buffer(gsensor_soft_fifo_buffer, &gsensor_soft_fifo_buffer_index, GSENSOR_SOFT_FIFO_BUFFER_MAX_LEN);

        #elif (__APP_MODE_CONFIG__ == __APP_MODE_HRV_DET__)
			
        uint16_t hrv_rr_value_array[10];
        uint8_t hrv_rr_value_fresh_cnt;
        hrv_rr_value_fresh_cnt = HBD_HrvCalculateByFifoInt(NULL, 0, __GS_SENSITIVITY_CONFIG__, hrv_rr_value_array);
        ble_module_add_rr_interval(hrv_rr_value_array, hrv_rr_value_fresh_cnt);
		ble_module_send_heartrate(0); 
		
		#elif (__APP_MODE_CONFIG__ == __APP_MODE_HSM_DET__)
        uint8_t hb_value = 0;
        uint8_t sleep_state = 0;
        uint8_t respiratory_rate = 0;
        
        /*1.get gsensor fifo data, fix fifo. gsensor_drv_get_fifo_data must must be defined by user.*/
        gsensor_drv_get_fifo_data(gsensor_soft_fifo_buffer, &gsensor_soft_fifo_buffer_index, GSENSOR_SOFT_FIFO_BUFFER_MAX_LEN);

        /*2. calc with gsensor fifo data. */
        HBD_HsmCalculateByFifoInt(gsensor_soft_fifo_buffer, gsensor_soft_fifo_buffer_index, __GS_SENSITIVITY_CONFIG__, &sleep_state, &hb_value, &respiratory_rate);
        
        /*3. clear gsensor fifo. */
        gsensor_drv_clear_buffer(gsensor_soft_fifo_buffer, &gsensor_soft_fifo_buffer_index, GSENSOR_SOFT_FIFO_BUFFER_MAX_LEN);
		
        #elif (__APP_MODE_CONFIG__ == __APP_MODE_BPD_DET__)
        uint16_t sbp_value = 0;
        uint16_t dbp_value = 0;
        
        /*1.get gsensor fifo data, fix fifo. gsensor_drv_get_fifo_data must must be defined by user.*/
        gsensor_drv_get_fifo_data(gsensor_soft_fifo_buffer, &gsensor_soft_fifo_buffer_index, GSENSOR_SOFT_FIFO_BUFFER_MAX_LEN);

        /*2. calc with gsensor fifo data. */
        HBD_BpdCalculateByFifoInt(gsensor_soft_fifo_buffer, gsensor_soft_fifo_buffer_index, __GS_SENSITIVITY_CONFIG__, &sbp_value, &dbp_value);
        
        /*3. clear gsensor fifo. */
        gsensor_drv_clear_buffer(gsensor_soft_fifo_buffer, &gsensor_soft_fifo_buffer_index, GSENSOR_SOFT_FIFO_BUFFER_MAX_LEN);

        #elif (__APP_MODE_CONFIG__ == __APP_MODE_PFA_DET__)
        uint8_t pressure_level = 0;
        uint8_t fatigue_level = 0;
        uint8_t body_age = 0;

        /*1.get gsensor fifo data, fix fifo. gsensor_drv_get_fifo_data must must be defined by user.*/
        gsensor_drv_get_fifo_data(gsensor_soft_fifo_buffer, &gsensor_soft_fifo_buffer_index, GSENSOR_SOFT_FIFO_BUFFER_MAX_LEN);

        /*2. calc with gsensor fifo data. */
        HBD_PfaCalculateByFifoInt(gsensor_soft_fifo_buffer, gsensor_soft_fifo_buffer_index, __GS_SENSITIVITY_CONFIG__, &pressure_level, &fatigue_level, &body_age);
        
        /*3. clear gsensor fifo. */
        gsensor_drv_clear_buffer(gsensor_soft_fifo_buffer, &gsensor_soft_fifo_buffer_index, GSENSOR_SOFT_FIFO_BUFFER_MAX_LEN);

		#endif
    }
    else if (uchChipIntStatus == INT_STATUS_NEW_DATA)
    {      
        ST_GS_DATA_TYPE gsensor_data;
        /*1. get gsensor data. gsensor_drv_get_data must be defined by user.*/
        gsensor_drv_get_data(&gsensor_data);

        #if (__USE_GOODIX_APP__)
        if ((gh30x_status == COMM_CMD_ALGO_IN_APP_HB_START) || (gh30x_status == COMM_CMD_ALGO_IN_APP_HRV_START))
        {
            /*2. send data package. */
            HBD_SendRawdataPackageByNewdataInt(&gsensor_data, __GS_SENSITIVITY_CONFIG__);
        }
        else
        #endif
        {
			#if (__APP_MODE_CONFIG__ == __APP_MODE_ADT_HB_DET__)
            static uint8_t calc_cnt2 = 0;
            uint8_t hb_value = 0;
            uint8_t wearing_state = 0;
            uint8_t wearing_quality = 0;
            uint8_t voice_broadcast = 0;
            uint16_t rr_value = 0;
            if (1 == HBD_HbCalculateByNewdataInt(&gsensor_data, __GS_SENSITIVITY_CONFIG__, &hb_value, &wearing_state, &wearing_quality, &voice_broadcast, &rr_value))
            {
                //value have reflash.
            }
            calc_cnt2 ++;
            if ( calc_cnt2 >= 25) // 25 Hz 
            {
                ble_module_add_rr_interval(&rr_value, 1);
                ble_module_send_heartrate(hb_value); // send data via ble heartrate profile
                calc_cnt2 = 0;
            }
			#elif (__APP_MODE_CONFIG__ == __APP_MODE_HRV_DET__)
	        static uint16_t adc_send_index = 0;
	        uint16_t hrv_rr_value_array[10] = {0};
	        uint8_t hrv_rr_value_fresh_cnt;
	        hrv_rr_value_fresh_cnt = HBD_HrvCalculateByNewdataInt(NULL, __GS_SENSITIVITY_CONFIG__, hrv_rr_value_array);
	        ble_module_add_rr_interval(hrv_rr_value_array, hrv_rr_value_fresh_cnt);
	        adc_send_index ++;
	        if (adc_send_index >= 100)
	        {
	            ble_module_send_heartrate(60); 
	            adc_send_index = 0;
	        }

			#elif (__APP_MODE_CONFIG__ == __APP_MODE_HSM_DET__)
            uint8_t hb_value = 0;
            uint8_t sleep_state = 0;
            uint8_t respiratory_rate = 0;
            if (1 == HBD_HsmCalculateByNewdataInt(&gsensor_data, __GS_SENSITIVITY_CONFIG__, &sleep_state, &hb_value, &respiratory_rate))
            {
                //value have reflash.
            }

            #elif (__APP_MODE_CONFIG__ == __APP_MODE_BPD_DET__)
            uint16_t sbp_value = 0;
            uint16_t dbp_value = 0;
            
            if (1 == HBD_BpdCalculateByNewdataInt(&gsensor_data, __GS_SENSITIVITY_CONFIG__, &sbp_value, &dbp_value))
            {
                //value have reflash.
            }

            #elif (__APP_MODE_CONFIG__ == __APP_MODE_PFA_DET__)
            uint8_t pressure_level = 0;
            uint8_t fatigue_level = 0;
            uint8_t body_age = 0;

            if (1 == HBD_PfaCalculateByNewdataInt(&gsensor_data, __GS_SENSITIVITY_CONFIG__, &pressure_level, &fatigue_level, &body_age))
            {
                //value have reflash.
            }
			#endif
        }	
    }
    else if (uchChipIntStatus == INT_STATUS_WEAR_DETECTED)
    {
        gsensor_drv_enter_normal_mode();	
		#if (__APP_MODE_CONFIG__ == __APP_MODE_ADT_HB_DET__)
		
        #if (__ENABLE_WEARING__)
        last_wearing_state = 1;
        #endif

		#if (__HB_MODE_ENABLE_FIFO__)  
		gsensor_drv_clear_buffer(gsensor_soft_fifo_buffer, &gsensor_soft_fifo_buffer_index, GSENSOR_SOFT_FIFO_BUFFER_MAX_LEN);
		gsensor_drv_enter_fifo_mode(); // if GH30x fifo enable     
		#endif
		
        GH30x_hb_start();      
		#elif (__APP_MODE_CONFIG__ == __APP_MODE_HRV_DET__)
		
        GH30x_hrv_start();    
		
		#elif (__APP_MODE_CONFIG__ == __APP_MODE_HSM_DET__)

        #if (__HB_MODE_ENABLE_FIFO__)  
        gsensor_drv_clear_buffer(gsensor_soft_fifo_buffer, &gsensor_soft_fifo_buffer_index, GSENSOR_SOFT_FIFO_BUFFER_MAX_LEN);
        gsensor_drv_enter_fifo_mode(); // if GH30x fifo enable     
        #endif
        GH30x_hsm_start();   
		
        #elif (__APP_MODE_CONFIG__ == __APP_MODE_BPD_DET__)

        #if (__HB_MODE_ENABLE_FIFO__)  
        gsensor_drv_clear_buffer(gsensor_soft_fifo_buffer, &gsensor_soft_fifo_buffer_index, GSENSOR_SOFT_FIFO_BUFFER_MAX_LEN);
        gsensor_drv_enter_fifo_mode(); // if GH30x fifo enable     
        #endif
        GH30x_bpd_start();      

        #elif (__APP_MODE_CONFIG__ == __APP_MODE_PFA_DET__)

        #if (__HB_MODE_ENABLE_FIFO__)  
        gsensor_drv_clear_buffer(gsensor_soft_fifo_buffer, &gsensor_soft_fifo_buffer_index, GSENSOR_SOFT_FIFO_BUFFER_MAX_LEN);
        gsensor_drv_enter_fifo_mode(); // if GH30x fifo enable     
        #endif
        GH30x_pfa_start();  

        #endif     
    }
    else if (uchChipIntStatus == INT_STATUS_UNWEAR_DETECTED)
    {
		#if ((__NEED_LOAD_NEW_COFIG__) && (__APP_MODE_CONFIG__ == __APP_MODE_ADT_HB_DET__))

		gsensor_drv_enter_normal_mode();  
		gsensor_drv_clear_buffer(gsensor_soft_fifo_buffer, &gsensor_soft_fifo_buffer_index, GSENSOR_SOFT_FIFO_BUFFER_MAX_LEN);
		#if (__ENABLE_WEARING__)
        last_wearing_state = 2;
        #endif
		#if (__START_ADT_AFTER_GSENSOR_MOTION__)
		gsensor_drv_enter_motion_det_mode();
		#else
		GH30x_Load_new_config(reg_config_array);
		GH30x_adt_wear_detect_start();
		#endif 

		#else

        gsensor_drv_enter_normal_mode();  
        gsensor_drv_clear_buffer(gsensor_soft_fifo_buffer, &gsensor_soft_fifo_buffer_index, GSENSOR_SOFT_FIFO_BUFFER_MAX_LEN);
        GH30x_Load_new_config(reg_config_array);
        GH30x_adt_wear_detect_start();
		#endif
    }
    else if (uchChipIntStatus == INT_STATUS_CHIP_RESET) // if gh30x reset, need reinit
    {
		GS8 reinit_ret = HBD_RET_OK;
		GU8 reinit_cnt = 5;
        gsensor_drv_enter_normal_mode();
        gsensor_drv_clear_buffer(gsensor_soft_fifo_buffer, &gsensor_soft_fifo_buffer_index, GSENSOR_SOFT_FIFO_BUFFER_MAX_LEN);
		// reinit
		do 
		{
			reinit_ret = HBD_SimpleInit(&HbdInitStruct);
			reinit_cnt --;
		}while (reinit_ret != HBD_RET_OK);
        if (reinit_ret == HBD_RET_OK)
        {		
            #if (__NEED_LOAD_NEW_COFIG__)
            GH30x_Load_new_config(reg_config_array);
            #endif
			GH30x_adt_wear_detect_start();
        }
    }
	else if (uchChipIntStatus == INT_STATUS_FIFO_FULL) // if gh30x fifo full, need restart
    {
        #if (__APP_MODE_CONFIG__ == __APP_MODE_ADT_HB_DET__)
		HBD_Stop();
        gsensor_drv_enter_normal_mode();
        #if (__HB_MODE_ENABLE_FIFO__)   
        gsensor_drv_clear_buffer(gsensor_soft_fifo_buffer, &gsensor_soft_fifo_buffer_index, GSENSOR_SOFT_FIFO_BUFFER_MAX_LEN);
        gsensor_drv_enter_fifo_mode(); // if GH30x fifo enable 
        #endif
        GH30x_hb_start();      
        
        #elif (__APP_MODE_CONFIG__ == __APP_MODE_HRV_DET__)

        gsensor_drv_enter_normal_mode();
        #if (__HB_MODE_ENABLE_FIFO__)   
        gsensor_drv_clear_buffer(gsensor_soft_fifo_buffer, &gsensor_soft_fifo_buffer_index, GSENSOR_SOFT_FIFO_BUFFER_MAX_LEN);
        gsensor_drv_enter_fifo_mode(); // if GH30x fifo enable 
        #endif
        GH30x_hrv_start();    
        
		#elif (__APP_MODE_CONFIG__ == __APP_MODE_HSM_DET__)
        #if (__HB_MODE_ENABLE_FIFO__)  
        gsensor_drv_clear_buffer(gsensor_soft_fifo_buffer, &gsensor_soft_fifo_buffer_index, GSENSOR_SOFT_FIFO_BUFFER_MAX_LEN);
        gsensor_drv_enter_fifo_mode(); // if GH30x fifo enable     
        #endif
        GH30x_hsm_start(); 
		
        #elif (__APP_MODE_CONFIG__ == __APP_MODE_BPD_DET__)

        #if (__HB_MODE_ENABLE_FIFO__)  
        gsensor_drv_clear_buffer(gsensor_soft_fifo_buffer, &gsensor_soft_fifo_buffer_index, GSENSOR_SOFT_FIFO_BUFFER_MAX_LEN);
        gsensor_drv_enter_fifo_mode(); // if GH30x fifo enable     
        #endif
        GH30x_bpd_start();      

        #elif (__APP_MODE_CONFIG__ == __APP_MODE_PFA_DET__)

        #if (__HB_MODE_ENABLE_FIFO__)  
        gsensor_drv_clear_buffer(gsensor_soft_fifo_buffer, &gsensor_soft_fifo_buffer_index, GSENSOR_SOFT_FIFO_BUFFER_MAX_LEN);
        gsensor_drv_enter_fifo_mode(); // if GH30x fifo enable     
        #endif
        GH30x_pfa_start();    
           
        #endif
    }
	
	if ((gh30x_adt_working_flag == 1) && (uchChipIntStatus != INT_STATUS_WEAR_DETECTED) && (uchChipIntStatus != INT_STATUS_UNWEAR_DETECTED))
	{
		GH30x_adt_wear_detect_start();
	}
}

#if (__USE_GOODIX_APP__)
/* ble rx data handler. */
void app_parse(uint8_t *buffer, uint8_t length) 
{
    /*1. parse data. */
    EM_COMM_CMD_TYPE eCommCmdType  = HBD_CommParseHandler(ble_comm_type, buffer, length);
    if (eCommCmdType < COMM_CMD_INVALID)
    {
        if (eCommCmdType == COMM_CMD_ALGO_IN_MCU_HB_START) 
        {
            gsensor_drv_enter_normal_mode();
            #if (__HB_MODE_ENABLE_FIFO__)
            gsensor_drv_clear_buffer(gsensor_soft_fifo_buffer, &gsensor_soft_fifo_buffer_index, GSENSOR_SOFT_FIFO_BUFFER_MAX_LEN);
            gsensor_drv_enter_fifo_mode();
            #endif
			#if (__APP_MODE_CONFIG__ == __APP_MODE_ADT_HB_DET__)
            HBD_HbDetectStart();
			#endif
        }
        else if (eCommCmdType == COMM_CMD_ALGO_IN_APP_HB_START)
        {
            gsensor_drv_enter_normal_mode();
            gsensor_drv_clear_buffer(gsensor_soft_fifo_buffer, &gsensor_soft_fifo_buffer_index, GSENSOR_SOFT_FIFO_BUFFER_MAX_LEN);
            #if (__HB_MODE_ENABLE_FIFO__)  
            HBD_FifoConfig(1, HBD_FUNCTIONAL_STATE_DISABLE);
            #endif
			#if (__APP_MODE_CONFIG__ == __APP_MODE_ADT_HB_DET__)
            HBD_HbDetectStart();
			#endif
            #if (__HB_MODE_ENABLE_FIFO__)  
            HBD_FifoConfig(1, HBD_FUNCTIONAL_STATE_ENABLE);
            #endif
        }
        else if (eCommCmdType == COMM_CMD_ALGO_IN_MCU_HRV_START)
        {
            gsensor_drv_enter_normal_mode();
            #if (__HB_MODE_ENABLE_FIFO__)
            gsensor_drv_clear_buffer(gsensor_soft_fifo_buffer, &gsensor_soft_fifo_buffer_index, GSENSOR_SOFT_FIFO_BUFFER_MAX_LEN);
            gsensor_drv_enter_fifo_mode();
            #endif
			#if (__APP_MODE_CONFIG__ == __APP_MODE_HRV_DET__)
            HBD_HrvDetectStart();
			#endif
        }
        else if (eCommCmdType == COMM_CMD_ALGO_IN_APP_HRV_START)
        {
            gsensor_drv_enter_normal_mode();
            gsensor_drv_clear_buffer(gsensor_soft_fifo_buffer, &gsensor_soft_fifo_buffer_index, GSENSOR_SOFT_FIFO_BUFFER_MAX_LEN);
            #if (__HB_MODE_ENABLE_FIFO__)  
            HBD_FifoConfig(0, HBD_FUNCTIONAL_STATE_DISABLE);
            #endif
			#if (__APP_MODE_CONFIG__ == __APP_MODE_HRV_DET__)
            HBD_HrvDetectStart();
			#endif
            #if (__HB_MODE_ENABLE_FIFO__)  
            HBD_FifoConfig(0, HBD_FUNCTIONAL_STATE_ENABLE);
            #endif
        }
        else if (eCommCmdType == COMM_CMD_ADT_SINGLE_MODE_START)
        {
            gsensor_drv_enter_normal_mode();
            gsensor_drv_clear_buffer(gsensor_soft_fifo_buffer, &gsensor_soft_fifo_buffer_index, GSENSOR_SOFT_FIFO_BUFFER_MAX_LEN);
            HBD_AdtWearDetectStart();
        }
        else //stop
        {
            gsensor_drv_enter_normal_mode();
            #if (__HB_MODE_ENABLE_FIFO__)
            gsensor_drv_clear_buffer(gsensor_soft_fifo_buffer, &gsensor_soft_fifo_buffer_index, GSENSOR_SOFT_FIFO_BUFFER_MAX_LEN);
            #endif
            HBD_Stop();
        }
        gh30x_status = eCommCmdType;
    }
}
#endif

/********END OF FILE********* (C) COPYRIGHT 2018 .********/
