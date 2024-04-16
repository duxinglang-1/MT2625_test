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
 *  File Description :                                                   */

/** \file
 *
 *      GRR interface signal definitions.
 **************************************************************************/

#ifndef GRR_SIG_H
#define GRR_SIG_H

/****************************************************************************
 * Nested Include Files
 ****************************************************************************/

#if !defined (L3_TYP_H)
#include     <l3_typ.h>
#endif

#if !defined (GRR_TYP_H)
#include <grr_typ.h>
#endif

#if !defined (GRRMRTYP_H)
#include     <grrmrtyp.h>
#endif

#include <as_nas_common_typ.h>

#if !defined (GPGENTYP_H)
#include <gpgentyp.h>
#endif

#if defined (UPGRADE_3G)
#if !defined (RRCMRTYP_H)
#include     <rrcmrtyp.h>
#endif
#endif

#if defined (UPGRADE_3G)
#if !defined (UBND_CFG_H)
#include     <ubnd_cfg.h>
#endif
#endif

#if !defined (U1_TYP_H)
#include <u1_typ.h>
#endif

#if !defined (MMR_SIG_H)
#include <mmr_sig.h>
#endif

#define NUM_UMTS_NCELL_INFO                8
#define NUM_DCH_NCELL_DEBUG_CELLS          12

/* Defines max size of GRR debug messages in octets */
#define MAX_GRR_INTERFACE_DEBUG_OCTETS     256

#define ENG_MAX_FREQUENCE_NUMBER          20



/** \defgroup SigGrr GRR Signalling Interface
 * \ingroup GSMProtoStack
 * \ingroup Sig
 * Signalling interface between (G)MM and (G)RR tasks.
 * This section defines all the primitives exchanged between
 * (G)MM and (G)RR, hereinafter called the GRR interface.
 *
 * GRR is a combined GSM/GPRS/EGPRS module that controls Layer 1, communicates with
 * the network via RLC-MAC and provides information to the higher layers.
 * In addition to handling circuit switched idle mode and dedicated mode operation
 * in the normal way, RR/GRR provides the following GPRS features:<UL>
 * <LI> initial network access (RACH/PRACH)
 * <LI> measurement acquisition and reporting
 * <LI> neighbour cell BSIC decode requests
 * <LI> cell selection/reselection
 * <LI> monitoring of system information
 * <LI> paging reception.
 * </UL>
 *
 * The context of  GRR signalling interface is shown in the Diagram below.
 *
 * \image html GSM_Interface.png
 * @{
 */

/****************************************************************************
 * Macros
 ****************************************************************************/

/****************************************************************************
 * Types
 ****************************************************************************/

/** Specifies the type of buffer used to store system information (SI) */
typedef enum SiBufferTypeTag
{
    /** No BCCH buffer available */
    NO_BCCH_BUFFER = 0,

    /** The serving cell BCCH buffer */
    SCELL_BCCH_BUFFER = 1,

    /** The neighbour cell BCCH buffer */
    NCELL_BCCH_BUFFER = 2,

    /** The reselection parameters buffer */
    RESEL_PARAM_BUFFER = 3,

    /** Frequency list buffer */
    FREQ_LIST_BUFFER = 4,

    /** Reference Frequency List buffer */
    RFL_BUFFER = 5,

    /** BSIC description list buffer */
    BSIC_DESCR_BUFFER = 6,
#if defined (UPGRADE_3G_FDD)
    /** UTRAN FDD neighbour cell BCCH buffer */
    UTRAN_FDD_NCELL_BUFFER = 7,
#endif /* UPGRADE_3G_FDD */
#if defined (UPGRADE_3G_TDD128)
    /** UTRAN TDD128 neighbour cell BCCH buffer */
    UTRAN_TDD128_NCELL_BUFFER = 8,
#endif /* UPGRADE_3G_TDD128 */
       /** Last element in SI buffer type */
    LAST_SI_BUFFER_TYPE
}
SiBufferType;

/** Contains measurement information about the current serving cell.
 * Only the levelFull field is valid when the MS is in idle mode.
 */
typedef struct ServingCellMeasTag
{
    /** TRUE if uplink DTX was used in the last reporting period */
    Boolean dtxUsed;

    /** Is the received signal level averages over all bursts in the last reporting period (0-63) */
    Int8    levelFull;

    /** The received signal level average over the subset of bursts which must be present if
     * the network is using downlink DTX in the last reporting period (0-63)
     */
    Int8 levelSubset;

    /** Full receive quality for the serving cell.
     * The receive quality is a value between 0 and 7 and indicates a
     * quantised bit error rate, as according to the following table,
     * reproduced from 05.08, section 8.2.4.
     *
     * <TABLE>
     * <TR><TD>RXQUAL_0</TD><TD>     </TD><TD>    BER&lt;</TD><TD> 0.2%</TD><TD>Assumed value = 0.14%</TD></TR>
     * <TR><TD>RXQUAL_1</TD><TD> 0.2%</TD><TD>&lt;BER&lt;</TD><TD> 0.4%</TD><TD>Assumed value = 0.28%</TD></TR>
     * <TR><TD>RXQUAL_2</TD><TD> 0.4%</TD><TD>&lt;BER&lt;</TD><TD> 0.8%</TD><TD>Assumed value = 0.57%</TD></TR>
     * <TR><TD>RXQUAL_3</TD><TD> 0.8%</TD><TD>&lt;BER&lt;</TD><TD> 1.6%</TD><TD>Assumed value = 1.13%</TD></TR>
     * <TR><TD>RXQUAL_4</TD><TD> 1.6%</TD><TD>&lt;BER&lt;</TD><TD> 3.2%</TD><TD>Assumed value = 2.26%</TD></TR>
     * <TR><TD>RXQUAL_5</TD><TD> 3.2%</TD><TD>&lt;BER&lt;</TD><TD> 6.4%</TD><TD>Assumed value = 4.53%</TD></TR>
     * <TR><TD>RXQUAL_6</TD><TD> 6.4%</TD><TD>&lt;BER&lt;</TD><TD>12.8%</TD><TD>Assumed value = 9.05%</TD></TR>
     * <TR><TD>RXQUAL_7</TD><TD>12.8%</TD><TD>&lt;BER    </TD><TD>     </TD><TD>Assumed value =18.10%</TD></TR>
     * </TABLE>
     *
     * The full receive quality includes all received blocks, so tends to be
     * high (i.e. of poor quality) during periods where the network is utilising
     * discontinuous transmission on the downlink.
     */
    Int8 qualityFull;

    /** Sub-set receive quality for the serving cell.
     * This is similar to the \a qualityFull, but only contains the measurements
     * of the mandatory sub-set of radio blocks.  It is similar to the
     * \a levelSubset parameter, but for Bit Error Rate in place of RX level.
     */
    Int8  qualitySubset;

    /** The timing advance value used during last reporting period */
    Int8  timingAdv;

    /** The currently commanded TX power level */
    Int8  txPowerLevel;
#if defined (DEVELOPMENT_VERSION)
    /** Bit errors averaged over all bursts (0-7) */
    Int32 bitErrQualityFull;

    /** The bit errors averaged over the subset of bursts that must be present
     * if the network is using downlink DTX (0-7)
     */
    Int32 bitErrQualitySubset;
#endif
}
ServingCellMeas;

/** Engineering mode commands **/
typedef enum GrrEngineeringCommandTag
{    
    ENG_LOCK_CELL,    
    ENG_UNLOCK_CELL,    
    ENG_LOCK_ARFCN,    
    ENG_UNLOCK_ARFCN,    
    ENG_FULL_SCAN,    
    ENG_SPECIFIED_ARFCN_SCAN,    
    ENG_TRIGGER_HANDOVER,    
    ENG_HANDOVER_PERMITTED,    
    ENG_TRIGGER_CELL_RESELECTION,    
    ENG_SELECT_A_CELL_ONE_TIME,   
    ENG_POWER_SAVING,
    ENG_NO_COMMAND
}
GrrEngineeringCommand;

/** Engineering mode GSM cell information. **/
typedef struct EngGsmCellInfoTag
{    
    /** The ARFCN of GSM cell.*/    
    Arfcn                   arfcn;    
    /** The BSIC of GSM cell.*/    
    Int8                    bsic;
}
EngGsmCellInfo;


/** Engineering mode power saving type, Only used when command is  * ' POWER_SAVING '. */
typedef enum EngPowerSavingTypeTag
{ 
    /** Invalid value  */ 
    ENG_POWER_SAVING_OFF = 0,
 
    /** NO CELL - all RxLev is lower than -105dbm */ 
    ENG_POWER_SAVING_NO_CELL = 1,

    /** NO SYNC - FB sync always fail for all Arfcns */ 
    ENG_POWER_SAVING_NO_SYNC_0 = 2,

    /** NO SYNC - FB sync always fail if already has 5 Arfcns which FB sync successful. */
    ENG_POWER_SAVING_NO_SYNC_5 = 3,

    /** NO SYNC - FB sync always fail if already has 6 Arfcns which FB sync successful. */
    ENG_POWER_SAVING_NO_SYNC_6 = 4,

    /** SYNC - FB sync always successful. */
    ENG_POWER_SAVING_SYNC_OK_ALL = 5,

    /** NO BA - The BA list is empty */
    ENG_POWER_SAVING_NO_BA = 6
}
EngPowerSavingType;


/** * Engineering mode command 'POWER_SAVING' information. */
typedef struct EngCmdPowerSavingInfoTag
{ 
    EngPowerSavingType    powerSavingType; 
}
EngCmdPowerSavingInfo;


typedef enum EngCommandFailureCauseTag
{
    ENG_CELL_NORMAL_SERVICE = 0,   
        
    /** Command can NOT be overrided. */    
    ENG_CMD_OVERRIDED,   
    
    /** Command is not permitted because of CSR status.*/    
    ENG_CMD_FORBIDDEN,
    
    /** ACCEPTABLE cell */    
    ENG_CELL_LIMITED_SERVICE,
    
    /** Target cell can'be camped on suitable */    
    ENG_CELL_NOT_CAMP,
    
    /** Target cell is not valid  */    
    ENG_CELL_NOT_VALID,
    
    /** Rssi scan only support in IDLE MODE,    
        * not support connected and deactivated mode    */    
    ENG_UE_NOT_IN_IDLE_MODE,    
    
    /** In first step, full Rssi scan only support in CSRR Proc,    
        * not support CSRC,CSRS,CSRP,NOT_PROC    */    
    ENG_FULL_SCAN_UE_NOT_IN_CSRR_PROC, 
    
    /** In first step, if RBC,CER should process SIR msg or    
         * pending request exist, full Rssi scan will be blocked    */    
    ENG_FULL_SCAN_BLOCKED,    

    /** Can be used for all commands,      
         * Indicate command process failure for unspecified cause.      */    
    ENG_UNSPECIFIED_CAUSE,    

    /** Indicate command can't be processed in current state.       */    
    ENG_STATE_MISMATCH,    

    /** Indicate target cell is not requested one.      */    
    ENG_OCCURRED_BUT_CELL_MISMATCH,    

    /** Indicate current camp on cell is locked.      */    
    ENG_CELL_LOCKED,    

    /** Indicate current camp on cell whose arfcn is locked.      */    
    ENG_ARFCN_LOCKED,    

    /** Indicate request handover is not occurred during specified timer.      */    
    ENG_NOT_OCCURRED_WHEN_TIMER_EXPIRY,    

    /** Indicate target cell's PLMN is not belongs to serving cell's PLMN or EPLMNs.        */    
    ENG_PLMN_MISMATCH
}
EngCommandFailureCause;

