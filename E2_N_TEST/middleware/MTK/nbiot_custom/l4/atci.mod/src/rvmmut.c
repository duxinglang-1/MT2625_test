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
 ***************************************************************************
 * File Description:
 *
 * Procedures for Mobility Management AT command execution
 *
 * AT+COPS     - which selects the network operator
 * AT+CPOL     - which configures the preferred operator list on the SIM.
 * AT+COPN     - displays a list of operators
 * AT+CREG     - controls network registration status indications
 * AT+CESQ     - command which displays the signal quality report
 * AT+CPLS     - command to select the file containing the preferred PLMN list
 * AT^MODE     - indicate System Mode Modification
 **************************************************************************/

#define MODULE_NAME "RVMMUT"

/***************************************************************************
 * Include Files
 ***************************************************************************/

#include <system.h>
#include <gkisig.h>
#include <gkimem.h>
#include <gkitimer.h>

#include <rvmmut.h>
#include <rvchman.h>
#include <rvcrhand.h>
#include <rvcrerr.h>
#include <rvslsigo.h>
#include <rvomtime.h>
#include <rvmmsigo.h>
#include <rvcimxut.h>
#include <rvmmss.h>
#include <rvslut.h>
#if defined (ENABLE_AT_ENG_MODE)
# include <abem_sig.h>
#endif
#include <abmm_sig.h>
#if defined (UPGRADE_SHARE_MEMORY)
#include <r2_hal.h>
#include <t1muxshmdrv.h>
#endif
#if defined (UPGRADE_SHMCL_SOLUTION)
#include <muxconn_at.h>      /* For at command data */
#endif /* UPGRADE_SHMCL_SOLUTION  */

#include <mmdbm_sig.h>

#include <math.h>

/***************************************************************************
 * Signal definitions
 ***************************************************************************/

union Signal
{
  MmDbmReestablishReq       mmDbmReestablishReq;
};

/***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/

/***************************************************************************
 * Types
 ***************************************************************************/

typedef struct vgCPOLPrefPlmnEntryTag
{
    Boolean  plmnEntryUsed;
    Plmn     plmn;
    Boolean  threeDigitMnc;
    Boolean  plmnUsed;
    PlmnName plmnName;
    Boolean  plmnNameUsed;
    Int16    physicalIndex;
} VgCPOLPrefPlmnEntry;

/***************************************************************************
 * Manifest Constants
 ***************************************************************************/
#define ACT_NBIOT                      9

#define LE_BYTE                        1 /* one byte length for LE in USIM command */

#define VG_CESQ_MIN_RXLEV                0
#define VG_CESQ_MAX_RXLEV               63


#define VG_CESQ_MIN_BER                 0
#define VG_CESQ_MAX_BER                 7
#define VG_CESQ_INVALID_BER             99

#define VG_CESQ_MIN_RSCP               0
#define VG_CESQ_MAX_RSCP               96
#define VG_CESQ_INVALID_RSCP           255

#define VG_CESQ_MIN_ECNO               0
#define VG_CESQ_MAX_ECNO               49
#define VG_CESQ_INVALID_ECNO           255

#define VG_CESQ_MIN_RSRQ               0
#define VG_CESQ_MAX_RSRQ               34


#define VG_CESQ_MIN_RSRP               0
#define VG_CESQ_MAX_RSRP               97


#if defined (UPGRADE_3G)
#define VG_CESQ_UTRAN_RSCP_MIN_DB      (-120)
#define VG_CESQ_UTRAN_ECNO_MIN_DB      (-24)
#endif /* UPGRADE_3G */

#define EUTRAN_MAX_RSSI                (-48)
#define EUTRAN_MIN_RSSI                (-110)

#define EUTRAN_RSSI_RANGE              (EUTRAN_MAX_RSSI - EUTRAN_MIN_RSSI)

#define EUTRAN_MAX_RXLEV               63
#define EUTRAN_MIN_RXLEV               0

#define VG_CESQ_MIN_RXLEV_DB             (-110)

#define VG_CESQ_EUTRAN_RSRQ_MIN_DB     (-19.5)
#define VG_CESQ_EUTRAN_RSRP_MIN_DB     (-140)

#if !defined (LTE_FDD_RAT)
#define LTE_FDD_RAT                    0
#endif

#if !defined (LTE_TD_RAT)
#define LTE_TD_RAT                     0
#endif

#define VG_CSQ_MIN_RSSI                 0
#define VG_CSQ_MAX_RSSI               31
#define VG_CSQ_INVALID_RSSI           99

#define VG_CSQ_MIN_RSSI_DB             (-113)
#define VG_CESQ_MAX_BER_PERCENT             (128)
#define VG_CESQ_MIN_BER_PERCENT   (2)

/***************************************************************************
 * Variables
 ***************************************************************************/
 /* 0 is EDRX_VALUE_0 - but in this case it is used to reset to the previous value in EMM */
static const Int8 validEdrxValues[] = {2,3,5,9,10,11,12,13,14,15,EDRX_VALUE_0};

/***************************************************************************
 * Macros
 ***************************************************************************/

#define MCC_DIGIT_LENGTH      (3)
#define MNC_DIGIT_LENGTH_MIN  (2)
#define MNC_DIGIT_LENGTH_MAX  (3)
# define MAX_CPOL_SIM_COMMAND (10)

#define VG_ERRC_BANDS_RF_BAND_1   0x00000001
#define VG_ERRC_BANDS_RF_BAND_2   0x00000002
#define VG_ERRC_BANDS_RF_BAND_3   0x00000004
#define VG_ERRC_BANDS_RF_BAND_4   0x00000008
#define VG_ERRC_BANDS_RF_BAND_5   0x00000010
#define VG_ERRC_BANDS_RF_BAND_8   0x00000020
#define VG_ERRC_BANDS_RF_BAND_11  0x00000040
#define VG_ERRC_BANDS_RF_BAND_12  0x00000080
#define VG_ERRC_BANDS_RF_BAND_13  0x00000100
#define VG_ERRC_BANDS_RF_BAND_14  0x00000200
#define VG_ERRC_BANDS_RF_BAND_17  0x00000400
#define VG_ERRC_BANDS_RF_BAND_18  0x00000800
#define VG_ERRC_BANDS_RF_BAND_19  0x00001000
#define VG_ERRC_BANDS_RF_BAND_20  0x00002000
#define VG_ERRC_BANDS_RF_BAND_21  0x00004000
#define VG_ERRC_BANDS_RF_BAND_25  0x00008000
#define VG_ERRC_BANDS_RF_BAND_26  0x00010000
#define VG_ERRC_BANDS_RF_BAND_28  0x00020000
#define VG_ERRC_BANDS_RF_BAND_31  0x00040000
#define VG_ERRC_BANDS_RF_BAND_66  0x00080000
#define VG_ERRC_BANDS_RF_BAND_70  0x00100000
#define VG_ERRC_BANDS_RF_BAND_71  0x00200000
#define VG_ERRC_BANDS_RF_BAND_72  0x00400000
#define VG_ERRC_BANDS_RF_BAND_73  0x00800000
#define VG_ERRC_BANDS_RF_BAND_74  0x01000000
#define VG_ERRC_BANDS_RF_BAND_85  0x02000000

extern uint32_t supportedBandsAsEncodedBandBitMask;

/***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/

static ResultCode_t checkActPresence (CommandLine_t *commandBuffer_p,
                                      Int32         *act);

static Boolean vgOperStringToPlmn (Char  *plmnString_p,
                                   Plmn  *convertedPlmn,
                                   Int16 *plmnStringLen);
static Boolean checkOperator (Char  *networkOperator,
                              Int16 length);
static ResultCode_t vgSelectSimFile (const VgmuxChannelNumber entity);
/***************************************************************************
 * Type Definitions
 ***************************************************************************/

/***************************************************************************
 * Local Functions
 ***************************************************************************/
/*--------------------------------------------------------------------------
*
* Function:    vgSelectSimFile
*
* Parameters:  entity
*
*
* Returns:     Resultcode_t    - result of function
*
* Description: This function selects the requested preferred PLMN file on the SIM.
* the response from the GET RESPONSE will provide useful info about the file
* such as the file size. From there, we can calculate how many PLMN entries are
 * supported on the SIM
*
*-------------------------------------------------------------------------*/

static ResultCode_t vgSelectSimFile (const VgmuxChannelNumber entity)
{
    SimLockGenericContext_t *simLockGenericContext_p = ptrToSimLockGenericContext ();
    VgSimInfo               *simInfo                 = &(simLockGenericContext_p->simInfo);
    MobilityContext_t       *mobilityContext_p       = ptrToMobilityContext ();
    VgCPLSData              *vgCPLSData_p            = &mobilityContext_p->vgCPLSData;
    ResultCode_t            result                   = RESULT_CODE_OK;
    SimEfId                 efId                     = SIM_EF_INVALID;
    Int8                    pathLength;
    Int8                    simCommand[MAX_CPOL_SIM_COMMAND] = {0};

    if (vgCPLSData_p->plmnSelector == ABMM_USER_SELECTOR)
    {
        if (simInfo->userPlmnSelector)
        {
            efId = SIM_EF_PLMNW_ACT;
        }
        else
        {
            efId = SIM_EF_PLMN_SEL;
        }
    }
    else if (vgCPLSData_p->plmnSelector == ABMM_OPERATOR_SELECTOR)
    {
        efId = SIM_EF_OPLMNW_ACT;
    }
    else if (vgCPLSData_p->plmnSelector == ABMM_HPLMN_SELECTOR)
    {
        efId = SIM_EF_HPLMNW_ACT;
    }

    if ( efId == SIM_EF_INVALID)
    {
        result = VG_CME_NOT_FOUND;
    }
    else
    {
        /* In 3G, the GET RESPONSE is done automatically by the drivers.
         * So we should only send a SELECT command, and the SIM drivers will pass the response
         * to the GET RESPONSE */
        if (!simInfo->cardIsUicc)
        {
            /*GSM SIM*/
            pathLength    = 2;
            simCommand[0] = VG_CRSM_CLASS_GSM_APP;  /*GSM class byte*/
            simCommand[1] = VG_CRSM_COMMAND_SELECT;
            simCommand[2] = 0;
            simCommand[3] = 0;
            simCommand[4] = pathLength;      /* length of data to follow */
            simCommand[5] = (Int8) (efId >> 8);      /*file ID*/
            simCommand[6] = (Int8) (efId & 0x00ff);  /*file ID*/

            result        = vgSendApexSimGenAccessReq (entity,
                VG_CRSM_COMMAND_HEADER_SIZE + pathLength,
                simCommand,
                (Int16)SIM_EF_INVALID,
                (Int16)SIM_DIR_DF_GSM,                                 /*will select GSM dir before selecting the file*/
                (Int16)SIM_DIR_INVALID,
                /* job134856: add handling for <pathid> field */
                /* field not used here */
                PNULL,
                0);

        }
        else
        {
            /*3G SIM*/
            pathLength    = 4;
            simCommand[0] = VG_CRSM_CLASS_3G_APP;
            simCommand[1] = VG_CRSM_COMMAND_SELECT;
            simCommand[2] = SELECT_BY_PATH_FROM_MF;  /*select by path from MF*/
            simCommand[3] = 0x04;      /*return FCP template*/
            simCommand[4] = pathLength;      /* length of path */
            simCommand[5] = (Int8)(SIM_DIR_ADF_USIM >> 8);
            simCommand[6] = (Int8)(SIM_DIR_ADF_USIM & 0x00ff);
            simCommand[7] = (Int8)(efId >> 8);  /*file ID*/
            simCommand[8] = (Int8)(efId & 0x00ff);      /*file ID*/
            simCommand[9] = 0; /*Le, length expected. Le=0 means up to 255 bytes expected*/

            result        = vgSendApexSimGenAccessReq (entity,
                VG_CRSM_COMMAND_HEADER_SIZE + pathLength + LE_BYTE,
                simCommand,
                (Int16) SIM_EF_INVALID,
                (Int16)SIM_DIR_INVALID,                                 /*will select GSM dir before selecting the file*/
                (Int16)SIM_DIR_INVALID,
                /* job134856: add handling for <pathid> field */
                /* field not used here */
                PNULL,
                0);
        }
    }
    return (result);
} /* vgSelectSimFile */

/*--------------------------------------------------------------------------
*
* Function:    checkActPresence
*
* Parameters:  commandBuffer_p - pointer to command line string
*
*
* Returns:     Resultcode_t    - result of function
*
* Description: This function checks whether GSM Act/ GSM Compact Act/ Utran Act
* has been specified by the user in the AT+CPOL command.
*
*-------------------------------------------------------------------------*/

static ResultCode_t checkActPresence (CommandLine_t *commandBuffer_p,
                                      Int32         *act)
{
    ResultCode_t result = RESULT_CODE_OK;

    if (getExtendedParameter (commandBuffer_p, act, ULONG_MAX) == TRUE)
    {
        if ((*act) != ULONG_MAX)
        {
            if ((*act) >= VG_OP_ACT_INVALID)
            {
                /*act has been specified, but is out of range*/
                result = VG_CME_CPOL_ACT_WRONG;
            }
        }
        else
        {
            /*act missing*/
            result = VG_CME_CPOL_ACT_MISSING;
        }
    }
    return (result);
} /* checkActPresence */

/*--------------------------------------------------------------------------
*
* Function:    vgOperStringToPlmn
*
* Parameters:  *plmnString_p - pointer to array containing PLMN text string
*              which contains numeric description of PLMN mcc and mnc
*
*              *convertedPlmn - pointer to structure 'Plmn' which will be
*              filled in by this function
*
* Returns:     Boolean success - TRUE if conversion is successful.
*
* Description: Converts PLMN (mcc,mnc) from text string format to numeric
*              format
*-------------------------------------------------------------------------*/

static Boolean vgOperStringToPlmn ( Char  *plmnString_p,
                                    Plmn  *convertedPlmn,
                                    Int16 *plmnStringLen)
{
    Int32 val;
    Boolean success = TRUE;
    Int16 len       = 0;
    Char plmnString[PLMN_NAME_FULL_LENGTH] = {0};  /* Max possible length */

    /* if 2 digit MNC has been entered as 12f then remove trailing f */
    if((plmnString_p[(*plmnStringLen) - 1] == 'f') ||
       (plmnString_p[(*plmnStringLen) - 1] == 'F'))
    {
        plmnString_p[(*plmnStringLen) - 1] = NULL_CHAR;
        (*plmnStringLen)--;
    }
    /* Initialise string */
    memcpy (plmnString, plmnString_p, PLMN_NAME_FULL_LENGTH * sizeof(Char));

    len = *plmnStringLen;

    /* check length is valid */
    if ((len < (MCC_DIGIT_LENGTH + MNC_DIGIT_LENGTH_MIN)) ||
        (len > (MCC_DIGIT_LENGTH + MNC_DIGIT_LENGTH_MAX)))
    {
        success = FALSE;
    }
    else
    {
        /* get MNC value */
        if (vgHexaToBin (&plmnString[MCC_DIGIT_LENGTH], &val) == FALSE)
        {
            success = FALSE;
        }
        else
        {
            /* record MNC */
            convertedPlmn->mnc           = (Mnc)val;

            /* null terminate string after MCC */
            plmnString[MCC_DIGIT_LENGTH] = NULL_CHAR;

            /* get MCC value */
            if (vgHexaToBin (plmnString, &val) == FALSE)
            {
                success = FALSE;
            }
            else
            {
                /* record MCC */
                convertedPlmn->mcc = (Mcc)val;
            }
        }
    }

    return (success);
} /* vgOperStringToPlmn */

/*--------------------------------------------------------------------------
*
* Function:    checkOperator
*
* Parameters:  entity          - current mux channel
*              networkOperator - network string
*
* Returns:     Boolean - TRUE if operator maybe allowed
*
* Description: Checks the suitability of the given string as a network
*              operator
*-------------------------------------------------------------------------*/

static Boolean checkOperator (Char  *networkOperator,
                              Int16 length)
{
    MobilityContext_t *mobilityContext_p = ptrToMobilityContext ();
    VgCOPSData        *vgCOPSData        = &(mobilityContext_p->vgCOPSData);
    Boolean result = TRUE;

    switch (vgCOPSData->format)
    {
        case VG_OP_NUM: /* Operator number format */
        {
            /* Plmn should be numeric (mcc,mnc). Convert from string to type 'Plmn' */
            result = vgOperStringToPlmn (networkOperator,
                &(mobilityContext_p->vgCOPSData.selectedPlmn.plmn),
                &length);
            break;
        }
        case VG_OP_SHORT: /* Operator short string format */
        {
            if (length < PLMN_NAME_ABBR_LENGTH)
            {
                memcpy (&mobilityContext_p->vgCOPSData.selectedPlmn.plmnName.abbr[0],
                    &networkOperator[0],
                    length);
                mobilityContext_p->vgCOPSData.selectedPlmn.plmnName.abbr[length] = (Char)0;
            }
            else
            {
                result = FALSE;
            }
            break;
        }
        case VG_OP_LONG: /* Operator long string format */
        {
            if (length < PLMN_NAME_FULL_LENGTH)
            {
                memcpy (&mobilityContext_p->vgCOPSData.selectedPlmn.plmnName.full[0],
                    &networkOperator[0],
                    length);
                mobilityContext_p->vgCOPSData.selectedPlmn.plmnName.full[length] = (Char)0;
            }
            else
            {
                result = FALSE;
            }
            break;
        }
        default: /* should never get here, otherwise current format
              * not valid */
        {
            /* Invalid operator name format */
            FatalParam(vgCOPSData->format, 0, 0);
  //          result = FALSE;
            break;
        }
    } /* switch */

    return (result);
} /* checkOperator */

/***************************************************************************
 * Global Functions
 ***************************************************************************/
/******************************************************************************
 * vgMmGetMncFromImsi - decodes the raw Imsi, and returns the MNC.
 * The threeDigitMncDecoding flag should be set to TRUE if the MNC is encoded
 * on 3 digits.
 ********************************************************************************/
Mnc  vgMmGetMncFromImsi (Boolean threeDigitMncDecoding, Imsi *rawImsi_p)
{
  Mnc        mnc = 0;
  Mnc        localMnc = 0;

  if (threeDigitMncDecoding)
  {
    localMnc = (rawImsi_p->contents[MNC_OFFSET] & 0x0f) << 8;
    localMnc |= (rawImsi_p->contents[MNC_OFFSET] & 0xf0);
    mnc = (Mnc)(localMnc | (rawImsi_p->contents[MNC_OFFSET + 1] & 0x0f));
  }
  else
  {
    localMnc = (Mnc)((rawImsi_p->contents[MNC_OFFSET] & 0xf0) >> 4);
    mnc = (Mnc)(localMnc | ((rawImsi_p->contents[MNC_OFFSET] & 0x0f) << 4));
  }

  return (mnc);
}

/******************************************************************************
 * vgMmGetMccFromImsi - gets the MCC from the raw IMSI.
 *******************************************************************************/
Mcc  vgMmGetMccFromImsi (Imsi *rawImsi_p)
{
  Mcc        mcc = 0;

  mcc = (rawImsi_p->contents[MCC_OFFSET] & 0xf0);
  mcc = (mcc << 4);
  mcc = mcc | ((rawImsi_p->contents[MCC_OFFSET + 1] & 0x0f) << 4);
  mcc = mcc | ((rawImsi_p->contents[MCC_OFFSET + 1] & 0xf0) >> 4);

  return (mcc);
}

/*--------------------------------------------------------------------------
*
* Function:    vgMmCPOLInitialiseData
*
* Parameters:  entity          - mux channel number
*              clearReadFormat - set readFormat
*
* Returns:     nothing
*
* Description: Initialises the data structure 'VgCPOLData' which contains all
*              the data require to execute the SET version of the +CPOL AT
*              command. The field 'readFormat' is a special case. It is only
*              initialised if the value of 'clearReadFormat' is TRUE.
*-------------------------------------------------------------------------*/

void vgMmCPOLInitialiseData (Boolean clearReadFormat)
{
    MobilityContext_t *mobilityContext_p = ptrToMobilityContext ();
    VgCPOLData        *vgCPOLData_p      = &mobilityContext_p->vgCPOLData;

    /* Special case. Only initialise if flag set */
    if (clearReadFormat == TRUE)
    {
        vgCPOLData_p->readFormat = VG_OP_NUM;
    }

    vgCPOLData_p->action                      = (PlmnMenuAction)0xFF;
    vgCPOLData_p->index                       = 0;
    vgCPOLData_p->plmnNumberPresent           = FALSE;
    vgCPOLData_p->plmnNumber.mcc              = (Mcc)0;
    vgCPOLData_p->plmnNumber.mnc              = (Mnc)0;
    vgCPOLData_p->plmnNumber.accessTechnology = (AccessTechnologyId)0;
    vgCPOLData_p->threeDigitMnc               = FALSE;
    vgCPOLData_p->plmnName.plmnCoding         = PLMN_CODING_DEFAULT;
    vgCPOLData_p->plmnNamePresent             = FALSE;
    vgCPOLData_p->firstPlmn                   = TRUE;

    memset (vgCPOLData_p->plmnName.full,
        0,
        PLMN_NAME_FULL_LENGTH * sizeof (Int8));
    memset (vgCPOLData_p->plmnName.abbr,
        0,
        PLMN_NAME_ABBR_LENGTH * sizeof (Int8));
    memset (vgCPOLData_p->plmnName.initials,
        0,
        COUNTRY_INITIALS_LENGTH * sizeof (Int8));
    memset (vgCPOLData_p->plmnName.format,
        NULL_CHAR,
        FORMAT_SPECIFIER_LENGTH * sizeof (Char));
} /* vgMmCPOLInitialiseData */

/*--------------------------------------------------------------------------
*
* Function:    vgMmCPOL
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This function executes the AT+CPOL command which displays and
*              sets one of the preferred PLMN lists which is stored on the SIM.
*              CPLS can be used to set which list should be read/updated using CPOL.
*
*              Defined set/display values
*              <indexn>: integer type; the order number of operator in the
*                        SIM preferred operator list.
*              <format>: 0 - long format alphanumeric <oper>
*                        1 - short format alphanumeric <oper>
*                        2 - numeric <oper>
*              <opern>:  string type; <format> indicates if the format is
*                        alphanumeric or numeric (see +COPS)
*-------------------------------------------------------------------------*/

