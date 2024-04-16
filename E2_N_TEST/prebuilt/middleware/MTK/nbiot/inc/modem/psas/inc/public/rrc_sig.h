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
 * File Description:                                                      */

/** \file
 * 3G PS Signal Data Type definitions for the RRC Interface
 **************************************************************************/

#ifndef RRC_SIG_H
#define       RRC_SIG_H

/***************************************************************************
* Nested Include Files
***************************************************************************/

#include <uas_asn.h>
#include <ups_typ.h>
#include <sir_typ.h>
#include <grrmrtyp.h>
#include <as_nas_common_typ.h>
#include <l3gp_typ.h>
#include <rrc_typ.h>
#include <cphy_sig.h>
#include <cmac_sig.h>
#include <urrmtcem.h>       /* 3G - RRC Engineering mode header file */
#include <utper.h>
#include <urlc_sig.h>
#include <ubnd_cfg.h>
#include <rrcmrtyp.h>
#include <u1_typ.h> /* include temp for CellIdentificationUnion to be removed once UPHY handled */
#if defined (UPGRADE_LTE)
#include <eps_typ.h>
#endif

/** \defgroup SigRrc RRC Signalling Interface
 * \ingroup Prd3gDmProtoStack
 * \ingroup Sig
 * This is a design document for the signal interface Mobility Management (MM/GMM/UMM)
 * and 3GPP Radio Resource Control (URRC) for Mediatek's 3GPP (3rd Generation
 * Partnership Project) product. The interface between URRC and MM is
 * known as the RRC interface shown in this \ref PSInterfaceFigure
 * It describes the signals sent between these two entities in terms of data
 * structures and data values, and provides a summary of the purpose of each
 * signal, when it may be expected to occur, and how it relates to other
 * signals on the same interface.
 *
 * The Mobility Management entity of the protocol stack embodies three elements,
 * the Mobility Management functionality for GSM (MM), that for GPRS (GMM) and
 * that for 3GPP (UMM).
 *
 * This document refers to the Mobility Management entity as MM throughout,
 * meaning the above functionality taken as a whole, unless it is necessary
 * to indicate functionality specific to one of the above systems.
 *
 * The Radio Resource functionality referred to by this document is that
 * for the 3GPP Radio Resource Control (RRC) entity which is referred to as RRC.
 *
 * This document incorporates requirements found in [3G TS 24.007].
 * @{
 */

/***************************************************************************
*   Manifest Constants
***************************************************************************/

/** Defines maximum number of frequencies. */
#define MAX_FREQUENCIES                    16

/** Defines maximum number of frequencies in RRC_CONN_REL_IND signal. */
#define MAX_FREQ_IN_RRC_CONN_REL_IND       8

/** Defines maximum number of BA Range in RRC_CONN_REL_IND signal. */
#define MAX_GSM_BA_RANGE_IN_RRC_CONN_REL_IND 32

#if defined (ENABLE_AT_TRACE)
/** Defines maximun number of AT trace data number. */
#define MAX_AT_TRACE_DATA   300
#endif /* ENABLE_AT_TRACE */

/**-------------------------------------------------------------------*
 ** Scaling factors to be used for all dB and dBm measurements.
 */
#define RRC_PHY_DB_SCALING_FACTOR          ((SignedInt8) 1)

/** Defines the scaling factors to be used for all dB and dBm measurements. */
#define RRC_SYS_INFO_DB_SCALING_FACTOR     MEASUREMENTS_TO_DB_SCALING_FACTOR

/***************************************************************************
*   Macro Functions
***************************************************************************/

/***************************************************************************
*   Types
***************************************************************************/

# define UENG_MAX_FREQUENCE_NUMBER                   20
#define UENG_MAX_REPORT_CELL_NUMBER                 8

/** Engineering mode mandatory command type */
typedef enum UEngForceCommandTag
{
    UENG_LOCK_CELL,
    UENG_UNLOCK_CELL,
    UENG_LOCK_ARFCN,
    UENG_UNLOCK_ARFCN,
    UENG_FULL_SCAN,
    UENG_SPECIFIED_ARFCN_SCAN,
    UENG_TRIGGER_HANDOVER,
    UENG_HANDOVER_PERMITTED,
    UENG_TRIGGER_CELL_RESELECTION,
    UENG_SELECT_A_CELL_ONE_TIME,
    UENG_NO_COMMAND
}
UEngForceCommand;

/** Engineering mode mandatory command result type */
typedef enum UEngForceCommandResultTypeTag
{
    UENG_CMD_RESULT_FULL_SCAN,
    UENG_CMD_RESULT_SPECIFIED_ARFCN_SCAN,
    UENG_CMD_NO_COMMAND
}
UEngForceCommandResultType;

/** Engineering mode Force command type */
typedef enum UEngForceCommandFailCauseTag
{
    UENG_CELL_NORMAL_SERVICE = 0,

    /** Command can NOT be overrided. */
    UENG_CMD_OVERRIDED,

    /** Command is not permitted because of CSR status.*/
    UENG_CMD_FORBIDDEN,

    /** ACCEPTABLE cell */
    UENG_CELL_LIMITED_SERVICE,

    /** Target cell can'be camped on suitable */
    UENG_CELL_NOT_CAMP,

    /** Target cell is not valid  */
    UENG_CELL_NOT_VALID,

    /** This command only support in IDLE MODE
    */
    UENG_UE_NOT_IN_IDLE_MODE,

    /** This command only support in CSRR Proc
    */
    UENG_UE_NOT_IN_CSRR_PROC,

    /** In first step, if RBC,CER should process SIR msg or
    * pending request exist, scan will be blocked
    */
    UENG_SCAN_BLOCKED,

    /** Indicate command can't be processed in current state.
      */
    UENG_STATE_MISMATCH,

    /** Indicate target cell is not requested one.
      */
    UENG_OCCURRED_BUT_CELL_MISMATCH,

    /** Indicate current camp on cell is locked.
      */
    UENG_CELL_LOCKED,

    /** Indicate current camp on cell whose arfcn is locked.
      */
    UENG_ARFCN_LOCKED,

    /** Indicate request handover is not occurred during specified timer.
      */
    UENG_NOT_OCCURRED_WHEN_TIMER_EXPIRY,

    /** Indicate target cell's PLMN is not belongs to serving cell's PLMN or EPLMNs.
      */
    UENG_PLMN_MISMATCH,

    /** Can be used for all commands,
      * Indicate command process failure for unspecified cause.
      */
    UENG_UNSPECIFIED_CAUSE
}
UEngForceCommandFailCause;

/** Engineering mode NCELL type, Only used when command is
  * 'TRIGGER_HANDOVER' or 'TRIGGER_CELL_RESELECTION'.
  */
typedef enum UEngNCellTypeTag
{
    /** Invalid value  */
    UENG_NCELL_INVALID_TYPE = 0,

    /** Inter-freq neighbour cell */
    UENG_NCELL_INTER_FREQ,

    /** Inter-rat neighbour cell */
    UENG_NCELL_INTER_RAT,

    /** Intra-freq neighbour cell */
    UENG_NCELL_INTRA_FREQ
}
UEngNCellType;

/**
 * Engineering mode information about
 * an arfcn rssi inf.
 */
typedef struct UEngArfcnRssiInfoTag
{
    /** The DL ARFCN of the cell */
    Int16 arfcn_dl;

    /** Received RSSI  */
    SignedInt16 rssi;
}
UEngArfcnRssiInfo;

/**
 * Engineering mode information about
 * All frequency scan result.
 */
typedef struct UEngScanResultListTag
{
    /** The number of scan frequency */
    Int16                    arfcnNumber;

    /** All frequency scan rssi result */
    UEngArfcnRssiInfo       arfcnRssiInfoList[UENG_MAX_FREQUENCE_NUMBER];
}
UEngScanResultList;

typedef  union UEngScanCellInfoUnionTag
{
#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
    /** Indicates the FDD Cell Identifier and Results.*/
    FddCellInfo                         fdd[UENG_MAX_REPORT_CELL_NUMBER];
#endif
#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
    /** Indicates the TDD128 Cell Identifier and Results.*/
    Tdd128CellInfo                      tdd128[UENG_MAX_REPORT_CELL_NUMBER];
#endif
}
UEngScanCellInfoUnion;

/**
 * Engineering mode information about
 * specified arfcn scan result.
 */
typedef struct UEngSpecifiedScanResultListTag
{
    /** The number of found cells, max number is 8
\assoc ARRAY \ref mode.fdd \opt CONDITIONAL
\assoc ARRAY \ref mode.tdd128 \opt CONDITIONAL */
    Int8                    cellNumber;

    /** UmtsMode: FDD or TDD128.
\assoc UNION \ref mode */
    UmtsMode                umtsMode;

    /** Scan cell info. */
    UEngScanCellInfoUnion   mode;
}
UEngSpecifiedScanResultList;

/**
 * Engineering mode command result information.
 */
typedef union UEngCommandResultInfoUnionTag
{
    /** For command 'FULL_SCAN',
     * Full band scan result information list.
     */
    UEngScanResultList              fullScanResultList;

    /** For command 'SPECIFIED_ARFCN_SCAN',
      * Carries the contents of specified arfcn scan result information.
      */
    UEngSpecifiedScanResultList     specifiedScanResultList;
}
UEngCommandResultInfoUnion;

/** Indicated type of the cell identity */
typedef enum UCellIdentityTypeTag
{
    /** Cell identified using LAI*/
    LOCATION_AREA_IDENTITY = 0,

    /**Cell identified using RAI*/
    ROUTING_AREA_IDENTITY = 1
}
UCellIdentityType;

/** Indicates priority of the NAS message. */
typedef enum UNAS_MessagePriorityTag
{
    /** NAS message has low priority. */
    UNAS_LOW_PRIORITY = 0,

    /** NAS message has high priority. */
    UNAS_HIGH_PRIORITY = 1
}
UNAS_MessagePriority;

/** Indicates if integrity protection was checked for the message.
 * If so it indicates if it was successful or not.
 */
typedef enum UIntegrityProtectionInfoTag
{
    /** Indicates the integrity check was not performed. */
    INTEGRITY_CHECK_NOT_PERFORMED = 0,

    /** Indicates the integrity check was successful. */
    INTEGRITY_CHECK_SUCCESSFUL = 1,

    /** Indicates the integrity check was not successful. */
    INTEGRITY_CHECK_UNSUCCESSFUL = 2
}
UIntegrityProtectionInfo;

/** Indicates if the integrity protection is started or not. */
typedef enum UrrAisIntegrityProtectionStartedTag
{
    /** Indicates that the integrity protection is started. */
    INTEGRITY_PROTECTION_STARTED,

    /** Indicates that the integrity protection is not started. */
    INTEGRITY_PROTECTION_NOT_STARTED
}
UrrAisIntegrityProtectionStarted;

/** Defines URRC status. */
typedef enum URrcStatusTag
{
    /** Connection to invalid cell. */
    CONNECTION_TO_INVALID_CELL_ERROR = 0,

    /** PLMN found during SNIFF */
    PLMN_FOUND_DURING_SNIFF = 1,

    /** No signalling connection for the CS domain
     * indicated in the RrcDataReq.
     */
    RRC_DATA_REQ_CS_DOMAIN_FAILED = 2,

    /** No signalling connection for the PS domain
     * indicated in the RrcDataReq.
     */
    RRC_DATA_REQ_PS_DOMAIN_FAILED = 3
}
URrcStatus;

/** Indicates the status of establishment and istates the
 * reason in case of failure the.
 */
