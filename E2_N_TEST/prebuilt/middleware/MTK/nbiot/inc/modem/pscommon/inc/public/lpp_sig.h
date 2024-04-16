/******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2013
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*******************************************************************************/

/******************************************************************************
 * Filename:
 * --------------------------------------------------------
 *   LppLbsMsg.h

 *
 * Project:
 * --------------------------------------------------------
 *   LTE project
 *
 * Description:
 * --------------------------------------------------------
 *   LTE Positioning Protocol - LBS Message Structure Definition
 *
 * Author:
 * --------------------------------------------------------
 *   Doug Shih
 *
 * --------------------------------------------------------
 * $Log$
 *
 * 03 24 2017 jie-yu.wang
 * [LR11 MP5][LPPe][MOLY00236804] Interface check-in.
 *
 * 01 14 2015 roland.li
 * [MOLY00092193] [UMOLY][LPP]LPP R10R11 feature checkin
 * .
 *
 * 09 26 2014 roland.li
 * [MOLY00079521] [LPP][User Plane]OTDOA assistance data size reduction
 * .
 *
 *
 *****************************************************************************/

#ifndef _LPP_LBS_MSG_STRUCT_H
#define _LPP_LBS_MSG_STRUCT_H

//#include "kal_public_api.h"
//#include "kal_public_defs.h"
//#include "kal_general_types.h"
#include "nbiot_modem_common_config.h"

#ifdef FEA_LPP_SUPPORT

#if defined(__LPP_EXT_SUPPORT__)
/* LPP and LPPe common interface */
#include "lpp_lppe_def.h"
#include "lpp_lppe_struct.h"
#endif
#include "lbsSs_typ.h"
#include "nphy_sig.h"

//#if defined(__LPP_CP_SUPPORT__) || defined(__LPP_UP_SUPPORT__)

/*****************************************************************************
 * Macros Without Parameters
 *****************************************************************************/
#define LPP_PEER_OTDOA_MAX_NBR_CELL_FREQ_LAYER_NUM   3   /* inherit LPP_OTDOA_MAX_NBR_CELL_FREQ_LAYER_NUM */
#define LPP_PEER_OTDOA_MAX_NBR_CELL_LIST_NUM        24   /* inherit LPP_OTDOA_MAX_NBR_CELL_LIST_NUM */

#define LPP_PEER_ECID_MAX_MEASURED_CELL_INFO_NUM    32   /* inherit LPP_ECID_MAX_MEASURED_CELL_INFO_NUM */

#define LPP_PEER_MAX_BANDS                          64   /* inherit LPP_MAX_BANDS */
/* Define of peerNbrCellBitmap*/
#define LPP_PEER_OTDOA_NBR_CELL_BIT_EARFCNVALID              0x80
#define LPP_PEER_OTDOA_NBR_CELL_BIT_ISCPLENGTHEXTENDEDVALID  0x40
#define LPP_PEER_OTDOA_NBR_CELL_BIT_ISCPLENGTHEXTENDED       0x20
#define LPP_PEER_OTDOA_NBR_CELL_BIT_ISFOURANTPORTCONFIGVALID 0x10
#define LPP_PEER_OTDOA_NBR_CELL_BIT_ISFOURANTPORTCONFIG      0x08
#define LPP_PEER_OTDOA_NBR_CELL_BIT_PRSINFOVALID             0x04
#define LPP_PEER_OTDOA_NBR_CELL_BIT_SLOTNUMBEROFFSETVALID    0x02
#define LPP_PEER_OTDOA_NBR_CELL_BIT_PRSSUBFRAMEOFFSETVALID   0x01
/*****************************************************************************
 * Macros With Parameters
 *****************************************************************************/
/*Macros for peerNbrCellBitmap*/
#define LPP_PEER_OTDOA_NBR_CELL_GET_EARFCNVALID(pPeerNbr)              (((pPeerNbr)->peerNbrCellBitmap & LPP_PEER_OTDOA_NBR_CELL_BIT_EARFCNVALID             )? KAL_TRUE : KAL_FALSE)
#define LPP_PEER_OTDOA_NBR_CELL_GET_ISCPLENGTHEXTENDEDVALID(pPeerNbr)  (((pPeerNbr)->peerNbrCellBitmap & LPP_PEER_OTDOA_NBR_CELL_BIT_ISCPLENGTHEXTENDEDVALID )? KAL_TRUE : KAL_FALSE)
#define LPP_PEER_OTDOA_NBR_CELL_GET_ISCPLENGTHEXTENDED(pPeerNbr)       (((pPeerNbr)->peerNbrCellBitmap & LPP_PEER_OTDOA_NBR_CELL_BIT_ISCPLENGTHEXTENDED      )? KAL_TRUE : KAL_FALSE)
#define LPP_PEER_OTDOA_NBR_CELL_GET_ISFOURANTPORTCONFIGVALID(pPeerNbr) (((pPeerNbr)->peerNbrCellBitmap & LPP_PEER_OTDOA_NBR_CELL_BIT_ISFOURANTPORTCONFIGVALID)? KAL_TRUE : KAL_FALSE)
#define LPP_PEER_OTDOA_NBR_CELL_GET_ISFOURANTPORTCONFIG(pPeerNbr)      (((pPeerNbr)->peerNbrCellBitmap & LPP_PEER_OTDOA_NBR_CELL_BIT_ISFOURANTPORTCONFIG     )? KAL_TRUE : KAL_FALSE)
#define LPP_PEER_OTDOA_NBR_CELL_GET_PRSINFOVALID(pPeerNbr)             (((pPeerNbr)->peerNbrCellBitmap & LPP_PEER_OTDOA_NBR_CELL_BIT_PRSINFOVALID            )? KAL_TRUE : KAL_FALSE)
#define LPP_PEER_OTDOA_NBR_CELL_GET_SLOTNUMBEROFFSETVALID(pPeerNbr)    (((pPeerNbr)->peerNbrCellBitmap & LPP_PEER_OTDOA_NBR_CELL_BIT_SLOTNUMBEROFFSETVALID   )? KAL_TRUE : KAL_FALSE)
#define LPP_PEER_OTDOA_NBR_CELL_GET_PRSSUBFRAMEOFFSETVALID(pPeerNbr)   (((pPeerNbr)->peerNbrCellBitmap & LPP_PEER_OTDOA_NBR_CELL_BIT_PRSSUBFRAMEOFFSETVALID  )? KAL_TRUE : KAL_FALSE)

