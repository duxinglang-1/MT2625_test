/* Copyright Statement:
 *
 * (C) 2005-2016  MediaTek Inc. All rights reserved.
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

#ifndef SIMAT_PROXY_COMMON_H
#define SIMAT_PROXY_COMMON_H

////////////////////////////////////////////////////////////////////////////////////////

// all message_id value mapping design values
typedef enum  {
    SIMAT_CODE_START = 0,
    SIMAT_CODE_REJ = 1,
    SIMAT_CONFIG_REQ,
    SIMAT_CONFIG_CNF,
    SIMAT_CONFIG_REJ,
    SIMAT_BRING_UP_REQ,
    SIMAT_BRING_UP_CNF,
    SIMAT_BRING_DOWN_REQ,
    SIMAT_BRING_DOWN_IND,
    SIMAT_SOCKET_OPEN_REQ,
    SIMAT_SOCKET_OPEN_CNF,
    SIMAT_SOCKET_CLOSE_REQ,
    SIMAT_SOCKET_CLOSE_IND,
    SIMAT_SOCKET_SEND_REQ,
    SIMAT_SOCKET_SEND_CNF,
    SIMAT_SOCKET_SEND_REJ,
    SIMAT_SOCKET_RECV_IND,
    SIMAT_SOCKET_RECV_RSP,
    SIMAT_CODE_END
} simat_msg_t;

//////////////////////////////////////////////////////////////////////////////

// SIMAT_Reject_Rsp
#define CORRUPT_PACKET                                  (1)
#define MISSING_PARAMETER                               (2)
#define INVALIDAE_PARAMETER                             (3)

// SIMAT_Bring_down_Ind
#define SUCCESSFUL_BRING_DOWN                           (1)
#define FAILED_BRING_UP                                 (2)
#define PASSIVE_NETWORK_DESTRUCTION                     (3)
#define FAILED_BRING_DOWN                               (4)


//SIMAT_close_Ind
#define SOCKET_SUCCESSFUL_CLOSE                         (1)
#define SOCKET_OPEN_FAILED                              (2) /* TCP connect failed */
#define SOCKET_PASSIVE_DESTRUCTION                      (3)
#define SOCKET_NONE_EXIST                               (4)
#define SOCKET_NETWORK_ERROR                            (5)

//SIMAT_send_reject Ind
#define SIMAT_SEND_TRANSIMMSION_FAIL                    (1)
#define SIMAT_PROXY_BUSY                                (2)


//TLV data Type
#define BEARER_TYPE_APN_NAME                            (1)
#define BEARER_TYPE_PDP_TYPE                            (2)
#define BEARER_TYPE_USER_NAME                           (3)
#define BEARER_TYPE_PASSWORD                            (4)
#define TYPE_REMOTE_SOCKET                              (5)

//max length for bearer infomation bytes array length for TLV
#define SIMAT_TLV_TL_SIZE                               (2)
#define MAX_APN_SIZE                                    (50)
#define MAX_USERNAME_SIZE                               (20)
#define MAX_PASSWD_SIZE                                 (20)
#define PDP_VALUE_SIZE                                  (1)

#define MAX_IP_ADDRESS_SIZE                             (16)    /* inculde both IPV4 and IPV6 */
#define PORT_PROTOCOL_TCP                               (0x06)
#define PORT_PROTOCOL_UDP                               (0x11)

// TLV parsing using
#define SIMAT_TLV_OFFSET_TYPE                           (0)    /**< offset of the Type field from the start of the TLV */
#define SIMAT_TLV_OFFSET_LENGTH                         (1)    /**< offset of the Length field from the start of the TLV */
#define SIMAT_TLV_OFFSET_VALUE                          (2)

// remote socket
#define SIMAT_TLV_OFFSET_REMOTE_SOCKET_PORT_PROTOCOL    (2)    /**< offset of the Port Protocol field from the start of the TLV */
#define SIMAT_TLV_OFFSET_REMOTE_SOCKET_PORT_NUMBER      (3)    /**< offset of the Port Number field from the start of the TLV */
#define SIMAT_TLV_OFFSET_REMOTE_SOCKET_IP_ADDRESS       (5)    /**< offset of the IP Address field from the start of the TLV */

