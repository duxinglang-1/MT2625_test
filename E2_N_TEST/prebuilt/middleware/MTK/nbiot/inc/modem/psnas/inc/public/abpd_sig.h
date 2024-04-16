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
 * File Description:                                                     */
/** \file
 * definition of signals sent to/from ABPD module within the AB task
 **************************************************************************/

#ifndef ABPD_SIG_H
#define ABPD_SIG_H

/* Must have UPGRADE_GENERIC_MUX for UPGRADE_NASMDL2 */
#if !defined (UPGRADE_GENERIC_MUX)
#error For UPGRADE_NASMDL2, UPGRADE_GENERIC_MUX must always be enabled
#endif

#include <system.h>
#include <rvsystem.h>

#include <gpqos.h>
#include <comp_typ.h>
#include <pdp_typ.h>
#include <pdn_typ.h>
#include <psdbearerdef.h>

#include <gppp_sig.h>
#include <gpcntr.h>

#include <abst_typ.h>
#include <alsa_typ.h>

/** \defgroup SigApexAbpd ABPD Signalling Interface
 * \ingroup NasApex
 * \ingroup Sig
 * This section defines the Application Background Packet Data Procedures.
 * This Application Background procedure is specific to Packet
 * Data.  It communicates with the following tasks:<UL>
 * <LI> ATCI (Client task) via ABPD interface
 * <LI> TCP/IP (Client task) via ABPD interface
 * <LI> MUX (Client task) via ABPD interface
 * <LI> ANRM For read/write of stored PDP Context and EPS bearer
 * information
 * <LI> SM for management of PDP Contexts and EPS Bearers
 * <LI> DBM for management of PDP Context and EPS Bearer data path
 * <LI> PPP for management of the PPP server for PPP connections with
 * the TE. </UL>
 *
 * The following services are provided to the client tasks (ATCI,
 * TCP/IP and MUX) via Apex signals:<UL>
 * <LI> co-ordination of Packet Data Stack access between
 * multiple controlling tasks (allocation of channel numbers and
 * connection IDs)
 * <LI> co-ordination of PDP and EPS bearer context activation,
 * deactivation and modification, by co-ordination of SM and DBM tasks
 * <LI> co-ordination of PPP connection activation and deactivation
 * with the TE for PPP Packet Data connections.
 * <LI> release of Packet Data connection identifiers (deallocation of channel
 * numbers and connection IDs))
 * <LI> reporting of run-time error conditions
 * <LI> transferring PPP configuration signals to the PPP task
 * <LI> transferring PPP loopback control signals to the PPP task
 * <LI> transferring counter signals to/from DBM. </UL>
 *
 * ABPD is the co-ordination point for all Packet Switched connections
 *(ie PDP contexts for 2G/3G and EPS bearers for LTE).
 *
 * The context of the ABPD interface is illustrated in the Figure 1 below:
 *
 * \image html ABPD_interface.png "Figure 1: ABPD Interface"
 * @{
 */

/**** CONSTANTS ************************************************************/

/** Maximum length of alpha string associated with PSD connection*/
#define MAX_PSD_APN_ALPHA_TAG               40
/** Maximum length of APN list stored on the SIM */
#define MAX_SIM_PSD_APN_LIST_SIZE           5

/* Bitmap setting for SIM Toolkit changes to the PSD context
 * setup.
 */
#define PSD_CONTEXT_NSAPI_CHANGED           0x01
#define PSD_CONTEXT_QOS_CHANGED             0x02
#define PSD_CONTEXT_PDN_TYPE_CHANGED        0x04
#define PSD_CONTEXT_PDN_ADDRESS_CHANGED     0x08
#define PSD_CONTEXT_APN_CHANGED             0x10
#define PSD_CONTEXT_CONFIG_OPTS_CHANGED     0x20

/** Allow for build-time selection of number of PDP context / PDN
 * connection records */
#define MAX_NUM_PSD_STORAGE_RECORDS         0


/** Maximum number of channels that have a channelNumber.
 * channelNumber is always in range 0-(MAX_NUM_MDL_CHANNELS - 1)
 */

/* For NB-IOT we currently need 14 channels. */
#define MAX_NUM_MDL_CHANNELS            14

/** Maximum number of connection IDs (connIDs) we can have.*/
/* One extra connID needed for storing attachDefaultBearer */
/* content to NVRAM before entering to sleep.              */
#define MAX_NUM_CONN_IDS               (MAX_NUM_PSD_BEARERS + 1)

/** Invalid connId value */
#define INVALID_CONN_ID                (0xFF)

/** Default connId value */
#define DEFAULT_CONN_ID                (0xF0)

/** Invalid channel number */
#define INVALID_PSD_CHANNEL_NUMBER     (0xFF)

/* Max number of UPDIR packet Id reported one time */
#define ABPD_MAX_NUM_UPDIR_PACKET_ID_REPORT (16)

/**** TYPEDEFS *************************************************************/

/** Specifies the PSD connection mode ABPD can operate in.  This is specifically
 *  for multimode operation.
 *
 *  NOTE: PSD mode functionality removed from ABPD. This definition is left ony for
 *        compilation purposes of other modules.
 */
typedef enum AbpdPsdModeTag
{
  ABPD_PSD_MODE_LTE = 0,     /**< Allows only LTE style connections (ie only one PDP context with a specific APN is possible)  This is the default mode of operation */
  ABPD_PSD_MODE_LEGACY_2G3G  /**< Allows legacy 2G/3G mode of operation such as multiple PDP context activation using the same APN */
}
AbpdPsdMode;

/** Specifies the type of connection to the PC for this PDP context / PDN
 * connection (ie None, PPP or Packet Transport)
 */
typedef enum AbpdPdpConnTypeTag
{
  ABPD_CONN_TYPE_NONE = 0,           /**< No connection to PC*/
  ABPD_CONN_TYPE_PPP,                /**< PPP type connection */
  ABPD_CONN_TYPE_CORE_PPP,           /**< Core PPP type connection */
  ABPD_CONN_TYPE_PACKET_TRANSPORT    /**< Packet Transport type connection (NDIS for example) */
} AbpdPdpConnType;

/** Reports the result of an NVRAM or SIM access */
typedef enum AbpdRequestStatusTag
{
  /** The request has succeeded */
  ABPD_REQ_OK,
  /** An NVRAM error has occurred */
  ABPD_REQ_NRAM_ERROR,
  /** implies either an invalid record number was supplied
   *  in the request  or that no record was found at
   * that location
   */
  ABPD_REQ_RECORD_NOT_FOUND,
  ABPD_REQ_INVALID_PARAMS,        /**< invalid parameters specified in the request*/
  ABPD_REQ_SIM_ERROR,             /**< SIM error*/
  ABPD_REQ_FILE_NOT_FOUND,        /**< File does not exist*/
  ABPD_REQ_POWERING_DOWN,         /**< powering down*/
  ABPD_REQ_ACCESS_DENIED,         /**< PIN/PIN2 has not been verified*/
  ABPD_REQ_MEMORY_PROBLEM,        /**< ACL file too small to store another APN*/
  ABPD_REQ_NOT_ALLOWED,           /**< operation not allowed*/
  ABPD_REQ_SERVICE_NOT_AVAILABLE  /**< service not available*/
} AbpdRequestStatus;

/** Specifies how to update the ACL */
typedef enum AbpdAclWriteModeTag
{
  ABPD_ACL_WRITE_FIRST_FREE,       /**< Adds new APN at the end of ACL file*/
  ABPD_ACL_WRITE_ABSOLUTE,         /**< will overwrite the existing APN*/
  ABPD_ACL_WRITE_INSERT            /**< inserts the new APN at the specified position*/
} AbpdAclWriteMode;

