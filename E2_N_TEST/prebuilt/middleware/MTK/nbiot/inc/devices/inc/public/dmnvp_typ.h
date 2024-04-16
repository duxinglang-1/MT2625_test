/* Copyright Statement:
*
* (C) 2005-2017  MediaTek Inc. All rights reserved.
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
/*
 **************************************************************************
 *  File Description : DM NV Processing Task Signal Type Definitions
 **************************************************************************/
#ifndef DMNVP_TYP_H
#define DMNVP_TYP_H

/***************************************************************************
 * Include Files
 **************************************************************************/
#include <system.h>

/***************************************************************************
 * Defines
 **************************************************************************/
/** Number of bytes to be used as data storage */
#if defined (ON_PC)
/*
 * For PC we must have a big value in order to be able to have the whole
 * of MOBILE_EQUIPMENT_DATA in a signal when read/writing NVRAM data.
 * Otherwise it tries to send a pointer which does not work on PC.
 */
#define NVDATASIZE          8192
#else
#define NVDATASIZE          2048
#endif

/****************************************************************************
 * Macros
 ****************************************************************************/
/* none */

 /****************************************************************************
 * Type Definitions
 ****************************************************************************/
typedef Int16 RecordNumber;
typedef Int16 DataLength;
typedef Int16 DataChecksum;
typedef Int32 NvDataType;