typedef enum UEstablishStatusTag
{
    /** Establish success. */
    ESTABLISH_SUCCESS = 0,

    /** Failure of RACH establishment. */
    ESTABLISH_FAIL_RACH_FAILURE = 1,

    /** RRC connection failed. */
    ESTABLISH_FAIL_CONFIG_ERROR = 2,

    /** RRC CONNECTION REQUEST is sent max times */
    ESTABLISH_FAIL_MAX_RETRIES = 3,

    /** The establishment request is rejected because the serving
     * cell is barred.
     */
    ESTABLISH_FAIL_REJECTED = 4,

    /** PHY synchronization has faild. */
    ESTABLISH_FAIL_PHY_SYNC_FAILURE = 5,

    /** Establishement has faild due to invalid CN domain. */
    ESTABLISH_FAIL_INVALID_CN_DOMAIN = 6,

    /** The access class for current cell prevents the
     * connection to be established.
     */
    ESTABLISH_FAIL_ACCESS_CLASS_BARRED = 7,

    /** No cell found. */
    ESTABLISH_FAIL_NO_CELL = 8,

    /** LAI has changed */
    ESTABLISH_FAIL_LAI_HAS_CHANGED = 9,

    /** The C_RNTI is not present */
    ESTABLISH_FAIL_C_RNTI_NOT_PRESENT = 10,

    /** RAC has changed */
    ESTABLISH_FAIL_RAC_HAS_CHANGED = 11,

}
UEstablishStatus;

/** Indicates reason of the connection release. */
typedef enum UConnRelReasonTag
{
    /** Normal connection release. */
    CONN_REL_NORMAL = 0,

    /** Authorisation for connection release is faild. */
    CONN_REL_AUTHORISATION_FAILED = 1
}
UConnRelReason;

/** This information element contains system information that belongs
 * to the non-access stratum for a GSM-MAP type of PLMN.
 * This information is transparent to RRC. It may contain either information
 * specific to one CN domain (CS or PS) or information common for both CN domains.
 */
typedef struct UCN_CommonSysInfoGSM_MAPTag
{
    /** \subsection LacDefinition LAC Definition
     * LAC, Location area code (octet 5 and 6)
     * In the LAC field bit 8 of octet 5 is the most significant
     * bit and bit 1 of octet 6 the least significant bit.
     */
    Lac lac;
}
UCN_CommonSysInfoGSM_MAP;

/** Defines cs domain specific info. The purpose of the CN
 * domain specific GSM-MAP NAS system information element,
 * when used for the CS domain, is to provide the MS with actual
 * parameter settings of parameters relevant only for MM functionality.
 *
 * The content of the CN domain specific GSM-MAP NAS system information
 * element is coded as shown in figure and Table below.
 *
 * \image html UCS_DomainSpecificSysInfo.png "CS domain specific system
 * information element"
 *
 * <TABLE>
 * <TR><TD><B>Parameter</B></TD>    <TD><B>Description              </B></TD></TR>
 * <TR><TD>T3212 timeout value (1 octet field)</TD><TD>The T3212 timeout field is
 * coded as the binary representation of the timeout value for periodic updating
 * in decihours. Bit 8 in octet 1 is the most significant bit and bit 1 in octet
 * 1 is the least significant bit.
 *
 * Range: 1 to 255
 *
 * The value 0 is used for infinite timeout value i.e. periodic updating shall not
 * be used</TD></TR>
 * <TR><TD>ATT, Attach-detach allowed (1 bit field)</TD><TD>Bit 1:
 *
 * 0  MSs shall not apply IMSI attach and detach procedure.
 *
 * 1  MSs shall apply IMSI attach and detach procedure
 *
 * The bits 2 ?8 of octet 2 are spare and shall be coded all zeros.</TD></TR>
 * </TABLE>
 */
typedef struct UCS_DomainSpecificSysInfoTag
{
    /** Coded as an 8 bit number contained in the first octet. */
    Int8 T3212;

    /** IMSI attach detach allowed flag coded as a 1 bit number
     * contained in the LSB of the second byte.
     */
    Boolean attachDetachAllowed;
}
UCS_DomainSpecificSysInfo;

/** Defines ps domain specific info. The purpose of the CN domain
 * specific GSM-MAP NAS system information element, when used for
 * the PS domain, is to provide the MS with actual parameter settings
 * of parameters relevant only for GMM functionality.
 *
 * The content of the CN domain specific GSM-MAP NAS system information
 * element is coded as shown in figure and Table below.
 * \subsection RacDefinition RAC Definition
 * \image html UPS_DomainSpecificSysInfo.png "PS domain specific system
 * information element"
 *
 * <TABLE>
 * <TR><TD><B>Parameter</B></TD>    <TD><B>Description      </B></TD></TR>
 * <TR><TD>RAC, Routing Area Code (8 bit field)</TD><TD>This field is the
 * binary representation of the Routing Area Code, see 3GPP TS 23.003.
 * Bit 8 in octet 1 is the most significant bit and bit 1 in octet 1 is
 * the least significant bit.</TD></TR>
 * <TR><TD>NMO, Network Mode of Operation (1 bit field) </TD><TD> This field
 * is the binary representation of the Network Mode of Operation, see 3GPP TS 23.060
 *
 * Bit  1:
 *
 * 0  Network Mode of Operation I
 * 1  Network Mode of Operation II
 *
 * The bits 2 ?8 of octet 2 are spare and shall be coded all zeros. </TD></TR>
 * </TABLE>
 */
typedef struct UPS_DomainSpecificSysInfoTag
{
    /** Identifies a routing area within a location area for a GSM-MAP
     * type of PLMN.
     */
    Rac rac;

    /** the Network Mode of OPeration coded as a 1 bit number
     * contained in the LSB of the second octet
     */
    GprsNetworkMode gprsNetworkMode;
}
UPS_DomainSpecificSysInfo;

/** Indicates the available FDD downlink frequencies for the last
 * registered PLMN.
 */
typedef struct FrequencyInformationTag
{
    /** Defines the number of frequencies in uarfcn_dl [] and is in
     * the range 0 to 8.

\assoc ARRAY \ref uarfcn_dl */
    Int8    numberOfFrequencies;

    /** Defines the list of the downlink frequencies.*/
    UUARFCN uarfcn_dl [MAX_FREQ_IN_RRC_CONN_REL_IND];
}
FrequencyInformation;

/** Define which FDD/TDD128 band to use. */
typedef union BandModeUnionTag
{
#ifdef UPGRADE_3G_FDD
    /** Indicates which FDD band to use. */
    FddBandMode    fddBandMode;
#endif
#ifdef UPGRADE_3G_TDD128
    /** Indicates which TDD128 band to use. */
    Tdd128BandMode tdd128BandMode;
#endif
}
BandModeUnion;

/** Contains system information. */
typedef struct USystemInformationTag
{
    /** Indicates cell identity. */
    UCellIdentity cellIdentity;

    /** Type for Public Land Mobile Network definition.*/
    Plmn          plmn;

    /** Indicates if MNC has three digits. */
    Boolean       threeDigitMncEncoding;

    /** Indicates which access classes are barred. Bit 0 corresponds
     * to AC0 and a 0 indicates barred and a 1 indicated not barred.
     */
    Int16                     barredAccessClasses;

#if defined (UPGRADE_3G_FDD) && defined (UPGRADE_REL6)
    /** Contains multiple PLMNs sharing the current cell */
    PlmnList                  mPlmnList;

    /** Indicates which domain specific access classes are barred. Bit 0 corresponds
     * to AC0 and a 0 indicates barred and a 1 indicated not barred.
     */
    /** indicate if the DSAC present in the cell
      \assoc PRESENT \ref barredDomainSpecificAccessClasses */

    Boolean                   barredDomainSpecificAccessClassesPresent;
    Int16                     barredDomainSpecificAccessClasses[maxCNdomains];
#endif

    /** Contains the common NAS system information. */
    UCN_CommonSysInfoGSM_MAP  cn_CommonSysInfoGSM_MAP;

    /** Indicates if the NAS system information for CS domain is present
      \assoc PRESENT \ref cs_DomainSpecificSysInfo */
    Boolean                   cs_DomainSpecificSysInfoPresent;

    /** Contains the cs domain specific NAS system information which is optional. */
    UCS_DomainSpecificSysInfo cs_DomainSpecificSysInfo;

    /** Indicates if the NAS system information for PS domain is present
      \assoc PRESENT \ref ps_DomainSpecificSysInfo */
    Boolean                   ps_DomainSpecificSysInfoPresent;

    /** Contains the ps domain specific NAS system information which is optional. */
    UPS_DomainSpecificSysInfo ps_DomainSpecificSysInfo;

    /** Defines UMTS mode FDD/TDD128.
\assoc UNION \ref currentBandMode */
    UmtsMode                  currentUmtsMode;
#ifdef UPGRADE_3G_FDD
    /** Defines FDD cell information. */
    FddCellInformation        fddCellInformation;
#endif
#ifdef UPGRADE_3G_TDD128
    /** Defines TDD128 cell information. */
    Tdd128CellInformation     tdd128CellInformation;
#endif
#if defined (UPGRADE_3G_HSDPA)
    /** Hsdpa Cell Indicator */
    Boolean                   hsdpaCellIndicator;
#endif /* UPGRADE_3G_HSDPA */
#if defined (UPGRADE_3G_HSUPA)
    /** Hsupa Cell Indicator */
    Boolean                   hsupaCellIndicator;
#endif /* UPGRADE_3G_HSUPA */
#if defined (UPGRADE_3G)
# if defined (UPGRADE_3G_MBMS)
#  if defined (UPGRADE_3G_TDD128)
    Boolean mbmsCellIndicator;
#  endif /*UPGRADE_3G_TDD128*/
# endif /* UPGRADE_3G_MBMS */
#endif /* UPGRADE_3G */

    /** Defines GSM cell information. */
    GsmCellInformation gsmCellInformation;

    /** Indicates which band type to use. */
    BandModeUnion      currentBandMode;
}
USystemInformation;

/** This defines the PLMN code and Location Area Code (LAC) for a
 * location area
 */
typedef struct ULocationAreaIdentityTag
{
    /** Type for Public Land Mobile Network definition.*/
    Plmn  plmn;

    /**  See \ref LacDefinition. */
    Int16 lac;
}
ULocationAreaIdentity;

/** This defines the PLMN code and Routing Area Code (RAC) for a routing area.
 * The MS shall construct the Routing Area Identification of the cell from this
 * chosen PLMN identity, and the LAC and the RAC received on the BCCH.
 */
typedef struct URoutingAreaIdentityTag
{
    /** Identifies uniquely a location area for a GSM-MAP type of PLMN */
    ULocationAreaIdentity lai;

    /** Identifies a routing area within a location area for a GSM-MAP
     * type of PLMN. see \ref RacDefinition for further details*/
    URoutingAreaCode      rac;
}
URoutingAreaIdentity;

/** Contains either the LAI or RAI depending on the setting of
 * cellIdentityType.
 */
typedef struct UAreaIdentityTag
{
    /** Defines whether this is a location or a routing area. */
    UCellIdentityType cellIdentityType;

    /** Contains either the LAI or RAI depending on the setting of
     * cellIdentityType.
     */
    URoutingAreaIdentity rai;
}
UAreaIdentity;

/** Defines CS or PS domain identity. */
typedef struct UDomainIdentityTag
{
    /** Defines whether this is a PS or CS domain. */
    UCN_DomainIdentity cn_DomainIdentity;

    /** Indicates the LAI or RAI as defined above. */
    UAreaIdentity      areaIdentity;
}
UDomainIdentity;

/** Defines the length of cipher key */
#define RRC_CIPHER_KEY_ARRAY_LENGTH        16

/** Defines the length of integrity key */
#define RRC_INTEGRITY_KEY_ARRAY_LENGTH     16

/** Key sequence that identifies the IK and CK key set. */
typedef enum RrcKeySequenceTag
{
    /** Not used in the source code. */
    UPS_KEY_SEQUENCE_0 = 0,

    /** Not used in the source code. */
    UPS_KEY_SEQUENCE_1 = 1,

    /** Not used in the source code. */
    UPS_KEY_SEQUENCE_2 = 2,

    /** Not used in the source code. */
    UPS_KEY_SEQUENCE_3 = 3,

    /** Not used in the source code. */
    UPS_KEY_SEQUENCE_4 = 4,

    /** Not used in the source code. */
    UPS_KEY_SEQUENCE_5 = 5,

    /** Not used in the source code. */
    UPS_KEY_SEQUENCE_6 = 6,

    /** Not available. */
    UPS_NO_KEY_SEQUENCE_AVAILABLE = 7
}
RrcKeySequence;