/**
 * Engineering mode information for
 * an arfcn rssi.
 */
typedef struct EngArfcnLevelTag
{
    Int16 arfcn;
    Int8  level;
}
EngArfcnLevel;


/**
 * Engineering mode information for
 * all frequency scan result.
 */
typedef struct EngFullScanResultListTag
{
    /** The number of scan frequency */
    Int16                    arfcnNumber;

    /** All frequency scan rssi result */
    EngArfcnLevel       arfcnLevelList[ENG_MAX_FREQUENCE_NUMBER];
}
EngFullScanResultList;


typedef struct EngGsmCellLevelTag
{
    Int8   bsic;                  
    Int8   level;
}
EngGsmCellLevel;

/** * Engineering mode command 'LOCK_CELL' information. */
typedef EngGsmCellInfo EngCmdGsmCellInfo;

/** * Engineering mode command 'LOCK_CELL' information. */
typedef EngGsmCellInfo EngCmdLockCellInfo;


/** * Engineering mode command 'LOCK_ARFCN' information. */
typedef struct EngCmdLockArfcnInfoTag
{        
    Arfcn     arfcn;
}
EngCmdLockArfcnInfo;


/** * Engineering mode command 'SPECIFIED_ARFCN_SCAN' information. */
typedef EngCmdLockArfcnInfo EngCmdSpecifiedArfcnScanInfo;

/** Engineering mode NCELL type, Only used when command is   * 'TRIGGER_HANDOVER' or 'TRIGGER_CELL_RESELECTION'.  */
typedef enum EngNCellTypeTag
{    
    /** Invalid value  */    
    ENG_NCELL_INVALID_TYPE = 0,    
    
    /** GSM neighbour cell */    
    ENG_NCELL_NOT_INTER_RAT,        

    /** Inter-rat neighbour cell */    
    ENG_NCELL_INTER_RAT
}
EngNCellType;

typedef union Eng3gCellIdentityUnionTag
{
#if defined (UPGRADE_3G_FDD)
    Int16    primaryScramblingCode;
#endif
#if defined (UPGRADE_3G_TDD128)
    Int8     cellParametersID;
#endif
} Eng3gCellIdentityUnion;


/** * Engineering mode UMTS cell information. */
typedef struct EngCmdUmtsCellInfoTag
{    
    /** The DL ARFCN of the cell. */    
    Int16                 arfcn_dl;    

    /** UmtsMode: FDD or TDD128. \assoc UNION \ref pscOrCpi */    
    UmtsMode                umtsMode;

    /** The psc or cpi of the cell. */    
    Eng3gCellIdentityUnion    pscOrCpi;
}
EngCmdUmtsCellInfo;    


typedef union EngCmdCellInfoTag
{ 
#if defined (UPGRADE_3G)
    /** 3G cell info. */    
    EngCmdUmtsCellInfo     umtsCellInfo;    
#endif    
    /** GSM cell info. */    
    EngCmdGsmCellInfo      gsmCellInfo; 
}
EngCmdCellInfo;


/** * Engineering mode command 'TRIGGER_HANDOVER' information. */
typedef struct EngCmdTriggerHandoverInfoTag
{       
    /** \assoc UNION \ref cellInfo */
    EngNCellType      nCellType;    
        
    EngCmdCellInfo    cellInfo;        
    
    /** Possible range is from 3 to 10 seconds, Default value is 5 seconds.      */    
    Int8              timerPeriod;
}
EngCmdTriggerHandoverInfo;

/** * Engineering mode command 'HANDOVER_PERMITTED' information. */
typedef struct EngCmdHandoverPermittedInfoTag
{    
    /** False indicate switching off handover function,       
        * true means executing handover normally.      
        */    
    Boolean         handoverSwitch;
}
EngCmdHandoverPermittedInfo;

/** * Engineering mode command 'TRIGGER_CELL_RESELECTION' information. */
typedef struct EngCmdTriggerCellReselectionInfoTag
{    
    /** \assoc UNION \ref cellInfo */
    EngNCellType      nCellType;        
    
    EngCmdCellInfo    cellInfo;    
}
EngCmdTriggerCellReselectionInfo;

/** * Engineering mode command 'SELECT_A_CELL_ONE_TIME' information. */
typedef EngCmdGsmCellInfo EngCmdSelectACellOneTimeInfo;

typedef union EngCommandInfoTag
{    
    /** For command 'LOCK_CELL' */    
    EngCmdLockCellInfo    lockCellInfo;    

    /** For command 'LOCK_ARFCN' */    
    EngCmdLockArfcnInfo   lockArfcnInfo;    

    /** For command 'SPECIFIED_ARFCN_SCAN' */    
    EngCmdSpecifiedArfcnScanInfo    specifiedArfcnScanInfo;    

    /** For command 'TRIGGER_HANDOVER' */    
    EngCmdTriggerHandoverInfo    triggerHandoverInfo;    

    /** For command 'HANDOVER_PERMITTED' */    
    EngCmdHandoverPermittedInfo    handoverPermittedInfo;    

    /** For command 'TRIGGER_CELL_RESELECTION' */    
    EngCmdTriggerCellReselectionInfo    triggerCellReselectionInfo;    

    /** For command 'SELECT_A_CELL_ONE_TIME' */    
    EngCmdSelectACellOneTimeInfo    selectACellOneTimeInfo;

    /** For command 'POWER_SAVING' */
    EngCmdPowerSavingInfo    powerSavingInfo;
}
EngCommandInfo;


/****************************************************************************
 * Signal types
 ****************************************************************************/

/** This signal is sent to (G)RR while the MS is in Idle mode. (G)RR will attempt to camp on
 * the best cell for the requested PLMN. If no PLMN is specified (G)RR will camp on the best
 * cell in any PLMN, but only emergency calls will be allowed. This is the only signal that
 * can get (G)RR out of the NULL state except the Inter-RAT case.
 *
 * \note The serviceType cannot be used to bias cell selection to GPRS or circuit switched when
 * the MS is GPRS class C; (G)RR will always choose the best cell irrespective of service offered.
 */
typedef struct GrrActReqTag
{
    /** Returned in the GrrActCnf to tie the confirm to the request */
    Int16                 sequenceId;

    /** Defines parameters which (G)RR should use for cell selection */
    SelectionParameters   selectionParameters;

    /** Service offered */
    ServiceType           serviceType;
#if defined (UPGRADE_3G)
    /** Indicates if L1 needs to SWITCH_RAT or ACTIVATE_RAT to GSM */
    RatActivateMode       ratActivateMode;

    /** handsetModes set to GSM, GSM+FDD or GSM+TDD
\assoc UNION \ref cellInfoAndBands */
    NetworkModes          handsetModes;

    /** Defines cell information and bands for the related network mode. */
    CellInfoAndBandsUnion cellInfoAndBands;
#endif
#if defined (PRODUCT_CALIBRATION_TEST_SWITCH)
    Boolean            lockCellEnable;
#endif /* PRODUCT_CALIBRATION_TEST_SWITCH */
}
GrrActReq;

/** This signal is sent to (G)MM when (G)RR reselects to a new Location Area or
 * Routing Area or the service status has changed.
 */
typedef struct GrrActIndTag
{
    /** Copied from #GrrActReq */
    Int16               sequenceId;

    /** Indicates if (G)RR has found any available cell to camp on */
    ActivateStatus      status;

    /** The selected cell ID */
    CellId              cellId;

    /** Updated system information */
    MmSystemInformation sysInfo;

    /** Service provided in the new cell */
    ServiceType         serviceType;
}
GrrActInd,

/** This signal is sent to (G)MM in response to a GrrActReq when (G)RR camps on a cell.
 * The signal will also be sent if (G)RR cannot find any cell to camp on. It contains data
 * about the cell camped and the activate status.
 */
GrrActCnf;

/** This signal activates diagnostics in (G)RR. Resulting signals are sent
 * directly from (G)RR to the application layer.
 */
typedef struct GrrTestModeReqTag
{
#if defined (UPGRADE_ADDIT_ENG_INFO)
    /* This parameter uses a 32-bit control field to turn specific test mode information
     * on and off
     */
    Int32   accStratumTestMode;
#else
    /** Indicates if dedicated mode is active */
    Boolean dedModeActive;

    /** Indicates if idle mode is active */
    Boolean idleModeActive;

    /** Whether or not GPRS is active */
    Boolean gprsActive;
#endif
}
GrrTestModeReq;

/** This signal is sent to (G)RR at initialisation. It contains all the initial data
 * for the mobile equipment.
 */
typedef struct GrrMeDataReqTag
{
    /** The International Mobile Equipment Identity is a unique identifier allocated to each ME*/
    MobileIdElement imei;

    /** Traffic channel types supported */
    MsCapability    msCapability;

    /** The aspects of MS;
     * mobiles when communicating to the network will initially indicate their classmark,
     * this includes service, equipment and radio capabilities
     */
    MsClassmarkElement msClassmark;

    /** Allows the two digit software version number to be added to the standard
     * IMEI for transmission to the network when IMEISV is requested
     */
    Svn                     svn;

    /** Capabilities related to GPRS */
    GprsCapability          gprsCapability;

    /** Specifies the service type that is preferred */
    ServiceType             preferredServiceType;
#if defined(UPGRADE_EDGE)
    /** Capabilities related to EGPRS */
    EgprsCapability         egprsCapability;
#endif
#if defined (UPGRADE_3G)
    /** UMTS mobile equipment capability */
    UmtsMobileEquipmentData umtsMobileEquipmentData;
#endif
}
GrrMeDataReq;

/** This signal is sent to (G)RR at initialisation. It contains data
 * from the SIM and is only updated when the SIM is changed
 */