// bearer infomation
// APN
#define SIMAT_TLV_OFFSET_BEARER_APN_ADDRESS             (2)
// pdp type
#define SIMAT_TLV_OFFSET_BEARER_PDP_TYPE                (2)
// user name
#define SIMAT_TLV_OFFSET_BEARER_USER_NAME               (2)
// password
#define SIMAT_TLV_OFFSET_BEARER_PASSWORD                (2)


/** Extract Type field from a TLV. */
#define SIMAT_TLV_GET_TYPE(TLVAddress)  \
    (*(((unsigned char *)(TLVAddress)) + SIMAT_TLV_OFFSET_TYPE))

/** Extract Length field from a TLV. */
#define SIMAT_TLV_GET_LENGTH(TLVAddress)    \
    (*(((unsigned char *)(TLVAddress)) + SIMAT_TLV_OFFSET_LENGTH))
    /** Extract Length field from a TLV. */

/** Extract Port Protocol field from a Remote Socket TLV. */
#define SIMAT_TLV_GET_REMOTE_SOCKET_PORT_PROTOCOL(TLVAddress)	\
    (*(((unsigned char *)(TLVAddress)) + SIMAT_TLV_OFFSET_REMOTE_SOCKET_PORT_PROTOCOL))

/** Extract Port Number field from a Remote Socket TLV. */
#define SIMAT_TLV_GET_REMOTE_SOCKET_PORT_NUMBER(TLVAddress) \
    ((((unsigned short)(*(((unsigned char *)(TLVAddress)) + SIMAT_TLV_OFFSET_REMOTE_SOCKET_PORT_NUMBER))) << 8) \
     + ((unsigned short)(*(((unsigned char *)(TLVAddress)) + SIMAT_TLV_OFFSET_REMOTE_SOCKET_PORT_NUMBER + 1))))

/** Gives the start address of the IP Address field of a Remote Socket TLV.
* This value can be used with a memcpy() operation in the implementation.
*/
#define SIMAT_TLV_GET_REMOTE_SOCKET_IP_ADDRESS_START(TLVAddress)    \
    (((unsigned char *)(TLVAddress)) + SIMAT_TLV_OFFSET_REMOTE_SOCKET_IP_ADDRESS)


/** Gives the start address of the APN Address field of a bearer infomation TLV.
* This value can be used with a memcpy() operation in the implementation.
*/
#define SIMAT_TLV_GET_BEARER_APN_START(TLVAddress)  \
    (((unsigned char *)(TLVAddress)) + SIMAT_TLV_OFFSET_BEARER_APN_ADDRESS)


/*[in] the start address of the APN Address field of a bearer infomation TLV.
* [out] also give the array pointer to destination APN address.
* pay attention, the memory should be allocated by user who use it for DestinationAddr.
*/
#define SIMAT_TLV_GET_BEARER_APN(TLVAddr, DestinationAddr)  \
    memcpy((void *)DestinationAddr, (void *)SIMAT_TLV_GET_BEARER_APN_START(TLVAddr), SIMAT_TLV_GET_LENGTH(TLVAddr)-2)

/*[in] the start address of the PDP Type field of a bearer infomation TLV.
* get bearer infomation pdp type fromTLV.
*/
#define SIMAT_TLV_GET_PDP_TYPE(TLVAddr) \
    (*(((unsigned char *)(TLVAddr)) + SIMAT_TLV_OFFSET_BEARER_PDP_TYPE))


/*[in] the start address of the user name for a bearer infomation TLV.
* get bearer infomation user name fromTLV.
*/
#define SIMAT_TLV_GET_USERNAME_START(TLVAddr)   \
    ((((unsigned char *)(TLVAddr)) + SIMAT_TLV_OFFSET_BEARER_USER_NAME))