#define LPP_PEER_OTDOA_NBR_CELL_SET_EARFCNVALID(pPeerNbr)              ((pPeerNbr)->peerNbrCellBitmap |= LPP_PEER_OTDOA_NBR_CELL_BIT_EARFCNVALID             )
#define LPP_PEER_OTDOA_NBR_CELL_SET_ISCPLENGTHEXTENDEDVALID(pPeerNbr)  ((pPeerNbr)->peerNbrCellBitmap |= LPP_PEER_OTDOA_NBR_CELL_BIT_ISCPLENGTHEXTENDEDVALID )
#define LPP_PEER_OTDOA_NBR_CELL_SET_ISCPLENGTHEXTENDED(pPeerNbr)       ((pPeerNbr)->peerNbrCellBitmap |= LPP_PEER_OTDOA_NBR_CELL_BIT_ISCPLENGTHEXTENDED      )
#define LPP_PEER_OTDOA_NBR_CELL_SET_ISFOURANTPORTCONFIGVALID(pPeerNbr) ((pPeerNbr)->peerNbrCellBitmap |= LPP_PEER_OTDOA_NBR_CELL_BIT_ISFOURANTPORTCONFIGVALID)
#define LPP_PEER_OTDOA_NBR_CELL_SET_ISFOURANTPORTCONFIG(pPeerNbr)      ((pPeerNbr)->peerNbrCellBitmap |= LPP_PEER_OTDOA_NBR_CELL_BIT_ISFOURANTPORTCONFIG     )
#define LPP_PEER_OTDOA_NBR_CELL_SET_PRSINFOVALID(pPeerNbr)             ((pPeerNbr)->peerNbrCellBitmap |= LPP_PEER_OTDOA_NBR_CELL_BIT_PRSINFOVALID            )
#define LPP_PEER_OTDOA_NBR_CELL_SET_SLOTNUMBEROFFSETVALID(pPeerNbr)    ((pPeerNbr)->peerNbrCellBitmap |= LPP_PEER_OTDOA_NBR_CELL_BIT_SLOTNUMBEROFFSETVALID   )
#define LPP_PEER_OTDOA_NBR_CELL_SET_PRSSUBFRAMEOFFSETVALID(pPeerNbr)   ((pPeerNbr)->peerNbrCellBitmap |= LPP_PEER_OTDOA_NBR_CELL_BIT_PRSSUBFRAMEOFFSETVALID  )

#define LPP_PEER_OTDOA_NBR_CELL_CLEAR_EARFCNVALID(pPeerNbr)              ((pPeerNbr)->peerNbrCellBitmap &= ~LPP_PEER_OTDOA_NBR_CELL_BIT_EARFCNVALID             )
#define LPP_PEER_OTDOA_NBR_CELL_CLEAR_ISCPLENGTHEXTENDEDVALID(pPeerNbr)  ((pPeerNbr)->peerNbrCellBitmap &= ~LPP_PEER_OTDOA_NBR_CELL_BIT_ISCPLENGTHEXTENDEDVALID )
#define LPP_PEER_OTDOA_NBR_CELL_CLEAR_ISCPLENGTHEXTENDED(pPeerNbr)       ((pPeerNbr)->peerNbrCellBitmap &= ~LPP_PEER_OTDOA_NBR_CELL_BIT_ISCPLENGTHEXTENDED      )
#define LPP_PEER_OTDOA_NBR_CELL_CLEAR_ISFOURANTPORTCONFIGVALID(pPeerNbr) ((pPeerNbr)->peerNbrCellBitmap &= ~LPP_PEER_OTDOA_NBR_CELL_BIT_ISFOURANTPORTCONFIGVALID)
#define LPP_PEER_OTDOA_NBR_CELL_CLEAR_ISFOURANTPORTCONFIG(pPeerNbr)      ((pPeerNbr)->peerNbrCellBitmap &= ~LPP_PEER_OTDOA_NBR_CELL_BIT_ISFOURANTPORTCONFIG     )
#define LPP_PEER_OTDOA_NBR_CELL_CLEAR_PRSINFOVALID(pPeerNbr)             ((pPeerNbr)->peerNbrCellBitmap &= ~LPP_PEER_OTDOA_NBR_CELL_BIT_PRSINFOVALID            )
#define LPP_PEER_OTDOA_NBR_CELL_CLEAR_SLOTNUMBEROFFSETVALID(pPeerNbr)    ((pPeerNbr)->peerNbrCellBitmap &= ~LPP_PEER_OTDOA_NBR_CELL_BIT_SLOTNUMBEROFFSETVALID   )
#define LPP_PEER_OTDOA_NBR_CELL_CLEAR_PRSSUBFRAMEOFFSETVALID(pPeerNbr)   ((pPeerNbr)->peerNbrCellBitmap &= ~LPP_PEER_OTDOA_NBR_CELL_BIT_PRSSUBFRAMEOFFSETVALID  )