typedef enum CalDataIdtag
{

/*
 * -------- IMPORTANT NOTE -------- IMPORTANT NOTE --------
 *
 * ENTRIES MUST BE ADDED AT THE END OF A SECTION NOT IN THE MIDDLE
 * THIS IS TO ENSURE ORDER STAYS THE SAME
 * THIS IS ENSURE CALIBRATION DATA REMAINS VALID
 *
 *  NO UPGRADE SWITCHES SHOULD BE USED IN THIS ENUM
 *
 * -------- IMPORTANT NOTE -------- IMPORTANT NOTE --------
 */

    /*
     * RADIO SPECIFIC CALIBRATION DATA ITEMS SECTION
     */
    CAL_CFGAINCONTROLCODETABLE,    /* used for cfGainControlCodeTable    */
    CAL_CFACCURATERADIOGAINTABLE,  /* used for cfAccurateRadioGainTable  */
    CAL_CFEGSMRSSICORRECTIONTABLE, /* used for cfEgsmRssiCorrectionTable */
    CAL_CFGSMRSSICORRECTIONTABLE,  /* used for cfGsmRssiCorrectionTable  */
    CAL_CFDCSRSSICORRECTIONTABLE,  /* used for cfDcsRssiCorrectionTable  */
    CAL_CFPCSRSSICORRECTIONTABLE,  /* used for cfPcsRssiCorrectionTable  */

    CAL_CFGSM450RSSICORRECTIONTABLE,  /* used for cfGsmRssiCorrectionTable  */
    CAL_CFGSM480RSSICORRECTIONTABLE,  /* used for cfGsmRssiCorrectionTable  */
    CAL_CFGSM850RSSICORRECTIONTABLE,  /* used for cfGsmRssiCorrectionTable  */
    /* AFC Calibration data */
    CAL_CFAFCDACDATA,                 /* used for cfAfrDacDataPtr           */
    /* VCXO Calibration data */
    CAL_CFAFCLOOKUP,
    CAL_CFAFCRADIO,

    /* Power Config Calibration data*/
    CAL_CFRAMPFACTORTABLE,            /* used for l1CfgRampFactorTable      */
    CAL_CFTXFREQCOMPENSATION,         /* used for l1CfgTxFreqCompensation   */

    /* EDGE Specific */
    CAL_CFRAMPFACTORTABLE8PSK,        /* used for 8PSK l1CfgRampFactorTable      */
    CAL_CFTXFREQCOMPENSATION8PSK,     /* used for 8PSK l1CfgRampFactorTable */
    CAL_CFRADIOSPECIFICDATA,          /* place holder to add handling for radio specific calibration data */
    CAL_CFRFDCOFFSET,                 /* Radio DC Offset calibration data */


    /* This Cal ID is for radio power ramp offsets (ramp pedestals) where
       supported by the radio driver, otherwise it is not used */
    CAL_CFTXDACOFFSET,

    /* 3G calibration ID's */
#if defined (PLATFORM_G1MCU)
# if defined (ENABLE_PA_HYSTERESIS)
    CAL_CF3GTXRAMPLOWMODETABLE,        /* 3G TX Ramp low mode Table */
    CAL_CF3GTXRAMPHIGHMODETABLE,       /* 3G TX Ramp high mode Table */
# else /* ENABLE_PA_HYSTERESIS */

#  if !defined(RADIO_3G_FDD_IS_TITAN)
    CAL_CF3GTXRAMPTABLE,              /* 3G TX Ramp Table */
#  else
    CAL_CF3GTXRAMPTABLEBANDI,
    CAL_CF3GTXRAMPTABLEBANDII,
    CAL_CF3GTXRAMPTABLEBANDV,
#  endif
# endif /* ENABLE_PA_HYSTERESIS */

# if !defined(RADIO_3G_FDD_IS_TITAN)
    CAL_CF3GGAINRISING,               /* 3G RX Gain Table - rising Gain */
    CAL_CF3GGAINFALLING,              /* 3G RX Gain Table - falling Gain */
# else
    CAL_CF3GRXAGCPARATABLEBANDI,
    CAL_CF3GRXAGCPARATABLEBANDII,
    CAL_CF3GRXAGCPARATABLEBANDV,
# endif

    /*
     * RADIO SPECIFIC CALIBRATION DATA ITEMS SECTION FOR TDD
     */
    CAL_CFGTDDBANDAAPCTABLE,          /* used for pT1Rda8207APCTableBandA */
#else
    /* Add new GGE CAL_ID here for G2 */
    /* Add for RATT Tools */
    CAL_CFTXTIMINGTABLE,              /* GGE Tx GPO timing control table */
    CAL_CFRAMPTHRESHOLDTABLE,         /* used for l1CfgRampThresholdTable      */
    CAL_CFRAMPUPDOWNDATATABLE,        /* used for l1CfgRampUpDownDataTable     */
    CAL_CFRAMPINTERDATATABLE,         /* used for l1CfgRampInterDataTable      */
    CAL_CFRAMPTHRESHOLDTABLE8PSK,        /* used for 8PSK l1CfgRampThresholdTable    */
    CAL_CFRAMPUPDOWNDATATABLE8PSK,      /* used for 8PSK l1CfgRampUpDownDataTable  */
    CAL_CFRAMPINTERDATATABLE8PSK,     /* used for 8PSK l1CfgRampInterDataTable */
    CAL_CFGSMASMCTRLTABLE,            /* GSM ASM control logic table */

    CAL_CF3GTXRAMPLOWMODETABLE = 33,  /* 3G TX Ramp low mode Table */
    CAL_CF3GTXRAMPHIGHMODETABLE,      /* 3G TX Ramp high mode Table */

    CAL_CF3GTXRAMPTABLE,              /* 3G TX Ramp Table */

    CAL_CF3GTXRAMPTABLEBANDI,
    CAL_CF3GTXRAMPTABLEBANDII,
    CAL_CF3GTXRAMPTABLEBANDV,

    CAL_CF3GTXCOMPENSATEBANDI,
    CAL_CF3GTXCOMPENSATEBANDII,
    CAL_CF3GTXCOMPENSATEBANDV,

    CAL_CF3GGAINRISING,               /* 3G RX Gain Table - rising Gain */
    CAL_CF3GGAINFALLING,              /* 3G RX Gain Table - falling Gain */

    CAL_CF3GRXAGCPARATABLEBANDI,
    CAL_CF3GRXAGCPARATABLEBANDII,
    CAL_CF3GRXAGCPARATABLEBANDV,

    CAL_CF3GRXAGCCOMPENSATEBANDI,
    CAL_CF3GRXAGCCOMPENSATEBANDII,
    CAL_CF3GRXAGCCOMPENSATEBANDV,

    /* Add new FDD CAL_ID here for G2 */
    /* Add for RATT Tools */
    CAL_CFFDDASMCTRLTABLE,               /* FDD ASM control logic table */
    CAL_CFFDDPAPHASEDISCONTINUITYTABLE,  /* FDD PA phase discontinuity offset table */
    CAL_CFFDDSARADCTABLE,                /* FDD Sar ADC threshold table */

    /*
     * RADIO SPECIFIC CALIBRATION DATA ITEMS SECTION FOR TDD
     */
    CAL_CFGTDDBANDAAPCTABLE = 66,      /* used for TDD APC BandA Table */
#endif  /* PLATFORM_G1MCU */

    
    CAL_CFGTDDBANDAAPCCOMPENSATION,    /* used for TDD APC Compensation BandA Table */
    CAL_CFGTDDBANDAAGCTABLE,          /* used for TDD AGC BandA Table */
    CAL_CFGTDDBANDAAGCCOMPENSATION,    /* used for TDD AGC Compensation BandA Table */
    
    CAL_CFGTDDBANDFAPCTABLE,           /* used for TDD APC BandF Table */
    CAL_CFGTDDBANDFAPCCOMPENSATION,     /* used for APC Compensation BandF Table */
    CAL_CFGTDDBANDFAGCTABLE,           /* used for TDD AGC BandF Table */
    CAL_CFGTDDBANDFAGCCOMPENSATION,     /* used for TDD AGC Compensation BandF Table */
    
#if defined (PLATFORM_G1MCU)
    CAL_LAST_RADIO_ENTRY,

    CAL_CF3GTXCOMPENSATEBANDI = 50,
    CAL_CF3GTXCOMPENSATEBANDII,
    CAL_CF3GTXCOMPENSATEBANDV,

    CAL_CF3GRXAGCCOMPENSATEBANDI,
    CAL_CF3GRXAGCCOMPENSATEBANDII,
    CAL_CF3GRXAGCCOMPENSATEBANDV,

    /* Add here if add new CAL_ID for G1 */

    /* Add GGE CalID for RATT Tools */
    CAL_CFTXTIMINGTABLE,                /* GGE Tx GPO timing control table */
    CAL_CFRAMPTHRESHOLDTABLE,           /* used for l1CfgRampThresholdTable      */
    CAL_CFRAMPUPDOWNDATATABLE,          /* used for l1CfgRampUpDownDataTable     */
    CAL_CFRAMPINTERDATATABLE,           /* used for l1CfgRampInterDataTable      */
    CAL_CFRAMPTHRESHOLDTABLE8PSK,          /* used for 8PSK l1CfgRampThresholdTable    */
    CAL_CFRAMPUPDOWNDATATABLE8PSK,        /* used for 8PSK l1CfgRampUpDownDataTable  */
    CAL_CFRAMPINTERDATATABLE8PSK,       /* used for 8PSK l1CfgRampInterDataTable */
    CAL_CFGSMASMCTRLTABLE,              /* GSM ASM control logic table */

    /* Add FDD CalID for RATT Tools */
    CAL_CFFDDASMCTRLTABLE,              /* FDD ASM control logic table */
    CAL_CFFDDPAPHASEDISCONTINUITYTABLE, /* FDD PA phase discontinuity offset table */
    CAL_CFFDDSARADCTABLE,               /* FDD Sar ADC threshold table */
#else
    /*
     * IMEI DATA ITEMS SECTION
     */
    CAL_CFSVN_PART,
    CAL_CFIMEI_PART,                    /* The default encrypted imei*/
    CAL_CFIMEI_1ST_PART,                /* The 1st encrypted imei*/
    CAL_CFIMEI_2ND_PART,                /* The 2nd encrypted imei*/
    CAL_CFIMEI_3RD_PART,                /* The 3rd encrypted imei*/
    CAL_CFIMEI_4TH_PART,                /* The 4th encrypted imei*/

    /* Add new TDD CAL_ID here for G2 */
    
    /* Add for RATT tools */
    CAL_CFGTDDBANDAPACOMPENSATION,
    CAL_CFGTDDBANDFPACOMPENSATION,
    CAL_CFGTDDBANDEPACOMPENSATION,

    CAL_CFGTDDBANDEAPCTABLE,           /* used for TDD APC BandE Table */
    CAL_CFGTDDBANDEAPCCOMPENSATION,     /* used for APC Compensation BandE Table */
    CAL_CFGTDDBANDEAGCTABLE,           /* used for TDD AGC BandE Table */
    CAL_CFGTDDBANDEAGCCOMPENSATION,     /* used for TDD AGC Compensation BandE Table */

    CAL_LAST_RADIO_ENTRY,
#endif  /* PLATFORM_G1MCU */

    /*
     * AUDIO CALIBRATION DATA ITEMS SECTION
     */
    CAL_FIRST_AUDIO_ENTRY = 100,
    CAL_CFMAINSPEAKERCONFIGTABLE,  /* used for cfMainSpeakerConfigTable  */

    CAL_CFDSPTESTSPEAKERCONFIGTABLE,  /* used for cfDspTestSpeakerConfigTable  */
    CAL_CFAUXSPEAKERCONFIGTABLE,   /* used for cfAuxSpeakerConfigTable   */

    CAL_CFEXTSPEAKERCONFIGTABLE,   /* used for cfExtSpeakerConfigTable   */

    CAL_CFAUDIOOUTPUT0CONFIGTABLE, /* used for cfAudioOutput0ConfigTable */
    CAL_CFAUDIOOUTPUT1CONFIGTABLE, /* used for cfAudioOutput0ConfigTable */
    CAL_CFAUDIOOUTPUT2CONFIGTABLE, /* used for cfAudioOutput0ConfigTable */

    CAL_CFMAINMICCONFIGTABLE,      /* used for cfMainMicConfigTable      */
    CAL_CFDSPTESTMICCONFIGTABLE,    /* used for cfDspTestMicConfigTable      */
    CAL_CFAUXMICCONFIGTABLE,       /* used for cfAuxMicConfigTable       */

    CAL_CFEXTMICCONFIGTABLE,       /* used for cfExtMicConfigTable       */

    CAL_CFVOLLEVELTOVALUES,        /* used for cfVolLevelToValuesConfigTable */

    CAL_CFAUDIOSTEREOOUTPUTCONFIGTABLE = CAL_CFVOLLEVELTOVALUES + 2, /* used for cfstereoOutputConfigTable */

    CAL_CFRINGTONESPEAKERCONFIGTABLE,

    CAL_CFBLUETOOTHSPEAKERCONFIGTABLE, /* used for cfBluetoothSpeakerConfigTable  */
    CAL_CFBLUETOOTHMICCONFIGTABLE,     /* used for cfBluetoothMicConfigTable      */

    CAL_CFSPEAKERPHONESPEAKERCONFIGTABLE, /* used for cfSpeakerphoneSpeakerConfigTable  */
    CAL_CFSPEAKERPHONEMICCONFIGTABLE,     /* used for cfSpeakerphoneMicConfigTable      */

/*
  * Include other files with Cal Data Id's in them here
  * Format of the file must be
  *
  *     CAL_CF<id 1>,
  *     CAL_CF<id 2>,
  *    ........
  *     CAL_CF<id n>,
  */

    /* make sure Audio Calibration header (TTTT) start from 0x3FC, Audio Calibration data start from 0x400 */
    CAL_LAST_ENTRY
}
CalDataId;

/** Type used to indicate how data is passed by a signal */
typedef enum NvControlTag
{
  /** Signal contains read/write data to be used */
  NV_CONT_USE_DATA,
  /** Signal contains pointer to read/write data */
  NV_CONT_USE_POINTER,
  /** Signal contains return pointer to access data item directly, no buffer */
  NV_CONT_GET_POINTER,
  /** Signal contains read/write data ( multiple blocks )   */
  NV_CONT_EXT_DATA
}
NvControl;

typedef struct NvDataTag
{
  DataLength  numBytes;         /* Number of bytes of data returned   */
  DataLength  offset;           /* Offset to start at from in bytes   */
  Int8       *dataBuffer_p;     /* Points to data is using pointers   */
  Int8        data[NVDATASIZE]; /* Allows data to be passed in signal */
}
NvData;

typedef struct NvDataReqTag
{
  DataLength  numBytes;      /* Maximum number of bytes of data requested, or zero        */
  DataLength  offset;        /* Offset to start at from in bytes   */
  Int8       *dataBuffer_p;  /* Points to destination if nvControl is NV_CONT_USE_POINTER */
}
NvDataReq;

#endif /* DMNVP_TYP_H */

/* END Of FILE */