typedef struct GrrMsDataReqTag
{
    /** Set to TRUE if SIM is present */
    Boolean         msDataValid;

    /** The value of the IMSI from the USIM */
    MobileIdElement imsi;

    /** Will be set to 0xff if invalid or not present */
    Plmn            hplmn;

    /** The access class of the current HPLMN */
    Int16           accessClass;

    /** Defines the operation mode of the MS */
    MsOperationMode msOperationMode;
#if defined (UPGRADE_3G)
    /** START-CS */
    Int32           startCs;
#if defined (UPGRADE_REL6)
    /** START-PS */
    Int32           startPs;
#endif
#endif

#if defined(ENABLE_EHPLMN)
    /** Indicates the number of EHPLMNs in the list Ehlmns. */
    Int8            numberofEhplmns;

    /** Contains the list of PLMNs that are considered to be HPLMN */
    Plmn            ehplmns [MAX_EHPLMNS];
#endif /* ENABLE_EHPLMN */
}
GrrMsDataReq;

/** This signal is sent to (G)RR whenever (G)MM needs to report a change in the ID
 * used to page the MS for circuit switched and GPRS operation, the ciphering
 * information, or the pageability of the MS.
 *
 * Providing the LAI in the message matches the LAI of the cell camped on, then
 *(G)RR will update its copy of the paging and ciphering information as appropriate.
 *
 * \note If csMode and gpMode are set to AM_FORBIDDEN_LA or AM_FORBIDDEN_NAT_LA then
 * (G)RR adds the lai to the list of forbidden LAs. An xxMode of NORMAL removes the
 * forbidden area.
 */
typedef struct GrrUpdateReqTag
{
    /** LAI; Identifies uniquely a location area for a GSM-MAP type of PLMN */
    Lai              lai;

    /** Mobile Id given to GRR so that it can respond to CS paging */
    MobileIdElement  csPagingId;

    /** Mobile Id given to GRR so that it can respond to GPRS paging */
    MobileIdElement  gprsPagingId;

    /** Current Temporary Logical Link Identifier (TLLI), filled in by MM. */
    Int32            tlli;

    /** New ciphering information; only applies to circuit switched calls */
    CipheringKeyInfo cipheringInfo;

    /**  The csMode is relevant to this area identity. If csMode is set to AM_FORBIDDEN_LA
    * or AM_FORBIDDEN_NAT_LA then URRC adds the cellAreaIdentity to
    * the relevant list of forbidden areas. A csMode of AM_NORMAL
    * removes the forbidden area.
    */
    ActivateMode csMode;

    /** Whether or not non-DRX is applied */
    Boolean      applyNonDrx;

    /**  The gpMode is relevant to this area identity. If gpMode is set to AM_FORBIDDEN_LA
     * or AM_FORBIDDEN_NAT_LA then URRC adds the cellAreaIdentity to
     * the relevant list of forbidden areas. A gpMode of AM_NORMAL
     * removes the forbidden area.
     */
    ActivateMode gpMode;

    /** Indicates the value of the DRX cycle length coefficient to be
     * used for the PS domain. The value 0 indicates that the value to
     * be used is that broadcast in SI. A value between 6 and 9 indicates
     * a specific DRX cycle length coefficient.
     */
    GprsDrxParameters drxParameters;

    /** If set to FALSE, that means the sim is removed  then the list of forbidden LAs is cleared completely  */
    Boolean           simPresent;

    /** Indicates if equivalent PLMN list has been changed */
    Boolean           ePlmnListChanged;

    /** Contains the list of PLMNs that are considered to be equivalent
    * to the Selected PLMN given by LAI.
    * This list will always contain the PLMN on which the list was received.
    */
    EquivalentPlmnList ePlmnList;
#if defined (UPGRADE_3G)
    /** Indicates the uArfcn when 3G -> 2G cell reselection is still valid or not */
    Boolean            umtsRatUArfcnValid;

    /** Indicates if there is a MM or GMM procedure running  */
    Boolean            irrProcRunning;

    /** START- CS */
    Int32              startCs;
# if defined (UPGRADE_REL6)
    /** START- PS */
    Int32              startPs;
# endif
#endif
}
GrrUpdateReq;

/** This signal is sent to (G)MM in response to a GrrPlmnListReq.
 * It contains the status of the scan and, if the scan was successful,
 * a list of all those PLMNs found.
 */
typedef struct GrrPlmnListCnfTag
{
    /** Indicates the outcome of the PLMN search procedure */
    PlmnListStatus status;

    /** lists all the PLMNs found */
    PlmnList       list;
#if defined (UPGRADE_3G)
    /** Contains set of frequencies which are detected */
    UarfcnList     uArfcnSrchList;
#endif
}
GrrPlmnListCnf;

/** This signal is sent when GRR detectes a change in the NAS system information.
 * This signal is sent from (G)RR to (G)MM in Idle mode if any of the following fields change
 * in the system information for the serving cell:<UL>
 * <LI> BCCH Allocation
 * <LI> TMSI Time (T3212)
 * <LI> IMSI Attach/Detach allowed Flag
 * <LI> Change of GPRS network mode
 * <LI> Change of access class
 * <LI> Change of service on this cell (e.g. GPRS service found on this cell)
 * </UL>
 */
typedef struct GrrSysInfoIndTag
{
    /** Updated system information */
    MmSystemInformation sysInfo;

    /** Service provided by the cell */
    ServiceType         serviceType;
}
GrrSysInfoInd;

/** Not used in any TPLGSM source code */
typedef struct GrrClassmarkRequestTag
{
    /** The aspects of MS;
     * mobiles when communicating to the network will initially indicate their classmark,
     * this includes service, equipment and radio capabilities
     */
    MsClassmarkElement msClassmark;
}
GrrClassmarkReq;

/** This signal is sent by (G)MM to (G)RR to trigger a search for the
 * available PLMNs that the UE can see.
 * (G)MM only sends this to (G)RR while the MS is in Idle mode. If a PLMN is included in the
 * signal then (G)RR will stop the scan as soon as this PLMN is found and return a #GrrPlmnListInd
 * to (G)MM. If the PLMN is not found, or a PLMN is not included, (G)RR will search for all visible
 * PLMNs and will return the list to (G)MM in a #GrrPlmnListCnf. A GrrPlmnListCnf will also be returned
 * if the PLMN scan fails or is aborted for some reason.
 */
typedef struct GrrPlmnListReqTag
{
    /** If set to FALSE then a list of all available PLMNs should be returned
\assoc PRESENT \ref plmn */
    Boolean  validPlmn;

    /** Specifies a PLMN to search for */
    Plmn     plmn;

    /** Indicates which band to use */
    BandMode bandMode;

    /** Indicates the type of search, for instance this parameter can specify to
     * search whole band for PLMNs or to search bandMode given in PlmnListReq
     */
    Int16           plmnSrchCtrl;
#if defined (UPGRADE_3G)
    /** Indicates if L1 needs to SWITCH_RAT or ACTIVATE_RAT to GSM */
    RatActivateMode ratActivateMode;

    /** Specifies if the PLMN search is for GSM, FDD or TDD PLMNs */
    NetworkModes    networkModeToSearch;

    /** Specifies if the handset Modes */
    NetworkModes    handsetModes;

    /** Specifies the Uarfcn list to search first */
    UarfcnList      uArfcnSrchList;
#if defined (UPGRADE_3G_TDD128)
    /** Indicates which TDD128 band to use */
    Tdd128BandMode  tdd128BandMode;
#endif /* UPGRADE_3G_TDD128 */
#if defined (UPGRADE_3G_FDD) && defined (UPGRADE_FDD_MULTIBAND)
    /** Indicates which FDD band to use */
    FddBandMode     fddBandMode;
#endif /* (UPGRADE_3G_FDD) && (UPGRADE_FDD_MULTIBAND) */
#endif

    /** BCCH allocation list from NVRAM or 3G SIM */
    BaList    baList;

    /** BCCH allocation list from 2G SIM */
    Si2BaList si2BaList;
#if defined (PRODUCT_CALIBRATION_TEST_SWITCH)
    Boolean lockCellEnable;
#endif /* PRODUCT_CALIBRATION_TEST_SWITCH */
}
GrrPlmnListReq;

/** This signal is sent to (G)MM in response to a GrrPlmnListReq if it finds the requested PLMN.*/
typedef struct GrrPlmnListIndTag
{
    /** Requested PLMN */
    PlmnList list;
}
GrrPlmnListInd;

/** If (G)RR receives this signal from (G)MM, it will release the data link layer
 * connection and enter the idle state. This signal should be used if (G)MM detects
 * a protocol violation (e.g. guard timer expiry). It is also used to make sure (G)RR
 * is in the Idle state before sending a GrrEstReq.
 */
typedef struct GrrAbortReqTag
{
    /** The reason for aborting, for insatnce due to authentication failure */
    GsmCause cause;

    /** Timer duration.
     * This field defines the duration, in kernel ticks, for which the timer
     * is required to run. The GKI macros #SECONDS_TO_TICKS() and
     * #MILLISECONDS_TO_TICKS() convert between seconds or milliseconds and
     * number of kernel ticks. The macros will round up to the nearest tick.
     * The smallest unit of time is one system tick. For a system based on the
     * GSM frame rate this is 4.615ms (the GSM TDMA frame period).
     *
     * If the cause is CAUSE_MM_AUTH_FAILURE, then this field indicates
     * the duration that current serving cell stays in the barred list
     */
    FrameTicks timeoutPeriod;
}
GrrAbortReq;

/** This signal is sent by (G)RR directly to the Application Layer on entry into idle
 * mode on a cell if requested by GrrTestModeReq. It provides information on the serving
 * cell configuration for engineering purposes.
 */