/** Specifies how to delete an ACL */
typedef enum AbpdAclDeleteModeTag
{
  ABPD_ACL_DELETE_FIRST,                     /**< delete first*/
  ABPD_ACL_DELETE_LAST,                      /**< delete last*/
  ABPD_ACL_DELETE_ABSOLUTE                   /**< delete absolute*/
} AbpdAclDeleteMode;

/** Indicates the storage status of reading or writing of PSD context
 * information in NVRAM.
 */
typedef enum AbpdPsdRequestStatusTag
{
  ABPD_PSD_OK,                                 /**< NVRAM operation completed OK */
  ABPD_PSD_NRAM_ERROR,                         /**< NVRAM error occurred during read/write */
  ABPD_PSD_RECORD_NOT_FOUND                    /**< Record to write or read was not found */
} AbpdPsdRequestStatus;

/** Generic ABPD Cause value for PSD events (this is separate from the GsmCause value.
 */
typedef enum AbpdPsdCauseTag
{
  ABPD_CAUSE_NO_CAUSE,                         /**< No cause value */
  ABPD_CAUSE_UNKNOWN,                          /**< Unknown reason for event */
  ABPD_CAUSE_INVALID_CHANNEL_NUMBER,           /**< The channel number provided to ABPD is invalid */
  ABPD_CAUSE_NO_PSD_SERVICE,                   /**< No PSD service */
  ABPD_CAUSE_POWERING_DOWN,                    /**< UE is switching off */
  ABPD_CAUSE_NO_FREE_CONN_ID,                  /**< No free connIds for this PSD bearer connection */
  ABPD_CAUSE_INVALID_SEC_CONTEXT,              /**< Invalid secondary context information supplied by upper layer */
  ABPD_CAUSE_INCORRECT_USERNAME_PASSWD,        /**< Incorrect username/password supplied by upper layer */
  ABPD_CAUSE_APN_ALREADY_IN_USE,               /**< APN already in use by another default bearer - this is for LTE only */
  ABPD_CAUSE_UE_ACTION_OK,                     /**< UE initiated action success */
  ABPD_CAUSE_UE_ACTION_FAIL,                   /**< UE initiated action failure */
  ABPD_CAUSE_NW_ACTION_OK,                     /**< NW initiated action success */
  ABPD_CAUSE_NW_ACTION_FAIL,                   /**< NW initiated action failure */
  ABPD_CAUSE_DED_B_DEACT_LOCAL,                /**< UE local initiated dedicated bearer / secondary PDP context deactivation */
  ABPD_CAUSE_DEDB_ACT_FAIL_NW_MOD_RESP,        /**< Dedicated bearer activation fail due to network modification response (LTE only) */
  ABPD_CAUSE_DEF_B_DEACT_OK_NW_REJ,            /**< Default bearer deactivation success, however network rejected request (LTE only) */
  ABPD_CAUSE_DED_B_DEACT_BY_DEF_B_DEACT,       /**< UE initiated dedicated bearer deactivation due to associated default bearer deactivation (LTE only) */
  NUM_ABPD_CAUSE_VALUES                        /**< Number of ABPD cause values */
} AbpdPsdCause;

/** Values for Release Assistance Indication information */
typedef enum AbpdRelAssistInformationTag
{
  ABPD_RAI_NO_INFO,               /**< No information available (or none of the other options apply) (default) */
  ABPD_RAI_UL_NO_DL,              /**< Only 1 UL packet and no DL packets expected */
  ABPD_RAI_UL_AND_DL              /**< Only 1 UL packet and only 1 DL packet expected */
} AbpdRelAssistInformation;

/** Values for APN Data Type  */
typedef enum AbpdApnDataTypeTag
{
  ABPD_APN_DATA_TYPE_NORMAL,      /**< Normal data (default) */
  ABPD_APN_DATA_TYPE_EXCEPTIONAL  /**< Exceptional data  */
} AbpdApnDataType;

/** Channel peer  */
typedef enum ChannelPeerTag
{
  INTERNAL,      /**< AP source is internal */
  EXTERNAL       /**< AP source is external */
} ChannelPeer;

#ifdef ENABLE_ABPD_UNIT_TEST
/** Modes for ABPD Unit Testing  */
typedef enum AbpdUnitTestModeTag
{
  ABPD_UNIT_TEST_MODE_PREPARE_FOR_SLEEP,  /**< UT mode for testing preparations for sleep */
  ABPD_UNIT_TEST_MODE_GO_TO_SLEEP,        /**< UT mode for testing going to sleep */
  ABPD_UNIT_TEST_MODE_SET_STUB_RET_VALUES /**< UT mode for setting return values for UT stub functions */
} AbpdUnitTestMode;

/** ABPD UT stub function return values  */
typedef struct AbpdStubRetValuesTag
{
  /**< Indicates whether current time information is present */
  Boolean                  currentTimePresent;
  /**< Current time in seconds */
  uint32_t                 currentTime;
  /**< Indicates whether HplmnCheck return value is present */
  Boolean                  hplmnCheckPresent;
  /**< HplmnCheck return value */
  Boolean                  hplmnCheck;
  /**< Indicates whether EhplmnCheck return value is present */
  Boolean                  ehplmnCheckPresent;
  /**< EhplmnCheck return value */
  Boolean                  ehplmnCheck;
  /**< Indicates whether isAc11to15 return value is present */
  Boolean                  isAc11to15Present;
  /**< isAc11to15 return value */
  Boolean                  isAc11to15;
} AbpdStubRetValues;
#endif /* ENABLE_ABPD_UNIT_TEST */


/** Entity for APN Data Type  */
typedef struct AbpdApnDataTypeEntityTag
{
  /**< Indicates whether the APN Data Type information is valid for this
       connection */
  Boolean                  abpdApnDataTypeValidity;
  /**< APN Data Type */
  AbpdApnDataType          abpdApnDataType;
} AbpdApnDataTypeEntity;

/** This structure provides a description as text of the APN */
typedef struct AbpdApnAlphaIdTag
{
  /** Indicates the size of the string contained in data (in bytes)
\assoc ARRAY \ref data */
  Int8    length;
  /** APN identifier data */
  Int8    data [ MAX_PSD_APN_ALPHA_TAG + 1 ];
} AbpdApnAlphaId;

/** APN List element */
typedef struct AbpdApnListTag
{
  /** Total number of APNs currently held on the SIM in EF ACL*/
  Int8                                         totalNumApns;
  /** The number of APNs in list below
\assoc ARRAY \ref apn */
  Int8                                         numApns;
  /** Holds a list of APN data */
  TextualAccessPointName                       apn[MAX_SIM_PSD_APN_LIST_SIZE];
} AbpdApnList;

/** Structure containng all the information for a PDP context (2G/3G) or
 * EPS bearer (LTE) which has been stored in NVRAM.
 */
typedef struct AbpdPsdRecordTag
{
  /** Whether the PSD profile information is defined in this record */
  Boolean                 profileDefined;
  /** Whether the AT+CGDCONT information is defined (Primary PDP context for 2G/3G, Default bearer for LTE) */
  Boolean                 cgdcontDefined;
  /** Whether the AT+CGQREQ information is defined */
  Boolean                 cgqreqDefined;
  /** Whether the AT+CGQMIN information is defined */
  Boolean                 cgqminDefined;
  /** The PDN address (IP address) */
  PdnAddress              pdnAddress;
  /** Indicates if APN is present
\assoc PRESENT \ref apn */
  Boolean                 apnPresent;
  /** APN */
  AccessPointName         apn;
  /** Username and password */
  PsdUser                 psdUser;
  /** Header Compression setting */
  HeaderCompType          hComp;
  /** Data Compression setting */
  DataCompType            dComp;
  /** Whether minimum QoS settings present
\assoc PRESENT \ref minimumQos */
  Boolean                 minQosPresent;
  /** Minimum QoS settings (not required for LTE) */
  QualityOfService        minimumQos;
  /** Whether required QoS settings present
\assoc PRESENT \ref qos */
  Boolean                 qosPresent;
  /** Required Qos settings */
  QualityOfService        qos;
  /** Whether AT+CGDSCONT information is defined (Secondary PDP context for 2G/3G, Dedicated bearer for LTE) */
  Boolean                 cgdscontDefined;
  /** Whether the AT+CGEQREQ information is defined */
  Boolean                 cgeqreqDefined;
  /** Whether the AT+CGEQMIN information is defined */
  Boolean                 cgeqminDefined;
  /** Whether the AT+CGEQOS information is defined */
  Boolean                 cgeqosDefined;
  /** Whether the AT+CGTFT information is defined */
  Boolean                 cgtftDefined;
  /** Is TFT present
\assoc PRESENT \ref tft */
  Boolean                 tftPresent;
  /** TFT information */
  TrafficFlowTemplate     tft;
  /** Is this a secondary PDP context (2G/3G) / Dedicated bearer (LTE) definition */
  Boolean                 secondaryContext;
  /** Primary CID value asociated with secondary PDP context / dedicated bearer */
  Int8                    primaryCid;
} AbpdPsdRecord;