/*****************************************************************************
 * Enums without tags
 *****************************************************************************/


/*****************************************************************************
 * Type Definitions
 *****************************************************************************/
#if 0
/* LPP -> LBS */
/* This structure for MSG_ID_LPP_LBS_SESSION_START_IND */
typedef struct
{
} LppLbsSessionStartInd;



/* This structure for MSG_ID_LPP_LBS_SESSION_END_IND */
typedef struct
{
} LppLbsSessionEndInd;
#endif


typedef enum
{
    LPP_POS_METHOD_TYPE_NONE,
    LPP_POS_METHOD_TYPE_AGNSS,
    LPP_POS_METHOD_TYPE_OTDOA,
    LPP_POS_METHOD_TYPE_ECID,
    LPP_POS_METHOD_TYPE_LPPE
} lpp_pos_method_type_enum;


/* This structure for MSG_ID_LPP_LBS_POS_METHOD_IND */
typedef struct
{
    lpp_pos_method_type_enum  posMethod;
} LppLbsPosMethodInd;



/*---LPPe interface---*/
/*Assistance Data*/
#if 0
/* Original design*/
/* MSG_ID_LPP_LBS_EXT_POS_ASSIST_DATA_IND: transfer the assist data to LBS */  /* LPPe only */
typedef struct
{
    Int16                      transID;  /* binded LPP transactio ID of this AD */

    LppLppeCommonAd       common;


    /* each pos tech type AD within one session should be sent seperately */
    lpp_lppe_pos_type_enum          adType;
    union
    {
        LppLppeHaGnssAd  haGNSS;

        LppLppeWlanApAd  wlanAP;

        LppLppeSensorAd   sensor;

        LppLppeSrnAd      srn;

    } adData;
} LppLbsExtPosAssistDataInd;



/* MSG_ID_LPP_LBS_EXT_POS_ASSIST_DATA_CNF: LPP confirms the assist data requests from AP connectivity module */ /* LPPe only */
typedef struct
{
    /* when AP connectivity module raise a AD request, there is no LPP transaction existed yet, how do we confirm it? */
    Int16  transID;  /* confirmed binded LPP transactio ID of this AD request */
    Boolean    result;   /* indicate the LPP/LPPe can initiate an AD request to the network */
} LppLbsExtPosAssistDataCnf;

#endif

#if defined(__LPP_EXT_SUPPORT__)
/* MSG_ID_LPP_LBS_EXT_WLAN_POS_ASSIST_DATA_PROVIDE_IND: transfer the assist data to LBS*//* LPPe only */
typedef struct
{
    Boolean    result;   /* indicate the LPP/LPPe can initiate an AD request to the network */
    //LppLppeWlanApAd  wlanAP;

} LppLbsExtWlanPosAssistDataProvideInd;


/* MSG_ID_LPP_LBS_EXT_BT_POS_ASSIST_DATA_PROVIDE_IND: transfer the assist data to LBS*//* LPPe only */
typedef struct
{
    Boolean    result;   /* indicate the LPP/LPPe can initiate an AD request to the network */
    //LppLppeSrnAd      srn;

} LppLbsExtBtPosAssistDataProvideInd;


/* MSG_ID_LPP_LBS_EXT_SENSOR_POS_ASSIST_DATA_PROVIDE_IND: transfer the assist data to LBS*//* LPPe only */
typedef struct
{
    Boolean    result;   /* indicate the LPP/LPPe can initiate an AD request to the network */
    //LppLppeSensorAd   sensor;

} LppLbsExtSensorPosAssistDataProvideInd;

#endif

/*Location Information*/
/*Original design*/
#if 0
/* MSG_ID_LPP_LBS_EXT_POS_LOC_REQ: contain the location info request */
typedef struct
{
   /* note: for MA, use horizontalAccuracy to represent pseudorange error */
   Boolean   horizontalAccuracyValid;
   Int8  horizontalAccuracy;            /* range: 0-127, refer TS 23.032, K1 decoding formula */
   Int8  horizontalAccuracyConfidence;  /* range: 0-100, refer TS 23.032, direct map to percentage, zero indicate "no inforamation" */
   Boolean   verticalAccuracyValid;
   Int8  verticalAccuracy;              /* range: 0-127, refer TS 23.032, K3 decoding formula */
   Int8  verticalAccuracyConfidence;    /* range: 0-100, refer TS 23.032, direct map to percentage, zero indicate "no inforamation" */
   Int8  responseTime;                  /* range: 1-128 seconds */
   Boolean   velocityRequest;               /* network request velocityEstimate */
} LppLbsLocReqQos;



