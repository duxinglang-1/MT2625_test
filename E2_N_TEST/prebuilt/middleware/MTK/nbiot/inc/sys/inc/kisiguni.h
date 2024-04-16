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
 **************************************************************************/
/** \file
 * GKI header file that defines the signal union of whole system, used by
 * host test tools only.
 **************************************************************************/

#ifndef KISIGUNI_H
#define KISIGUNI_H

/** \addtogroup PrdGki
 * @{
 */

/****************************************************************************
 * Macro constants
 ****************************************************************************/

/** Indicates files are being processed by a host test tool for signal information.
 * This define is used to indicate that the header files are being processed by
 * a host test tool to extract the signal information for the whole system.
 * This can be used in the target header files to prevent other signal unions
 * from being defined (or hiding other items that the tools do not need).
*******************************************************************************/
#if !defined (KI_TEST_TOOL_SIGNAL_INFO)
# define KI_TEST_TOOL_SIGNAL_INFO
#endif

/****************************************************************************
 * Nested Include Files
 ****************************************************************************/

#include      <system.h>
#include      <kernel.h>
#include      <pssignal.h>

/****************************************************************************
 * Signal Type Include Files
 ****************************************************************************/
#include      <kitvinc.h>

#if defined(UPGRADE_CORE_FR)
#include <fr_sig.h>
#endif

#if !defined (EXCLUDE_D1)
#if !defined (EXCLUDE_L1AL)
# include <l1al_sig.h>
#endif /* EXCLUDE_L1AL */
#if !defined (EXCLUDE_L1SI)
# include <l1si_sig.h>
#endif /* EXCLUDE_L1SI */
#if defined (UPGRADE_CTM)
# include <l1ctmsig.h>
#endif /* UPGRADE_CTM */
#endif /* EXCLUDE_D1 */

#if !defined (EXCLUDE_DM)
# include <dmpm_sig.h>
# include <dmnvp_sig.h>
#endif

#if !defined (EXCLUDE_L1UT)
# include <l1ut_sig.h>
#endif

#if !defined (EXCLUDE_GSM_PS_AS) || !defined (EXCLUDE_GSM_L1)
# include <l1tvinc.h>
#endif

#if !defined (EXCLUDE_GSM_PS)
# include <pstvinc.h>
#endif

#if defined (USE_TTP_AL)
# include <anrm2typ.h>
# include <anrm2sig.h>
# include <abtvinc.h>
#endif

#if defined (UPGRADE_TCP_IP)
# include <tcip_sig.h>
#endif  /* defined (UPGRADE_TCP_IP) */

#if defined (UPGRADE_WAP_PPP)
# include <wppp_sig.h>
#endif

#if !defined (EXCLUDE_VG)
# include <vgtvinc.h>
# include <vgetvinc.h>
#endif /* EXCLUDE_VG */

#if defined (UPGRADE_GENERIC_MUX)
#  include <cimux_sig.h>
#  include <csdmux_sig.h>
#  include <pppmux_sig.h>
#  include <muxmux_sig.h>
#if defined (UPGRADE_NASMDL2)
#  include <mdt_sig.h>
#  include <conn_sig.h>
#else
#  include <muxconn_sig.h>
#endif
#  include <pppadtest_sig.h>
#if defined (EMMI_OVER_GENERIC_MUX)
#  include <emmimux_sig.h>
#endif
#if defined (UPGRADE_SHARE_MEMORY)
#  include <shmconn_sig.h>
#elif defined (UPGRADE_SHMCL_SOLUTION)
#  include <shmconn_sig_pruned.h>
#  include <shm_nvramsig.h>
#  include <t1msyspara_sig.h>
#endif  /* UPGRADE_SHARE_MEMORY or  UPGRADE_SHMCL_SOLUTION */
#endif  /* UPGRADE_GENERIC_MUX */

#if defined(UPGRADE_LOGGING_OVER_SHM)
#  include <shm_logsig.h>
#endif  /*  UPGRADE_LOGGING_OVER_SHM */

# if !defined (EXCLUDE_GPRS_PS)
#  include <gptvinc.h>
# endif

#if !defined (EXCLUDE_UT)
# include <uttvinc.h>
#endif

#if  defined (UPGRADE_3G)
# if !defined (EXCLUDE_3G_PS)
#  include <upstvinc.h>
# endif
# if !defined (EXCLUDE_3G_PS) || !defined (EXCLUDE_3G_L1)
#  include <upsl1inc.h>
# endif
# if defined (EXCLUDE_3G_PS)
#  include <u1aptestinc.h>
# endif
# if defined (WL1_UPHY_STUB)
#  include <w1aptestinc.h>
# endif
# if !defined (EXCLUDE_3G_L1)
#  include <u1tvinc.h>
# endif
#if defined (USE_3G_FDD_L1_NET_SIMU)
#  include <nsinc.h>
#endif
#endif