ResultCode_t vgMmCPOL (CommandLine_t            *commandBuffer_p,
                       const VgmuxChannelNumber entity)
{
    ResultCode_t        result                       = RESULT_CODE_OK;
    ExtendedOperation_t operation                    = getExtendedOperation (commandBuffer_p);

    Boolean             indexPresent                 = FALSE; /* TRUE if index present */
    Boolean             formatPresent                = FALSE; /* TRUE if format present */
    Boolean             operPresent                  = FALSE; /* TRUE if operator present */
    Boolean             actPresent                   = FALSE; /* TRUE if <GSM Act>, <GSM Compact Act> and <Utran Act> are present */

    Char plmnString[PLMN_NAME_FULL_LENGTH + NULL_TERMINATOR_LENGTH] = {0};
    Int16                   plmnStringLen            = 0;
    Int32                   index                    = 0; /* List index */
    Int32                   format                   = 0; /* Format for reading/writing list */
    Int32                   gsmAct                   = 0; /* GSM Act*/
    Int32                   gsmCompactAct            = 0; /* GSM compact Act*/
    Int32                   utranAct                 = 0; /* UTRAN Act*/

    Int32                   eutranAct                = 0; /* EUTRAN Act*/
    Plmn                    convertedPlmn            = {0}; /* Numeric plmn values */
    Boolean                 threeDigitMnc            = FALSE;
    SimLockGenericContext_t *simLockGenericContext_p = ptrToSimLockGenericContext ();
    VgSimInfo               *simInfo                 = &(simLockGenericContext_p->simInfo);
    MobilityContext_t       *mobilityContext_p       = ptrToMobilityContext ();
    VgCPOLData              *vgCPOLData_p            = &mobilityContext_p->vgCPOLData;
    VgCPLSData              *vgCPLSData_p            = &mobilityContext_p->vgCPLSData;
#if defined (UPGRADE_3G)
    ChManagerContext_t      *chManagerContext_p      =  ptrToChManagerContext ();
#endif

    switch (operation)
    {
        case EXTENDED_RANGE: /* +CPOL=? */
        {
            if (simLockGenericContext_p->simState == VG_SIM_READY)
            {
                if ((vgCPLSData_p->plmnSelector == ABMM_USER_SELECTOR) &&
                    (!simInfo->plmnSelector) &&
                    (!simInfo->userPlmnSelector))
                {
                    /*by default, vgCPLSData_p->plmnSelector is set to user selector*/
                    /*However, this SIM does not support EF PLMN SEL/EF PLMN w ACT*/
                    result = VG_CME_SIM_FILE_NOT_PRESENT;
                }
                else
                {
                    result = vgSelectSimFile (entity);
                }
            }
            else
            {
                /* get SIM state error */
                result = vgGetSimCmeErrorCode ();
            }
            break;
        }
        case EXTENDED_ASSIGN: /* +CPOL=[<index>][[,<format>][,<oper>]] */
        {
            if (simLockGenericContext_p->simState == VG_SIM_READY)
            {
                /* See whether the 'index' has been entered */
                if (getExtendedParameter (commandBuffer_p, &index, ULONG_MAX) == TRUE)
                {
                    if (index != ULONG_MAX)
                    {
                        indexPresent = TRUE;
                    }
                }
                /* check index is in range */
                if (indexPresent)
                {
                    if ((vgCPLSData_p->plmnSelector == ABMM_USER_SELECTOR) &&
                        (!simInfo->plmnSelector) &&
                        (!simInfo->userPlmnSelector))
                    {
                        /*by default, vgCPLSData_p->plmnSelector is set to User selector*/
                        /*However, this SIM does not support EF PLMN SEL/EF PLMN w ACT*/
                        result = VG_CME_SIM_FILE_NOT_PRESENT;
                    }
                    else if (vgCPLSData_p->plmnSelector != ABMM_USER_SELECTOR)
                    {
                        /* the Operator PLMN with ACT selector and the HPLMN selector
                         * cannot be modified by the user since they require the ADM code.
                         * Only the user selector can be modified by the user*/
                        result = VG_CME_OPERATION_NOT_ALLOWED;
                    }
                    else if ((index > 0) && (index <= ABMM_MAX_PREF_PLMNS))
                    {
                        index -= 1; /* Code index starts from 0 instead of 1 */
                    }
                    else
                    {
                        result = VG_CME_INVALID_INDEX;
                    }
                }

                if (result == RESULT_CODE_OK)
                {
                    /* See whether the 'format' has been entered */
                    if (getExtendedParameter (commandBuffer_p, &format, ULONG_MAX) == TRUE)
                    {
                        if (format != ULONG_MAX)
                        {
                            formatPresent = TRUE;
                        }
                    }
                    /* Check format is in range */
                    if (formatPresent == TRUE)
                    {
                        /* check format is in range */
                        if (((VgOpFormat)format != VG_OP_LONG)  &&
                            ((VgOpFormat)format != VG_OP_SHORT) &&
                            ((VgOpFormat)format != VG_OP_NUM))
                        {
                            result = VG_CME_INVALID_TEXT_CHARS;
                        }
                    }
                }

                if (result == RESULT_CODE_OK)
                {
                    /* now get the 'operator' */
                    if (getExtendedString ( commandBuffer_p,
                            plmnString,
                            PLMN_NAME_FULL_LENGTH,
                            &plmnStringLen) == TRUE)
                    {
                        if (plmnStringLen > 0)
                        {
                            if (format == VG_OP_NUM)
                            {
                                /* Plmn should be numeric (mcc,mnc). Convert from string to type 'Plmn' */
                                if ((vgOperStringToPlmn (plmnString,
                                         &convertedPlmn,
                                         &plmnStringLen)) == FALSE)
                                {
                                    result = VG_CME_CPOL_OPER_FORMAT_WRONG;
                                }
                                else
                                {
                                    if (plmnStringLen == MCC_DIGIT_LENGTH + MNC_DIGIT_LENGTH_MAX)
                                    {
                                        threeDigitMnc = TRUE;
                                    }
                                }
                            }
                            else if ((((VgOpFormat)format == VG_OP_LONG)  &&
                                      (plmnStringLen > PLMN_NAME_FULL_LENGTH))  ||
                                     (((VgOpFormat)format == VG_OP_SHORT) &&
                                      (plmnStringLen > PLMN_NAME_ABBR_LENGTH)))
                            {
                                /* Length too long */
                                result = VG_CME_CPOL_OPER_TOO_LONG;
                            }

                            /* <operator> is present and in correct format */
                            if (result == RESULT_CODE_OK)
                            {
                                operPresent = TRUE;
                            }
                        }
                    }

                }

                if ((operPresent) && (result == RESULT_CODE_OK))
                {
                    /* See whether the <GSM Act>, <GSM Compact Act>, and <Utran Act> parameters have been entered.
                     * These should only be present if <operator> has been specified */
                    /* Either all of them should be present or none of them*/

                    if ((result = checkActPresence (commandBuffer_p, &gsmAct)) == VG_CME_CPOL_ACT_MISSING)
                    {
                        result = RESULT_CODE_OK;
                    }
                    else if (result == RESULT_CODE_OK)
                    {
                        if (((result = checkActPresence (commandBuffer_p, &gsmCompactAct)) == RESULT_CODE_OK) &&
                            ((result = checkActPresence (commandBuffer_p, &utranAct)) == RESULT_CODE_OK) &&
                            ((result = checkActPresence (commandBuffer_p, &eutranAct)) == RESULT_CODE_OK))
                        {
                            if (simInfo->userPlmnSelector)
                            {
                                actPresent = TRUE;
                            }
                            else
                            {
                                /*access technology has been specified by the user
                                 *but this file on the SIM cannot store the access technology*/
                                result = VG_CME_CPOL_ACT_WRONG;
                            }
                        }
                    }
                }

                if (result == RESULT_CODE_OK)
                {
                    /* Determine ACTION to follow. Action depends upon
                     * which of the input parameters are present */
                    vgMmCPOLInitialiseData (FALSE); /* Initialise all data except 'readFormat' */

                    if ((indexPresent  == FALSE) &&
                        (formatPresent == TRUE) &&
                        (operPresent   == FALSE))
                    { /* Is user only setting format for read version of this command */
                        vgCPOLData_p->readFormat = (VgOpFormat)format;
                    }
                    else if ((indexPresent  == TRUE) &&
                             (formatPresent == FALSE) &&
                             (operPresent   == FALSE))
                    { /* User requires deletion of list entry */
                        vgCPOLData_p->action = PLMN_SEL_DELETE;
                        vgCPOLData_p->index  = (Int16)index;
                    }
                    else if ((indexPresent  == FALSE) &&
                             (formatPresent == TRUE) &&
                             (operPresent   == TRUE))
                    { /* User requires an entry to be appended */
                        vgCPOLData_p->action = PLMN_SEL_APPEND;
                    }
                    else if ((indexPresent  == TRUE) &&
                             (formatPresent == TRUE) &&
                             (operPresent   == TRUE))
                    { /* User requires an entry to be inserted */
                        vgCPOLData_p->action      = PLMN_SEL_INSERT;
                        vgCPOLData_p->index = (Int16)index;
                    }
                    else
                    { /* Invalid combination */
                        result = VG_CME_INVALID_TEXT_CHARS;
                    }
                }

                if (result == RESULT_CODE_OK)
                {
                    if (vgCPOLData_p->action == PLMN_SEL_APPEND ||
                        vgCPOLData_p->action == PLMN_SEL_INSERT)
                    {
                        /* Fill in appropriate plmn format */
                        if ((VgOpFormat)format == VG_OP_NUM)
                        {
                            vgCPOLData_p->plmnNumber        = convertedPlmn;
                            vgCPOLData_p->threeDigitMnc     = threeDigitMnc;
                            vgCPOLData_p->plmnNumberPresent = TRUE;
                            vgCPOLData_p->plmnNamePresent   = FALSE;
                            if (actPresent)
                            {
                                vgCPOLData_p->plmnNumber.accessTechnology = (AccessTechnologyId)
                                        ((gsmAct << VG_GSM_ACT_OFFSET) |
                                         (gsmCompactAct << VG_GSM_COMPACT_ACT_OFFSET) |
                                         (utranAct << VG_UTRAN_ACT_OFFSET) |
                                         (eutranAct << VG_EUTRAN_ACT_OFFSET));

                            }
                            else
                            {
                                vgCPOLData_p->plmnNumber.accessTechnology = 0;
                            }
                        }
                        else if ((VgOpFormat)format == VG_OP_SHORT)
                        {
                            memcpy (vgCPOLData_p->plmnName.abbr, plmnString, sizeof (vgCPOLData_p->plmnName.abbr));
                            vgCPOLData_p->plmnNumberPresent = FALSE;
                            vgCPOLData_p->plmnNamePresent   = TRUE;
                        }
                        else if ((VgOpFormat)format == VG_OP_LONG)
                        {
                            memcpy (vgCPOLData_p->plmnName.full, plmnString, sizeof (vgCPOLData_p->plmnName.full));
                            vgCPOLData_p->plmnNumberPresent = FALSE;
                            vgCPOLData_p->plmnNamePresent   = TRUE;
                        }
                        else
                        {
                            vgCPOLData_p->plmnNumberPresent = FALSE;
                            vgCPOLData_p->plmnNamePresent   = FALSE;
                        }
                    }

                    if (vgCPOLData_p->action == PLMN_SEL_APPEND ||
                        vgCPOLData_p->action == PLMN_SEL_INSERT ||
                        vgCPOLData_p->action == PLMN_SEL_DELETE)
                    {
                        if (simLockGenericContext_p->simState == VG_SIM_READY)
                        {
                            /* SIM is ready, send write request off */
    #if defined(UPGRADE_3G)
                            /*force change control*/
                            chManagerContext_p->isImmediate = TRUE;
    # if defined(USE_ABAPP)
                            mobilityContext_p->isImmediate  = chManagerContext_p->isImmediate;
    # endif
    #endif
                            result = vgChManContinueAction (entity, SIG_APEX_MM_WRITE_PLMN_SEL_REQ);
                        }
                        else
                        {
                            /* get SIM state error */
                            result = vgGetSimCmeErrorCode ();
                        }
                    }
                }
            }
            else
            {
                /* get SIM state error */
                result = vgGetSimCmeErrorCode ();
            }
            break;
        }
        case EXTENDED_QUERY: /* +CPOL? */
        {
            if (simLockGenericContext_p->simState == VG_SIM_READY)
            {
                /* SIM is ready, get list of valid operators */
                vgCPOLData_p->firstPlmn = TRUE;

                if ((vgCPLSData_p->plmnSelector == ABMM_USER_SELECTOR) &&
                    (!simInfo->plmnSelector) &&
                    (!simInfo->userPlmnSelector))
                {
                    /*by default, vgCPLSData_p->plmnSelector is set to user selector*/
                    /*However, this SIM does not support EF PLMN SEL/EF PLMN w ACT*/
                    result = VG_CME_SIM_FILE_NOT_PRESENT;
                }
                else
                {
                    result = RESULT_CODE_PROCEEDING;
                    /*No need to request control of MM procedure for this signal*/
                    vgSigApexMmReadPlmnSelReq (0,
                        PLMN_READ_LIMIT,
                        vgCPLSData_p->plmnSelector,
                        entity);
                }
            }
            else
            {
                /* get SIM state error */
                result = vgGetSimCmeErrorCode ();
            }
            break;
        }
        case EXTENDED_ACTION: /* AT+CPOL  */
        default:
        {
            result = RESULT_CODE_ERROR;
            break;
        }
    } /* switch */

    return (result);
} /* vgMmCPOL */

/*--------------------------------------------------------------------------
*
* Function:    vgMmCOPS
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This function executes the AT+COPS command, this allows a
*              valid network operator to be selected.
*
*-------------------------------------------------------------------------*/

ResultCode_t vgMmCOPS ( CommandLine_t            *commandBuffer_p,
                        const VgmuxChannelNumber entity)
{
    ResultCode_t result           = RESULT_CODE_OK;
    ExtendedOperation_t operation = getExtendedOperation (commandBuffer_p);
    Int32 val1, val2;
    Char networkOperator[PLMN_NAME_FULL_LENGTH + NULL_TERMINATOR_LENGTH] = {0};
    Int16 length;
    Boolean accepted;
    VgCOPSData oldCOPSData;
    MobilityContext_t       *mobilityContext_p        = ptrToMobilityContext ();
    VgCOPSData              *vgCOPSData               = &(mobilityContext_p->vgCOPSData);
    VgCREGData              *vgCREGData               = &(mobilityContext_p->vgCREGData);
    ChManagerContext_t      *chManagerContext_p       = ptrToChManagerContext ();
    SimLockGenericContext_t *simLockGenericContext_p  = ptrToSimLockGenericContext ();
    GprsGenericContext_t    *gprsGenericContext_p     = ptrToGprsGenericContext ();
    VgCEREGData             *vgCEREGData              = &(gprsGenericContext_p->vgCEREGData);

    switch (operation)
    {
        case EXTENDED_RANGE: /* AT+COPS=? */
        {
            /* request a list of valid operators which may be selected */
            vgCOPSData->state = VG_COPS_LIST;
            chManagerContext_p->isImmediate = TRUE;
#if defined(USE_ABAPP)
            mobilityContext_p->isImmediate  = chManagerContext_p->isImmediate;
#endif
            result = vgChManContinueAction (entity, SIG_APEX_MM_PLMNLIST_REQ);
            break;
        }
        case EXTENDED_QUERY: /* AT+COPS? */
        {
            /* if registered successfully with a network operator display details */
            if (((mobilityContext_p->vgNetworkState.serviceStatus == PLMN_NORMAL) ||
                (mobilityContext_p->vgNetworkState.serviceStatus == PLMN_ACCESS_DIFFICULTY))
                &&(VGMNL_NOT_REGISTRATED!=vgCEREGData->regStatus)&&(VGMNL_SEARCHING!=vgCEREGData->regStatus))
            {
                vgPutNewLine (entity);
                {
                    vgPrintf (entity,
                        (const Char *)"+COPS: %d,%d,",
                        vgCOPSData->mode,
                        vgCOPSData->format);

                    /* depending on format setting display the current operator details */
                    switch (vgCOPSData->format)
                    {
                        case VG_OP_LONG:
                        { /* Long alphabetic name */
                            vgPutc (entity, '\"');
                            if ( mobilityContext_p->vgNetworkState.name.plmnCoding == PLMN_CODING_DEFAULT)
                            {
                                vgPrintf (entity,
                                    (const Char *)"%s",
                                    mobilityContext_p->vgNetworkState.name.full);
                            }
                            vgPutc (entity, '\"');
                            break;
                        }
                        case VG_OP_SHORT:
                        { /* Short alphabetic name */
                            vgPutc(entity, '\"');
                            if ( mobilityContext_p->vgNetworkState.name.plmnCoding == PLMN_CODING_DEFAULT)
                            {
                                vgPrintf (entity,
                                    (const Char *)"%s",
                                    mobilityContext_p->vgNetworkState.name.abbr);
                            }
                            vgPutc (entity, '\"');
                            break;
                        }
                        case VG_OP_NUM:
                        { /* MCC & MNC operator code */
                          /* check if MNC is 2 or 3 digits long */
                            if (mobilityContext_p->vgNetworkState.threeDigitMnc)
                            {
                                vgPrintf (entity,
                                    (const Char *)"\"%03x%03x\"",
                                    mobilityContext_p->vgNetworkState.plmn.mcc,
                                    mobilityContext_p->vgNetworkState.plmn.mnc);
                            }
                            else
                            {
                                vgPrintf (entity,
                                    (const Char *)"\"%03x%02x\"",
                                    mobilityContext_p->vgNetworkState.plmn.mcc,
                                    mobilityContext_p->vgNetworkState.plmn.mnc);
                            }
                            break;
                        }
                        default:
                        {
                            /* invalid format encountered */
                            FatalParam (vgCOPSData->format, vgCOPSData->mode, 0);
 #if defined (ATCI_SLIM_DISABLE)                 
                            /* switch to default format */
                            mobilityContext_p->vgCOPSData.format = VG_OP_NUM;
                            /* ensure a valid response is returned */
                            vgPutc (entity, '\"');
                            vgPutc (entity, '\"');
 #endif
                            break;
                        }
                    } /* switch */

                    vgPrintf (entity, (const Char *)",9");/* NB-IOT Access tech */
                    vgPutNewLine (entity);
                }
            }
            else
            {
                /* if not registered with a network operator just display operator
                 * selection mode */
                vgPutNewLine (entity);
                vgPrintf (entity, (const Char *)"+COPS: %d", vgCOPSData->mode);
                vgPutNewLine (entity);
            }
            break;
        }
        case EXTENDED_ASSIGN: /* AT+COPS=... */
        {
#if defined(UPGRADE_3G)
            vgCOPSData->returnToRplmn = FALSE;
#endif
            {
                accepted = FALSE;

                /* copy current COPS settings */
                memcpy (&oldCOPSData, vgCOPSData, sizeof(VgCOPSData));

                /* get operator mode */
                if (getExtendedParameter (commandBuffer_p, &val1, NUM_OF_VG_OP_MODES) == TRUE)
                {
                    if ((VgOpMode)val1 < NUM_OF_VG_OP_MODES)
                    {

                        vgCOPSData->mode = (VgOpMode)val1;

                        /* get operator format */
                        if (getExtendedParameter (commandBuffer_p, &val2, NUM_OF_VG_OPS) == TRUE)
                        {
                            if ((VgOpFormat)val2 == VG_OP_NUM)
                            {
                                vgCOPSData->format = (VgOpFormat)val2;

                                /* get operator details if required */
                                if ((vgCOPSData->mode == VG_OP_MANUAL_OPERATOR_SELECTION) ||
                                    (vgCOPSData->mode == VG_OP_MANUAL_THEN_AUTOMATIC))
                                {

                                    if (getExtendedString (commandBuffer_p,
                                            networkOperator,
                                            PLMN_NAME_FULL_LENGTH,
                                            &length) == TRUE)
                                    {
                                        /* check string is ok considering the given format */
                                       if(6==length)
                                       {
                                           vgCOPSData->threeDigitMnc = TRUE;                                       
                                       }
                                       else
                                       {
                                           vgCOPSData->threeDigitMnc = FALSE;
                                       }
                                        accepted = checkOperator (networkOperator, length);
                                    }
                                }
                                else
                                {
                                    /* automatic mode, manual deregister and set format do not
                                     * require a network operator string */
                                    if((vgCOPSData->mode == VG_OP_AUTOMATIC_MODE) ||
                                        (vgCOPSData->mode == VG_OP_MANUAL_DEREGISTER))
                                    {
                                         accepted = FALSE;
                                    }
                                    else
                                    {
                                        accepted = TRUE;
                                    }
                                }
                            }
                            /* long and short format is not allowed, for now do not support string PLMN */
                            else if(((VgOpFormat)val2 > NUM_OF_VG_OPS) ||
                                      ((VgOpFormat)val2 == VG_OP_LONG) || ((VgOpFormat)val2 == VG_OP_SHORT))
                            {
                                accepted = FALSE;
                            }
                            else
                            {
                                /* automatic mode and manual deregister don't require a format */
                                if ((vgCOPSData->mode == VG_OP_AUTOMATIC_MODE) ||
                                    (vgCOPSData->mode == VG_OP_MANUAL_DEREGISTER))
                                {
                                    accepted = TRUE;
                                }
                            }
                        }
                    }
                }
                if (accepted == FALSE)
                {
                    /* parameters were not accepted, reset COPS data */
                    memcpy (vgCOPSData, &oldCOPSData, sizeof(VgCOPSData));

                    result = RESULT_CODE_ERROR;
                }
                else
                {
                    switch (vgCOPSData->mode)
                    {
                        case VG_OP_SET_FORMAT:
                        {
                            /* nothing more to do other than reset mode since we've already set format */
                            vgCOPSData->mode = oldCOPSData.mode;
                            break;
                        }
                        case VG_OP_MANUAL_DEREGISTER:
                        {
                            if((vgCREGData->state == VGMNL_SEARCHING)
                                ||(vgCREGData->state == VGMNL_NOT_REGISTRATED)
                                ||(vgCREGData->state == VGMNL_REGISTRATION_DENIED))
                            {
                                 memcpy (vgCOPSData, &oldCOPSData, sizeof(VgCOPSData));
                                 result= RESULT_CODE_ERROR;                           
                            }
                            else
                            {
                            /*force change control*/
                                chManagerContext_p->isImmediate = TRUE;
#if defined(USE_ABAPP)
                                mobilityContext_p->isImmediate  = chManagerContext_p->isImmediate;
#endif
                                result = vgChManContinueAction (entity,
                                    SIG_APEX_MM_DEREGISTER_REQ);
                        
                            }
                            break;
                        }
                        case VG_OP_AUTOMATIC_MODE:
                        {
                            /* if we are selecting in automatic mode already then return OK.
                             * Note that if it is already registered on a network in automatic mode it forces another
                             * search in case, for example, on roaming case, it picks another network.
                             * This can take over 20 seconds on a 3G/2G network.
                             */
                            if (((oldCOPSData.mode == VG_OP_AUTOMATIC_MODE) &&
                                      (vgCREGData->state == VGMNL_SEARCHING))  ||
                                     (vgCOPSData->autoSelectInProgress == TRUE))
                            {
                                result = RESULT_CODE_OK;
                            }
                            else
                            {
                                /*force change control*/
                                chManagerContext_p->isImmediate = TRUE;
#if defined(USE_ABAPP)
                                mobilityContext_p->isImmediate  = chManagerContext_p->isImmediate;
#endif

                                /* automatic mode, send request off to select operator */
                                result =  vgChManContinueAction (entity, SIG_APEX_MM_PLMN_SELECT_REQ);
                            }
                            break;
                        }
                        case VG_OP_MANUAL_OPERATOR_SELECTION:
                        case VG_OP_MANUAL_THEN_AUTOMATIC:
                        {
                            /* job119076: set vgCOPSData->state later */
                            /* job 117612: If Manual PLMN selection is requested with numeric PLMN values
                               defined, proceed directly to PLMN selection */
                            if (vgCOPSData->format == VG_OP_NUM)
                            {

                                /* job 117612: Request immediate control of the module for
                                   current internal PLMN search to be aborted */
                                chManagerContext_p->isImmediate = TRUE;
#if defined(USE_ABAPP)
                                mobilityContext_p->isImmediate  = chManagerContext_p->isImmediate;
#endif

                                result =  vgChManContinueAction (entity, SIG_APEX_MM_PLMN_SELECT_REQ);

                            }
                            else
                            {
                                /* get list of valid operators which can be selected */
                                result = vgChManContinueAction (entity,
                                    SIG_APEX_MM_PLMNLIST_REQ);

                                /* Job 113053 If CREG enabled then indicate ME is searching */
                                if (mobilityContext_p->vgNetworkState.isSelecting)
                                {
                                    VgmuxChannelNumber profileEntity = VGMUX_CHANNEL_INVALID;

                                    vgCREGData->state = VGMNL_SEARCHING;

                                    for (profileEntity = 0; profileEntity < CI_MAX_ENTITIES; profileEntity++)
                                    {
                                        if (isEntityActive (profileEntity))
                                        {
                                            Int8 profileValue = getProfileValue (profileEntity, PROF_CREG);

                                            if ((profileValue == VG_CREG_ENABLED) ||
                                                (profileValue == VG_CREG_ENABLED_WITH_LOCATION_INFO))
                                            {
                                                viewCREG (profileEntity, FALSE);
                                            }
                                        }
                                    }
                                }
                            }

                            /* job119076: only change state from default if command proceeding */
                            if (result == RESULT_CODE_PROCEEDING)
                            {
                                vgCOPSData->state = VG_COPS_MANUAL;
                            }
                            break;
                        }
                        default:
                        {
                            /* invalid mode encountered */
                            FatalParam (vgCOPSData->mode, vgCOPSData->format, 0);
                            /* switch mode back to default */
                           // vgCOPSData->mode = VG_OP_AUTOMATIC_MODE;
                            /* indicate an error has ocurred */
                            //result           = RESULT_CODE_ERROR;
                            break;
                        }
                    } /* switch */
                }
                break;
            }
        }
        case EXTENDED_ACTION: /* AT+COPS... */
        default:
        {
            result = RESULT_CODE_ERROR;
            break;
        }
    } /* switch */

    return (result);
} /* vgMmCOPS */
/*--------------------------------------------------------------------------
*
* Function:    calcCESQLevel
*
* Parameters:  none
*
* Returns:     Nothing
*
* Description: Converts the received signal value to the value returned by
*              *MSQN
*
*-------------------------------------------------------------------------*/
static Int8 calcCESQLevel (void)
{
    int16_t signalLevel;
    MobilityContext_t   *mobilityContext_p = ptrToMobilityContext ();

    /* access technology must be  EUTRAN_ACCESS_TECHNOLOGY for NB-IOT */

    /* For LTE - the RXLEV reported in the MmrRssiInd is a mapping of
      * the dBm levels -110 to -48 to 0 to 63.  So we need to map that back
      * to sensible levels in ATCI which match what AT+CESQ expects.
      */
    signalLevel = (mobilityContext_p->receiveLevel  - VG_CESQ_MIN_RXLEV_DB);
	
    /* crop at upper limit */
    if (signalLevel > VG_CESQ_MAX_RXLEV)
    {
        signalLevel = VG_CESQ_MAX_RXLEV;
    }
    else if (signalLevel < VG_CESQ_MIN_RXLEV)
    {
        signalLevel = VG_CESQ_MIN_RXLEV;	
    }
    return (Int8) signalLevel;
} /* calcCESQLevel */