typedef struct
{
    Int16                           transID;       /* binded LPP transactio ID of this LI */

    Int8                            locSrcMethod;  /* specify which location source method(s) are requested, mapping to LOC_SRC_METHOD_* */

    /* QoS accuracy and response time should be handled in LPP or LPPe? */
    LppLbsLocReqQos           lppQoS;        /* response time and accuracy, extracted from LPP stack */


    /* common */
    LppLppeCommonPosLocReq   commonReq;

    /* HA GNSS */
    LppLppeHaGnssPosLocReq  haGNSSReq;

    /* wlanAP */
    LppLppeWlanApPosLocReq  wlanAPReq;

    /* sensor */
    LppLppeSensorPosLocReq   sensorReq;

    /* SRN */
    LppLppeSrnPosLocReq      srnReq;

} LppLbsExtPosLocReq;



/* MSG_ID_LPP_LBS_EXT_POS_LOC_REQ_ABORT: abort specified location info request */
typedef struct
{
    Int16  transID;            /* binded LPP transactio ID of this LI */
    Int8   locSrcMethodAbort;  /* specify which location source method(s) is aborted, mapping to LOC_SRC_METHOD_* */
} LppLbsExtPosLocReqAbort;

#endif

#if defined(__LPP_EXT_SUPPORT__)
/* MSG_ID_LPP_LBS_EXT_WLAN_START_IND: transfer the start message to LBS*//* LPPe only */
typedef struct
{
    LppLppeWlanMeasReq   wlanAPReq;

} LppLbsExtWlanStartInd;


/* MSG_ID_LPP_LBS_EXT_BT_START_IND: transfer the start message to LBS*//* LPPe only */
typedef struct
{
    LppLppeBtMeasReq   btReq;

} LppLbsExtBtStartInd;


/* MSG_ID_LPP_LBS_EXT_SENSOR_START_IND: transfer the start message to LBS*//* LPPe only */
typedef struct
{
    LppLppeSensorMeasReq   sensorReq;

} LppLbsExtSensorStartInd;


/* MSG_ID_LPP_LBS_EXT_NETWORK_START_IND: transfer the start message to LBS*//* LPPe only */
typedef struct
{
} LppLbsExtNetworkStartInd;


/* MSG_ID_LPP_LBS_EXT_WLAN_MEAS_QUERY_REQ: transfer the query message to LBS*//* LPPe only */
typedef struct
{
} LppLbsExtWlanMeasQueryReq;


/* MSG_ID_LPP_LBS_EXT_BT_MEAS_QUERY_REQ: transfer the query message to LBS*//* LPPe only */
typedef struct
{
} LppLbsExtBtMeasQueryReq;


/* MSG_ID_LPP_LBS_EXT_SENSOR_MEAS_QUERY_REQ: transfer the query message to LBS*//* LPPe only */
typedef struct
{
} LppLbsExtSensorMeasQueryReq;


/* MSG_ID_LPP_LBS_EXT_NETWORK_MEAS_QUERY_REQ: transfer the query message to LBS*//* LPPe only */
typedef struct
{
} LppLbsExtNetworkMeasQueryReq;


/* MSG_ID_LPP_LBS_EXT_IPADDRESS_MEAS_QUERY_REQ: transfer the query message to LBS*//* LPPe only */
typedef struct
{
} LppLbsExtIpaddressMeasQueryReq;



/* MSG_ID_LPP_LBS_EXT_WLAN_STOP_IND: transfer the stop message to LBS*//* LPPe only */
typedef struct
{
} LppLbsExtWlanStopInd;


/* MSG_ID_LPP_LBS_EXT_BT_STOP_IND: transfer the stop message to LBS*//* LPPe only */
typedef struct
{
} LppLbsExtBtStopInd;


/* MSG_ID_LPP_LBS_EXT_SENSOR_STOP_IND: transfer the stop message to LBS*//* LPPe only */
typedef struct
{
} LppLbsExtSensorStopInd;


/* MSG_ID_LPP_LBS_EXT_NETWORK_STOP_IND: transfer the stop message to LBS*//* LPPe only */
typedef struct
{
} LppLbsExtNetworkStopInd;

#endif

/*Capability*/
/*TBD: AP provide capabilities without request*/
#if 0
/* MSG_ID_LPP_LBS_EXT_CAPABILITY_REQ: solicited capability request to AP connectivity module */
typedef struct
{
    Int8  locSrcMethod;  /* specify which location source method(s) capability are requested, mapping to LOC_SRC_METHOD_* */
} LppLbsExtCapabilityReq;


/* LBS -> LPP */
/* This structure for MSG_ID_LPP_LBS_SESSION_END_REQ */
typedef struct
{
} LppLbsSessionEndReq;
#endif


/* This structure for MSG_ID_LPP_LBS_CAPABILITY_UPDATE_REQ */
typedef struct
{
    /* TBD: only allow change positioning method support capability */
    Boolean  agnssSupport;
    Boolean  otdoaSupport;
    Boolean  ecidSupport;
} LppLbsCapabilityUpdateReq;



/* This structure for MSG_ID_LPP_LBS_RESET_UE_STORED_POS_INFO_REQ */
typedef struct
{
    Boolean  resetAgnss;
    Boolean  resetOtdoa;
} LppLbsResetUeStoredPosInfoReq;



