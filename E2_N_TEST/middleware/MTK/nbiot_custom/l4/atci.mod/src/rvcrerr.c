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
 * vgcrerr.c - utility functions which take an error typically received from
 *             the background layer and map it to a valid CME error code
 **************************************************************************/

#define MODULE_NAME "RVCRERR"

/***************************************************************************
 * Include Files
 ***************************************************************************/
#include <system.h>
#include <rvsystem.h>
#include <rvcrerr.h>
#include <rvslut.h>

/***************************************************************************
 * Signal definitions
 ***************************************************************************/

/***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/

/***************************************************************************
 * Types
 ***************************************************************************/

typedef struct VgGsmCauseToCmeErrorTag
{
  const GsmCause     cause;
  const ResultCode_t code;
} VgGsmCauseToCmeError;

/***************************************************************************
 * Manifest Constants
 ***************************************************************************/

static const VgGsmCauseToCmeError vgGsmCauseToCmeError[] =
{
  {SM_CAUSE_LLC_OR_SNDCP_FAILURE,         VG_CME_LLC_OR_SNDCP_FAILURE          },
  {SM_CAUSE_INSUFFIC_RESOURCES,           VG_CME_INSUFFIC_RESOURCES            },
  {SM_CAUSE_MISSING_OR_UNKNOWN_APN,       VG_CME_MISSING_OR_UNKNOWN_APN        },
  {SM_CAUSE_UNKNOWN_PDP_ADDR_OR_TYPE,     VG_CME_UNKNOWN_PDP_ADDR_OR_TYPE      },
  {SM_CAUSE_USER_AUTH_FAILED,             VG_CME_USER_AUTH_FAILED              },
  {SM_CAUSE_ACTIV_REJ_BY_GGSN,            VG_CME_ACTIV_REJ_BY_GGSN             },
  {SM_CAUSE_ACTIV_REJ_UNSPECIFIED,        VG_CME_ACTIV_REJ_UNSPECIFIED         },
  {SM_CAUSE_SERVICE_OPT_NOT_SUPPORTED,    VG_CME_SERVICE_OPTION_NOT_SUPPORTED  },
  {SM_CAUSE_SERVICE_OPT_NOT_SUBSCRIBED,   VG_CME_SERVICE_OPTION_NOT_SUBSCRIBED },
  {SM_CAUSE_SERVICE_OPT_TEMP_OUT_OF_ORDER,             VG_CME_SERVICE_OPTION_OUT_OF_ORDER               },
  {SM_CAUSE_NSAPI_ALREADY_USED,           VG_CME_NSAPI_ALREADY_USED            },
  {SM_CAUSE_REGULAR_DEACTIVATION,         VG_CME_REGULAR_DEACTIVATION          },
  {SM_CAUSE_QOS_NOT_ACCEPTED,             VG_CME_QOS_NOT_ACCEPTED              },
  {SM_CAUSE_NETWORK_FAILURE,              VG_CME_NETWORK_FAILURE               },
  {SM_CAUSE_REACTIVATION_REQUIRED,        VG_CME_REACTIVATION_REQUIRED         },
  {SM_CAUSE_FEATURE_NOT_SUPPORTED,        VG_CME_FEATURE_NOT_SUPPORTED         },
  {SM_CAUSE_SEMANTIC_ERROR_IN_TFT_OPERATION,           VG_CME_SEMANTIC_ERROR_IN_TFT_OPERATION           },
  {SM_CAUSE_SYNTACTICAL_ERROR_IN_TFT_OPERATION,        VG_CME_SYNTACTICAL_ERROR_IN_TFT_OPERATION        },
  {SM_CAUSE_UNKNOWN_PDP_CONTEXT,          VG_CME_UNKNOWN_PDP_CONTEXT           },
  {SM_CAUSE_PDP_CONTEXT_WITHOUT_TFT_ALREADY_ACTIVATED, VG_CME_PDP_CONTEXT_WITHOUT_TFT_ALREADY_ACTIVATED },
  {SM_CAUSE_SEMANTIC_ERRORS_IN_PACKET_FILTER,          VG_CME_SEMANTIC_ERRORS_IN_PACKET_FILTER          },
  {SM_CAUSE_SYNTACTICAL_ERRORS_IN_PACKET_FILTER,       VG_CME_SYNTACTICAL_ERRORS_IN_PACKET_FILTER       },
  {SM_CAUSE_INVALID_TI_VALUE,             VG_CME_INVALID_TI_VALUE              },
  {SM_CAUSE_SEMANTICALLY_INCORRECT_MSG,   VG_CME_SEMANTICALLY_INCORRECT_MSG    },
  {SM_CAUSE_INVALID_MAND_INFORMATION,     VG_CME_INVALID_MAND_INFORMATION      },
  {SM_CAUSE_MSG_TYPE_NONEXIST_OR_NOT_IMP, VG_CME_MSG_TYPE_NONEXIST_OR_NOT_IMP  },
  {SM_CAUSE_MSG_TYPE_INCOMPAT_WITH_STATE, VG_CME_MSG_TYPE_INCOMPAT_WITH_STATE  },
  {SM_CAUSE_IE_NONEXIST_OR_NOT_IMP,       VG_CME_IE_NONEXIST_OR_NOT_IMP        },
  {SM_CAUSE_CONDITIONAL_IE_ERROR,         VG_CME_CONDITIONAL_IE_ERROR          },
  {SM_CAUSE_MSG_INCOMPAT_WITH_STATE,      VG_CME_MSG_INCOMPAT_WITH_STATE       },
  {SM_CAUSE_PROTOCOL_ERROR_UNSPEC,        VG_CME_PROTOCOL_ERROR_UNSPEC         },
  {SM_CAUSE_NO_CAUSE_SET,                 VG_CME_UNSPECIFIED_PSD_ERROR         },

  {ESM_CAUSE_OPERATOR_DETERMINED_BARRING,                       VG_CME_OPERATOR_DETERMINED_BARRING                  },
  {ESM_CAUSE_INSUFFICIENT_RESOURCES,                            VG_CME_INSUFFIC_RESOURCES                           },
  {ESM_CAUSE_UNKNOWN_MISSING_APN,                               VG_CME_MISSING_OR_UNKNOWN_APN                       },
  {ESM_CAUSE_UNKNOWN_PDN_TYPE,                                  VG_CME_UNKNOWN_PDP_ADDR_OR_TYPE                     },
  {ESM_CAUSE_USER_AUTHENTICATION_FAILED,                        VG_CME_USER_AUTH_FAILED                             },
  {ESM_CAUSE_REQ_REJECTED_BY_SERVING_GW_PDNGW,                  VG_CME_ACTIVE_REJ_BY_SERVING_GW_PDNGW               },
  {ESM_CAUSE_REQ_REJECTED_UNSPECIFIED,                          VG_CME_ACTIV_REJ_UNSPECIFIED                        },
  {ESM_CAUSE_SERVICE_OPTION_NOT_SUPPORTED,                      VG_CME_SERVICE_OPTION_NOT_SUPPORTED                 },
  {ESM_CAUSE_REQ_SERVICE_OPTION_NOT_SUBSCRIBED,                 VG_CME_SERVICE_OPTION_NOT_SUBSCRIBED                },
  {ESM_CAUSE_SERVICE_OPT_TEMP_OUT_OF_ORDER,                     VG_CME_SERVICE_OPTION_OUT_OF_ORDER                  },
  {ESM_CAUSE_PTI_ALREADY_IN_USE,                                VG_CME_PTI_ALREADY_IN_USE                           },
  {ESM_CAUSE_REGULAR_DEACTIVATION,                              VG_CME_REGULAR_DEACTIVATION                         },
  {ESM_CAUSE_EPS_QOS_NOT_ACCEPTED,                              VG_CME_QOS_NOT_ACCEPTED                             },
  {ESM_CAUSE_NETWORK_FAILURE,                                   VG_CME_NETWORK_FAILURE                              },
  {ESM_CAUSE_REACTIVATION_REQUESTED,                            VG_CME_REACTIVATION_REQUESTED                       },
  {ESM_CAUSE_SEMANTIC_ERROR_IN_TFT_OP,                          VG_CME_SEMANTIC_ERROR_IN_TFT_OPERATION              },
  {ESM_CAUSE_SYNTACTICAL_ERROR_IN_TFT_OP,                       VG_CME_SYNTACTICAL_ERROR_IN_TFT_OPERATION           },
  {ESM_CAUSE_INVALID_EPS_BEARER_ID,                             VG_CME_ACTIV_REJ_UNSPECIFIED                        },
  {ESM_CAUSE_SEMANTIC_ERRORS_IN_PACKET_FILTERS,                 VG_CME_SEMANTIC_ERRORS_IN_PACKET_FILTER             },
  {ESM_CAUSE_SYNTACTICAL_ERRORS_IN_PACKET_FILTERS,              VG_CME_SYNTACTICAL_ERRORS_IN_PACKET_FILTER          },
  {ESM_CAUSE_EPS_B_CONTEXT_WITHOUT_TFT_ALREADY_ACTIVATED,       VG_CME_EPS_B_CONTEXT_WITHOUT_TFT_ALREADY_ACTIVATED  },
  {ESM_CAUSE_PTI_MISMATCH,                                      VG_CME_PTI_MISMATCH                                 },
  {ESM_CAUSE_LAST_PDN_DISCONNECTION_NOT_ALLOWED,                VG_CME_LAST_PDN_DISCONNECTION_NOT_ALLOWED           },
  {ESM_CAUSE_PDN_TYPE_IPV4_ONLY_ALLOWED,                        VG_CME_PDN_TYPE_IPV4_ONLY_ALLOWED                   },
  {ESM_CAUSE_PDN_TYPE_IPV6_ONLY_ALLOWED,                        VG_CME_PDN_TYPE_IPV6_ONLY_ALLOWED                   },
  {ESM_CAUSE_SINGLE_ADDRESS_BEARERS_ONLY_ALLOWED,               VG_CME_SINGLE_ADDR_BEARERS_ONLY_ALLOWED             },
  {ESM_CAUSE_ESM_INFORMATION_NOT_RECEIVED,                      VG_CME_ESM_INFO_NOT_RECEIVED                        },
  {ESM_CAUSE_PDN_CONNECTION_DOES_NOT_EXIST,                     VG_CME_PDN_CONNECTION_DOES_NOT_EXIST                },
  {ESM_CAUSE_MULTIPLE_PDN_CONN_NOT_ALLOWED_FOR_ONE_APN,         VG_CME_MULTI_PDN_CONN_NOT_ALLOWED_FOR_ONE_APN       },
  {ESM_CAUSE_COLLISION_WITH_NW_INITIATED_REQUEST,               VG_CME_COLLISION_WITH_NW_INIT_REQ                   },
  {ESM_CAUSE_UNSUPPORTED_QCI_VALUE,                             VG_CME_UNSUPPORTED_QCI_VALUE                        },
  {ESM_CAUSE_INVALID_PTI_VALUE,                                 VG_CME_INVALID_PTI_VALUE                            },
  {ESM_CAUSE_SEMANTICALLY_INCORRECT_MESSAGE,                    VG_CME_SEMANTICALLY_INCORRECT_MSG                   },
  {ESM_CAUSE_INVALID_MANDATORY_INFORMATION,                     VG_CME_INVALID_MAND_INFORMATION                     },
  {ESM_CAUSE_MESSAGE_TYPE_NON_EXIST_OR_NOT_IMP,                 VG_CME_MSG_TYPE_NONEXIST_OR_NOT_IMP                 },
  {ESM_CAUSE_MESSAGE_TYPE_NOT_COMPATIBLE_WITH_STATE,            VG_CME_MSG_TYPE_INCOMPAT_WITH_STATE                 },
  {ESM_CAUSE_IE_NON_EXIST_OR_NOT_IMP,                           VG_CME_IE_NONEXIST_OR_NOT_IMP                       },
  {ESM_CAUSE_CONDITIONAL_IE_ERROR,                              VG_CME_CONDITIONAL_IE_ERROR                         },
  {ESM_CAUSE_MESSAGE_NOT_COMPAT_WITH_STATE,                     VG_CME_MSG_INCOMPAT_WITH_STATE                      },
  {ESM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED,                        VG_CME_PROTOCOL_ERROR_UNSPEC                        },
  {ESM_CAUSE_APN_RESTRICTION_VALUE_INCOMPATIBLE,                VG_CME_APN_RESTRICTION_VALUE_INCOMPAT               },
  {ESM_CAUSE_NO_CAUSE_SET,                                      VG_CME_UNSPECIFIED_PSD_ERROR                        },

  {PSD_CAUSE_NOT_SET,                                           VG_CME_UNSPECIFIED_PSD_ERROR                        },
  {PSD_CAUSE_NORMAL_TERMINATION,                                VG_CME_NORMAL_TERMINATION                           },
  {PSD_CAUSE_PSD_MODE_NOT_POSSIBLE,                             VG_CME_PSD_MODE_NOT_POSSIBLE,                       },
  {PSD_CAUSE_CANT_MODIFY_ADDRESS,                               VG_CME_CANT_MODIFY_ADDRESS                          },
  {PSD_CAUSE_INVALID_PDP_TYPE,                                  VG_CME_PDP_TYPE_NOT_SUPPORTED                       },
  {PSD_CAUSE_INVALID_ADDRESS_LENGTH,                            VG_CME_INVALID_ADDRESS_LENGTH                       },
  {PSD_CAUSE_INVALID_CONN_TYPE,                                 VG_CME_INVALID_CONN_TYPE                            },
  {PSD_CAUSE_NO_FREE_PSD_BEARER_IDS,                            VG_CME_NO_FREE_PSD_BEARER_IDS                       },
  {PSD_CAUSE_NO_FREE_PTIS,                                      VG_CME_NO_FREE_PTIS                                 },
  {PSD_CAUSE_PSD_SERVICE_NOT_AVAILABLE,                         VG_CME_PSD_SERVICE_NOT_AVAILABLE                    },
  {PSD_CAUSE_POWERING_DOWN,                                     VG_CME_PSD_SERVICE_NOT_AVAILABLE                    },
  {PSD_CAUSE_FDN_FAILURE,                                       VG_CME_FDN_FAILURE                                  },
  {PSD_CAUSE_CONTEXT_ACT_BARRED_BY_SIM,                         VG_CME_CONTEXT_ACT_BARRED_BY_SIM                    },
  {PSD_CAUSE_RESTRICTED_APN_DESTINATION,                        VG_CME_RESTRICTED_APN_DESTINATION                   },
  {PSD_CAUSE_BAD_PSD_CONN_PARAMS,                               VG_CME_BAD_CONTEXT_PARAMS                           },
  {PSD_CAUSE_ALREADY_ACTIVE,                                    VG_CME_ALREADY_ACTIVE                               },
  {PSD_CAUSE_UNABLE_TO_OPEN_DATA_CONN,                          VG_CME_UNABLE_TO_OPEN_DATA_CONN                     },
  {PSD_CAUSE_INCORRECT_USERNAME_PASSWD,                         VG_CME_INCORRECT_USERNAME_PASSWD                    },

#if defined (FEA_PPP)
  {PPP_CAUSE_PEER_REFUSES_OUR_MRU,        VG_CME_PEER_REFUSES_OUR_MRU          },
  {PPP_CAUSE_PEER_REFUSES_OUR_ACCM,       VG_CME_PEER_REFUSES_OUR_ACCM         },
  {PPP_CAUSE_PEER_REFUSES_OUR_IP_ADDR,    VG_CME_PEER_REFUSES_OUR_IP_ADDR      },
  {PPP_CAUSE_PEER_REREQUESTED_CHAP,       VG_CME_PEER_REREQUESTED_CHAP         },
  {PPP_CAUSE_LCP_REQ_NEGOTIATION_TIMEOUT, VG_CME_LCP_REQ_NEGOTIATION_TIMEOUT       },
  {PPP_CAUSE_LCP_TERM_NEGOTIATION_TIMEOUT, VG_CME_LCP_TERM_NEGOTIATION_TIMEOUT       },
  {PPP_CAUSE_IPCP_NEGOTIATION_TIMEOUT,    VG_CME_IPCP_NEGOTIATION_TIMEOUT      },
  {PPP_CAUSE_PAP_CLOSE,                   VG_CME_PAP_CLOSE                     },
  {PPP_CAUSE_CHAP_CLOSE,                  VG_CME_CHAP_CLOSE                    },
  {PPP_CAUSE_NORMAL_TERMINATION,          VG_CME_NORMAL_TERMINATION            },
  /* PPP_CAUSE_NCP_CLOSE is now a synonym for PPP_CAUSE_NORMAL_TERMINATION */

  {PPP_CAUSE_BAD_CODE_OR_PROTOCOL_REJ,    VG_CME_BAD_CODE_OR_PROTOCOL_REJ      },
  {PPP_CAUSE_NO_ECHO_REPLY,               VG_CME_NO_ECHO_REPLY                 },
  {PPP_CAUSE_CANT_MODIFY_ADDRESS,         VG_CME_CANT_MODIFY_ADDRESS           },
  {PPP_CAUSE_TOO_MANY_RXJS,               VG_CME_TOO_MANY_RXJS                 },
  {PPP_CAUSE_INVALID_DIALSTRING_LENGTH,   VG_CME_INVALID_DIALSTRING_LENGTH     },
  {PPP_CAUSE_INVALID_PDP_TYPE,            VG_CME_PDP_TYPE_NOT_SUPPORTED        },
  {PPP_CAUSE_INVALID_CHAR_IN_ADDRESS_STRING,           VG_CME_INVALID_CHAR_IN_ADDRESS_STRING },
  {PPP_CAUSE_OOR_ADDRESS_ELEMENT,         VG_CME_OOR_ADDRESS_ELEMENT           },
  {PPP_CAUSE_INVALID_ADDRESS_LENGTH,      VG_CME_INVALID_ADDRESS_LENGTH        },
  {PPP_CAUSE_NO_FREE_NSAPIS,              VG_CME_NO_FREE_NSAPIS                },
  {PPP_CAUSE_GPRS_SERVICE_NOT_AVAILABLE,  VG_CME_PSD_SERVICE_NOT_AVAILABLE     },
  {PPP_CAUSE_POWERING_DOWN,               VG_CME_PSD_SERVICE_NOT_AVAILABLE     },
  {PPP_CAUSE_FDN_FAILURE,                 VG_CME_FDN_FAILURE                   },
  {PPP_CAUSE_CONTEXT_ACT_BARRED_BY_SIM,   VG_CME_CONTEXT_ACT_BARRED_BY_SIM     },
  {PPP_CAUSE_RESTRICTED_APN_DESTINATION,  VG_CME_RESTRICTED_APN_DESTINATION    },
  {PPP_CAUSE_BAD_CONTEXT_PARAMS,          VG_CME_BAD_CONTEXT_PARAMS            },
  {PPP_CAUSE_ALREADY_ACTIVE,              VG_CME_ALREADY_ACTIVE                },
  {PPP_CAUSE_NO_MEMORY_FOR_ENTITY,        VG_CME_MEMORY_FULL                   },
#endif /* FEA_PPP */

  /* EMM Cause values */
  {EMM_CAUSE_IMSI_UNKNOWN_IN_HSS,                      VG_CME_IMSI_UNKNOWN_IN_HSS             },
  {EMM_CAUSE_ILLEGAL_UE,                               VG_CME_ILLEGAL_UE                      },
  {EMM_CAUSE_ILLEGAL_ME,                               VG_CME_ILLEGAL_ME                      },
  {EMM_CAUSE_EPS_SERVICES_NOT_ALLOWED,                 VG_CME_EPS_SVC_NOT_ALLOWED             },
  {EMM_CAUSE_EPS_AND_NON_EPS_SERVICES_NOT_ALLOWED,     VG_CME_EPS_AND_NON_EPS_SVC_NOT_ALLOWED },
  {EMM_CAUSE_UE_ID_CANNOT_BE_DERIVED,                  VG_CME_UE_ID_CANNOT_BE_DERIVED         },
  {EMM_CAUSE_IMPLICITLY_DETACHED,                      VG_CME_IMPLICITLY_DETACHED             },
  {EMM_CAUSE_PLMN_NOT_ALLOWED,                         VG_CME_PLMN_NOT_ALLOWED                },
  {EMM_CAUSE_TA_NOT_ALLOWED,                           VG_CME_TA_NOT_ALLOWED                  },
  {EMM_CAUSE_ROAMING_NOT_ALLOWED_IN_TA,                VG_CME_ROAMING_NOT_ALLOWED_IN_TA       },
  {EMM_CAUSE_EPS_SERVICES_NOT_ALLOWED_IN_PLMN,         VG_CME_EPS_SVC_NOT_ALLOWED_IN_PLMN     },
  {EMM_CAUSE_NO_SUITABLE_CELLS_IN_TA,                  VG_CME_NO_SUITABLE_CELLS_IN_TA         },
  {EMM_CAUSE_MSC_TEMP_NOT_REACHABLE,                   VG_CME_MSC_TEMP_NOT_REACHABLE          },
  {EMM_CAUSE_NETWORK_FAILURE,                          VG_CME_NETWORK_FAILURE                 },
  {EMM_CAUSE_CS_DOMAIN_NOT_AVAILABLE,                  VG_CME_CS_DOMAIN_NOT_AVAILABLE         },
  {EMM_CAUSE_ESM_FAILURE,                              VG_CME_ESM_FAILURE                     },
  {EMM_CAUSE_MAC_FAILURE,                              VG_CME_MAC_FAILURE                     },
  {EMM_CAUSE_SYNCH_FAILURE,                            VG_CME_SYNCH_FAILURE                   },
  {EMM_CAUSE_CONGESTION,                               VG_CME_CONGESTION                      },
  {EMM_CAUSE_UE_SECURITY_CAP_MISMATCH,                 VG_CME_UE_SEC_CAP_MISMATCH             },
  {EMM_CAUSE_SECURITY_MODE_REJ_UNSPEC,                 VG_CME_SEC_MODE_REJ_UNSPEC             },
  {EMM_CAUSE_NOT_AUTHORIZED_FOR_CSG,                   VG_CME_NON_AUTH_FOR_CSG                },
  {EMM_CAUSE_NON_EPS_AUTH_UNACCEPTABLE,                VG_CME_NON_EPS_AUTH_UNACCEPTABLE       },
  {EMM_CAUSE_CS_DOMAIN_TEMP_UNAVAILABLE,               VG_CME_CS_DOMAIN_TEMP_UNAVAIL          },
  {EMM_CAUSE_NO_EPS_BEARER_CONTEXT_ACTIVATED,          VG_CME_NO_EPS_BEARER_CONTEXT_ACT       },
  {EMM_CAUSE_SEMANTICALLY_INCORRECT_MSG,               VG_CME_SEMANTICALLY_INCORRECT_MSG      },
  {EMM_CAUSE_INVALID_MANDATORY_INFO,                   VG_CME_INVALID_MAND_INFORMATION        },
  {EMM_CAUSE_MESSAGE_TYPE_NON_EXIST_OR_NOT_IMP,        VG_CME_MSG_TYPE_NONEXIST_OR_NOT_IMP    },
  {EMM_CAUSE_MESSAGE_TYPE_NOT_COMPATIBLE_WITH_STATE,   VG_CME_MSG_TYPE_INCOMPAT_WITH_STATE    },
  {EMM_CAUSE_IE_NON_EXIST_OR_NOT_IMP,                  VG_CME_IE_NONEXIST_OR_NOT_IMP          },
  {EMM_CAUSE_CONDITIONAL_IE_ERROR,                     VG_CME_CONDITIONAL_IE_ERROR            },
  {EMM_CAUSE_MESSAGE_NOT_COMPAT_WITH_STATE,            VG_CME_MSG_INCOMPAT_WITH_STATE         },
  {EMM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED,               VG_CME_PROTOCOL_ERROR_UNSPEC           },
  {EMM_CAUSE_NO_CAUSE_SET,                             VG_CME_UNSPECIFIED_PSD_ERROR           },

  {CAUSE_GMM_GPRS_SERVICES_NOT_ALLOWED,                VG_CME_PSD_SERVICES_NOT_ALLOWED        },
  {CAUSE_GMM_COMBINED_SERVICES_NOT_ALLOWED,            VG_CME_COMBINED_SERVICES_NOT_ALLOWED   },
  {CAUSE_GMM_MS_ID_NOT_IN_NETWORK,                     VG_CME_MS_ID_NOT_IN_NETWORK            },
  {CAUSE_GMM_IMPLICITLY_DETACHED,                      VG_CME_IMPLICITLY_DETACHED             },
  {CAUSE_GMM_MSC_TEMP_NOT_REACHABLE,                   VG_CME_MSC_TEMP_NOT_REACHABLE          },
  {CAUSE_NO_PDP_CONTEXT_ACTIVATED,                     VG_CME_NO_PDP_CONTEXT_ACTIVATED        },
  {LOWER_LAYER_FAILURE,                                VG_CME_LOWER_LAYER_FAILURE             },

  /* CAUSE_GMM_NO_CAUSE must be the last entry */
  {CAUSE_GMM_NO_CAUSE,                                 VG_CME_UNSPECIFIED_PSD_ERROR           }

};