/*--------------------------------------------------------------------------
*
* Function:    calcCESQRsrqLevel
*
* Parameters:  none
*
* Returns:     Nothing
*
* Description: Converts the received RSRQ level in to the value to be displayed
*              by AT+CESQ command.
*
*-------------------------------------------------------------------------*/
static Int8 calcCESQRsrqLevel(void)
{
    int16_t               rsrqLevel;
    MobilityContext_t   *mobilityContext_p = ptrToMobilityContext ();

    /* First convert to dBm starting at 0 = -40 dBm and 34 = 0dBm */

    rsrqLevel = (int16_t)(mobilityContext_p->eutraRsrq  - VG_CESQ_EUTRAN_RSRQ_MIN_DB) * 2;
	 
    if (rsrqLevel > VG_CESQ_MAX_RSRQ)
    {
      rsrqLevel = VG_CESQ_MAX_RSRQ;
    }
    else if (rsrqLevel < VG_CESQ_MIN_RSRQ)
    {
      rsrqLevel = VG_CESQ_MIN_RSRQ;
    }
    return ((Int8) rsrqLevel);
}

/*--------------------------------------------------------------------------
*
* Function:    calcRsrpLevel
*
* Parameters:  none
*
* Returns:     Nothing
*
* Description: Converts the received RSRP level in to the value to be displayed
*              by AT+CESQ command.
*
*-------------------------------------------------------------------------*/
static Int8 calcCESQRsrpLevel(void)
{
    int16_t               rsrpLevel;
    MobilityContext_t   *mobilityContext_p = ptrToMobilityContext ();



    /* First convert to dBm starting at 0 = -140dBm and 97 = -40dBm */
    rsrpLevel = (mobilityContext_p->eutraRsrp - VG_CESQ_EUTRAN_RSRP_MIN_DB);
    

    if (rsrpLevel > VG_CESQ_MAX_RSRP)
    {
      rsrpLevel = VG_CESQ_MAX_RSRP;
    }
    else if(rsrpLevel < VG_CESQ_MIN_RSRP)
    {
	  rsrpLevel = VG_CESQ_MIN_RSRP;	
    }

    return ((Int8) rsrpLevel);
}

/*--------------------------------------------------------------------------
*
* Function:    calcCSQRssiLevel
*
* Parameters:  none
*
* Returns:     Nothing
*
* Description: Converts the received RSSI level in to the value to be displayed
*              by AT+CSQ command.
*
*-------------------------------------------------------------------------*/
static Int8 calcCSQRssiLevel(void)
{
    int16_t signalLevel;
    MobilityContext_t   *mobilityContext_p = ptrToMobilityContext ();

    /* access technology must be  EUTRAN_ACCESS_TECHNOLOGY for NB-IOT */

    /* the RSSI reported in the MmrRssiInd is a mapping of
      * the dBm levels -113 to -51 to 0 to 31.  So we need to map that back
      * to sensible levels in ATCI which match what AT+CSQ expects.
      */
    signalLevel = (mobilityContext_p->receiveLevel  - VG_CSQ_MIN_RSSI_DB) / 2;
    
    /* crop at upper limit */
    if (signalLevel > VG_CSQ_MAX_RSSI)
    {
        signalLevel = VG_CSQ_MAX_RSSI;
    }
    else if (signalLevel < VG_CSQ_MIN_RSSI)
    {
        signalLevel = VG_CSQ_MIN_RSSI;
    }
    return (Int8) signalLevel;
}

/*--------------------------------------------------------------------------
*
* Function:    calcCSQRxQualLevel
*
* Parameters:  none
*
* Returns:     Nothing
*
* Description: Converts the received BER level in to the value to be displayed
*              by AT+CSQ command.
*
*-------------------------------------------------------------------------*/
static Int8 calcCSQRxQualLevel(void)
{
    double rxqualLevel;
    MobilityContext_t   *mobilityContext_p = ptrToMobilityContext ();

    /* access technology must be  EUTRAN_ACCESS_TECHNOLOGY for NB-IOT */

    /* the BER reported in the MmrRssiInd is a mapping of
      * the RxQual levels.  So we need to map that back
      * to sensible levels in ATCI which match what AT+CSQ expects.
      */
    if(VG_CESQ_MAX_BER_PERCENT < mobilityContext_p->vgdlber)
    {
        rxqualLevel = VG_CESQ_MAX_BER;
    }
    else if(VG_CESQ_MIN_BER_PERCENT > mobilityContext_p->vgdlber)
    {
        rxqualLevel = VG_CESQ_MIN_BER;
    }
    else
    {
        rxqualLevel = (log((double)mobilityContext_p->vgdlber))/(log(2.0));
    }
    
    return (Int8)rxqualLevel;
}


/*--------------------------------------------------------------------------
*
* Function:    displayCESQLevel
*
* Parameters:  entity  - mux channel number
*              atQuery - type of view
*              level   - signal level to display
*
* Returns:     Nothing
*
* Description: Sends current signal stength to the entity specified
*
*-------------------------------------------------------------------------*/
void displayCESQLevel (const VgmuxChannelNumber entity,
                     const CesqReason_t        reason,
                     Int8                     rxLev,
                     Int16                    rsrq,
                     Int16                    rsrp)
{
    EntityMobilityContext_t *mobilityContext_p  = ptrToEntityMobilityContext (entity);
    Int16              ber;
    Int16              rscp;
    Int16              ecno;

 if (reason == CESQ_QUERY_COMMAND)
 {
     /* only rxlevel, RSRQ and RSRP valid  */

     ber            = calcCSQRxQualLevel();
     rscp           = VG_CESQ_INVALID_RSCP;
     ecno           = VG_CESQ_INVALID_ECNO;
     
     mobilityContext_p->msqnState.lastSentLevel = rxLev;
     mobilityContext_p->msqnState.lastSentRsrq  = rsrq;
     mobilityContext_p->msqnState.lastSentRsrp  = rsrp;


     vgPutNewLine (entity);
     vgPrintf (entity, (const Char *)"+CESQ: ");
     vgPrintf (entity,
               (const Char *)"%d,%d,%d,%d,%d,%d",
               rxLev, ber, rscp, ecno, rsrq,rsrp);
      vgPutNewLine (entity);
 }

} /* displayCESQLevel */

void displayMSQNLevel   (const VgmuxChannelNumber entity,
                         const CesqReason_t reason,
                         int16_t  rxLev,
                         int8_t rsrq,
                         int16_t rsrp)
{

    if (reason == CESQ_MSQN_UNSOLICITED)
    {
        vgPutNewLine (entity);
        vgPrintf (entity, (const Char *)"*MSQN: ");
        vgPrintf (entity,
                  (const Char *)"%d,%d,%d",
                  rxLev,
                  rsrq,
                  rsrp);
        vgPutNewLine (entity);

    }


}

/*--------------------------------------------------------------------------
*
* Function:    displayCESQ
*
* Parameters:  entity                    mux channel number
*                    CesqReason_t        reason
*
* Returns:     Nothing
*
* Description: Displays CESQ information
*
*-------------------------------------------------------------------------*/
static void displayCESQ (const VgmuxChannelNumber entity,
                        const CesqReason_t        reason)
{
    MobilityContext_t   *mobilityContext_p = ptrToMobilityContext ();

    if (mobilityContext_p->haveReceiveInfo == TRUE)
    {
        displayCESQLevel (entity, reason, calcCESQLevel (),calcCESQRsrqLevel(), calcCESQRsrpLevel());
    }
    else
    {
        /* Nothing to show */

        vgPutNewLine (entity);

        if ( reason == CESQ_QUERY_COMMAND )
        {
            vgPrintf (entity, (const Char *)"+CESQ: %d,%d,%d,%d,%d,%d",
                      VG_CESQ_INVALID_RXLEV,
                      VG_CESQ_INVALID_BER,
                      VG_CESQ_INVALID_ECNO,
                      VG_CESQ_INVALID_RSCP,
                      VG_CESQ_INVALID_RSRQ,
                      VG_CESQ_INVALID_RSRP);
        }
        else
        {
            vgPrintf (entity, (const Char *)"*MSQN: %d,%d,%d",
                      VG_CESQ_INVALID_RXLEV,
                      VG_CESQ_INVALID_RSRQ,
                      VG_CESQ_INVALID_RSRP);
        }

        vgPutNewLine (entity);
    }
} /* displayCESQ */

/*--------------------------------------------------------------------------
*
* Function:    displayCSQ
*
* Parameters:  entity                    mux channel number
*
* Returns:     Nothing
*
* Description: Displays CSQ information
*
*-------------------------------------------------------------------------*/
static void displayCSQ (const VgmuxChannelNumber entity)
{
    MobilityContext_t   *mobilityContext_p = ptrToMobilityContext ();

    if (mobilityContext_p->haveReceiveInfo == TRUE)
    {
        vgPutNewLine (entity);
        vgPrintf (entity, (const Char *)"+CSQ: ");
        vgPrintf (entity,
                  (const Char *)"%d,%d",
                  calcCSQRssiLevel(), calcCSQRxQualLevel());
         vgPutNewLine (entity);
    }
    else
    {
        /* Nothing to show */
        vgPutNewLine (entity);

        vgPrintf (entity, (const Char *)"+CSQ: %d,%d",
                  VG_CSQ_INVALID_RSSI,
                  VG_CESQ_INVALID_BER);
        vgPutNewLine (entity);
    }
} /* displayCESQ */

int getCSQ (void)
{
	return calcCSQRssiLevel();
}


/*--------------------------------------------------------------------------
*
* Function:    viewCESQ
*
* Parameters:  entity  - mux channel number
*              atQuery - type of view
*
* Returns:     Nothing
*
* Description: Sends current signal stength to the entity specified
*
*-------------------------------------------------------------------------*/

void viewCESQ (const VgmuxChannelNumber entity,
              const CesqReason_t        reason)
{
    EntityMobilityContext_t *mobilityContext_p  = ptrToEntityMobilityContext (entity);
    MobilityContext_t *genericMobilityContext_p = ptrToMobilityContext ();
    int16_t             level;
    int8_t              rsrq;
    int16_t             rsrp;

    
    switch ( reason)
    {
 
        case CESQ_QUERY_COMMAND:
        {
            displayCESQ(entity, reason);
        }
        break;

        case CESQ_MSQN_UNSOLICITED:
        {
            /* display the first one anyway */

            level = genericMobilityContext_p->receiveLevel;
            rsrq = genericMobilityContext_p->eutraRsrq;
            rsrp = genericMobilityContext_p->eutraRsrp;
            /* no valid data yet */
            if ( ((mobilityContext_p->msqnState.lastSentLevel == VG_CESQ_INVALID_RXLEV)&&
              (mobilityContext_p->msqnState.lastSentRsrq == VG_CESQ_INVALID_RSRQ) &&
              (mobilityContext_p->msqnState.lastSentRsrp == VG_CESQ_INVALID_RSRP))
              && 
              ((level != VG_CESQ_INVALID_RXLEV) ||
                (rsrq != VG_CESQ_INVALID_RSRQ) || (rsrp != VG_CESQ_INVALID_RSRP))) 
              {
                /* the first one... */
                
                displayMSQNLevel(entity, reason, level,rsrq, rsrp);
            }
            else if ((genericMobilityContext_p->haveReceiveInfo == TRUE) 
                      && 
                    ((mobilityContext_p->msqnState.lastSentLevel != level) ||
                     (mobilityContext_p->msqnState.lastSentRsrq != rsrq) ||
                     (mobilityContext_p->msqnState.lastSentRsrp != rsrp)))
            {
                displayMSQNLevel(entity, reason, level, rsrq, rsrp);
 
            }
          break;  
          }

        default:
          break;
      }
} /* viewCESQ */

/*--------------------------------------------------------------------------
*
* Function:    vgMmSYSCONFIG
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: executes AT^SYSCONFIG command
*
*
*-------------------------------------------------------------------------*/
ResultCode_t vgMmSYSCONFIG (CommandLine_t            *commandBuffer_p,
                            const VgmuxChannelNumber entity)
{
  ResultCode_t            result             = RESULT_CODE_OK;
  ExtendedOperation_t     operation          = getExtendedOperation (commandBuffer_p);
  MobilityContext_t      *mobilityContext_p  = ptrToMobilityContext ();
  ChManagerContext_t     *chManagerContext_p = ptrToChManagerContext ();
  VgSYSCONFIGData        *vgSYSCONFIGData_p  = &mobilityContext_p->vgSYSCONFIGData;
  VgSYSCONFIGParam       *newParam_p         = &(vgSYSCONFIGData_p->newParam);
  Int8                    paramIndex;
  Int32                   paramValue;

  /* this function will need modification when the specification for ^SYSCONFIG for NB-IOT is made available (CMCC specific) */
  switch (operation)
  {
    case EXTENDED_QUERY:   /* AT^SYSCONFIG? */
    {
      vgSYSCONFIGData_p->read           = TRUE;
      chManagerContext_p->isImmediate   = TRUE;
#if defined(USE_ABAPP)
      mobilityContext_p->isImmediate    = chManagerContext_p->isImmediate;
#endif
      result = vgChManContinueAction (entity, SIG_APEX_MM_READ_BAND_MODE_REQ);
      break;
    }

    case EXTENDED_RANGE:   /* AT^SYSCONFIG=? */
    {
      vgPutNewLine (entity);
      vgPrintf(entity, (const Char *)"^SYSCONFIG: (2,16),(0),(0-2),(1,3,4)"); /* not valid for NB-IOT */
      vgPutNewLine (entity);
      setResultCode (entity, RESULT_CODE_OK);

      break;
    }

    case EXTENDED_ASSIGN:  /* AT^SYSCONFIG=... */
    {
      vgSYSCONFIGData_p->read = FALSE;

      /* Get input parameters*/
      for(paramIndex=0;paramIndex<VG_MM_SYSCONFIG_NUM;paramIndex++)
      {
        if( getExtendedParameter(commandBuffer_p, &paramValue, ULONG_MAX) )
        {
          switch(paramIndex)
          {
            /*system mode*/
            case VG_MM_SYSCONFIG_MODE:
            {
              if((paramValue != SYSCONFIG_MODE_AUTO) &&
                 (paramValue != SYSCONFIG_MODE_NO_MODIFICATION))
              {
                result = VG_CMS_ERROR_INVALID_PARMETER;
              }
              break;
            }

            case VG_MM_ACQORDER:
            {
              if(paramValue != ACCESS_AUTO)
              {
                result = VG_CMS_ERROR_INVALID_PARMETER;
              } 
              break;            
            }

            /*roam*/
            case VG_MM_SYSCONFIG_ROAM:
            {
              if(paramValue>ROAMING_NO_MODIFICATION)
              {
                result = VG_CMS_ERROR_INVALID_PARMETER;
              }
              else
              {
                newParam_p->roamSupport = (VgRoamingSupport)paramValue;
              }
              break;
            }
            /*domain setting*/
            case VG_MM_SYSCONFIG_SRVDOMAIN:
            {
              /* TODO: Not applicable to NB-IOT */
              switch (paramValue)
              {
                case DOMAIN_PS_ONLY:
                case DOMAIN_ANY:
                case DOMAIN_NO_MODIFICATION:
                {
                  newParam_p->srvDomain = (VgDomainSetting)paramValue;
                  break;
                }
                default:
                {
                  result = VG_CMS_ERROR_INVALID_PARMETER;
                  break;
                }
              }
              break;
            }

            default:
              break;
          }  /*switch*/

          if(result!=RESULT_CODE_OK)
          {
            /*leave the for loop*/
            break;
          }
        }
        else
        {
          result = VG_CMS_ERROR_INVALID_PARMETER;

          /*leave the for loop*/
          break;
        }

      } /*for loop*/

      if(result==RESULT_CODE_OK)
      {
        /*force change control*/
        chManagerContext_p->isImmediate = TRUE;
#if defined(USE_ABAPP)
        mobilityContext_p->isImmediate  = chManagerContext_p->isImmediate;
#endif
        result = vgChManContinueAction (entity, SIG_APEX_MM_READ_BAND_MODE_REQ);
      }

     break;
    }

    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }

  } /* switch */

  return (result);

} /* vgMmSYSCONFIG */



/*--------------------------------------------------------------------------
*
* Function:    vgMmCREG
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: executes AT+CREG command (network registration)
*              + displays current bearer service type
*              + ranges of parameters
*              + modify the configuration
*
*-------------------------------------------------------------------------*/

ResultCode_t vgMmCREG (CommandLine_t            *commandBuffer_p,
                       const VgmuxChannelNumber entity)
{
    ResultCode_t result           = RESULT_CODE_OK;
    ExtendedOperation_t operation = getExtendedOperation (commandBuffer_p);
    Int32 param;

    switch (operation)
    {
        case EXTENDED_QUERY:   /* AT+CREG? */
        {
            viewCREG (entity, TRUE);
            break;
        }
        case EXTENDED_RANGE:   /* AT+CREG=? */
        {
            vgPutNewLine (entity);
            vgPuts (entity, (const Char *)"+CREG: (0-2)");
            break;
        }
        case EXTENDED_ASSIGN:  /* AT+CREG= */
        {
            if (getExtendedParameter (commandBuffer_p,
                    &param,
                    VG_CREG_NUMBER_OF_SETTINGS) == TRUE)
            {
                if (param < VG_CREG_NUMBER_OF_SETTINGS)
                {
                    result = setProfileValue (entity, PROF_CREG, (Int8)param);
#if 0
                /* For AP do not send this */
                    if(param != VG_CREG_DISABLED)
                        viewCREG (entity, FALSE);
#endif
                }
                else
                {
                    result = RESULT_CODE_ERROR;
                }
            }
            else
            {
                result = RESULT_CODE_ERROR;
            }
            break;
        }
        case EXTENDED_ACTION:  /* AT+CREG */
        default:
        {
            result = RESULT_CODE_ERROR;
            break;
        }
    } /* switch */
    return (result);
} /* vgMmCREG */

/*--------------------------------------------------------------------------
*
* Function:    vgMmGATTCFG
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: executes AT*MGATTCFG command (setting GPRS Attach on Power On option)
*-------------------------------------------------------------------------*/

ResultCode_t vgMmGATTCFG (CommandLine_t            *commandBuffer_p,
                         const VgmuxChannelNumber entity)
{
    ResultCode_t result                    = RESULT_CODE_OK;
    ExtendedOperation_t operation          = getExtendedOperation (commandBuffer_p);
    MobilityContext_t   *mobilityContext_p = ptrToMobilityContext ();
    Int32 param;

    switch (operation)
    {
        case EXTENDED_QUERY:   /* AT*MGATTCFG? */
        {
            result = vgChManContinueAction (entity, SIG_APEX_MM_READ_PWON_OPTIONS_REQ);
            break;
        }
        case EXTENDED_RANGE:   /* AT*MGATTCFG=? */
        {
            vgPutNewLine (entity);
            vgPuts (entity, (const Char *)"*MGATTCFG: (0-3)");
            break;
        }
        case EXTENDED_ASSIGN:  /* AT*MGATTCFG= */
        {
            if (getExtendedParameter (commandBuffer_p,
                    &param,
                    ABMM_GAPWON_MAX_OPTIONS) == TRUE)
            {
                if (param < ABMM_GAPWON_MAX_OPTIONS)
                {
                    mobilityContext_p->gaOption = (ApexMmGAPowerUpOption) param;
                    result =  vgChManContinueAction (entity, SIG_APEX_MM_WRITE_PWON_OPTIONS_REQ);
                }
                else
                {
                    result = RESULT_CODE_ERROR;
                }
            }
            else
            {
                result = RESULT_CODE_ERROR;
            }
            break;
        }
        case EXTENDED_ACTION:  /* AT*MGATTCFG */
        default:
        {
            result = RESULT_CODE_ERROR;
            break;
        }
    } /* switch */
    return (result);
} /* vgMmGATTCFG */