/** Used by the ApexAbpd signals to access stored APNs.
 * Contains the APN, a descriptive name, username and a usage field and is
 * the structure used by the ApexAbpd signals to access stored APNs.
 *
 * Note that there is only ever 1 APN available to read/write.  This data is
 * used for the first PDN connection during the attach procedure.
 */
typedef struct AbpdApnTag
{
  /** Indicates if the APN is is present or not.  It is not
   * mandatory for LTE during the attach procedure
\assoc PRESENT \ref apn
\assoc PRESENT \ref textualApn */
  Boolean                apnPresent;
  /** The APN to be used in PDP context / EPS bearer activation.
   * This is in the network format i.e. "3wap2o22co2uk"
   */
  AccessPointName        apn;
  /** APN in text format.  ie. "wap.o2.co.uk" */
  TextualAccessPointName textualApn;
  /** IP Addressing type for this PDP context / EPS bearer when activated */
  PdnType                pdnType;
  /** Username / password to supply at context activation, maybe blank */
  PsdUser                psdUser;
} AbpdApn;

/** Request for a channel number from any task to ABPD. */
typedef struct ApexAbpdChannelAllocReqTag
{
  /** The sending task ID */
  TaskId       sourceTaskId;
  /** The connection layer task ID */
  TaskId       connectionLayerTaskId;
  /** Channel peer */
  ChannelPeer  channelPeer;
} ApexAbpdChannelAllocReq;

/** Response to ApexAbpdChannelAllocReq with allocated channel number. */
typedef struct ApexAbpdChannelAllocCnfTag
{
  /** Channel number successfully allocated */
  Boolean success;
  /** Channel number allocated */
  Int32   channelNumber;
} ApexAbpdChannelAllocCnf;


/** Request for a channel number to be freed by ABPD. */
typedef struct ApexAbpdChannelFreeReqTag
{
  /** The sending task ID */
  TaskId    sourceTaskId;
  /** Channel number to free */
  Int32     channelNumber;
} ApexAbpdChannelFreeReq;

/** This signal is sent by ATCI or TCPIP to ABPD in order to activate a mobile
 * originated PSD connection (PDP context for 2G/3G or EPS bearer for LTE).
 * ( For ATCI this can be when ATD*99....#, AT+CGDATA or AT+CGACT command is entered.
 * ABPD will then perform the requested activation, activating PPP if
 * required for a PPP type connection (connType = PPP), or Packet Transport connection
 * (connType = PACKET_TRANSPORT).
 */
typedef struct ApexAbpdDialReqTag
{
    /** The sending task ID */
    TaskId                    sourceTaskId;
    /** Channel Number */
    Int32                     channelNumber;
    /** Specifies the type of connection */
    AbpdPdpConnType           connType;
    /** Indicates if APN is present.
\assoc PRESENT \ref apn */
    Boolean                   apnPresent;
    /** APN to be used for any new PDP contexts. */
    AccessPointName           apn;
    /** Requested PDN address information if present */
    PdnAddress                reqPdnAddress;
    /** Whether or not to check the FDN for APN */
    Boolean                   doFdnCheck;
    /** Username and password.  Required when connType is Packet Transport */
    PsdUser                   psdUser;
    /** Minimum QoS present
\assoc PRESENT \ref minimumQos */
    Boolean                   minimumQosPresent;
    /** Minimum QoS (only required for 2G/3G) */
    QualityOfService          minimumQos;
    /** Requested QoS present
\assoc PRESENT \ref qos */
    Boolean                   qosPresent;
    /** Requested QoS */
    QualityOfService          qos;
    /** Traffic Flow Template present
\assoc PRESENT \ref tft */
    Boolean                   tftPresent;
    /** Traffic Flow Template */
    TrafficFlowTemplate       tft;
    /** Header Compression setting */
    HeaderCompType            headerComp;
    /** Data Compression setting */
    DataCompType              dataComp;
    /** Whether this context is a secondary context (2G/3G) / dedicated bearer for LTE */
    Boolean                   secondaryContext;
    /** Associated primary PDP context / default EPS bearer connection ID if this is a
     * secondary PDP context / dedicated EPS bearer */
    Int8                      primaryConnId;
    /** Specifies the flow control mechanism */
    FlowCtrlType              flowControlType;
#if defined (ENABLE_PPP_RAW_LOGGING)
    /** Raw logging level */
    Int8                      rawLoggingLevel;
    /** Raw logger task */
    TaskId                    rawLoggerTask;
#endif
    /** ipv4 Link MTU Request */
    MtuSizeRequested                      ipv4LinkMTURequest;
    /** non IP Link MTU Request */
    MtuSizeRequested                      nonIPLinkMTURequest;
#if defined(FEA_TEMP_REL14_ABPD_DUAL_PRIORITY)
    /** Dual priority configured by Upper Layers*/
    DevicePropertiesElement               deviceProperties;
#endif
}
ApexAbpdDialReq;

/** This signal is sent by ATCI to ABPD in order to activate the data connection
 * to the PC for a context/EPS bearer which has already previously been activated.
 * This only applies to:
 * - For LTE: Default EPS bearers only
 * - For 2G/3G: Primary contexts, or secondary contexts with its own channel number
 *   (i.e. not using the same channel number as the primary context it is associated with).
 * This is specifically used for when the user may have already activated the
 * context/EPS bearer with AT+CGACT (i.e. with not data connection), but now
 * wants to give it a data connection (i.e. with ATD*99# or  AT+CGDATA command).
 * In response, ABPD will either send a ApexAbpdDataConnCnf, or a
 * ApexAbpdErrorInd to indicate the data connection failed, but the EPS bearer
 * is still active.
 */
typedef struct ApexAbpdActivateDataConnReqTag
{
    /** Connection ID */
    Int8                      connId;
    /** Specifies the type of connection */
    AbpdPdpConnType           connType;
}
ApexAbpdActivateDataConnReq;

/** This signal is sent by ABPD to ATCI or TCP/IP to indicate the activation
 * of a data connection for a previously already active PSD bearer is complete.
 */
typedef struct ApexAbpdActivateDataConnCnfTag
{
    /** Connection ID */
    Int8                      connId;
}
ApexAbpdActivateDataConnCnf;

/** This signal is sent by ABPD to ATCI or TCP/IP to indicate the connId
 * assigned to the PSD connection requested by ATCI or TCP/IP.  These tasks will then
 * store this connId in order to refernce the PSD connection.
 */
typedef struct ApexAbpdContextIndTag
{
    /** Channel Number */
    Int32       channelNumber;
    /** Connection ID */
    Int8        connId;
}
ApexAbpdContextInd;

/** This signal is sent by ABPD to ATCI or TCP/IP to request that they
 * need to now start PSD data mode.  for ATCI this means sending out the
 * "CONNECT" String to the PC before switching to data mode.
 */