/** the ba range information used in GSM system */
typedef struct RrcGsmBaRangeElementTag
{
    /** define the number of the gsm ba range
\assoc ARRAY \ref gsmBaRange */
    Int8    numOfGsmBaRange;
    /** the gsm ba range information */
    BaRange gsmBaRange[MAX_GSM_BA_RANGE_IN_RRC_CONN_REL_IND];

}RrcGsmBaRangeElement;

/** This primitive defines the START info. */
typedef struct RrcStartInformationTag
{
    /** Defines the CN domain identity the start information
     * applies to.
     */
    UCN_DomainIdentity cn_DomainIdentity;

    /** Represents the 20 bit START value calculated when the RRC
     * connection is released.
     */
    USTART_Value startValue;

    /** Is the KSI that identifies the IK and CK key set that
     * was most recently activated for the CN domain.
     */
    RrcKeySequence keySetIdentifier;
}
RrcStartInformation;

/** This indicates if connection establishment has been triggered by
 * an inter-RAT change  or Cell change order to UTRAN.
 */
typedef enum UMmProcReasonTag
{
    /** Inter-Rat cell change order. */
    UMM_PROC_DUE_TO_CCO,

    /** Inter-RAT cell reselection to UTRAN. */
    UMM_PROC_DUE_TO_RESEL,

    /** No reason. */
    UMM_PROC_NORMAL
}
UMmProcReason;

/** \defgroup SigRrcSignals  RRC Primitives
 * Primitives exchanged between URRC and MM.
 * This section defines primitives exchanged between URRC and MM
 *
 * @{
 *
 */

/** This primitive is sent to URRC by MM.
 * For the first attempt at initial cell selection MM will send this signal
 * to URRC with requestedPlmnValid set FALSE.  This will allow the UE to
 * search for cells before information from the USIM is available and before
 * the user has entered a PIN number.
 *
 * Subsequently MM will send this signal to URRC with requestedPlmnValid
 * set TRUE and the PLMN to be searched for in requestedPlmn.  A list of
 * frequency information may also be present in fddCellInformation.
 * The result of the search will be returned to MM in signal RrcActCnf.
 *
 * If the initial search has been carried out in response to RrcActReq
 * and signal RrcActCnf has indicated that the search was not successful,
 * i.e. the required PLMN was not found on any of the frequencies indicated,
 * MM will send the signal to URRC with an appropriate alternative value of
 * requestedPlmn.  The result of the search will be returned to MM in signal
 * RrcActCnf.
 */
typedef struct RrcActReqTag
{
    /** Controls how URRC uses the signal
\assoc PRESENT \ref requestedPlmn */
    Boolean requestedPlmnValid;

    /** Contains the identity of the PLMN to be searched for.
     * If the initial search has been carried out in response to RrcActReq and signal
     * RrcActCnf has indicated that the search was not successful, i.e, the required
     * PLMN was not found on any of the frequencies indicated, MM will send the signal
     * to URRC with an appropriate alternative value of requestedPlmn.
     * The result of the search will be returned to MM in signal RrcActCnf.
    */
    Plmn requestedPlmn;

    /** Indicates that the PLMN has been selected manually by the user which
     * allows URRC to camp on a PLMN cell with a forbidden LAI.
     * For the first attempt at initial cell selection MM will send this
     * signal to URRC with requestedPlmnValid set FALSE.  This will allow
     * the UE to search for cells before information from the USIM is
     * available and before the user has entered a PIN number.
     *
     * Subsequently MM will send this signal to URRC with requestedPlmnValid
     * set TRUE and the PLMN to be searched for in requestedPlmn.
     * A list of frequency information may also be present in fddCellInformation.
     */
    Boolean plmnManuallySelected;

    /** Indicates if L1 needs to SWITCH_RAT or ACTIVATE_RAT to either FDD
     * or TDD128. The RAT is determined by the handsetModes.
     */
    RatActivateMode ratActivateMode;

    /** Used in a multi-mode UMTS/GSM UE to indicate which modes the handset is
     * operating in, i.e. with certain USIMs the UE may only be allowed to operate
     * on FDD/TDD so GSM cell monitoring and selection can be disabled by setting
     * handsetModes to NMODE_UMTS.

\assoc UNION \ref cellInfoAndBands */
    NetworkModes          handsetModes;

    /** Defines cell information and bands for the related network mode. */
    CellInfoAndBandsUnion cellInfoAndBands;
}
RrcActReq;

/** This signal is sent by URRC to MM in response to an RrcActReq signal.
 * This signal is sent to MM as a result of the search triggered by RrcActReq.
 * If MM has sent an RrcActReq signal without a requestedPlmn the value
 * of activateStatus will be either ACT_EMERGENCY_ONLY or ACT_NO_SERVICE.
 */
typedef struct RrcActCnfTag
{
    /** Indicates if URRC has found any available cell to camp on.
     * If MM has sent an RrcActReq signal without a requestedPlmn the
     * value of activateStatus will be either ACT_EMERGENCY_ONLY or
     * ACT_NO_SERVICE. See Table below.
     *
     * If MM has sent the RrcActReq signal with a requestedPlmn then
     * the activateStatus in the returned RrcActCnf signal will be
     * ACT_SUCCESS, ACT_EMERGENCY_ONLY, ACT_FORBIDDEN_LA,
     * ACT_FORBIDDEN_NAT_LA or ACT_NO_SERVICE. See Table below.
     *
     *\subsection activateStatus Table
     * <TABLE>
     * <TR><TD><B> Value </B></TD>    <TD><B> Indication              </B></TD></TR>
     * <TR><TD> RrcActReq without requestedPlmn  </TD>
     *     <TD> If activateStatus has the value ACT_EMERGENCY_ONLY
     * this indicates that URRC has successfully found an available
     * cell and camped on it, regardless of PLMN.
     *
     * If activateStatus has the value ACT_NO_SERVICE this indicates
     * that URRC has failed to find any available cell to camp on. </TD></TR>
     * <TR><TD> RrcActReq with requestedPlmn        </TD>
           <TD> If activateStatus has the value ACT_SUCCESS, this
     * indicates that URRC has successfully found a suitable cell on
     * the requested PLMN (or equivalent) and camped on it.
     *
     * If activateStatus has the value ACT_EMERGENCY_ONLY, this indicates
     * that URRC has successfully found a cell with a PLMN other than that
     * requested, and the UE is able to camp on the cell for emergency
     * service only.
     *
     * If activateStatus has the value ACT_FORBIDDEN_LA or ACT_FORBIDDEN_NAT_LA,
     * this indicates that URRC has camped on a cell that is in the forbidden list.
     * In this case systemInformation lists the carrier frequency, PLMN code and
     * scrambling of the cell that is being camped on.
     *
     * If activateStatus has the value ACT_NO_SERVICE, this indicates that
     * URRC has failed to find any cell with any PLMN (this may just be
     * from a frequency list supplied in the RrcActReq signal) and the UE
     * is unable to obtain even emergency service.</TD></TR>
     * </TABLE>
     */
    ActivateStatus activateStatus;

    /** Contains non-access stratum system information for
     * MM and neighbour cell information to be sent to the
     * application layer. See Table Below.
     *
     * \subsection systemInformation Table
     * <TABLE>
     * <TR><TD><B>  activateStatus </B></TD>
     * <TD><B> systemInformation           </B></TD></TR>
     * <TR><TD> ACT_EMERGENCY_ONLY  </TD>    <TD> lists the carrier
     * frequency, PLMN code and scrambling of the cell that is being
     * camped on.         </TD></TR>
     * <TR><TD> ACT_FORBIDDEN_LA or ACT_FORBIDDEN_NAT_LA </TD>
     * <TD> lists the carrier frequency, PLMN code and scrambling of
     * the cell that is being camped on.   </TD></TR>
     * </TABLE>
     */
    USystemInformation systemInformation;
}
RrcActCnf;

/** This signal is sent by URRC to MM when URRC reselects to a new LAI
 * or RAI or the service status has changed.
 */
typedef struct RrcActIndTag
{
    /** Indicates if URRC has found any available cell to camp on.
     * See this \ref activateStatus
     */
    ActivateStatus     activateStatus;

    /** Contains system information. See this \ref systemInformation.
     */
    USystemInformation systemInformation;
}
RrcActInd;

/** This signal is sent by MM to deactivate the 3G Access Stratum,
 * either because the stack should be shut down, or to switch to
 * the GSM stack.
 */
typedef struct RrcDeactReqTag
{
    /** Indicates the cause of deactivation*/
    AccStratumDeactCause deactivateCause;
}
RrcDeactReq;

/** Select either GSM, FDD or TDD128 Mode.
 *  This signal is sent from MM to RRC to select the active RAT.
 */

typedef struct RrcModeSelectReqTag
{
    /** Indicate whether to activate FDD or TDD128 mode. */
    NetworkModes networkMode;

    /** Indicate the RATs the UE will operate over (includes networkMode). */
    NetworkModes handsetModes;
}
RrcModeSelectReq;

/** Confirmation that mode is selected.
 * This signal is sent from RRC to MM in response to a #RrcModeSelectReq.
 */
typedef EmptySignal RrcModeSelectCnf;

/** Indicated if the UE was paged by IMSI or TMSI. */
typedef enum UCnPagingIdTypeTag
{
    /** Paged by IMSI. */
    CN_PAGED_BY_IMSI,

    /** Paged by TMSI. */
    CN_PAGED_BY_TMSI
}
UCnPagingIdType;

/** This signal is sent to MM when URRC receives a paging indication
 * originating in the Core Network, but not when URRC receives a paging
 * indication originating in the UTRAN.
 */
typedef struct RrcPageIndTag
{
    /** Indicates why the network is paging the UE. */
    UPagingCause       pagingCause;

    /** Indicates whether paging is for the CS or PS domain. */
    UCN_DomainIdentity cn_DomainIdentity;

    /** Indicated if the UE was paged by IMSI or TMSI
     * (where TMSI could be P-TMSI).
     */
    UCnPagingIdType cnPagingIdType;
}
RrcPageInd;

/** Indicated the cs domain call type. */
typedef enum UCsCallTypeTag
{
    /** SPEECH. */
    UCSCALLTYPE_SPEECH,
    /** video. */
    UCSCALLTYPE_VIDEO,
    /** except speech,video */
    UCSCALLTYPE_OTHER,
    /** reserved. */
    UCSCALLTYPE_SPARE
}
UCsCallType;


/** This signal is sent by MM to request URRC to establish a
 * signalling connection between the UE and the indicated domain
 * in the Core Network. If no RRC connection exists, URRC will
 * establish an RRC connection using RRC CONNECTION REQUEST, and
 * will then send the NAS message to the indicated domain using
 * INITIAL DIRECT TRANSFER.
 *
 * If an RRC connection already exists the NAS message is sent
 * immediately using INITIAL DIRECT TRANSFER.
 */

typedef struct RrcEstablishReqTag
{
    /** Indicates the length of the message. */
    Int16 nasMessageLength;

    /** Points to the NAS information to be sent using the new connection.
     * The nasMessage is a GSM or GPRS L2 encoded message.
     */
    Int8                * nasMessage;

    /** Indicates what the connection is being established for. */
    UEstablishmentCause establishmentCause;

    /** Indicates whether a CS or PS domain connection is required.*/
    UCN_DomainIdentity  cn_DomainIdentity;

    /** Indicates the LAI/RAI.*/
    UAreaIdentity       areaIdentity;
#if defined (UPGRADE_GGE)
    /** In a dual mode target build, procReason indicates whether
     * establishment is needed to send a message triggered by an
     * inter-RAT change. In this case RRC will overwrite
     * establishmentCause with either
     * UEstablishmentCause_interRAT_CellReselection,OR
     * UEstablishmentCause_interRAT_CellChangeOrder
     */
    UMmProcReason procReason;
#endif

#if defined(UPGRADE_REL6)
    /** Indicate the cs call type */
    UCsCallType csCallType;
#endif
#if defined(UPGRADE_REL6)  && defined (UPGRADE_3G_FDD)
    /** Indicate combined NAS procedure */
    Boolean isCombinedProcedure;

    /** Indicate if MNC is 3 digit endocoded */
    Boolean threeDigitMncEncoding;
#endif

}
RrcEstablishReq;