#define NUM_GSM_CAUSES (sizeof(vgGsmCauseToCmeError) / sizeof(VgGsmCauseToCmeError))


/***************************************************************************
 * Variables
 ***************************************************************************/

/***************************************************************************
 * Macros
 ***************************************************************************/

/***************************************************************************
 * Local Functions
 ***************************************************************************/

/***************************************************************************
 * Global Functions
 ***************************************************************************/


/*--------------------------------------------------------------------------
 *
 * Function:        vgGetSimCmeErrorCode
 *
 * Parameters:      none
 *
 * Returns:         ResultCode_t - error code
 *
 * Description:     Matches SIM state error to CME error code.
 *
 *-------------------------------------------------------------------------*/

ResultCode_t vgGetSimCmeErrorCode (void)
{
  ResultCode_t            cmeErrorCode;
  SimLockGenericContext_t *simLockGenericContext_p = ptrToSimLockGenericContext ();

  switch (simLockGenericContext_p->simState)
  {
    case VG_SIM_PIN:
    {
      cmeErrorCode = VG_CME_SIM_PIN_REQUIRED;
      break;
    }
    case VG_SIM_PIN2:
    {
      cmeErrorCode = VG_CME_SIM_PIN2_REQUIRED;
      break;
    }
    case VG_SIM_PUK:
    {
      cmeErrorCode = VG_CME_SIM_PUK_REQUIRED;
      break;
    }
    case VG_SIM_PUK2:
    {
      cmeErrorCode = VG_CME_SIM_PUK2_REQUIRED;
      break;
    }
    case VG_SIM_NOT_READY:
    {
      if (simLockGenericContext_p->simInsertedState != VG_SIM_INSERTED)
      {
        /* SIM isn't currently inserted */
        cmeErrorCode = VG_CME_SIM_NOT_INSERTED;
      }
      else if (simLockGenericContext_p->simRejected)
      {
        /* SIM has been rejected by the network */
        cmeErrorCode = VG_CME_SIM_NETWORK_REJECT;
      }
      else if (simLockGenericContext_p->simWrong)
      {
        /* SIM has been rejected by the device */
        cmeErrorCode = VG_CME_SIM_WRONG;
      }
      else if (!simLockGenericContext_p->simPoweredUp)
      {
        /* SIM powered down */
        cmeErrorCode = VG_CME_SIM_POWERED_DOWN;
      }
      else
      {
        /* other SIM failures */
        cmeErrorCode = VG_CME_SIM_FAILURE;
      }
      break;
    }
    default:
    {
      cmeErrorCode = RESULT_CODE_OK;
      break;
    }
  }

  return (cmeErrorCode);
}