typedef struct ApexAbpdConnectIndTag
{
    /** Connection ID */
    Int8        connId;
}
ApexAbpdConnectInd;

/** This signal is sent by ATCI or TCP/IP to ABPD to indicate that the
 * ATCI or TCP/IP have started PSD data mode in response to the ApexAbpdConnectInd
 * from the ABPD task.
 * For the ATCI this means that it has sent out the "CONNECT" string to the PC and then
 * switched the MUX to PSD data mode.
 */
typedef struct ApexAbpdConnectRspTag
{
    /** Connection ID */
    Int8        connId;
}
ApexAbpdConnectRsp;


/** This signal is sent by ATCI or TCP/IP to ABPD to indicate that the
 * ATCI or TCP/IP have rejected the connection setup request from ABPD.
 * This can be caused, for example, when the MUX cannot switch to PSD data
 * mode for some reason.
 */
typedef struct ApexAbpdConnectRejTag
{
    /** Connection ID */
    Int8        connId;
    /** The cause of error */
    GsmCause    cause;
}
ApexAbpdConnectRej;

/** This signal is sent by ABPD to ATCI or TCP/IP to indicate progress
 * in the activation of a PSD connection.  At this point ABPD requests SM task
 * to activate the PSD connection.
 */
typedef struct ApexAbpdConnectingIndTag
{
    /** Connection ID */
    Int8        connId;
}
ApexAbpdConnectingInd;

/** This signal is sent by ABPD to ATCI or TCP/IP to indicate the activation
 * of a PSD connection is now complete.  All relevent information about the
 * connection is passed to ATCI/TCPIP at in this message.
 */
typedef struct ApexAbpdConnectedIndTag
{
    /** Connection ID */
    Int8                      connId;
    /** PSD Bearer ID (NSAPI for 2G/3G, EPS bearer ID for LTE) */
    Int8                      psdBearerId;
    /** Has PSD context information been altered by SIM as part of SIM toolkit
     * CC of PSD activation
     */
    Boolean                   modifiedBySim;
    /** Bitmap of modifications to the PSD bearer by the SIM
     */
    Int8                      simMods;
    /** Is the APN present
\assoc PRESENT \ref apn */
    Boolean                   apnPresent;
    /** access point name - APN - we need to send this back to upper layer task
     * in case it was changed by the SIM
     */
    AccessPointName           apn;
    /** IP Address, DNS and Gateway addresses */
    PdnConnectionAddressInfo  pdnConnectionAddressInfo;
    /** Negotiated QoS present
\assoc PRESENT \ref qos */
    Boolean                   qosPresent;
    /** Negotiated QoS */
    QualityOfService          qos;
    /** Maximum bit rates present for APN
\assoc PRESENT \ref apnAmbr */
    Boolean                   apnAmbrPresent;
    /** Maximum bit rates for APN */
    ApnAmbr                   apnAmbr;
    /** Traffic Flow Template present
\assoc PRESENT \ref tft */
    Boolean                   tftPresent;
    /** Traffic Flow Template */
    TrafficFlowTemplate       tft;
    /** Whether this context is a secondary context (2G/3G) / dedicated bearer for LTE */
    Boolean                   secondaryContext;
    /** Associated primary PDP context / default EPS bearer connection ID if this is a
     * secondary PDP context / dedicated EPS bearer */
    Int8                      primaryConnId;
    /** Specifies the flow control mechanism */
    FlowCtrlType              flowControlType;
    /** Generic ABPD cause for PSD connection activation */
    AbpdPsdCause              abpdCause;
    /** 3GPP Cause present
\assoc PRESENT \ref cause */
    Boolean          causePresent;
    /** Determines the 3GPP cause from the protocol stack for activation
     * (where applicable). */
    GsmCause         cause;

    /** APN Rate Control Parameters */
    Boolean                   apnRateControlPresent;
    /** Additional exception reports at maximum rate reached allowed or not */
    /* \assoc PRESENT \ref apnRateControlAdditionalExceptionReportsAllowed */
    Boolean                   apnRateControlAdditionalExceptionReportsAllowed;
    /** Uplink time unit */
    /* \assoc PRESENT \ref apnRateControluplinkTimeUnit */
    Int8                      apnRateControluplinkTimeUnit;
    /** Maximum uplink rate */
    /* \assoc PRESENT \ref apnRateControlMaxUplinkRate */
    Int32                     apnRateControlMaxUplinkRate;

#if defined (ENABLE_PPP_RAW_LOGGING)
    /** Raw logging level */
    Int8                      rawLoggingLevel;
    /** Raw logger task */
    TaskId                    rawLoggerTask;
#endif
}
ApexAbpdConnectedInd;

/** This signal is sent by ABPD to ATCI for each active PSD bearer after
 * wake-up from deep sleep in order to allow ATCI to copy data in to internal
 * structures before S/W resumes activity after wake-up.
 */
typedef struct ApexAbpdPsdBearerStatusIndTag
{
    /** Connection ID.  NOTE: ATCI Must already have stored
     * at least connId for the associated CID in order to copy the data in
     * this signal back in to appropriate structures in ATCI
     */
    Int8                      connId;
    /** Specifies the type of connection */
    AbpdPdpConnType           connType;
    /** PSD Bearer ID (NSAPI for 2G/3G, EPS bearer ID for LTE) */
    Int8                      psdBearerId;
    /** Has PSD context information been altered by SIM as part of SIM toolkit
     * CC of PSD activation
     */
    Boolean                   modifiedBySim;
    /** Bitmap of modifications to the PSD bearer by the SIM
     */
    Int8                      simMods;

    /** Username and password */
    PsdUser                   psdUser;

    /** Is the APN present
\assoc PRESENT \ref apn */
    Boolean                   apnPresent;
    /** access point name - APN - we need to send this back to upper layer task
     * in case it was changed by the SIM
     */
    AccessPointName           apn;
    /** IP Address, DNS and Gateway addresses */
    PdnConnectionAddressInfo  pdnConnectionAddressInfo;
    /** Negotiated QoS present
\assoc PRESENT \ref qos */
    Boolean                   qosPresent;
    /** Negotiated QoS */
    QualityOfService          qos;
    /** Maximum bit rates present for APN
\assoc PRESENT \ref apnAmbr */
    Boolean                   apnAmbrPresent;
    /** Maximum bit rates for APN */
    ApnAmbr                   apnAmbr;
    /** Traffic Flow Template present
\assoc PRESENT \ref tft */
    Boolean                   tftPresent;
    /** Traffic Flow Template */
    TrafficFlowTemplate       tft;
    /** Whether this context is a secondary context (2G/3G) / dedicated bearer for LTE */
    Boolean                   secondaryContext;
    /** Associated primary PDP context / default EPS bearer connection ID if this is a
     * secondary PDP context / dedicated EPS bearer */
    Int8                      primaryConnId;
    /** Specifies the flow control mechanism */
    FlowCtrlType              flowControlType;
    /** Generic ABPD cause for PSD connection activation */
    AbpdPsdCause              abpdCause;

    /** APN Rate Control Parameters */
    Boolean                   apnRateControlPresent;
    /** Additional exception reports at maximum rate reached allowed or not */
    /* \assoc PRESENT \ref apnRateControlAdditionalExceptionReportsAllowed */
    Boolean                   apnRateControlAdditionalExceptionReportsAllowed;
    /** Uplink time unit */
    /* \assoc PRESENT \ref apnRateControluplinkTimeUnit */
    Int8                      apnRateControluplinkTimeUnit;
    /** Maximum uplink rate */
    /* \assoc PRESENT \ref apnRateControlMaxUplinkRate */
    Int32                     apnRateControlMaxUplinkRate;
    /** Serving PLMN Rate Control Value Present */
    Boolean                   plmnRateControlPresent;
    /** Serving PLMN Rate Control Value (3GPP TS 24.301, 9.9.4.28) */
    /* \assoc PRESENT \ref plmnRateControlValue */
    Int16                     plmnRateControlValue;
    /** IPv4 Link MTU  Present */
    Boolean                  ipv4MTUPresent;
    /** IPv4 Link MTU (3GPP TS 24.008 10.5.6.3) */
    Int16                     ipv4LinkMTU;
     /** Non IP Link MTU  Present */
    Boolean                   nonIPMTUPresent;
     /** Non-IP-Link MTU (3GPP TS 24.008 10.5.6.3) */
    Int16                     nonIPLinkMTU;
}
ApexAbpdPsdBearerStatusInd;

