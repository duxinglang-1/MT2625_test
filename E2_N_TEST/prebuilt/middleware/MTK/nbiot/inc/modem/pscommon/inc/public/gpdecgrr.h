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
 *  File Description :
 *
 *      GPRS Packet System Information types and definitions
 *
 **************************************************************************/

#if !defined (GPDECGRR_H)
#define GPDECGRR_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include <gpgrrtyp.h>

/***************************************************************************
 * Manifest Constants
 **************************************************************************/

/***************************************************************************
 * Typed Constants
 **************************************************************************/

/***************************************************************************
 * Types
 **************************************************************************/

/*
 * In-line comments in the following enums indicate the decode functions to
 * be called to decode a particular 'OptIeType' in each message
 */

typedef enum PccoOptIeTypeTag
{
    /*      Header fields:             GpDecPacketCellChangeOrder           */
    PCCO_OPTIE_GSM_TARGET    = 0,   /* GpDecPccoGsmTarget                   */
    PCCO_OPTIE_NCELL         = 1,   /* GpDecPccoNcell                       */
    PCCO_OPTIE_NCELL_LSA     = 2,   /* GpDecPccoNcellLsa                    */
    PCCO_OPTIE_ENH_MEAS      = 3,
    PCCO_OPTIE_R4_ADDIT      = 4,
#if defined (UPGRADE_3G) && defined (UPGRADE_GGE)
    PCCO_OPTIE_3G_TARGET     = 5,
    PCCO_OPTIE_R5_ADDIT      = 6,
    PCCO_OPTIE_R6_ADDIT      = 7,
    PCCO_OPTIE_PADDING       = 8,
#else
    PCCO_OPTIE_R5_ADDIT      = 5,
    PCCO_OPTIE_R6_ADDIT      = 6,
    PCCO_OPTIE_PADDING       = 7,
#endif
    PCCO_NUM_OPTIE
}
PccoOptIeType;

typedef enum PmoOptIeTypeTag
{
    /*      Header fields:             GpDecPacketMeasurementOrder          */
    PMO_OPTIE_NC_MEAS_PARAM  = 0,   /* GpDecPmoNcMeas                       */
    PMO_OPTIE_NCELL          = 1,   /* GpDecPmoNcell                        */
    PMO_OPTIE_EXT            = 2,   /* GpDecPmoExt                          */
    PMO_OPTIE_NCELL_LSA      = 3,   /* GpDecPmoNcellLsa                     */
    PMO_OPTIE_ENH_MEAS       = 4,
    PMO_OPTIE_R4_ADDIT       = 5,
    PMO_OPTIE_R5_ADDIT       = 6,
    PMO_OPTIE_R6_ADDIT       = 7,
    PMO_OPTIE_PADDING        = 8,
    PMO_NUM_OPTIE            = 9
}
PmoOptIeType;

typedef enum Psi2OptIeTypeTag
{
    /*      Header fields:             GpDecPsiType2             */
    PSI2_OPTIE_CELL_ID       = 0,   /* GpDecPsiType2CellId       */
    PSI2_OPTIE_NGCO          = 1,   /* GpDecPsiType2Ngco         */
    PSI2_OPTIE_RFL           = 2,   /* GpDecPsiType2Rfl          */
    PSI2_OPTIE_CELL_ALLOC    = 3,   /* GpDecPsiType2CellAlloc    */
    PSI2_OPTIE_MOB_ALLOC     = 4,   /* GpDecPsiType2MobAlloc     */
    PSI2_OPTIE_PCCCH         = 5,   /* GpDecPsiType2Pccch        */
    PSI2_OPTIE_PADDING       = 6,
    PSI2_COMPACT_CTRL_INFO   = 7,   /* R99 IE */
    PSI2_ADDIT_PSI_MSGS      = 8,   /* R99 IE */
    PSI2_NUM_OPTIE
}
Psi2OptIeType;


