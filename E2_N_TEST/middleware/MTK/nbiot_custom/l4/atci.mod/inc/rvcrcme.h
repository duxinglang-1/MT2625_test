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
 * File Description
 * ----------------
 *
 * RVCRCME.H
 * Cme error codes and strings for inclusion in the CR Manager.
 **************************************************************************/

#ifndef RVCRCME_H
#define RVCRCME_H

#if defined (UPGRADE_RAVEN_NO_VERBOSE)
#define RESULT_CODE_ENTRY(enumerate, numeric, text) \
{ enumerate, numeric }

#else
#define RESULT_CODE_ENTRY(enumerate, numeric, text) \
{ enumerate, numeric, (const Char*)text }
#endif

  /* result code */
  RESULT_CODE_ENTRY(  RESULT_CODE_OK,             0, "OK"          ),
  RESULT_CODE_ENTRY(  RESULT_CODE_CONNECT,        1, "CONNECT"     ),
  RESULT_CODE_ENTRY(  RESULT_CODE_RING,           2, "RING"        ),
  RESULT_CODE_ENTRY(  RESULT_CODE_NO_CARRIER,     3, "NO CARRIER"  ),
  RESULT_CODE_ENTRY(  RESULT_CODE_ERROR,          4, "ERROR"       ),
  RESULT_CODE_ENTRY(  RESULT_CODE_INV,            5, "CONNECT 1200"),
  RESULT_CODE_ENTRY(  RESULT_CODE_NO_DIALTONE,    6, "NO DIALTONE" ),
  RESULT_CODE_ENTRY(  RESULT_CODE_BUSY,           7, "BUSY"        ),
  RESULT_CODE_ENTRY(  RESULT_CODE_NO_ANSWER,      8, "NO ANSWER"   ),
  RESULT_CODE_ENTRY(  RESULT_CODE_PROCEEDING,     9, "PROCEEDING"  ),

  RESULT_CODE_ENTRY(  RESULT_CODE_NULL,           0, ""            ),
  RESULT_CODE_ENTRY(  RESULT_CODE_CUSTOM_CONNECT, 12,""            ),

  /* CME error codes */

  /* general errors */

  RESULT_CODE_ENTRY(  VG_CME_PHONE_FAILURE,                   0, "phone failure"                                 ),
  RESULT_CODE_ENTRY(  VG_CME_NO_MS_CONNECTION,                1, "no connection to phone"                        ),
  RESULT_CODE_ENTRY(  VG_CME_PHONE_LINK_RESERVED,             2, "phone-adaptor link reserved"                   ),
  RESULT_CODE_ENTRY(  VG_CME_OPERATION_NOT_ALLOWED,           3, "operation not allowed"                         ),
  RESULT_CODE_ENTRY(  VG_CME_OPERATION_NOT_SUPPORTED,         4, "operation not supported"                       ),
  RESULT_CODE_ENTRY(  VG_CME_PH_SIM_PIN_REQUIRED,             5, "PH-SIM PIN required"                           ),
  RESULT_CODE_ENTRY(  VG_CME_FSIM_PIN_REQUIRED,               6, "PH-FSIM PIN required"                          ),
  RESULT_CODE_ENTRY(  VG_CME_FSIM_PUK_REQUIRED,               7, "PH-FSIM PUK required"                          ),
  RESULT_CODE_ENTRY(  VG_CME_SIM_NOT_INSERTED,               10, "SIM not inserted"                              ),
  RESULT_CODE_ENTRY(  VG_CME_SIM_PIN_REQUIRED,               11, "SIM PIN required"                              ),
  RESULT_CODE_ENTRY(  VG_CME_SIM_PUK_REQUIRED,               12, "SIM PUK required"                              ),
  RESULT_CODE_ENTRY(  VG_CME_SIM_FAILURE,                    13, "SIM failure"                                   ),
  RESULT_CODE_ENTRY(  VG_CME_SIM_BUSY,                       14, "SIM busy"                                      ),
  RESULT_CODE_ENTRY(  VG_CME_SIM_WRONG,                      15, "SIM wrong"                                     ),
  RESULT_CODE_ENTRY(  VG_CME_INCORRECT_PASSWORD,             16, "incorrect password"                            ),
  RESULT_CODE_ENTRY(  VG_CME_SIM_PIN2_REQUIRED,              17, "SIM PIN2 required"                             ),
  RESULT_CODE_ENTRY(  VG_CME_SIM_PUK2_REQUIRED,              18, "SIM PUK2 required"                             ),
  RESULT_CODE_ENTRY(  VG_CME_MEMORY_FULL,                    20, "memory full"                                   ),
  RESULT_CODE_ENTRY(  VG_CME_INVALID_INDEX,                  21, "invalid index"                                 ),
  RESULT_CODE_ENTRY(  VG_CME_NOT_FOUND,                      22, "not found"                                     ),
  RESULT_CODE_ENTRY(  VG_CME_MEMORY_FAILURE,                 23, "memory failure"                                ),
  RESULT_CODE_ENTRY(  VG_CME_LONG_TEXT,                      24, "text string too long"                          ),
  RESULT_CODE_ENTRY(  VG_CME_INVALID_TEXT_CHARS,             25, "invalid characters in text string"             ),
  RESULT_CODE_ENTRY(  VG_CME_LONG_DIALSTRING,                26, "dial string too long"                          ),
  RESULT_CODE_ENTRY(  VG_CME_INVALID_DIALSTRING_CHARS,       27, "invalid characters in dial string"             ),
  RESULT_CODE_ENTRY(  VG_CME_NO_NETWORK_SERVICE,             30, "no network service"                            ),
  RESULT_CODE_ENTRY(  VG_CME_NETWORK_TIMEOUT,                31, "network timeout"                               ),
  RESULT_CODE_ENTRY(  VG_CME_EMERGENCY_ONLY,                 32, "network not allowed - emergency calls only"    ),
  RESULT_CODE_ENTRY(  VG_CME_NET_PIN_REQUIRED,               40, "network personalisation PIN required"          ),
  RESULT_CODE_ENTRY(  VG_CME_NET_PUK_REQUIRED,               41, "network personalisation PUK required"          ),
  RESULT_CODE_ENTRY(  VG_CME_NETSUB_PIN_REQUIRED,            42, "network subset personalisation PIN required"   ),
  RESULT_CODE_ENTRY(  VG_CME_NETSUB_PUK_REQUIRED,            43, "network subset personalisation PUK required"   ),
  RESULT_CODE_ENTRY(  VG_CME_NETPROV_PIN_REQUIRED,           44, "service provider personalisation PIN required" ),
  RESULT_CODE_ENTRY(  VG_CME_NETPROV_PUK_REQUIRED,           45, "service provider personalisation PUK required" ),
  RESULT_CODE_ENTRY(  VG_CME_CORPORATE_PIN_REQUIRED,         46, "corporate personalisation PIN required"        ),
  RESULT_CODE_ENTRY(  VG_CME_CORPORATE_PUK_REQUIRED,         47, "corporate personalisation PUK required"        ),
  RESULT_CODE_ENTRY(  VG_CME_HIDDEN_KEY_REQUIRED,            48, "hidden key required"                           ),
  RESULT_CODE_ENTRY(  VG_CME_UNKNOWN,                       100, "unknown"                                       ),

  /* PSD errors */
  RESULT_CODE_ENTRY(  VG_CME_ILLEGAL_MS,                    103, "illegal MS"                                    ),

  RESULT_CODE_ENTRY(  VG_CME_ILLEGAL_ME,                    106, "illegal ME"                                    ),
  RESULT_CODE_ENTRY(  VG_CME_PSD_SERVICES_NOT_ALLOWED,      107, "psd services not allowed"                      ),

  RESULT_CODE_ENTRY(  VG_CME_PLMN_NOT_ALLOWED,              111, "plmn not allowed"                              ),
  RESULT_CODE_ENTRY(  VG_CME_LOCATION_AREA_NOT_ALLOWED,     112, "location area not allowed"                     ),
  RESULT_CODE_ENTRY(  VG_CME_ROAMING_NOT_ALLOWED,           113, "roaming not allowed in this location area"     ),

  RESULT_CODE_ENTRY(  VG_CME_SERVICE_OPTION_NOT_SUPPORTED,  132, "service option not supported"                  ),
  RESULT_CODE_ENTRY(  VG_CME_SERVICE_OPTION_NOT_SUBSCRIBED, 133, "requested service option not subscribed"       ),
  RESULT_CODE_ENTRY(  VG_CME_SERVICE_OPTION_OUT_OF_ORDER,   134, "service option temporarily out of order"       ),

  RESULT_CODE_ENTRY(  VG_CME_UNSPECIFIED_PSD_ERROR,         148, "unspecified psd error"                         ),
  RESULT_CODE_ENTRY(  VG_CME_PDP_AUTHENTIFICATION_ERROR,    149, "PDP authentication failure"                    ),
  RESULT_CODE_ENTRY(  VG_CME_INVALID_MOBILE_CLASS,          150, "invalid mobile class"                          ),
  RESULT_CODE_ENTRY(  VG_CME_LAST_PDN_DISCONNECTION_NOT_ALLOWED,
                                                            151, "PSD - last PDN disconnection not allowed"      ),
  /* Other values in the range 101 - 150 are reserved for use by GPRS */

  /* Other values in the range 151 - 170 are reserved for use by VBS/VGCS and eMLPP */

  /* Also all other values below 256 are reserved */

  /* CMS error codes */

  /* added for job132548 */
  /* 24.011 E.2 SMS error codes */

  RESULT_CODE_ENTRY(  VG_CMS_ERROR_UNASSIGNED_NO,             1, "unassigned (unallocated) number"               ),
  RESULT_CODE_ENTRY(  VG_CMS_ERROR_OPER_DETERM_BARRING,       8, "operator determined barring"                   ),
  RESULT_CODE_ENTRY(  VG_CMS_ERROR_CALL_BARRED,              10, "call barred"                                   ),
  RESULT_CODE_ENTRY(  VG_CMS_ERROR_NETWORK_ERROR,            17, "network error"                                 ),
  RESULT_CODE_ENTRY(  VG_CMS_ERROR_REJECTED,                 21, "short message transfer rejected"               ),
  RESULT_CODE_ENTRY(  VG_CMS_ERROR_CONGESTION,               22, "congestion"                                    ),
  RESULT_CODE_ENTRY(  VG_CMS_ERROR_DEST_OUT_OF_ORDER,        27, "destination out of service"                    ),
  RESULT_CODE_ENTRY(  VG_CMS_ERROR_UNIDENTIFIED_SUBSCRIBER,  28, "unidentified subscriber"                       ),
  RESULT_CODE_ENTRY(  VG_CMS_ERROR_FACILITY_REJECTED,        29, "facility rejected"                             ),
  RESULT_CODE_ENTRY(  VG_CMS_ERROR_UNKNOWN_SUBSCRIBER,       30, "unknown subscriber"                            ),
  RESULT_CODE_ENTRY(  VG_CMS_ERROR_NET_OUT_OF_ORDER,         38, "network out of order"                          ),
  RESULT_CODE_ENTRY(  VG_CMS_ERROR_TEMP_FAILURE,             41, "temporary failure"                             ),
  RESULT_CODE_ENTRY(  VG_CMS_ERROR_SWITCH_CONGESTION,        42, "congestion"                                    ),
  RESULT_CODE_ENTRY(  VG_CMS_ERROR_RESOURCES_UNAV,           47, "resources unavailable, unspecified"            ),
  RESULT_CODE_ENTRY(  VG_CMS_ERROR_REQ_FAC_NOT_SUBSCR,       50, "requested facility not subscribed"             ),
  RESULT_CODE_ENTRY(  VG_CMS_ERROR_REQ_FACIL_NOT_IMPL,       69, "requested facility not implemented"            ),
  RESULT_CODE_ENTRY(  VG_CMS_ERROR_INVALID_SM_TRANSFER_REF,  81, "invalid short message transfer reference value" ),
  RESULT_CODE_ENTRY(  VG_CMS_ERROR_INVALID_MSG_SEMANTIC,     95, "invalid message, unspecified"                  ),
  RESULT_CODE_ENTRY(  VG_CMS_ERROR_MAND_IE_ERROR,            96, "invalid mandatory information"                 ),
  RESULT_CODE_ENTRY(  VG_CMS_ERROR_MSG_NONEXISTENT,          97, "message type non-existent or not implemented"  ),
  RESULT_CODE_ENTRY(  VG_CMS_ERROR_MSG_GEN_ERROR,            98, "message not compatible with short message protocol state" ),
  RESULT_CODE_ENTRY(  VG_CMS_ERROR_IE_NONEXISTENT,           99, "information element non-existent or not implemented" ),
  RESULT_CODE_ENTRY(  VG_CMS_ERROR_PROTOCOL_ERROR,          111, "protocol error, unspecified"                   ),
  RESULT_CODE_ENTRY(  VG_CMS_ERROR_INTERWORKING,            127, "interworking, unspecified"                     ),

  /* 27.005 error codes */                          
  RESULT_CODE_ENTRY(  VG_CMS_ERROR_OP_NOT_ALLOWED,          302, "operation not allowed"                         ),
  RESULT_CODE_ENTRY(  VG_CMS_ERROR_OP_NOT_SUPPORTED,        303, "operation not supported"                       ),
  RESULT_CODE_ENTRY(  VG_CMS_ERROR_INVALID_PDU_MODE_PARM,   304, "invalid PDU mode parameter"                    ),
  RESULT_CODE_ENTRY(  VG_CMS_ERROR_INVALID_TEXT_MODE_PARM,  305, "invalid text mode parameter"                   ),

  RESULT_CODE_ENTRY(  VG_CMS_ERROR_SIM_PIN_NECESSARY,       311, "SIM pin necessary"                             ),