/** Indicates a PDP context (2G/3G) or EPS bearer (LTE) has been deactivated.
 * This signal is sent by ABPD to the ATCI or TCP/IP task depending on the owner
 * of the PSD connection.
 */
typedef struct ApexAbpdDisconnectedIndTag
{
   /** Connection ID: Identifies the PDP context / EPS bearer which has
    * been deactivated.
    */
  Int8             connId;
  /** Generic ABPD cause for deactivation */
  AbpdPsdCause     abpdCause;
  /** 3GPP Cause present
\assoc PRESENT \ref cause */
  Boolean          causePresent;
  /** Determines the 3GPP cause for deactivation */
  GsmCause         cause;
  /** Indicates whether release was initiated by the UE. */
  Boolean          isUEInitiated;
} ApexAbpdDisconnectedInd;

/** Signal sent by ABPD to ATCI or TCP/IP task to indicate that the connection
 * for the session cannot be made due to network congestion. When the ATCI task receives
 * this signal it sends a BUSY response code to the PC. There is no response
 * to this signal.
 */
typedef struct ApexAbpdBusyIndTag
{
  /** Connection ID */
  Int8             connId;
  /** Generic ABPD cause for Busy indication */
  AbpdPsdCause     abpdCause;
  /** 3GPP Cause present
\assoc PRESENT \ref cause */
  Boolean          causePresent;
  /** Determines the cause for busy indication */
  GsmCause         cause;
}
ApexAbpdBusyInd;

/** This signal is sent by ABPD to ATCI or TCP/IP to indicate that there is
 * an error in the ApexAbpdDialReq signal. When the ATCI task receives
 * this signal the cause field is reported to the PC as an error
 * result code. There is no response to this signal.
 */
typedef struct ApexAbpdErrorIndTag
{
  /** Connection ID */
  Int8             connId;
  /** Generic ABPD cause for Erro indication */
  AbpdPsdCause     abpdCause;
  /** 3GPP Cause present
\assoc PRESENT \ref cause */
  Boolean          causePresent;
  /** The cause of error */
  GsmCause         cause;
}
ApexAbpdErrorInd;

/** This signal is sent by TCP/IP or ATCI tasks to disconnect a PSD session
 * (PDP context for 2G/3G or EPS bearer for LTE).
 */
typedef struct ApexAbpdHangupReqTag
{
  /** Connection ID */
  Int8             connId;
}
ApexAbpdHangupReq;

/** This signal is sent by ATCI to ABPD to request a modification to
 * a PSD connection (PDP context for 2G/3G or EPS bearer for LTE).
 */
typedef struct ApexAbpdPsdModifyReqTag
{
  /** Connection ID */
  Int8                  connId;
  /** Minimum QoS present
\assoc PRESENT \ref minimumQos */
  Boolean               minimumQosPresent;
  /** Minimum QoS (only required for 2G/3G) */
  QualityOfService      minimumQos;
  /** Requested QoS present
\assoc PRESENT \ref qos */
  Boolean               qosPresent;
  /** Requested QoS */
  QualityOfService      qos;
  /** Traffic Flow Template present
\assoc PRESENT \ref tft */
  Boolean               tftPresent;
  /** Traffic Flow Template */
  TrafficFlowTemplate   tft;
}
ApexAbpdPsdModifyReq;

/** This signal is sent by ABPD to ATCI to indicate a modification to
 * a PSD connection has now completed (PDP context for 2G/3G or EPS bearer
 * for LTE).
 */
typedef struct ApexAbpdPsdModifyCnfTag
{
  /** Connection ID */
  Int8                  connId;
  /** Requested QoS present
\assoc PRESENT \ref qos */
  Boolean               qosPresent;
  /** Requested QoS */
  QualityOfService      qos;
  /** Traffic Flow Template present
\assoc PRESENT \ref tft */
  Boolean               tftPresent;
  /** Traffic Flow Template */
  TrafficFlowTemplate   tft;
}
ApexAbpdPsdModifyCnf;

/** This signal is sent by ABPD to ATCI to indicate that a request to
 * modify a PSD connection has been rejected by the network.
 */
typedef struct ApexAbpdPsdModifyRejTag
{
  /** Connection ID */
  Int8             connId;
  /** Generic ABPD cause for modification reject */
  AbpdPsdCause     abpdCause;
  /** 3GPP Cause present
\assoc PRESENT \ref cause */
  Boolean          causePresent;
  /** The cause of the modify reject */
  GsmCause         cause;
}
ApexAbpdPsdModifyRej;

/** This signal is sent by ABPD to ATCI to indicate a modification to
 * a PSD connection has been requested by the network (PDP context for 2G/3G or
 * EPS bearer for LTE).
 */
typedef struct ApexAbpdPsdModifyIndTag
{
  /** Connection ID */
  Int8                  connId;
  /** Requested QoS present
\assoc PRESENT \ref qos */
  Boolean               qosPresent;
  /** Requested QoS */
  QualityOfService      qos;
  /** Traffic Flow Template present
\assoc PRESENT \ref tft */
  Boolean               tftPresent;
  /** Traffic Flow Template */
  TrafficFlowTemplate   tft;
}
ApexAbpdPsdModifyInd;


/** Indication from ABPD to ATCI of MT PDP Context activation (2G/3G only).
 * Note that MT PDP Context activation can only occur for Primary
 * PDP contexts.
 */
typedef struct ApexAbpdSetupIndTag
{
  /** Connection ID */
  Int8             connId;
  /** PSD Bearer ID (NSAPI for 2G/3G, EPS bearer ID for LTE) */
  Int8             psdBearerId;
  /** PDN address for the PSD connection */
  PdnAddress       pdnAddress;
  /** Indicates if APN is present. */
  Boolean          apnPresent;
  /** APN to be used for new PSD connection */
  AccessPointName  apn;
}
ApexAbpdSetupInd;

/** Response from ATCI to ABPD to indicate if the MT PDP Context
 * has been accepted by ATCI (by answering the incoming call indication (ATA or
 * AT+CGANS)(2G/3G only) */
typedef struct ApexAbpdSetupRspTag
{

  /** Channel Number ATCI has associated with this MT PDP Context */
  Int32            channelNumber;
  /** Connection ID */
  Int8             connId;
  /** Connection Type */
  AbpdPdpConnType  connType;
  /** Is MT PDP context accepted */
  Boolean          connectionAccepted;
} ApexAbpdSetupRsp;

/** Request to perform a PSD attach. */
typedef struct ApexAbpdPsdAttachReqTag
{
  /** The sending task ID */
  TaskId    sourceTaskId;
}
ApexAbpdPsdAttachReq;

/** Request to detach from PSD service. */
typedef struct ApexAbpdPsdDetachReqTag
{
  /** The sending task ID */
  TaskId     sourceTaskId;
  /** Request combined detach (i.e. PS and CS service).  Note that for LTE if you
   * do a PS detach it will also cause a CS (IMSI) detach so this field it not
   * relevent for LTE.
   */
  Boolean    combinedDetach;
}
ApexAbpdPsdDetachReq;