/*---LPPe Interface---*/
/*Assistance Data*/
/*Orignal design*/
#if 0
/* MSG_ID_LPP_LBS_EXT_POS_ASSIST_DATA_RSP: LBS indicates the relay AD message response */
typedef struct
{
    Int16  transID;  /* binded LPP transactio ID of this AD */
    Boolean    result;   /* indicate the relay result */
} LppLbsExtPosAssistDataRsp;



/* MSG_ID_LPP_LBS_EXT_POS_ASSIST_DATA_REQ: AP connectivity module requests to assist data request */
typedef struct
{
    /* when AP connectivity module raise a AD request, there is no LPP transaction existed yet, how do we select a transID? */
    Int16                          transID;  /* binded LPP transactio ID of this AD request */

    /* each pos tech type AD request within one session should be sent seperately */
    lpp_lppe_pos_type_enum              adType;
    union
    {
        LppLppeHaGnssAdReq  haGNSS;

        LppLppeWlanApAdReq  wlanAP;

        lpp_lppe_sensor_ad_req_strcut   sensor;
        LppLppeSrnAdReq      srn;

    } adReq;
} LppLbsExtPosAssistDataReq;

#endif

#if defined(__LPP_EXT_SUPPORT__)
/* MSG_ID_LPP_LBS_EXT_WLAN_POS_ASSIST_DATA_REQUEST_IND: AP connectivity module requests to assist data request */
typedef struct
{
    void* ptr;
} LppLbsExtWlanPosAssistDataRequestInd;


/* MSG_ID_LPP_LBS_EXT_BT_POS_ASSIST_DATA_REQUEST_IND: AP connectivity module requests to assist data request */
typedef struct
{
    void* ptr;
} LppLbsExtBtPosAssistDataRequestInd;


/* MSG_ID_LPP_LBS_EXT_SENSOR_POS_ASSIST_DATA_REQUEST_IND: AP connectivity module requests to assist data request */
typedef struct
{
    void* ptr;
} LppLbsExtSensorPosAssistDataRequestInd;

#endif

/*Location Information*/
/*Orignal design*/
#if 0
/* MSG_ID_LPP_LBS_EXT_POS_LOC_CNF: provide location information of specified positioning methods */
typedef struct
{
    Int16                           transID;       /* binded LPP transactio ID of this LI */

    Int8                            locSrcMethod;  /* specify which location source method(s) are provided, mapping to LOC_SRC_METHOD_* */
    /* common */
    LppLppeCommonPosLocCnf   commonCnf;

    /* HA GNSS */
    LppLppeHaGnssPosLocCnf  haGNSSCnf;

    /* WLAN AP */
    LppLppeWlanApPosLocCnf  wlanAPCnf;

    /* sensor */
    LppLppeSensorPosLocCnf   sensorCnf;

    /* SRN */
    LppLppeSrnPosLocCnf      srnCnf;

} LppLbsExtPosLocCnf;

#endif
#if defined(__LPP_EXT_SUPPORT__)
/* MSG_ID_LPP_LBS_EXT_WLAN_MEAS_QUERY_CNF: provide location information of specified positioning methods */
typedef struct
{
    LppLppeWlanMeas  wlanAPCnf;

} LppLbsExtWlanMeasQueryCnf;


/* MSG_ID_LPP_LBS_EXT_BT_MEAS_QUERY_CNF: provide location information of specified positioning methods */
typedef struct
{
    LppLppeBtMeas  btCnf;

} LppLbsExtBtMeasQueryCnf;


/* MSG_ID_LPP_LBS_EXT_SENSOR_MEAS_QUERY_CNF: provide location information of specified positioning methods */
typedef struct
{
    LppLppeSensorMeas  sensorCnf;

} LppLbsExtSensorMeasQueryCnf;


/* MSG_ID_LPP_LBS_EXT_NETWORK_MEAS_QUERY_CNF: provide location information of specified positioning methods */
typedef struct
{
    LppLppeNetworkMeas  networkCnf;

} LppLbsExtNetworkMeasQueryCnf;


/* MSG_ID_LPP_LBS_EXT_IPADDRESS_MEAS_QUERY_CNF: provide location information of specified positioning methods */
typedef struct
{
    LppLppeIpaddressMeas  ipAddressCnf;

} LppLbsExtIpaddressMeasQueryCnf;


/*Capability*/
/*Original design*/
#if 0
/* MSG_ID_LPP_LBS_EXT_CAPABILITY_IND: unsolicited capability indications from AP connectivity module */
/* MSG_ID_LPP_LBS_EXT_CAPABILITY_CNF: solicited capability transfer from AP connectivity module */
typedef struct
{
    /* some feature can be hard-coded support in LPP? */
    /* common */
    //Int8                   ipAddrReport;              /* report the capabilities of a target to provide its local IP addresses, mapping to IP_ADDR_TYPE_BIT_IP_* */
    //Boolean                    relativeLocChangeReport;   /* TRUE if target supports to report relative change of location */
    //Boolean                    HAformatReport;
    //Boolean                    scheduleLocReport;
    //Boolean                    wirelessAccessTypeReport;

    Int8                   locSrcMethod;              /* specify which location source method(s) capability are provided, mapping to LOC_SRC_METHOD_* */
    /* HA GNSS */
    GnssHaCapability   haGNSSCapa;

    /* WLAN AP */
    WlanApCapability   wlanAPCapa;

    /* Sensor */
    SensorCapability    sensorCapa;

    /* SRN */
    SrnCapability       srnCapa;

} LppLbsExtCapabilityInd, lpp_lbs_ext_capability_cnf_struct;