#if defined (CONN_LAYER)
#include <conn_layer_sig.h>
#endif /* CONN_LAYER */

#if defined (UPGRADE_COOLART)
/* UPGRADE_COOLART should never be defined for an MS target build.
 * It is used exclusively for the testing on PC */
# include    <catvinc.h>
#endif /* UPGRADE_COOLART */

#if defined (USE_TREE_SIGNALS)
# if !defined (EXCLUDE_TREE)
#  include <treesigtypes.h>
# endif
#endif

#if defined (UPGRADE_RTSP)
#  include <rtsp_sig.h>
#endif

#if defined (UPGRADE_RTP)
#  include <rtp_sig.h>
#endif

#if defined (UPGRADE_AUDIO_MANAGER_V2)
# include <am_sig.h>
#endif

#if defined (UPGRADE_AUDIO_STREAM_MANAGER)
# include <au_strmgr_sig_pub.h>
#endif

#if defined (UPGRADE_MMAC)
# include <mmac_sig.h>
#endif

#if defined (UPGRADE_MES)
# if defined (UPGRADE_MPEG4_ENC)
#  include <m4e_api.h>
# endif
# include <mes_typ.h>
# include <mes_sig.h>
#endif

#if defined (UPGRADE_MDS)
# include <mds_sig.h>
#endif

#if defined (UPGRADE_MMSC) || defined (UPGRADE_MDS_STREAMING)
# include <mmsc_sig.h>
#endif

#if defined (UPGRADE_MULTIMEDIA_FRAMEWORK)
# include <mf_sig.h>
#endif

#if defined (UPGRADE_FSYSTEM)
#    include <fs_sig.h>
#endif /* (UPGRADE_FSYSTEM) */

#if defined (ENABLE_TELEPHONY_AUDIO_CONTROLLER)
#if defined (UPGRADE_GGE)
#include <cssgcsai_sig.h>
#endif /* defined (UPGRADE_GGE) */
#if defined (UPGRADE_3G)
#include <csscsdi_sig.h>
#endif /* defined (UPGRADE_3G) */
#include <cssau_sig.h>
#endif /* defined (ENABLE_TELEPHONY_AUDIO_CONTROLLER) */

#if defined (UPGRADE_LTE)
# include <e1tvinc.h>
#if !defined (EXCLUDE_LTE_PS)
# include <epstvinc.h>
#endif
#endif

#if defined (UPGRADE_NB)
# include <n1tvinc.h>
#endif

#if defined (NB_PC_HW_SIM)
#include <nb_pc_hw_sig.h>
#endif

#if defined (NB_PC_L1_NET_SIM)
#include <l1netsim_msgif.h>
#include <mns_n_airc_ue_intf.h>
#endif

#if defined (EXS_SIGNALS)
# include <exstvinc.h>
#endif

#include     <extvinc.h>

#include <frhsl_sig.h>
#if defined (FR_PRIMELOG_PRESENT)
#include <frprimelogts_sig.h>
#endif
#if defined (OSDRIVERS_TEST_ENABLE)
#include <osdriver_test_main_sig.h>
#include <osdriver_test_dma_sig.h>

#if defined (MEMORY_TEST_ENABLE)
#include <memory_test_sig.h>
#endif
#endif

#ifdef ENABLE_AP_BRIDGE_FEATURE
#include <ciapb_sig.h>
#endif

#if defined (KI_ENABLE_REF_DATA_LOGGING)
#include <kiref_sig.h>
#endif /* KI_ENABLE_REF_DATA_LOGGING */

#define      LARGEST_SIGNAL_SIZE        (10000)

/****************************************************************************
 * Signal Union definition
 ****************************************************************************/

/* This include causes the definition of the SIG_DEF macro to be changed
** such that it will generate entries in the Signal union. */
#define SIGNAL TVSIGNAL
#include      <kisigdef.h>

/** Union of all signal bodies.
 * The Signal union defines all the signal structures in the system. This
 * union is generated from the use of the SIG_DEF() macro in signal
 * definition files e.g. kisig.h. This union has to have exactly the same
 * number of members as the #SignalId enumeration in PSSIGNAL.H (the members
 * must also be in the same order). */
union Signal
{
#   include  <kisiginc.h>

    EmptySignal                     nonSignal;
    EmptySignal                     noMoreSignals;
};

#if !defined (GENIEXEC_C)
# include    <kitvcom.h>
#endif


#if defined(TTPCOM_DOX_COMPILING)
/* Pull in documentation files if parsing for Doxygen */
#ifdef LINUX
#include <../docs/doxgroups/system.dox>
#else
#include <..\docs\doxgroups\system.dox>
#endif
#ifdef LINUX
#include <../docs/doxgroups/includes.dox>
#else
#include <..\docs\doxgroups\includes.dox>
#endif
#endif

/** @} */ /* End of PrdGki group */

#endif
/* END OF FILE */