/** This signal is sent to indicate the outcome of the attach request.
 * Confirmation signal associated with an ApexAbpdPsdAttachReq.
 * The success field is set to TRUE if the mobile becomes (or already is)
 * attached for PSD services. Following this signal the service type of ABMM
 * in NVRAM is updated. The mobile remains attached (even after PDP context
 * deactivation for 2G/3G, or PDN disconnect for LTE (apart from the
 * remaining PDN connection)) until an ApexAbpdPsdDetachReq is sent. This
 * condition persists across power cycles, so if GPRS_ATTACH_AT_POWER_ON is
 * defined the mobile will attach at power on.
 * Note that for LTE, the remaining PDN connection cannot be disconnected until
 * a detach from PSD services is performed.  At this point the PDN connection
 * will be disconnected.
 */
typedef struct ApexAbpdPsdAttachCnfTag
{
  /** Indicates if attach request was successful. */
  Boolean    success;
}
ApexAbpdPsdAttachCnf;

/** This signal is sent to indicate the outcome of the detach request.
 * Confirmation signal associated with an ApexAbpdPsdDetachReq.
 * The success field is set to TRUE if the mobile becomes (or already is)
 * detached from PSD services. Following this signal the ABMM service type in NVRAM
 * is updated. The mobile remains detached until an ApexAbpdPsdAttachReq
 * is sent (or a PDP context for 2G/3G or PDN connection for LTE is activated
 * in which case the mobile will attach for PSD services and then detach again
 * after the PDP context is deactivated (for 2G/3G only)).
 * This condition persists across power cycles, so even if GPRS_ATTACH_AT_POWER_ON
 * is defined the mobile will not attach at power on until ApexAbpdPsdAttachReq
 * (or PDP context / PDN connection activation).
 */
typedef struct ApexAbpdPsdDetachCnfTag
{
  /** Indicates if detach request was successful. */
  Boolean    success;
}
ApexAbpdPsdDetachCnf;

/** Request to display the APNs cached in ABPD.
 * This signal is used to display the APNs cached in ABPD.
 * (The APNs are read from the USIM at initialisation, and cached in ABPD)
 * \note Startfield can take the value 0 (in which case the
 * ApexAbpdListAclCnf will return the first five APNs in the cache).
 * If there are more than 5 APNs in the cache, several ApexAbpdListAclReq
 * should be sent to get the whole list.
 */
typedef struct ApexAbpdListAclReqTag
{
  /** The requesting task Id */
  TaskId            taskId;
  /** Command reference */
  Int16             commandRef;
  /** Specifies the first APN we should read
   * (starts from position 0). */
  Int8              startField;
}
ApexAbpdListAclReq;

/** Confirmation of the success or failure of the request.
 * Returns the list of APNs stored in the cache. (The cache mirrors
 * what is on the USIM) Returns up to 5 APNs at a time).
 */
typedef struct ApexAbpdListAclCnfTag
{
  /** Command reference copied from the corresponding request */
  Int16             commandRef;
  /** Outcome of the request */
  AbpdRequestStatus requestStatus;
  /** Copied from the request */
  Int8              startField;
  /** Access point names control list*/
  AbpdApnList       apnList;
}
ApexAbpdListAclCnf;

/** Request to update the ACL on the USIM.
 * This signal can be used to update the ACL on the USIM (stored in EF ACL file).
 * It can: <UL>
 * <LI> insert a new APN in the ACL at the end of the list. (mode = ABPD_ACL_WRITE_FIRST_FREE)
 * <LI> to overwrite an existing APN at a specific position. (mode = ABPD_ACL_WRITE_ABSOLUTE)
 * <LI> to insert a new APN in the list at a specific position.  (mode = ABPD_ACL_WRITE_INSERT)
 * </UL>
 * \note To add the Network provided APN, the length of the APN should be set to 0.
 *
 * If the length is set to 0, the AB will assume that the Network provided APN
 * should be written to the USIM.
 *
 * \note position parameter can take the value 0. If set to 0, the APN will be
 * inserted in the first position (except if mode = ABPD_ACL_WRITE_FIRST_FREE.
 * In this case, the position field is not relevant).
 * The APN to write should be in the textual form, with dots to separate each label.
 */
typedef struct ApexAbpdWriteAclReqTag
{
  /** The requesting task Id */
  TaskId                 taskId;
  /** Command reference */
  Int16                  commandRef;
  /** Indicates how to update the ACL */
  AbpdAclWriteMode       mode;
  /** Starts from position 0*/
  Int8                   position;
  /** APN to write (To add the Network provided APN to the APN
   * Control List, the length of the APN should be set to 0).
   */
  TextualAccessPointName apn;
}
ApexAbpdWriteAclReq;

/** Specifies whether the APN was written successfully. */
typedef struct ApexAbpdWriteAclCnfTag
{
  /** Command reference copied from ApexAbpdWriteAclReq */
  Int16                 commandRef;
  /** Outcome of the request.
   * If there was no space to store the APN, the requestStatus
   * will be set to ABPD_REQ_MEMORY_PROBLEM.
   *
   * If PIN2 has not been verified, requestStatus will be set to
   * ABPD_REQ_ACCESS_DENIED.
   */
  AbpdRequestStatus     requestStatus;
  /** copied from the request */
  Int8                  position;
  /** Specifies the total number of APNs in the ACL after the new record was added.*/
  Int8                  numApns;
}
ApexAbpdWriteAclCnf;

/** Request to delete an APN from the ACL on the USIM.
 * This signal can be used to delete an APN from the ACL on the USIM.
 * It can: <UL>
 * <LI> Delete the first APN in the list (mode = ABPD_ACL_DELETE_FIRST)
 * <LI> Delete the last APN in the list (mode = ABPD_ACL_DELETE_LAST)
 * <LI> Delete an APN at a specific position  (mode = ABPD_ACL_DELETE_ABSOLUTE)</UL>
 * \note When an APN is deleted, there is no empty record left at the middle
 * of the list, the end of the list gets shifted, so if APN 7 is deleted,
 * APN 8 will then become APN7, APN 9 will become APN 8, and so on.
 * \note According to 31.121, section 9.2, there should always be at least
 * one APN in the ACL.  Therefore ABPD will  prevent the user from deleting
 * all the APNs in the list.
 *
 * If the list contains only one APN, and the user tries to delete this last
 * APN, ABPD will return ABPD_REQ_NOT_ALLOWED to indicate that the operation is not allowed.
 * \note position  can take the value 0.  It is only relevant if mode = ABPD_ACL_DELETE_ABSOLUTE.
 */
typedef struct ApexAbpdDeleteAclReqTag
{
  /** The sending task Id */
  TaskId                 taskId;
  /** Command reference */
  Int16                  commandRef;
  /** Indicates how to delete the ACL */
  AbpdAclDeleteMode      mode;
  /** Starts from position 0*/
  Int8                   position;
}
ApexAbpdDeleteAclReq;

/** Specifies whether the APN was deleted successfully.
 * This signal is sent as a response to a request to delete
 * the APN.
 * \note PIN2 must be verified to delete an APN from the ACL.
 * There should always be at least one APN in the list according
 * to 31.121, section 9.2.
 */
typedef struct ApexAbpdDeleteAclCnfTag
{
  /** Command reference copied from the corresponing request */
  Int16                   commandRef;
  /** Whether or not the request was successful */
  AbpdRequestStatus       requestStatus;
  /** Copied from the request */
  Int8                    position;
  /** Specifies the total number of APNs in the ACL after
   * the new record was deleted.
   */
  Int8                    numApns;
}
ApexAbpdDeleteAclCnf;

/** Sets ACL status to enabled or disabled.
 * This signal can be used to enable/disable the ACL on the USIM.
 * This will set EF EST appropriately (see 31.102 section 4.2.47
 * for more information).
 */
typedef struct ApexAbpdSetAclReqTag
{
  /** The sending task Id */
  TaskId                 taskId;
  /** Command reference */
  Int16                  commandRef;
  /** Whether or not the ACL to be enabled/disabled */
  Boolean                enable;
}
ApexAbpdSetAclReq;