#endif

/* MSG_ID_LPP_LBS_EXT_WLAN_CAPABILITY_IND: unsolicited capability indications from AP connectivity module */
typedef struct
{
    WlanApCapability   wlanAPCapa;

} LppLbsExtWlanCapabilityInd;


/* MSG_ID_LPP_LBS_EXT_BT_CAPABILITY_IND: unsolicited capability indications from AP connectivity module */
typedef struct
{
    BtCapability       btCapa;

} LppLbsExtBtCapabilityInd;


/* MSG_ID_LPP_LBS_EXT_SENSOR_CAPABILITY_IND: unsolicited capability indications from AP connectivity module */
typedef struct
{
   SensorCapability    sensorCapa;

} LppLbsExtSensorCapabilityInd;


/* MSG_ID_LPP_LBS_EXT_NETWORK_CAPABILITY_IND: unsolicited capability indications from AP connectivity module */
typedef struct
{
    Boolean  networkLocationSupported;
} LppLbsExtNetworkCapabilityInd;


/* MSG_ID_LPP_LBS_EXT_IPADDRESS_CAPABILITY_IND: unsolicited capability indications from AP connectivity module */
typedef struct
{
    IpaddressCapability  ipAddressCapa;

} LppLbsExtIpaddressCapabilityInd;

#endif

/* LPP (SWIP) -> LPP */
/* This structure for MSG_ID_LPP_PEER_OTDOA_MEAS_REQ */
typedef struct
{
    /* refer to LppEl1OtdoaMeasReq, for UP forward only */

    Int16  transactionID;
    Int8   responseTime;     /* [1..128], unit in seconds, for EL1 knowing about how long it will take for OTDOA measurement, but LPP still controls the response timer */
} LppPeerOtdoaMeasReq;



/* this structure is dedicated for UP peer forward */
typedef struct
{
   Int8   prsBandwidth;  /* map to lpp_otdoa_prs_info_bandwidth_enum */
   Int16   numDLFrames;   /* map to lpp_otdoa_prs_info_num_dl_frame_enum */
   Int8   prsMutingInfoValid; /* map to Boolean */
   Int8   prsMutingInfoType;  /* map to lpp_otdoa_prs_info_muting_po_enum */
   Int16  prsConfigIndex;     /* [0..4095] */
   Int16  prsMutingInfoBits;  /* Position Occasion: PO-2: 2 bits, PO-4: 4 bits, PO-8: 8 bits, PO-16: 16 bits */
} lpp_peer_otdoa_prs_info;


/* this structure is dedicated for UP peer forward */
typedef struct
{
    Int8                earfcnValid;               /* map to Boolean, FALSE: indicate this is a intra-freq cell, EL1 should use current serving cell's earfcn; TRUE: indicate this is a inter-freq cell */
    Int8                isFourAntPortConfigValid;  /* map to Boolean */
    Int8                isFourAntPortConfig;       /* map to Boolean, TRUE: 4 antenna ports for CRS are used in the assistance data reference cell, FALSE: 1 (or 2) antenna port(s) */
    Int8                isCPLengthExtended;        /* map to Boolean, TRUE: cyclic prefix length of PRS or CRS is extended, FALSE: cyclic prefix length is normal */
    Int8                prsInfoValid;              /* map to Boolean */
    Int16               phyCellId;                 /* [0..503] */
    Int32               earfcn;                    /* [0..262143], 65535 is invalid to indicate ARFCN-ValueEUTRA-v9a0 present; LPP assign reference cell if received message IE does not present */
    lpp_peer_otdoa_prs_info  prsInfo;
} LppPeerOtdoaRefCellInfo;



/* this structure is dedicated for UP peer forward */
typedef struct
{
    Int8                peerNbrCellBitmap;         /*earfcnValid; isCPLengthExtendedValid; isCPLengthExtended; isFourAntPortConfigValid; isFourAntPortConfig; prsInfoValid; slotNumberOffsetValid; prsSubFrameOffsetValid;*/
    Int8                slotNumberOffset;          /* [0..19] */
    Int16               phyCellId;                 /* [0..503] */
    Int16               prsSubFrameOffset;         /* [0..1279] */
    Int16               expectedRSTD;              /* [0..16383] */
    Int16               expectedRSTDUnc;           /* [0..1023] */
    Int32               earfcn;                    /* [0..262143], 65535 is invalid to indicate ARFCN-ValueEUTRA-v9a0 present; LPP assign reference cell if received message IE does not present */
    lpp_peer_otdoa_prs_info  prsInfo;
    
} LppPeerOtdoaNbrCellInfo;



/* this structure is dedicated for UP peer forward */
typedef struct
{
    Int8                            numCellInfo;
    LppPeerOtdoaNbrCellInfo  cellInfo[LPP_PEER_OTDOA_MAX_NBR_CELL_LIST_NUM];

} LppPeerOtdoaNbrCellInfoList;