/** This signal is sent by URRC to MM to indicate that the connection
 * requested by the RrcEstablishReq signal has been set up or URRC
 * has failed to set it up.
 */
typedef struct RrcEstablishCnfTag
{
    /** Indicates whether the procedure has succeeded and, if not, why not.*/
    UEstablishStatus   establishStatus;

    /** Indicates whether the UE has established to the CS or PS domain.*/
    UCN_DomainIdentity cn_DomainIdentity;
}
RrcEstablishCnf;

/** This signal is sent from MM to URRC to release a connection to the CN */
typedef struct RrcConnRelReqTag
{
    /** Indicates the domain from which the connection should be released.*/
    UCN_DomainIdentity cn_DomainIdentity;

    /** Indicates the reason of release. */
    UConnRelReason     reason;
}
RrcConnRelReq;

/** This signal is sent from URRC to MM to indicate that a connection
 * has been released as requested by the RrcConnRelReq signal.
 */
typedef struct RrcConnRelCnfTag
{
    /** Indicates the domain from which the connection has been released. */
    UCN_DomainIdentity cn_DomainIdentity;

    /** will be set to FALSE if no connection existed for the domain specified.*/
    Boolean            success;

    /** Indicates the number of elements in the startInfo array. This may be zero.
\assoc ARRAY \ref startInfo */
    Int8               numberOfStartInfo;

    /** Is an array start information for all CN domains that were present in
     * the RRC connection. The elements are only present when the RRC connection
     * itself is released (rather than the Layer 3 signalling connection).
     */
    RrcStartInformation startInfo [maxCNdomains];
}
RrcConnRelCnf;

/** This signal is sent from URRC to MM to indicate that one or
 * more CN domain connections have been released by the UTRAN.
 * URRC notifies MM that one or more CN domain connections have
 * been releasedThis can happen for three reasons:<ul>
 * <ol> The RRC has received a SIGNALLING CONNECTION RELEASE
 * message for the relevant CN domain.
 * <ol> The RRC has received an air interface message from the
 * UTRAN which includes the “signalling connection release indication?
 * IE and all RABs for the CN domain indicated have been released.
 * <ol> The RRC has received the RRC CONNECTION RELEASE message.
 *</ul>
 *
 *\note On RRC connection release this signal will only be sent for CN
 * domain connections that exist at the time and will not be sent for
 * any CN domains for which the connection has been released previously.
 * If all CN domain connections have been released prior to the RRC connection
 * release (i.e. as a result of SIGNALLING CONNECTION RELEASE, the “signalling
 * connection release indication?IE or RrcConnRelReq) this signal will not be sent.
 */
typedef struct RrcConnRelIndTag
{
    /** Indicates if the CS domain has been released.*/
    Boolean       releaseCsDomain;

    /** Indicates if the PS domain has been released.*/
    Boolean       releasePsDomain;

    /** Indicates why the connection was released. */
    UReleaseCause cause;
#ifdef UPGRADE_3G_FDD
    /**  Indicates the available FDD downlink frequencies for
     * the last registered PLMN.
     */
    FrequencyInformation fddFrequencyInformation;
#endif
#ifdef UPGRADE_3G_TDD128
    /**  Indicates the available TDD128 downlink frequencies for
         * the last registered PLMN.
         */
    FrequencyInformation tdd128FrequencyInformation;
#endif

    /** refers to the number of elements in the startInfo array
     * and this may be zero.

\assoc ARRAY \ref startInfo */
    Int8 numberOfStartInfo;

    /** Is an array start information for all CN domains that were
     * present in the RRC connection.  The elements are only present
     * when the RRC connection itself is released (rather than the Layer
     * 3 signalling connection).
     */
    RrcStartInformation     startInfo [maxCNdomains];
#if defined (UPGRADE_GGE)
    /** Indicates the available GSM frequencies for the last
     * registered PLMN.
     */
    RrcGsmBaRangeElement    gsmBaRangeList;
#endif /* UPGRADE_GGE */
}
RrcConnRelInd;

/** This signal is used by MM to request URRC to send a NAS message
 * to its peer entity. MM requests URRC to send a NAS message
 * using the CN domain indicated by cn_DomainIdentity.
 */
typedef struct RrcDataReqTag
{
    /** Identifies CN domain. */
    UCN_DomainIdentity cn_DomainIdentity;

    /** Indicates the length of the NAS message. */
    Int16              nasMessageLength;

    /** Points to the NAS message to be sent.
     * Is a GSM or GPRS L2 encoded message.
     */
    Int8 * nasMessage;

    /** Indicates to URRC whether to use RB3 or RB4 to send
     * the message.
     */
    UNAS_MessagePriority messagePriority;
}
RrcDataReq;

/** This signal is used by URRC to send NAS message information to MM.
 * URRC passes the NAS message contents to MM.
 */
typedef struct RrcDataIndTag
{
    /** Indicates if integrity protection was checked for the message
     * and if so if it was successful. */
    UIntegrityProtectionInfo integrityProtectionInfo;

    /** Indicates the domain from which the message was received. */
    UCN_DomainIdentity       cn_DomainIdentity;

    /** Indicates the length of nasMessage. */
    Int16                    nasMessageLength;

    /** Points to the NAS message information to be sent to MM.
    * Is a GSM or GPRS L2 encoded message.
    */
    Int8 * nasMessage;
}
RrcDataInd;

/** This signal is sent from URRC to MM if any of the NAS system
 * information changes.
 * This primitive is sent to MM, by URRC, if the received data is
 * different from the exisiting data.
 *
 * This primitive requires the UE to be camped on a cell before a
 * RrcSysInfoInd can be sent.
 */
typedef struct RrcSysInfoIndTag
{
    /**  Contains NAS system information and neighbour cell information.*/
    USystemInformation systemInformation;
}
RrcSysInfoInd;

#if defined (UPGRADE_REL6) && defined (UPGRADE_3G_FDD)
/** This signal is sent from URRC to MM if any of the DSAC
 * information changes.
 * This primitive is sent to MM, by URRC, if the received data is
 * different from the exisiting data.
 *
 * This primitive requires the RRC to be in connected mode
 */
typedef struct RrcDsacInfoIndTag
{
    /** Indicates which domain specific access classes are barred. Bit 0 corresponds
     * to AC0 and a 0 indicates barred and a 1 indicated not barred.
     */
    /** indicate if the DSAC present in the cell
      \assoc PRESENT \ref barredDomainSpecificAccessClasses */
    Boolean                   barredDomainSpecificAccessClassesPresent;
    Int16                     barredDomainSpecificAccessClasses[maxCNdomains];
}
RrcDsacInfoInd;
#endif

/** This signal is sent to URRC at initialization and inter-RAT change,
 * or if the GSM classmarks change.
 */
typedef struct RrcMeDataReqTag
{
    /** Contains the UMTS UE Capability information. */
    UmtsMobileEquipmentData umtsMobileEquipmentData;

    /** Contains the encoded GSM classmarks. the purpose of
     * gsmClassmark2 is to provide the network with information
     * concerning aspects of both high and low priority of the
     * mobile station equipment.  */
    UGSM_Classmark2 gsmClassmark2;

    /** Provides information concerning aspects of the mobile station. */
    UGSM_Classmark3 gsmClassmark3;

    /** Contains mobile identity information. */
    MobileIdElement imei;
#if defined (UPGRADE_GGE)
    /** Defines the maximum RF power in dBm for each GSM band
     * (GSM900, DCS, PCS, GSM850, etc).
     */
    UMaxGsmTxPower     gsmMaxTxPower[NUM_BANDS];

    /** Raw GSM Mobile Equipment data also included */
    MsClassmarkElement classmark;

    /** Defines capability of MS  */
    MsCapability       msCapability;

    /** Defines GPRC capability */
    GprsCapability     gprsCapability;

    /** Provides the network with information concerning aspects of
     * the mobile station related to GPRS. The contents might affect
     * the manner in which the network handles the operation of the
     * mobile station.
     */
    MsNetworkCapability msNetworkCapability;
# if defined(UPGRADE_EDGE)
    /** Defines EGPRS capability */
    EgprsCapability     egprsCapability;
# endif
#endif
# if defined (UPGRADE_LTE)
    /** Contains the mobile equipment data for EUTRA mode. */
    EutraMobileEquipmentData            eutraMobileEquipmentData;
# endif

}
RrcMeDataReq;

/* MsOperationMode which is defined in alsi_sig.h has been redefined */
/* here since use of alsi_sig.h produces insoluble references to     */
/* RrrRssiIndicationTag     */
/** Defines the operation mode of the MS. */
typedef enum UMsOperationModeTag
{
    UMS_OPERN_NORMAL               = 0x00, /**< Normal operation */
    UMS_OPERN_TA                   = 0x80, /**< Type approval operation */
    UMS_OPERN_NORMAL_PLUS_SPECIFIC = 0x01, /**< Normal + specific operation */
    UMS_OPERN_TA_PLUS_SPECIFIC     = 0x81, /**< Type approval + specific oper'n */
    UMS_OPERN_MAINTENANCE          = 0x02, /**< Maintenance operation */
    UMS_OPERN_CELL_TEST            = 0x04  /**< Cell test operation */
}
UMsOperationMode;

/** This signal is sent to URRC at initialization and inter-RAT change.
 * It contains data from the USIM and is only updated when the USIM is changed.
 */
typedef struct RrcMsDataReqTag
{
    /** Set to TRUE if SIM is present */
    Boolean          msDataValid;

    /** Defines the IMSI from the USIM. */
    MobileIdElement  imsi;

    /** Defines the access class from the USIM. */
    Int16            accessClass;

    /** Defines the operation mode of the MS. */
    UMsOperationMode msOperationMode;

    /** Defines HOME PLMN. */
    Plmn             hplmn;

#if defined(ENABLE_EHPLMN)
    /** Indicates the number of EHPLMNs in the list Ehlmns.
    \assoc ARRAY \ref ehplmns */
    Int8             numberofEhplmns;

    /** Contains the list of PLMNs that are considered to be HPLMN */
    Plmn             ehplmns [MAX_EHPLMNS];
#endif /* ENABLE_EHPLMN */
}
RrcMsDataReq;

/** This signal is sent to URRC to inform it that new security keys
 * are available for a core network domain.
 * RrcSyncInd is used to indicate when the keys have been enabled.
 */
typedef struct RrcSecurityKeySetConfigReqTag
{
    /** Indicates the CN domain identity the ciphering information
     * applies to.
     */
    UCN_DomainIdentity cn_DomainIdentity;

    /** Defines the CK value to use for ciphering.*/
    Int8               newCipherKey [RRC_CIPHER_KEY_ARRAY_LENGTH];

    /** Defines the IK value to use for integrity protection. */
    Int8               newIntegrityKey [RRC_INTEGRITY_KEY_ARRAY_LENGTH];

    /** Defines the 20 bit START value to use with the new key set. */
    USTART_Value       newStartValue;

    /** Defines the KSI that identifies the IK and CK key set. */
    RrcKeySequence     newKeySetIdentifier;
}
RrcSecurityKeySetConfigReq;

/** This signal is sent to URRC whenever MM needs to report a change in the
 * IDs used to page the MS for circuit switched and packet switched operation,
 * or the pageability of the MS.
 * It will also be sent at start-up and after the RrcEstablishReq.
 */