/** Specifies whether the ACL was enabled/disabled successfully. */
typedef struct ApexAbpdSetAclCnfTag
{
  /** Command reference copied from the corresponing request */
  Int16                   commandRef;
   /** Whether or not the request was successful */
  AbpdRequestStatus       requestStatus;
  /** Whether or not the ACL was enabled/disabled */
  Boolean                 enabled;
}
ApexAbpdSetAclCnf;

/** This signal can be used to query the status of the ACL on the USIM. */
typedef struct ApexAbpdAclStatusReqTag
{
  /** The sending task Id */
  TaskId                 taskId;
  /** Command reference */
  Int16                  commandRef;
}
ApexAbpdAclStatusReq;

/** Specifies whether the ACL is available on the USIM and whether it is enabled. */
typedef struct ApexAbpdAclStatusCnfTag
{
  /** Command reference copied from the corresponing request */
  Int16                   commandRef;
  /** Whether the ACL is enabled */
  Boolean                 enabled;
   /** Whether the ACL is available */
  Boolean                 available;
}
ApexAbpdAclStatusCnf;

/** Request to read an APN.
 * The ApexAbpdApnReadReq signal is used to read a record from
 * the APN store. The recordNumber runs from 1 to the maximum
 * number of records available inclusively. The mode field enables
 * the caller to specify that the first, next, previous, last or
 * absolute record should be read with respect to recordNumber.
 * For LTE, note that recordNumber set to DEFAULT_PDN_CONN_RECORD will
 * read from NVRAM the default EPS bearer settings used during the LTE attach
 * procedure.
 */
typedef struct ApexAbpdApnReadReqTag
{
  /** Sending task ID */
  TaskId            taskId;
  /** Command reference which is mirrored in cnf signal */
  Int16             commandRef;
}
ApexAbpdApnReadReq;

/** The ApexAbpdApnReadCnf  is used by the AB to return the APN details.
 * This signal is sent as a response to ApexAbpdApnReadReq to
 * read a record from the APN store.
 * For LTE, note that recordNumber set to DEFAULT_PDN_CONN_RECORD will
 * read from NVRAM the default EPS bearer settings used during the LTE attach
 * procedure.
 */
typedef struct ApexAbpdApnReadCnfTag
{
  /** Command reference copied from corresponding request */
  Int16             commandRef;
  /** Indicates the outcome of the request to read a record from APN */
  AbpdRequestStatus requestStatus;
  /** Descriptive APN */
  AbpdApn           apnDesc;
}
ApexAbpdApnReadCnf;

/** Request to write APN data.
 * The ApexAbpdApnWriteReq is used to write APN data to the APN storage area.
 * This may be used to modify an existing entry or create a new entry.
 * For LTE, note that recordNumber set to DEFAULT_PDN_CONN_RECORD will
 * write to NVRAM the default EPS bearer settings used during the LTE attach
 * procedure.
 */
typedef struct ApexAbpdApnWriteReqTag
{
  /** The sending task ID */
  TaskId            taskId;
  /** Command reference mirrored in the corresponding cnf signal */
  Int16             commandRef;
  /** Descriptive APN */
  AbpdApn           apnDesc;
}
ApexAbpdApnWriteReq;

/** Determines the outcome of writing an APN record.
 * The ApexAbpdApnWriteCnf signal is used by the AB to signal the result
 * of writing an APN record.
 */
typedef struct ApexAbpdApnWriteCnfTag
{
  /** Copied from ApexAbpdApnWriteReq */
  Int16             commandRef;
  /** Indicates the result of writing an APN */
  AbpdRequestStatus requestStatus;
}
ApexAbpdApnWriteCnf;

/** Informs all registered tasks of a STK PSD call control event.
 */
typedef struct ApexAbpdStkInfoIndTag
{
  /** Sending task ID (TASK_BL_ID) */
  TaskId                taskId;
  /** SIM Toolkit Call Control Status */
  SimatCcStatus         ccStatus;
  /** If AlphaId is present
\assoc PRESENT \ref alphaId */
  Boolean               alphaIdPresent;
  /** Alpha ID */
  SimatAlphaIdentifier  alphaId;
}
ApexAbpdStkInfoInd;

/** This signal is sent in order to query the data volume counters. */
typedef struct ApexAbpdReportCounterReqTag
{
    /** Connection ID, INVALID_CONN_ID stands for querying all the data volume counters of active PDN */
    Int8                connId;
}
ApexAbpdReportCounterReq;

/** This signal is sent in order to inform ATCI whether the specified PDN is active or not. */
typedef struct ApexAbpdReportCounterCnfTag
{
    /** Connection ID */
    Int8                connId;
    /** If this associated PDN is active */
    Boolean             isActive;
}
ApexAbpdReportCounterCnf;

/** This signal is sent in order to reset the data volume counters. */
typedef struct ApexAbpdResetCounterReqTag
{
    /** Connection ID, INVALID_CONN_ID stands for resetting all the data volume counters of active PDN */
    Int8                connId;
}
ApexAbpdResetCounterReq;

/** This signal is used to report the current counter values. */
typedef struct ApexAbpdCounterIndTag
{
    /** UL data volume counters in KB */
    uint64_t            ulDataVolumeCounters;
    /** DL data volume counters in KB */
    uint64_t            dlDataVolumeCounters;
    /** pdnType, ATCI shall translate it to IP or NON-IP */
    PdnType             pdnType;
    /** Connection ID */
    Int8                connId;
}
ApexAbpdCounterInd;

/** This signal is sent from ATCI to ABPD to set UPDIR info.
 */
typedef struct ApexAbpdSetUpdirInfoReqTag
{
  /* Source IP address */
  PdnAddress  srcIpAddress;
  /* Destination IP address */
  PdnAddress  dstIpAddress;
  /** indicates if source port is present */
  Boolean     srcPortPresent;
  /** indicates if destination port is present */
  Boolean     dstPortPresent;
  /** Source port */
  Int16       srcPort;
  /** Destination port */
  Int16       dstPort;
  /** Offset in byte for message Id. Start from 0 */
  Int16       msgIdOffset;
  /** Size in byte for message Id. Range: 1 or 2 */
  Int8        msgIdSize;
}
ApexAbpdSetUpdirInfoReq;

/** This signal is sent from ATCI to ABPD to reset UPDIR info.
 */
typedef struct ApexAbpdResetUpdirInfoReqTag
{
    /** Pattern ID */
    Int8        patternId;
}
ApexAbpdResetUpdirInfoReq;

/** This signal is sent from ATCI to ABPD to confirm UPDIR info set request.
 */
typedef struct ApexAbpdSetUpdirInfoCnfTag
{
    /** indicates whether the UPDIR information has been set successfully */
    Boolean     isSuccess;
    /** Pattern ID */
    Int8        patternId;
}
ApexAbpdSetUpdirInfoCnf;

/** This signal is sent from ATCI to ABPD to report UPDIR info.
 */
typedef struct ApexAbpdUpdirIndTag
{
    /** 1-delivery success, 0-delivery fail */
    Int8        status;
    /** Pattern ID */
    Int8        patternId;
    /** Number of message Id reported together. Range: <= ABPD_MAX_NUM_PACKET_ID_REPORT_TOGETHER */
    Int8        msgIdNum;
    /** Message Id extracted from the UDP packet */
    Int16       msgId [ABPD_MAX_NUM_UPDIR_PACKET_ID_REPORT];
}
ApexAbpdUpdirInd;

#if defined (FEA_PPP)
/** Loopback request.
 * Sent from ATCI to ABPD to enable or disable PPP loopback for test purposes.
 * ABPD will then forward this request to the PPP task.
 */
typedef struct PppLoopbackReqTag
ApexAbpdPppLoopbackReq;

/** Sent from ABPD to ATCI to confirm the previous ApexAbpdPppLoopbackReq signal.
 * This is a forwarded response from the PPP task
 */
typedef struct PppLoopbackCnfTag
ApexAbpdPppLoopbackCnf;