typedef enum Psi3OptIeTypeTag
{
    /*      Header fields:             GpDecPsiType3               */
    PSI3_OPTIE_SCELL         = 0,   /* GpDecPsiType3Scell          */
    PSI3_OPTIE_GEN_CELL      = 1,   /* GpDecPsiType3GenCell        */
    PSI3_OPTIE_NCELL         = 2,   /* GpDecPsiType3Ncell          */
    PSI3_OPTIE_SCELL_LSA     = 3,   /* GpDecPsiType3ScellLsa       */
    PSI3_OPTIE_NCELL_LSA     = 4,   /* GpDecPsiType3NcellLsa       */
    PSI3_OPTIE_CMPCT_CELL_ID = 5,
    PSI3_OPTIE_CMPCT_NCELL   = 6,
    PSI3_OPTIE_CCN           = 7,
    PSI3_OPTIE_PADDING       = 8,
    PSI3_NUM_OPTIE           = 9
}
Psi3OptIeType;

typedef enum Psi3bisOptIeTypeTag
{
    /*      Header fields:             GpDecPsiType3bis            */
    PSI3BIS_OPTIE_NCELL        = 0,   /* GpDecPsiType3bisNcell       */
    PSI3BIS_OPTIE_NCELL2       = 1,   /* GpDecPsiType3bisNcell2      */
    PSI3BIS_OPTIE_NCELL_LSA    = 2,   /* GpDecPsiType3bisNcellLsa    */
    PSI3BIS_OPTIE_CMPCT_NCELL2 = 3,
    PSI3BIS_OPTIE_CCN          = 4,
    PSI3BIS_OPTIE_PADDING      = 5,
    PSI3BIS_NUM_OPTIE          = 6
}
Psi3bisOptIeType;

/***************************************************************************
 *  Signal Definitions
 **************************************************************************/

/***************************************************************************
 *  FUNCTION PROTOTYPES
 **************************************************************************/

DecodeResult
GpDecPacketCellChangeOrder      (const     Int8              *rawMsg_p,
                                 Int16                       *bitOffset_p,
                                 PacketCellChangeOrder       *pcco_p,
                                 PccoOptIeType               *pccoType_p);

DecodeResult
GpDecPccoGsmTarget (const     Int8              *rawMsg_p,
                    Int16                       *bitOffset_p,
                    PacketCellChangeOrderBody   *pcco_p,
                    PccoOptIeType               *pccoType_p,
                    Int16                        lenInBits);

DecodeResult
GpDecPccoNcell (const     Int8              *rawMsg_p,
                Int16                       *bitOffset_p,
                NcellParameters             *pcco_p,
                PccoOptIeType               *pccoType_p,
                Int16                        lenInBits);

DecodeResult
GpDecPccoNcellLsa (const  Int8              *rawMsg_p,
                   Int16                    *bitOffset_p,
                   NcellLsaInfo             *pcco_p,
                   PccoOptIeType            *pccoType_p,
                   Int16                     lenInBits);

DecodeResult
GpDecPacketMeasurementOrder     (const     Int8              *rawMsg_p,
                                 Int16                       *bitOffset_p,
                                 PacketMeasurementOrder      *pmo_p,
                                 PmoOptIeType                *pmoType_p,
                                 Int16                        lenInBits);

DecodeResult
GpDecPmoNcMeas (const     Int8              *rawMsg_p,
                Int16                       *bitOffset_p,
                PacketMeasurementOrderBody  *pmo_p,
                PmoOptIeType                *pmoType_p,
                Int16                        lenInBIts);

DecodeResult
GpDecPmoNcell (const     Int8             *rawMsg_p,
               Int16                      *bitOffset_p,
               NcellParameters            *pmo_p,
               PmoOptIeType               *pmoType_p,
               Int16                       lenInBIts);

DecodeResult
GpDecPmoExt  (const     Int8              *rawMsg_p,
              Int16                       *bitOffset_p,
              ExtMeasurementParam         *pmo_p,
              PmoOptIeType                *pmoType_p,
              Int16                        lenInBIts);

DecodeResult
GpDecPmoNcellLsa (const     Int8          *rawMsg_p,
                  Int16                   *bitOffset_p,
                  NcellLsaInfo            *pmo_p,
                  PmoOptIeType            *pmoType_p,
                  Int16                    lenInBIts);

