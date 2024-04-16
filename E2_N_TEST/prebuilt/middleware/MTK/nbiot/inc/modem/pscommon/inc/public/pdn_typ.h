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
 *************************************************************************/
/** \file
 * Eutra/Utra/GPRS Protocol Stack Common Data Types
 * PDN type and address type definitions
 *************************************************************************/

#ifndef PDN_TYP_H
#define PDN_TYP_H

/***************************************************************************
* Nested Include Files
***************************************************************************/

#include <system.h>

/***************************************************************************
 *   Manifest Constants
 ***************************************************************************/

/***************************************************************************
*   Macro Functions
***************************************************************************/

/** \def IPV4_ADDR_LEN
 *  Defines the length of IPv4 address */
#define IPV4_ADDR_LEN          4

/** \def MAX_IPV6_ADDR_LEN
 *  Defines the maximum length of IPv6 address data */
#define MAX_IPV6_ADDR_LEN      16

/** \def NONIP_SPARE_ADDR_LEN
 *  Defines the nummber of spare octects for the address information when
 *  the PDN type is "non IP"
 */
#define NONIP_SPARE_ADDR_LEN   4

/** \def IPV6_ADDR_INTERFACE_ID_LEN
 *  Defines the length of the interface identifier in IPv6 address
 *  Interface identifier is 8 octets */
#define IPV6_ADDR_INTERFACE_ID_LEN              8

/** \def IPV6_ADDR_INTERFACE_ID_OFFSET
 *  Defines the offset of the interface identifier in IPv6 address
 *  Interface identifier is in the least significant 8 octets of the IPv6 address */
#define IPV6_ADDR_INTERFACE_ID_OFFSET           8

/** Maximum length of PSD username used when activated */
#define MAX_PSD_USERNAME_LENGTH                 50
/** Maximum length of PSD password used when activated */
#define MAX_PSD_PWD_LENGTH                      50

/** The MAX_TEXTUAL_PDN_ADDR value of 64 allows for ipv6:
 *  0000:0000:0000:0000:0000:0000:0000:0000; and ipv6 with embedded
 *  ipv4: 0000:0000:0000:0000:0000:0000:000.000.000.000; and the 3gpp
 *  dotty-decimal form for ipv6:
 *  000.000.000.000.000.000.000.000.000.000.000.000.000.000.000.000.
 */
#define MAX_TEXTUAL_PDN_ADDR 64

/* Maximum number of digits a field in a dot separated IP address
 */
#define MAX_FIELD_DIGITS_IN_DOT_SEPARATED_IP_ADDRESS    3

/* Maximum number of bits that can be set in an IPV6 subnet mask */
#define MAX_IPV6_SUBNET_MASK_BITS                       (MAX_IPV6_ADDR_LEN * 8)
/***************************************************************************
*   Types
***************************************************************************/

/** PDN type:
 * The PDN type indicates the IP version capability of the IP stack
 * associated with the UE.
 * ref. 24.301/9.9.4.10.
 */
typedef enum PdnTypeTag
{
    /** IPv4 */
    PDN_TYPE_IPV4       = 1,
    /** IPv6 */
    PDN_TYPE_IPV6       = 2,
    /** IPv4v6 */
    PDN_TYPE_IPV4V6     = 3,
    /** Unused */
    PDN_TYPE_UNUSED     = 4,
    /** non IP */
    PDN_TYPE_NONIP      = 5
}
PdnType;

/** IPv6 address type:
 * Indicates if the IPv6 address contains an interface identifier
 * (8 bytes) or a full IPv6 address (16 bytes).
 */
typedef enum Ipv6AddressTypeTag
{
    /** Full IPv6 address, 16 bytes long. This is used for 2/3G only. */
    IPV6_ADDRESS_FULL,
    /** Interface identifier for IPv6 address, 8 bytes long. */
    IPV6_ADDRESS_INTERFACE_IDENTIFIER
}
Ipv6AddressType;

/* MTU request according to AT command AT+CGDCONT */
typedef enum MtuSizeRequestedTag
{
  MTU_SIZE_NOT_REQUESTED,
  MTU_SIZE_REQUESTED
} MtuSizeRequested;

/** PDN address:
 * PDN address is used to to assign an IPv4 and/or IPv6 address to the UE
 * associated with a packet data network or to provide the UE with an interface
 * identifier to be used to build the IPv6 link local address.
 * ref. 24.301/9.9.4.9.
 */