typedef struct RrcUpdateReqTag
{
    /** Contains a LAI or RAI.*/
    UAreaIdentity cellAreaIdentity;

    /** Contains a LAI or RAI or both. This is the area identity in which
     * any temporary subscriber identities (TMSI and P-TMSI) were assigned.
     */
    UAreaIdentity   simAreaIdentity;

    /** Contains the ID of the UE for CS paging (TMSI, IMSI or IMEI). */
    MobileIdElement csPagingId;

    /** Contains the ID of the UE for PS paging (P-TMSI, IMSI or IMEI).*/
    MobileIdElement psPagingId;

    /**  The csMode and psMode are relevant to
     * this area identity. If csMode is set to AM_FORBIDDEN_LA
     * or AM_FORBIDDEN_NAT_LA then URRC adds the cellAreaIdentity to
     * the relevant list of forbidden areas. A csMode of AM_NORMAL
     * removes the forbidden area.
     */
    ActivateMode csMode;

    /**  The csMode and psMode are relevant to
     * this area identity. If psMode is set to AM_FORBIDDEN_LA
     * or AM_FORBIDDEN_NAT_LA then URRC adds the cellAreaIdentity to
     * the relevant list of forbidden areas. A psMode of AM_NORMAL
     * removes the forbidden area.
     */
    ActivateMode psMode;

    /** This parameter proved to cause functional problems and
     * it is ignored by CSR.
     */

    /**  should be deleted from signal rrcUpdateReq.
     * \todo ePlmnListChanged should be deleted from signal rrcUpdateReq.
     */
    Boolean ePlmnListChanged;

    /** Indicates the number of PLMNs in the list equivalentPlmns.
\assoc ARRAY \ref equivalentPlmns */
    Int8    numberofEquivalentPlmns;

    /** Contains the list of PLMNs that are considered to be equivalent
    * to the Selected PLMN given by cellAreaIdentity.
    * This list will always contain the PLMN on which the list was received.
    */
    Plmn equivalentPlmns [MAX_EQUIVALENT_PLMNS];

    /** Indicates the value of the DRX cycle length coefficient to be
     * used for the PS domain. The value 0 indicates that the value to
     * be used is that broadcast in SI. A value between 6 and 9 indicates
     * a specific DRX cycle length coefficient.
     */
    UPsDrxParameter psDrxParameter;

    /** Indicates if the SIM is present. If the SIM is removed the lists
     * of forbidden areas must be cleared.
     */
    Boolean simPresent;
#if defined (UPGRADE_GGE)
    /** Indicates whether CSR may or may not carry out a "fall-back on GSM"
     * 3G->2G reselection shortly after a successful 2G->3G reselection.
     */
    Boolean gsmRatArfcnValid;

    /** Indicates if there is a MM or GMM procedure running. */
    Boolean irrProcRunning;
#endif
}
RrcUpdateReq;

/** This signal can be sent be URRC to MM to indicate a change of cell information.
 * This primitive is sent to MM based on changes in MM-related
 * (i.e., NAS) sys info. This normally takes place <UL>
 * <li> when going from UE CELL_FACH to UE IDLE
 * <li> Upon (re)selection.
 * </UL>
 * \note If the cell crosses a LA or RA boundary then the RrcActInd
 * signal is sent instead.
 */
typedef struct RrcCellUpdateIndTag
{
    /** Contains NAS system information and neighbour cell information. */
    USystemInformation systemInformation;
}
RrcCellUpdateInd;

/** This signal is sent by MM to URRC when a power booster is connected
 * to/disconnected from the MS.
 */
typedef struct RrcPowerClassReqTag
{
    /** Is in the range 1 to 4, but only power classes 3 and 4 are
     * valid for FDD and power class 2 and 3 are applicable for TDD128.
     *
     * Table below shows UE power classes for FDD
     * <TABLE>
     * <TR><TD><B>Power Class Type</B></TD> <TD><B>Nominal maximum output power</B></TD>
     * <TD><B>Tolerance</B></TD></TR>
     * <TR><TD>3  </TD>    <TD>+24 dBm   </TD>    <TD>+1 dB / -3 dB  </TD> </TR>
     * <TR><TD>4  </TD>    <TD>+21 dBm   </TD>    <TD>+2 dB / -2 dB  </TD> </TR>
     * </TABLE>
     *
     *
     * Table below shows UE power classes for TDD128
     * <TABLE>
     * <TR><TD><B>Power Class Type</B></TD> <TD><B>Nominal maximum output power</B></TD>
     * <TD><B>Tolerance</B></TD></TR>
     * <TR><TD>2  </TD>    <TD>+24 dBm   </TD>    <TD>+1 dB / -3 dB  </TD> </TR>
     * <TR><TD>3  </TD>    <TD>+21 dBm   </TD>    <TD>+2 dB / -2 dB  </TD> </TR>
     * </TABLE>
     */
    UUE_PowerClass powerClass;
}
RrcPowerClassReq;

/** This signal is used to indicate either a problem occurred on NAS
 * message sending or to report the result of plmn search.
 */
typedef struct RrcStatusIndTag
{
    /** Indicates the status of RRC. */
    URrcStatus rrcStatus;
}
RrcStatusInd;

/** This signal is sent by URRC to MM to inform MM of all the
 * PLMNs which the UE can see.
 * This primitive is sent to MM as a response
 * to a RrcPlmnListReq from MM requesting CSR to do a
 * PLMN search.
 */
typedef struct RrcPlmnListCnfTag
{
    /** Indicates the outcome of the PLMN search procedure
     * and is one of PLMN_LIST_SUCCESS, PLMN_LIST_FAILED or
     * PLMN_LIST_ABORTED.
     */
    PlmnListStatus status;

    /** lists all the PLMNs found. */
    PlmnList       list;

    /** Contains the ARFCN's that each PLMN was found. */
    UarfcnList     uArfcnList;
}
RrcPlmnListCnf;

/** Every time a PLMN is found the complete list of PLMNs found so far
 * is sent in this primitive.
 * This primitive is sent to MM as a response
 * to a RrcPlmnListReq from MM requesting CSR to do a
 * PLMN search.
 */
typedef struct RrcPlmnListIndTag
{
    /** lists all the PLMNs found. */
    PlmnList list;
}
RrcPlmnListInd;

/** This signal is sent by MM to URRC to trigger a search for the
 * available PLMNs that the UE can see. In a dual mode UE the search
 * can be for PLMNs on UMTS or GSM cells.
 */
typedef struct RrcPlmnListReqTag
{
    /** if FALSE then a list of all available PLMNs should be returned.
\assoc PRESENT \ref requestedPlmn */
    Boolean requestedPlmnPresent;

    /** Specifies a PLMN to search for */
    Plmn    requestedPlmn;

    /** Contains a reduced set of frequencies on which to search
     * for PLMNs and it is used to speed up PLMN search
     */
    UarfcnList     uArfcnList;
#ifdef UPGRADE_3G_FDD
    /** Indicates which FDD band to use */
    FddBandMode    fddBandMode;
#endif
#ifdef UPGRADE_3G_TDD128
    /** Indicates which TDD128 band to use */
    Tdd128BandMode tdd128BandMode;
#endif

    /** See Table below how to interpret this parameter.
     *
     * <TABLE>
     * <TR><TD><B>Value</B></TD>  <TD><B> Operation </B></TD></TR>
     * <TR><TD> TRUE      </TD>    <TD> the user requested the PLMN
     * search and the search should be performed as quickly as possible.</TD></TR>
     * <TR><TD> FALSE                </TD>    <TD> This a background search
     * requested by MM to try and gain better service (e.g. regain Home PLMN
     * coverage if roaming) and this should not affect operation of the UE on
     * the serving cell.      </TD></TR>
     * </TABLE>
     */
    Boolean manuallyTriggered;

    /** If set to TRUE then the whole UMTS band must be searched */
    Boolean exhaustiveSearch;

    /** If set to TRUE then only list of frequencies in
     * UArfcnList will be searched
     */
    Boolean uArfcnListOnlySearch;

    /** Indicates if L1 needs to SWITCH_RAT or ACTIVATE_RAT to either FDD
     * or TDD128. The RAT is determined by the networkModeToSearch.
     */
    RatActivateMode ratActivateMode;

    /** Specifies if the PLMN search is for GSM or FDD or TDD PLMNs */
    NetworkModes    networkModeToSearch;

    /** Specifies the RATs supported */
    NetworkModes    handsetModes;
# if defined (UPGRADE_GGE)
    /** Specify which GSM bands to search first */
    BandMode        gsmBandMode;
#endif
}
RrcPlmnListReq;

/** @} */ /* End of SigRrcSignals group */

/** Defines Test mode that is requested */
typedef enum URrcTestModeReqTestModeTag
{
    /** Debug signals for System Information Block */
    URRC_TEST_MODE_SYS_INFO_DEBUG_MODE,

    /** Integrity protection. */
    URRC_TEST_MODE_INTEGRITY_PROTECTION_MODE,

    /** Change in the RRC state. */
    URRC_TEST_MODE_STATE_CHANGE_DEBUG_MODE,

    /** MSC debug info. */
    URRC_TEST_MODE_MSC_DEBUG_INFO_MODE,

    /** Debug signals for Air interface signals  */
    URRC_TEST_MODE_AIR_INTERFACE_MODE,

    /** Override value for testing */
    URRC_TEST_MODE_OVERRIDE_T305_VALUE,

    /** Use debug signals for input. */
    URRC_TEST_MODE_LOG_2_TEST_MODE,

    /** Set test mode ue capability. */
    URRC_TEST_MODE_SET_UE_CAPABILITY,

    /** perform normal IP check as 25.331 if SMC not started */
    URRC_TEST_MODE_NORMAL_IP_CHECK,

#if defined(L3_TEST_HARNESS_FDD_HSUPA)
    /** Set Fake Edch Config to send upon next RBSetup. */
    URRC_TEST_MODE_SET_FAKE_EDCH_CONFIG
#endif
}
URrcTestModeReqTestMode;

/** Bit map to the SERVING_CELL_MEASUREMENT_IND signal. */
#define URRC_MSC_DEBUG_SERVING_CELL_MEASUREMENT_IND     ((Int32)0x01)

/** Bit map to the UE_INTERNAL_MEASUREMENT_IND signal. */
#define URRC_MSC_DEBUG_UE_INTERNAL_MEASUREMENT_IND      ((Int32)0x02)

/** Bit map to the MONITOR_INTER_FREQ_CELL_IND signal. */
#define URRC_MSC_DEBUG_MONITOR_INTER_FREQ_CELL_IND      ((Int32)0x04)

/** Bit map to the MONITOR_INTRA_FREQ_CELL_IND signal. */
#define URRC_MSC_DEBUG_MONITOR_INTRA_FREQ_CELL_IND      ((Int32)0x08)

/** This is just for the RRC MSC tool debug information.
 * Defines what type of debugging is enabled. */
typedef struct URrcMscDebugTag
{
    /** Indicates if debugging internal signal is enabled. */
    Boolean internalSignalsEnabled;

    /** Indicates if debugging external signal is enabled. */
    Boolean externalSignalsEnabled;

    /** Indicates if debugging timer expired is enabled. */
    Boolean timerExpiryEnabled;

    /** Indicates if debugging start timer is enabled. */
    Boolean startTimersEnabled;

    /** Indicates if debugging stop timer is enabled. */
    Boolean stopTimersEnabled;

    /** Indicates if debugging function call is enabled. */
    Boolean functionCallsEnabled;

    /** Defines measurement signals. */
    Int32   measurementsBitMask;
}
URrcMscDebug;

/** Defines what type of ue capability is set for test. */
typedef struct URrcTestModeUeCapabilityTag
{
    /** Indicates if hsdpa is enabled. */
    Boolean hsdpaEnabledPresent;
    Boolean hsdpaEnabled;

    /** Indicates if ciphering is enabled. */
    Boolean cipheringEnabledPresent;
    Boolean cipheringEnabled;
}
URrcTestModeUeCapability;
#if defined(L3_TEST_HARNESS_FDD_HSUPA)
/** Defines the Edch Parameters to use for fake EDCH config. */
typedef struct MacDFlowListParamsTag
{
    Int8                            fakeRbId;
    Int8                            fakeLogChId;
    E_DCHAddReconfMACdFlowListFDD   macDFlowParams;
}
MacDFlowListParams;