/** Configuration request sent from ABPD to ABPD to change parameters for PPP timers.
 * ABPD will then forward this request to the PPP task.
 */
typedef struct PppConfigReqTag
ApexAbpdPppConfigReq;

/** This signal is sent as a response to ApexAbpdPppConfigReq.
 * This is forwarded from the response send to ABPD by the PPP task.
 */
typedef struct PppConfigCnfTag

ApexAbpdPppConfigCnf;

/** Sent from ABPD to PPP to configure the authentication mode for PSD connections.
 * ABPD will then forward this request to the PPP task.
 */
typedef struct PppConfigAuthReqTag
ApexAbpdPppConfigAuthReq;

/** Sent from PPP to ABPD in response to a ApexAbpdPppConfigAuthReq.
 * Confirmation of the success or failure of the request
 * to change the PPP authentication mode for PSD connections.
 * This is forwarded from the response send to ABPD by the PPP task.
 */
typedef struct PppConfigAuthCnfTag
ApexAbpdPppConfigAuthCnf;
#endif /* FEA_PPP */

/** Sent from ATCI to ABPD to request writing of Release Assistance Indication
 * information.
 */
typedef struct ApexAbpdWriteRelAssistReqTag
{
  /** Sending task ID (TASK_BL_ID) */
  TaskId                taskId;

  /** Release Assistance Indication information */
  AbpdRelAssistInformation  relAssistInformation;
}
ApexAbpdWriteRelAssistReq;

/** Sent from ABPD to ATCI to confirm the previous ApexAbpdWriteRelAssistReq
 * signal.
 */
typedef struct ApexAbpdWriteRelAssistCnfTag
{
    /** Empty signal */
    Int8                    empty;
}
ApexAbpdWriteRelAssistCnf;

/** Sent from ATCI to ABPD to request reading of Release Assistance Indication
 * information.
 */
typedef struct ApexAbpdReadRelAssistReqTag
{
  /** Sending task ID (TASK_BL_ID) */
  TaskId                taskId;
}
ApexAbpdReadRelAssistReq;

/** Sent from ABPD to ATCI to confirm the previous ApexAbpdReadRelAssistReq
 * signal.
 */
typedef struct ApexAbpdReadRelAssistCnfTag
{
  /** Release Assistance Indication information */
  AbpdRelAssistInformation  relAssistInformation;
}
ApexAbpdReadRelAssistCnf;

/** Sent from ATCI to ABPD to request writing of APN Data Type.
 *
 */
typedef struct ApexAbpdWriteApnDataTypeReqTag
{
  /** Sending task ID (TASK_BL_ID) */
  TaskId                    taskId;

  /** APN Data Type Entity */
  AbpdApnDataTypeEntity     apnDataTypeEntity [MAX_NUM_CONN_IDS];
}
ApexAbpdWriteApnDataTypeReq;

/** Sent from ABPD to ATCI to confirm the previous ApexAbpdWriteApnDataTypeReq
 * signal.
 */
typedef struct ApexAbpdWriteApnDataTypeCnfTag
{
  /** Empty signal */
  Int8                      empty;
}
ApexAbpdWriteApnDataTypeCnf;

/** Sent from ATCI to ABPD to request reading of APN Data Type.
 *
 */
typedef struct ApexAbpdReadApnDataTypeReqTag
{
  /** Sending task ID (TASK_BL_ID) */
  TaskId                    taskId;
}
ApexAbpdReadApnDataTypeReq;

/** Sent from ABPD to ATCI to confirm the previous ApexAbpdReadApnDataTypeReq
 * signal.
 */
typedef struct ApexAbpdReadApnDataTypeCnfTag
{
  /** APN Data Type Entity */
  AbpdApnDataTypeEntity     apnDataTypeEntity [MAX_NUM_CONN_IDS];
}
ApexAbpdReadApnDataTypeCnf;

/** Sent from ABPD to ATCI to configure MTU parameters.
 */
typedef struct ApexAbpdMtuIndTag
{
    /** Connection ID */
    Int8                      connId;
 /** IPv4 Link MTU  Present */
    Boolean                  ipv4MTUPresent;
    /** IPv4 Link MTU (3GPP TS 24.008 10.5.6.3) */
    Int16                     ipv4LinkMTU;
     /** Non IP Link MTU  Present */
    Boolean                   nonIPMTUPresent;
     /** Non-IP-Link MTU (3GPP TS 24.008 10.5.6.3) */
    Int16                     nonIPLinkMTU;
}
ApexAbpdMtuInd;


/** Sent from ABPD to ATCI to configure APN rate control parameters.
 *
 */
typedef struct ApexAbpdApnUlRateControlIndTag
{
    /** Connection ID */
    Int8                      connId;

    /** APN Rate Control Parameters */
    Boolean                   apnRateControlPresent;
    /** Additional exception reports at maximum rate reached allowed or not */
    /* \assoc PRESENT \ref apnRateControlAdditionalExceptionReportsAllowed */
    Boolean                   apnRateControlAdditionalExceptionReportsAllowed;
    /** Uplink time unit */
    /* \assoc PRESENT \ref apnRateControluplinkTimeUnit */
    Int8                      apnRateControluplinkTimeUnit;
    /** Maximum uplink rate */
    /* \assoc PRESENT \ref apnRateControlMaxUplinkRate */
    Int32                     apnRateControlMaxUplinkRate;
}
ApexAbpdApnUlRateControlInd;

/** Sent from ABPD to ATCI to configure serving PLMN rate control parameters.
 *
 */
typedef struct ApexAbpdPlmnUlRateControlIndTag
{
    /** Serving PLMN Rate Control Value Present */
    Boolean                   plmnRateControlPresent;
    /** Serving PLMN Rate Control Value (3GPP TS 24.301, 9.9.4.28) */
    /* \assoc PRESENT \ref plmnRateControlValue */
    Int16                     plmnRateControlValue;
}
ApexAbpdPlmnUlRateControlInd;


/** Sent from ABPD to ATCI to indicate that modem has started to
 *  discard uplink data packets for a particular APN.
 */
typedef struct ApexAbpdPacketDiscardIndTag
{
    /** Connection ID */
    Int8                      connId;

    /** indicates START (TRUE)/STOP (FALSE) discard restriction */
    Boolean                    status;
    /** The amount of time in seconds that the uplink packets will be
     *  discarded for. */

    /* \assoc PRESENT \ref discardTime */
    Int32                     discardTime;
}
ApexAbpdPacketDiscardInd;

/** Sent from ABPD to SIM AT to indicate status of
 *  a PDN connection setup for the Attach Default Bearer.
 */
typedef struct ApexAbpdAttachPdnConnIndTag
{
    /** Connection ID */
    Int8                      connId;

    /* \assoc PRESENT \ref connected */
    Boolean                   connected;
}
ApexAbpdAttachPdnConnInd;

/** Sent from ATCI to ABPD in order to transmit data over control plane.
*
*/
typedef struct ApexAbpdTransmitDataIndTag
{
    /** Connection ID */
    Int8                      connId;

    /** Length of user data */
    Int32                     userDataLength;

    /** Pointer to user data */
    uint8_t*                  userData;

    /** Indicates whether the user data that is transmitted is regular or exceptional */
    AbpdApnDataTypeEntity     ApnDataTypeEntity;

    /** Release Assistance Indication information */
    AbpdRelAssistInformation  relAssistInformation;
}
ApexAbpdTransmitDataInd;

#ifdef ENABLE_ABPD_UNIT_TEST
/**
 *
 */
typedef struct AbpdSetUnitTestModeReqTag
{
  /** APN Data Type Entity */
  AbpdUnitTestMode          abpdUnitTestMode;
  AbpdStubRetValues         abpdStubRetValues;
}
AbpdSetUnitTestModeReq;
#endif /* ENABLE_ABPD_UNIT_TEST */

/** @} */ /* End of SigApexAbpd group */
#endif

/* END OF FILE */
