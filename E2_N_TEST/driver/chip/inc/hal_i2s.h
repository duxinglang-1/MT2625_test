/* Copyright Statement:
 *
 * (C) 2005-2016  MediaTek Inc. All rights reserved.
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. ("MediaTek") and/or its licensors.
 * Without the prior written permission of MediaTek and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 * You may only use, reproduce, modify, or distribute (as applicable) MediaTek Software
 * if you have agreed to and been bound by the applicable license agreement with
 * MediaTek ("License Agreement") and been granted explicit permission to do so within
 * the License Agreement ("Permitted User").  If you are not a Permitted User,
 * please cease any access or use of MediaTek Software immediately.
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT MEDIATEK SOFTWARE RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES
 * ARE PROVIDED TO RECEIVER ON AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 */

#ifndef __HAL_I2S_H__
#define __HAL_I2S_H__

#include "hal_platform.h"

#ifdef HAL_I2S_MODULE_ENABLED

/**
 * @addtogroup HAL
 * @{
 * @addtogroup I2S
 * @{
 * This section introduces the I2S APIs including terms and acronyms,
 * supported features, software architecture, details on how to use this driver, I2S function groups, enums, structures and functions.
 *
 * @section HAL_I2S_Terms_Chapter Terms and acronyms
 *
 * |Terms               |Details                                                                                                                                                 |
 * |--------------------|--------------------------------------------------------------------------------------------------------------------------------------------------------|
 * | \b I2S             | Inter-IC Sound is a standard electrical serial bus interface used for connecting digital audio devices.
 * | \b TDM             | Time division multiplexed (TDM) formats are used when more than two channels of data are to be transferred on a single data line.|
 * | \b VFIFO \b DMA    | A virtual first in first out (VFIFO) direct memory access (DMA) device applied in an electronic device having a processor, a I2S unit and a VFIFO unit is provided. In the VFIFO DMA device, a DMA unit is for transferring data between the I2S unit and the VFIFO unit. A VFIFO controller, which has a read pointer and a write pointer, is electrically connected with the DMA unit. When the DMA unit reads data from or writes data into the VFIFO unit, the VFIFO controller correspondingly changes the value of the read pointer or the write pointer. A virtual port is electrically connected to the DMA unit and the processor. A processor reads data from or writes data into the VFIFO unit via the virtual port and the DMA unit. |
 * | \b ISR             | Interrupt service routine is a software routine that is executed in response to an interrupt.|
 * | \b NVIC            | Nested Vectored Interrupt Controller is the interrupt controller of ARM Cortex-M. For more details, please refer to <a href="http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.100166_0001_00_en/ric1417175922867.html"> NVIC introduction in ARM Cortex-M4 Processor Technical Reference Manual</a>.|
 * | \b TX/RX           | Transmitter/Receiver. |
 * | \b PINMUX          | Pin Multiplexer provides a method of configuring the individual pin peripheral multiplexers to select alternate pin functions. |
 *
 * @section HAL_I2S_Features_Chapter Supported features
 * - \b Note. \n
 *   User must configure the I2S and use this driver based on the hardware platform design. For more details about the I2S functions, please refer to platform specification document.
 *
 * - \b Master \b and \b slave \b mode. \n
 *   Based on the diversities of audio applications and hardware capabilities of external codec components, the I2S operates in different modes.
 *   In master mode, bit clock and word select lines are driven by the I2S master, while all other devices derive their internal clocks from this reference. On the contrary,
 *   bit clock and word select are derived from the external devices when slave mode is configured. Except for the clock source, the I2S in master and slave mode also provide different
 *   capabilities in sample rate, channel number and applications.
 *
 * - \b Support \b callback \b function \b registration. \n
 *   An interrupt will be triggered when the data is ready to be transferred using the I2S interface. A callback function must be registered and called to handle data transfer in the I2S interrupt service routine. \n
 *
 * @section HAL_I2S_Architecture_Chapter Software architecture of I2S
 *   - For transmit operation, call #hal_i2s_tx_write() function to transmit data.
 *      The user must query the free space in the TX buffer to ensure there is enough free space in the TX buffer for the write operation.
 *      The DMA interrupt will be triggered if the remaining data length in the TX buffer is less than the threshold.
 *      Then the DMA interrupt handler will call user's callback to notify that the TX buffer is available.
 *      User can write data to the TX buffer again by calling #hal_i2s_tx_write() function.
 *   - For receive operation, #hal_i2s_rx_read() function is used.
 *      The user must check the RX buffer and fetch data from the RX buffer until it's empty.
 *      The DMA interrupt will be triggered if the data length in the RX buffer is larger than the threshold.
 *      Then the DMA interrupt handler calls the user's callback to notify that the data is available.
 *      User can fetch the data from the RX buffer again by calling #hal_i2s_rx_read() function.
 *
 *
 * @section HAL_I2S_Driver_Usage_Chapter How to use this driver
 *
 * - Configure the GPIO pins used by the I2S. \n
 *  - Set the GPIO pins for Bit clock line(BCK), word select line(WS), master clock(MCLK), data in line(DI) and data out line(DO) to support the I2S, based on the user`s hardware platform design. For more details about the PINMUX setting of the I2S functions, please refer to @ref GPIO driver document.
 *
 * - Use I2S transmit/receive (TX/RX) both on for external codec functionality. \n
 *  - Step1:   Call #hal_i2s_init() to configure the I2S type.
 *  - Step2:   Call #hal_i2s_set_config() to configure the I2S settings.
 *  - Step3:   Call #hal_i2s_set_tx_dma() and #hal_i2s_set_rx_dma() to set the memory for the I2S TX and RX DMA channel use.
 *  - Step4:   Call #hal_i2s_register_tx_callback() and #hal_i2s_register_rx_callback() to register the user-defined callback functions.
 *  - Step5:   Call #hal_i2s_enable_tx_interrupt() and #hal_i2s_enable_rx_interrupt() to enable DMA interrupt.
 *  - Step6:   Call #hal_i2s_enable_clock(),#hal_i2s_enable_tx() and #hal_i2s_enable_rx() functions to start TX and RX link data flow.
 *  - Sample code:
 *    @code
 *
 *       // Allocate memory for DMA
 *      #define TX_BUFFER_LENGTH   256
 *      #define RX_BUFFER_LENGTH   256
 *
 *      ATTR_ZIDATA_IN_NONCACHED_RAM_4BYTE_ALIGN uint32_t tx_buffer[TX_BUFFER_LENGTH];
 *      ATTR_ZIDATA_IN_NONCACHED_RAM_4BYTE_ALIGN uint32_t rx_buffer[RX_BUFFER_LENGTH];
 *
 *      static void user_i2s_tx_callback(hal_i2s_event_t event, void *user_data)
 *      {
 *          //Always send I2S TX events to user task
 *          //audio_procedure_call_in_media_task(src_module_id, user_i2s_tx_event_handler, event, user_data);
 *      }
 *
 *      static void user_i2s_rx_callback(hal_i2s_event_t event, void *user_data)
 *      {
 *          //Always send I2S RX events to user task
 *          //audio_procedure_call_in_media_task(src_module_id, user_i2s_rx_event_handler, event, user_data);
 *      }
 *
 *      static void i2s_open(void)
 *      {
 *
 *          hal_i2s_config_t i2s_config;
 *          hal_i2s_dma_config_t tx_dma_config, rx_dma_config;
 *          hal_i2s_status_t result = HAL_I2S_STATUS_OK;
 *
 *          memset(&i2s_config, 0, sizeof(hal_i2s_config_t));
 *          memset(&dma_config, 0, sizeof(hal_i2s_dma_config_t));
 *          memset(&rx_dma_config, 0, sizeof(hal_i2s_dma_config_t));
 *
 *          // Set the I2S to an external mode
 *          result = hal_i2s_init(HAL_I2S_TYPE_EXTERNAL_MODE);
 *          if(HAL_I2S_STATUS_OK != result) {
 *             // error handle
 *          }
 *
 *          //Configure the DMA of the I2S
 *          tx_dma_config.buffer = tx_buffer;
 *          tx_dma_config.threshold = TX_BUFFER_LENGTH / 2;
 *          tx_dma_config.buffer_length = TX_BUFFER_LENGTH;
 *          rx_dma_config.buffer = rx_buffer;
 *          rx_dma_config.threshold = RX_BUFFER_LENGTH / 2;
 *          rx_dma_config.buffer_length = RX_BUFFER_LENGTH;
 *
 *          //Configure the clock mode of the I2S
 *          i2s_config.clock_mode = HAL_I2S_MASTER;
 *
 *         //Configure the frame sync with of the I2S
 *          i2s_config.frame_sync_width=HAL_I2S_FRAME_SYNC_WIDTH_32;
 *
 *          //Configure the sample with of the I2S
 *          i2s_config.sample_width=HAL_I2S_SAMPLE_WIDTH_16BIT;
 *
 *          //Configure the RX settings
 *          i2s_config.i2s_in.sample_rate = HAL_I2S_SAMPLE_RATE_16K;
 *          i2s_config.i2s_in.msb_offset=0;
 *          i2s_config.i2s_in.word_select_inverse=HAL_I2S_WORD_SELECT_INVERSE_DISABLE;
 *          i2s_config.i2s_in.lr_swap=HAL_I2S_LR_SWAP_DISABLE;
 *
 *          //Configure the TX settings
 *          i2s_config.tx_mode=HAL_I2S_TX_DUPLICATE_DISABLE;
 *          i2s_config.i2s_out.channel_number = HAL_I2S_MONO;
 *          i2s_config.i2s_out.sample_rate = HAL_I2S_SAMPLE_RATE_16K;
 *          i2s_config.i2s_out.msb_offset=0;
 *          i2s_config.i2s_out.word_select_inverse=HAL_I2S_WORD_SELECT_INVERSE_DISABLE;
 *          i2s_config.i2s_out.lr_swap=HAL_I2S_LR_SWAP_DISABLE;
 *
 *          //Configure the I2S links
 *          result = hal_i2s_set_config(&i2s_config);
 *          if(HAL_I2S_STATUS_OK != result) {
 *             // error handle
 *          }
 *
 *          //Configure the DMA for TX
 *          result = hal_i2s_set_tx_dma(&tx_dma_config);
 *          if(HAL_I2S_STATUS_OK != result) {
 *               // error handle
 *          }
 *
 *          //Configure the DMA for RX
 *          result = hal_i2s_set_rx_dma(&rx_dma_config);
 *          if(HAL_I2S_STATUS_OK != result) {
 *               // error handle
 *          }
 *
 *          hal_i2s_register_tx_callback(user_i2s_tx_callback, NULL);
 *          hal_i2s_register_rx_callback(user_i2s_rx_callback, NULL);
 *
 *          hal_i2s_enable_tx();
 *          hal_i2s_enable_rx();
 *          hal_i2s_enable_clock();
 *
 *          //Enable the interrupt
 *          hal_i2s_enable_tx_interrupt();
 *          hal_i2s_enable_rx_interrupt();
 *      }
 *
 *      static void i2s_close(void)
 *      {
 *          //Disable the interrupt
 *          hal_i2s_disable_tx_interrupt();
 *          hal_i2s_disable_rx_interrupt();
 *
 *          //Disable the I2S links
 *          hal_i2s_disable_tx();
 *          hal_i2s_disable_rx();
 *          hal_i2s_disable_clock();
 *          hal_i2s_deinit();
 *
 *      }
 *    @endcode
 *
 * - Event handling for TX and RX links data transfer operations.\n
 *  - The callback functions of TX and RX links are always invoked in the NVIC ISR. It is recommended to send the events of TX and RX links to user-defined media task in callback functions and handle the corresponding data operations in media task to avoid long operations in the ISR.
 *  - An example implementation below explain how to handle events in a user-defined media task.
 *    @code
 *      static void user_i2s_tx_callback(hal_i2s_event_t event, void *user_data)
 *      {
 *          //Always send I2S TX events to user task
 *          //audio_procedure_call_in_media_task(src_module_id, user_i2s_event_handler, event, user_data);
 *      }
 *
 *      static void user_i2s_rx_callback(hal_i2s_event_t event, void *user_data)
 *      {
 *          //Always send I2S RX events to user task
 *          //audio_procedure_call_in_media_task(src_module_id, user_i2s_event_handler, event, user_data);
 *      }
 *
 *      void user_i2s_event_handler(hal_i2s_event_t event, void *user_data)
 *      {
 *          switch(event) {
 *
 *              case HAL_I2S_EVENT_DATA_REQUEST:
 *                //Write data to the TX buffer
 *                break;
 *              case HAL_I2S_EVENT_DATA_NOTIFICATION:
 *                //Read data from the RX buffer
 *                break;
 *           }
 *      }
 *
 *    @endcode
 * @section Function_Group_Chapter I2S function groups description
 *
 * The APIs are grouped by their functionality for an easier use.
 * User can refer to the details of each function by clicking the function name.
 * - \b Initialization \b and \b configuration \b functions \n
 *   The #hal_i2s_init() function configures the I2S type to be set at the beginning of an I2S operation.\n
 *   The #hal_i2s_deinit() function resets the I2S type configuration. \n
 *   The #hal_i2s_set_config() function configures the I2S settings providing the respective sampling rate, downsampling rate in RX mode, data swapping between right and left channels, word select(WS) inverting, mono duplication in TX mode, bits delay from frame sync and channel number for TX and RX links as well as the clock mode for the whole I2S. The user should configure settings before enabling the I2S functions. \n
 *   The #hal_i2s_get_config() function gets the current I2S settings. \n
 *   The #hal_i2s_set_tx_dma() function configures DMA TX channel. \n
 *   The #hal_i2s_set_rx_dma() function configures DMA RX channel. \n
 *   The functions are: \n
 *   - #hal_i2s_init()
 *   - #hal_i2s_deinit()
 *   - #hal_i2s_set_config()
 *   - #hal_i2s_get_config()
 *   - #hal_i2s_set_tx_dma()
 *   - #hal_i2s_set_rx_dma()
 *
 * - \b Callback \b register \b functions \n
 *   Use these functions to register callback functions for TX and RX link data operation. \n
 *   The functions are: \n
 *  - #hal_i2s_register_tx_callback()
 *  - #hal_i2s_register_rx_callback()
 *
 * - \b Enable \b and \b disable \b functions \n
 *   Call #hal_i2s_enable_tx() function to enable the I2S TX link to transmit the data. \n
 *   Call #hal_i2s_disable_tx() function to disable the I2S TX data transmission immediately. \n
 *   Call #hal_i2s_enable_rx() function to enable the I2S RX link to receive data. \n
 *   Call #hal_i2s_disable_rx() function to disable the I2S RX immediately. \n
 *   Call #hal_i2s_enable_clock() function to enable the I2S HW FIFO and clock immediately. \n
 *   Call #hal_i2s_disable_clock() function to disable the I2S HW FIFO and clock immediately. \n
 *   The functions are: \n
 *   - #hal_i2s_enable_tx()
 *   - #hal_i2s_disable_tx()
 *   - #hal_i2s_enable_rx()
 *   - #hal_i2s_disable_rx()
 *   - #hal_i2s_enable_clock()
 *   - #hal_i2s_disable_clock()
 *
 * - \b Data \b transfer \b functions
 *   - TX link data transfer \n
 *    Call #hal_i2s_get_tx_sample_count() function before #hal_i2s_tx_write() function to determine the free space available in the TX buffer for the write operation. \n
 *    The functions are: \n
 *    - #hal_i2s_tx_write()
 *    - #hal_i2s_get_tx_sample_count()
 *    - sample code:
 *    @code
 *     uint32_t user_buffer[data_buffer_length] = { // with data };
 *     static void i2s_write_data(void)
 *     {
 *         uint32_t sample_count = 0;
 *         uint32_t write_count = 0;
 *
 *         //Get available free space of TX buffer
 *         hal_i2s_get_tx_sample_count(&sample_count);
 *
 *          if( data_buffer_length < sample_count) {
 *              //Write numbers of samples for audio out
 *              for(write_count=0;write_count<data_buffer_length;write_count++)
 *                   hal_i2s_tx_write(user_buffer[write_count]);
 *          }
 *     }
 *    @endcode
 *   - RX link data transfer \n
 *    Call #hal_i2s_get_rx_sample_count() function before #hal_i2s_rx_read() function to determine the samples available in the RX buffer for the I2S read operation. \n
 *    The functions are: \n
 *    - #hal_i2s_rx_read()
 *    - #hal_i2s_get_rx_sample_count()
 *    - sample code:
 *    @code
 *     static void i2s_read_data(void)
 *     {
 *         uint32_t user_buffer[data_buffer_length];
 *         uint32_t sample_count = 0;
 *         uint32_t read_count = 0;
 *         uint32_t store_data = 0;
 *
 *         memset(user_buffer, 0, sizeof(user_buffer));
 *
 *         //Get available data count of RX buffer
 *         hal_i2s_get_rx_sample_count(&sample_count);
 *
 *         while(sample_count!=0) {
 *              hal_i2s_rx_read(&store_data);
 *              if(read_count<data_buffer_length){
 *                  user_buffer[read_count]=store_data;
 *                  read_count++;
 *              }
 *              else
 *                  break;
 *              hal_i2s_get_rx_sample_count(&sample_count);
 *         }
 *     }
 *    @endcode
 */