typedef struct L2StaticParamsTag
{
    Int8                numberOfEDchMacDFlows;
    MacDFlowListParams  macDFlowList [maxE_DCHMACdFlow];
}
L2StaticParams;

typedef struct FakeFddHsupaSemiStaticCfgTag
{
    CphyEdchConfigReq   edchConfigReq;
    L2StaticParams      l2Params;
    Boolean             releaseEdch;
    Boolean             resetMac_es_e;
}
FakeFddHsupaSemiStaticCfg;
#endif
/** \defgroup SigRrcDebug RRC Debug Primitives
 * Primitives for debugging purpose
 * This section defines primitives used for debugging purpose
 * @{
 *
 */

/** This primitive defines RRC test mode and sets its flags/parameters. */
typedef struct RrcTestModeReqTag
{
    /** Defines RRC test mode.
\assoc UNION \ref parameters */
    URrcTestModeReqTestMode mode;

    /** Defines parameters */
    union
    {
        /** Indicates if debugging signals for System Information
         * Block is enabled.
         */
        Boolean sysInfoDebugEnabled;

        /** Indicates if debugging signals for integrity protection
         * is enabled.
         */
        Boolean integrityProtectionDisabled;

        /** Indicates if debugging change of RRC state
         * is enabled.
         */
        Boolean stateChangeDebugEnabled;

        /** Indicates if MSC debug is enabled. */
        URrcMscDebug mscDebugInfo;

        /** Indicates if air interace signal debug is enabled. */
        Boolean airInterfaceDebugEnabled;

        /** Defines value of T305. */
        Int8 testT305value;

        /** Use debug signals for input. */
        Int8 dummy;

        /** Indicates which ue capabilities will be set for test. */
        URrcTestModeUeCapability    testUeCapability;

        /** perform normal IP check as 25.331 if SMC not started */
        Boolean normalIPCheckEnabled;

#if defined(L3_TEST_HARNESS_FDD_HSUPA)
        /** Set Fake Edch Config to send upon next RBSetup. */
        FakeFddHsupaSemiStaticCfg   fakeFddHsupaCfg;
#endif
    }
    parameters;
}
RrcTestModeReq;

/** @} */ /* End of SigRrcDebug group */

/** \defgroup SigRrcSignals  RRC Primitives
 * Primitives exchanged between URRC and MM
 * This section defines primitives exchanged between URRC and MM
 *
 * @{
 *
 */

/** This signal is sent by URRC to MM to inform MM of security
 * changes and resource assignments.
 */
typedef struct RrcSyncIndTag
{
    /** Indicates cause of security changes */
    SyncCause          cause;

    /** Identifies CN domain. */
    UCN_DomainIdentity cn_DomainIdentity;
}
RrcSyncInd;

/** This primitive is sent to MM to notify it of pending NAS messages.
 * This signal is used to tell MM of any NAS message that RRC has not been
 * able to send before a change in RAT. This signal is just for the RRC MSC
 * tool debug information
 */
typedef struct RrcNasTxQueueInfoIndTag
{
    /** Defines number in transmission queue. */
    Int16 numInTxQueue;

    /** Defines NAS message header. The format of nasMsgHeader is as follows:
    * xx pp aa bb where: <ul>
    * <li>xx == don't care
    * <li>pp == PD/1st octet of nasMessage
    * <li>aa == MsgType/2nd octet of nasMessage
    * <li>bb == 3rd octet of nasMessage (in case of extended TI)
    * </ul>
    */
    Int32 nasMsgHeader[MAX_NAS_TX_QUEUE_INFO_SIZE];
}
RrcNasTxQueueInfoInd;

/** @} */ /* End of SigRrc group */

/** Defines Max SIB data size in octet. */
#define MAX_SYS_INFO_OCTETS     1000

/** \defgroup SigRrcDebug RRC Debug Primitives
 * Primitives for debugging purpose
 * This section defines primitives used for debugging purpose
 * @{
 *
 */

/** This signal contains System Information Block wich does not have
 * PER error. This signal is sent to the RRC to be
 * captured by the passthrough task which decodes system
 * information ASN.1 for debugging purposes.
 */
typedef struct RrcDebugSystemInformationIndTag
{
    /** Defines type of SIB. */
    USIB_Type         sib_Type;

    /** Defines the PDU Asn.1 type
\assoc ASN \ref data \opt "%TPLGSM%\tools\Projects\3GPS\asntools\asndll\rrc7c0_td.dll"  */
    UPduIdentifier    pduIdentifier;

    /** Defines length of data in byte.
\assoc ARRAY \ref data */
    Int16             byteLength;

    /** Contains SIB data. */
    Int8              data [MAX_SYS_INFO_OCTETS];
}
RrcDebugSystemInformationInd;

/** This signal contains System Information Block wich has PER error.
 * This signal is sent to the RRC to be
 * captured by the passthrough task which decodes system
 * information ASN.1 for debugging purposes.
 */
typedef struct RrcDebugSysInfoWithErrorIndTag
{
    /** Defines type of SIB. */
    USIB_Type         sib_Type;

    /** Indicates type of PER error. */
    PerError          per_error;

    /** Defines the PDU Asn.1 type
\assoc ASN \ref data \opt "%TPLGSM%\tools\Projects\3GPS\asntools\asndll\rrc7c0_td.dll"  */
    UPduIdentifier    pduIdentifier;

    /** Defines length of data in byte.
\assoc ARRAY \ref data */
    Int16             byteLength;

    /** Contains SIB data. */
    Int8              data [MAX_SYS_INFO_OCTETS];
}
RrcDebugSysInfoWithErrorInd;

/** Defines Max size of air interface to debug in octet (\note Not used) */
#define MAX_AIR_INTERFACE_DEBUG_OCTETS     384

/** Containing the data received from RLC if status is OK.
 * This signal is generated because the RLC signal
 * may use TMM memory which is copied by this function into
 * the signal so that it is visible to Genie.
 */
typedef struct RrcDebugAirInterfaceRxIndTag
{
    /** Defines RLC mode (AM/UM/TM).*/
    T_UDL_RLC_Mode_r7       rlcMode;

    /** Determines frequency info and it ranges from  0 to 16383. */
    UUARFCN                 uarfcn_DL;

    /** UmtsMode: FDD or TDD128.
\assoc UNION \ref cellIdentification */
    UmtsMode                umtsMode;

    /** Defines FDD or TDD128 Cell information. */
    CellIdentificationUnion cellIdentification;

    /** Indicates downlink frame number. */
    UFrameNumber            dlFrameNumber;

    /** Indicates bearer identity. */
    BearerIdentity          bearerIdentity;

    /** Indicates transport channel type. Only present if the rlcMode is TM
     * and is used to determine if BCCH data is received on BCH or FACH.

\assoc PRESENT \ref trChType */
    Boolean               trChTypePresent;
    UTransportChannelType trChType;

    /** Defines the PDU Asn.1 type
\assoc ASN \ref data \opt "%TPLGSM%\tools\Projects\3GPS\asntools\asndll\rrc7c0_td.dll"  */
    UPduIdentifier        pduIdentifier;

    /** Defines length of data in byte.
\assoc ARRAY \ref data */
    Int16                 byteLength;

    /** Contains data received in air interface. */
    Int8                  data [MAX_AIR_INTERFACE_DEBUG_OCTETS];
}
RrcDebugAirInterfaceRxInd;

/** Containing the data received from RLC if status is OK.
 * This signal is generated because the RLC signal
 * may use TMM memory which is copied by this function into
 * the signal so that it is visible to Genie.
 */
typedef struct RrcDebugAirInterfaceStatusIndTag
{
    /** Defines RLC mode (AM/UM/TM).*/
    T_UDL_RLC_Mode_r7       rlcMode;

    /** Determines frequency info and it ranges from  0 to 16383. */
    UUARFCN                 uarfcn_DL;

    /** UmtsMode: FDD or TDD128.
\assoc UNION \ref cellIdentification */
    UmtsMode                umtsMode;

    /** Defines FDD or TDD128 Cell information. */
    CellIdentificationUnion cellIdentification;

    /** Indicates downlink frame number. */
    UFrameNumber            dlFrameNumber;

    /** Indicates bearer identity on which signal received. */
    BearerIdentity          bearerIdentity;

    /** Indicates transport channel type. */
    UTransportChannelType   trChType;

    /** This is used to indicate the status of a RLC SDU. */
    UrlcSduStatus           rlcSduStatus;
}
RrcDebugAirInterfaceStatusInd;

/** Generates a SIG_RRC_DEBUG_AIR_INTERFACE_TX_IND signal containing
 *  the data trasnmitted to RLC. This signal is generated because the
 * RLC signal may use TMM memory which is copied by this function into
 * the signal so that it is visible to Genie.
 */
typedef struct RrcDebugAirInterfaceTxIndTag
{
    /** Defines RLC mode (AM/UM/TM).*/
    T_UUL_RLC_Mode rlcMode;

    /** Indicates bearer identity on which data is transmitted. */
    BearerIdentity bearerIdentity;

    /** Defines the PDU Asn.1 type
\assoc ASN \ref data \opt "%TPLGSM%\tools\Projects\3GPS\asntools\asndll\rrc7c0_td.dll"  */
    UPduIdentifier pduIdentifier;

    /** Defines length of data in byte.
\assoc ARRAY \ref data */
    Int16          byteLength;

    /** Contains the data trasnmitted to RLC */
    Int8           data [MAX_AIR_INTERFACE_DEBUG_OCTETS];
}
RrcDebugAirInterfaceTxInd;

#if defined(UPGRADE_3G_FDD)
typedef struct RrcDebugActiveSetIndTag
{
    Int8                numberOfActivecells;

    UPrimaryScramblingCode activeCells [UPS_MAX_RADIO_LINKS];
}
RrcDebugActiveSetInd;
#endif

#ifdef UPGRADE_3G_FDD
#ifdef ENABLE_ENHANCED_RL_MAINTENANCE
typedef struct RrcDebugEnhancedRlMaintenanceIndTag
{
    /*************************************************
     *          Type of Intra-Freq Criteria          *
     *************************************************/
    Int16                        measIdIaCheating;
    UMeasurementIdentity         e1a1b1cEcN0MeasId;
    UMeasurementIdentity         e1e1fEcN0MeasId;
    UMeasurementIdentity         e1e1fRSCPMeasId;

    /* event-1A Parameters */
    UHysteresis                  orgE1aHysteresis;
    UTimeToTrigger               orgE1aTimeToTrigger;
    UReportingRange              orgE1aReportingRange;
    UReportDeactivationThreshold orgE1aDeactThreshold;

    UHysteresis                  modE1aHysteresis;
    UTimeToTrigger               modE1aTimeToTrigger;
    UReportingRange              modE1aReportingRange;
    UReportDeactivationThreshold modE1aDeactThreshold;

    /* event-1B Parameters */
    UHysteresis                  orgE1bHysteresis;
    UTimeToTrigger               orgE1bTimeToTrigger;
    UReportingRange              orgE1bReportingRange;

    UHysteresis                  modE1bHysteresis;
    UTimeToTrigger               modE1bTimeToTrigger;
    UReportingRange              modE1bReportingRange;

    /* event-1C Parameters */
    UHysteresis                  orgE1cHysteresis;
    UTimeToTrigger               orgE1cTimeToTrigger;

    UHysteresis                  modE1cHysteresis;
    UTimeToTrigger               modE1cTimeToTrigger;

    /*************************************************
     *          Type of Inter-Freq Criteria          *
     *************************************************/
    Int16                        measIdIeCheating;
    UMeasurementIdentity         e2d2fEcN0MeasId;
    UMeasurementIdentity         e2d2fRSCPMeasId;

#ifdef ENABLE_UL_TX_CONTROL
    /*************************************************
     *        UL Tx Control: Stop or Non-Stop        *
     *************************************************/
    Int8                         bestACStatus;
    Int8                         bestACStatusCount;
    Boolean                      ulTxNonStop;
#endif /* ENABLE_UL_TX_CONTROL */
}
RrcDebugEnhancedRlMaintenanceInd;
#endif /* ENABLE_ENHANCED_RL_MAINTENANCE */
#endif /* UPGRADE_3G_FDD */