DecodeResult
GpDecPacketPrachParameters      (const     Int8              *rawMsg_p,
                                 PacketPrachParameters       *ppp_p,
                                 Int16                        lenInBits);

DecodeResult
GpDecPsiType1                   (const     Int8              *rawMsg_p,
                                 Int16                       *bitOffset_p,
                                 PacketSysInfoType1          *psi1_p);

DecodeResult GpDecPsiType1Body (const     Int8           *rawMsg_p,
                                Int16                    *bitOffset_p,
                                PacketSysInfoType1Body   *psi1_p,
                                Int16                     lenInBits);

DecodeResult
GpDecPsiType2                   (const     Int8              *rawMsg_p,
                                 Int16                       *bitOffset_p,
                                 PacketSysInfoType2          *psi2_p,
                                 Psi2OptIeType               *psi2Type_p,
                                 Int16                       lenInBits);

DecodeResult
GpDecPsiType2CellId             (const     Int8              *rawMsg_p,
                                 Int16                       *bitOffset_p,
                                 GprsCellIdentification      *psi2_p,
                                 Psi2OptIeType               *psi2OptIe_p,
                                 Int16                        lenInBits);

DecodeResult
GpDecPsiType2Ngco               (const     Int8              *rawMsg_p,
                                 Int16                       *bitOffset_p,
                                 NonGprsCellOptions          *psi2_p,
                                 Psi2OptIeType               *psi2OptIe_p,
                                 Int16                        lenInBits);

DecodeResult
GpDecPsiType2Rfl                (const     Int8              *rawMsg_p,
                                 Int16                       *bitOffset_p,
                                 RefFrequencyList            *psi2_p,
                                 Psi2OptIeType               *psi2OptIe_p,
                                 Int16                        lenInBits);

DecodeResult
GpDecPsiType2CellAlloc          (const     Int8              *rawMsg_p,
                                 Int16                       *bitOffset_p,
                                 GprsCellAllocation          *psi2_p,
                                 Psi2OptIeType               *psi2OptIe_p,
                                 Int16                        lenInBits);

DecodeResult
GpDecPsiType2MobAlloc           (const     Int8              *rawMsg_p,
                                 Int16                       *bitOffset_p,
                                 GprsMobileAllocation        *psi2_p,
                                 Psi2OptIeType               *psi2OptIe_p,
                                 Int16                        lenInBits);

DecodeResult
GpDecPsiType2Pccch              (const     Int8              *rawMsg_p,
                                 Int16                       *bitOffset_p,
                                 PccchDescription            *psi2_p,
                                 Psi2OptIeType               *psi2OptIe_p,
                                 Int16                        lenInBits);

DecodeResult
GpDecPsiType3                   (const     Int8              *rawMsg_p,
                                 Int16                       *bitOffset_p,
                                 PacketSysInfoType3          *psi3_p);

DecodeResult
GpDecPsiType3Scell              (const     Int8              *rawMsg_p,
                                 Int16                       *bitOffset_p,
                                 ScellParameters             *psi3_p);

DecodeResult
GpDecPsiType3GenCell            (const     Int8              *rawMsg_p,
                                 Int16                       *bitOffset_p,
                                 GenCellSelectionParam       *psi3_p,
                                 Psi3OptIeType               *psi3OptIe_p,
                                 Int16                        lenInBits);

DecodeResult
GpDecPsiType3Ncell              (const     Int8              *rawMsg_p,
                                 Int16                       *bitOffset_p,
                                 NcellParameters             *psi3_p,
                                 Psi3OptIeType               *psi3OptIe_p,
                                 Int16                        lenInBits);

DecodeResult
GpDecPsiType3ScellLsa           (const     Int8              *rawMsg_p,
                                 Int16                       *bitOffset_p,
                                 ScellLsaInfo                *psi3_p,
                                 Psi3OptIeType               *psi3OptIe_p,
                                 Int16                        lenInBits);