#if defined (UPGRADE_SHARE_MEMORY) || defined(UPGRADE_SHMCL_SOLUTION)
/*--------------------------------------------------------------------------
*
* Function:    writeCregDataToShareMemory
*
* Parameters:  none
*
* Returns:     Nothing
*
* Description: place current status of network registration to share memory
*              and record it locally
*
*-------------------------------------------------------------------------*/
void writeCregDataToShareMemory (void)
{
    AtDataType        atDataType;
    Boolean           showRegInfo          = FALSE;
    MobilityContext_t *mobilityContext_p = ptrToMobilityContext ();
    VgCREGData        *vgCREGData        = &mobilityContext_p->vgCREGData;
    VgCOPSData        *vgCOPSData        = &mobilityContext_p->vgCOPSData;
    Int8              accessTechnology   = 0;
    AccessTechnologyId plmnAccessTechnology = vgCOPSData->selectedPlmn.plmn.accessTechnology;

    /* if registered, lac and cellId are also appended to string */
    if (((vgCREGData->state == VGMNL_REGISTRATED_HOME)
         ||(vgCREGData->state == VGMNL_REGISTRATED_ROAMING)
         || (mnState == VGMNL_REG_HOME_SMS_ONLY)
         || (mnState == VGMNL_REG_ROAMING_SMS_ONLY))
         && ((vgCREGData->lac    != 0)||(vgCREGData->cellId != 0)))
    {
      accessTechnology = ACT_NBIOT;
      showRegInfo = TRUE;
    }

    mobilityContext_p->cregData.state                = vgCREGData->state;
    if(showRegInfo == TRUE)
    {
      mobilityContext_p->cregData.accessTechnology   = accessTechnology;
      mobilityContext_p->cregData.cellId             = vgCREGData->cellId;
      mobilityContext_p->cregData.lac                = vgCREGData->lac;
    }
    else
    {
      mobilityContext_p->cregData.accessTechnology   = 0;
      mobilityContext_p->cregData.cellId             = 0;
      mobilityContext_p->cregData.lac                = 0;
    }

    atDataType.cmdType                               = CREG_CMD;
    atDataType.atData.cregData.accessTechnology      = mobilityContext_p->cregData.accessTechnology;
    atDataType.atData.cregData.cellId                = mobilityContext_p->cregData.cellId;
    atDataType.atData.cregData.lac                   = mobilityContext_p->cregData.lac;
    atDataType.atData.cregData.state                 = mobilityContext_p->cregData.state;
    atDataType.atData.cregData.serviceStatus         = mobilityContext_p->cregData.serviceStatus;
    if(DriWriteUnsolictedInd(&atDataType) == TRUE)
    {
      /* update is failure, so the data will be updated later */
      mobilityContext_p->cregUpdated                 = FALSE;
    }
    else
    {
      mobilityContext_p->cregUpdated                 = TRUE;
    }
}
#endif /* UPGRADE_SHARE_MEMORY || UPGRADE_SHMCL_SOLUTION */


/*--------------------------------------------------------------------------
*
* Function:    viewCREG
*
* Parameters:  entity     - mux channel number
*
* Returns:     Nothing
*
* Description: Sends current status of network registration.
*              If registered, location information elements lac and
*              cellId are also returned.
*
*-------------------------------------------------------------------------*/

void viewCREG (const VgmuxChannelNumber profileEntity,
               const Boolean            atQuery)
{
    MobilityContext_t *mobilityContext_p = ptrToMobilityContext ();
    VgCREGData        *vgCREGData        = &mobilityContext_p->vgCREGData;

    Int8              accessTechnology = 0;

    vgPutNewLine (profileEntity);

    if ( atQuery == TRUE )
    {
        vgPrintf (profileEntity,
            (const Char *)"+CREG: %d,%d",
            getProfileValue (profileEntity, PROF_CREG),
            (Int16)vgCREGData->state);
    }
    else
    {
        vgPrintf (profileEntity,
            (const Char *)"+CREG: %d",
            (Int16)vgCREGData->state);
    }

    /* if registered, lac and cellId are also appended to string */

    if (((vgCREGData->state == VGMNL_REGISTRATED_HOME)
         || (vgCREGData->state == VGMNL_REGISTRATED_ROAMING)
         || (vgCREGData->state == VGMNL_REG_HOME_SMS_ONLY)
         || (vgCREGData->state == VGMNL_REG_ROAMING_SMS_ONLY)) &&
        ((vgCREGData->lac    != 0) || (vgCREGData->cellId != 0)) &&
        (getProfileValue (profileEntity, PROF_CREG) == VG_CREG_ENABLED_WITH_LOCATION_INFO))
    {
      accessTechnology = ACT_NBIOT;

      vgPrintf (profileEntity,
            (const Char*)",\"%04X\",\"%08X\",%d",
            vgCREGData->lac,
            vgCREGData->cellId,
            accessTechnology);

    }

    vgPutNewLine (profileEntity);
} /* viewCREG */

/*--------------------------------------------------------------------------
*
* Function:    vgMmCPLS
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This function executes the AT+CPLS command, this allows the SIM file
*              which contains the preferred PLMN list to be selected.
*
*-------------------------------------------------------------------------*/

ResultCode_t vgMmCPLS ( CommandLine_t            *commandBuffer_p,
                        const VgmuxChannelNumber entity)
{
    ResultCode_t result = RESULT_CODE_OK;
    ExtendedOperation_t operation                    = getExtendedOperation (commandBuffer_p);
    SimLockGenericContext_t *simLockGenericContext_p = ptrToSimLockGenericContext ();
    MobilityContext_t       *mobilityContext_p       = ptrToMobilityContext ();
    VgCPLSData              *vgCPLSData_p            = &mobilityContext_p->vgCPLSData;
    VgSimInfo               *simInfo                 = &simLockGenericContext_p->simInfo;
    Int32 listSelector                               = 0;
    AbmmPlmnSelector plmnSelector;

    switch (operation)
    {
        case EXTENDED_RANGE: /* AT+CPLS=? */
        {
            vgPutNewLine (entity);
            vgPrintf(entity, (const Char *)"+CPLS: ");
            vgPrintf(entity, (const Char *)"(");

            if ((simInfo->userPlmnSelector) || (simInfo->plmnSelector))
            {
                vgPrintf(entity, (const Char *)"0");
            }

            if ( simInfo->operatorPlmnSelector )
            {
                if ((simInfo->userPlmnSelector) || (simInfo->plmnSelector))
                {
                    vgPrintf(entity, (const Char *)",");
                }
                vgPrintf(entity, (const Char *)"1");
            }

            if ( simInfo->hplmnSelector )
            {
                if ((simInfo->userPlmnSelector) ||
                    (simInfo->plmnSelector) ||
                    (simInfo->operatorPlmnSelector))
                {
                    vgPrintf(entity, (const Char *)",");
                }
                vgPrintf(entity, (const Char *)"2");
            }
            vgPrintf(entity, (const Char *)")");
            vgPutNewLine(entity);

            break;
        }

        case EXTENDED_ASSIGN: /* AT+CPLS=... */
        {

            /* get the input parameter from the string to be able
             * to know which file to select                        */
            if (getExtendedParameter (commandBuffer_p, &listSelector, ULONG_MAX) == TRUE)
            {
                if ( listSelector >  ABMM_HPLMN_SELECTOR )
                /* * can only process indices which are within the list */
                {
                    result = RESULT_CODE_ERROR;
                }
                else
                {
                    plmnSelector = (AbmmPlmnSelector) listSelector;

                    if (simLockGenericContext_p->simState == VG_SIM_READY)
                    {
                        switch (plmnSelector)
                        {
                            case ABMM_USER_SELECTOR:
                                if ((!simInfo->userPlmnSelector) && (!simInfo->plmnSelector))
                                {
                                    result = VG_CME_SIM_FILE_NOT_PRESENT;
                                }
                                break;

                            case ABMM_OPERATOR_SELECTOR:
                                if (!simInfo->operatorPlmnSelector)
                                {
                                    result = VG_CME_SIM_FILE_NOT_PRESENT;
                                }
                                break;

                            case ABMM_HPLMN_SELECTOR:
                                if (!simInfo->hplmnSelector)
                                {
                                    result = VG_CME_SIM_FILE_NOT_PRESENT;
                                }
                                break;

                            default:
                                result = RESULT_CODE_ERROR; /* invalid list selector shouldn't be able to get here */
                                break;
                        } /* switch */
                        if ( result == RESULT_CODE_OK)
                        {
                            vgCPLSData_p->plmnSelector = plmnSelector;
                        }
                    }
                    else
                    {
                        /* get SIM state error */
                        result = vgGetSimCmeErrorCode ();
                    }
                }
            }
            else /* error reading supplied parameter */
            {
                result = RESULT_CODE_ERROR;
            }
            break;
        }

        case EXTENDED_QUERY: /* AT+CPLS? */
        {
            vgPutNewLine (entity);
            vgPrintf(entity, (const Char *)"+CPLS: %d", vgCPLSData_p->plmnSelector );
            vgPutNewLine(entity);
            break;
        }

        case EXTENDED_ACTION: /* AT+CPLS... */
        default:
            result = RESULT_CODE_ERROR;
            break;
    } /* switch */
    return (result);
} /* vgMmCPLS */




/*--------------------------------------------------------------------------
*
* Function:    vgMmMNON
*
* Parameters:   commandBuffer_p - pointer to command line string
*               entity          - mux channel number
*
* Returns:      Resultcode_t    - result of function
*
* Description:  This function executes the AT*MNON command.
*-------------------------------------------------------------------------*/
ResultCode_t vgMmMNON (CommandLine_t *commandBuffer_p,
                       const VgmuxChannelNumber  entity)
{
    ResultCode_t            result = RESULT_CODE_OK;


    SimLockContext_t        *simLockContext_p = ptrToSimLockContext (entity);
    Int32 index = 0;
#if defined (ATCI_SLIM_DISABLE)

    FatalCheck (simLockContext_p != PNULL, entity, 0, 0);
#endif
    switch (getExtendedOperation (commandBuffer_p))
    {
        case EXTENDED_QUERY:  /* AT*MNON? */
        {
            simLockContext_p->vgMnonData.operation = VG_MNON_OP_READ_ALL;
            simLockContext_p->startField = 1;
            result = vgChManContinueAction (entity,APEX_SIM_LIST_PNN_REQ);
        }
        break;

        case EXTENDED_RANGE:  /* AT*MNON=? */
        {
            simLockContext_p->vgMnonData.operation = VG_MNON_OP_TEST;
            simLockContext_p->startField = 1;
            simLockContext_p->vgMnonData.startIntIndex = 0;
            simLockContext_p->vgMnonData.stopIntIndex = 0;
            result = vgChManContinueAction (entity,APEX_SIM_LIST_PNN_REQ);
        }
        break;

        case EXTENDED_ASSIGN: /* AT*MNON= */
        {
            if( (getExtendedParameter (  commandBuffer_p,
                                        &index,
                                        1) == TRUE) &&
                (index>0) &&
                (index<=VG_MAX_UINT8))
            {
                simLockContext_p->vgMnonData.operation = VG_MNON_OP_READ_INDEX;
                simLockContext_p->startField = (Int8)index;
                result = vgChManContinueAction (entity,APEX_SIM_LIST_PNN_REQ);
            }
            else
            {
                result = RESULT_CODE_ERROR;
            }
        }
        break;

        case EXTENDED_ACTION: /* AT*MNON */
        default:
        {
            result = RESULT_CODE_ERROR;
        }
        break;
    }

    return (result);
}

/*--------------------------------------------------------------------------
*
* Function:    vgMmMOPL
*
* Parameters:   commandBuffer_p - pointer to command line string
*               entity          - mux channel number
*
* Returns:      Resultcode_t    - result of function
*
* Description:  This function executes the AT*MOPL command.
*-------------------------------------------------------------------------*/

ResultCode_t vgMmMOPL (CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber  entity)
{
  ResultCode_t            result = RESULT_CODE_OK;
  SimLockGenericContext_t *simLockGenericContext_p = ptrToSimLockGenericContext ();
  SimLockContext_t        *simLockContext_p = ptrToSimLockContext (entity);
  VgSimInfo               *simInfo = &(simLockGenericContext_p->simInfo);
  Int32                   index = 0;
#if defined (ATCI_SLIM_DISABLE)

  FatalCheck (simLockContext_p != PNULL, entity, 0, 0);

#endif


  switch (getExtendedOperation (commandBuffer_p))
  {
    case EXTENDED_QUERY:  /* AT*MOPL? */
    {
      if(simInfo->serviceTableType == SIM_SERVICE_TABLE)
      {
        result = VG_CME_SIM_FILE_NOT_PRESENT;
      }
      else
      {
        simLockContext_p->commandField = 0;
        simLockContext_p->startField = 1;
        result = vgChManContinueAction (entity,APEX_SIM_LIST_OPL_REQ);
      }
      break;
    }

    case EXTENDED_RANGE:  /* AT*MOPL=? */
    {
      if(simInfo->serviceTableType == SIM_SERVICE_TABLE)
      {
        result = VG_CME_SIM_FILE_NOT_PRESENT;
      }
      else
      {
        vgPutNewLine(entity);
        vgPrintf( entity, (Char *)"*MOPL: (1-255)");
        vgPutNewLine(entity);
      }
      break;
    }

    case EXTENDED_ASSIGN: /* AT*MOPL= */
    {
      if(simInfo->serviceTableType == SIM_SERVICE_TABLE)
      {
        result = VG_CME_SIM_FILE_NOT_PRESENT;
      }
      else
      {
        if (getExtendedParameter ( commandBuffer_p,
                                   &index,
                                   1) == TRUE)
        {
          simLockContext_p->commandField = (Int8)index;
          simLockContext_p->startField = (Int8)index;
          result = vgChManContinueAction (entity,APEX_SIM_LIST_OPL_REQ);
        }
      }
      break;
    }

    case EXTENDED_ACTION: /* AT*MOPL */
    default:
    {
      result = RESULT_CODE_ERROR;
    }
    break;
  }

  return (result);
}

#if defined(UPGRADE_MTNET)

/*--------------------------------------------------------------------------
*
* Function:    vgMmOFF
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This function executes the AT+OFF command which close protocal
*              stack without detach procedure.
*
*-------------------------------------------------------------------------*/

ResultCode_t vgMmOFF (CommandLine_t            *commandBuffer_p,
                      const VgmuxChannelNumber entity)
{
    ResultCode_t            result              = RESULT_CODE_OK;
    MobilityContext_t       *mobilityContext_p  = ptrToMobilityContext ();
    ChManagerContext_t      *chManagerContext_p =  ptrToChManagerContext ();

    /*force change control*/
    chManagerContext_p->isImmediate = TRUE;
#if defined(USE_ABAPP)
    mobilityContext_p->isImmediate  = chManagerContext_p->isImmediate;
#endif

    result = vgChManContinueAction (entity, SIG_APEX_MM_SUSPEND_REQ);

    return (result);
} /* vgMmOFF */

/*--------------------------------------------------------------------------
*
* Function:    vgMmRESET
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This function executes the AT+RESET command which reset ME.
*
*-------------------------------------------------------------------------*/

ResultCode_t vgMmRESET (CommandLine_t            *commandBuffer_p,
                      const VgmuxChannelNumber entity)
{
    ResultCode_t            result              = RESULT_CODE_OK;
    Int32                   state;
    MobilityContext_t       *mobilityContext_p  = ptrToMobilityContext ();
    ChManagerContext_t      *chManagerContext_p =  ptrToChManagerContext ();

    switch (getExtendedOperation (commandBuffer_p))
    {
        case EXTENDED_ASSIGN: /* AT+RESET= */
        {
            /* get <state> */
            if (getExtendedParameter (commandBuffer_p,
                                 &state,
                                  ULONG_MAX) == TRUE)
            {
                if(state > VG_STATE_WITHOUT_DETACH)
                {
                    result = VG_CME_INVALID_INPUT_VALUE;
                }
            }
            else
            {
                result = VG_CME_INVALID_INPUT_VALUE;
            }

            if(RESULT_CODE_OK == result)
            {
                /*force change control*/
                chManagerContext_p->isImmediate = TRUE;
#if defined(USE_ABAPP)
                mobilityContext_p->isImmediate  = chManagerContext_p->isImmediate;
#endif
                if(VG_STATE_WITHOUT_DETACH == state)
                {
                    result = vgChManContinueAction (entity,
                        SIG_APEX_MM_SUSPEND_REQ);
                }
                else
                {
                    result = vgChManContinueAction (entity,
                        SIG_APEX_MM_DEREGISTER_REQ);
                }
            }
            break;
        }

        case EXTENDED_QUERY:  /* AT+RESET? */
        case EXTENDED_RANGE:  /* AT+RESET=? */
        case EXTENDED_ACTION: /* AT+RESET */
        default:
        {
          result = RESULT_CODE_ERROR;
        }
        break;
      }
    return (result);
} /* vgMmRESET */
#endif /* #if defined(UPGRADE_MTNET) */

/*--------------------------------------------------------------------------
 *
 * Function:        vgIsCurrentAccessTechnologyLte
 *
 * Parameters:      None
 *
 * Returns:         TRUE if we are in LTE mode currently.  FALSE otherwise.
 *
 * Description:
 *
 *-------------------------------------------------------------------------*/