typedef enum RrcGsmAirInterfaceAsnTypeTag
{
    /**  Handover to UTRAN . */
    GSM_AI_TYPE_HO_TO_UTRAN_COMMAND,

    /** invalid type. */
    GSM_AI_TYPE_INVALID
}
RrcGsmAirInterfaceAsnType;

/** This signal is sent when requesting handover to UMTS. This signal
 * is sent for an encoded HANDOVER TO UTRAN COMMAND which has been recieved
 * on another RAT air interface
 */
typedef struct RrcDebugGsmAirInterfaceRxIndTag
{
    /** Defines type of ASN */
    RrcGsmAirInterfaceAsnType type;

    /** Defines the PDU Asn.1 type
\assoc ASN \ref data \opt "%TPLGSM%\tools\Projects\3GPS\asntools\asndll\rrc7c0_td.dll"  */
    UPduIdentifier            pduIdentifier;

    /** Defines length of data in byte.
\assoc ARRAY \ref data */
    Int16                     byteLength;

    /** Carries data to decode. */
    Int8                      data [MAX_AIR_INTERFACE_DEBUG_OCTETS];
}
RrcDebugGsmAirInterfaceRxInd;

/** This signal is sent to RRc if RRC state has changed.
 * when RRC state is changed this signal is sent to RRC for
 * debugging purpose.
 */
typedef struct RrcDebugStateChangeIndTag
{
    /** Indicates RRC mode. */
    UrrMode             mode;

    /** Used only if mode is connected */
    URRC_StateIndicator connectedState;
}
RrcDebugStateChangeInd;

/** Defines Max size of data to be sent in RrcDebugInformationInd*/
#define MAX_URRC_DEBUG_INFORMATION_INFO_OCTETS     1000

/* currently first byte of 'data' defines type of debug information */
/** type 1 is for MSC generation */
#define RRC_DEBUG_INFORMATION_IND_TYPE_MSC         1

typedef struct RrcDebugInformationIndTag
{
    /** Defines length of data. */
    Int16 byteLength;

    /** Carries data. */
    Int8  data [MAX_URRC_DEBUG_INFORMATION_INFO_OCTETS];
}
RrcDebugInformationInd;

/** @} */ /* End of SigRrcDebug group */

/** Determines what type of information SIG_RRC_DEBUG_MSC_IND
 * contains.
 */
typedef enum RrcDebugMscTypeTag
{
#if defined(UPGRADE_NOT_FOR_RELEASE)
    /* Make sure the order is the same as the UNION below, duplicate
       fields in the union as required or the genie assosiations won't
       work */
#endif

    /** Internal signal */
    URR_DBG_MSC_INTERNAL_SIGNAL,

    /** Enqueued internal signal. */
    URR_DBG_MSC_ENQUEUED_INTERNAL_SIGNAL,

    /** Dequeued internal signal. */
    URR_DBG_MSC_DEQUEUED_INTERNAL_SIGNAL,

    /** External transmitted Signal */
    URR_DBG_MSC_TX_EXT_SIGNAL,

    /** External received signal. */
    URR_DBG_MSC_RX_EXT_SIGNAL,

    /** Enqueued external signal. */
    URR_DBG_MSC_ENQUEUED_EXT_SIGNAL,

    /** Dequeued external signal. */
    URR_DBG_MSC_DEQUEUED_EXT_SIGNAL,

    /** Timer expired */
    URR_DBG_MSC_TIMER_EXPIRY,

    /** Timer started */
    URR_DBG_MSC_TIMER_START,

    /** Timer stoped */
    URR_DBG_MSC_TIMER_STOP,

    /** Inter RRC subprocess function call. */
    URR_DBG_MSC_FUNCTION_CALL
}
RrcDebugMscType;

/** Contains information of external recived signal. */
typedef struct RrcDebugMscRxExternalSignalTag
{
    /** Indicates signal ID. */
    SignalId id;

    /** Indicates destination (UrrSubProcessId). */
    Int32    dst;
}
RrcDebugMscRxExternalSignal;

/** Contains information of external transmitted signal.
 * This signal is just for the RRC MSC tool debug information.
 * It holds information of external transmitted signal.
 */
typedef struct RrcDebugMscTxExternalSignalTag
{
    /** Indicates signal ID. */
    SignalId id;

    /** Indicates source (UrrSubProcessId). */
    Int32    src;       /* UrrSubProcessId */
    /** Indicates destination. */
    TaskId   dst;
}
RrcDebugMscTxExternalSignal;

/** Contains internal signal details. */
typedef struct RrcDebugMscInternalSignalTag
{
    /** Indicates internal signal ID (UrrInternalSignalId)  */
    Int32 id;

    /** UrrSubProcessId */
    Int32 src;

    /** UrrSubProcessId */
    Int32 dst;
}
RrcDebugMscInternalSignal;

/** Contains timer details. */
typedef struct RrcDebugMscTimerTag
{
    Int32            task;           /**< UrrSubProcessId */
    /** Task specific data.*/
    KiTimerUserValue value;

    /** Timer reference number.*/
    KiTimerId        id;
}
RrcDebugMscTimer;

/** Defines maximum length of Inter RRC subprocess function. */
#define URR_DBG_MSC_FUNCTION_NAME_LENGTH     80

/** Contains information of Inter RRC subprocess function call. */
typedef struct RrcDebugMscFunctionTag
{
    Int32 src;                                          /**< UrrSubProcessId */
    Int32 dst;                                          /**< UrrSubProcessId */

    /** Defines FunctionName. \note Zero terminated string
     * so there is no need for length.
     */
    char name [URR_DBG_MSC_FUNCTION_NAME_LENGTH];
}
RrcDebugMscFunction;

/** \defgroup SigRrcDebug RRC Debug Primitives
 * Primitives for debugging purpose
 * This section defines primitives used for debugging purpose
 * @{
 *
 */

/** MSC that log a certain type of information for debugging purpose. */
typedef struct RrcDebugMscIndTag
{
    /** Defines what type of information is carried in MSC.
\assoc UNION \ref signal */
    RrcDebugMscType messageType;

#if defined(UPGRADE_NOT_FOR_RELEASE)
    /** Duplicate types are required for genie associations and pass through
     * tasks to work properly. Order must be the same as in the enum. */
#endif
    union {
        RrcDebugMscInternalSignal   internal;
        RrcDebugMscInternalSignal   enqueInternal;
        RrcDebugMscInternalSignal   dequeInternal;

        RrcDebugMscTxExternalSignal txExternal;
        RrcDebugMscRxExternalSignal rxExternal;
        RrcDebugMscRxExternalSignal enqueExternal;
        RrcDebugMscRxExternalSignal dequeExternal;

        RrcDebugMscTimer            timerExpiry;
        RrcDebugMscTimer            timerStart;
        RrcDebugMscTimer            timerStop;

        RrcDebugMscFunction         function;
    }
    signal;
}
RrcDebugMscInd;

/** @} */ /* End of SigRrcDebug group */

/** Defines reason for discarding a message .*/
typedef enum RrcDebugDiscardReasonTag
{
    /** Message is not discarded. */
    RRC_DISCARD_REASON_NO_DISCARD,

    /** Integrity chek information is not available. */
    RRC_DISCARD_REASON_NO_INTEG_CHECK_INFO,

    /** Downlink SN is duplicated. */
    RRC_DISCARD_REASON_SAME_DL_RRC_SN,

    /** Invalid integrity chek information. */
    RRC_DISCARD_REASON_INTEG_CHECK_PRESENT_INVALID
}
RrcDebugDiscardReason;

/** Integrity protection variable declarations for uplink and downlink */
typedef struct UrrAisIntegrityProtectionSrbSpecificInfoTag
{
    /** Defines current uplink COUNT-I */
    URRC_MessageSequenceNumber uplinkRrcSequenceNum;

    /** Defines uplink HFN. */
    Int32                      uplinkRrcHfn;

    /** Indicates if downlinkRrcSequenceNum contains valid information. */
    Boolean                    downlinkRrcSequenceNumPresent;

    /** Defines current downlink COUNT-I. */
    URRC_MessageSequenceNumber downlinkRrcSequenceNum;

    /** Defines downlink HFN. */
    Int32                      downlinkRrcHfn;

    /** URRC_IGNORE_THIS_DL_RRCSN_BEFORE_SMC_REINIT indicates this field not active */
    URRC_MessageSequenceNumber downlinkRrcSequenceNumBeforeSmcReinit;

    /** Pending UL config data, valid if 'uplinkPendingConfig' is TRUE */
    Boolean                    uplinkPendingConfig;

    /** Activation time to apply new HFN, and IK */
    URRC_MessageSequenceNumber ulActTime;

    /** HFN to set when activation time reached */
    Int32                      pendingUplinkRrcHfn;

    /** Pending DL config data, valid if 'downlinkPendingConfig' is TRUE */
    Boolean                    downlinkPendingConfig;

    /** Activation time to apply new HFN, and IK */
    URRC_MessageSequenceNumber dlActTime;

    /** HFN to set when activation time reached */
    Int32                      pendingDownlinkRrcHfn;

    /** Indicates the current IK for uplink */
    USecurityKey               uplinkIntegrityKey;

    /** Indicates uplink KSI */
    KeySeqId                   uplinkKsi;

    /** Indicates the current IK for downlink */
    USecurityKey               downlinkIntegrityKey;

    /** Indicates downlink KSI */
    KeySeqId                   downlinkKsi;

    /** IK to set when activation time reached in either UL or DL */
    USecurityKey               pendingIntegrityKey;

    /** KSI to set when activation time reached in either UL or DL */
    KeySeqId                   pendingKsi;
}
UrrAisIntegrityProtectionSrbSpecificInfo;

/** \defgroup SigRrcDebug RRC Debug Primitives
 * Primitives for debugging purpose.
 * This section defines primitives used for debugging purpose
 * @{
 *
 */

/** This primitive is logged when integrity check is unsuccessful.
 * This primitive is logged prior discarding a message due to the
 * faillure of the integrity check.
 */
typedef struct RrcDebugIntegrityProtectFailIndTag
{
    /** Gives details of received message */
    UReceivedMessageType                     rxMessageType;

    /** Indicates received MAC */
    Int32                                    rxMac;

    /** Defines received COUNT-I. */
    URRC_MessageSequenceNumber               rxRrc_MessageSequenceNumber;

    /* Gives details of current integrity status */
    UrrAisIntegrityProtectionStarted         status;
    Boolean                                  reconfiguration;

    /* details of the XMAC calculation */
    /** Indicates the length of the message. */
    Int16                                    messageBitLength;

    /** Indicates KSI used for downlink. */
    KeySeqId                                 downlinkKsi;

    /** Indicates FRESH value. */
    UIntegrityProtInitNumber                 fresh;

    /** Indicates count-I used for integrity. */
    Int32                                    countI;

    /** Identifies radio bearer on which integrity is applied. */
    BearerIdentity                           bearerIdentity;

    /** Defines calculated XMAC. */
    Int32                                    calculatedXmac;

    /* misc */
    /** Indicates why the message is discarded. */
    RrcDebugDiscardReason                    discardReason;

    /** Defines integrity protection variables. */
    UrrAisIntegrityProtectionSrbSpecificInfo fullSrbDetails;

    /** Indicates first CS domain security procedure completed. */
    Boolean                                  debugFirstCSSecurityCommand;

    /** Indicates first PS domain security procedure completed. */
    Boolean                                  debugFirstPSSecurityCommand;

    /** Contains 'modified' message data which is passed into the integrity
      * check algorithmn.
      */
    Int8 msgData [MAX_AIR_INTERFACE_DEBUG_OCTETS];
}
RrcDebugIntegrityProtectFailInd;