DecodeResult
GpDecPsiType3NcellLsa           (const     Int8              *rawMsg_p,
                                 Int16                       *bitOffset_p,
                                 NcellLsaInfo                *psi3_p,
                                 Psi3OptIeType               *psi3Type_p,
                                 Int16                        lenInBits);

DecodeResult
GpDecPsiType3bis                (const     Int8              *rawMsg_p,
                                 Int16                       *bitOffset_p,
                                 PacketSysInfoType3bis       *psi3bis_p,
                                 Psi3bisOptIeType            *psi3bisOptIe_p,
                                 Int16                        lenInBits);

DecodeResult
GpDecPsiType3bisNcell           (const     Int8              *rawMsg_p,
                                 Int16                       *bitOffset_p,
                                 NcellParameters             *psi3bis_p,
                                 Psi3bisOptIeType            *psi3bisOptIe_p,
                                 Int16                        lenInBits);

DecodeResult
GpDecPsiType3bisNcell2          (const     Int8              *rawMsg_p,
                                 Int16                       *bitOffset_p,
                                 NcellParameters             *psi3bis_p,
                                 Psi3bisOptIeType            *psi3bisOptIe_p,
                                 Int16                        lenInBits);

DecodeResult
GpDecPsiType3bisNcellLsa        (const     Int8              *rawMsg_p,
                                 Int16                       *bitOffset_p,
                                 NcellLsaInfo                *psi3bis_p,
                                 Psi3bisOptIeType            *psi3bisType_p,
                                 Int16                        lenInBits);

DecodeResult
GpDecPsiType4                   (const     Int8              *rawMsg_p,
                                 Int16                       *bitOffset_p,
                                 PacketSysInfoType4          *psi4_p);

DecodeResult
GpDecPsi4ChannelGroup           (const     Int8              *rawMsg_p,
                                 Int16                       *bitOffset_p,
                                 IntChannelGroup             *chanGroup_p,
                                 Int16                        lenInBits);

DecodeResult
GpDecPsiType5                   (const     Int8              *rawMsg_p,
                                 Int16                       *bitOffset_p,
                                 PacketSysInfoType5          *psi5_p);

DecodeResult
GpDecPsiType5Body               (const     Int8              *rawMsg_p,
                                 Int16                       *bitOffset_p,
                                 PacketSysInfoType5Body      *psi5_p,
                                 Int16                        lenInBits);

DecodeResult
GpDecPsiType13                  (const     Int8              *rawMsg_p,
                                 Int16                       *bitOffset_p,
                                 PacketSysInfoType13         *psi13_p);

DecodeResult
GpDecPsiType13Body              (const     Int8              *rawMsg_p,
                                 Int16                       *bitOffset_p,
                                 PacketSysInfoType13Body     *psi13_p,
                                 Int16                        lenInBits);

DecodeResult
GpDecPsiType14 (const     Int8            *rawMsg_p,
                Int16                     *bitOffset_p,
                PacketSysInfoType14       *psi14_p,
                Int16                      lenInBits);

DecodeResult
GpDecPsiType8                   (const     Int8              *rawMsg_p,
                                 Int16                       *bitOffset_p,
                                 PacketSysInfoType8          *psi8_p);

DecodeResult
GpDecPsiType8Body               (const     Int8              *rawMsg_p,
                                 Int16                       *bitOffset_p,
                                 PacketSysInfoType8Body      *psi8_p,
                                 Int16                        lenInBits);

DecodeResult
GpDecCompactCtrlInfo (const Int8            *rawMsg_p,
                            Int16           *bitOffset_p,
                            CompactCtrlInfo *compactCtrlInfo_p,
                            Psi2OptIeType   *psi2Type_p,
                            Int16            lenInBits);

DecodeResult
GpDecAdditionalPsiMsgs (const   Int8                *rawMsg_p,
                                Int16               *bitOffset_p,
                                AdditionalPsiMsgs   *additionalPsiMsgs_p,
                                Psi2OptIeType       *psi2Type_p,
                                Int16                lenInBits);