typedef struct GrrTestIdleSCellIndTag
{   /* RR idle Serving cell data */
    /** The channel number of the serving cell BCCH carrier */
    Arfcn c0Arfcn;

    /** The timeslot of the CCCH */
    Int8  timeslot;

    /** Base transceiver Station Identity Code of the serving cell; this is a code in GSM which is
     * broadcast on the SCH (Synchronization Channel) in order to identify the NCC
     *(Network Colour Code) and the BCC (Base Station Colour Code)
     */
    Bsic   bsic;

    /** The Location Area ID of the serving cell */
    Lai    lai;

    /** The Cell ID of the serving cell */
    CellId cellId;

    /** The maximum number of permitted retransmissions of RACH bursts
     * during an access derived from the system information broadcast
     * by the network
     */
    Int16 maxRetrans;                        /* translated by decode */

    /** The Cell Bar access cell reselection parameter derived from the
     * system information broadcast by the network
     */
    Boolean cellBarAccess;

    /** Indicates whether the broadcast system information permits
     * re-establishment for circuit switched calls
     */
    Boolean     reestabNotAllowed;

    /** The access class of the serving cell from system information */
    AccessClass accessClass;

    /** The number of block on the BCCH carrier reserved for access grant
     * (derived from system information)
     */
    Int8              numAccessGrantBlocks;

    /** The configuration of the CCCH channel */
    CcchConfiguration ccchConf;

    /** Paging repeat rate in multiframes ( The
     * number of multiframes between pages and it is derived from system information)
     */
    Int8 bsPaMfrms;                                 /* raw value + 2 */

    /** Whether to include sCell in measurements.
     * If TRUE C0 not included in serving-cell measurements.
     */
    Boolean pwrc;

    /** Derived from system information and indicates whether the mobile
     * should use DTX on the uplink
     */
    Boolean applyDtx;

    /** The downlink signalling failure initial value derived from system information */
    Int8    dsc;

    /** Indicates the Additional Hysteresis which applies in Ready state for cells in same RA;
     * it ranges from 0 to 14 dB in steps of 2 dB.
     * A Cell Reselect Hysteresis is necessary when the mobile is transported along the border
     * of two cells.*/
    Int16   cellReselectHysteresis;

    /** The maximum allowed power for transmission on CCH in the service cell */
    Int8    msTxpwrMaxCch;

    /** Minimum access level for serving cell */
    Int8    rxlevAccessMin;

    /** Indicates if C2 parameter is present
\assoc PRESENT \ref cellBarQualify
\assoc PRESENT \ref reselectOffset
\assoc PRESENT \ref temporaryOffset
\assoc PRESENT \ref penaltyTime */
    Boolean c2ParamPresent;

    /** If enabled, then cell selection priority will be low, but cell reselection status
     * (barred/normal)will be normal
     */
    Boolean cellBarQualify;

    /** The cell reselection offset.
     * The cell reselection offset is relatively static, mandatory parameter, which can be positive or
     * negative or zero. It is used to adjust the measurement result of the cell for network
     * planning purposes.
     */
    SignedInt16 reselectOffset;

    /** Used to obtain adjusted signal quality measurements of the candidate cell */
    Int16       temporaryOffset;

    /** When the MS places the cell on the list of the strongest carriers, it starts a timer,
     * which expires after the Penalty_Time. This timer will be reset when the cell is taken
     * off the list
     */
    Int16               penaltyTime;

    /* channel config and control info for CBCH */
    /** Indicates whether the CBCH channel is supported on the serving cell
\assoc PRESENT \ref cbch */
    Boolean             cbchSupported;

    /** Contains the channel configuration for the CBCH channel */
    ChannelDescrElement cbch;

    /** The paging ID being used by (G)RR */
    IdType              pagingId;

    /** The paging group being used by (G)RR */
    Int8                pagingGroup;

    /** Indicates whether system information 2 ter is being broadcast */
    Boolean             si2TerAvailable;

    /** Indicates whether extended BCCH is being used */
    Boolean             extBcchRequired;

    /** Indicates whether early classmark sending is required by the network */
    Boolean             ecsControl;

    /** Indicates the band of the current serving cell */
    Band                band;

    /** Multiband reporting derived from system information  */
    Int8                multiBandNcellReports;

    /** Maximum value of DSF */
    Int8                dsfInitValue;

    /** TRUE if MSC is R99+, FALSE if R98- */
    Boolean             mscR99OrLater;

    /** TRUE if GPRS available on cell */
    Boolean             gprsSupported;

    /** TRUE if SGSN is R99+, FALSE if R98- */
    Boolean             sgsnR99OrLater;

    /** TRUE if EDGE available on cell */
    Boolean             egprsSupported;

    /** TRUE if default packet control ack format is RLC/MAC control
     * block, FALSE if 4 access bursts
     */
    Boolean ackIsRlcFormat;

    /** TRUE if 8-bit access burst format used, FALSE if 11-bit format used */
    Boolean accessBurstIsEightBit;

    /** TRUE if MS is in GPRS READY state */
    Boolean inReadyState;

    /** TRUE if PBCCH present */
    Boolean pbcchActive;

    /** Network Mode of Operation (1, 2 or 3) */
    Int8    gprsNetworkMode;

    /** Network Control Order (0, 1 or 2); see \link NetworkControl \endlink */
    Int8    nc;
#if defined (UPGRADE_ADDIT_ENG_INFO)
    /** Whether the PBCCH is allocated as a hopping or non-hopping PBCCH*/
    Boolean pbcchHopping;

    /** PBCCH ARFCN */
    Arfcn   pbcchArfcn;
#endif

    /** PBCCH timeslot */
    Int8    pbcchTimeslot;

    /** PCCCH timeslot */
    Int8    pccchTimeslot;

    /** PSI1 repeat period (0-15, see 04.18 for format).
     * The field is coded according to the following table:
     * <TABLE>
     *  <TR><TD>Bits Band</TD><TD> Definition           </TD></TR>
     *  <TR><TD> 0000 </TD><TD> PSI1 repeat period = 1 multiframe </TD></TR>
     *  <TR><TD> 0001 </TD><TD> PSI1 repeat period = 2 multiframe </TD></TR>
     *  <TR><TD> 0002 </TD><TD> PSI1 repeat period = 3 multiframe </TD></TR>
     *  <TR><TD> 0003 </TD><TD> PSI1 repeat period = 4 multiframe </TD></TR>
     *  <TR><TD> 0004 </TD><TD> PSI1 repeat period = 5 multiframe </TD></TR>
     *  <TR><TD> 0005 </TD><TD> PSI1 repeat period = 6 multiframe </TD></TR>
     *  <TR><TD> 0006 </TD><TD> PSI1 repeat period = 7 multiframe </TD></TR>
     *  <TR><TD> 0007 </TD><TD> PSI1 repeat period = 8 multiframe </TD></TR>
     *  <TR><TD> 0008 </TD><TD> PSI1 repeat period = 9 multiframe </TD></TR>
     *  <TR><TD> 0009 </TD><TD> PSI1 repeat period = 10 multiframe </TD></TR>
     *  <TR><TD> 0010 </TD><TD> PSI1 repeat period = 11 multiframe </TD></TR>
     *  <TR><TD> 0011 </TD><TD> PSI1 repeat period = 12 multiframe </TD></TR>
     *  <TR><TD> 0012 </TD><TD> PSI1 repeat period = 13 multiframe </TD></TR>
     *  <TR><TD> 0013 </TD><TD> PSI1 repeat period = 14 multiframe </TD></TR>
     *  <TR><TD> 0014 </TD><TD> PSI1 repeat period = 15 multiframe </TD></TR>
     *  <TR><TD> 0015 </TD><TD> PSI1 repeat period = 16 multiframe </TD></TR>
     * </Table>
     */
    Int8  psi1RepeatPeriod;

    /** power reduction value on PBCCH blocks (0-15, see 04.60 for format)*/
    Int8  pb;

    /**  Priority access threshold (0-7, see 04.18 for format) */
    Int8  priorityAccThr;

    /** Timeout value of timer T3192 in TDMA frames */
    Int32 t3192InFrames;
}
GrrTestIdleSCellInd;

/** This signal is sent by (G)RR directly to the Application Layer on entry into
 * dedicated mode if requested by GrrTestModeReq. It provides information on the
 * configuration of the traffic channel for engineering purposes.
 */
typedef struct GrrTestDedSCellIndTag
{   /* Dedicated channel engineering info. */
    /** The channel number of the BCCH carrier of the serving cell */
    Arfcn c0Arfcn;

    /** Base transceiver Station Identity Code of the serving cell; this is a code in GSM which is
     * broadcast on the SCH (Synchronization Channel) in order to identify the NCC
     *(Network Colour Code) and the BCC (Base Station Colour Code)
     */
    Bsic                bsic;

    /** Contains the channel description of the dedicated channel */
    ChannelDescrElement chanDesc;

    /** The mode of the assigned channel */
    ChannelMode         mode;
}
GrrTestDedSCellInd;

/** Provides information on the serving cell and neighbour cells while in idle mode */
typedef struct GrrTestIdleNcellInfoTag
{
    /** The neighbour cell BCCH channel number */
    Arfcn   arfcn;

    /** The current level of the neighbour cell */
    Int8    level;

    /** The BSIC of the neighbour cell */
    Bsic    bsic;

    /** Whether or not BCCH is valid */
    Boolean bcchValid;

    /** Cell reselection C1 for the neighbour cell;
     * C1 is a cell selection algorithm employed in GSM and GPRS;
     * The algorithm uses the power received from cells plus additional
     * parameters in order to assess the cell that will provide the best
     * radio connection for the MS
     */
    SignedInt16 c1;

    /** GSM cell reselection C2 for the neighbour cell. Once the MS (Mobile Station)
     * has camped onto a cell it will continue to assess the surrounding cells to
     * ensure it is monitoring the cell that will offer the best radio connection.
     * As the user moves the camped on cell may become unsuitable. This situation
     * will generate a cell reselection.
     */
    SignedInt16 c2;

    /** Cell reselection C31 for the neighbour cell.
     * C31 is a criteria based algorithm used as part of the GPRS cell reselection process.
     * It is used to assess if prospective cells qualify as reselection candidates prior
     * to applying the C32 GPRS cell reselection algorithm.
     */
    SignedInt16 c31;

    /** Cell reselection C32 for the neighbour cell.
    * C32 is the GPRS cell reselection algorithm. Once the MS (Mobile Station) has camped
    * onto a cell it will continue to assess the surrounding cells to ensure it is
    * monitoring the cell that will offer the best radio connection. As the user moves
    * the camped on cell may become unsuitable. This situation will generate a cell reselection.
     */
    SignedInt16 c32;
#if defined (UPGRADE_ADDIT_ENG_INFO)
    /** The Mobile Country Code is a three digit number uniquely identifying a given country */
    Mcc         mcc;

    /** The Mobile Network Code is either a two or three digit number used to uniquely identify
     * a given network from within a specified country (MCC)
     */
    Mnc    mnc;

    /** Cell ID for the neighbour cell */
    CellId cellId;
#endif

    /** The Location Area Code for the neighbour cell; The Location Area Code uniquely identifies
     * a Location Area (LA) within a PLMN
     */
    Lac lac;
}
GrrTestIdleNcellInfo;

#if defined (UPGRADE_3G) && defined (UPGRADE_GGE)

#if defined (UPGRADE_3G_FDD)

/** Provides FDD Neighbour cell information  */
typedef struct GrrTestIdleFddNcellInfoTag
{
    /** Frequency for neighbour cell */
    Int16       uArfcn;

    /** Primary scrambling code for neighbour cell */
    Int16       psc;

    /** Given only signal power is received, the average power of the received signal
     * from the neighbour cell after despreading and combining*/
    SignedInt16 rscp;

    /** Ec/No for neighbour cell, in units of 1/8 dB*/
    SignedInt16 ecno;
}
GrrTestIdleFddNcellInfo;
#endif  /* UPGRADE_3G_FDD */

#if defined (UPGRADE_3G_TDD128)

