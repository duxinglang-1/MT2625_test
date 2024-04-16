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
 *   LTE Positioning Protocol - Message Structure Definition
 *****************************************************************************/

#ifndef _LPP_EMM_H
#define _LPP_EMM_H

//#include "kal_public_api.h"
//#include "kal_public_defs.h"
//#include "kal_general_types.h"
#include "nbiot_modem_common_config.h"

#ifdef FEA_LPP_SUPPORT
#include <system.h>
/*Always trun on LPP CP options for L1 stand alone build*/
//#if defined(__LPP_CP_SUPPORT__) || defined(__LPP_UP_SUPPORT__) || defined(__L1EDPS_ENABLE__)

//#include "ps_public_struct.h"  /* for L4C SS struct */
//#include "as2l4c_struct.h"     /* for L4CPS NBR cell info */
//#include "mcd_l3_inc_struct.h" /* for PLMN id */
//#include "l3_inc_enums.h"      /* for LCS/EMM enum */

/*****************************************************************************
 * Macros Without Parameters
 *****************************************************************************/


/*****************************************************************************
 * Macros With Parameters
 *****************************************************************************/


/*****************************************************************************
 * Enums without tags
 *****************************************************************************/


/*****************************************************************************
 * Type Definitions
 *****************************************************************************/

/* LPP -> EMM (EVAL) */
/* This structure for MSG_ID_LPP_EVAL_DATA_REQ */
typedef struct
{
    Int16  mui;
    /* for Additional Information IE of UL generic NAS transport */
    Int8   addInfoLen;
    Int8   *addInfo;
    /* raw data is in peer buffer */
} LppEvalDataReq;



/* This structure for MSG_ID_LPP_EVAL_DATA_IND */
typedef struct
{
    Int16     DataLen;
    Int8      *pBuffer;
    /*        for Additional Information IE of DL generic NAS transport */
    Int8      addInfoLen;
    Int8      *addInfo;
    /* raw data is in peer buffer */
} LppEvalDataInd;


/* LCS (CISS/LPP) between EMM */
typedef enum
{
    LCS_EMM_STATUS_SUCCESS,            /* PDU is sent successfully */
    LCS_EMM_STATUS_ERR_CONN_FAIL,      /* EMM connection failure */
    LCS_EMM_STATUS_ERR_NW_NO_LCS,      /* NW does not support LCS on 2G/3G/4G */
    LCS_EMM_STATUS_ERR_CS_LCS_ONLY     /* NW does not support LCS on 4G, may CSFB to 2G/3G */
} lcs_emm_status_enum;

/* This structure for MSG_ID_LPP_EVAL_DATA_CNF */
typedef struct
{
    lcs_emm_status_enum  result;
    Int16           mui;
} LppEvalDataCnf;


#define EAS_MAX_MEASURED_CELL_NUM            32

typedef struct
{ 
    //EARFCN                              dl_freq;   
    Int16                          pci;        //0~503
    Int8                           rsrp;       //0~97
    Int8                           rsrq;       //0~34
    
    Int8                           is_cell_id_used;
    Int32                          cell_id;    //0~0x0FFFFFFF
    Int16                          mcc;        //0~999
    Int16                          mnc;        //0~999
    Int8                           is_2_digit_mnc; //ncell cell plmn is 2 digits or 3 digits    
    Int16                          tac;        //0~65535
} EasCellMeasuredResults;


/* LTE cell info */
typedef struct 
{
    /* Serving cell info */
    Int16                          mcc;            //0~999
    Int16                          mnc;            //0~999
    Int8                           is_2_digit_mnc; //serving cell plmn is 2 digits or 3 digits
    Int32                          cell_id;        //0~0x0FFFFFFF
    //EARFCN                              dl_freq;        
    Int8                           ul_freq_used;   
    //EARFCN                              ul_freq;        
    Int16                          pci;            //0~503
    Int8                           is_ta_used; 
    Int16                          time_advance;   //0~1282
    Int8                           is_rsrp_valid; 
    Int8                           rsrp;           //0~97
    Int8                           is_rsrq_valid; 
    Int8                           rsrq;           //0~34
    Int16                          tac;            //0~65535
 
    /* Neighbor cell info */   
    Int8                           num_nbr_cell;
    EasCellMeasuredResults    nbr_cell_list[EAS_MAX_MEASURED_CELL_NUM]; 

} EasNbrCellInfo;

/*-----EAS cell info struct end-------------*/

typedef union ps_nbr_cell_info
{
    EasNbrCellInfo eas_nbr_cell_info;

#if !defined(__UMTS_RAT__) && !defined(__LTE_RAT__)
    Int32 dummy_padding;
#endif
}ps_nbr_cell_info_union_type;

typedef struct L4cNbrCellInfoRegCnf

{
    Int8 rat_mode;
    Int8 is_nbr_info_valid;
    EasNbrCellInfo ps_nbr_cell_info_union;

} L4cNbrCellInfoRegCnf,L4cNbrCellInfoInd;



typedef enum {
    UE_POS_TECH_AGNSS = 0,
    UE_POS_TECH_OTDOA = 1
} ue_pos_tech_enum;
/* This structure for MSG_ID_EVAL_LPP_NW_RESET_UE_STORED_POS_INFO_IND */
typedef struct
{
    ue_pos_tech_enum  pos_tech;
} EvalLppNwResetUeStoredPosInfoInd;

//#endif  /* #if defined(__LPP_CP_SUPPORT__) || defined(__LPP_UP_SUPPORT__) || defined(__L1EDPS_ENABLE__) */
#endif

#endif /* _LPP_MSG_STRUCT_H */