/* this structure is dedicated for UP peer forward */
typedef struct
{
    Int8                                 numFreqInfo;
    LppPeerOtdoaNbrCellInfoList  freqInfo[LPP_PEER_OTDOA_MAX_NBR_CELL_FREQ_LAYER_NUM];

}  LppPeerOtdoaNbrFreqInfoList;



/* This structure for MSG_ID_LPP_PEER_OTDOA_ASSIST_DATA_REQ */
typedef struct
{
    /* refer to LppEl1OtdoaAssistDataReq, for UP forward only */
    Int16                     transactionID;
    NphyLppOtdoaAssistDataReq nphyLppOtdoaAssistData;

} LppPeerOtdoaAssistDataReq;

/* This structure for MSG_ID_LPP_PEER_OTDOA_MEAS_ABORT_REQ */
typedef struct
{
    /* refer to LppEl1OtdoaMeasAbortReq, for UP forward only */

    Int16  transactionID;
} LppPeerOtdoaMeasAbortReq;



/* This structure for MSG_ID_LPP_PEER_ECID_MEAS_REQ */
typedef struct
{
    /* refer to LppErrcEcidMeasReq, for UP forward only */

    Int16  transactionID;
    Int8   rsrpReq;            /* map to Boolean */
    Int8   rsrqReq;            /* map to Boolean */
    Int8   ueRxTxTimeDiffReq;  /* map to Boolean */
    Int16  MeasureTime;
} LppPeerEcidMeasReq;



/* This structure for MSG_ID_LPP_PEER_ECID_MEAS_ABORT_REQ */
typedef struct
{
    /* refer to LppErrcEcidAbortReq, for UP forward only */

    Int16  transactionID;
} LppPeerEcidMeasAbortReq;



/* LPP -> LPP (SWIP) */

/* this structure is dedicated for UP peer forward */
typedef struct
{
    Int8  errResolution;     /* 2 bits, refer description as above */
    Int8  errValue;          /* 5 bits, refer description as above */
    Int8  errSamplesValid;   /* map to Boolean */
    Int8  errSamples;        /* 3 bits, refer description as above */
} LppPeerOtdoaMeasuredQuality;



/* this structure is dedicated for UP peer forward */
typedef struct
{
    Int16                                      phyCellId;
    Int32                                      earfcn;        /* [0..262143], 65535 is invalid to indicate ARFCN-ValueEUTRA-v9a0 present */
    Int16                                      rstd;          /* [0..12711], mapping refer to TS 36.133 table 9.1.10.3-1 */
    LppPeerOtdoaMeasuredQuality     rstdQuality;

} LppPeerOtdoaMeasuredNbrCell;



/* this structure is dedicated for UP peer forward */
typedef struct
{
    Int16                                       sysFrameNumber;      /* 10 bits */
    Int16                                       refPhyCellId;        /* [0..503] */
    Int32                                       refEarfcn;           /* [0..262143], 65535 is invalid to indicate ARFCN-ValueEUTRA-v9a0 present */
    Int8                                        refQualityValid;     /* map to Boolean */
    LppPeerOtdoaMeasuredQuality      refQuality;

    Int8                                        numMeasuredNbrCell;
    LppPeerOtdoaMeasuredNbrCell     measuredNbrCell[LPP_PEER_OTDOA_MAX_NBR_CELL_LIST_NUM];

}  LppPeerOtdoaMeasuredInfo;



/* This structure for MSG_ID_LPP_PEER_OTDOA_MEAS_CNF */
typedef struct
{
    /* refer to LppEl1OtdoaMeasCnf, for UP forward only */

    Int16                           transactionID;
    LppPeerOtdoaMeasuredInfo  measurementResult;

} LppPeerOtdoaMeasCnf;



/* This structure for MSG_ID_LPP_PEER_OTDOA_ASSIST_DATA_CNF */
typedef struct
{
    /* refer to LppEl1OtdoaAssistDataCnf, for UP forward only */

    Int16  transactionID;
} LppPeerOtdoaAssistDataCnf;



/* This structure for MSG_ID_LPP_PEER_OTDOA_ADDITIONAL_ASSIST_DATA_REQ */
typedef struct
{
    Int16  transactionID;   /* assoicated measurement transaction ID */
    Int16  physCellId;      /* [0..503] */
} LppPeerOtdoaAdditionalAssistDataReq;  /* same as lpp_el1_otdoa_additional_assist_data_ind_struct */



typedef struct LbsPlmnIdTag{
    Int8 mcc1;
    Int8 mcc2;
    Int8 mcc3;
    Int8 mnc1;
    Int8 mnc2;
    Int8 mnc3;
} LbsPlmnId;



/* this structure is dedicated for UP peer forward */
typedef struct
{
    Int16          phyCellId;            /* [0..503] */
    Int8           plmnIdValid;          /* map to Boolean */
    LbsPlmnId  plmnId;

    Int8           cellIdValid;          /* map to Boolean */
    Int32          cellId;               /* eutra 28 bits */
    Int32          earfcn;               /* [0..262143], 65535 is invalid to indicate ARFCN-ValueEUTRA-v9a0 present */
    Int8           sysFrameNumberValid;  /* map to Boolean */
    Int16          sysFrameNumber;       /* 10 bits */
    /* measured result */
    Int8           rsrpResultValid;      /* map to Boolean */
    Int8           rsrpResult;           /* [0..97] */
    Int8           rsrqResultValid;      /* map to Boolean */
    Int8           rsrqResult;           /* [0..34] */
    Int8           ueRxTxTimeDiffValid;  /* map to Boolean */
    Int16          ueRxTxTimeDiff;       /* [0..4095] */
} LppPeerEcidCellMeasuredInfo;