/** Provides TDD128 Neighbour cell information  */
typedef struct GrrTestIdleTdd128NcellInfoTag
{
    /** Frequency for neighbour cell */
    Int16       uArfcn;

    /** Cell Parameters Id for neighbour cell
     * Contain the sctdIndicator and tstdIndicator information */
    Int16       cpi;

    /** Given only signal power is received, the average power of the received signal
     * from the neighbour cell after despreading and combining */
    SignedInt16 rscp;
}
GrrTestIdleTdd128NcellInfo;
#endif /* UPGRADE_3G_TDD128 */

#endif

/** This signal is sent by (G)RR directly to the Application Layer every 5 seconds
 * while in idle mode if requested by GrrTestModeReq. It provides information on
 * the serving cell and neighbour cells for engineering purposes.
 */
typedef struct GrrTestIdleInfoIndTag
{   /* GRR idle ncell data */
    /** Indicates whether the software is in packet idle mode or CS idle mode */
    Boolean isInPacketIdle;

    /** Is the channel number of the serving cell BCCH carrier */
    Arfcn   sCellArfcn;

    /** Is the level of the serving cell BCCH carrier */
    Int8    sCellLevel;

    /** Cell reselection C1 for the serving cell;
     * C1 is a cell selection algorithm employed in GSM and GPRS;
     * The algorithm uses the power received from cells plus additional
     * parameters in order to assess the cell that will provide the best
     * radio connection for the MS
     */
    SignedInt16 sCellC1;

    /**Cell reselection C2 for the serving cell . Once the MS (Mobile Station)
     * has camped onto a cell it will continue to assess the surrounding cells to
     * ensure it is monitoring the cell that will offer the best radio connection.
     * As the user moves the camped on cell may become unsuitable. This situation
     * will generate a cell reselection.
     */
    SignedInt16 sCellC2;

    /** The cell reselection C31 for the serving cell
     * (only relevant if isInPacketIdle is TRUE)
     */
    SignedInt16 sCellC31;

    /** The cell reselection C32 for the serving cell
     * (only relevant if isInPacketIdle is TRUE)
     */
    SignedInt16          sCellC32;

    /** Is the number of items contained in the nCellInfo array */
    Int8                 top6Size;

    /** Bit map of 32-bit indicating which cell is blocked */
    Int32                gsmBlockingActive;

    /** Information relevant to neighbour cell */
    GrrTestIdleNcellInfo ncellInfo[NUM_NCELLS];

    /** Network control (NC) parameters */
    NcMeasurementParam   ncParam;
#if defined (UPGRADE_3G) && defined (UPGRADE_GGE)
    /** Qsearch_I defines a threshold and also indicates whether these
     * measurements shall be performed when RLA_C of the serving cell
     * is below or above the threshold
     */
    Int8 qSearchI;

    /** Qsearch_C defines a threshold and also indicates whether these
     * tasks shall be performed when RXLEV of the serving BCCH carrier
     * is below or above the threshold.
     * Search for 3G cells if signal level below threshold (0-7)
     * or above threshold (8-15).
     */
    Int8 qSearchC;

    /** Qsearch_P defines a threshold and also indicates whether these measurements
     * shall be performed when RLA_P of the serving cell is below or above the threshold
     */
    Int8  qSearchP;

    /** The number of frequencies in reselection list */
    Int8  numUarfcnInReselList;                   /* Shared by FDD and TDD128 */
    /** List of reselected frequencies */
    Int16 uArfcn[NUM_UMTS_NCELL_INFO];                   /* Shared by FDD and TDD128 */

#if defined (UPGRADE_3G_FDD)
    /** Specifies the offset between the two cells.
     * Applies an offset to RLA_C for cell re selection to access technology/mode FDD, where: <UL>
     * <LI> 0 = (-infinity) always select a cell if acceptable, 1 = -28 dB, 2 = -24 dB,..., 15 = 28 dB.
     * </UL>
     */
    Int8 fddQOffset;

    /** Minimum required quality level in the cell.
     * A minimum threshold for Ec/No for UTRAN FDD cell re-selection, where: <UL>
     * <LI> 0= -20dB , 1= -6dB, 2= -18dB, 3= -8dB, 4= -16dB, 5= -10dB, 6= -14dB, 7= -12dB.
     *</UL>
     */
    Int8 fddQMin;

    /* Release 5 additions .... */

    /** Offset to the signalled fddQMin (0 = 0 dB, 1 = 2 dB, 2 = 4 dB, 3 = 6 dB,
     * 4 = 8 dB, 5 = 10 dB, 6 = 12 dB, 7 = 14 dB)
     */
    Int8 fddQMinOffset;

    /** A minimum threshold of RSCP for UTRAN FDD cell re-selection
     * (0 = -114 dBm, 1 = -112 dBm, 2 = -110 dBm, 3 = -108 dBm, 4 = -106 dBm,
     * 5 = -104 dBm, 6 = -102 dBm, 7 = -100 dBm, 8 = -98 dBm, 9 = -96 dBm,
     * 10 = -94 dBm, 11 = -92 dBm, 12 = -90 dBm, 13 = -88 dBm, 14 = -86 dBm, 15 = -84 dBm,
     * Default value = -102 dBm)
     */
    Int8 fddRscpMin;

    /* Measurement Reporting parameters */

    /** Apply an offset to the reported value when prioritising the cells for reporting for
     * GSM frequency band or access technology/mode FDD
     */
    Int8 fddReportOffset;

    /** Apply priority reporting if the reported value is above threshold for GSM frequency
    * band or access technology/mode FDD
    */
    Int8                    fddReportThreshold;
    Int8                    fddReportThreshold2;

    /** FDD EcN0 threshold */
    SignedInt16             fddEcnoThreshold;

    /** FDD RSCP threshold */
    SignedInt16             fddRscpThreshold;

    /** The FDD band indication for FDD POWER CLASS4 */
    Int16                   class4Indicator;

    /** TRUE means inclusion RSCP in the report is requested */
    Boolean                 reportRscp;

    /** List of blocking FDD cells; where each entry is a
     * bit map of 32-bit indicating which cell is blocked */
    Int32                   fddBlockingActive[3];

    /** The number of cells in the list where the quality measure for cell
     * selection and re-selection is set to CPICH RSCP */
    Int8                    numRscpCells;

    /** The number of cells in the list where the quality measure for cell
     * selection and re-selection is set to CPICH Ec/No */
    Int8                    numEcnoCells;

    /** Holds top six cells selected using CPICH RSCP quality measurement */
    GrrTestIdleFddNcellInfo rscpTopSix[NUM_NCELLS];

    /** Holds top six cells selected using CPICH Ec/No quality measurement */
    GrrTestIdleFddNcellInfo ecnoTopSix[NUM_NCELLS];
#endif  /* UPGRADE_3G_FDD */
#if defined (UPGRADE_3G_TDD128)
    /** An absolute threshold of RSCP for UTRAN TDD cell re-selection, where: <UL>
     * <LI> 0 = -105dBm, 1 = -102dBm, 2=-99dBm, 3 = -96dBm, ..., 15 = -60dBm.
     * Default value = -90dBm.
     * </UL>
     */
    Int8 tdd128QOffset;

    /* Measurement Reporting parameters */

    /** Apply an offset to the reported value when prioritising the cells for reporting for
     * GSM frequency band or access technology/mode TDD
     */
    Int8  tdd128ReportOffset;

    /** TDD RSCP threshold */
    Int8  tdd128ReportThreshold;

    /** List of blocking TDD128 cells; where each entry is a
     * bit map of 32-bit indicating which cell is blocked */
    Int32 tdd128BlockingActive[3];

    /** The number of cells in the Top Six list */
    Int8  numTdd128RscpCells;

    /** Holds top six cells */
    GrrTestIdleTdd128NcellInfo tdd128RscpTopSix[NUM_NCELLS];
#endif /* UPGRADE_3G_TDD128 */
#endif /* UPGRADE_3G && UPGRADE_GGE */
}
GrrTestIdleInfoInd;

/** This signal is sent by (G)RR directly to the Application Layer every 5 seconds
 * while in dedicated mode if requested by GrrTestModeReq. It provides information
 * on the serving cell and neighbour cells during dedicated mode for engineering purposes.
 */
typedef struct GrrTestDedInfoIndTag
{   /* GRR ded. ncell data */
    /** The measurement information for the dedicated channel */
    ServingCellMeas scMeas;

    /** The number of items in the following arrays
\assoc ARRAY \ref nCellArfcn
\assoc ARRAY \ref nCellLevel
\assoc ARRAY \ref nCellBsic */
    Int8            numMeas;

    /** The current setting of the BA ind flag */
    Int8            baUsed;

    /** The channel numbers of the neighbour cells */
    Arfcn           nCellArfcn[NUM_NCELLS];

    /** The level of the neighbour cells */
    Int8            nCellLevel[NUM_NCELLS];

    /** The BSIC of the neighbour cells; if set to 0xFF there is no
     * synchronisation information for this neighbour cell
     */
    Int8 nCellBsic[NUM_NCELLS];

    /** MCC of the serving cell
     * (The Mobile Country Code (MCC)is a three digit number uniquely identifying a given country)
     */
    Int16 mcc;

    /** MNC of the serving cell
     *  (The Mobile Network Code is either a two or three digit number used to uniquely identify
     * a given network from within a specified country (MCC))
     */
    Int16 mnc;

    /** The Location Area Code for the serving cell; The Location Area Code uniquely identifies
     * a Location Area (LA) within a PLMN
     */
    Int16 lac;

    /** Cell ID of the serving cell */
    Int16 cellId;

    /** Whether or not NCC is permitted */
    Int8  nccPermitted;

    /** Whether to include sCell in measurements.
     * If TRUE C0 not included in serving-cell measurements.
     */
    Boolean pwrc;

    /** Whether or not DTX is permitted */
    Boolean dtxAllowed;

    /** Radio link timeout value 0, 4, 8 upto 64 */
    Int8    radioLinkTimeout;

    /** Handover counter */
    Int16   hoCount;

    /** Handover success conuter */
    Int16   hoSuccessCount;

    /** Channel assignment counter */
    Int16   chanAssCount;

    /** Channel assignment success counter */
    Int16   chanAssSuccessCount;
}
GrrTestDedInfoInd;

/** Circuit Switched Connection establishment request.
 * This signal is sent by (G)MM to establish a mobile originated circuit switched connection.
 * (G)RR will carry out the immediate assignment procedure. (G)MM must indicate the reason for
 * the request in order for (G)RR to send the correct channel request message.
 */
typedef struct L3EstDataRequestTag
GrrEstReq,

/** Send data request.
 * This signal is used by (G)MM to request (G)RR to send a Layer 3 message on a circuit-switched
 * connection to its peer entity.
 */
GrrDataReq;