#if defined (ATCI_SLIM_DISABLE)
  RESULT_CODE_ENTRY(  RESULT_CODE_FCERROR,                   10, "+FCERROR"                                      ),
  RESULT_CODE_ENTRY(  VG_CMS_ERROR_ME_FAILURE,              300, "ME failure"                                    ),
  RESULT_CODE_ENTRY(  VG_CMS_ERROR_SMS_ME_RESERVED,         301, "SMS ME reserved"                               ),
  RESULT_CODE_ENTRY(  VG_CMS_ERROR_SIM_NOT_INSERTED,        310, "SIM not inserted"                              ),
  RESULT_CODE_ENTRY(  VG_CMS_ERROR_PH_SIM_PIN_NECESSARY,    312, "PH SIM pin necessary"                          ),
  RESULT_CODE_ENTRY(  VG_CMS_ERROR_SIM_BUSY,                314, "SIM busy"                                      ),
  RESULT_CODE_ENTRY(  VG_CMS_ERROR_SIM_WRONG,               315, "SIM wrong"                                     ),
  RESULT_CODE_ENTRY(  VG_CMS_ERROR_SIM_PUK_REQUIRED,        316, "SIM PUK required"                              ),
  RESULT_CODE_ENTRY(  VG_CMS_ERROR_SIM_PIN2_REQUIRED,       317, "SIM PIN2 required"                             ),
  RESULT_CODE_ENTRY(  VG_CMS_ERROR_SIM_PUK2_REQUIRED,       318, "SIM PUK2 required"                             ),
  RESULT_CODE_ENTRY(  VG_CMS_ERROR_MEMORY_FAILURE,          320, "memory failure" 
  RESULT_CODE_ENTRY(  VG_CMS_ERROR_SMSC_ADDRESS_UNKNOWN,    330, "SMSC address unknown"                          ),
  RESULT_CODE_ENTRY(  VG_CMS_ERROR_NO_NETWORK,              331, "no network"                                    ),
  RESULT_CODE_ENTRY(  VG_CMS_ERROR_NETWORK_TIMEOUT,         332, "network timeout"                               ),
  RESULT_CODE_ENTRY(  VG_CMS_ERROR_NO_CNMA_ACK_EXPECTED,    340, "no +CNMA acknowledgment expected"              ),
  RESULT_CODE_ENTRY(  VG_CMS_CB_ERROR_UNKNOWN,              514, "CB error unknown"                              ),
  RESULT_CODE_ENTRY(  VG_CMS_ERROR_INVALID_PDU_ADDRESS,     532, "invalid address (no digits read)"              ),
  RESULT_CODE_ENTRY(  VG_CMS_ERROR_INVALID_PDU_CHAR,        541, "invalid charater(s) in PDU"                    ),
  RESULT_CODE_ENTRY(  VG_CME_COMBINED_SERVICES_NOT_ALLOWED, 584, "PSD - combined services not allowed"          ),
  RESULT_CODE_ENTRY(  VG_CME_DUPLICATE_TI_RECEIVED,         587, "PSD - duplicate TI received"                  ),
  RESULT_CODE_ENTRY(  VG_CME_LLC_ERROR,                     604, "PSD - LLC error"                              ),
  RESULT_CODE_ENTRY(  VG_CME_NCP_CLOSE,                     614, "PSD - NCP close"                              ),
  RESULT_CODE_ENTRY(  VG_CME_SERVICE_TYPE_NOT_AVAILABLE,    639, "PSD - service type not yet available"         ),
  RESULT_CODE_ENTRY(  VG_CME_QOS_INVALID_PARAMETER,         646, "PSD - QOS invalid parameter"                  ),  
  RESULT_CODE_ENTRY(  VG_CME_SIM_MODIFIED_SS_ERROR,         723, "error in SS modified by SIM"                  ),
  RESULT_CODE_ENTRY(  VG_CME_CSCS_MODE_NOT_SUPPORTED,       737, "+CSCS type not supported"                     ),
  RESULT_CODE_ENTRY(  VG_CME_CPOL_FORMAT_MISSING,           741, "must include <format> with <oper>"            ),
  RESULT_CODE_ENTRY(  VG_CME_CRSM_3G_SIM,                   751, "command not allowed for 3G SIM"               ),
  RESULT_CODE_ENTRY(  VG_CME_CRSM_MISSING_P_PARAMETER,      756, "missing required P1/2/3 parameter"            ),
  RESULT_CODE_ENTRY(  VG_CME_CRSM_INVALID_CHARS_IN_CMD_DATA,759, "invalid characters in command data"           ),
  RESULT_CODE_ENTRY(  VG_CME_MUX_ALREADY_RUNNING,           768, "multiplexer already active"                   ),
  RESULT_CODE_ENTRY(  VG_CME_CALL_SETUP_IN_PROGRESS,        771, "call setup in progress"                       ),
#endif
  RESULT_CODE_ENTRY(  VG_CMS_ERROR_SIM_FAILURE,             313, "SIM failure"                                   ),

                             
  RESULT_CODE_ENTRY(  VG_CMS_ERROR_INVALID_MEMORY_INDEX,    321, "invalid memory index"                          ),
  RESULT_CODE_ENTRY(  VG_CMS_ERROR_MEMORY_FULL,             322, "memory full"                                   ),



  RESULT_CODE_ENTRY(  VG_CMS_ERROR_UNKNOWN,                 500, "unknown"                                       ),

  /* Other values in the range 256 - 511 are reserved */

  /* Other values in the range 512... are manufacturer specific */

  /* proprietary CME errors */

  /* if a new CME error code is needed, the numeric code must be greater or
   * equal to 512. This ensures that changes to the standards can be implemented */

  /* values in the range 512 - 576 are nominally reserved for proprietary SMS error codes */

  RESULT_CODE_ENTRY(  VG_CMS_ERROR_SIM_NOT_READY,           512, "SIM not ready"                                 ),
  RESULT_CODE_ENTRY(  VG_CMS_UNREAD_RECORDS_ON_SIM,         513, "unread records on SIM"                         ),

  RESULT_CODE_ENTRY(  VG_CMS_ERROR_PS_BUSY,                 515, "PS busy"                                       ),
  RESULT_CODE_ENTRY(  VG_CMS_ERROR_SMREADY_COULDNT_READ_SMSP,
                                                            516, "couldn't read SMS parameters from SIM"         ),
  RESULT_CODE_ENTRY(  VG_CMS_ERROR_SM_NOT_READY,            517, "SM BL not ready"                               ),
  RESULT_CODE_ENTRY(  VG_CMS_ERROR_INVALID_PARMETER,        518, "invalid parameter"                             ),
  RESULT_CODE_ENTRY(  VG_CMS_ERROR_NVRAM_NOT_AVAILABLE,     519, "ME temporary not available"                    ),

  /* error codes for send SMS (PDU) */
  RESULT_CODE_ENTRY(  VG_CMS_ERROR_INVALID_PDU_HEXCHAR,     528, "invalid (non-hex) chars in PDU"                ),
  RESULT_CODE_ENTRY(  VG_CMS_ERROR_INVALID_PDU_LENGTH,      529, "incorrect PDU length"                          ),
  RESULT_CODE_ENTRY(  VG_CMS_ERROR_INVALID_PDU_MTI,         530, "invalid MTI"                                   ),

  RESULT_CODE_ENTRY(  VG_CMS_ERROR_INVALID_PDU_DIGITS_IN_ADDRESS,
                                                            531, "invalid (non-hex) chars in address"            ),

  RESULT_CODE_ENTRY(  VG_CMS_ERROR_INVALID_PDU_INCORRECT_UDL,
                                                            533, "incorrect PDU length (UDL)"                    ),
  RESULT_CODE_ENTRY(  VG_CMS_ERROR_INVALID_PDU_SCA_LENGTH,  534, "incorrect SCA length"                          ),

  /* error codes for send SMS (text) */
  RESULT_CODE_ENTRY(  VG_CMS_ERROR_SMSCMD_INVALID_FIRSTOCTET,
                                                            536, "invalid First Octet (should be 2 or 34)"       ),
  RESULT_CODE_ENTRY(  VG_CMS_ERROR_SMSCMD_INVALID_CMDTYPE,  537, "invalid Command Type"                          ),
  RESULT_CODE_ENTRY(  VG_CMS_ERROR_SMSCMD_SRR_BIT_NOT_SET,  538, "SRR bit not set"                               ),
  RESULT_CODE_ENTRY(  VG_CMS_ERROR_SMSCMD_SRR_BIT_IS_SET,   539, "SRR bit set"                                   ),
  RESULT_CODE_ENTRY(  VG_CMS_ERROR_INVALID_UDH,             540, "invalid User Data Header IE"                   ),
  

  /* values in the range 577 - 699 are nominally reserved for proprietary PSD/PPP error codes */

  /* TODO: Remove legacy errors (specifically PPP) under which are not required
   */

  RESULT_CODE_ENTRY(  VG_CME_ACTIV_REJ_BY_GGSN,             577, "PSD - activation rejected by GGSN"            ),
  RESULT_CODE_ENTRY(  VG_CME_ACTIV_REJ_UNSPECIFIED,         578, "PSD - unspecified activation rejection"       ),
  RESULT_CODE_ENTRY(  VG_CME_BAD_CODE_OR_PROTOCOL_REJ,      579, "579"                                          ),
  RESULT_CODE_ENTRY(  VG_CME_CANT_MODIFY_ADDRESS,           580, "580"                                          ),
  RESULT_CODE_ENTRY(  VG_CME_CHAP_CLOSE,                    581, "581"                                          ),
  RESULT_CODE_ENTRY(  VG_CME_CID_NOT_AVAILABLE,             582, "profile (cid) currently unavailable"    ),
  RESULT_CODE_ENTRY(  VG_CME_CID_ALREADY_ACTIVE,            583, "a profile (cid) is currently active"    ),

  RESULT_CODE_ENTRY(  VG_CME_CONDITIONAL_IE_ERROR,          585, "conditional IE error"                   ),
  /* Error code VG_CME_CONTX_ACT_REJECTED, 586, removed */
  RESULT_CODE_ENTRY(  VG_CME_FEATURE_NOT_SUPPORTED,         588, "feature not supported"                  ),
  RESULT_CODE_ENTRY(  VG_CME_PSD_SERVICE_NOT_AVAILABLE,     589, "service not available"                  ),
  RESULT_CODE_ENTRY(  VG_CME_IE_NONEXIST_OR_NOT_IMP,        590, "unknown IE from network"                ),
  RESULT_CODE_ENTRY(  VG_CME_IMPLICITLY_DETACHED,           591, "implicitly detached"                    ),
  RESULT_CODE_ENTRY(  VG_CME_INSUFFIC_RESOURCES,            592, "insufficient resources"                 ),
  RESULT_CODE_ENTRY(  VG_CME_INVALID_ACTIVATION_STATE,      593, "invalid activation state (0-1)"         ),
  RESULT_CODE_ENTRY(  VG_CME_INVALID_ADDRESS_LENGTH,        594, "594"                                          ),
  RESULT_CODE_ENTRY(  VG_CME_INVALID_CHAR_IN_ADDRESS_STRING,595, "595"                                          ),
  RESULT_CODE_ENTRY(  VG_CME_INVALID_CID_VALUE,             596, "PSD - invalid cid value"                      ),
  RESULT_CODE_ENTRY(  VG_CME_INVALID_DIALSTRING_LENGTH,     597, "597"                                          ),
  RESULT_CODE_ENTRY(  VG_CME_INVALID_EREP_MODE,             598, "mode value not in range"                ),
  RESULT_CODE_ENTRY(  VG_CME_INVALID_MAND_INFORMATION,      599, "invalid MAND information"               ),
  RESULT_CODE_ENTRY(  VG_CME_INVALID_SMS_SERVICE_PREF,      600, "SMS service preference out of range"    ),
  RESULT_CODE_ENTRY(  VG_CME_INVALID_TI_VALUE,              601, "invalid TI value"                       ),
  RESULT_CODE_ENTRY(  VG_CME_IPCP_NEGOTIATION_TIMEOUT,      602, "602"                                          ),
  RESULT_CODE_ENTRY(  VG_CME_LCP_REQ_NEGOTIATION_TIMEOUT,   603, "603"                                          ),
  RESULT_CODE_ENTRY(  VG_CME_LLC_OR_SNDCP_FAILURE,          605, "LLC or SNDCP failure"                   ),
  RESULT_CODE_ENTRY(  VG_CME_LOWER_LAYER_FAILURE,           606, "lower layer failure"                    ),
  RESULT_CODE_ENTRY(  VG_CME_MISSING_OR_UNKNOWN_APN,        607, "missing or unknown APN"                 ),
  RESULT_CODE_ENTRY(  VG_CME_MOBILE_NOT_READY,              608, "mobile not ready"                       ),
  RESULT_CODE_ENTRY(  VG_CME_MS_ID_NOT_IN_NETWORK,          609, "MS identity not in network"             ),
  RESULT_CODE_ENTRY(  VG_CME_MSC_TEMP_NOT_REACHABLE,        610, "MSC temporarily not reachable"          ),
  RESULT_CODE_ENTRY(  VG_CME_MSG_INCOMPAT_WITH_STATE,       611, "message incompatible with state"        ),
  RESULT_CODE_ENTRY(  VG_CME_MSG_TYPE_INCOMPAT_WITH_STATE,  612, "message type incompatible with state"   ),
  RESULT_CODE_ENTRY(  VG_CME_MSG_TYPE_NONEXIST_OR_NOT_IMP,  613, "unknown message from network"           ),
  RESULT_CODE_ENTRY(  VG_CME_NETWORK_FAILURE,               615, "network failure"                        ),
  RESULT_CODE_ENTRY(  VG_CME_NO_ECHO_REPLY,                 616, "616"                                          ),
  RESULT_CODE_ENTRY(  VG_CME_NO_FREE_NSAPIS,                617, "617"                                          ),
  RESULT_CODE_ENTRY(  VG_CME_NO_FURTHER_CIDS_SUPPORTED,     618, "processing of multiple cids not supported" ),
  RESULT_CODE_ENTRY(  VG_CME_NO_PDP_CONTEXT_ACTIVATED,      619, "no PDP context activated"               ),
  RESULT_CODE_ENTRY(  VG_CME_NORMAL_TERMINATION,            620, "620"                                          ),
  RESULT_CODE_ENTRY(  VG_CME_NSAPI_ALREADY_USED,            621, "NSAPI already used"                     ),
  RESULT_CODE_ENTRY(  VG_CME_OOR_ADDRESS_ELEMENT,           622, "address element out of range"           ),
  RESULT_CODE_ENTRY(  VG_CME_PAP_CLOSE,                     623, "623"                                          ),
  RESULT_CODE_ENTRY(  VG_CME_PDP_CONTEXT_WITHOUT_TFT_ALREADY_ACTIVATED,
                                                            624, "GPRS - PDP context w/o TFT already activated" ),
  RESULT_CODE_ENTRY(  VG_CME_PDP_TYPE_NOT_SUPPORTED,        625, "PSD - pdp type not supported"                 ),
  RESULT_CODE_ENTRY(  VG_CME_PEER_REFUSES_OUR_ACCM,         626, "626"                                          ),
  RESULT_CODE_ENTRY(  VG_CME_PEER_REFUSES_OUR_IP_ADDR,      627, "627"                                          ),
  RESULT_CODE_ENTRY(  VG_CME_PEER_REFUSES_OUR_MRU,          628, "628"                                          ),
  RESULT_CODE_ENTRY(  VG_CME_PEER_REREQUESTED_CHAP,         629, "629"                                          ),
  RESULT_CODE_ENTRY(  VG_CME_PROFILE_NOT_DEFINED,           630, "PSD - profile (cid) not defined"              ),
  RESULT_CODE_ENTRY(  VG_CME_PROTOCOL_ERROR_UNSPEC,         631, "unspecified protocol error"             ),
  RESULT_CODE_ENTRY(  VG_CME_QOS_NOT_ACCEPTED,              632, "QOS not accepted"                       ),
  /* Error code VG_CME_QOS_VALIDATION_FAIL, 633, removed */
  RESULT_CODE_ENTRY(  VG_CME_REACTIVATION_REQUIRED,         634, "reactivation required"                  ),
  RESULT_CODE_ENTRY(  VG_CME_REGULAR_DEACTIVATION,          635, "regular deactivation"                   ),
  RESULT_CODE_ENTRY(  VG_CME_SEMANTIC_ERROR_IN_TFT_OPERATION,
                                                            636, "semantic error in TFT operation"        ),
  RESULT_CODE_ENTRY(  VG_CME_SEMANTIC_ERRORS_IN_PACKET_FILTER,
                                                            637, "semantic errors in packet filter"       ),
  RESULT_CODE_ENTRY(  VG_CME_SEMANTICALLY_INCORRECT_MSG,    638, "semantically incorrect message"         ),
  RESULT_CODE_ENTRY(  VG_CME_SYNTACTICAL_ERROR_IN_TFT_OPERATION,
                                                            640, "syntactical error in TFT"      ),
  RESULT_CODE_ENTRY(  VG_CME_SYNTACTICAL_ERRORS_IN_PACKET_FILTER,
                                                            641, "syntactical error in packet filter"    ),
  RESULT_CODE_ENTRY(  VG_CME_TOO_MANY_RXJS,                 642, "642"                                          ),
  RESULT_CODE_ENTRY(  VG_CME_UNKNOWN_PDP_ADDR_OR_TYPE,      643, "PSD - unknown PDP address or type"            ),
  RESULT_CODE_ENTRY(  VG_CME_UNKNOWN_PDP_CONTEXT,           644, "PSD - unknown PDP context"                    ),
  RESULT_CODE_ENTRY(  VG_CME_USER_AUTH_FAILED,              645, "PSD - user authorisation failed"              ),
  RESULT_CODE_ENTRY(  VG_CME_FDN_FAILURE,                   647, "PSD - FDN failure"                            ),
  RESULT_CODE_ENTRY(  VG_CME_BAD_CONTEXT_PARAMS,            649, "PSD - bad pdp context parameters"             ),
  RESULT_CODE_ENTRY(  VG_CME_ALREADY_ACTIVE,                650, "650"                                          ),
  RESULT_CODE_ENTRY(  VG_CME_LCP_TERM_NEGOTIATION_TIMEOUT,  651, "651"                                          ),
  RESULT_CODE_ENTRY( VG_CME_UTV6AD_MORE_THAN_ONE_DOUBLE_COLON,
                                                            652, "652"   ),//more than one double colon in IPv6 address
  RESULT_CODE_ENTRY( VG_CME_UTV6AD_IPV4_ADDRESS_OFF_END,    653, "653"),//IPv6 address ended with part of an IPv4 address
  RESULT_CODE_ENTRY( VG_CME_UTV6AD_DOTTED_DECIMAL_OUTSIDE_IPV4,
                                                            654, "654"),//IPv6 address used dotted-decimal form outside an IPv4 address
  RESULT_CODE_ENTRY( VG_CME_UTV6AD_IPV4_ERR_BYTE_VALUE_OVERFLOW,
                                                            655, "655"),//in an IPv6 address, a byte of an IPv4 address was too big, causing overflow
  RESULT_CODE_ENTRY( VG_CME_UTV6AD_IPV4_ERR_EMPTY_BYTE,     656, "656"),//in an IPv6 address, a byte of an IPv4 address was missing
  RESULT_CODE_ENTRY( VG_CME_UTV6AD_IPV4_ERR_BYTE_VALUE_TOO_LARGE,
                                                            657, "IPv4 address more than 255"),//in an IPv6 address, a byte of an IPv4 address was more than 255
  RESULT_CODE_ENTRY( VG_CME_UTV6AD_BYTE_PAIR_TOO_LARGE,     658, "658"),//in an IPv6 address, a byte pair was more than hex ffff
  RESULT_CODE_ENTRY( VG_CME_UTV6AD_IPV4_ADDRESS_TOO_SHORT_OR_BAD_CHARS,
                                                            659, "659"),//in an IPv6 address, a byte of an IPv4 address was too short or contained invalid characters
  RESULT_CODE_ENTRY( VG_CME_UTV6AD_IPV6_ADDRESS_TOO_SHORT_OR_BAD_CHARS,
                                                            660, "660"),//an IPv6 address was too short or contained invalid characters
  RESULT_CODE_ENTRY( VG_CME_UTV6AD_BYTE_PAIR_VALUE_OVERFLOW,661, "661"),//in an IPv6 address, a byte pair was too big, causing overflow
  RESULT_CODE_ENTRY( VG_CME_UTV6AD_LEADING_SINGLE_COLON,    662, "662"  ),//an IPv6 address started with a single colon
  RESULT_CODE_ENTRY( VG_CME_UTV6AD_TRAILING_SINGLE_COLON,   663, "663"    ),//an IPv6 address ended with a single colon
  RESULT_CODE_ENTRY( VG_CME_UTV6AD_IPV4_EMBEDDED_NONFINALLY,664, "664"),//an IPv6 address contained an IPv4 address other than at the end
  RESULT_CODE_ENTRY( VG_CME_UTV6AD_BUFFER_TOO_SMALL,        665, "an IPv6 address was too long"                 ),
  RESULT_CODE_ENTRY( VG_CME_UTV6AD_TRAILING_JUNK,           666, "666"),//an IPv6 address was followed by invalid characters
  RESULT_CODE_ENTRY(  VG_CME_CONTEXT_ACT_BARRED_BY_SIM,     667, "Call Control - Context barred by SIM"   ),

  /* values in the range 670 - 704 are nominally reserved for proprietary PSD LTE/NASMDL2 error codes */
  /* ESM Errors */
  RESULT_CODE_ENTRY(  VG_CME_OPERATOR_DETERMINED_BARRING,                         670,     "operator determined barring"                        ),
  RESULT_CODE_ENTRY(  VG_CME_ACTIVE_REJ_BY_SERVING_GW_PDNGW,                      671,     "activation rejected by GW or PDNGW"                 ),
  RESULT_CODE_ENTRY(  VG_CME_PTI_ALREADY_IN_USE,                                  672,     "PTI already in use"                                 ),
  RESULT_CODE_ENTRY(  VG_CME_EPS_B_CONTEXT_WITHOUT_TFT_ALREADY_ACTIVATED,         673,     "EPS Bearer Context without TFT already activated"   ),
  RESULT_CODE_ENTRY(  VG_CME_PTI_MISMATCH,                                        674,     "PTI mismatch"                                       ),
  RESULT_CODE_ENTRY(  VG_CME_PDN_TYPE_IPV4_ONLY_ALLOWED,                          675,     "PDN Type IPV4 only allowed"                         ),
  RESULT_CODE_ENTRY(  VG_CME_PDN_TYPE_IPV6_ONLY_ALLOWED,                          676,     "PDN Type IPV6 only allowed"                         ),
  RESULT_CODE_ENTRY(  VG_CME_SINGLE_ADDR_BEARERS_ONLY_ALLOWED,                    677,     "single address bearers only allowed"                ),
  RESULT_CODE_ENTRY(  VG_CME_ESM_INFO_NOT_RECEIVED,                               678,     "ESM information not received"                       ),
  RESULT_CODE_ENTRY(  VG_CME_PDN_CONNECTION_DOES_NOT_EXIST,                       679,     "PDN connection does not exist"                      ),
  RESULT_CODE_ENTRY(  VG_CME_MULTI_PDN_CONN_NOT_ALLOWED_FOR_ONE_APN,              680,     "multiple PDN connection not allowed for one APN"    ),
  RESULT_CODE_ENTRY(  VG_CME_COLLISION_WITH_NW_INIT_REQ,                          681,     "collision with network initiated request"           ),
  RESULT_CODE_ENTRY(  VG_CME_UNSUPPORTED_QCI_VALUE,                               682,     "unsupported QCI value"                              ),
  RESULT_CODE_ENTRY(  VG_CME_INVALID_PTI_VALUE,                                   683,     "invalid PTI value"                                  ),
  RESULT_CODE_ENTRY(  VG_CME_APN_RESTRICTION_VALUE_INCOMPAT,                      684,     "incompatible APN restriction value"                 ),
  RESULT_CODE_ENTRY(  VG_CME_REACTIVATION_REQUESTED,                              685,     "reactivation requested"                             ),

  /* EMM Errors */
  RESULT_CODE_ENTRY(  VG_CME_IMSI_UNKNOWN_IN_HSS,                                 690,     "IMSI unknown"                              ),//IMSI unknown in HSS 
  RESULT_CODE_ENTRY(  VG_CME_ILLEGAL_UE,                                          691,     "lllegal UE"                                     ),
  RESULT_CODE_ENTRY(  VG_CME_EPS_SVC_NOT_ALLOWED,                                 692,     "EPS not allowed"                           ),//EPS service not allowed 
  RESULT_CODE_ENTRY(  VG_CME_EPS_AND_NON_EPS_SVC_NOT_ALLOWED,                     693,     "EPS and non EPS not allowed"               ),//EPS and non EPS Service not allowed 
  RESULT_CODE_ENTRY(  VG_CME_UE_ID_CANNOT_BE_DERIVED,                             694,     "UE ID ERROR" ), //UE ID cannot be derived                          
  RESULT_CODE_ENTRY(  VG_CME_TA_NOT_ALLOWED,                                      695,     "EPS tracking area not allowed"                      ),
  RESULT_CODE_ENTRY(  VG_CME_ROAMING_NOT_ALLOWED_IN_TA,                           696,     "roam not allowed in TA"                 ), //roaming not allowed in TA        
  RESULT_CODE_ENTRY(  VG_CME_EPS_SVC_NOT_ALLOWED_IN_PLMN,                         697,     "roam not allowed in PLMN"                     ),//roaming not allowed in PLMN   
  RESULT_CODE_ENTRY(  VG_CME_NO_SUITABLE_CELLS_IN_TA,                             698,     "no suitable cells"                    ),//no suitable cells in TA        
  RESULT_CODE_ENTRY(  VG_CME_CS_DOMAIN_NOT_AVAILABLE,                             699,     "CS not avail"                          ),//CS domain not available  
  RESULT_CODE_ENTRY(  VG_CME_ESM_FAILURE,                                         700,     "ESM failure"                                        ),
  RESULT_CODE_ENTRY(  VG_CME_MAC_FAILURE,                                         701,     "MAC failure"                                        ),
  RESULT_CODE_ENTRY(  VG_CME_SYNCH_FAILURE,                                       702,     "synch failure"                                      ),
  RESULT_CODE_ENTRY(  VG_CME_CONGESTION,                                          703,     "congestion"                                         ),
  RESULT_CODE_ENTRY(  VG_CME_UE_SEC_CAP_MISMATCH,                                 704,     "UE secu cap mismatch"          ),//UE security capability mismatch          
  RESULT_CODE_ENTRY(  VG_CME_SEC_MODE_REJ_UNSPEC,                                 705,     "secu mode rej"              ),//security mode rejected, unspecified  
  RESULT_CODE_ENTRY(  VG_CME_NON_AUTH_FOR_CSG,                                    706,     "UE not authorised in CSG cell"                      ),
  RESULT_CODE_ENTRY(  VG_CME_NON_EPS_AUTH_UNACCEPTABLE,                           707,     "non-EPS authorisation unacceptable"                 ),
  RESULT_CODE_ENTRY(  VG_CME_CS_DOMAIN_TEMP_UNAVAIL,                              708,     "CS temporarily unavailable"   ),//CS domain temporarily unavailable               
  RESULT_CODE_ENTRY(  VG_CME_NO_EPS_BEARER_CONTEXT_ACT,                           709,     "no EPS bearer context activated"                    ),

  RESULT_CODE_ENTRY(  VG_CME_PSD_MODE_NOT_POSSIBLE,                               710,     "PSD mode not possible"            ),
  RESULT_CODE_ENTRY(  VG_CME_INVALID_CONN_TYPE,                                   711,     "invalid connection type"          ),
  RESULT_CODE_ENTRY(  VG_CME_NO_FREE_PSD_BEARER_IDS,                              712,     "no free Bearer IDs"           ),//no free PSD Bearer IDs
  RESULT_CODE_ENTRY(  VG_CME_NO_FREE_PTIS,                                        713,     "no free PTIs"                 ),//no free PSD PTIs
  RESULT_CODE_ENTRY(  VG_CME_UNABLE_TO_OPEN_DATA_CONN,                            714,     "714"   ),//unable to open data connection
  RESULT_CODE_ENTRY(  VG_CME_INCORRECT_USERNAME_PASSWD,                           715,     "incorrect UN/PWD"      ),//incorrect username/password

  /* values in the range 720 - 729 are nominally reserved for proprietary STK error codes */
#if defined(FEA_TEMP_SIMAT_MENU)
  RESULT_CODE_ENTRY(  VG_CME_STK_NO_MENU_AVAILABLE,         720, "sim toolkit menu has not been configured"      ),
#endif

  RESULT_CODE_ENTRY(  VG_CME_STK_ALREADY_IN_USE,            721, "sim toolkit already in use"                    ),
  RESULT_CODE_ENTRY(  VG_CME_STK_NOT_ENABLED,               722, "sim toolkit not enabled"                       ),
  RESULT_CODE_ENTRY(  VG_CME_STK_PROFILE_NOT_UPDATED,       724, "MMI profile not updated"                       ),
  RESULT_CODE_ENTRY(  VG_CME_STK_INVALID_COMMAND_ID,        725, "invalid sim toolkit proactive command ID"      ),
  RESULT_CODE_ENTRY(  VG_CME_STK_INVALID_RESPONSE_DATA,     726, "invalid stk proactive command response data"   ),


  /* values in the range 737 - 1024 are nominally reserved for proprietary general error codes */

  /* codes for AT+CSCS command */
  RESULT_CODE_ENTRY(  VG_CME_CSCS_MODE_NOT_FOUND,           738, "+CSCS type not found"                          ),

  /* codes for AT+CPOL command */
  RESULT_CODE_ENTRY(  VG_CME_CPOL_OPER_FORMAT_WRONG,        742, "incorrect <oper> format"                       ),
  RESULT_CODE_ENTRY(  VG_CME_CPOL_OPER_TOO_LONG,            743, "<oper> length too long"                        ),
  RESULT_CODE_ENTRY(  VG_CME_CPOL_SIM_FULL,                 744, "SIM full"                                      ),
  RESULT_CODE_ENTRY(  VG_CME_CPOL_NOT_IN_CONTROL,           745, "unable to change PLMN list"                    ),
  RESULT_CODE_ENTRY(  VG_CME_CPOL_OPERATOR_NOT_FOUND,       746, "network operator not recognised"               ),
  RESULT_CODE_ENTRY(  VG_CME_CPOL_ACT_MISSING,              747, "access technology missing"                     ),
  RESULT_CODE_ENTRY(  VG_CME_CPOL_ACT_WRONG,                748, "access technology not supported"               ),

  /* codes for AT+CSIM command */
  RESULT_CODE_ENTRY(  VG_CME_CSIM_LENGTH_INCORRECT,         749, "invalid command length"                        ),
  RESULT_CODE_ENTRY(  VG_CME_CSIM_INVALID_INPUT_STRING,     750, "invalid input string"                          ),

  /* codes for AT+CRSM command */
  /* added for job134856 */
  RESULT_CODE_ENTRY(  VG_CME_CRSM_PATHID_INVALID,           752, "invalid <pathid> parameter"                    ),
  RESULT_CODE_ENTRY(  VG_CME_CRSM_MISSING_PARAMETER,        753, "missing required cmd parameter"                ),
  RESULT_CODE_ENTRY(  VG_CME_CRSM_INVALD_COMMAND,           754, "invalid SIM command"                           ),
  RESULT_CODE_ENTRY(  VG_CME_CRSM_INVALID_FILE_ID,          755, "invalid File Id"                               ),
  RESULT_CODE_ENTRY(  VG_CME_CRSM_INVALID_P_PARAMETER,      757, "invalid P1/2/3 parameter"                      ),
  RESULT_CODE_ENTRY(  VG_CME_CRSM_MISSING_CMD_DATA,         758, "missing required command data"                 ),

  /* miscellaneous codes */

  RESULT_CODE_ENTRY(  VG_CME_INVALID_INPUT_VALUE,           765, "invalid input value"                           ),
  RESULT_CODE_ENTRY(  VG_CME_UNSUPPORTED_MODE,              766, "unsupported value or mode"                     ),
  RESULT_CODE_ENTRY(  VG_CME_OPERATION_FAILED,              767, "operation failed"                              ),
  RESULT_CODE_ENTRY(  VG_CME_UNABLE_TO_GET_CONTROL,         769, "unable to get control of required module"      ),
  RESULT_CODE_ENTRY(  VG_CME_SIM_NETWORK_REJECT,            770, "SIM invalid - network reject"                  ),
  RESULT_CODE_ENTRY(  VG_CME_SIM_POWERED_DOWN,              772, "SIM powered down"                              ),
  RESULT_CODE_ENTRY(  VG_CME_SIM_FILE_NOT_PRESENT,          773, "SIM File not present"                          ),
  RESULT_CODE_ENTRY(  VG_CME_NVRAM_ITEM_READ_FAILED,        774, "NVRAM item read failed"                        ),
#if defined (FEA_PHONEBOOK)

  /*proprietary codes for AT+CPBW command*/
  RESULT_CODE_ENTRY(  VG_CME_CPBW_CANNOT_UPDATE_GROUP,      780, "cannot update group info"                      ),
  RESULT_CODE_ENTRY(  VG_CME_CPBW_CANNOT_UPDATE_ANR,        781, "cannot update additional number"               ),
  RESULT_CODE_ENTRY(  VG_CME_CPBW_CANNOT_UPDATE_SNE,        782, "cannot update second name"                     ),
  RESULT_CODE_ENTRY(  VG_CME_CPBW_CANNOT_UPDATE_EMAIL,      783, "cannot update email"                           ),
  RESULT_CODE_ENTRY(  VG_CME_CPBW_NO_FREE_EMAIL_NUMBER,     784, "no free email number"                          ),
  RESULT_CODE_ENTRY(  VG_CME_CPBW_NO_FREE_EXT_RECORD,       785, "no free ext record"                            ),
  RESULT_CODE_ENTRY(  VG_CME_CPBW_NO_FREE_CCP_RECORD,       786, "no free ccp record"                            ),
  RESULT_CODE_ENTRY(  VG_CME_CPBW_NO_FREE_CCP_EXT_RECORD,   787, "no free ccp and ext record"                    ),
  RESULT_CODE_ENTRY(  VG_CME_CPBW_NO_FREE_RECORD,           788, "no free record"                                ),

  /*proprietary codes for AT+CPBR command*/
  RESULT_CODE_ENTRY(  VG_CME_CPBR_CANNOT_READ_GROUP,        790, "cannot read group info"                        ),
  RESULT_CODE_ENTRY(  VG_CME_CPBR_CANNOT_READ_ANR,          791, "cannot read additional number"                 ),
  RESULT_CODE_ENTRY(  VG_CME_CPBR_CANNOT_READ_SNE,          792, "cannot read second name"                       ),
  RESULT_CODE_ENTRY(  VG_CME_CPBR_CANNOT_READ_EMAIL,        793, "cannot read email"                             ),

  RESULT_CODE_ENTRY(  VG_CME_MECC_NO_VALID_ECC_CODE,        794, "No valid ECC code"                             ),
  RESULT_CODE_ENTRY(  VG_CME_MGID_NO_VALID_GID,             795, "No valid GId"                                  ),


  RESULT_CODE_ENTRY(  VG_CME_MBMS_SERV_ACT_FULL,            800, "MBMS - services activated is full"               ),
  RESULT_CODE_ENTRY(  VG_CME_MBMS_TMGI_NOT_PROVID,          801, "MBMS - TMGI is not provided"                     ),
  RESULT_CODE_ENTRY(  VG_CME_MBMS_CELL_NOT_SUPPORT,         802, "MBMS - cell not supported"                       ),
  RESULT_CODE_ENTRY(  VG_CME_MBMS_NO_FREE_LSAPI,            803, "MBMS - no free lsapi"                            ),
  RESULT_CODE_ENTRY(  VG_CME_MBMS_RESOURCE_CONFLICT,        804, "MBMS - resource conflict"                        ),
  RESULT_CODE_ENTRY(  VG_CME_MBMS_NETWORK_NORM_RELEASE,     805, "MBMS - network normal release"                   ),
  RESULT_CODE_ENTRY(  VG_CME_MBMS_OUT_OF_MB_COVERAGE,       806, "MBMS - out of MBMS service coverage"             ),
  RESULT_CODE_ENTRY(  VG_CME_MBMS_NET_ABNORMAL_RELEASE,     807, "MBMS - network abnormal release"                 ),
  RESULT_CODE_ENTRY(  VG_CME_MBMS_AUTHENTICATION_REQ,       808, "MBMS - need authentication"                      ),
  RESULT_CODE_ENTRY(  VG_CME_MBMS_AUTH_SYNC_ERROR,          809, "MBMS - auth sync error"                          ),
  RESULT_CODE_ENTRY(  VG_CME_MBMS_AUTH_INVALID_MAC,         810, "MBMS - auth invalid mac"                         ),
  RESULT_CODE_ENTRY(  VG_CME_MBMS_MIKEY_MESSAGE_EXPIRED,    811, "MBMS - mikey message expired"                    ),
  RESULT_CODE_ENTRY(  VG_CME_MBMS_LOCAL_NORM_RELEASE,       812, "MBMS - local normal release"                     ),
  RESULT_CODE_ENTRY(  VG_CME_MBMS_NETWORK_SWITCH,           813, "MBMS - network switch"                           ),
  RESULT_CODE_ENTRY(  VG_CME_MBMS_ABNORMAL_REL_UNSPECIFIED, 814, "MBMS - abnormal rel unspecified"                 ),
#endif

#if defined (ENABLE_AT_ENG_MODE)
  /*proprietary codes for AT*MEMPSET command*/
  RESULT_CODE_ENTRY(  VG_CME_EM_UNSPECIFIED_CAUSE,                      820, "EM - unspecified cause"                       ),
  RESULT_CODE_ENTRY(  VG_CME_EM_BACKGROUND_SEARCH_ONLY_IN_IDLE_STATE,   821, "EM - background search only in idle state"    ),
  RESULT_CODE_ENTRY(  VG_CME_EM_CELL_RESEL_ONLY_IN_NON_DCH_TCH_STATE,   822, "EM - cell resel only in non DCH TCH state"    ),
  RESULT_CODE_ENTRY(  VG_CME_EM_HANDOVER_ONLY_IN_DCH_TCH_STATE,         823, "EM - handover only in DCH TCH state"          ),
  RESULT_CODE_ENTRY(  VG_CME_EM_OCCURRED_BUT_CELL_MISMATCH,             824, "EM - occurred but cell mismatch"              ),
  RESULT_CODE_ENTRY(  VG_CME_EM_CELL_LOCKED,                            825, "EM - cell locked"                             ),
  RESULT_CODE_ENTRY(  VG_CME_EM_ARFCN_LOCKED,                           826, "EM - arfcn locked"                            ),
  RESULT_CODE_ENTRY(  VG_CME_EM_NOT_OCCURRED_WHEN_TIMER_EXPIRY,         827, "EM - not occurred when timer expiry"          ),
  RESULT_CODE_ENTRY(  VG_CME_EM_PLMN_MISMATCH,                          828, "EM - plmn mismatch"                           ),
  RESULT_CODE_ENTRY(  VG_CME_EM_SYSMODE_MISMATCH,                       829, "EM - sysmode mismatch"                        ),
  RESULT_CODE_ENTRY(  VG_CME_EM_OPERATION_NOT_ALLOWED,                  830, "EM - operation not allowed"                   ),
#endif

  RESULT_CODE_ENTRY(  VG_CME_MENGINFO_NO_SERVICE, 840, "No Service state"                 ),
  RESULT_CODE_ENTRY(  VG_CME_MENGINFO_CELL_SEARCH, 841, "In cell search state"                 ),
  RESULT_CODE_ENTRY(  VG_CME_MENGINFO_ERRC_DEACTIVATED, 842, "ERRC is deactivated"                 ),
  RESULT_CODE_ENTRY(  VG_CME_MENGINFO_CELL_RESELECTION, 843, "In cell reselection state"                 ),  
  RESULT_CODE_ENTRY(  VG_CME_MENGINFO_IN_L1_TEST_MODE, 844, "In L1 test mode"                 ),  
  RESULT_CODE_ENTRY(  VG_CME_MENGINFO_IN_REESTABLISHMENT_STATE, 845, "In reestablishment state"                 ),  
  RESULT_CODE_ENTRY(  VG_CME_MENGINFO_PSM_STATE, 846, "In PSM state"                 ),  
  RESULT_CODE_ENTRY(  VG_CME_MENGINFO_NO_DATA_TRANSFER_INFO_IN_IDLE_STATE, 847, "No data transfer in idle state"                 ),
  RESULT_CODE_ENTRY(  VG_CME_MENGINFO_IN_BACKGROUND_SEARCH_STATE, 848, "In background search state"                 ),    

/* Other values in the range 1024 - 65535 are nominally reserved */

#endif

/* END OF FILE */