#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup hal_i2s_enum Enum
  * @{
  */


/** @brief This enum defines the I2S status. */
typedef enum {
    HAL_I2S_STATUS_INVALID_PARAMETER  = -2,  /**<  A wrong parameter is given. */
    HAL_I2S_STATUS_ERROR              = -1,  /**<  An error occurred during the function call. */
    HAL_I2S_STATUS_OK                 =  0   /**<  No error occurred during the function call. */
} hal_i2s_status_t;


/** @brief This enum defines the I2S clock mode. */
typedef enum {
    HAL_I2S_MASTER  = 0,  /**< Clock source fed to an external device. */
    HAL_I2S_SLAVE   = 1   /**< Clock source comes from an external device. */
} hal_i2s_clock_mode_t;


/** @brief This enum defines the I2S channel number. */
typedef enum {
    HAL_I2S_MONO   = 0,   /**<  A single channel.  */
    HAL_I2S_STEREO = 1    /**<  Two channels. */
} hal_i2s_channel_number_t;


/**
  * @}
  */

/** @defgroup hal_i2s_struct Struct
  * @{
  */


/** @brief I2S link structure */
typedef struct {
    hal_i2s_sample_rate_t           sample_rate;         /**< Sampling rate of the I2S link.*/
    hal_i2s_channel_number_t        channel_number;      /**< Channel number of the I2S link.*/
#ifdef HAL_I2S_EXTENDED
    uint32_t                        msb_offset;          /**< The number of bits delay between frame sync and first data bit of the I2S link. The function only work for TDM mode.\n*/
    hal_i2s_lr_swap_t               lr_swap;             /**< Data swapping between right and left channels of the I2S link by setting this value.*/
    hal_i2s_word_select_inverse_t   word_select_inverse; /**< Word select clock inverting of the I2S link by setting this value.*/
#endif
} hal_i2s_link_config_t;


/** @brief I2S configure structure */
typedef struct {
    hal_i2s_link_config_t        i2s_in;             /**< I2S input link configuration.*/
    hal_i2s_link_config_t        i2s_out;            /**< I2S output link configuration.*/
    hal_i2s_clock_mode_t         clock_mode;         /**< I2S clock source. Please refer to #hal_i2s_clock_mode_t.*/
#ifdef HAL_I2S_FEATURE_TDM
    hal_i2s_tdm_channel_t        tdm_channel;        /**< Channels per frame sync. Number of channel in each frame sync>*/
#endif
#ifdef HAL_I2S_EXTENDED
    hal_i2s_frame_sync_width_t   frame_sync_width;   /**< Width per frame sync. Number of bits in each frame sync>*/
    hal_i2s_sample_width_t       sample_width;       /**< Sample width of the I2S link.*/
    hal_i2s_tx_mode_t            tx_mode;            /**< Data mode of the I2S TX. Please refer to #hal_i2s_tx_mode_t.*/
#endif
} hal_i2s_config_t;



/**
  * @}
  */

/** @defgroup hal_i2s_typedef Typedef
  * @{
  */


/** @brief  This function defines the I2S RX callback function prototype.
 *          The user should register a callback function while turning the I2S RX link on.
 *          For more details about the callback, please refer to #hal_i2s_register_rx_callback().
 *  @param[in] event is the value defined in #hal_i2s_event_t. This parameter is given by the driver to tell the user of data flow processing behavior.
 *  @param[in] user_data is a parameter given by the user with #hal_i2s_register_rx_callback().
 */
typedef void (*hal_i2s_rx_callback_t)(hal_i2s_event_t event, void *user_data);


/** @brief  This function defines the I2S TX callback function prototype.
 *          The user should register a callback function while turning the I2S TX link on.
 *          For more details about the callback, please refer to #hal_i2s_register_tx_callback().
 *  @param[in] event is the value defined in #hal_i2s_event_t. This parameter is given by the driver to provide the data flow processing information to the user.
 *  @param[in] user_data is a parameter given by the user with #hal_i2s_register_tx_callback().
 */
typedef void (*hal_i2s_tx_callback_t)(hal_i2s_event_t event, void *user_data);


/**
  * @}
  */

/**
 * @brief     This function initializes the I2S hardware type.
 * @param[in] i2s_initial_type is the initial configuration parameter. Please refer to #hal_i2s_initial_type_t.
 * @return    #HAL_I2S_STATUS_INVALID_PARAMETER, if input parameter is invalid. \n
 *            #HAL_I2S_STATUS_ERROR, if one of I2S links is still available. Call #hal_i2s_disable_tx() and #hal_i2s_disable_rx() to disable the TX/RX links before calling this function. \n
 *            #HAL_I2S_STATUS_OK, if the operation is successful.
 * @sa        #hal_i2s_deinit()
 */
hal_i2s_status_t hal_i2s_init(hal_i2s_initial_type_t i2s_initial_type);

/**
 * @brief     This function deinitializes the I2S hardware.
 * @return    #HAL_I2S_STATUS_ERROR, if one of I2S links is still available. Call #hal_i2s_disable_tx() and #hal_i2s_disable_rx() to disable the TX/RX links before calling this function. \n
 *            #HAL_I2S_STATUS_OK, if the operation is successful.
 * @sa        #hal_i2s_init()
 */
hal_i2s_status_t hal_i2s_deinit(void);

 /**
 * @brief     This function sets the I2S configuration details.
 * @param[in] config is the link configuration of the I2S module. For more details about this parameter, please refer to #hal_i2s_config_t.
 * @return    #HAL_I2S_STATUS_INVALID_PARAMETER, if wrong parameter is given. User should check the parameter when receiving this value. \n
 *            #HAL_I2S_STATUS_ERROR, if one of the I2S links is still available. Call #hal_i2s_disable_tx() and #hal_i2s_disable_rx() to disable the TX/RX links before calling this function. \n
 *            #HAL_I2S_STATUS_OK, if the operation is successful.
 * @note      To avoid unpredictable system operation, calling #hal_i2s_set_config() during the I2S execution is not allowed. User should disable the TX/RX links before setting the configuration. \n
              Set the sampling rates of the TX and RX to the same value. Different values are not allowed.
 * @sa        #hal_i2s_get_config()
 */
hal_i2s_status_t hal_i2s_set_config(const hal_i2s_config_t *config);

/**
 * @brief      This function queries the I2S configuration details.
 * @param[out] config is the link configuration of I2S module which is set in system. For more details about this parameter, please refer to #hal_i2s_config_t.
 * @return     #HAL_I2S_STATUS_INVALID_PARAMETER,  if the input parameter is invalid. \n
 *              #HAL_I2S_STATUS_OK, if the operation is successful.
 * @sa         #hal_i2s_set_config()
 */
hal_i2s_status_t hal_i2s_get_config(hal_i2s_config_t *config);

/**
 * @brief     This function disables the I2S output link.
 * @return    #HAL_I2S_STATUS_OK, if the operation is successful. \n
              The return value is reserved for further expansion. The current value is always set to #HAL_I2S_STATUS_OK.
 * @sa        #hal_i2s_enable_tx()
 */
hal_i2s_status_t  hal_i2s_disable_tx(void);

/**
 * @brief     This function disables the I2S input link.
 * @return    #HAL_I2S_STATUS_OK, if the operation is successful. \n
              The return value is reserved for further expansion. The current value is always set to #HAL_I2S_STATUS_OK.
 * @sa        #hal_i2s_enable_rx()
 */
hal_i2s_status_t hal_i2s_disable_rx(void);

/**
 * @brief     This function enables uplink and downlink FIFOs of the I2S link.
 * @return    #HAL_I2S_STATUS_OK, if the operation is successful. \n
              The return value is reserved for further expansion. The current value is always set to #HAL_I2S_STATUS_OK.
 * @note       Call this function after #hal_i2s_enable_tx() or #hal_i2s_enable_rx().
 * @sa        #hal_i2s_disable_clock()
 */
hal_i2s_status_t  hal_i2s_enable_clock(void);

/**
 * @brief     This function disables uplink and downlink FIFOs of the I2S link.
 * @return    #HAL_I2S_STATUS_OK, if the operation is successful. \n
 *             The return value is reserved for further expansion. The current value is always set to #HAL_I2S_STATUS_OK.
 * @sa        #hal_i2s_enable_clock()
 */
hal_i2s_status_t  hal_i2s_disable_clock(void);

/**
 * @brief     This function enables the TX DMA interrupt.
 * @return   #HAL_I2S_STATUS_ERROR, if TX callback function is not registered. Call #hal_i2s_register_tx_callback() to register TX callback function before calling this function. \n
 *               #HAL_I2S_STATUS_OK, if the operation is successful.
 * @sa        #hal_i2s_disable_tx_interrupt()
 * @note   To avoid unpredictable system operation, calling this function is not allowed if TX callback function is not registered.
 */
hal_i2s_status_t  hal_i2s_enable_tx_interrupt(void);

/**
 * @brief     This function disables the TX DMA interrupt.
 * @return    #HAL_I2S_STATUS_OK, if the operation is successful. \n
              The return value is reserved for further expansion. The current value is always set to #HAL_I2S_STATUS_OK.
 * @sa        #hal_i2s_enable_tx_interrupt()
 */
hal_i2s_status_t  hal_i2s_disable_tx_interrupt(void);

/**
 * @brief     This function enables the RX DMA interrupt.
 * @return    #HAL_I2S_STATUS_ERROR, if RX callback function is not registered. Call #hal_i2s_register_rx_callback() to register RX callback function before calling this function. \n
 *               #HAL_I2S_STATUS_OK, if the operation is successful.
 * @sa        #hal_i2s_disable_rx_interrupt()
 * @note   To avoid unpredictable system operation, calling this function is not allowed if RX callback function is not registered.
 */
hal_i2s_status_t  hal_i2s_enable_rx_interrupt(void);

/**
 * @brief     This function disables the RX DMA interrupt.
 * @return    #HAL_I2S_STATUS_OK, if the operation is successful. \n
 *             The return value is reserved for further expansion. The current value is always set to #HAL_I2S_STATUS_OK.
 * @sa        #hal_i2s_enable_rx_interrupt()
 */
hal_i2s_status_t  hal_i2s_disable_rx_interrupt(void);

/**
 * @brief     This function sets up the transmit operation of the DMA TX channel.
 *            The DMA starts to pump data from the DMA TX FIFO into I2S TX if the I2S TX is enabled. \n
 *            DMA will trigger an interrupt when the amount of output data in DMA TX FIFO is lower than the threshold.
 * @param[in] dma_config is the dma configuration of the I2S module. For more details about this parameter, please refer to #hal_i2s_dma_config_t.
 * @return   #HAL_I2S_STATUS_ERROR, if one of I2S links is still available. Call #hal_i2s_disable_tx(), #hal_i2s_disable_rx() and #hal_i2s_disable_clock() to disable the TX/RX links before calling this function. \n
 *              #HAL_I2S_STATUS_INVALID_PARAMETER, if the buffer is a NULL pointer. \n
 *              #HAL_I2S_STATUS_OK, if the operation is successful.
 * @note   To avoid unpredictable system operation, calling #hal_i2s_set_tx_dma() during the I2S execution is not allowed. User should disable the I2S links before setting the configuration. \n
 *              Buffer is occupied by DMA until the transmission is complete.
 */
hal_i2s_status_t hal_i2s_set_tx_dma(const hal_i2s_dma_config_t *dma_config);

/**
 * @brief     This function sets up the receive operationof the DMA RX channel.
 *            The DMA starts to pump data from I2S RX into the DMA RX FIFO if the I2S RX is enabled. \n
 *            DMA will trigger an interrupt when the amount of available receive data in the  DMA RX FIFO is larger than the threshold.
 * @param[in] dma_config is the dma configuration of the I2S module. For more details about this parameter, please refer to #hal_i2s_dma_config_t.
 * @return  #HAL_I2S_STATUS_ERROR, if one of I2S links is still available. Call #hal_i2s_disable_tx(), #hal_i2s_disable_rx() and #hal_i2s_disable_clock() to disable the TX/RX links before calling this function. \n
 *              #HAL_I2S_STATUS_INVALID_PARAMETER, if the buffer is a NULL pointer. \n
 *              #HAL_I2S_STATUS_OK, if the operation is successful.
 * @note   To avoid unpredictable system operation, calling #hal_i2s_set_rx_dma() during the I2S execution is not allowed. User should disable the I2S links before setting the configuration. \n
 *              Buffer is occupied by DMA until the transmission is complete.
 */
hal_i2s_status_t hal_i2s_set_rx_dma(const hal_i2s_dma_config_t *dma_config);

/**
 * @brief     This function registers the callback function for input data.
 * @param[in] rx_callback is a callback function for the received data control.
 * @param[in] user_data is a user defined input data.
 * @return    #HAL_I2S_STATUS_INVALID_PARAMETER, if the rx_callback is invalid. \n
 *              #HAL_I2S_STATUS_OK, if the operation is successful.
 * @note      Send I2S RX events to a user-defined task and handle the data transfer there to avoid longer performances in the NVIC ISR.
 */
hal_i2s_status_t hal_i2s_register_rx_callback(hal_i2s_rx_callback_t rx_callback, void *user_data);

/**
 * @brief     This function registers the callback function for output data.
 * @param[in] tx_callback is a pointer to the callback function to control data transmission.
 * @param[in] user_data is a user defined input data.
 * @return    #HAL_I2S_STATUS_INVALID_PARAMETER, if the tx_callback is invalid. \n
 *              #HAL_I2S_STATUS_OK, if the operation is successful.
 * @note      Send I2S TX events to a user-defined task and handle the data transfer there to avoid longer performances in the NVIC ISR.
 */
hal_i2s_status_t hal_i2s_register_tx_callback(hal_i2s_tx_callback_t tx_callback, void *user_data);

/**
* @brief     This function enables the I2S output link.
* @return    #HAL_I2S_STATUS_OK, if the operation is successful.  \n
             The return value is reserved for further expansion. The current value is always set to #HAL_I2S_STATUS_OK.
* @sa        #hal_i2s_disable_tx()
*/
hal_i2s_status_t  hal_i2s_enable_tx(void);

 /**
 * @brief     This function enables the I2S input link.
 * @return    #HAL_I2S_STATUS_OK, if the operation is successful. \n
              The return value is reserved for further expansion. The current value is always set to #HAL_I2S_STATUS_OK.
 * @sa        #hal_i2s_disable_rx()
 */
hal_i2s_status_t  hal_i2s_enable_rx(void);

/**
 * @brief     This function transmits data to the I2S output link.
 * @param[in] data is the 32-bit output data to send. For more details about the data format for left and right channel, please refer to platform document.\n
 * @return    #HAL_I2S_STATUS_OK, if the operation is successful. \n
              The return value is reserved for further expansion. The current value is always set to #HAL_I2S_STATUS_OK.
 * @note      Call this function after #hal_i2s_get_tx_sample_count() function to ensure there is enough free space in the TX buffer for the write operation.\n
                    One sample count queried by #hal_i2s_get_tx_sample_count() will be consumed as #hal_i2s_tx_write() called once.
 * @sa        #hal_i2s_get_tx_sample_count()
 */
hal_i2s_status_t hal_i2s_tx_write(uint32_t data);


/**
 * @brief     This function receives data from the I2S input link.
 * @param[out] data is the 32-bit data to receive. For more details about the data format for left and right channel, please refer to platform document.\n
 * @return    #HAL_I2S_STATUS_OK, if the operation is successful. \n
              The return value is reserved for further expansion. The current value is always set to #HAL_I2S_STATUS_OK.
 * @note      Call this function after #hal_i2s_get_rx_sample_count() function to ensure there is data available in the RX buffer to perform read operation.
 * @sa        #hal_i2s_get_rx_sample_count()
 */
hal_i2s_status_t hal_i2s_rx_read(uint32_t *data);

/**
 * @brief      This function queries available free space in the TX buffer.
 * @param[out] sample_count is the available free space in the TX buffer.(Number of data words to write)
 * @return     #HAL_I2S_STATUS_ERROR, if the user do not configure the buffer length for TX buffer by #hal_i2s_set_tx_dma(). \n
 *                #HAL_I2S_STATUS_OK, if the operation is successful.
 * @note       Call this function before #hal_i2s_tx_write() function to ensure there is enough free space in the TX buffer for the write operation.
 * @sa         #hal_i2s_tx_write()
 */
 hal_i2s_status_t hal_i2s_get_tx_sample_count(uint32_t *sample_count);

/**
 * @brief      This function queries length of the received data available in the RX buffer.
 * @param[out] sample_count is the length of the received data available in the RX buffer.
 * @return     #HAL_I2S_STATUS_OK, if the operation is successful. \n
               The return value is reserved for further expansion. The current value is always set to #HAL_I2S_STATUS_OK.
 * @note       Call this function before #hal_i2s_rx_read() function to ensure there is data available in the RX buffer to perform read operation.
 * @sa         #hal_i2s_rx_read()
 */
hal_i2s_status_t hal_i2s_get_rx_sample_count(uint32_t *sample_count);

/**
 * @brief      This function enable EOF event notification.
 * @return     #HAL_I2S_STATUS_OK, if the operation is successful. \n
               The return value is reserved for further expansion. The current value is always set to #HAL_I2S_STATUS_OK.
 */
hal_i2s_status_t hal_i2s_set_eof(void);


#ifdef  HAL_I2S_FEATURE_MULTI_I2S
/**
 * @brief     This extended function initializes the I2S hardware type in a multiple I2S mode.
 * @param[in] i2s_port is the I2S HW number. Please refer to #hal_i2s_port_t.
 * @param[in] i2s_initial_type is the initial configuration parameter. Please refer to #hal_i2s_initial_type_t.
 * @return  #HAL_I2S_STATUS_INVALID_PARAMETER, if input parameter is invalid. \n
 *              #HAL_I2S_STATUS_ERROR, if one of I2S links is still available. Call #hal_i2s_disable_tx_ex() and #hal_i2s_disable_rx_ex() to disable the TX/RX links before calling this function. \n
 *              #HAL_I2S_STATUS_OK, if the operation is successful.
 * @note    User can use  multiple I2S HW interface simultaneously with extended APIs.
 * @sa        #hal_i2s_deinit_ex()
 */
hal_i2s_status_t hal_i2s_init_ex(hal_i2s_port_t i2s_port, hal_i2s_initial_type_t i2s_initial_type);

/**
 * @brief     This extended function deinitializes the I2S hardware in a multiple I2S mode.
 * @param[in] i2s_port is the I2S HW number. Please refer to #hal_i2s_port_t.
 * @return    #HAL_I2S_STATUS_ERROR, if one of I2S links is still available. Call #hal_i2s_disable_tx() and #hal_i2s_disable_rx() to disable the TX/RX links before calling this function. \n
 *            #HAL_I2S_STATUS_OK, if the operation is successful.
 * @note     User can use  multiple I2S HW interface simultaneously with extended APIs.
 * @sa        #hal_i2s_init_ex()
 */
hal_i2s_status_t hal_i2s_deinit_ex(hal_i2s_port_t i2s_port);

 /**
 * @brief     This extended function sets the I2S configuration details in a multiple I2S mode.
 * @param[in] i2s_port is the I2S HW number. Please refer to #hal_i2s_port_t.
 * @param[in] config is the link configuration of the I2S module. For more details about this parameter, please refer to #hal_i2s_config_t.
 * @return    #HAL_I2S_STATUS_INVALID_PARAMETER, if wrong parameter is given. User should check the parameter when receiving this value. \n
 *            #HAL_I2S_STATUS_ERROR, if one of the I2S links is still available. Call #hal_i2s_disable_tx_ex() and #hal_i2s_disable_rx_ex() to disable the TX/RX links before calling this function. \n
 *            #HAL_I2S_STATUS_OK, if the operation is successful.
 * @note      To avoid unpredictable system operation, calling #hal_i2s_set_config_ex() during the I2S execution is not allowed. User should disable the TX/RX links before setting the configuration. \n
              Set the sampling rates of the TX and RX to the same value. Different values are not allowed.
 * @sa        #hal_i2s_get_config_ex()
 */
hal_i2s_status_t hal_i2s_set_config_ex(hal_i2s_port_t i2s_port, const hal_i2s_config_t *config);

/**
 * @brief      This extended function queries the I2S configuration details in a multiple I2S mode.
 * @param[in] i2s_port is the I2S HW number. Please refer to #hal_i2s_port_t.
 * @param[out] config is the link configuration of I2S module which is set in system. For more details about this parameter, please refer to #hal_i2s_config_t.
 * @return     #HAL_I2S_STATUS_INVALID_PARAMETER,  if the input parameter is invalid. \n
 *              #HAL_I2S_STATUS_OK, if the operation is successful.
 * @sa         #hal_i2s_set_config_ex()
 */
hal_i2s_status_t hal_i2s_get_config_ex(hal_i2s_port_t i2s_port, hal_i2s_config_t *config);

/**
 * @brief     This extended function enables uplink and downlink FIFOs of the I2S link in a multiple I2S mode.
 * @param[in] i2s_port is the I2S HW number. Please refer to #hal_i2s_port_t.
 * @return    #HAL_I2S_STATUS_OK, if the operation is successful. \n
 *             The return value is reserved for further expansion. The current value is always set to #HAL_I2S_STATUS_OK.
 * @note       Call this function after #hal_i2s_enable_tx_ex() or #hal_i2s_enable_rx_ex().
 * @sa        #hal_i2s_disable_clock_ex()
 */
hal_i2s_status_t  hal_i2s_enable_clock_ex(hal_i2s_port_t i2s_port);

/**
 * @brief     This extended function disables uplink and downlink FIFOs of the I2S link in a multiple I2S mode.
 * @param[in] i2s_port is the I2S HW number. Please refer to #hal_i2s_port_t.
 * @return    #HAL_I2S_STATUS_OK, if the operation is successful. \n
 *             The return value is reserved for further expansion. The current value is always set to #HAL_I2S_STATUS_OK.
 * @sa        #hal_i2s_enable_clock_ex()
 */
hal_i2s_status_t  hal_i2s_disable_clock_ex(hal_i2s_port_t i2s_port);

/**
 * @brief     This extended function enables the TX interrupt in a multiple I2S mode.
 * @param[in] i2s_port is the I2S HW number. Please refer to #hal_i2s_port_t.
 * @return   #HAL_I2S_STATUS_ERROR, if TX callback function is not registered. Call #hal_i2s_register_tx_callback_ex() to register TX callback function before calling this function. \n
 *               #HAL_I2S_STATUS_OK, if the operation is successful.
 * @sa        #hal_i2s_disable_tx_interrupt_ex()
 * @note   To avoid unpredictable system operation, calling this function is not allowed if TX callback function is not registered.
 */
hal_i2s_status_t  hal_i2s_enable_tx_interrupt_ex(hal_i2s_port_t i2s_port);

/**
 * @brief     This extended function disables the TX interrupt in a multiple I2S mode.
 * @param[in] i2s_port is the I2S HW number. Please refer to #hal_i2s_port_t.
 * @return    #HAL_I2S_STATUS_OK, if the operation is successful. \n
              The return value is reserved for further expansion. The current value is always set to #HAL_I2S_STATUS_OK.
 * @sa        #hal_i2s_enable_tx_interrupt_ex()
 */
hal_i2s_status_t  hal_i2s_disable_tx_interrupt_ex(hal_i2s_port_t i2s_port);

/**
 * @brief     This extended function enables the RX interrupt in a multiple I2S mode.
 * @param[in] i2s_port is the I2S HW number. Please refer to #hal_i2s_port_t.
 * @return    #HAL_I2S_STATUS_ERROR, if RX callback function is not registered. Call #hal_i2s_register_rx_callback_ex() to register RX callback function before calling this function. \n
 *               #HAL_I2S_STATUS_OK, if the operation is successful.
 * @sa        #hal_i2s_disable_rx_interrupt_ex()
 * @note   To avoid unpredictable system operation, calling this function is not allowed if RX callback function is not registered.
 */
hal_i2s_status_t  hal_i2s_enable_rx_interrupt_ex(hal_i2s_port_t i2s_port);

/**
 * @brief     This extended function disables the RX interrupt in a multiple I2S mode.
 * @param[in] i2s_port is the I2S HW number. Please refer to #hal_i2s_port_t.
 * @return    #HAL_I2S_STATUS_OK, if the operation is successful. \n
 *             The return value is reserved for further expansion. The current value is always set to #HAL_I2S_STATUS_OK.
 * @sa        #hal_i2s_enable_rx_interrupt_ex()
 */
hal_i2s_status_t  hal_i2s_disable_rx_interrupt_ex(hal_i2s_port_t i2s_port);

/**
 * @brief     This extended function sets up the transmit operation of the DMA TX channel in a multiple I2S mode.
 *            The DMA starts to pump data from the DMA TX FIFO into I2S TX if the I2S TX is enabled. \n
 *            DMA will trigger an interrupt when the amount of output data in DMA TX FIFO is lower than the threshold.
 * @param[in] i2s_port is the I2S HW number. Please refer to #hal_i2s_port_t.
 * @param[in] dma_config is the dma configuration of the I2S module. For more details about this parameter, please refer to #hal_i2s_dma_config_t.
 * @return   #HAL_I2S_STATUS_ERROR, if one of I2S links is still available. Call #hal_i2s_disable_tx_ex(), #hal_i2s_disable_rx_ex() and #hal_i2s_disable_clock_ex() to disable the TX/RX links before calling this function. \n
 *              #HAL_I2S_STATUS_INVALID_PARAMETER, if the buffer is a NULL pointer. \n
 *              #HAL_I2S_STATUS_OK, if the operation is successful.
 * @note   To avoid unpredictable system operation, calling #hal_i2s_set_tx_dma_ex() during the I2S execution is not allowed. User should disable the I2S links before setting the configuration. \n
 *              Buffer is occupied by DMA until the transmission is complete.
 */
hal_i2s_status_t hal_i2s_set_tx_dma_ex(hal_i2s_port_t i2s_port, const hal_i2s_dma_config_t *dma_config);

/**
 * @brief     This extended function sets up the receive operation of the DMA RX channel in a multiple I2S mode.
 *            The DMA starts to pump data from I2S RX into the DMA RX FIFO if the I2S RX is enabled. \n
 *            DMA will trigger an interrupt when the amount of available receive data in the DMA RX FIFO is larger than the threshold.
 * @param[in] i2s_port is the I2S HW number. Please refer to #hal_i2s_port_t.
 * @param[in] dma_config is the dma configuration of the I2S module. For more details about this parameter, please refer to #hal_i2s_dma_config_t.
 * @return  #HAL_I2S_STATUS_ERROR, if one of I2S links is still available. Call #hal_i2s_disable_tx_ex(), #hal_i2s_disable_rx_ex() and #hal_i2s_disable_clock_ex() to disable the TX/RX links before calling this function. \n
 *              #HAL_I2S_STATUS_INVALID_PARAMETER, if the buffer is a NULL pointer. \n
 *              #HAL_I2S_STATUS_OK, if the operation is successful.
 * @note   To avoid unpredictable system operation, calling #hal_i2s_set_rx_dma_ex() during the I2S execution is not allowed. User should disable the I2S links before setting the configuration. \n
 *              Buffer is occupied by DMA until the transmission is complete.
 */
hal_i2s_status_t hal_i2s_set_rx_dma_ex(hal_i2s_port_t i2s_port, const hal_i2s_dma_config_t *dma_config);

/**
 * @brief     This extended function registers the callback function for input data in a multiple I2S mode.
 * @param[in] i2s_port is the I2S HW number. Please refer to #hal_i2s_port_t.
 * @param[in] rx_callback is a callback function for the received data control.
 * @param[in] user_data is a user defined input data.
 * @return    #HAL_I2S_STATUS_INVALID_PARAMETER, if the rx_callback is invalid. \n
 *              #HAL_I2S_STATUS_OK, if the operation is successful.
 * @note      Send I2S RX events to a user-defined task and handle the data transfer there to avoid longer performances in the NVIC ISR.
 */
hal_i2s_status_t hal_i2s_register_rx_callback_ex(hal_i2s_port_t i2s_port, hal_i2s_rx_callback_t rx_callback, void *user_data);

/**
 * @brief     This extended function registers the callback function for output data in a multiple I2S mode.
 * @param[in] i2s_port is the I2S HW number. Please refer to #hal_i2s_port_t.
 * @param[in] tx_callback is a pointer to the callback function to control data transmission.
 * @param[in] user_data is a user defined input data.
 * @return    #HAL_I2S_STATUS_INVALID_PARAMETER, if the tx_callback is invalid. \n
 *              #HAL_I2S_STATUS_OK, if the operation is successful.
 * @note      Send I2S TX events to a user-defined task and handle the data transfer there to avoid longer performances in the NVIC ISR.
 */
hal_i2s_status_t hal_i2s_register_tx_callback_ex(hal_i2s_port_t i2s_port, hal_i2s_tx_callback_t tx_callback, void *user_data);

/**
* @brief     This extended function enables the I2S output link in a multiple I2S mode.
* @param[in] i2s_port is the I2S HW number. Please refer to #hal_i2s_port_t.
* @return    #HAL_I2S_STATUS_OK, if the operation is successful.  \n
             The return value is reserved for further expansion. The current value is always set to #HAL_I2S_STATUS_OK.
* @sa        #hal_i2s_disable_tx_ex()
*/
hal_i2s_status_t  hal_i2s_enable_tx_ex(hal_i2s_port_t i2s_port);

 /**
 * @brief     This extended function enables the I2S input link in a multiple I2S mode.
 * @param[in] i2s_port is the I2S HW number. Please refer to #hal_i2s_port_t.
 * @return    #HAL_I2S_STATUS_OK, if the operation is successful. \n
              The return value is reserved for further expansion. The current value is always set to #HAL_I2S_STATUS_OK.
 * @sa        #hal_i2s_disable_rx_ex()
 */
hal_i2s_status_t  hal_i2s_enable_rx_ex(hal_i2s_port_t i2s_port);

/**
 * @brief     This extended function disables the I2S output link in a multiple I2S mode.
 * @param[in] i2s_port is the I2S HW number. Please refer to #hal_i2s_port_t.
 * @return    #HAL_I2S_STATUS_OK, if the operation is successful. \n
              The return value is reserved for further expansion. The current value is always set to #HAL_I2S_STATUS_OK.
 * @sa        #hal_i2s_enable_tx_ex()
 */
hal_i2s_status_t  hal_i2s_disable_tx_ex(hal_i2s_port_t i2s_port);

/**
 * @brief     This extended function disables the I2S input link in a multiple I2S mode.
 * @param[in] i2s_port is the I2S HW number. Please refer to #hal_i2s_port_t.
 * @return    #HAL_I2S_STATUS_OK, if the operation is successful. \n
              The return value is reserved for further expansion. The current value is always set to #HAL_I2S_STATUS_OK.
 * @sa        #hal_i2s_enable_rx_ex()
 */
hal_i2s_status_t hal_i2s_disable_rx_ex(hal_i2s_port_t i2s_port);

/**
 * @brief     This extended function transmits data to the I2S output link.
 * @param[in] i2s_port is the I2S HW number. Please refer to #hal_i2s_port_t.
 * @param[in] data is the 32-bit output data to send. For more details about the data format for left and right channel, please refer to platform document.\n
              The return value is reserved for further expansion. The current value is always set to #HAL_I2S_STATUS_OK.
 * @note      Call this function after #hal_i2s_get_tx_sample_count_ex() function to ensure there is enough free space in the TX buffer for the write operation.\n
                    One sample count queried by #hal_i2s_get_tx_sample_count() will be consumed as #hal_i2s_tx_write() called once.
 * @sa        #hal_i2s_get_tx_sample_count_ex()
 */
hal_i2s_status_t hal_i2s_tx_write_ex(hal_i2s_port_t i2s_port, uint32_t data);

/**
 * @brief     This extended function receives data from the I2S input link.
 * @param[in] i2s_port is the I2S HW number. Please refer to #hal_i2s_port_t.
 * @param[out] data is the 32-bit data to receive. For more details about the data format for left and right channel, please refer to platform document.\n
 * @return    #HAL_I2S_STATUS_OK, if the operation is successful. \n
              The return value is reserved for further expansion. The current value is always set to #HAL_I2S_STATUS_OK.
 * @note      Call this function after #hal_i2s_get_rx_sample_count_ex() function to ensure there is data available in the RX buffer to perform read operation.
 * @sa        #hal_i2s_get_rx_sample_count_ex()
 */
hal_i2s_status_t hal_i2s_rx_read_ex(hal_i2s_port_t i2s_port, uint32_t *data);

/**
 * @brief      This extended function queries available free space in the TX buffer in a multiple I2S mode.
 * @param[in] i2s_port is the I2S HW number. Please refer to #hal_i2s_port_t.
 * @param[out] sample_count is the available free space in the TX buffer.(Number of data words to write)
 * @return     #HAL_I2S_STATUS_ERROR, if the user do not configure the buffer length for TX buffer by #hal_i2s_set_tx_dma_ex(). \n
 *                #HAL_I2S_STATUS_OK, if the operation is successful.
 * @note       Call this function before #hal_i2s_tx_write_ex() function to ensure there is enough free space in the TX buffer for the write operation.
 * @sa         #hal_i2s_tx_write_ex()
 */
 hal_i2s_status_t hal_i2s_get_tx_sample_count_ex(hal_i2s_port_t i2s_port, uint32_t *sample_count);

/**
 * @brief      This extended function queries length of the received data available in the RX buffer in a multiple I2S mode.
 * @param[in] i2s_port is the I2S HW number. Please refer to #hal_i2s_port_t.
 * @param[out] sample_count is the length of the received data available in the RX buffer.(Number of data words to read)
 * @return     #HAL_I2S_STATUS_OK, if the operation is successful. \n
               The return value is reserved for further expansion. The current value is always set to #HAL_I2S_STATUS_OK.
 * @note       Call this function before #hal_i2s_rx_read_ex() function to ensure there is data available in the RX buffer to perform read operation.
 * @sa         #hal_i2s_rx_read_ex()
 */
hal_i2s_status_t hal_i2s_get_rx_sample_count_ex(hal_i2s_port_t i2s_port, uint32_t *sample_count);

/**
 * @brief      This function enable EOF event notification.
 * @param[in] i2s_port is the I2S HW number. Please refer to #hal_i2s_port_t.
 * @return     #HAL_I2S_STATUS_OK, if the operation is successful. \n
               The return value is reserved for further expansion. The current value is always set to #HAL_I2S_STATUS_OK.
 */
hal_i2s_status_t hal_i2s_set_eof_ex(hal_i2s_port_t i2s_port);

#endif/*HAL_I2S_FEATURE_MULTI_I2S*/


#ifdef __cplusplus
}
#endif


/**
* @}
* @}
*/
#endif /*HAL_I2S_MODULE_ENABLED*/
#endif /* __HAL_I2S_H__ */