/** RR connection release indication.
 * Sent from (G)RR to (G)MM to indicate that the RR connection has been terminated as requested
 * by the peer (G)RR entity.
 */
typedef struct GrrRelIndTag
{
    /** Indicates the cause for connection release */
    GsmCause cause;

    /** Set to a non-zero value when (G)MM is expected to wait this number of seconds before
     * retrying to establish. In the event of a cell reselection to a new cell (G)MM can stop
     * this timer and start the establishment.
     */
    Int8 waitIndication;

    /** Indicates that, after the RR connection for circuit switched operation has terminated,
     * the MS can continue with GPRS operation.
     */
    Boolean gprsResumptionAcknowledged;
}
GrrRelInd;

/** This signal is used for synchronizing (G)RR and the (G)MM entities.
 * This signal is sent to (G)MM whenever (G)RR carries out one of the following:<UL>
 * <LI> Set ciphering mode: in this case the additional parameters will indicate:<UL>
 * <LI> cipheringInUse whether ciphering is turned on or off
 * <LI> algorithm  which cipher algorithm is currently selected if ciphering is on.</UL>
 * <LI> Channel assignment
 * <LI> Channel mode modify
 * <LI> Handover
 *</UL>
 */
typedef struct GrrSyncIndTag
{
    /** The cause that triggers sending this signal */
    SyncCause cause;

    /** Updated current band information */
    Band      band;

    /** Indicates if ciphering is enabled or disabled; valid only if the cause is set
     * to SYNC_CIPHERING_MODE_SET
     */
    Boolean         cipheringInUse;

    /** Ciphering algorithm to be applied; valid only if cipheringInUse is set to TRUE */
    CipherAlgorithm algorithm;
}
GrrSyncInd;

/** Deactivate request.
 * If (G)RR receives this signal it will deactivate the physical layer and then enter
 * the NULL state.
 */
typedef struct GrrDeactReqTag
{
    /** Indicates the cause of deactivation*/
    AccStratumDeactCause deactivateCause;
}
GrrDeactReq;

#if defined (UPGRADE_3G) && defined (UPGRADE_GGE)

/** Select either GSM, FDD or TDD128 Mode.
 *  This signal is sent from MM to GRR to select the active RAT.
 */

typedef struct GrrModeSelectReqTag
{
    /** Mode to activate. Always NMODE_GSM on the 2GAS */
    NetworkModes networkMode;

    /** Indicate the RATs the UE will operate over (includes networkMode). */
    NetworkModes handsetModes;
}
GrrModeSelectReq;

/** Confirmation that mode is selected.
 *  This signal is sent from GRR to MM in response to a #GrrModeSelectReq.
 */
typedef EmptySignal GrrModeSelectCnf;

/** Contains the Asn1 encoded UInterRATHandoverInfo sent in a UtranClassmarkChangeMsg.
 *   This structure is logged as a SIG_GRR_DEBUG_INTER_RAT_HO_INFO_TX_IND (if enabled in genie filter)
 */
typedef struct GrrDebugInterRatHoTxIndTag
{
    /** Defines length of data in bytes. */
    Int16 byteLength;

    /** Contains the ASN1 encoded UInterRATHandoverInfo */
    Int8  data [MAX_GRR_INTERFACE_DEBUG_OCTETS];
}
GrrDebugInterRatHoTxInd;

#endif /* UPGRADE_3G && UPGRADE_GGE */

/** This signal is used for all NAS signals sent on a dedicated CS channel (SDCCH, FACCH).
 * This signal is sent to (G)MM whenever (G)RR receives a layer 3 message on a
 * circuit-switched connection whose protocol discriminator does not indicate RR.
 * (G)MM will either process the message or pass it on to the relevant CM entity.
 */
typedef struct GrrDataIndTag
{
    /** Contains Layer3 message.The system information is received on the current BCCH if RR is in the IDLE state.
     * If a RR connection has been established, the system info is received on the SACCH.
     */
    L3Message l3Message;
}
GrrDataInd;

/** This signal may be sent by (G)RR to itself for diagnostic purposes.
 * This signal is used for all Access Stratum signals (e.g. GRR) sent on both
 * common control and dedicated channels.
 * It contains a decoded
 * layer 3 message whose protocol discriminator indicates RR and is used to give visibility of
 * the message contents.
 */
typedef struct GrrUnitDataIndTag
{
    /** Holds Access Stratum signals */
    AccessStratumMessage asMessage;
}
GrrUnitDataInd;

/** This signal is sent by (G)RR to (G)MM to indicate a change of cell. Note that if the cell crosses
 * a Location Area or Routing Area boundary then a GrrActInd is sent instead.
 */
typedef struct GrrCellUpdateIndTag
{
    /** Identifies the new cell */
    CellId              cellId;

    /** System information relate to the new cell */
    MmSystemInformation sysInfo;

    /** Service provided by the new cell */
    ServiceType         serviceType;
}
GrrCellUpdateInd;

/** This signal is sent to (G)MM when (G)RR receives a page via the GPRS stack */
typedef struct GrrPageIndTag
{
    /** Paging ID */
    IdType idType;
}
GrrPageInd;

/** Ready state request.
 * Used to inform (G)RR of the ready state as (G)RR needs to apply hysteresis to
 * cell reselection in the ready state.
 */
typedef struct GrrGprsReadyStateReqTag
{
    /** The status of GPRS (Idle, Standby or Ready) */
    GprsReadyState readyState;
}
GrrGprsReadyStateReq;

/** This signal may be sent by (G)RR for diagnostic purposes */
typedef struct GrrTstIdleSiBufferIndTag
{
    /** SI buffer operation */
    SiBufferOperation op;

    /** BCCH system information buffer number */
    Int8              bufferNumber;

    /** Indicates the status of SI buffer (valid/or not ) */
    SiBufferStatus    status[MAX_NUM_NCELL_SYS_INFO];

    /** Holds the list of frequencies */
    Arfcn             arfcn[MAX_NUM_NCELL_SYS_INFO];

    /** Holds the list of BSIC */
    Bsic              bsic[MAX_NUM_NCELL_SYS_INFO];

    /** Index to the neighbour cells in the current BA list */
    Int8              baIndex[MAX_NUM_NCELL_SYS_INFO];

    /** Specifies the type of buffer used to hold SI */
    SiBufferType      bufferType;

#if defined (ENABLE_GRR_INTERNAL_DIAG)
    union
    {
        /** Not used in any source code */
        Int16 noBcch;

        /** The system information of the serving cell */
        BcchBufferInfo bcch;

        /** The system information of the neighbour cell */
        NcellBcchBufferInfo ncellBcch;

        /** Reselection parameters */
        GrrNcellReselStoreInfo reselParam;

        /** Frequency parameters */
        GrrFreqParamStoreInfo freqParam;

        /** frequency list parameters */
        GrrRflStoreInfo rflParam;

        /** BSIC information */
        BsicDescriptionInfo bsicDescrInfo;
#if defined (UPGRADE_3G_FDD)
        /** FDD neighbour cell description information */
        FddNcellDescriptionInfo fddNcellDescriptionInfo;
#endif /* UPGRADE_3G_FDD */
#if defined (UPGRADE_3G_TDD128)
        /** TDD128 neighbour cell description information */
        Tdd128NcellDescriptionInfo tdd128NcellDescriptionInfo;
#endif /* UPGRADE_3G_TDD128 */
    }
    buffer;
#endif
}
GrrTstIdleSiBufferInd;

/** This signal may be sent by (G)RR for diagnostic purposes.*/
typedef struct GrrTstIdleReselIndTag
{
    /** Indicates the cause for re-selection */
    Int16 cause;

    /** The number of reselected GSM cells */
    Int16 numGsmReselectCells;

    /** Holds the list of frequencies */
    Arfcn arfcn[6];
}
GrrTstIdleReselInd;

/** This signal is sent to (G)RR when a measurement report is sent */
typedef struct GrrTstDedBsicMonIndTag
{
    /** Holds the top six neighbour cells */
    GrrNcellTopSix            topSix;

    /** Neighbour cell reports for other band */
    Int8                      multiBandNcellReports;

    /** Indicates the Arfcn information */
    Arfcn                     arfcnInReport[NUM_MEAS_RECORDS];

    /** Indicates the measurement results */
    MeasurementResultsElement meas;
#if defined (ENABLE_GRR_INTERNAL_DIAG)
    /** Neighbour cell BSIC requests */
    GrrNcellBsicReq           bsicReq;

    /** Holds the top six neighbour cells */
    GrrNcellTopSix            candidateTopSix;

    /** Neighbour cell monitering operation */
    GrrNcellBsicMonOp         ncellMonOp;

    /** New Ncell Index */
    Int8                      newNcellIndex;

    /** The best neighbour cell */
    Arfcn                     bestNcell;

    /** Bit map of 32-bit indicating which cell is blocked */
    Int32                     gsmBlockingActive;

    /** The number of entries in the sorted level list */
    Int16                     sortedListLength;

    /** The channel numbers of the neighbour cells */
    Arfcn                     nCellArfcn[NUM_DCH_NCELL_DEBUG_CELLS];

    /** Ncell BSIC valid timer */
    FrameTicks                bsicTime[NUM_DCH_NCELL_DEBUG_CELLS];

    /** Absolute current time difference */
    FrameTicks                absTimeDiff[NUM_DCH_NCELL_DEBUG_CELLS];

    /** Recieved signal strengths for measured cells */
    Int8                      nCellLevel[NUM_DCH_NCELL_DEBUG_CELLS];

    /** BSIC of the neighbour cells */
    Int8                      nCellBsic[NUM_DCH_NCELL_DEBUG_CELLS];
#endif
}
GrrTstDedBsicMonInd;

/** The list of cells reported using enhanced measurement report */
typedef struct GrrTstEnhCellListTag
{
    /** The number of cells in the ennhanced ncell list */
    Int8 numEntries;

    /** Index of the cell in the list */
    Int8 index[12];
}
GrrTstEnhCellList;

#if defined (UPGRADE_3G_FDD)

/** Provides information on the top FDD neighbour cells */
typedef struct GrrTstFddNcellTopSixTag
{
    /** The number of valid entries */
    Int8  numEntries;

    /** Absolute index of the neighbur cell in the BA list */
    Int8  absIndex[6];

    /** Format is :- UU SSSSSS where U == uArfcn index, S == scrambling code index */
    Int16 index[6];
}
GrrTstFddNcellTopSix;
#endif /* UPGRADE_3G_FDD */

#if defined (UPGRADE_3G_TDD128)
typedef struct GrrTstTdd128NcellTopSixTag
{
    /** The number of valid entries */
    Int8  numEntries;

    /** Absolute index of the neighbur cell in the BA list */
    Int8  absIndex[6];

    /** Format is :- UU SSSSSS where U == uArfcn index, S == Cell parameters Id index */
    Int16 index[6];
}
GrrTstTdd128NcellTopSix;
#endif /* UPGRADE_3G_TDD128 */