DecodeResult
GpDecPsiType3CmpctNcellInfo (const     Int8                 *rawMsg_p,
                                       Int16                *bitOffset_p,
                                       CompactNcellInfo     *psi3_p,
                                       Psi3OptIeType        *psi3Type_p,
                                       Int16                 lenInBits);


DecodeResult
GpDecPsi3bisCmpctNcellParam2 (const Int8                 *rawMsg_p,
                                    Int16                *bitOffset_p,
                                    CompactNcellInfo     *cmpctNcell_p,
                                    Psi3bisOptIeType     *psi3bisType_p,
                                    Int16                 lenInBits);

DecodeResult
GpDecPccoEnhMeasParam(const     Int8            *rawMsg,
                      Int16                     *bitOffset,
                      EnhMeasurementParam       *enhMeasParam,
                      PccoOptIeType             *pccoType_p,
                      Int16                      lenInBits);

DecodeResult
GpDecPmoEnhMeasParam(const     Int8            *rawMsg,
                     Int16                     *bitOffset,
                     EnhMeasurementParam       *enhMeasParam,
                     PmoOptIeType              *pmoType_p,
                     Int16                      lenInBits);

void
GpDecPsiType3quaHdr (const     Int8            *rawMsg,
                     Int16                     *bitOffset,
                     PacketSysInfoType3quaHdr  *psi3qua);

Boolean
GpDecPsiType3quaBody (const     Int8            *rawMsg,
                      Int16                     *bitOffset,
                      Int16                      lenInBits,
                      PacketSysInfoType3quaBody *psi3qua);

void
GpDecPsiType3terHdr (const     Int8            *rawMsg,
                     Int16                     *bitOffset,
                     PacketSysInfoType3terHdr  *psi3terHdr);

Boolean
GpDecPsiType3terBody (const     Int8               *rawMsg,
                      Int16                        *bitOffset,
                      Int16                         lenInBits,
                      PacketSysInfoType3terBody    *psi3ter);

DecodeResult
GpDecPccoR4Addit(const     Int8            *rawMsg,
                 Int16                     *bitOffset,
                 PccoRel4Additions         *rel4Addit,
                 PccoOptIeType             *pccoType_p,
                 Int16                      lenInBits);

DecodeResult
GpDecPmoR4Addit(const     Int8            *rawMsg,
                 Int16                    *bitOffset,
                 PmoRel4Additions         *rel4Addit,
                 PmoOptIeType             *pmoType,
                 Int16                     lenInBits);

#if defined (UPGRADE_3G) && defined (UPGRADE_GGE)
DecodeResult
GpDecPccoR5Addit(const     Int8            *rawMsg,
                 Int16                     *bitOffset,
                 PccoRel5Additions         *rel5Addit,
                 PccoOptIeType             *pccoType,
                 Int16                      lenInBits);

DecodeResult
GpDecPccoR6Addit(const     Int8            *rawMsg,
                 Int16                     *bitOffset,
                 PccoRel6Additions         *rel6Addit,
                 PccoOptIeType             *pccoType,
                 Int16                      lenInBits);

DecodeResult
GpDecPmoR5Addit(const     Int8            *rawMsg,
                Int16                     *bitOffset,
                PmoRel5Additions          *rel5Addit,
                PmoOptIeType              *pmoType,
                Int16                      lenInBits);

DecodeResult
GpDecPmoR6Addit(const     Int8            *rawMsg,
                Int16                     *bitOffset,
                PmoRel6Additions          *rel6Addit,
                PmoOptIeType              *pmoType,
                Int16                      lenInBits);

DecodeResult
GpDecPcco3gTarget (const     Int8              *rawMsg_p,
                   Int16                       *bitOffset_p,
                   PacketCellChangeOrderBody   *pcco_p,
                   Int16                        lenInBits);

DecodeResult
GpDecPsiType15Body (const     Int8            *rawMsg_p,
                    Int16                     *bitOffset_p,
                    PacketSysInfoType15       *psi15_p,
                    Int16                      lenInBits);
#endif  /* UPGRADE_3G && UPGRADE_GGE */

#endif

/* END OF FILE */