Boolean      vgIsCurrentAccessTechnologyLte (void)
{
    return (TRUE);
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgPsdAttached
 *
 * Parameters:      None
 *
 * Returns:         TRUE if we are attached on a network for PSD services.
 *
 * Description:
 *
 *-------------------------------------------------------------------------*/
Boolean      vgPsdAttached (void)
{
    MobilityContext_t     *mobilityContext_p   = ptrToMobilityContext ();
    Boolean               psdAttached             = FALSE;

    if ((mobilityContext_p->vgNetworkPresent) &&
        (mobilityContext_p->vgNetworkState.serviceType == GPRS_SERVICE))
    {
      psdAttached = TRUE;
    }

    return (psdAttached);
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgCIPCAPermitsActivateAttachDefBearer
 *
 * Parameters:      None
 *
 * Returns:         TRUE if CIPCA setting allows activation of attach default
 *                  bearer.  If so, then ATCI will attempt to activate locally
 *                  depending on PROF_MLTEGGCF setting.
 *
 * Description:
 *
 *-------------------------------------------------------------------------*/
Boolean      vgCIPCAPermitsActivateAttachDefBearer (void)
{
  MobilityContext_t   *mobilityContext_p   = ptrToMobilityContext ();
  VgCipcaData         *vgCipcaData_p       = &(mobilityContext_p->vgCipcaData);
  Boolean             retVal = TRUE;

  /* This should never be called before we get a NetworkStateInd! */
  FatalAssert (vgCipcaData_p->dataValid);

  if ((vgCipcaData_p->dataValid) && (vgCipcaData_p->vgCipcaOpt == VG_CIPCA_ATTACH_WITHOUT_PDN))
  {
    retVal = FALSE;
  }
  return (retVal);
}

/*--------------------------------------------------------------------------
*
* Function:    vgMmCESQ
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This function executes the AT+CESQ command which displays the
*              signal quality report details depending on the current RAT.
*
*-------------------------------------------------------------------------*/

ResultCode_t vgMmCESQ (CommandLine_t            *commandBuffer_p,
                       const VgmuxChannelNumber entity)
{
    ResultCode_t result           = RESULT_CODE_OK;
    ExtendedOperation_t operation = getExtendedOperation (commandBuffer_p);

    switch (operation)
    {
        case EXTENDED_RANGE: /* AT+CESQ=? */
        {
            vgPutNewLine (entity);
            vgPrintf (entity, (const Char *)"+CESQ: (%d-%d,%d),(%d),(%d),(%d),(%d-%d,%d),(%d-%d,%d)",
                      VG_CESQ_MIN_RXLEV,
                      VG_CESQ_MAX_RXLEV,
                      VG_CESQ_INVALID_RXLEV,
                      VG_CESQ_INVALID_BER,
                      VG_CESQ_INVALID_RSCP,
                      VG_CESQ_INVALID_ECNO,
                      VG_CESQ_MIN_RSRQ,
                      VG_CESQ_MAX_RSRQ,
                      VG_CESQ_INVALID_RSRQ,
                      VG_CESQ_MIN_RSRP,
                      VG_CESQ_MAX_RSRP,
                      VG_CESQ_INVALID_RSRP);
            vgPutNewLine (entity);
            break;
        }
        case EXTENDED_ACTION: /* AT+CESQ... */
        {
            viewCESQ (entity, CESQ_QUERY_COMMAND);
            break;
        }
        case EXTENDED_QUERY: /* AT+CESQ?  */
        case EXTENDED_ASSIGN: /* AT+CESQ=  */
        default:
        {
            result = RESULT_CODE_ERROR;
            break;
        }
    } /* switch */

    return (result);
} /* vgMmCESQ */

/*--------------------------------------------------------------------------
*
* Function:    vgMmCSQ
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This function executes the AT+CSQ command which displays the
*              signal quality report details.
*
*-------------------------------------------------------------------------*/

ResultCode_t vgMmCSQ (CommandLine_t            *commandBuffer_p,
                       const VgmuxChannelNumber entity)
{
    ResultCode_t result           = RESULT_CODE_OK;
    ExtendedOperation_t operation = getExtendedOperation (commandBuffer_p);

    switch (operation)
    {
        case EXTENDED_RANGE: /* AT+CSQ=? */
        {
            vgPutNewLine (entity);
            vgPrintf (entity, (const Char *)"+CSQ: (%d-%d,%d),(%d-%d,%d)",
                      VG_CSQ_MIN_RSSI,
                      VG_CSQ_MAX_RSSI,
                      VG_CSQ_INVALID_RSSI,
                      VG_CESQ_MIN_BER,
                      VG_CESQ_MAX_BER,
                      VG_CESQ_INVALID_BER);
            vgPutNewLine (entity);
            break;
        }
        case EXTENDED_ACTION: /* AT+CSQ... */
        {
            displayCSQ (entity);
            break;
        }
        case EXTENDED_QUERY: /* AT+CSQ?  */
        case EXTENDED_ASSIGN: /* AT+CSQ=  */
        default:
        {
            result = RESULT_CODE_ERROR;
            break;
        }
    } /* switch */

    return (result);
} /* vgMmCSQ */

/*--------------------------------------------------------------------------
 * LTE Specific MM related AT commands.
 *-------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------
*
* Function:    vgMmMEMMREEST
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This function executes the AT*MEMMREEST command which
*              causes ATCI to send an MmDbmReestablishReq to MM task to force
*              a connection re-establishment as if UL data transmission were
*              going to happen.
*
*-------------------------------------------------------------------------*/

ResultCode_t vgMmMEMMREEST (CommandLine_t            *commandBuffer_p,
                               const VgmuxChannelNumber entity)
{
  ResultCode_t            result              = RESULT_CODE_OK;
  SignalBuffer            sigBuf              = kiNullBuffer;

  switch (getExtendedOperation (commandBuffer_p))
  {
    case EXTENDED_ACTION: /* AT*MEMMREEST */
    {
      /* Create the signal and send it to MM */
      KiCreateSignal (SIG_MMDBM_REESTABLISH_REQ, sizeof (MmDbmReestablishReq), &sigBuf);
      sigBuf.sig->mmDbmReestablishReq.mostDemandingTrfcClass = GPRS_TRAFFIC_CLASS_SUBSCRIBED;
      sigBuf.sig->mmDbmReestablishReq.psdBearerContextStatus = 0;
      sigBuf.sig->mmDbmReestablishReq.resetFollowOn = FALSE;

      KiSendSignal (MM_TASK_ID, &sigBuf);

      result = RESULT_CODE_OK;
      break;
    }
    case EXTENDED_ASSIGN: /* AT*MEMMREEST= */
    case EXTENDED_QUERY:  /* AT*MEMMREEST? */
    case EXTENDED_RANGE:  /* AT*MEMMREEST=? */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }

  return (result);
} /* vgMmRESET */

/***************************************************************************
 * NB-IOT AT commands
 ***************************************************************************/

/*--------------------------------------------------------------------------
 *
 * Function:        vgMmCEDRXS
 *
 * Parameters:      commandBuffer_p - pointer to command line string
 *                  entity          - mux channel number
 *
 * Returns:         AT result code.
 *
 * Description:     AT command to set and read the EDRX settings for
 *                  NB-IOT
 *
 *-------------------------------------------------------------------------*/
ResultCode_t vgMmCEDRXS (CommandLine_t *commandBuffer_p, const VgmuxChannelNumber entity)
{
    ResultCode_t        result               = RESULT_CODE_OK;
    MobilityContext_t   *mobilityContext_p   = ptrToMobilityContext ();
    VgReqEdrxData       *reqEdrxData_p       = &(mobilityContext_p->vgReqEdrxData);
    VgEdrxData          *currentEdrxData_p   = &(mobilityContext_p->vgEdrxData);
    VgEdrxMode          mode;
    Int32               param;
    Boolean             reqEdrxValPresent = FALSE;
    Char                reqEdrxStr[VG_CEDRX_MAX_BIN_STR_LEN + NULL_TERMINATOR_LENGTH] = {0};
    Char                tempStr[VG_CEDRX_MAX_BIN_STR_LEN + NULL_TERMINATOR_LENGTH] = {0};
    Int8                reqEdrxVal;
    Int16               reqEdrxStrLen;
    Int8                i;
    Boolean             found = FALSE;
    ChManagerContext_t      *chManagerContext_p       = ptrToChManagerContext ();

    switch (getExtendedOperation (commandBuffer_p))
    {
        case EXTENDED_RANGE:  /* AT+CEDRXS=? */
        {
          vgPutNewLine (entity);
          vgPrintf (entity, (const Char *)"+CEDRXS: (%d-%d),(%d),(\"0000\"-\"1111\")",
                    VG_EDRX_MODE_DISABLE_EDRX,
                    VG_EDRX_MODE_DISABLE_EDRX_AND_RESET,
                    VG_CEDRX_ACT_NB_IOT);
          vgPutNewLine (entity);
        }
        break;

        case EXTENDED_ASSIGN: /* AT+CEDRXS=... */
        {
            /* Get the mode */
            if( (getExtendedParameter ( commandBuffer_p,
                                        &param,
                                        ULONG_MAX) == TRUE) &&
                (param != ULONG_MAX))
            {
                if (param >= NUM_VG_EDRX_MODE)
                {
                    result = VG_CME_INVALID_INPUT_VALUE;
                }
                else
                {
                    mode = param;
                }
            }
            else
            {
                result = VG_CME_INVALID_INPUT_VALUE;
            }

            /* Get the Act type */
            if (result == RESULT_CODE_OK)
            {
                if(getExtendedParameter ( commandBuffer_p,
                                            &param,
                                            ULONG_MAX) == TRUE)
                {
                    if (param != ULONG_MAX)
                    {
                        if (param != VG_CEDRX_ACT_NB_IOT)
                        {
                            result = VG_CME_INVALID_INPUT_VALUE;
                        }
                        /* No need to store the Act because it can only be NB-IOT */
                    }
                    /* else Act was missing - but doesn't matter because it can only be NB-IOT */
                }
                else
                {
                    result = VG_CME_INVALID_INPUT_VALUE;
                }
            }

            /* Get the Requested eDRX value */
            if (result == RESULT_CODE_OK)
            {

                if (getExtendedString ( commandBuffer_p,
                    reqEdrxStr,
                    VG_CEDRX_MAX_BIN_STR_LEN,
                    &reqEdrxStrLen) == TRUE)
                {
                    if (reqEdrxStrLen > 0)
                    {
                        if (reqEdrxStrLen == VG_CEDRX_MAX_BIN_STR_LEN)
                        {
                            /* String is correct length - convert to number */
                            if (vgBinStrToBin (reqEdrxStr, &reqEdrxVal) == TRUE)
                            {
                                reqEdrxValPresent = TRUE;
                            }
                            else
                            {
                                result = VG_CME_INVALID_INPUT_VALUE;
                            }
                        }
                        else
                        {
                            result = VG_CME_INVALID_INPUT_VALUE;
                        }
                    }
                    /* else Act was missing - but doesn't matter because it can only be NB-IOT */
                }
                else
                {
                    result = VG_CME_INVALID_INPUT_VALUE;
                }
            }

            /* Have all the parameters now - so send the signal */
            if (result == RESULT_CODE_OK)
            {
                if ((mode == VG_EDRX_MODE_DISABLE_EDRX) ||
                    (mode == VG_EDRX_MODE_DISABLE_EDRX_AND_RESET))
                {
                  /* We don't need the EDRX value present for disable */
                  reqEdrxValPresent = FALSE;
                  reqEdrxData_p->userEdrxSupport = FALSE;
                  reqEdrxData_p->userPtwPresence = FALSE;
                }
                else /*  we need to set the support flag */
                {
                   reqEdrxData_p->userEdrxSupport = TRUE;
                }


                if (reqEdrxValPresent)
                {
                  /* Check the value is valid */
                  for (i=0; (!found) && (validEdrxValues[i] != EDRX_VALUE_0); i++)
                  {
                    if (validEdrxValues[i] == reqEdrxVal)
                    {
                      found = TRUE;
                    }
                  }

                  if ((found) || (reqEdrxVal == EDRX_VALUE_0))
                  {
                    reqEdrxData_p->userEdrxValue = reqEdrxVal;

                  }
                  else
                  {
                    result = VG_CME_INVALID_INPUT_VALUE;
                  }
                }
                else
                {
                  reqEdrxData_p->userEdrxValue = EDRX_VALUE_0;
                }

                if (result == RESULT_CODE_OK)
                {

                  /* If the unsolicited is enable on this channel - then enable in the profile.
                   * Any other value - and it is disabled.
                   * Default is off.
                   */
                  if (mode == VG_EDRX_MODE_ENABLE_EDRX_ENABLE_UNSOL)
                  {
                    setProfileValue(entity, PROF_CEDRXS, VG_CEDRXP_UNSOL_ENABLE);

                    /* Change mode now to not say UNSOL */
                    mode = VG_EDRX_MODE_ENABLE_EDRX;
                  }
                  else
                  {
                    setProfileValue(entity, PROF_CEDRXS, VG_CEDRXP_UNSOL_DISABLE);
                  }

                  /* All parameters OK */
                  reqEdrxData_p->mode = mode;

                  /* Always sent request to AB even if it looks like nothing has changed */
                  chManagerContext_p->isImmediate = TRUE;
                  mobilityContext_p->isImmediate  = chManagerContext_p->isImmediate;
                  result = vgChManContinueAction(entity,SIG_APEX_MM_SET_EDRX_REQ);

                }
            }
        }
        break;

        case EXTENDED_QUERY:  /* AT+CEDRXS? */
        {
            /* Always sent request to AB even if it looks like nothing has changed */
            chManagerContext_p->isImmediate = TRUE;
            mobilityContext_p->isImmediate  = chManagerContext_p->isImmediate;
              result = vgChManContinueAction(entity, SIG_APEX_MM_READ_EDRX_REQ);
           }
        break;

        case EXTENDED_ACTION: /* AT*CEDRXS... */
        default:
        {
            result = RESULT_CODE_ERROR;
        }
        break;
    }
    return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgMmCEDRXRDP
 *
 * Parameters:      commandBuffer_p - pointer to command line string
 *                  entity          - mux channel number
 *
 * Returns:         AT result code.
 *
 * Description:     AT command to read current EDRX settings for current
 *                  cell.
 *
 *-------------------------------------------------------------------------*/
ResultCode_t vgMmCEDRXRDP (CommandLine_t *commandBuffer_p, const VgmuxChannelNumber entity)
{
    ResultCode_t        result               = RESULT_CODE_OK;
    MobilityContext_t   *mobilityContext_p   = ptrToMobilityContext ();
    VgEdrxData          *currentEdrxData_p   = &(mobilityContext_p->vgEdrxData);
    Char                tempStr[VG_CEDRX_MAX_BIN_STR_LEN + NULL_TERMINATOR_LENGTH] = {0};
    ChManagerContext_t      *chManagerContext_p       = ptrToChManagerContext ();

    switch (getExtendedOperation (commandBuffer_p))
    {
        case EXTENDED_RANGE:  /* AT+CEDRXRDP=? */
        {
          /* Do nothing */
        }
        break;

        case EXTENDED_ACTION: /* AT*CEDRXRDP... */
        {
           /* Always sent request to AB even if it looks like nothing has changed */
           chManagerContext_p->isImmediate = TRUE;
           mobilityContext_p->isImmediate  = chManagerContext_p->isImmediate;
             result = vgChManContinueAction(entity, SIG_APEX_MM_READ_EDRX_REQ);
          }
        break;

        default:
        {
            result = RESULT_CODE_ERROR;
        }
        break;
    }
    return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgMmMEDRXCFG
 *
 * Parameters:      commandBuffer_p - pointer to command line string
 *                  entity          - mux channel number
 *
 * Returns:         AT result code.
 *
 * Description:     AT command to set and read the EDRX related settings for
 *                  NB-IOT
 *
 *-------------------------------------------------------------------------*/
ResultCode_t vgMmMEDRXCFG (CommandLine_t *commandBuffer_p, const VgmuxChannelNumber entity)
{
    ResultCode_t        result               = RESULT_CODE_OK;
    MobilityContext_t   *mobilityContext_p   = ptrToMobilityContext ();
    VgReqEdrxData       *reqEdrxData_p       = &(mobilityContext_p->vgReqEdrxData);
    VgEdrxData          *currentEdrxData_p   = &(mobilityContext_p->vgEdrxData);
    VgEdrxMode          mode;
    Int32               param;
    Boolean             reqEdrxValPresent = FALSE;
    Boolean             reqPtwValPresent = FALSE;
    Char                reqEdrxStr[VG_CEDRX_MAX_BIN_STR_LEN + NULL_TERMINATOR_LENGTH] = {0};
    Char                tempStr[VG_CEDRX_MAX_BIN_STR_LEN + NULL_TERMINATOR_LENGTH] = {0};
    Int8                reqEdrxVal;
    Char                reqPtwStr[VG_CEDRX_MAX_BIN_STR_LEN + NULL_TERMINATOR_LENGTH] = {0};
    Int8                reqPtwVal;
    Int16               reqEdrxStrLen;
    Int16               reqPtwStrLen;
    Int8                i;
    Boolean             found = FALSE;
    ChManagerContext_t      *chManagerContext_p       = ptrToChManagerContext ();

    switch (getExtendedOperation (commandBuffer_p))
    {
        case EXTENDED_RANGE:  /* AT*MEDRXCFG=? */
        {
          vgPutNewLine (entity);
          vgPrintf (entity, (const Char *)"*MEDRXCFG: (%d-%d),(%d),(\"0000\"-\"1111\"),(\"0000\"-\"1111\")",
                    VG_EDRX_MODE_DISABLE_EDRX,
                    VG_EDRX_MODE_DISABLE_EDRX_AND_RESET,
                    VG_CEDRX_ACT_NB_IOT);
          vgPutNewLine (entity);
        }
        break;

        case EXTENDED_ASSIGN: /* AT*MEDRXCFG=... */
        {
            /* Get the mode */
            if( (getExtendedParameter ( commandBuffer_p,
                                        &param,
                                        ULONG_MAX) == TRUE) &&
                (param != ULONG_MAX))
            {
                if (param >= NUM_VG_EDRX_MODE)
                {
                    result = VG_CME_INVALID_INPUT_VALUE;
                }
                else
                {
                    mode = param;
                }
            }
            else
            {
                result = VG_CME_INVALID_INPUT_VALUE;
            }

            /* Get the Act type */
            if (result == RESULT_CODE_OK)
            {
                if(getExtendedParameter ( commandBuffer_p,
                                            &param,
                                            ULONG_MAX) == TRUE)
                {
                    if (param != ULONG_MAX)
                    {
                        if (param != VG_CEDRX_ACT_NB_IOT)
                        {
                            result = VG_CME_INVALID_INPUT_VALUE;
                        }
                        /* No need to store the Act because it can only be NB-IOT */
                    }
                    /* else Act was missing - but doesn't matter because it can only be NB-IOT */
                }
                else
                {
                    result = VG_CME_INVALID_INPUT_VALUE;
                }
            }

            /* Get the Requested eDRX value */
            if (result == RESULT_CODE_OK)
            {

                if (getExtendedString ( commandBuffer_p,
                    reqEdrxStr,
                    VG_CEDRX_MAX_BIN_STR_LEN,
                    &reqEdrxStrLen) == TRUE)
                {
                    if (reqEdrxStrLen > 0)
                    {
                        if (reqEdrxStrLen == VG_CEDRX_MAX_BIN_STR_LEN)
                        {
                            /* String is correct length - convert to number */
                            if (vgBinStrToBin (reqEdrxStr, &reqEdrxVal) == TRUE)
                            {
                                reqEdrxValPresent = TRUE;
                            }
                            else
                            {
                                result = VG_CME_INVALID_INPUT_VALUE;
                            }
                        }
                        else
                        {
                            result = VG_CME_INVALID_INPUT_VALUE;
                        }
                    }
                    /* else no EDRX value is inputted */
                }
                else
                {
                    result = VG_CME_INVALID_INPUT_VALUE;
                }
            }
            
            /* Get the Requested PTW value */
            if (result == RESULT_CODE_OK)
            {
                if (getExtendedString ( commandBuffer_p,
                    reqPtwStr,
                    VG_CEDRX_MAX_BIN_STR_LEN,
                    &reqPtwStrLen) == TRUE)
                {
                    if (reqPtwStrLen > 0)
                    {
                        if (reqPtwStrLen == VG_CEDRX_MAX_BIN_STR_LEN)
                        {
                            /* String is correct length - convert to number */
                            if (vgBinStrToBin (reqPtwStr, &reqPtwVal) == TRUE)
                            {
                                reqPtwValPresent = TRUE;
                            }
                            else
                            {
                                result = VG_CME_INVALID_INPUT_VALUE;
                            }
                        }
                        else
                        {
                            result = VG_CME_INVALID_INPUT_VALUE;
                        }
                    }
                    /* else no PTW value is inputted */
                }
                else
                {
                    result = VG_CME_INVALID_INPUT_VALUE;
                }
            }

            /* Have all the parameters now - so send the signal */
            if (result == RESULT_CODE_OK)
            {
                if ((mode == VG_EDRX_MODE_DISABLE_EDRX) ||
                    (mode == VG_EDRX_MODE_DISABLE_EDRX_AND_RESET))
                {
                  /* We don't need the EDRX value present for disable */
                  reqEdrxValPresent = FALSE;
                  reqPtwValPresent = FALSE;
                  reqEdrxData_p->userEdrxSupport = FALSE;
                }
                else /*  we need to set the support flag */
                {
                   reqEdrxData_p->userEdrxSupport = TRUE;
                   reqEdrxData_p->userPtwPresence = reqPtwValPresent;
                   reqEdrxData_p->userPtwValue = reqPtwVal;
                }

                if (reqEdrxValPresent)
                {
                  /* Check the value is valid */
                  for (i=0; (!found) && (validEdrxValues[i] != EDRX_VALUE_0); i++)
                  {
                    if (validEdrxValues[i] == reqEdrxVal)
                    {
                      found = TRUE;
                    }
                  }

                  if ((found) || (reqEdrxVal == EDRX_VALUE_0))
                  {
                    reqEdrxData_p->userEdrxValue = reqEdrxVal;

                  }
                  else
                  {
                    result = VG_CME_INVALID_INPUT_VALUE;
                  }
                }
                else
                {
                  reqEdrxData_p->userEdrxValue = EDRX_VALUE_0;
                }

                if (result == RESULT_CODE_OK)
                {

                  /* If the unsolicited is enable on this channel - then enable in the profile.
                   * Any other value - and it is disabled.
                   * Default is off.
                   */
                  if (mode == VG_EDRX_MODE_ENABLE_EDRX_ENABLE_UNSOL)
                  {
                    setProfileValue(entity, PROF_CEDRXS, VG_CEDRXP_UNSOL_ENABLE);

                    /* Change mode now to not say UNSOL */
                    mode = VG_EDRX_MODE_ENABLE_EDRX;
                  }
                  else
                  {
                    setProfileValue(entity, PROF_CEDRXS, VG_CEDRXP_UNSOL_DISABLE);
                  }

                  /* All parameters OK */
                  reqEdrxData_p->mode = mode;
                  reqEdrxData_p->userPtwPresence = reqPtwValPresent;
                  reqEdrxData_p->userPtwValue = reqPtwVal;
                  
                  /* Always sent request to AB even if it looks like nothing has changed */
                  chManagerContext_p->isImmediate = TRUE;
                  mobilityContext_p->isImmediate  = chManagerContext_p->isImmediate;
                  result = vgChManContinueAction(entity,SIG_APEX_MM_SET_EDRX_REQ);
                }
            }
        }
        break;

        case EXTENDED_QUERY:  /* AT+MEDRXCFG? */
        {
           if (currentEdrxData_p->userDataValid)
           {
              vgPutNewLine (entity);
              vgPrintf (entity,
                 (const Char*)"*MEDRXCFG: %d",
                  VG_CEDRX_ACT_NB_IOT);
              if ((currentEdrxData_p->userEdrxSupport == FALSE ) &&
                (currentEdrxData_p->userEdrxValue != EDRX_VALUE_0))
              {
                 currentEdrxData_p->userEdrxValue = EDRX_VALUE_0;
              }
              vgPrintf (entity, (const Char*)",\"");
              vgInt8ToBinString(currentEdrxData_p->userEdrxValue, VG_CEDRX_MAX_BIN_STR_LEN, tempStr);
              vgPrintf (entity, (const Char*)tempStr);
              vgPrintf (entity, (const Char*)"\"");
              if(currentEdrxData_p->userPtwPresence == TRUE)
              {
                vgPrintf (entity, (const Char*)",\"");
                vgInt8ToBinString(currentEdrxData_p->userPagingTimeWindow, VG_CEDRX_MAX_BIN_STR_LEN, tempStr);
                vgPrintf (entity, (const Char*)tempStr);
                vgPrintf (entity, (const Char*)"\"");
              }
              vgPutNewLine (entity);
           }
           else
           {           
              /* Always sent request to AB even if it looks like nothing has changed */
              chManagerContext_p->isImmediate = TRUE;
              mobilityContext_p->isImmediate  = chManagerContext_p->isImmediate;
              result = vgChManContinueAction(entity, SIG_APEX_MM_READ_EDRX_REQ);
           }
        }
        break;

        case EXTENDED_ACTION: /* AT*CEDRXS... */
        default:
        {
            result = RESULT_CODE_ERROR;
        }
        break;
    }
    return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgMmCCIOTOPT
 *
 * Parameters:      commandBuffer_p - pointer to command line string
 *                  entity          - mux channel number
 *
 * Returns:         AT result code.
 *
 * Description:     AT command to set and read the CIoT option settings for
 *                  NB-IOT
 *
 *-------------------------------------------------------------------------*/
ResultCode_t vgMmCCIOTOPT (CommandLine_t *commandBuffer_p, const VgmuxChannelNumber entity)
{
    ResultCode_t        result               = RESULT_CODE_OK;
    MobilityContext_t   *mobilityContext_p   = ptrToMobilityContext ();
    VgReqCciotoptData   *reqCciotoptData_p   = &(mobilityContext_p->vgReqCciotoptData);
    VgCciotoptData      *currentCciotData_p   = &(mobilityContext_p->vgCciotoptData);
    VgCiotOptN          n;
    Boolean             reqSupportedPresent = FALSE;
    SupportedIotOptions reqSupported = IOT_NOT_SUPPORTED;
    Boolean             reqPrefPresent = FALSE;
    CiotPreference      reqPref = IOT_NO_PREFERENCE;
    Int32               param;

    switch (getExtendedOperation (commandBuffer_p))
    {
        case EXTENDED_RANGE:  /* AT+CCIOTOPT=? */
        {
          vgPutNewLine (entity);
          vgPrintf (entity, (const Char *)"+CCIOTOPT: (%d-%d),(%d,%d),(%d-%d)",
                    VG_CCIOTOPT_N_DISABLE_UNSOL,
                    VG_CCIOTOPT_N_DISABLE_UNSOL_RESET_PARAMS_TO_DEF,
                    IOT_CP_SUPPORTED,
                    IOT_BOTH_SUPPORTED,
                    IOT_NO_PREFERENCE,
                    IOT_UP_PREFERENCE);
          vgPutNewLine (entity);
        }
        break;

        case EXTENDED_ASSIGN: /* AT+CCIOTOPT=... */
        {
            /* Get n */
            if( (getExtendedParameter ( commandBuffer_p,
                                        &param,
                                        ULONG_MAX) == TRUE) &&
                (param != ULONG_MAX))
            {
                if (param >= NUM_VG_CCIOTOPT_N)
                {
                    result = VG_CME_INVALID_INPUT_VALUE;
                }
                else
                {
                    n = param;
                }
            }
            else
            {
                result = VG_CME_INVALID_INPUT_VALUE;
            }

            /* Get the supported param */
            if (result == RESULT_CODE_OK)
            {
                if(getExtendedParameter ( commandBuffer_p,
                                            &param,
                                            ULONG_MAX) == TRUE)
                {
                    if (param != ULONG_MAX)
                    {
                        if (param > IOT_BOTH_SUPPORTED || IOT_UP_SUPPORTED == param || IOT_NOT_SUPPORTED == param)
                        {
                            result = VG_CME_INVALID_INPUT_VALUE;
                        }
                        else
                        {
                            reqSupportedPresent = TRUE;
                            reqSupported = param;
                        }
                    }
                }
                else
                {
                    result = VG_CME_INVALID_INPUT_VALUE;
                }
            }

            /* Get the pref param */
            if (result == RESULT_CODE_OK)
            {
                if(getExtendedParameter ( commandBuffer_p,
                                            &param,
                                            ULONG_MAX) == TRUE)
                {
                    if (param != ULONG_MAX)
                    {
                        if (param > IOT_UP_PREFERENCE)
                        {
                            result = VG_CME_INVALID_INPUT_VALUE;
                        }
                        else
                        {
                            reqPrefPresent = TRUE;
                            reqPref = param;
                        }
                    }
                }
                else
                {
                    result = VG_CME_INVALID_INPUT_VALUE;
                }
            }


            /* Have all the parameters now - so send the signal */
            if (result == RESULT_CODE_OK)
            {
                if (n == VG_CCIOTOPT_N_DISABLE_UNSOL_RESET_PARAMS_TO_DEF)
                {
                  /* We don't need any other parameters */
                  reqSupportedPresent = FALSE;
                  reqPrefPresent = FALSE;
                  reqCciotoptData_p->resetCiotOptParams = TRUE;
                }
                else
                {
                  reqCciotoptData_p->resetCiotOptParams = FALSE;
                }

                /* If the unsolicited is enable on this channel - then enable in the profile.
                 * Any other value - and it is disabled.
                 * Default is off.
                 */
                if (n == VG_CCIOTOPT_N_ENABLE_UNSOL)
                {
                  setProfileValue(entity, PROF_CCIOTOPT, VG_CCIOTOPT_N_ENABLE_UNSOL);
                }
                else
                {
                  setProfileValue(entity, PROF_CCIOTOPT, VG_CCIOTOPT_N_DISABLE_UNSOL);
                }
                /* All parameters OK */
                /* store the reporting option set */
                currentCciotData_p->reportOpt = n;
                /* need to send the signal to write the optimisations unless we are only setting reporting enable/disable */
                if ((reqSupportedPresent == TRUE) || (reqPrefPresent == TRUE) ||
                    (n == VG_CCIOTOPT_N_DISABLE_UNSOL_RESET_PARAMS_TO_DEF))
                {

                   reqCciotoptData_p->supportedUEOptPresent = reqSupportedPresent;
                   reqCciotoptData_p->supportedUEOpt = reqSupported;
                   reqCciotoptData_p->prefUEOptPresent = reqPrefPresent;
                   reqCciotoptData_p->prefUEOpt = reqPref;

                   result = vgChManContinueAction(entity, SIG_APEX_WRITE_IOT_OPT_CFG_REQ);
                 }

                 /* otherwise just leave with RESULT_CODE_OK */
               }
        }
        break;

        case EXTENDED_QUERY:  /* AT+CCIOTOPT? */
        {

           if (currentCciotData_p->uEdataValid)
           {
              vgPutNewLine (entity);
              vgPrintf (entity,(const Char*)"+CCIOTOPT: %d,%d,%d", currentCciotData_p->reportOpt,
              currentCciotData_p->supportedUEOpt, currentCciotData_p->prefUEOpt);
              vgPutNewLine (entity);
           }
           else
           {
            result = vgChManContinueAction(entity, SIG_APEX_READ_IOT_OPT_CFG_REQ);
           }
        }
        break;

        case EXTENDED_ACTION: /* AT*CCIOTOPT... */
        default:
        {
            result = RESULT_CODE_ERROR;
        }
        break;
    }
    return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgMmCPSMS
 *
 * Parameters:      commandBuffer_p - pointer to command line string
 *                  entity          - mux channel number
 *
 * Returns:         AT result code.
 *
 * Description:     AT command to set and read power-save mode
 *
 *-------------------------------------------------------------------------*/
 ResultCode_t vgMmCPSMS (CommandLine_t *commandBuffer_p, const VgmuxChannelNumber entity)
{
    ResultCode_t        result               = RESULT_CODE_OK;
    MobilityContext_t   *mobilityContext_p   = ptrToMobilityContext ();
    Int32               param;
    VgReqCpsmsData      *vgReqCpsmsData_p     = &(mobilityContext_p->vgReqCpsmsData);
    VgCpsmsData         *vgCpsmsData_p        = &(mobilityContext_p->vgCpsmsData);
    Char                reqPeriodicTauStr[VG_CPSMS_MAX_STR_LEN + NULL_TERMINATOR_LENGTH] = {0};
    Char                reqActiveTimeStr[VG_CPSMS_MAX_STR_LEN + NULL_TERMINATOR_LENGTH] = {0};
    Int16               reqCpsmsStrLen;
    Int8                reqVal;
    Int8                unit;
    Boolean             paramPresent = FALSE;
    ChManagerContext_t      *chManagerContext_p       = ptrToChManagerContext ();

   switch (getExtendedOperation (commandBuffer_p))
    {
        case EXTENDED_RANGE:  /* AT+CPSMS=? */
        {
          vgPutNewLine (entity);
          vgPrintf (entity, (const Char *)
            "+CPSMS: (%d-%d),,,(\"00000000\"-\"1101111\"),(\"00000000\"-\"11111111\")",
            VG_CPSMS_DISABLE,VG_CPSMS_DISABLE_OR_RESET_TO_DEFAULTS);
          vgPutNewLine (entity);
        }
        break;

        case EXTENDED_ASSIGN: /* AT+CPSMS=... */
        {
            /* Get mode */
            if( (getExtendedParameter ( commandBuffer_p,
                                        &param,
                                        ULONG_MAX) == TRUE) &&
            (param != ULONG_MAX))
            {
                if (param >= NUM_VG_CPSMS_OPTS)
                {

                    result = VG_CME_INVALID_INPUT_VALUE;
                }
                else
                {
                   vgReqCpsmsData_p->mode = (VgCpsmsOpt)param;
                }
            }
            else
            {  /* no input so ERROR */
               result = VG_CME_INVALID_INPUT_VALUE;
            }

            if ((result == RESULT_CODE_OK) && (vgReqCpsmsData_p->mode == VG_CPSMS_ENABLE))
            /* don't need any further input if this is disabling or resetting to defaults */
            {
               /* shouldn't have input for requested periodic RAU for NB-IOT */
               (void)getExtendedParameterPresent(commandBuffer_p, &param, 0, &paramPresent);
               if (paramPresent == TRUE)
               {
                 result = VG_CME_INVALID_INPUT_VALUE;
               }
               else
               {
                  /* shouldn't have input for requested GPRS-READY timer for NB-IOT */
                  (void)getExtendedParameterPresent(commandBuffer_p, &param, 0, &paramPresent);
                  if(paramPresent == TRUE)
                  {
                     result = VG_CME_INVALID_INPUT_VALUE;
                  }
                  else
                  {
                    if (getExtendedString ( commandBuffer_p, reqPeriodicTauStr, VG_CPSMS_MAX_STR_LEN,
                                            &reqCpsmsStrLen) == TRUE)
                    {
                       if (reqCpsmsStrLen > 0)
                       {
                          if (reqCpsmsStrLen == VG_CPSMS_MAX_STR_LEN)
                          {
                             /* String is correct length - convert to number */
                             if (vgBinStrToBin (reqPeriodicTauStr, &reqVal) == TRUE)
                             {
                                unit = reqVal >> 5;
                                if (unit >= VG_CPSMS_MAX_TAU_UNIT_VAL)
                                {
                                  result = VG_CME_INVALID_INPUT_VALUE;
                                }
                                else
                                {
                                   vgReqCpsmsData_p->reqTau = reqVal;
                                   vgReqCpsmsData_p->reqTauPresent = TRUE;
                                }
                             }  
                             else
                             {
                               result = VG_CME_INVALID_INPUT_VALUE;
                             }
                          }
                          else
                          {
                            result = VG_CME_INVALID_INPUT_VALUE;
                          }
                       }
                       else  /* no values input */
                       {
                           vgReqCpsmsData_p->reqTau = 0;
                           vgReqCpsmsData_p->reqTauPresent = FALSE;
                       }
                    }
                    else
                    {
                       result = VG_CME_INVALID_INPUT_VALUE;
                    }
                    
                    if (result == RESULT_CODE_OK)
                    {
                      if (getExtendedString ( commandBuffer_p, reqActiveTimeStr, VG_CPSMS_MAX_STR_LEN,
                                              &reqCpsmsStrLen) == TRUE)
                      {
                         if (reqCpsmsStrLen > 0)
                         {
                            if (reqCpsmsStrLen == VG_CPSMS_MAX_STR_LEN)
                            {
                               /* String is correct length - convert to number */
                               if (vgBinStrToBin (reqActiveTimeStr, &reqVal) == TRUE)
                               {
                                  vgReqCpsmsData_p->reqActiveTime = reqVal;
                                  vgReqCpsmsData_p->reqActiveTimePresent = TRUE;
                               }
                               else
                               {
                                  result = VG_CME_INVALID_INPUT_VALUE;
                               }
                            }
                            else
                            {
                               result = VG_CME_INVALID_INPUT_VALUE;
                            }
                         }
                         else  /* no values input  */
                         {
                            vgReqCpsmsData_p->reqActiveTime = 0;
                            vgReqCpsmsData_p->reqActiveTimePresent = FALSE;
 
                         }
                      }
                      else
                      {
 
                         result = VG_CME_INVALID_INPUT_VALUE;
                      }
                    }
                 }
               }
            }

            if (result == RESULT_CODE_OK)
            {
              /* Always sent request to AB even if it looks like nothing has changed */
              chManagerContext_p->isImmediate = TRUE;
#if defined(USE_ABAPP)
              mobilityContext_p->isImmediate  = chManagerContext_p->isImmediate;
#endif
              result = vgChManContinueAction(entity,SIG_APEX_WRITE_PSM_CONF_REQ);
            }
        }
        break;

        case EXTENDED_QUERY:  /* AT+CPSMS? */
        {
           if (vgCpsmsData_p->dataValid)
           {
              vgPutNewLine (entity);
              /*convert the values to the strings */

              vgPrintf (entity, (const Char*)"+CPSMS: %d",vgCpsmsData_p->mode);
              if (vgCpsmsData_p->mode == VG_CPSMS_ENABLE)
              {
                 vgPrintf (entity, (const Char*)",,,\"");

                 vgInt8ToBinString(vgCpsmsData_p->requestedTau, VG_CPSMS_MAX_STR_LEN, reqPeriodicTauStr);
                 vgPrintf (entity, (const Char*)reqPeriodicTauStr);

                 vgPrintf (entity, (const Char*)"\",\"");
                 vgInt8ToBinString(vgCpsmsData_p->requestedActiveTime, VG_CPSMS_MAX_STR_LEN, reqActiveTimeStr);
                 vgPrintf (entity, (const Char*)reqActiveTimeStr);
                 vgPrintf (entity, (const Char*)"\"");
             }
             vgPutNewLine (entity);
           }
           else
           {
              /* Always sent request to AB even if it looks like nothing has changed */
              chManagerContext_p->isImmediate = TRUE;
#if defined(USE_ABAPP)
              mobilityContext_p->isImmediate  = chManagerContext_p->isImmediate;
#endif
              result = vgChManContinueAction(entity, SIG_APEX_READ_PSM_CONF_REQ);
           }
        }
        break;

        case EXTENDED_ACTION: /* AT*CPSMS... */
        default:
        {
            result = RESULT_CODE_ERROR;
        }
        break;
    }
    return (result);
}


/*--------------------------------------------------------------------------
 *
 * Function:        vgMmCIPCA
 *
 * Parameters:      commandBuffer_p - pointer to command line string
 *                  entity          - mux channel number
 *
 * Returns:         AT result code.
 *
 * Description:     AT command to set and read Inital PDN connection activation state
 *
 *-------------------------------------------------------------------------*/
 ResultCode_t vgMmCIPCA (CommandLine_t *commandBuffer_p, const VgmuxChannelNumber entity)
{
    ResultCode_t        result               = RESULT_CODE_OK;
    MobilityContext_t   *mobilityContext_p   = ptrToMobilityContext ();
    VgCipcaData         *vgCipcaData_p       = &(mobilityContext_p->vgCipcaData);
    Int32               param;


    switch (getExtendedOperation (commandBuffer_p))
     {
         case EXTENDED_RANGE:  /* AT+CIPCA=? */
         {
           vgPutNewLine (entity);
           vgPrintf (entity, (const Char *)
            "+CIPCA: (%d),(%d-%d)", VG_CIPCA_EUTRAN_SETTING,VG_CIPCA_ATTACH_WITH_PDN,
            VG_CIPCA_ATTACH_WITHOUT_PDN);
           vgPutNewLine (entity);
         }
         break;

         case EXTENDED_ASSIGN: /* AT+CIPCA=... */
         {
            if( (getExtendedParameter ( commandBuffer_p,
                                         &param,
                                         ULONG_MAX) == FALSE) ||
             (param != VG_CIPCA_EUTRAN_SETTING))  /* for NB-IOT n must equal 3 */
             {
                 result = VG_CME_INVALID_INPUT_VALUE;
             }

             if (result == RESULT_CODE_OK)
             {
                 if( (getExtendedParameter ( commandBuffer_p,
                                         &param, ULONG_MAX) == FALSE) ||
                 (!((param == VG_CIPCA_ATTACH_WITH_PDN) ||
                    (param == VG_CIPCA_ATTACH_WITHOUT_PDN))))
                {
                   result = VG_CME_INVALID_INPUT_VALUE;
                }
                else
                {  /*  check if this is same as already valid data then ok otherwise send signal */
                   if (!((vgCipcaData_p->dataValid == TRUE) &&
                        (param == vgCipcaData_p->vgCipcaOpt)))
                   {
                      mobilityContext_p->vgReqCipcaOpt = (VgCipcaOpt)param;
                      result = vgChManContinueAction(entity, SIG_APEX_MM_WRITE_ATTACH_PDN_CONF_REQ);
                   }
               }
            }
         }
         break;

         case EXTENDED_QUERY:  /* AT+CIPCA? */
         {
            if (vgCipcaData_p->dataValid)
            {
               vgPutNewLine (entity);
               vgPrintf (entity, (const Char *)
               "+CIPCA: %d,%d", VG_CIPCA_EUTRAN_SETTING,vgCipcaData_p->vgCipcaOpt);
               vgPutNewLine (entity);
            }
            else
            {
               result = vgChManContinueAction(entity, SIG_APEX_MM_READ_ATTACH_PDN_CONF_REQ);
            }
         }
         break;

         case EXTENDED_ACTION: /* AT*CIPCA... */
         default:
         {
             result = RESULT_CODE_ERROR;
         }
         break;
     }
     return (result);
}

#if defined (FEA_NFM)
/*--------------------------------------------------------------------------
 *
 * Function:        vgMmNFM
 *
 * Parameters:      commandBuffer_p - pointer to command line string
 *                  entity          - mux channel number
 *
 * Returns:         AT result code.
 *
 * Description:     AT command to set and read nfm mode
 *
 *-------------------------------------------------------------------------*/
 ResultCode_t vgMmNFM (CommandLine_t *commandBuffer_p, const VgmuxChannelNumber entity)
{
    ResultCode_t        result               = RESULT_CODE_OK;
    MobilityContext_t   *mobilityContext_p   = ptrToMobilityContext ();
    Int32               param;
    VgReqNfmData        *vgReqNfmData_p     = &(mobilityContext_p->vgReqNfmData);
    VgNfmData           *vgNfmData_p        = &(mobilityContext_p->vgNfmData);
    ChManagerContext_t      *chManagerContext_p       = ptrToChManagerContext ();

   switch (getExtendedOperation (commandBuffer_p))
    {
        case EXTENDED_RANGE:  /* AT+NFM=? */
        {
          vgPutNewLine (entity);
          vgPrintf (entity, (const Char *)
            "+NFM: (%d,%d),(%d,%d)",
            VG_NFM_DISABLED,VG_NFM_ENABLED,VG_NFM_DISABLED,VG_NFM_ENABLED);
          vgPutNewLine (entity);
        }
        break;

        case EXTENDED_ASSIGN: /* AT+NFM=... */
        {
            /* Get NFM Active */
            if( (getExtendedParameter ( commandBuffer_p,
                                        &param,
                                        ULONG_MAX) == TRUE) &&
            (param != ULONG_MAX))
            {
                if (param > VG_NFM_ENABLED)
                {

                    result = VG_CME_INVALID_INPUT_VALUE;
                }
                else
                {
                   vgReqNfmData_p->reqNfmActive = param;
                }
            }
            else
            {  /* no input so ERROR */
               result = VG_CME_INVALID_INPUT_VALUE;
            }

            vgReqNfmData_p->reqStartTimerActivePresent = FALSE;
            if (result == RESULT_CODE_OK)
            {
                /* Get Start Timer Active */
                if( (getExtendedParameter ( commandBuffer_p,
                                            &param,
                                            ULONG_MAX) == TRUE) &&
                (param != ULONG_MAX))
                {
                    if (param > VG_NFM_ENABLED)
                    {
                        result = VG_CME_INVALID_INPUT_VALUE;
                    }
                    else
                    {
                        vgReqNfmData_p->reqStartTimerActivePresent = TRUE;
                        vgReqNfmData_p->reqStartTimerActive = param;
                    }
                }
            }

            if (result == RESULT_CODE_OK)
            {
              chManagerContext_p->isImmediate = TRUE;
#if defined(USE_ABAPP)
              mobilityContext_p->isImmediate  = chManagerContext_p->isImmediate;
#endif
              result = vgChManContinueAction(entity,SIG_APEX_WRITE_NFM_REQ);
            }
        }
        break;

        case EXTENDED_QUERY:  /* AT+NFM? */
        {
           if (vgNfmData_p->dataValid)
           {
              vgPutNewLine (entity);
              /*convert the values to the strings */
              vgPrintf (entity, (const Char*)"+NFM: %d,%d",vgNfmData_p->nfmActive, vgNfmData_p->startTimerActive);
              vgPutNewLine (entity);
           }
           else
           {
              chManagerContext_p->isImmediate = TRUE;
              mobilityContext_p->isImmediate  = chManagerContext_p->isImmediate;
              result = vgChManContinueAction(entity, SIG_APEX_READ_NFM_REQ);
           }
        }
        break;

        case EXTENDED_ACTION: /* AT*NFM... */
        default:
        {
            result = RESULT_CODE_ERROR;
        }
        break;
    }
    return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgMmNFMC
 *
 * Parameters:      commandBuffer_p - pointer to command line string
 *                  entity          - mux channel number
 *
 * Returns:         AT result code.
 *
 * Description:     AT command to set and read nfmc
 *
 *-------------------------------------------------------------------------*/
 ResultCode_t vgMmNFMC (CommandLine_t *commandBuffer_p, const VgmuxChannelNumber entity)
{
    ResultCode_t        result               = RESULT_CODE_OK;
    MobilityContext_t   *mobilityContext_p   = ptrToMobilityContext ();
    Int32               param;
    VgReqNfmcData       *vgReqNfmcData_p     = &(mobilityContext_p->vgReqNfmcData);
    VgNfmcData          *vgNfmcData_p        = &(mobilityContext_p->vgNfmcData);
    Int8                count;
    Boolean             noMore;
    ChManagerContext_t  *chManagerContext_p       = ptrToChManagerContext ();

   switch (getExtendedOperation (commandBuffer_p))
    {
        case EXTENDED_RANGE:  /* AT+NFMC=? */
        {
          vgPutNewLine (entity);
          vgPrintf (entity, (const Char *)
            "+NFMC: (%d-%d),(%d-%d),(%d-%d),(%d-%d),(%d-%d),(%d-%d),(%d-%d),(%d-%d)",
            1,VG_NFMC_MAX_VALUE,1,VG_NFMC_MAX_VALUE,1,VG_NFMC_MAX_VALUE,
            1,VG_NFMC_MAX_VALUE,1,VG_NFMC_MAX_VALUE,1,VG_NFMC_MAX_VALUE,
            1,VG_NFMC_MAX_VALUE,1,VG_NFMC_MAX_VALUE);
          vgPutNewLine (entity);
        }
        break;

        case EXTENDED_ASSIGN: /* AT+NFMC=... */
        {
          for (count = 0; count < MAX_NFM_PAR_VALUE; count++)
          {
            vgReqNfmcData_p->reqNfmPar[count] = 1;
            vgReqNfmcData_p->reqNfmParPresent[count] = FALSE;
          }
          vgReqNfmcData_p->reqStPar = 1;
          vgReqNfmcData_p->reqStParPresent = FALSE;

          count  = 0;
          noMore = FALSE;
          do
          {
            /* Get NFMC values */
            if( getExtendedParameter ( commandBuffer_p,
                                        &param,
                                        ULONG_MAX) == TRUE )
            {
              if (param != ULONG_MAX)
              {
                if ((param > 0) && (param <= VG_NFMC_MAX_VALUE))
                {
                  switch(count)
                  {
                     case 0:
                     case 1:
                     case 2:
                     case 3:
                     case 4:
                     case 5:
                     case 6:
                       vgReqNfmcData_p->reqNfmPar[count]        = param;
                       vgReqNfmcData_p->reqNfmParPresent[count] = TRUE;
                       break;
                     case 7:
                       vgReqNfmcData_p->reqStPar        = param;
                       vgReqNfmcData_p->reqStParPresent = TRUE;
                       break;
                     default:
                       result = VG_CME_INVALID_INPUT_VALUE;
                       break;
                  }
                  count++;
                }
                else
                {
                  result = VG_CME_INVALID_INPUT_VALUE;
                }
              }
              else
              {
                count++;
              }
            }
            else  /* no values input  */
            {
              noMore = TRUE;
            }
          }
          while ((noMore == FALSE)
                  && (count <= MAX_NFM_PAR_VALUE) && (result == RESULT_CODE_OK));

          if (result == RESULT_CODE_OK)
          {
            chManagerContext_p->isImmediate = TRUE;
            mobilityContext_p->isImmediate  = chManagerContext_p->isImmediate;
            result = vgChManContinueAction(entity,SIG_APEX_WRITE_NFM_CONF_REQ);
          }
        }
        break;

        case EXTENDED_QUERY:  /* AT+NFMC? */
        {
           if (vgNfmcData_p->dataValid)
           {
              vgPutNewLine (entity);
              /*convert the values to the strings */
              vgPrintf (entity, (const Char*)"+NFMC: %d,%d,%d,%d,%d,%d,%d,%d",
                  vgNfmcData_p->nfmPar[0], vgNfmcData_p->nfmPar[1], vgNfmcData_p->nfmPar[2],
                  vgNfmcData_p->nfmPar[3], vgNfmcData_p->nfmPar[4], vgNfmcData_p->nfmPar[5],
                  vgNfmcData_p->nfmPar[6], vgNfmcData_p->stPar );
              vgPutNewLine (entity);
           }
           else
           {
              chManagerContext_p->isImmediate = TRUE;
              mobilityContext_p->isImmediate  = chManagerContext_p->isImmediate;
              result = vgChManContinueAction(entity, SIG_APEX_READ_NFM_CONF_REQ);
           }
        }
        break;

        case EXTENDED_ACTION: /* AT*NFMC... */
        default:
        {
            result = RESULT_CODE_ERROR;
        }
        break;
    }
    return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgMmMNFM
 *
 * Parameters:      commandBuffer_p - pointer to command line string
 *                  entity          - mux channel number
 *
 * Returns:         AT result code.
 *
 * Description:     AT command to set URC mode and read nfm timer values
 *
 *-------------------------------------------------------------------------*/
 ResultCode_t vgMmMNFM (CommandLine_t *commandBuffer_p, const VgmuxChannelNumber entity)
{
    ResultCode_t        result               = RESULT_CODE_OK;
    MobilityContext_t   *mobilityContext_p   = ptrToMobilityContext ();
    Int32               param;
    ChManagerContext_t  *chManagerContext_p  = ptrToChManagerContext ();

   switch (getExtendedOperation (commandBuffer_p))
    {
        case EXTENDED_RANGE:  /* AT*MNFM=? */
        {
          vgPutNewLine (entity);
          vgPrintf (entity, (const Char *)
            "*MNFM: (%d,%d)",
            VG_NFM_UNSOL_DISABLE,VG_NFM_UNSOL_ENABLE);
          vgPutNewLine (entity);
        }
        break;

        case EXTENDED_ASSIGN: /* AT*MNFM=... */
        {
            /* Get NFM URC mode */
            if( (getExtendedParameter ( commandBuffer_p,
                                        &param,
                                        ULONG_MAX) == TRUE) &&
            (param != ULONG_MAX))
            {
                if (param > VG_NFM_UNSOL_ENABLE)
                {
                    result = VG_CME_INVALID_INPUT_VALUE;
                }
                else
                {
                  if (param == VG_NFM_UNSOL_ENABLE)
                  {
                    setProfileValue(entity, PROF_NFM, VG_NFM_UNSOL_ENABLE);
                  }
                  else
                  {
                    setProfileValue(entity, PROF_NFM, VG_NFM_UNSOL_DISABLE);
                  }
                }
            }
            else
            {  /* no input so ERROR */
               result = VG_CME_INVALID_INPUT_VALUE;
            }

        }
        break;

        case EXTENDED_QUERY:  /* AT*MNFM? */
        {
            chManagerContext_p->isImmediate = TRUE;
            mobilityContext_p->isImmediate  = chManagerContext_p->isImmediate;
            result = vgChManContinueAction(entity, SIG_APEX_READ_NFM_STATUS_REQ);
        }
        break;

        default:
        {
            result = RESULT_CODE_ERROR;
        }
        break;
    }
    return (result);
}
/*--------------------------------------------------------------------------
 *
 * Function:        vgMmMTC
 *
 * Parameters:      commandBuffer_p - pointer to command line string
 *                  entity          - mux channel number
 *
 * Returns:         AT result code.
 *
 * Description:     AT command to set and read nfm mode
 *
 *-------------------------------------------------------------------------*/
 ResultCode_t vgMmMTC (CommandLine_t *commandBuffer_p, const VgmuxChannelNumber entity)
{
    ResultCode_t        result               = RESULT_CODE_OK;
    MobilityContext_t   *mobilityContext_p   = ptrToMobilityContext ();
    Int32               param;
    VgReqNfmData        *vgReqNfmData_p     = &(mobilityContext_p->vgReqNfmData);
    VgNfmData           *vgNfmData_p        = &(mobilityContext_p->vgNfmData);
    ChManagerContext_t      *chManagerContext_p       = ptrToChManagerContext ();

   switch (getExtendedOperation (commandBuffer_p))
    {

        case EXTENDED_ASSIGN: /* AT*MTC=... */
        {

            if( (getExtendedParameter ( commandBuffer_p,
                                        &param,
                                        ULONG_MAX) == TRUE) &&
            (param != ULONG_MAX))
            {
                if (param > VG_NFM_ENABLED)
                {

                    result = VG_CME_INVALID_INPUT_VALUE;
                }
                else
                {
                   vgReqNfmData_p->reqNfmActive = param;
                }
            }
            else
            {  /* no input so ERROR */
               result = VG_CME_INVALID_INPUT_VALUE;
            }

            vgReqNfmData_p->reqStartTimerActivePresent = FALSE;
            if (result == RESULT_CODE_OK)
            {

                if( (getExtendedParameter ( commandBuffer_p,
                                            &param,
                                            ULONG_MAX) == TRUE) &&
                (param != ULONG_MAX))
                {
                    if (param > VG_NFM_ENABLED)
                    {
                        result = VG_CME_INVALID_INPUT_VALUE;
                    }
                    else
                    {
                        vgReqNfmData_p->reqStartTimerActivePresent = TRUE;
                        vgReqNfmData_p->reqStartTimerActive = param;
                    }
                }
            }
            
            vgReqNfmData_p->reqStParPresent = FALSE;
            
            if (result == RESULT_CODE_OK)
            {

                if( (getExtendedParameter ( commandBuffer_p,
                                            &param,
                                            ULONG_MAX) == TRUE) &&
                (param != ULONG_MAX))
                {

                    vgReqNfmData_p->reqStParPresent = TRUE;
                    vgReqNfmData_p->reqStPar = param;
                }
            }
            
            vgReqNfmData_p->reqStTmPresent = FALSE;

            if (result == RESULT_CODE_OK)
            {

                if( (getExtendedParameter ( commandBuffer_p,
                                            &param,
                                            ULONG_MAX) == TRUE) &&
                (param != ULONG_MAX))
                {

                    vgReqNfmData_p->reqStTmPresent = TRUE;
                    vgReqNfmData_p->reqStTm = param;
                }
            }

            if (result == RESULT_CODE_OK)
            {
              chManagerContext_p->isImmediate = TRUE;
#if defined(USE_ABAPP)
              mobilityContext_p->isImmediate  = chManagerContext_p->isImmediate;
#endif
              result = vgChManContinueAction(entity,SIG_APEX_WRITE_NFM_REQ);
            }
        }
        break;
        case EXTENDED_ACTION: /* AT*MTC... */
        default:
        {
            result = RESULT_CODE_ERROR;
        }
        break;
    }
    return (result);
}

#endif /* FEA_NFM */

/*--------------------------------------------------------------------------
*
* Function:    vgReadRfErrcSupportBand
*
* Parameters:  bandSupportBmp                    band_list
*
* Returns:     Nothing
*
* Description: Get RF ERRC supported band
*
*-------------------------------------------------------------------------*/
void vgReadRfErrcSupportBand(Int32  bandSupportBmp,Int8 *band_list)
{
    if( VG_ERRC_BANDS_RF_BAND_1 & bandSupportBmp )
    {
        band_list[0]=1;
    }
    if( VG_ERRC_BANDS_RF_BAND_2 & bandSupportBmp )
    {
        band_list[1]=2;
    }
    if( VG_ERRC_BANDS_RF_BAND_3 & bandSupportBmp )
    {
        band_list[2]=3;
    }
    if( VG_ERRC_BANDS_RF_BAND_4 & bandSupportBmp)
    {
        band_list[3]=4;
    }
    if( VG_ERRC_BANDS_RF_BAND_5 & bandSupportBmp )
    {
        band_list[4]=5;
    }
    if( VG_ERRC_BANDS_RF_BAND_8 & bandSupportBmp )
    {
        band_list[5]=8;
    }
    if( VG_ERRC_BANDS_RF_BAND_11 & bandSupportBmp )
    {
        band_list[6]=11;
    }
    if( VG_ERRC_BANDS_RF_BAND_12 & bandSupportBmp )
    {
        band_list[7]=12;
    }
    if( VG_ERRC_BANDS_RF_BAND_13 & bandSupportBmp )
    {
        band_list[8]=13;
    }
    if( VG_ERRC_BANDS_RF_BAND_14 & bandSupportBmp )
    {
        band_list[9]=14;
    }
    if( VG_ERRC_BANDS_RF_BAND_17 & bandSupportBmp )
    {
        band_list[10]=17;
    }
    if( VG_ERRC_BANDS_RF_BAND_18 & bandSupportBmp )
    {
        band_list[11]=18;
    }
    if( VG_ERRC_BANDS_RF_BAND_19 & bandSupportBmp )
    {
        band_list[12]=19;
    }
    if( VG_ERRC_BANDS_RF_BAND_20 & bandSupportBmp )
    {
        band_list[13]=20;
    }
    if( VG_ERRC_BANDS_RF_BAND_21 & bandSupportBmp )
    {
        band_list[14]=21;
    }
    if( VG_ERRC_BANDS_RF_BAND_25 & bandSupportBmp )
    {
        band_list[15]=25;
    }
    if( VG_ERRC_BANDS_RF_BAND_26 & bandSupportBmp )
    {
        band_list[16]=26;
    }
    if( VG_ERRC_BANDS_RF_BAND_28 & bandSupportBmp )
    {
        band_list[17]=28;
    }
    if( VG_ERRC_BANDS_RF_BAND_31 & bandSupportBmp )
    {
        band_list[18]=31;
    }
    if( VG_ERRC_BANDS_RF_BAND_66 & bandSupportBmp )
    {
        band_list[19]=66;
    }
    if( VG_ERRC_BANDS_RF_BAND_70 & bandSupportBmp )
    {
        band_list[20]=70;
    }
    if( VG_ERRC_BANDS_RF_BAND_71 & bandSupportBmp )
    {
        band_list[21]=71;
    }
    if( VG_ERRC_BANDS_RF_BAND_72 & bandSupportBmp )
    {
        band_list[22]=72;
    }
    if( VG_ERRC_BANDS_RF_BAND_73 & bandSupportBmp )
    {
        band_list[23]=73;
    }
    if( VG_ERRC_BANDS_RF_BAND_74 & bandSupportBmp )
    {
        band_list[24]=74;
    }
    if( VG_ERRC_BANDS_RF_BAND_85 & bandSupportBmp )
    {
        band_list[25]=85;
    }
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgMmMBAND
 *
 * Parameters:      commandBuffer_p - pointer to command line string
 *                        entity          - mux channel number
 *
 * Returns:          AT result code.
 *
 * Description:     AT command read currcent band
 *
 *-------------------------------------------------------------------------*/
 ResultCode_t vgMmMBAND (CommandLine_t *commandBuffer_p, const VgmuxChannelNumber entity)
{
    ResultCode_t            result             = RESULT_CODE_OK;
    ExtendedOperation_t     operation          = getExtendedOperation (commandBuffer_p);
    MobilityContext_t      *mobilityContext_p  = ptrToMobilityContext ();
    ChManagerContext_t     *chManagerContext_p = ptrToChManagerContext ();    
    Int8                    band_list[26];
    Int8                    pIndex = 0;    
    Boolean                 first_band = TRUE;

    /* this function only support to read band now */
    switch (operation)
    {
    case EXTENDED_QUERY:   /* AT*MBAND? */
        {
            chManagerContext_p->isImmediate   = TRUE;
        #if defined(USE_ABAPP)
            mobilityContext_p->isImmediate    = chManagerContext_p->isImmediate;
        #endif
            result = vgChManContinueAction (entity, SIG_APEX_MM_READ_BAND_MODE_REQ);
        }
        break;
    case EXTENDED_RANGE:   /* AT*MBAND=? */
        {
            memset((Int8*)(band_list), 0, 26 * sizeof (Int8));
#if defined (MTK_NBIOT_TARGET_BUILD)
            vgReadRfErrcSupportBand((Int32)ErrBandsSupportedBandsGet(),band_list);
            vgPutNewLine (entity);
            vgPrintf (entity,(const Char *)"*MBAND: ");
            
            for(pIndex=0;pIndex<26;pIndex++)
            {
                if(0!=band_list[pIndex])
                {
                   if(first_band)
                   {
                        vgPrintf (entity,
                                 (const Char *)"%d",band_list[pIndex]);
                        first_band = FALSE;
                   }
                   else
                   {
                        vgPrintf (entity,
                                 (const Char *)",%d",band_list[pIndex]);
                   }
                }
            }
            vgPutNewLine (entity);
#endif            
            setResultCode (entity, RESULT_CODE_OK);
        }
        break;
    case EXTENDED_ASSIGN:  /* AT*MBAND=... */
        {
            /*Do not support set action now*/
        }

    default:
        {
            result = RESULT_CODE_ERROR;
        }
        break;

    } /* switch */

    return (result);

}

 ResultCode_t vgMmMBANDSL (CommandLine_t *commandBuffer_p, const VgmuxChannelNumber entity)
{
    ResultCode_t            result             = RESULT_CODE_OK;
    ExtendedOperation_t     operation          = getExtendedOperation (commandBuffer_p);
    MobilityContext_t      *mobilityContext_p  = ptrToMobilityContext ();
    ChManagerContext_t     *chManagerContext_p = ptrToChManagerContext ();
    Int32               param;
    VgMBANDSLData             *vgMANDSLData = &(mobilityContext_p->vgMandSLData);
    Int8 i = 0;
    /* this function only support to read band now */
    switch (operation)
    {

       case EXTENDED_ASSIGN:  /* AT*MBANDSL=... */
       {
           if( (getExtendedParameter ( commandBuffer_p,
                                     &param, NUM_MBANDSL_OPTIONS) == FALSE) ||
             (!((param == VG_BAND_SEATCH_LIST_DISABLE) ||
                (param == VG_BAND_SEATCH_LIST_ENABLE))))
            {
               result = VG_CME_INVALID_INPUT_VALUE;
            }
            else
            {
                vgMANDSLData->enable = param;
            }
            if (result == RESULT_CODE_OK)
            {
                if((getExtendedParameter ( commandBuffer_p,
                                            &param,
                                            ULONG_MAX) == TRUE) && (5>param))
                {
                    vgMANDSLData->numSearchBand = param;
                }
                else
                {
                    result = VG_CME_INVALID_INPUT_VALUE;
                }
                if(!vgMANDSLData->enable)
                {
                    vgMANDSLData->numSearchBand = 0;
                    result = RESULT_CODE_OK;
                }
            }
            if (result == RESULT_CODE_OK)
            {

                for(i=0;i<vgMANDSLData->numSearchBand;i++)
                {
                   getExtendedParameter ( commandBuffer_p,
                                            &vgMANDSLData->searchBandList[i],ULONG_MAX);
                }

                chManagerContext_p->isImmediate   = TRUE;
#if defined(USE_ABAPP)
                mobilityContext_p->isImmediate    = chManagerContext_p->isImmediate;
#endif
                result = vgChManContinueAction (entity, SIG_APEX_MM_SEARCH_BAND_LIST_REQ);
            }
         
        }
        break;
        default:
        {
            result = RESULT_CODE_ERROR;
        }
        break;

    } /* switch */

    return (result);

}
/*--------------------------------------------------------------------------
*
* Function:    viewCSCON
*
* Parameters:  entity     - mux channel number
*
* Returns:     Nothing
*
* Description: Sends current status of EMM connection status.
*              If registered, location information elements lac and
*              cellId are also returned.
*
*-------------------------------------------------------------------------*/

void viewCSCON (const VgmuxChannelNumber profileEntity,
               const Boolean            atQuery)
{
    VgCsconRptCfg      csconRptCfg;
    MobilityContext_t *mobilityContext_p = ptrToMobilityContext ();
    VgCSCONData       *vgCSCONData       = &mobilityContext_p->vgCSCONData;

    csconRptCfg       = vgCSCONData->rptCfgOp;
    
    vgPutNewLine (profileEntity);
    
    if ( atQuery == TRUE )
    {
        vgPrintf (profileEntity,
            (const Char *)"+CSCON: %d,%d", 
            csconRptCfg, vgCSCONData->SigConMode);
    }
    else
    {
        vgPrintf (profileEntity,
            (const Char *)"+CSCON: %d", vgCSCONData->SigConMode);
    }

    vgPutNewLine (profileEntity);
}


/*--------------------------------------------------------------------------
 *
 * Function:        vgMmCSCON
 *
 * Parameters:      commandBuffer_p - pointer to command line string
 *                        entity          - mux channel number
 *
 * Returns:          AT result code.
 *
 * Description:     AT command query and disable/enable report signalling  connection status *
 *-------------------------------------------------------------------------*/
 ResultCode_t vgMmCSCON(CommandLine_t *commandBuffer_p, const VgmuxChannelNumber entity)
{
    ResultCode_t            result             = RESULT_CODE_OK;
    ExtendedOperation_t     operation          = getExtendedOperation (commandBuffer_p);
    MobilityContext_t      *mobilityContext_p  = ptrToMobilityContext ();
    Int32                   paramValue;
    VgmuxChannelNumber      profileEntity     = 0;

    /* this function only support to read band now */
    switch (operation)
    {

    case EXTENDED_ASSIGN:  /* AT+CSCON=... */
        {
            Boolean parseResult = FALSE;

            parseResult = getExtendedParameter (commandBuffer_p,
                                            &paramValue,
                                            VG_CSCON_QUERY);
            
            if ( TRUE == parseResult)
            {
                if (paramValue < VG_CSCON_QUERY)
                {
                    mobilityContext_p->vgCSCONData.rptCfgOp = (VgCsconRptCfg)paramValue;
                    
                    for (profileEntity = 0; profileEntity < CI_MAX_ENTITIES; profileEntity++)
                    {
                      if (isEntityActive (profileEntity))
                      {
                        if (VG_CSCON_ENABLED == mobilityContext_p->vgCSCONData.rptCfgOp)
                        {
                            viewCSCON(profileEntity, FALSE);
                        }
                      }
                    }
                }
                else
                {
                    result = RESULT_CODE_ERROR;
                }
            }
            else
            {
                result = RESULT_CODE_ERROR;
            }
        }
        break;
    case EXTENDED_QUERY:   /* AT+CSCON? */
        {
            viewCSCON(entity, TRUE);
            setResultCode (entity, RESULT_CODE_OK);
        }
        break;
    case EXTENDED_RANGE:   /* AT+CSCON=? */
        {
            vgPutNewLine (entity);
            vgPrintf(entity, (const Char *)"+CSCON: (%d-%d)",
                   VG_CSCON_DISABLED, VG_CSCON_ENABLED);
            vgPutNewLine (entity);
            setResultCode (entity, RESULT_CODE_OK);
        }
        break;
    default:
        {
            result = RESULT_CODE_ERROR;
        }
        break;
    } /* switch */

    return (result);

}/*vgMmCSCON*/
/*--------------------------------------------------------------------------
 *
 * Function:        vgMmMFRCLLCK
 *
 * Parameters:      commandBuffer_p - pointer to command line string
 *                        entity          - mux channel number
 *
 * Returns:          AT result code.
 *
 * Description:     AT command lock Cell & freq, camps on specified frequenry and Cell only *
 *-------------------------------------------------------------------------*/
 ResultCode_t vgMmMFRCLLCK(CommandLine_t *commandBuffer_p, const VgmuxChannelNumber entity)
{
    ResultCode_t            result             = RESULT_CODE_OK;
    ExtendedOperation_t     operation          = getExtendedOperation (commandBuffer_p);
    MobilityContext_t      *mobilityContext_p  = ptrToMobilityContext ();
    ChManagerContext_t     *chManagerContext_p = ptrToChManagerContext ();
    Int32                   lock;
    Int32                   earfcn;
    Int32                   earfcnOffset;
    Int32                   pci;
    Int32                   byPassSCriteria;
    Boolean                 earfcnOffsetPresent = FALSE;
    Boolean                 pciPresent          = FALSE;
    Boolean                 byPassSCriteriaPresent = FALSE;
    /* this function only support to read band now */
    switch (operation)
    {
    case EXTENDED_ASSIGN:  /* AT*MFRCLLCK=... */
        {
            Boolean parseResult = FALSE;
            /*get Lock status*/
            parseResult = getExtendedParameter (commandBuffer_p,
                                            &lock,
                                            ULONG_MAX);
            if ( TRUE == parseResult)
            {
                if (lock <= VG_MFRCLLCK_LOCK_ACTIVATE_LOCK)
                {
                    mobilityContext_p->vgMfrcllckData.lockStatus = (Boolean)lock;
                
                }
                else
                {
                    result = VG_CME_INVALID_INPUT_VALUE;
                }
            }
            else
            {
                result = RESULT_CODE_ERROR;
            }
            /*RESET INFO*/
            mobilityContext_p->vgMfrcllckData.isPciValid          = FALSE;

            /*if activate lock, need get others param*/
            if ((RESULT_CODE_OK == result) 
             && (VG_MFRCLLCK_LOCK_ACTIVATE_LOCK == lock))
            {
                /*Get EARFCN*/
                parseResult = getExtendedParameter (commandBuffer_p,
                                                &earfcn,
                                                ULONG_MAX);
                if ( TRUE == parseResult)
                {
                    if (earfcn <= VG_MFRCLLCK_EARFCN_MAX_NUMBER)
                    {
                        mobilityContext_p->vgMfrcllckData.earfcn= earfcn;
                    
                    }
                    else
                    {
                        result = VG_CME_INVALID_INPUT_VALUE;
                    }
                }
                else
                {
                    result = RESULT_CODE_ERROR;
                } 
                /* if earfcn parse successful, parse other param*/
                if (RESULT_CODE_OK == result)
                {
                    /*Get earfcn offset if present */
                    parseResult = getExtendedParameterPresent(commandBuffer_p,
                                                            &earfcnOffset,
                                                            ULONG_MAX,
                                                            &earfcnOffsetPresent);
                    
                    if (TRUE == earfcnOffsetPresent) /*present earfcn offset*/
                    {
                        if (TRUE == parseResult) /*earfcn offset parse successsful*/
                        {
                            if (earfcnOffset < VG_MFRCLLCK_NUMBER_OF_EARFCN_OFFSET)
                            {
                                mobilityContext_p->vgMfrcllckData.earfcnOffset        = (VgMfrcllckEarfcnOffset)earfcnOffset;
                            }
                            else /*earfcn offset invalid*/
                            {
                                result = VG_CME_INVALID_INPUT_VALUE;
                            }
                        }
                        else /*earfcn offset parse failure*/    
                        {
                            result = RESULT_CODE_ERROR;
                        }
                    }
                    else
                    {
                      /* Must always have offset */
                      result = VG_CME_INVALID_INPUT_VALUE;
                    }               

                    /* if earfcn offset parse successful, parse other param*/
                    if (RESULT_CODE_OK == result)
                    {
                        /*Get PCI if present */
                        parseResult = getExtendedParameterPresent(commandBuffer_p,
                                                                &pci,
                                                                ULONG_MAX,
                                                                &pciPresent);
                  
                        if (TRUE == pciPresent)/*PCI present */
                        {
                            if (TRUE == parseResult) /*PCI parse successful*/
                            {
                                if (pci <= VG_MFRCLLCK_PCI_MAX_NUMBER)
                                {
                                    mobilityContext_p->vgMfrcllckData.isPciValid = TRUE;
                                    mobilityContext_p->vgMfrcllckData.pci        = (uint16_t)pci;
                                }
                                else /*PCI invalid*/
                                {
                                    result = VG_CME_INVALID_INPUT_VALUE;
                                }
                            }
                            else /*PCI parse failure*/
                            {
                                result = RESULT_CODE_ERROR;
                            }
                        }

                    }/* End: if earfcn offset parse successful*/

                }/* End: if earfcn parse successful*/
                
            }/* End: if activate lock, need get others param*/
           if (RESULT_CODE_OK == result)
           {

                parseResult = getExtendedParameter(commandBuffer_p,
                                                        &byPassSCriteria,
                                                        ULONG_MAX);
                if(byPassSCriteria == ULONG_MAX)
                {
                   mobilityContext_p->vgMfrcllckData.byPassSCriteria = FALSE;
                }
                else
                {
                    mobilityContext_p->vgMfrcllckData.byPassSCriteria = byPassSCriteria;  
                }
           }

           /*Send signal */
           if (RESULT_CODE_OK == result)
           {
               mobilityContext_p->vgMfrcllckData.lockArfcnCmdMode = MM_LOCK_ARFCN_CMD_MODE_SET;
               chManagerContext_p->isImmediate         = TRUE;
            #if defined(USE_ABAPP)
               mobilityContext_p->isImmediate          = chManagerContext_p->isImmediate;
            #endif      
               result = vgChManContinueAction (entity, SIG_APEX_MM_LOCK_ARFCN_REQ);
           }
        }
        break;        
    case EXTENDED_QUERY:   /* AT*MFRCLLCK? */
        {          
            mobilityContext_p->vgMfrcllckData.lockArfcnCmdMode = MM_LOCK_ARFCN_CMD_MODE_READ;
            chManagerContext_p->isImmediate   = TRUE;
#if defined(USE_ABAPP)
            mobilityContext_p->isImmediate    = chManagerContext_p->isImmediate;
#endif
            result = vgChManContinueAction (entity, SIG_APEX_MM_LOCK_ARFCN_REQ);
        }
         break;
    case EXTENDED_RANGE:   /* AT*MFRCLLCK=? */
        {
            
        }
        break;
    default:
        {
            result = RESULT_CODE_ERROR;
        }
        break;

    } /* switch */

    return (result);

}

/*--------------------------------------------------------------------------
*
* Function:    viewMOOSAIND
*
* Parameters:  entity     - mux channel number
*
* Returns:     Nothing
*
* Description: Sends current status of EMM connection status.
*              If registered, location information elements lac and
*              cellId are also returned.
*
*-------------------------------------------------------------------------*/

void viewMOOSAIND (const VgmuxChannelNumber profileEntity,
                      const Boolean            atQuery)
{
    MobilityContext_t *mobilityContext_p = ptrToMobilityContext ();
    VgMOOSAINDData    *vgMoosaIndData       = &mobilityContext_p->vgMoosaIndData;

    vgPutNewLine (profileEntity);

    if ( atQuery == TRUE )
    {
        vgPrintf (profileEntity,
            (const Char *)"*MOOSAIND: %d,%d",
            getProfileValue(profileEntity, PROF_MOOSAIND), vgMoosaIndData->oosaStatus);
    }
    else
    {
        vgPrintf (profileEntity,
            (const Char *)"*MOOSAIND: %d", vgMoosaIndData->oosaStatus);
    }

    vgPutNewLine (profileEntity);
}

/*--------------------------------------------------------------------------
*
* Function:    vgMmMOOSAIND
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This executes command AT*MOOSAIND which enable/disable indication of OOSA event.
*-------------------------------------------------------------------------*/
ResultCode_t vgMmMOOSAIND(CommandLine_t *commandBuffer_p,
                                  const VgmuxChannelNumber entity)
{
    Int32               mode;
    Int32               param;
    ResultCode_t        result = RESULT_CODE_OK;

    ExtendedOperation_t      operation = getExtendedOperation (commandBuffer_p);
    switch (operation)
    {
        case EXTENDED_ASSIGN:      /* AT*MOOSAIND= */
        {
            /* Set Enable/Disable Info */
            if(getExtendedParameter ( commandBuffer_p,
                                      &mode,
                                      VG_MOOSAIND_NUMBER_OF_SETTINGS) == TRUE)
            {
                if(VG_MOOSAIND_NUMBER_OF_SETTINGS > mode)
                {
                    result = setProfileValue(entity, PROF_MOOSAIND, mode);
                }
                else
                {
                   result = VG_CME_INVALID_INPUT_VALUE;
                }
            }
            else
            {
                result = VG_CME_OPERATION_NOT_SUPPORTED;
            }
            break;
        }
        case EXTENDED_QUERY:      /* AT*MOOSAIND? */
        {
            viewMOOSAIND(entity, TRUE);
        }
        break;
        case EXTENDED_RANGE:      /* AT*MOOSAIND=? */
        {
            vgPutNewLine (entity);
            vgPrintf(entity, (const Char *)"*MOOSAIND: (%d-%d)",
                    VG_MOOSAIND_DISABLED, VG_MOOSAIND_ENABLED);
            vgPutNewLine (entity);
        }
      
        break;
        default:
            result = VG_CME_OPERATION_NOT_SUPPORTED;
        break;
    }
    return result;
}

/*--------------------------------------------------------------------------
*
* Function:    viewMENGINFORadioCellInfo
*
* Parameters:  entity     - mux channel number
*
* Returns:     Nothing
*
* Description: Sends current serving cell and neighbour cell information.
*
*-------------------------------------------------------------------------*/
void viewMENGINFORadioCellInfo (const VgmuxChannelNumber entity )
{
    Int8                i;
    MobilityContext_t   *mobilityContext_p = ptrToMobilityContext ();
    VgMENGINFOSCInfo    *vgSrvCellData_p   = &(mobilityContext_p->vgMengInfoData.curSrvCell);
    VgMENGINFONCInfo    *vgNbCellData_p    = &(mobilityContext_p->vgMengInfoData.neighbourCell);
    /*serving cell information is valild*/
    if (TRUE == vgSrvCellData_p->cellInfoValid)
    {
        vgPutNewLine (entity);

        /*print: <sc_earfcn>,<sc_earfcn_offset>,<sc_pci>*/
        vgPrintf (entity,
                (const Char *)"*MENGINFOSC: %d,%d,%d,",
                vgSrvCellData_p->cellInfo.earfcn,
                vgSrvCellData_p->cellInfo.earfcnOffset,
                vgSrvCellData_p->cellInfo.pci);
        /*print: <sc_cellid>   string type hex format*/
        vgPrintf (entity,
                (const Char *)"\"%X\",",
                vgSrvCellData_p->cellInfo.cellIdentity);
                
        /*Print: [<sc_rsrp>],[<sc_rsrq>],[<sc_rssi>],[<sc_sinr>]*/
        if (TRUE == vgSrvCellData_p->cellInfo.servingCellMeasResultsPresent)
        {
        vgPrintf (entity,
                (const Char *)"%d,%d,%d,%d,",
                vgSrvCellData_p->cellInfo.servingCellMeasResults.rsrp,
                vgSrvCellData_p->cellInfo.servingCellMeasResults.rsrq,
                vgSrvCellData_p->cellInfo.servingCellMeasResults.rssi,
                vgSrvCellData_p->cellInfo.servingCellMeasResults.snr );
        }
        else /*no serving cell meas info*/
        {
             vgPrintf (entity,
                (const Char *)",,,,");
        }   /*End serving measResult*/

        /*print: <sc_band>,<sc_tac>*/
        vgPrintf (entity,
                (const Char *)"%d,\"%04X\",",
                vgSrvCellData_p->cellInfo.band,
                vgSrvCellData_p->cellInfo.tac );

        /*print:[<sc_ecl>]*/
        if (TRUE == vgSrvCellData_p->cellInfo.eclPresent)
        {
             vgPrintf (entity,
                (const Char *)"%d,", vgSrvCellData_p->cellInfo.ecl);
        }
        else
        {
             vgPrintf (entity,
                (const Char *)",");
        }

        /*print:[<sc_tx_pwr>]*/
        if (TRUE == vgSrvCellData_p->cellInfo.txPowerPresent)
        {
             vgPrintf (entity,
                (const Char *)"%d", vgSrvCellData_p->cellInfo.txPower);
        }
       
        vgPutNewLine (entity);
    }   /*End: serving cell info valid*/
    else
    {
        /*If no service cell info, donothing*/
        return;
    }

    /* continue check neighbour cell info
     * *MENINFONC:<nc_earfcn>,<nc_earfcn_offset>,<nc_pci>,<nc_cellid>,<nc_rsrp>
     */
    for (i = 0; i < vgNbCellData_p->neighbourCellinfoNumber; i++)
    {   
        vgPrintf (entity,
                (const Char *)"*MENGINFONC: %d,%d,%d,%d",
                vgNbCellData_p->neighbourCellRadioInfoList[i].earfcn,
                vgNbCellData_p->neighbourCellRadioInfoList[i].earfcnOffset,
                vgNbCellData_p->neighbourCellRadioInfoList[i].pci,
                vgNbCellData_p->neighbourCellRadioInfoList[i].rsrp);
                
        vgPutNewLine (entity);
    }
    
} /*viewMENGINFORadioCellInfo*/

/*--------------------------------------------------------------------------
*
* Function:    viewDataTransferInfo
*
* Parameters:  entity     - mux channel number
*
* Returns:     Nothing
*
* Description: Sends data transfor information.
*
*-------------------------------------------------------------------------*/
void viewMENGINFODataTransferInfo (const VgmuxChannelNumber entity )
{      

    MobilityContext_t   *mobilityContext_p = ptrToMobilityContext ();
    VgMENGINFODTInfo    *vgDataTransData_p = &(mobilityContext_p->vgMengInfoData.dataTransfer);

    /* neighbour cell info
     * *MENINFONC:<RLC_UL_BLER>,<RLC_DL_BLER>,<MAC_UL_BLER>,<MAK_DL_BLER>,
     *            <MAC_UL_total_bytes>,<MAC_DL_total_bytes>,
     *            <MAC_UL_total_HARQ_TX>,<MAC_DL_total_HARQ_TX>,
     *            <MAC_UL_HARQ_re_TX>,<MAC_DL_HARQ_re_TX>,
     *            <RLC_UL_tput>,<RLC_UL_tput>,<MAC_UL_tput>,<MAC_DL_tput>
     */
    vgPutNewLine (entity);

    vgPrintf (entity,
            (const Char *)"*MENGINFODT: %d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
            vgDataTransData_p->dataTransferInfo.rlcUlBler,
            vgDataTransData_p->dataTransferInfo.rlcDlBler,
            vgDataTransData_p->dataTransferInfo.macUlBler,
            vgDataTransData_p->dataTransferInfo.macDlBler,
            vgDataTransData_p->dataTransferInfo.macUlTotalBytes,
            vgDataTransData_p->dataTransferInfo.macDlTotalBytes,
            vgDataTransData_p->dataTransferInfo.macUlTotalHarqTrans,
            vgDataTransData_p->dataTransferInfo.macDlTotalHarqTrans,
            vgDataTransData_p->dataTransferInfo.macUlHarqRetrans,
            vgDataTransData_p->dataTransferInfo.macDlHarqRetrans,
            vgDataTransData_p->dataTransferInfo.rlcUlThroughput,
            vgDataTransData_p->dataTransferInfo.rlcDlThroughput,
            vgDataTransData_p->dataTransferInfo.macUlThroughput,
            vgDataTransData_p->dataTransferInfo.macDlThroughput);

    vgPutNewLine (entity);
} /*viewDataTransferInfo*/

/*--------------------------------------------------------------------------
 *
 * Function:        vgMmMENGINFO
 *
 * Parameters:      commandBuffer_p - pointer to command line string
 *                        entity          - mux channel number
 *
 * Returns:          AT result code.
 *
 * Description:     AT command query current network status, and modem status information
 *                  for serving cell /neighbor cell /data transfer
 *-------------------------------------------------------------------------*/
 ResultCode_t vgMmMENGINFO(CommandLine_t *commandBuffer_p, const VgmuxChannelNumber entity)
{
    ResultCode_t                result             = RESULT_CODE_OK;
    ExtendedOperation_t         operation          = getExtendedOperation (commandBuffer_p);
    MobilityContext_t          *mobilityContext_p  = ptrToMobilityContext ();
    ChManagerContext_t         *chManagerContext_p = ptrToChManagerContext ();
    VgMENGINFOStatisData       *mengInfoData_p     = &(mobilityContext_p->vgMengInfoData);
    Int32                       paramValue;

    /* this function only support to read band now */
    switch (operation)
    {
    
    case EXTENDED_ASSIGN:  /* AT*MENGINFO=... */
        {
            Boolean parseResult = FALSE;

            parseResult = getExtendedParameter (commandBuffer_p,
                                            &paramValue,
                                            VG_MENGINFO_NUMBER_OF_QUERING);
            if ( TRUE == parseResult)
            {
                if (paramValue < VG_MENGINFO_NUMBER_OF_QUERING)
                {
                    mengInfoData_p->queryMode       = (VgMengInfoQuerMode)paramValue;
                    chManagerContext_p->isImmediate = TRUE;
                #if defined(USE_ABAPP)
                    mobilityContext_p->isImmediate  = chManagerContext_p->isImmediate;
                #endif     
                    result = vgChManContinueAction (entity, SIG_APEX_MM_UE_STATS_REQ);
                }
                else
                {
                    result = RESULT_CODE_ERROR;
                }
            }
            else
            {
                result = RESULT_CODE_ERROR;
            }
        }
        break;        
    case EXTENDED_QUERY:   /* AT*MENGINFO? */
        
        break;
    case EXTENDED_RANGE:   /* AT*MENGINFO=? */
        {
            vgPutNewLine (entity);
            vgPrintf(entity, (const Char *)"*MENGINFO: (%d-%d)",
                   VG_MENGINFO_QUERY_MODEM_INFO_RADIO_INFO, 
                   VG_MENGINFO_QUERY_MODEM_INFO_DATA_TRANSFER_INFO); 
            vgPutNewLine (entity);
            setResultCode (entity, RESULT_CODE_OK);
        }
        break;
    default:
        {
            result = RESULT_CODE_ERROR;
        }
        break;

    } /* switch */

    return (result);

}/*vgMmMENGINFO*/

/*--------------------------------------------------------------------------
 *
 * Function:        vgMmMCELLINFO
 *
 * Parameters:      commandBuffer_p - pointer to command line string
 *                        entity          - mux channel number
 *
 * Returns:          AT result code.
 *
 * Description:     AT command query current cell info,including serving cell and neighbour cell
 *-------------------------------------------------------------------------*/
 ResultCode_t vgMmMCELLINFO(CommandLine_t *commandBuffer_p, const VgmuxChannelNumber entity)
{
    ResultCode_t                result             = RESULT_CODE_OK;
    ExtendedOperation_t         operation          = getExtendedOperation (commandBuffer_p);
    MobilityContext_t          *mobilityContext_p  = ptrToMobilityContext ();
    ChManagerContext_t         *chManagerContext_p = ptrToChManagerContext ();

    /* this function only support to read cell info */
    switch (operation)
    {
      case EXTENDED_ACTION:  /* AT*MCELLINFO=... */
        {
            chManagerContext_p->isImmediate = TRUE;
            mobilityContext_p->isImmediate  = chManagerContext_p->isImmediate;
            result = vgChManContinueAction (entity, SIG_APEX_MM_LOC_CELL_INFO_REQ);
        }
        break;
    case EXTENDED_RANGE:   /* AT*MCELLINFO=? */
        break;
    default:
        {
            result = RESULT_CODE_ERROR;
        }
        break;

    } /* switch */

    return (result);

}/*vgMmMENGINFO*/


/*--------------------------------------------------------------------------
*
* Function:    vgMmMNBIOTEVENT
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: This executes command AT*MNBIOTEVENT which enable/disable indication of NBIOT related event.
*-------------------------------------------------------------------------*/
ResultCode_t vgMmMNBIOTEVENT(CommandLine_t *commandBuffer_p,
                                  const VgmuxChannelNumber entity)
{
    Int32               mode;
    Int32               param;
    Int8                 profileValue;
    ResultCode_t        result = RESULT_CODE_OK;

    ExtendedOperation_t      operation = getExtendedOperation (commandBuffer_p);
    switch (operation)
    {
      case EXTENDED_ASSIGN:      /* AT*MNBIOTEVENT= */
      {
        /* Get Enable/Disable Info */
        if(getExtendedParameter ( commandBuffer_p,
                                    &mode,
                                    VG_MNBIOTEVENT_NUMBER_OF_SETTINGS) == TRUE)
        {
            if(VG_MNBIOTEVENT_NUMBER_OF_SETTINGS > mode)
            {
                if(getExtendedParameter ( commandBuffer_p,
                                                        &param,
                                                        ULONG_MAX) == TRUE)
                {
                   if(param != ULONG_MAX)
                   {
                      if(mode == VG_MNBIOTEVENT_DISABLED)
                      {
                          profileValue = (~param) & (getProfileValue(entity, PROF_MNBIOTEVENT)); 
                      }
                      else
                      {
                          profileValue = param | (getProfileValue(entity, PROF_MNBIOTEVENT));
                      }
                      result = setProfileValue(entity, PROF_MNBIOTEVENT, profileValue);
                   }
                   else
                   {
                      result = VG_CME_INVALID_INPUT_VALUE;
                   }
                }
                else
                {
                    result = VG_CME_INVALID_INPUT_VALUE;
                }
            }
            else
            {
               result = VG_CME_INVALID_INPUT_VALUE;
            }
        }
        else
        {
            result = VG_CME_OPERATION_NOT_SUPPORTED;
        }
        break;
     }
     case EXTENDED_QUERY:
     case EXTENDED_RANGE:
        break;
     default:
        result = VG_CME_OPERATION_NOT_SUPPORTED;
        break;
  }
    return result;
}

#if defined (FEA_RPM)
/*--------------------------------------------------------------------------
 *
 * Function:    vgMmMRPM
 *
 * Parameters:  commandBuffer_p - pointer to command line string
 *              entity          - mux channel number
 *
 * Returns:     Resultcode_t    - result of function
 *
 * Description: Executes command AT*MRPM
 *
 *-------------------------------------------------------------------------*/

ResultCode_t vgMmMRPM (CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity)
{
  ExtendedOperation_t   operation = getExtendedOperation (commandBuffer_p);
  ResultCode_t          result = RESULT_CODE_OK;
  Int32                 param;
  MobilityContext_t     *mobilityContext_p   = ptrToMobilityContext ();
  VgMrpmData            *vgMrpmData_p     = &(mobilityContext_p->vgMrpmData);

  switch (operation)
  {
    case EXTENDED_ASSIGN:    /* *MRPM= */
    {
        if (getExtendedParameter (commandBuffer_p,
                &param,
                VG_MRPM_NUMBER_OF_SETTINGS) == TRUE)
        {
            if (param < VG_MRPM_NUMBER_OF_SETTINGS)
            {
                vgMrpmData_p->enable = param;
            }
            else
            {
                result = RESULT_CODE_ERROR;
            }
        }
        else
        {
            result = RESULT_CODE_ERROR;
        }
        break;
    }
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }

  return (result);
}
/*--------------------------------------------------------------------------
 *
 * Function:        vgMmMRPMR
 *
 * Parameters:      commandBuffer_p - pointer to command line string
 *                  entity          - mux channel number
 *
 * Returns:         AT result code.
 *
 * Description:     AT command to  read rpm parameter
 *
 *-------------------------------------------------------------------------*/
 ResultCode_t vgMmMRPMR (CommandLine_t *commandBuffer_p, const VgmuxChannelNumber entity)
 {
    ResultCode_t        result               = RESULT_CODE_OK;
    MobilityContext_t   *mobilityContext_p   = ptrToMobilityContext ();
    Int32               param;
    VgMrpmData          *vgMrpmData_p     = &(mobilityContext_p->vgMrpmData);
    ChManagerContext_t      *chManagerContext_p       = ptrToChManagerContext ();

   switch (getExtendedOperation (commandBuffer_p))
    {
        case EXTENDED_ASSIGN: /* AT+NFM=... */
        {
            /* Get NFM Active */
            if( (getExtendedParameter ( commandBuffer_p,
                                        &param,
                                        ULONG_MAX) == TRUE) &&
            (param != ULONG_MAX))
            {
                vgMrpmData_p->read_area= param;
            }
            else
            {  /* no input so ERROR */
               result = VG_CME_INVALID_INPUT_VALUE;
            }
            if (result == RESULT_CODE_OK)
            {
              chManagerContext_p->isImmediate = TRUE;
              mobilityContext_p->isImmediate  = chManagerContext_p->isImmediate;
              vgApexReadRpmReq(entity);
              result = RESULT_CODE_PROCEEDING;

            }
        }
        break;

        default:
        {
            result = RESULT_CODE_ERROR;
        }
        break;
    }
    return (result);
}

#endif
/*--------------------------------------------------------------------------
 *
 * Function:        vgMmMHPLMNS
 *
 * Parameters:      commandBuffer_p - pointer to command line string
 *                  entity          - mux channel number
 *
 * Returns:         AT result code.
 *
 * Description:     AT command set hplmn search parameter
 *
 *-------------------------------------------------------------------------*/

ResultCode_t vgMmMHPLMNS (CommandLine_t *commandBuffer_p, const VgmuxChannelNumber entity)
{
    ResultCode_t            result             = RESULT_CODE_OK;
    ExtendedOperation_t     operation          = getExtendedOperation (commandBuffer_p);
    MobilityContext_t      *mobilityContext_p  = ptrToMobilityContext ();
    ChManagerContext_t     *chManagerContext_p = ptrToChManagerContext ();
    Int32                   param;
    VgMHPLMNSData          *vgMHPLMNSData = &(mobilityContext_p->vgMhplmnData);
    /* this function only support to read band now */
    switch (operation)
    {
       case EXTENDED_ASSIGN:  /* AT*MHPLMNS=... */
       {
           if( (getExtendedParameter ( commandBuffer_p,
                                     &param, ULONG_MAX) == FALSE) ||
             (!((param == 0) ||
                (param == 1))))
            {
               result = VG_CME_INVALID_INPUT_VALUE;
            }
            else
            {
                vgMHPLMNSData->enable = param;
            }
            if (result == RESULT_CODE_OK)
            {
                chManagerContext_p->isImmediate   = TRUE;
                mobilityContext_p->isImmediate    = chManagerContext_p->isImmediate;
                result = vgChManContinueAction (entity, SIG_APEX_MM_DISABLE_HPLMN_SEARCH_REQ);
            }
         
        }
        break;
        default:
        {
            result = RESULT_CODE_ERROR;
        }
        break;

    } /* switch */

    return (result);

}

/*--------------------------------------------------------------------------
 *
 * Function:        vgMmMEHPLMN
 *
 * Parameters:      commandBuffer_p - pointer to command line string
 *                  entity          - mux channel number
 *
 * Returns:         AT result code.
 *
 * Description:     AT command set Ehplmn list
 *
 *-------------------------------------------------------------------------*/

 ResultCode_t vgMmMEHPLMN (CommandLine_t *commandBuffer_p, const VgmuxChannelNumber entity)
{
    ResultCode_t            result             = RESULT_CODE_OK;
    ExtendedOperation_t     operation          = getExtendedOperation (commandBuffer_p);
    MobilityContext_t      *mobilityContext_p  = ptrToMobilityContext ();
    ChManagerContext_t     *chManagerContext_p = ptrToChManagerContext ();
    VgMEHPLMNData           *vgMEHPLMNData = &(mobilityContext_p->vgMehplmnData);
    Char                    networkOperator[PLMN_NAME_FULL_LENGTH + NULL_TERMINATOR_LENGTH] = {0};
    Int32                   param;
    Int8                    i = 0;      
    Int16                   length;
    
    /* this function supports to set EHPLMN list */
    switch (operation)
    {
       case EXTENDED_ASSIGN:  /* AT*MEHPLMN=... */
       {
           if((getExtendedParameter ( commandBuffer_p,
                                     &param, ULONG_MAX) == TRUE) &&
             (param <= MAX_NUM_USER_CFG_EHPLMN_LIST) &&
             (param != ULONG_MAX))
            {
                vgMEHPLMNData->numEhplmn = (Int8)param;
            }
            else
            {
                result = VG_CME_INVALID_INPUT_VALUE;
            }
            
            if (result == RESULT_CODE_OK)
            {
                for(i = 0; i<vgMEHPLMNData->numEhplmn; i++)
                {
                    if (getExtendedString (commandBuffer_p,
                            networkOperator,
                            PLMN_NAME_FULL_LENGTH,
                            &length) == TRUE)
                    {
                        if(TRUE == vgOperStringToPlmn(networkOperator,&(vgMEHPLMNData->ehplmnList[i]),&length))
                        {
                            if(length == (MCC_DIGIT_LENGTH + MNC_DIGIT_LENGTH_MAX))
                            {
                                vgMEHPLMNData->ehplmnList[i].is_three_digit_mnc = TRUE;
                            }
                            else
                            {
                                vgMEHPLMNData->ehplmnList[i].is_three_digit_mnc = FALSE;
                            }
                            memset(networkOperator, NULL_CHAR, (PLMN_NAME_FULL_LENGTH + NULL_TERMINATOR_LENGTH) * sizeof (Char));
                        }
                        else
                        {
                            result = VG_CME_INVALID_INPUT_VALUE;
                            break;
                        }
                    }
                }
            }

            if(result == RESULT_CODE_OK)
            {
                chManagerContext_p->isImmediate   = TRUE;
                mobilityContext_p->isImmediate    = chManagerContext_p->isImmediate;
                result = vgChManContinueAction (entity, SIG_APEX_MM_SET_EHPLMN_REQ);

                if(result == RESULT_CODE_PROCEEDING)
                {
                    result = RESULT_CODE_OK;
                }
            }
        }
        break;
        
        case EXTENDED_RANGE:   /* AT*MEHPLMN=? */
        {
            vgPutNewLine (entity);
            vgPrintf(entity, (const Char *)"*MEHPLMN: (0-%d)",
                   MAX_NUM_USER_CFG_EHPLMN_LIST);
            vgPutNewLine (entity);
        }
        break;
        
        default:
        {
            result = RESULT_CODE_ERROR;
        }
        break;

    } /* switch */

    return (result);

}
/***************************************************************************
 * Processes
 ***************************************************************************/

/* END OF FILE */