/** Contain measurement results about reception characteristics from the current
 * cell and from neighbour cells
 */
typedef struct GrrTstEnhReportTag
{
#if defined (UPGRADE_3G_FDD)
    /** Set to TRUE if dual mode is defined and quality measurement is based on CPICH RSCP ;
     * otherwise set to FALSE */
    Boolean                 reportRscp;

    /** The six best valid FDD neighboring cells */
    GrrTstFddNcellTopSix    fddTopSix;

    /** FDD report list  */
    GrrTstFddNcellTopSix    fddReportList;
#endif /* UPGRADE_3G_FDD */
       /** Enhanced Ncell List */
    GrrTstEnhCellList       enhNcellList;

    /** Contains the report of cells with invalid BSIC */
    GrrTstEnhCellList       invalidBsicList;
#if defined (UPGRADE_3G_TDD128)
    /** The six best valid TDD128 neighboring cells */
    GrrTstTdd128NcellTopSix tdd128TopSix;

    /** TDD128 report list  */
    GrrTstTdd128NcellTopSix tdd128ReportList;
#endif /* UPGRADE_3G_TDD128 */
}
GrrTstEnhReportInd;

/** This signal is sent to (G)RR whenever the GPRS multislot class changes.
 */
typedef struct GrrGprsMslotChangeReqTag
{
    /** Indicates that MS supports the use of multiple
     * timeslots and it ranges form 1 to 45
     */
    Int8 gprsMultislotClass;
#if defined (UPGRADE_EDGE)
    /** Indicates that MS supports the use of multiple
    * timeslots and it ranges form 1 to 45
    */
    Int8 egprsMultislotClass;
#endif
}
GrrGprsMslotChangeReq;

/** This signal may be sent by (G)RR to itself for diagnostic purposes whenever
 * a GPRS CSN.1 encoded message is received.
 *
 * Note: L3PktDnlinkMsg is a union of the CSN.1 encoded downlink messages.
 */
typedef struct GrrPktMsgDecodeIndTag
{
    /** Frame number on which encoded message is received */
    Int32               frameNumber;

    /** Determines the outcome of decoding the message */
    DecodeResult        decodeResult;

    /** Specifies the release cause */
    RlcmacReleaseCause  processResult;

    /** Indicates the message type
\assoc UNION \ref l3Msg */
    DownlinkMessageType msgType;

    /** Packet system information or Packet measurement order count */
    Int8                count;

    /** Packet system information or Packet measurement order index */
    Int8                index;

    /** Holds the received encoded message */
    L3PktDnlinkMsg      l3Msg;
}
GrrPktMsgDecodeInd;

/** Information relevant to neighbour cell */
typedef struct GrrTestReselNcellInfoTag
{
    /** Whether or not the information is valid */
    Boolean valid;

    /** Is the channel number of the neighbour cell */
    Arfcn   arfcn;

    /** Is the level of the neighbour cell  */
    Int8    level;

    /** Base Station Identity Code of the neighbour cell*/
    Bsic    bsic;

    /** Cell reselection C1 for the neighbour cell;
     * C1 is a cell selection algorithm employed in GSM and GPRS;
     * The algorithm uses the power received from cells plus additional
     * parameters in order to assess the cell that will provide the best
     * radio connection for the MS
     */
    SignedInt16 c1;

    /** GSM cell reselection C2 for the neighbour cell. Once the MS (Mobile Station)
     * has camped onto a cell it will continue to assess the surrounding cells to
     * ensure it is monitoring the cell that will offer the best radio connection.
     * As the user moves the camped on cell may become unsuitable. This situation
     * will generate a cell reselection.
     */
    SignedInt16 c2;

    /** The cell reselection C31 for the neighbour cell
     * (only relevant if isInPacketIdle is TRUE)
     */
    SignedInt16 c31;

    /** The cell reselection C32 for the neighbour cell
     * (only relevant if isInPacketIdle is TRUE)
     */
    SignedInt16 c32;

    /** Adjusted C32 for the neighbour cell */
    SignedInt16 adjustedC32;

    /** The Location Area Code for the neighbour cell; The Location Area Code uniquely identifies
     * a Location Area (LA) within a PLMN
     */
    Lac    lac;
#if defined (UPGRADE_ADDIT_ENG_INFO)
    /** ID of the neighbour cell */
    CellId cellId;
#endif
}
GrrTestReselNcellInfo;

/** Reselection information indication.
 * This signal may be sent by (G)RR for diagnostic purposes.
 */
typedef struct GrrTestReselInfoIndTag
{
    /** Is the channel number of the serving cell BCCH carrier */
    Arfcn sCellArfcn;

    /** Is the level of the serving cell BCCH carrier */
    Int8  sCellLevel;                        /* 0 - 63 */

    /** Cell reselection C1 for the neighbour cell;
     * C1 is a cell selection algorithm employed in GSM and GPRS;
     * The algorithm uses the power received from cells plus additional
     * parameters in order to assess the cell that will provide the best
     * radio connection for the MS
     */
    SignedInt16 sCellC1;

    /** GSM cell reselection C2 for the neighbour cell. Once the MS (Mobile Station)
     * has camped onto a cell it will continue to assess the surrounding cells to
     * ensure it is monitoring the cell that will offer the best radio connection.
     * As the user moves the camped on cell may become unsuitable. This situation
     * will generate a cell reselection.
     */
    SignedInt16 sCellC2;

    /** The cell reselection C31 for the serving cell
     * (only relevant if isInPacketIdle is TRUE)
     */
    SignedInt16 sCellC31;

    /** The cell reselection C32 for the serving cell
     * (only relevant if isInPacketIdle is TRUE)
     */
    SignedInt16           sCellC32;

    /** Network control (NC) parameters */
    NcMeasurementParam    ncParam;

    /** Bit map of 32-bit indicating which cell is blocked */
    Int32                 gsmBlockingActive;

    /** Is the number of items contained in the nCellInfo array */
    Int8                  top6Size;

    /** Information relevant to neighbour cell */
    GrrTestReselNcellInfo ncellInfo[MAX_NUM_NCELL_SYS_INFO];
#if defined (UPGRADE_3G) && defined (UPGRADE_GGE)
    /** Qsearch_I defines a threshold and also indicates whether these
     * measurements shall be performed when RLA_C of the serving cell
     * is below or above the threshold
     */
    Int8 qSearchI;

    /** Qsearch_C defines a threshold and also indicates whether these
     * tasks shall be performed when RXLEV of the serving BCCH carrier
     * is below or above the threshold.
     * Search for 3G cells if signal level below threshold (0-7)
     * or above threshold (8-15).
     */
    Int8 qSearchC;

    /** Qsearch_P defines a threshold and also indicates whether these measurements
     * shall be performed when RLA_P of the serving cell is below or above the threshold
     */
    Int8  qSearchP;

    /** The number of frequencies in reselection list */
    Int8  numUarfcnInReselList;                   /* Shared by FDD and TDD128 */
    /** List of reselected frequencies */
    Int16 uArfcn[NUM_UMTS_NCELL_INFO];                   /* Shared by FDD and TDD128 */
#if defined (UPGRADE_3G_FDD)
    /** Specifies the offset between the two cells.
     * Applies an offset to RLA_C for cell re selection to access technology/mode FDD, where: <UL>
     * <LI> 0 = (-infinity) always select a cell if acceptable, 1 = -28 dB, 2 = -24 dB,..., 15 = 28 dB.
     * </UL>
     */
    Int8 fddQOffset;

    /** Minimum required quality level in the cell.
     * A minimum threshold for Ec/No for UTRAN FDD cell re-selection, where: <UL>
     * <LI> 0= -20dB , 1= -6dB, 2= -18dB, 3= -8dB, 4= -16dB, 5= -10dB, 6= -14dB, 7= -12dB.
     *</UL>
     */
    Int8 fddQMin;

    /* Release 5 additions .... */

    /** Offset to the signalled fddQMin (0 = 0 dB, 1 = 2 dB, 2 = 4 dB, 3 = 6 dB,
     * 4 = 8 dB, 5 = 10 dB, 6 = 12 dB, 7 = 14 dB)
     */
    Int8 fddQMinOffset;

    /** A minimum threshold of RSCP for UTRAN FDD cell re-selection
     * (0 = -114 dBm, 1 = -112 dBm, 2 = -110 dBm, 3 = -108 dBm, 4 = -106 dBm,
     * 5 = -104 dBm, 6 = -102 dBm, 7 = -100 dBm, 8 = -98 dBm, 9 = -96 dBm,
     * 10 = -94 dBm, 11 = -92 dBm, 12 = -90 dBm, 13 = -88 dBm, 14 = -86 dBm, 15 = -84 dBm,
     * Default value = -102 dBm)
     */
    Int8                    fddRscpMin;

    /* Measurement Reporting parameters */
    Int8                    fddReportOffset;
    Int8                    fddReportThreshold;
    Int8                    fddReportThreshold2;

    /** FDD EcN0 threshold */
    SignedInt16             fddEcnoThreshold;

    /** FDD RSCP threshold */
    SignedInt16             fddRscpThreshold;

    /** Power class 4 indicator*/
    Int16                   class4Indicator;

    /** TRUE means inclusion RSCP in the report is requested */
    Boolean                 reportRscp;

    /** List of blocking FDD cells; where each entry is a
     * bit map of 32-bit indicating which cell is blocked */
    Int32                   fddBlockingActive[3];

    /** The number of cells in the list where the quality measure for cell
     * selection and re-selection is set to CPICH RSCP */
    Int8                    numRscpCells;

    /** The number of cells in the list where the quality measure for cell
     * selection and re-selection is set to CPICH Ec/No */
    Int8                    numEcnoCells;

    /** Holds top six cells selected using CPICH RSCP quality measurement */
    GrrTestIdleFddNcellInfo rscpTopSix[NUM_NCELLS];

    /** Holds top six cells selected using CPICH Ec/No quality measurement */
    GrrTestIdleFddNcellInfo ecnoTopSix[NUM_NCELLS];
#endif /* UPGRADE_3G_FDD */
#if defined (UPGRADE_3G_TDD128)
    /** An absolute threshold of RSCP for UTRAN TDD cell re-selection, where: <UL>
     * <LI> 0 = -105dBm, 1 = -102dBm, 2=-99dBm, 3 = -96dBm, ..., 15 = -60dBm.
     * Default value = -90dBm.
     * </UL>
     */
    Int8 tdd128QOffset;

    /** Apply an offset to the reported value when prioritising the cells for reporting for
     * GSM frequency band or access technology/mode TDD
     */
    Int8 tdd128ReportOffset;

    /** Apply priority reporting if the reported value is above threshold for GSM frequency
    * band or access technology/mode TDD
    */
    Int8  tdd128ReportThreshold;

    /** List of blocking TDD128 cells; where each entry is a
     * bit map of 32-bit indicating which cell is blocked */
    Int32 tdd128BlockingActive[3];

    /** The number of cells in the Top Six list */
    Int8  numTdd128RscpCells;

    /** Holds top six cells */
    GrrTestIdleTdd128NcellInfo tdd128RscpTopSix[NUM_NCELLS];
#endif /* UPGRADE_3G_TDD128 */
#endif
}
GrrTestReselInfoInd;