/*[in] the start address of the user name field of a bearer infomation TLV.
* [out] also give the array pointer to UserName address.
* pay attention, the memory should be allocated by user who use it for UserName.
*/
#define SIMAT_TLV_GET_BEARER_USER_NAME(TLVAddr, UserName)   \
    memcpy((void *)UserName,(void *)SIMAT_TLV_GET_USERNAME_START(TLVAddr), SIMAT_TLV_GET_LENGTH(TLVAddr)-2)



/*[in] the start address of the password for a bearer infomation TLV.
* get bearer infomation password address fromTLV.
*/
#define SIMAT_TLV_GET_PASSWORD_START(TLVAddr)   \
    ((((unsigned char *)(TLVAddr)) + SIMAT_TLV_OFFSET_BEARER_PASSWORD))


/*[in] the start address of the password Address field of a bearer infomation TLV.
* [out] also give the array pointer to password address.
* pay attention, the memory should be allocated by user who use it for password.
*/
#define SIMAT_TLV_GET_BEARER_PASSWORD(TLVAddr, Password)    \
    memcpy((void *)Password,(void *)SIMAT_TLV_GET_PASSWORD_START(TLVAddr), SIMAT_TLV_GET_LENGTH(TLVAddr)-2)


///////////////////////////////////////////////////////////////////////////////////

typedef struct {
    unsigned short simat_code;                  /* SIMAT code in data format */
    unsigned short simat_length;                /* SIMAT total length in data pacakge */
} SIMAT_Base_Header, SIMAT_Config_Conf, SIMAT_Bring_Up_Req,
SIMAT_Bring_Up_Conf, SIMAT_Bring_Down_Req;


typedef struct {
    SIMAT_Base_Header base_header;              /* basement SIMAT header */
    unsigned short reject_code;                 /* copy from code data member */
} SIMAT_Cmd_Reject;


typedef struct {
    SIMAT_Base_Header base_header;              /* basement SIMAT header */
    unsigned char bearer_info[(SIMAT_TLV_TL_SIZE + MAX_APN_SIZE) + (SIMAT_TLV_TL_SIZE + PDP_VALUE_SIZE) + (SIMAT_TLV_TL_SIZE + MAX_USERNAME_SIZE) + (SIMAT_TLV_TL_SIZE + MAX_PASSWD_SIZE)];  /* bearer info */
} SIMAT_Config_Req;


typedef struct {
    SIMAT_Base_Header base_header;              /* basement SIMAT header */
    unsigned char cause;                        /* reject cause */
} SIMAT_Reject_Rsp, SIMAT_Config_Reject, SIMAT_Bring_Down_Ind;


typedef struct {
    SIMAT_Base_Header base_header;              /* basement SIMAT header */
    unsigned int socket_id;                     /* socket id for SIMAT using */
} SIMAT_Soc_Base, SIMAT_Open_Conf, SIMAT_Close_Req, SIMAT_Send_Data_Conf,
SIMAT_Recv_Data_Rsp;

typedef struct {
    SIMAT_Soc_Base soc_base;                /* basement SIMAT header */
    unsigned char remote_socket_info[SIMAT_TLV_OFFSET_REMOTE_SOCKET_IP_ADDRESS + MAX_IP_ADDRESS_SIZE];
} SIMAT_SOC_Open_Req;

typedef struct {
    SIMAT_Soc_Base soc_base;
#ifndef __CC_ARM                            //for user data for this socket
    unsigned char data[0];
#else
    unsigned char data[];
#endif
} SIMAT_Data_Req, SIMAT_Send_Data_Req, SIMAT_Recv_Data_Ind;

typedef struct {
    SIMAT_Soc_Base soc_base;
    unsigned char cause;
} SIMAT_Close_Ind, SIMAT_Send_Data_Reject;


//////////////////////////////////////////////////////////////////////////////////////////////////////

#endif /* #ifndef SIMAT_PROXY_COMMON_H */