/* this structure is dedicated for UP peer forward */
typedef struct
{
    /**
     * Please note that the interface here is more straight-forward then ECID-SignalMeasurementInformation IE of LPP Message:
     * when ERRC has measured primary cell, the result will place at primaryCellMeasuredResult;
     * (if serving (primary) cell is not present, the whole measured info should be ignored since result will be error)
     *
     * in addition to primary cell, if ERRC also measured neighbor cells, they will be put into the array of measuredCellResult[],
     * and the number of neighbor will specify in numMeasuredCellResult
     */
    LppPeerEcidCellMeasuredInfo  primaryCellMeasuredResult;

    Int8                                numMeasuredCellResult;
    LppPeerEcidCellMeasuredInfo  measuredCellResult[LPP_PEER_ECID_MAX_MEASURED_CELL_INFO_NUM];

} LppPeerEcidMeasuredInfo;



/* This structure for MSG_ID_LPP_PEER_ECID_MEAS_CNF */
typedef struct
{
    /* refer to LppErrcEcidMeasCnf, for UP forward only */

    Int16                          transactionID;
    Int8                           result;  /* map to lpp_loc_ecid_result_enum */
    LppPeerEcidMeasuredInfo  measurementResult;

} LppPeerEcidMeasCnf;


/* This structure for MSG_ID_LPP_PEER_EUTRAN_BAND_UPDATE_REQ */
#define  LppPeerEutranBandUpdateReq void



/* This structure for MSG_ID_LPP_PEER_EUTRAN_BAND_UPDATE_CNF */
typedef struct
{
    Int8   supportEutraBandListNum;
    Int16  supportEutraBandList[LPP_PEER_MAX_BANDS];
} LppPeerEutranBandUpdateCnf;


typedef struct
{
    Int16 DataLen;
    Int8 *pData;
}MuxLbsDataInd;
typedef struct
{
    Int16 DataLen;
    Int8 *pData;
}MuxLbsDataReq;

/**
allocated by the Application or the Protocol Stack
Used by CC and SS
*/
typedef Int16
CallReference;
#define     MAX_LCS_FACILITY_DATA_LENGTH         (249)


/** Used to aid the decoding of the Facility information element.
 * The SS version indicator is a type 4 information element with
 * one octet of the information element.
 * This is used in L3 messages.
 */
typedef struct LcsSsVersionIndicatorTag
{
    /** Represents information elements of SS version indicator */
    Int8                            information;
}
LcsSsVersionIndicatorElement,
/** SS version indicator struct that is used in MNXX signals */
LcsSsVersionIndicator;

/** Used to transport supplementary service related information.
 * The facility Holds coded facility information element data.
 * It is a type 4 information element with a minimum
 * length of 2 octets.
 * This is used in L3 messages.
 */
typedef struct LcsFacilityTag
{
    /** Length of facility contents
\assoc ARRAY \ref data */
    Int16                           dataLength;
    /** Facility information */
    Int8                            data[MAX_LCS_FACILITY_DATA_LENGTH];
}
LcsFacilityElement,
/** Facility struct that is used in MNXX signals */
LcsFacility;

typedef struct ssLcsBeginReqTag
{
   /** The call reference is provided by the Application Layer and must be between
     * zero and the number of tvs (the range is 0-6).
     */
    CallReference                   callReference;
    /** Transports supplementary service related information. */
    LcsFacility                        facility;
    /** Indicates if ss version indicator is present
\assoc PRESENT \ref ssVersionIndicatorOptional */
    Boolean                         ssVersionIndicatorPresent;
    /** This specifies the SS version used to encode and decode
     * Supplementary Services Facilities */
    LcsSsVersionIndicator              ssVersionIndicatorOptional;
}
SsLcsBeginReq;

typedef struct ApexSsLcsMOLRReqTag
{
   /** The sending task ID */
   /** This must correspond to a live invokeHandle associated 
    * with an outgoing invoke (If a matching invoke handle is not 
    * found then this signal is not generated)
    */
   InvokeHandle           moInvokeHandle;
   /** operationCode will be the same as in the originating invoke message */
   SsLcsCode                 ssCode;
   /** The LCS mobile originating location request data */
   SsLcsMOLRData          lcsMOLRData;
}
ApexSsLcsMOLRReq;

typedef struct LppLcsDataIndTag
{
    /** The call reference is provided by the Application Layer and must be between
     * zero and the number of tvs (the range is 0-6).
     */
    CallReference                   callReference;
    /** Transports supplementary service related information. */
    LcsFacility                        facility;
}
LppLcsDataInd;

typedef struct LppSuplDataIndTag
{
    Int16 dataLen;
    Int8 *pData;
}LppSuplDataInd,LppSuplDataReq;

#endif
//#endif  /* #if defined(__LPP_CP_SUPPORT__) || defined(__LPP_UP_SUPPORT__) */

#endif /* _LPP_LBS_MSG_STRUCT_H */