/** This signal indicates a change in capabilities of the Access Stratum.
 * This signal is sent from GRR to (G)MM to indicate if there are service restrictions in place
 * (e.g. GPRS must be temporarily suspended).*/
typedef struct GrrAccStratumCapabInfoIndTag
{
    /** Indicates the current capabilities of the Access Stratum */
    AccStratumCapabInfo capabInfo;
}
GrrAccStratumCapabInfoInd;

/** This signal is a response to a GrrLocalInfoReq.
 *  This signal used to supply local information
 */
typedef struct GrrLocalInfoCnfTag
{
    /** Indicates if ME is in idle mode */
    Boolean inIdleMode;

    /** Indicates if measurement report is present */
    Boolean measReportPresent;

    /** Holds encoded measurement report
     * (Encoded according to 44.018 section 10.5.2.20 )
     */
    Int8              encodedMeasReport[ENCODED_MEAS_RESULTS_IE_SIZE];

    /** Indicates whether local information is present */
    Boolean           localLocInfoPresent;

    /** Local information (LAI, Cell ID and Band to use)*/
    LocalLocationInfo localLocInfo;

    /** The number of BCCH ARFCN */
    Int8              numC0Arfcn;

    /** Lists BCCH frequencies */
    Arfcn             c0ArfcnList[MAX_BA_CHANNELS];

    /** Whether or not timing advance is present */
    Boolean           timingAdvancePresent;

    /** Determines timing advance (only available when the ME is connected
     * to a GSM access network)
     */
    Int8 timingAdvance;
}
GrrLocalInfoCnf;

#if defined (UPGRADE_3G) && defined (UPGRADE_GGE)

/** NAS transmission queue information indication.
 * This signal send the queued NAS messages to MM.
 */
typedef struct GrrNasTxQueueInfoIndTag
{
    /** Number of NAS messages in transmission queue */
    Int16 numInTxQueue;

    /** Holds the header of NAS message.
     * The format of nasPdAndMsgType is as follows:
     * xxxx pppp aaaa bbbb where: <UL>
     * <LI> xxxx == don't care
     * <LI> pppp == PD/element[0] of DlDataReq
     * <LI> aaaa == MsgType/element[1] of DLDataReq message
     * <LI> bbbb == element[2] of DlDataReq message (in case of extended TI)
     *</UL>
     */
    Int32 nasMsgHeader[MAX_NAS_TX_QUEUE_INFO_SIZE];
}
GrrNasTxQueueInfoInd;
#endif

/** Decode the RR short protocol discriminator message indication */
typedef L3ShortPdMessage GrrShortPdDecodeInd;

/** Neighbour cell information in PTM*/
typedef struct GrrTestPtmNcellInfoTag
{
    /** The neighbour cell BCCH channel number in PTM mode*/
    Arfcn   arfcn;

    /** The current level of the neighbour cell */
    Int8    level;

    /** Base Station Identity Code of the neighbour cell*/
    Bsic    bsic;

    /** Whether or not BCCH is valid */
    Boolean bcchValid;

    /** Cell reselection C1 for the neighbour cell;
     * C1 is a cell selection algorithm employed in GSM and GPRS;
     * The algorithm uses the power received from cells plus additional
     * parameters in order to assess the cell that will provide the best
     * radio connection for the MS
     */
    SignedInt16 c1;

    /** The cell reselection C31 for the neighbour cell
     * (only relevant if isInPacketIdle is TRUE)
     */
    SignedInt16 c31;

    /** The cell reselection C32 for the neighbour cell
     * (only relevant if isInPacketIdle is TRUE)
     */
    SignedInt16 c32;
#if defined (UPGRADE_ADDIT_ENG_INFO)
    /** The Mobile Country Code is a three digit number uniquely identifying a given country */
    Mcc         mcc;

    /** The Mobile Network Code is either a two or three digit number used to uniquely identify
     * a given network from within a specified country (MCC)
     */
    Mnc mnc;

    /** The Location Area Code for the neighbour cell; The Location Area Code uniquely identifies
     * a Location Area (LA) within a PLMN
     */
    Lac    lac;

    /** ID od the neighbour cell */
    CellId cellId;
#endif
}
GrrTestPtmNcellInfo;

/** GPRS Engineering Information element */
typedef struct GrrTestGprsEngInfoIndTag
{
    /** Bit map of UL slot assignment */
    Int8 ulSlotAssignment;

    /** Bit map of DL slot assignment */
    Int8 dlSlotAssignment;

    /** Uplink TFI (0 to 31) */
    Int8 ulTfi;

    /** Downlink TFI (0 to 31) */
    Int8 dlTfi;

    /** Normalised received signal level.
     * In packet idle mode, the MS measures the C value on each monitored PPCH block, while
     * in packet transfer mode, the MS measures the C value on the BCCH carrier.
     */
    Int8 cValue;

    /** Received signal quality.
     * For each channel, the measured parameters (RXQUAL) shall be the received signal quality,
     * averaged on that channel over the reporting period of length one SACCH multiframe
     */
    Int8                rxQual;

    /** The variance of the received signal level */
    Int8                signVariance;
    Int16               numGprsUlTbfs;                      /**< Total number of GPRS uplink TBFs                                    */
    Int16               numGprsDlTbfs;                      /**< Total number of GPRS downlink TBFs                                  */
#if defined (UPGRADE_EDGE)
    /** Whether or not TBF is supporting EDGE */
    Boolean             isEdgeTbf;
    Int16               numEgprsUlTbfs;                     /**< Total number of EDGE uplink TBFs                                    */
    Int16               numEgprsDlTbfs;                     /**< Total number of EDGE downlink TBFs                                  */
#endif
    Int32               totNumRlcBlocksTx;                  /**< Total number of RLCMAC blocks transmitted including Tx, ReTx, Ctrl + Dummy      */
    Int32               totNumRlcBlocksRx;                  /**< Total number of RLCMAC blocks received including Rx, Ctrl, discarded + Dummy */
    Int32               totNumRlcDataBlocksTx;              /**< Number of RLCMAC data blocks transmitted                                       */
    Int32               totNumRlcDataBlocksReTx;            /**< Number of RLCMAC data blocks received and transmitted                                    */
    Int32               totNumRlcCtrlBlocksTx;              /**< Number of RLCMAC control blocks transmiteted                                    */
    Int32               totNumRlcDummyBlocksTx;             /**< Number of RLCMAC dummy blocks transmiteted                                      */
    Int32               totNumRlcDataBlocksRx;              /**< Number of RLCMAC data blocks received                                       */
    Int32               totNumRlcCtrlBlocksRx;              /**< Number of RLCMAC control blocks received                                    */
    Int32               totNumRlcDummyBlocksRx;             /**< Number of RLCMAC dummy blocks received                                     */
    Int32               totNumRlcDataBlocksRxDiscWin;       /**< Number of RLCMAC blocks outside window                                 */
    Int32               totNumRlcDataBlocksRxDiscTfi;       /**< Number of RLCMAC blocks discarded due to incorrect TFI                 */
    Int32               numGoodDecodeBlksRx;                /**< Number blocks received that have been decoded correctly                     */
    Int32               numBadDecodeBlksRx;                 /**< Number blocks received that have been decoded incorrectly                   */
    Int8                ulCommandedCodingScheme;            /**< The coding scheme commanded by the network                           */
    Int8                dlReceivedCodingScheme;             /**< The coding scheme of the last received payload block                 */

    /** Serving cell ARFCN */
    Arfcn               sCellArfcn;

    /** The strength of the received BCCH signal (0-63) */
    Int8                sCellLevel;
#if defined (UPGRADE_ADDIT_ENG_INFO)
    /** Base Station Identity Code of the serving cell*/
    Int8                sCellBsic;
#endif

    /** Path loss parameter */
    SignedInt16         sCellC1;

    /** Signal level threshold parameter */
    SignedInt16         sCellC31;

    /** Cell ranking parameter */
    SignedInt16         sCellC32;

    /** Number of entries in ncellInfo[] */
    Int8                top6Size;

    /** Neighbour cell information */
    GrrTestPtmNcellInfo ncellInfo[NUM_NCELLS];
}
GrrTestGprsEngInfoInd;


typedef union EngCommandResultInfoTag
{
    /** For command 'FULL_SCAN',
     * Full band scan result information list. 
     */
    EngFullScanResultList        fullScanResultList;

    /** For command 'SPECIFIED_ARFCN_SCAN',
      * Carries the contents of specified arfcn scan result information. 
      */
    EngGsmCellLevel             specifiedScanResultList;
    EngArfcnLevel               specifiedScanResult;
}
EngCommandResultInfo;

typedef struct GrrEngineeringModeDataTag
{   
    Boolean    lockCell;
    Boolean    handoverPermit;
    Boolean    fullScan;
    Boolean    specifiedScan;
    Boolean    cellReselection;
    Boolean    triggerHandoverGsm;
    Boolean    triggerHandoverFdd;
    Boolean    triggerHandoverTdd;

    EngCmdLockCellInfo    lockCellInfo; 
    Arfcn      arfcnScanInfo;
    Int8                  handoverIndexGsm;
    Int8                  handoverIndexUmts;
    EngPowerSavingType powerSavingIndex;	
}
GrrEngineeringModeData;

/** Engineering mode mandatory command result type */
typedef enum EngCommandResultTypeTag
{    
    ENG_CMD_RESULT_FULL_SCAN,    
    ENG_CMD_RESULT_SPECIFIED_ARFCN_SCAN,    
    ENG_CMD_NO_COMMAND
}EngCommandResultType;

/** Commands for engineering mode **/
typedef struct GrrEngineeringCommandReqTag
{   
    /** \assoc UNION \ref commandInfo */
    GrrEngineeringCommand command;

    EngCommandInfo commandInfo;
}
GrrEngineeringCommandReq;


typedef struct GrrEngineeringCommandCnfTag
{   
    Boolean success;   

    EngCommandFailureCause failureCause;  
    
    /** \assoc UNION \ref result */
    EngCommandResultType commandResultType;
    
    EngCommandResultInfo    result;
}
GrrEngineeringCommandCnf;



/** @} */ /* End of SigGrr group */
#endif   /* GRR_SIG_H */
/* END OF FILE */