typedef struct PdnAddressTag
{
    /** Indicates the type of address */
    PdnType             pdnType;
    /** Indicates if the addrees (either IPV4 or IPV6) is present or not.
     * Note that if the pdnType is set to IPV4 then the IPV6 address is not present;
     * if pdnType is set to IPV6 then IPV4 address is not present;
     * and if pdnType is set to IPV4V6 then both addresses are present.
     * However, it addressPresent is FALSE then none are present, however the pdnType
     * can still be valid (for example in the smrPdnConnectReq where only the pdnType is
     * needed).
\assoc PRESENT \ref ipv4Address
\assoc PRESENT \ref ipv6AddressType
\assoc PRESENT \ref ipv6Address */
    Boolean             addressPresent;
    /** IPv4 address information */
    Int8                ipv4Address [IPV4_ADDR_LEN];
    /** Indicates the type of IPv6 address */
    Ipv6AddressType     ipv6AddressType;
    /** IPv6 address information */
    Int8                ipv6Address [MAX_IPV6_ADDR_LEN];
}
PdnAddress;

/** APN Rate Control Parameters
 *
 * APN Rate Control Parameters are received from network as a part of
 * the Extented Protocol Configuration Options (ePCO)
 *
 * 3GPP TS 24.301, 9.9.4.26
 * 3GPP TS 24.008, 10.5.6.3
 */
typedef struct ApnRateControlParamsTag
{

/*
\assoc PRESENT \ref additionalExceptionReportsAllowed
\assoc PRESENT \ref uplinkTimeUnit
\assoc PRESENT \ref maxUplinkRate
*/

    /** APN Rate Control Parameters present or not */
    Boolean             apnRateControlPresent;

    /** Additional exception reports at maximum rate reached allowed or not */
    Boolean             additionalExceptionReportsAllowed;
    /** Uplink time unit */
    Int8                uplinkTimeUnit;
    /** Maximum uplink rate */
    Int32               maxUplinkRate;
}
ApnRateControlParams;

typedef struct PcoMtuParamsTag
{
   Boolean                   ipv4MTUPresent;
   Int16                     ipv4LinkMTU;
   Boolean                   nonIPMTUPresent;
   Int16                     nonIPLinkMTU;
}
PcoMtuParams;

/** PDN Connection Address Info.
 * This information contains all the IP addressing and username/password
 * required to configure a PDP connection (PDP context for 2G/3G,
 * EPS bearer for LTE).
 */
typedef struct PdnConnectionAddressInfoTag
{
    /** PDN address.  Note that the pdnType of all other addresses should match
     * the pdnType of the pdnAddress.
     */
    PdnAddress              pdnAddress;
    /** Primary DNS address */
    PdnAddress              primaryDnsAddress;
    /** Secondary DNS address */
    PdnAddress              secondaryDnsAddress;
    /** Gateway address */
    PdnAddress              gatewayAddress;
    /** SubnetMask */
    PdnAddress              subnetMask;
}
PdnConnectionAddressInfo;

/** This structure provides the username and password (if any)
 * associated with a PSD connection
 */
typedef struct PsdUserTag
{
  /** Username field present
\assoc PRESENT \ref usernameLength
\assoc PRESENT \ref username */
  Boolean usernamePresent;
  /** Username field length
   *
\assoc ARRAY \ref username */
  Int8    usernameLength;
  /** Username */
  Char    username [MAX_PSD_USERNAME_LENGTH];
  /** Password field present
\assoc PRESENT \ref passwdLength
\assoc PRESENT \ref passwd */
  Boolean passwdPresent;
  /** Password field length
\assoc ARRAY \ref passwd */
  Int8    passwdLength;
  /** Password */
  Char    passwd [MAX_PSD_PWD_LENGTH];
} PsdUser;

/** PDP address in textual format */
typedef struct TextualPdnAddressTag
{
    /** Address present flag
\assoc PRESENT \ref length
\assoc PRESENT \ref address
     */
    Boolean addressPresent;
    /** Length of address
\assoc ARRAY \ref address */
    Int8    length;
    /** Address data - must add 1 because we may have an additional
     *  character for the separation char betweeen the source address and
     *  subnet mask
     */
    Int8    address [MAX_TEXTUAL_PDN_ADDR + 1];
}
TextualPdnAddress;

#endif /* PDN_TYP_H */

/* END OF FILE */