/** the code will assert if there are more than 64 RBC transactions preset
 * at one time */
#define TRANS_LIST_DEBUG_MAX_SIZE     64

/** Logs the list of transactions currently outstanding. */
typedef struct RrcDebugTransListIndTag
{
    /** Indicates number of the outstanding transactions.
\assoc ARRAY \ref transMsg */
    Int8                 numberOfTransactions;

    /** Lists type of the outstanding transactions. */
    UReceivedMessageType transMsg [TRANS_LIST_DEBUG_MAX_SIZE];
} RrcDebugTransListInd;

#ifdef UPGRADE_3G_TDD128
typedef struct RrcDebugIntraCellMeasReportTDD128Tag
{
    /** Specifies the cell parameters Id of the TDD128 cell. */
    UCellParametersID                   cellParametersID;

    /** Specifies the RSCP measurement for the P-CCPCH in 1/8 dBm steps, \ref
     * example
     */
    SignedInt16                         p_ccpchRscp;
}
RrcDebugIntraCellMeasReportTDD128;

typedef struct RrcDebugInterCellMeasReportTDD128Tag
{
    /** The frequency on which the measurement was made. */
    UUARFCN                             frequency;

    /** Specifies the cell parameters Id of the TDD128 cell. */
    UCellParametersID                   cellParametersID;

    /** Specifies the RSCP measurement for the P-CCPCH in 1/8 dBm steps, \ref
     * example
     */
    SignedInt16                         p_ccpchRscp;
}
RrcDebugInterCellMeasReportTDD128;

/** The number of intra cells measured for debugoutput, including
 * CPHY_MAX_REPORTS plus Serving Cell
 */
#define INTRA_CELL_DEBUG_MAX_REPORTS     (CPHY_MAX_REPORTS + 1)

typedef struct RrcDebugIntraFreqMeasIndTag
{
    /** The number of intra cells measured for debugoutput.
\assoc ARRAY \ref intraCellReport */
    Int8                                numberOfCells;

    /** intra cell's measured information(Cpi and PCCPCH RSCP) for debugoutput. */
    RrcDebugIntraCellMeasReportTDD128   intraCellReport [INTRA_CELL_DEBUG_MAX_REPORTS];
}RrcDebugIntraFreqMeasInd;


typedef struct RrcDebugInterFreqMeasIndTag
{
    /** Specifies the RSCP measurement for the P-CCPCH in 1/8 dBm steps, \ref
     * example
     */
    SignedInt16                         sCellPccpchRscp;

    /** The frequency of latest CphyMonitorInterFreqCellInd, which is useful in
        NON-DCH mode.*/
    UUARFCN                             frequency;

    /** The number of inter freq cells measured for debugoutput.
\assoc ARRAY \ref interCellReport */
    Int8                                numberOfCells;

    /** inter cell's measured information(ARFCN, Cpi and PCCPCH RSCP) for debugoutput. */
    RrcDebugInterCellMeasReportTDD128   interCellReport [maxCellMeas];
}RrcDebugInterFreqMeasInd;

#endif /* UPGRADE_3G_TDD128 */

typedef struct RrcConfigInfoIndTag
{
    /** Lines */
    Int32 lines;

    /*Config file name
\assoc STRING   */
    Int8   configFileName[20];
}
RrcDebugConfigInfoInd;

typedef union UrrCellIdentityUnionTag
{
    UPrimaryScramblingCode primaryScramblingCode;
    UCellParametersID      cellParametersID;
} UrrCellIdentityUnion;

/**
 * Engineering mode UMTS cell information.
 */
typedef struct UEngCmdUmtsCellInfoTag
{
    /** The DL ARFCN of the cell. */
    UUARFCN                 arfcn_dl;

    /** UmtsMode: FDD or TDD128.
\assoc UNION \ref pscOrCpi */
    UmtsMode                umtsMode;

    /** The psc or cpi of the cell. */
    UrrCellIdentityUnion    pscOrCpi;
}
UEngCmdUmtsCellInfo;

#if defined (UPGRADE_GGE)
/**
 * Engineering mode GSM cell information.
 */
typedef struct UEngCmdGsmCellInfoTag
{
    /** Specifies the value of this particular GSM cell's ARFCN. */
    UBCCH_ARFCN             bcch_arfcn;

    /** The BSIC of GSM cell.*/
    Int8                    bsic;
}
UEngCmdGsmCellInfo;
#endif

/**
 * Engineering mode cell information.
 */
typedef union UEngCmdCellInfoUnionTag
{
    /** UMTS cell information.*/
    UEngCmdUmtsCellInfo     umtsCellInfo;

#if defined (UPGRADE_GGE)
    /** GSM cell information.*/
    UEngCmdGsmCellInfo      gsmCellInfo;
#endif
}
UEngCmdCellInfoUnion;

/**
 * Engineering mode command 'LOCK_CELL' information.
 */
typedef UEngCmdUmtsCellInfo UEngCmdLockCellInfo;

/**
 * Engineering mode command 'LOCK_ARFCN' information.
 */
typedef struct UEngCmdLockArfcnInfoTag
{
    /** The DL ARFCN of the cell */
    UUARFCN     arfcn_dl;
}
UEngCmdLockArfcnInfo;

/**
 * Engineering mode command 'SPECIFIED_ARFCN_SCAN' information.
 */
typedef UEngCmdLockArfcnInfo UEngCmdSpecifiedArfcnScanInfo;

/**
 * Engineering mode command 'TRIGGER_HANDOVER' information.
 */
typedef struct UEngCmdTriggerHandoverInfoTag
{
    /** Tell RRC destination neighbor cell type which is intra-freq, inter-freq or inter-rat cell.
\assoc UNION \ref cellInfo */
    UEngNCellType           nCellType;

    /** Cell info. */
    UEngCmdCellInfoUnion    cellInfo;

    /** Possible range is from 3 to 10 seconds, Default value is 5 seconds.
      */
    Int8                    timerPeriod;
}
UEngCmdTriggerHandoverInfo;

/**
 * Engineering mode command 'HANDOVER_PERMITTED' information.
 */
typedef struct UEngCmdHandoverPermittedInfoTag
{
    /** False indicate switching off handover function,
      * true means executing handover normally.
      */
    Boolean         handoverSwitch;
}
UEngCmdHandoverPermittedInfo;

/**
 * Engineering mode command 'TRIGGER_CELL_RESELECTION' information.
 */
typedef struct UEngCmdTriggerCellReselectionInfoTag
{
    /** Tell RRC destination neighbor cell type which is intra-freq, inter-freq or inter-rat cell.
\assoc UNION \ref cellInfo */
    UEngNCellType           nCellType;

    /** Cell info. */
    UEngCmdCellInfoUnion    cellInfo;
}
UEngCmdTriggerCellReselectionInfo;

/**
 * Engineering mode command 'SELECT_A_CELL_ONE_TIME' information.
 */
typedef UEngCmdUmtsCellInfo UEngCmdSelectACellOneTimeInfo;

/**
 * Engineering mode command information.
 */
typedef union UEngCommandInfoUnionTag
{
    /** For command 'LOCK_CELL' */
    UEngCmdLockCellInfo                 lockCellInfo;

    Int8                                dummy1;

    /** For command 'LOCK_ARFCN' */
    UEngCmdLockArfcnInfo                lockArfcnInfo;

    Int8                                dummy2;

    Int8                                dummy3;

    /** For command 'SPECIFIED_ARFCN_SCAN' */
    UEngCmdSpecifiedArfcnScanInfo       specifiedArfcnScanInfo;

    /** For command 'TRIGGER_HANDOVER' */
    UEngCmdTriggerHandoverInfo          triggerHandoverInfo;

    /** For command 'HANDOVER_PERMITTED' */
    UEngCmdHandoverPermittedInfo        handoverPermittedInfo;

    /** For command 'TRIGGER_CELL_RESELECTION' */
    UEngCmdTriggerCellReselectionInfo   triggerCellReselectionInfo;

    /** For command 'SELECT_A_CELL_ONE_TIME' */
    UEngCmdSelectACellOneTimeInfo       selectACellOneTimeInfo;
}
UEngCommandInfoUnion;

/** Signal structure for carrying RRC engineering mode information.
 * Sent from RRC to the AB layer.
 */
typedef struct RrcEngInfoIndTag
{
    /** Defines RRC mode and connected states. */
    UEngRrcModeState    rrcModeState;

    /** Defines UE Operation. */
    UEngUeInfo          ueOperation;

    /** Indicates if UE operation data is required
\assoc PRESENT \ref ueOperation */
    Boolean             ueOperationPresent;

    /** Carries the contents of the supplied serving cell */
    UEngServingCellInfo servingCell;

    /** Indicates if current Serving Cell data is required
     * and supported by the current mode/state.

\assoc PRESENT \ref servingCell */
    Boolean servingCellPresent;

    /** Carries the contents of the supplied Intra-Frequency
     * FDD cell list structure.
     */
    UEngIntraFreqCellList intraFreqCells;

    /** Set to TRUE if Intra-Frequency FDD Cell data is required.
\assoc PRESENT \ref intraFreqCells */
    Boolean               intraFreqCellsPresent;

    /** Carries the contents of the supplied Inter-Frequency cell. */
    UEngInterFreqCellList interFreqCells;

    /** Set to TRUE if Inter-Frequency FDD Cell data is required.
\assoc PRESENT \ref interFreqCells */
    Boolean               interFreqCellsPresent;
#if defined (UPGRADE_GGE)
    /** Contains information of the supplied Inter-RAT GSM cell. */
    UEngGsmCellList       interRatGsmCells;

    /** Set to TRUE if Inter-RAT GSM Cell data is required.
\assoc PRESENT \ref interRatGsmCells */
    Boolean               interRatGsmCellsPresent;
#endif /* UPGRADE_GGE */

#if defined (ENABLE_AT_TRACE)
    /** Set to TRUE if AT trace info included, when other elements in this structure
      * are ignored.
      */
    Boolean               atTraceIncluded;

    /** Air interface message category ,like UL_DCCH.*/
    Int8                  message;

    /** Air interface message type in specific category, like cellUpdataConfirm.*/
    Int32                 messageType;

    /** PDU length.*/
    Int32                 dataLen;

    /** PDU data.*/
    Int8                * data_p;
#endif/* ENABLE_AT_TRACE*/
}
RrcEngInfoInd;

/** Signal structure for configuring engineering mode within the RRC.
 * Sent from the UMM to the RRC.
 */
typedef struct RrcEngInfoReqTag
{
#if defined(UPGRADE_ADDIT_ENG_INFO)
    /** Configures engineering mode */
    Int32   engModeConfig;
#else
    /** Indicates if engineering mode is enable */
    Boolean engModeActive;
#endif
}
RrcEngInfoReq;

/** Signal structure for compulsive camp or select on particular cell command.
 * Sent from the UMM to the RRC.
 */
typedef struct RrcForceCommandReqTag
{
    /** force handle command
\assoc UNION \ref commandInfo */
    UEngForceCommand        forceHandleCommand;

    /** Carries the contents of force command information. */
    UEngCommandInfoUnion    commandInfo;
}
RrcForceCommandReq;

/** Signal structure for carrying RRC handle force command result.
 * Sent from RRC to the AB layer.
 */
typedef struct RrcForceCommandCnfTag
{
    /** Set to TRUE if force command is successful handle is required */
    Boolean                     commandSuccessHandle;

    /** Carries the contents of the force command return error cause */
    UEngForceCommandFailCause   commandFailCause;

    /** force handle command
\assoc UNION \ref commandResultInfo */
    UEngForceCommandResultType  commandResultType;

    /** Carries the contents of the force command result information */
    UEngCommandResultInfoUnion  commandResultInfo;
}
RrcForceCommandCnf;

/** @} */  /* End of SigRrcDebug group */
/** @} */ /* End of SigRrc group */
#endif

/* END OF FILE */