/*--------------------------------------------------------------------------
 *
 * Function:        vgGetSimReqCmeErrorCode
 *
 * Parameters:      code - SIM request status code
 *
 * Returns:         ResultCode_t - CME error code value
 *
 * Description:     Maps a SimRequestStatus value to an appropriate CME error
 *                  code
 *-------------------------------------------------------------------------*/

ResultCode_t vgGetSimReqCmeErrorCode (const SimRequestStatus code)
{
  SimLockGenericContext_t *simLockGenericContext_p = ptrToSimLockGenericContext ();
  ResultCode_t            result;

  switch (code)
  {
    case SIM_REQ_MEMORY_PROBLEM: /* a memory problem has been encountered */
    {
      result = VG_CME_MEMORY_FULL;
      break;
    }
    case SIM_REQ_SERVICE_NOT_AVAILABLE: /* requested service to available */
    {
      if (simLockGenericContext_p->simState != VG_SIM_READY)
      { /* if the SIM is not in a ready state then display SIM state error */
        result = vgGetSimCmeErrorCode ();
      }
      else
      { /* if the SIM is OK then the operation was not allowed */
        result = VG_CME_OPERATION_NOT_ALLOWED;
      }
      break;
    }
    case SIM_REQ_SIM_GENERAL_FAULT: /* a general fault has ocurred */
    {
      result = VG_CME_SIM_FAILURE;
      break;
    }
    case SIM_REQ_ACCESS_DENIED:
    { /* access has been denied to the requested service */
      result = VG_CME_OPERATION_NOT_ALLOWED;
      break;
    }
    case SIM_REQ_ALLOC_ERROR: /* a memory allocation error has occurred */
    {
      result = VG_CME_MEMORY_FULL;
      break;
    }
    case SIM_REQ_SM_FAULT: /* an internal SIM manager fault has occurred */
    {
      result = VG_CME_SIM_FAILURE;
      break;
    }
    default: /* an error without a CME code mapping has been received */
    {
      result = VG_CME_UNKNOWN;
      break;
    }
  }

  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgGetLmCmeErrorCode
 *
 * Parameters:      code - LM error code
 *
 * Returns:         ResultCode_t - CME error code value
 *
 * Description:     Maps a LmRequestStatus value to an appropriate CME error
 *                  code
 *
 *-------------------------------------------------------------------------*/

ResultCode_t vgGetLmCmeErrorCode (const LmRequestStatus code)
{
  ResultCode_t result;

  switch (code)
  {
    case LM_REQ_SIM_NOT_READY: /* SIM is not ready, display SIM state error */
    {
      result = vgGetSimCmeErrorCode ();
      break;
    }
    case LM_REQ_ACCESS_DENIED:
    {
      /* access to some of the files is not permited
      * it could be FDN/BDN and PIN2 or SDN and PIN/ADM
      * we shouldnt change Sim state as this is not result
      * of the verification process */
      result = VG_CME_OPERATION_NOT_ALLOWED;
      break;
    }
    case LM_REQ_SIM_ERROR:
    {
      result = VG_CME_SIM_FAILURE;
      break;
    }
    case LM_REQ_NRAM_ERROR: /* an NVRAM error has occurred */
    {
      result = VG_CME_MEMORY_FAILURE;
      break;
    }
    case LM_REQ_FILE_NOT_SUPPORTED: /* file is not supported */
    {
      result = VG_CME_OPERATION_NOT_SUPPORTED;
      break;
    }
    case LM_REQ_RECORD_NOT_FOUND: /* requested record has not been found */
    {
      result = VG_CME_NOT_FOUND;
      break;
    }
    case LM_REQ_EXT_INCOMPLETE_WRITE:
    {
      result = VG_CME_CPBW_NO_FREE_EXT_RECORD;
      break;
    }

    case LM_REQ_CCP_INCOMPLETE_WRITE:
    {
      result = VG_CME_CPBW_NO_FREE_CCP_RECORD;
      break;
    }

    case LM_REQ_CCP_EXT_INCOMPLETE_WRITE:
    {
      result = VG_CME_CPBW_NO_FREE_CCP_EXT_RECORD;
      break;
    }

    case LM_REQ_INCOMPLETE_WRITE: /* the requested write was not completed */
    {
      result = VG_CME_CPBW_NO_FREE_RECORD;
      break;
    }
    case LM_REQ_BUSY: /* The stack is busy dealing with another request */
    {
      result = RESULT_CODE_BUSY;
      break;
    }
    case LM_REQ_ILLEGAL_OPERATION: /* requested operation was not allowed */
    {
      result = VG_CME_OPERATION_NOT_ALLOWED;
      break;
    }
    default:
    { /* an error without a CME code mapping has been received */
      result = VG_CME_UNKNOWN;
      break;
    }
  }

  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgGetSimPinCmeErrorCode
 *
 * Parameters:      state - SIM request status code
 *                  Key ref - key reference of the password
 *
 * Returns:         ResultCode_t - CME error code value
 *
 * Description:     Maps a SimRequestStatus value to an appropriate CME error
 *                  code when access has been denied
 *
 *-------------------------------------------------------------------------*/

ResultCode_t vgGetSimPinCmeErrorCode (const SimRequestStatus state,
                                       const SimUiccKeyRefValue keyRef)
{
  ResultCode_t result;
  SimLockGenericContext_t *simLockGenericContext_p = ptrToSimLockGenericContext ();

  switch (state)
  {
    case SIM_REQ_CODE_BLOCKED: /* passcode given has been blocked */
    {
      if (keyRef == simLockGenericContext_p->simInfo.pin1KeyRef) /* PUK code required for access to be given */
      {
        vgSetSimState (VG_SIM_PUK);

        result = VG_CME_SIM_PUK_REQUIRED;
      }
      else /* PUK2 code required for access to be given */
      {
        vgSetSimState (VG_SIM_PUK2);

        result = VG_CME_SIM_PUK2_REQUIRED;
      }
      break;
    }
    case SIM_REQ_ACCESS_DENIED: /* passcode given was incorrect */
    {
      /*Here no state need to be changed. Just report password incorrect. */
      result = VG_CME_INCORRECT_PASSWORD;
      break;
    }
    default:
    { /* use sim req error mappings for other error */
      result = vgGetSimReqCmeErrorCode (state);
      break;
    }
  }

  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgGetSimChvCmeErrorCode
 *
 * Parameters:      code - SIM request status code
 *                  SimChvNum - passcode type required
 *
 * Returns:         ResultCode_t - CME error code value
 *
 * Description:     Maps a SimRequestStatus value to an appropriate CME error
 *                  code when access has been denied
 *
 *-------------------------------------------------------------------------*/

ResultCode_t vgGetSimChvCmeErrorCode (const SimRequestStatus state,
                                       const SimChvNum chvNum)
{
  ResultCode_t result;
  SimLockGenericContext_t *simLockGenericContext_p = ptrToSimLockGenericContext ();

  switch (state)
  {
    case SIM_REQ_CODE_BLOCKED: /* passcode given has been blocked */
    {
      if (chvNum == SIM_CHV_1) /* PUK code required for access to be given */
      {
        /* PIN verification failed, CHV1 code blocked */
        vgSetSimState (VG_SIM_PUK);
        result = VG_CME_SIM_PUK_REQUIRED;
      }
      else /* PUK2 code required for access to be given */
      {
        vgSetSimState (VG_SIM_PUK2);

        result = VG_CME_SIM_PUK2_REQUIRED;
      }
      break;
    }
    case SIM_REQ_ACCESS_DENIED: /* passcode given was incorrect */
    {
      if (chvNum == SIM_CHV_1)
      {
        if ((simLockGenericContext_p->simState != VG_SIM_PUK)&&
            (simLockGenericContext_p->simState != VG_SIM_READY))
        {
          vgSetSimState (VG_SIM_PIN);
        }
      }
      if (chvNum == SIM_CHV_2) /* PIN2 code required for access to be given */
      {
        if (simLockGenericContext_p->simState != VG_SIM_PUK2)
        {
          vgSetSimState (VG_SIM_PIN2);
        }
      }
      result = VG_CME_INCORRECT_PASSWORD;
      break;
    }
    default:
    { /* use sim req error mappings for other error */
      result = vgGetSimReqCmeErrorCode (state);
      break;
    }
  }

  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgGetGsmCauseCmeErrorCode
 *
 * Parameters:      code - gsmCause error code
 *
 * Returns:         ResultCode_t - CME error code value
 *
 * Description:     Maps a GsmCause value to an appropriate CME error
 *                  code
 *
 *-------------------------------------------------------------------------*/

ResultCode_t vgGetGsmCauseCmeErrorCode (const GsmCause code)
{
  ResultCode_t result = VG_CME_UNSPECIFIED_PSD_ERROR;
  Int16        causeIndex;
  Boolean      found = FALSE;

  for (causeIndex = 0;
       (causeIndex < NUM_GSM_CAUSES) && (found == FALSE);
         causeIndex++ )
  {
    if (vgGsmCauseToCmeError[causeIndex].cause == code)
    {
      result = vgGsmCauseToCmeError[causeIndex].code;
      found  = TRUE;
    }
  }

  return (result);
}


/***************************************************************************
 * Processes
 ***************************************************************************/

/* END OF FILE */

