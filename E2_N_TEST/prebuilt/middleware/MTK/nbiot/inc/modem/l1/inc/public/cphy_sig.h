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
 * 3G L1 Primitive Data Type definitions for the CPHY Interface
 *************************************************************************/

#if !defined (CPHY_SIG_H)
#define       CPHY_SIG_H

/***************************************************************************
* Nested Include Files
***************************************************************************/

#include <system.h>
#include <ki_typ.h>
#include <uas_asn.h>
#include <u1_typ.h>
#include <l13_typ.h>
#include <ubnd_cfg.h>

# if defined (UPGRADE_LTE)
#include <el1_typ.h>
# endif
/** \defgroup SigCphy CPHY Signalling Interface
 * \ingroup Layer13G
 * \ingroup Sig
 *
 * This section defines all the primitives exchanged on the control plane of
 * RRC and 3G L1, hereinafter called the CPHY interface.
 *
 * The CPHY interface is illustrated in the figure below:
 *
 * \image html prd3gdualmodeinterface.png "Fig: Dual Mode Architecture"
 * \image latex prd3gdualmodeinterface.eps "Dual Mode Architecture" width=12cm
 *
 * All the primitives have been categorised based on the common purpose for
 * which they are exchanged. For example, all the primitives exchanged during
 * initial cell selection and reselection are put into <tt>Cell Selection and
 * Re-selection Primitives</tt> category.
 *
 * \section SigCphyParDesc Common Parameter Description
 * <B>Common Parameter Description</B>
 *
 * Many of the configuration primitives (Req and Cnf) include the following
 * parameters:
 *
 * <TABLE>
 * <TR><TD><B>Parameter Type</B></TD>    <TD><B>Name               </B></TD></TR>
 * <TR><TD>Int8                 </TD>    <TD>configIdentifier          </TD></TR>
 * <TR><TD>Int8                 </TD>    <TD>configNoOfMessages        </TD></TR>
 * <TR><TD>Int8                 </TD>    <TD>configMessageNo           </TD></TR>
 * </TABLE>
 *
 * These are used to group configuration primitives together so that the
 * physical layer knows how many messages to expect for a configuration event.
 * The \c configIdentifier is allocated by RRC and has the same value for all
 * primitives that need to be processed by the physical layer at the same time.
 * The \c configNoOfMessages tells the physical layer how many primitives there
 * are in total in this group of primitives. The \c configMessageNo identifies
 * an individual primitive in this group and is in the range 1 to \c
 * configNoOfMessages.
 *
 * When these parameters are used in Req primitives the physical layer is
 * expected to send only one #CphyCcTrChConfigCnf primitive to confirm the whole
 * group of Req primitives. This is to reduce the signalling bandwidth on the
 * CPHY interface.
 *
 * <B> CcTrch Index </B>
 *
 * A number of primitives use a CcTrCh index to identify the mapping of transport
 * channels to physical channels., plus   The following index values are used:
 *
 * <TABLE>
 * <TR><TD><B>#DEFINE              </B></TD>    <TD><B>VALUE         </B></TD></TR>
 * <TR><TD>CCTRCH_SCCPH_0              </TD>    <TD>0                    </TD></TR>
 * <TR><TD>CCTRCH_SCCPH_1              </TD>    <TD>1                    </TD></TR>
 * <TR><TD>CCTRCH_SCCPH_FOR_MBMS_0     </TD>    <TD>3                    </TD></TR>
 * <TR><TD>CCTRCH_SCCPH_FOR_MBMS_1     </TD>    <TD>4                    </TD></TR>
 * <TR><TD>CCTRCH_SCCPH_FOR_MBMS_2     </TD>    <TD>5                    </TD></TR>
 * <TR><TD>CCTRCH_DPCH                 </TD>    <TD>0                    </TD></TR>
 * </TABLE>
 *
 * These are used in the layer 1 states as follows:
 *
 * <TABLE>
 * <TR><TD><B>STATE                </B></TD>    <TD><B>OPTION 1          </B></TD> <TD><B>OPTION 2         </B></TD></TR>
 * <TR><TD><B>IDLE                 </B></TD>    <TD>CCTRCH_SCCPH_0 carries PCH and CTCH  </TD> <TD>CCTRCH_SCCPH_0 carries PCH </TD></TR>
 * <TR><TD>                            </TD>    <TD>                                     </TD> <TD>CCTRCH_SCCPH_1 carries CTCH</TD></TR>
 * <TR><TD>                            </TD>    <TD>CCTRCH_SCCPCH_FOR_MBMS_0 carries MCCH</TD> <TD>                           </TD></TR>
 * <TR><TD>                            </TD>    <TD>CCTRCH_SCCPCH_FOR_MBMS_1 carries MTCH and MSCH</TD> <TD>                           </TD></TR>
 * <TR><TD>                            </TD>    <TD>CCTRCH_SCCPCH_FOR_MBMS_2 carries MTCH and MSCH</TD> <TD>                           </TD></TR>
 * <TR><TD><B>FACH                 </B></TD>    <TD>CCTRCH_SCCPH_0 carries FACH          </TD> <TD>                           </TD></TR>
 * <TR><TD>                            </TD>    <TD>CCTRCH_SCCPCH_FOR_MBMS_0 carries MCCH</TD> <TD>                           </TD></TR>
 * <TR><TD>                            </TD>    <TD>CCTRCH_SCCPCH_FOR_MBMS_1 carries MTCH and MSCH</TD> <TD>                           </TD></TR>
 * <TR><TD>                            </TD>    <TD>CCTRCH_SCCPCH_FOR_MBMS_2 carries MTCH and MSCH</TD> <TD>                           </TD></TR>
 * <TR><TD><B>DCH                  </B></TD>    <TD>CCTRCH_SCCPH_0 carries DCH          </TD> <TD>                           </TD></TR>
 * <TR><TD>                            </TD>    <TD>CCTRCH_SCCPCH_FOR_MBMS_0 carries MCCH</TD> <TD>                           </TD></TR>
 * <TR><TD>                            </TD>    <TD>CCTRCH_SCCPCH_FOR_MBMS_1 carries MTCH and MSCH</TD> <TD>                           </TD></TR>
 * <TR><TD>                            </TD>    <TD>CCTRCH_SCCPCH_FOR_MBMS_2 carries MTCH and MSCH</TD> <TD>                           </TD></TR>
 * </TABLE>
 *
 * The Option 1 and 2 for IDLE mode is required because it is possible to have the CBS service
 * mapped to a CTCH, which is either on the same SCCPCH as the PCH or on a separate SCCPCH.
 *
 * <B> Physical Layer State Transitions </B>
 *
 * A typical physical layer state transition diagram is illustrated as below:
 *
 * \image html prd3gphystatetrans.png "Fig: Physical Layer State Transitions"
 * \image latex prd3gphystatetrans.eps "Physical Layer State Transitions" width=12cm
 *
 * <B> Implicit Actions </B>
 *
 * Furthermore, to reduce the signalling between the protocol stack and the
 * physical layer there are a number of implicit actions as defined below:
 *
 * <TABLE>
 * <TR><TD><B>Cell types UE scenario              </B></TD> <TD><B>Intra-frequency Measurements</B></TD> <TD><B>Inter-frequency Measurements</B></TD> <TD><B>Intra-RAT Measurements  </B></TD></TR>
 * <TR><TD>Inter-freq HHO - timing initialised (FDD only)         </TD> <TD>L1 stops implicitly                </TD> <TD>L1 stops implicitly                </TD> <TD>L1 stops implicitly            </TD></TR>
 * <TR><TD>Inter-freq HHO - timing maintained (FDD only)          </TD> <TD>L1 stops implicitly                </TD> <TD>L1 stops implicitly                </TD> <TD>L1 stops implicitly            </TD></TR>
 * <TR><TD>Inter-freq HO - CPI or PF changed (TDD128 only)        </TD> <TD>L1 stops implicitly                </TD> <TD>L1 stops implicitly                </TD> <TD>L1 stops implicitly            </TD></TR>
 * <TR><TD>Inter-freq HHO failure - timing initialised (FDD only) </TD> <TD>L1 restores                        </TD> <TD>L1 restores                        </TD> <TD>L1 restores                    </TD></TR>
 * <TR><TD>Inter-freq HHO failure - timing maintained (FDD only)  </TD> <TD>L1 restores                        </TD> <TD>L1 restores                        </TD> <TD>L1 restores                    </TD></TR>
 * <TR><TD>Inter-freq HO failure - CPI or PF changed (TDD128 only)</TD> <TD>L1 restores                        </TD> <TD>L1 restores                        </TD> <TD>L1 restores                   </TD></TR>
 * <TR><TD>Intra-freq HHO - timing initialised (FDD only)          </TD> <TD>L1 continues                       </TD> <TD>L1 stops implicitly                </TD> <TD>L1 stops implicitly           </TD></TR>
 * <TR><TD>Intra-freq HHO - timing maintained  (FDD only)          </TD> <TD>L1 continues                       </TD> <TD>L1 continues                       </TD> <TD>L1 continues                  </TD></TR>
 * <TR><TD>Intra-freq HO - CPI or PF changed (TDD128 only)        </TD> <TD>L1 continues                       </TD> <TD>L1 continues                       </TD> <TD>L1 stops implicitly           </TD></TR>
 * <TR><TD>Intra-freq HHO failure - timing initialised (FDD only)  </TD> <TD>L1 restores                        </TD> <TD>L1 restores                        </TD> <TD>L1 restores                   </TD></TR>
 * <TR><TD>Intra-freq HHO failure - timing maintained  (FDD only)  </TD> <TD>L1 continues                       </TD> <TD>L1 continues                       </TD> <TD>L1 continues                  </TD></TR>
 * <TR><TD>Intra-freq HO failure - CPI or PF changed (TDD128 only) </TD> <TD>L1 continues                       </TD> <TD>L1 continues                       </TD> <TD>L1 restores                   </TD></TR>
 * <TR><TD>Leaving CELL_DCH                                        </TD> <TD>L1 stops implicitly                </TD> <TD>L1 stops implicitly                </TD> <TD>L1 stops implicitly            </TD></TR>
 * <TR><TD>Entering CELL_DCH                                       </TD> <TD>L1 stops implicitly                </TD> <TD>L1 stops implicitly                </TD> <TD>L1 stops implicitly            </TD></TR>
 * <TR><TD>Intra-freq reselection                                  </TD> <TD>L1 stops implicitly                </TD> <TD>L1 stops implicitly                </TD> <TD>L1 stops implicitly            </TD></TR>
 * <TR><TD>Inter-freq reselection                                  </TD> <TD>L1 stops implicitly                </TD> <TD>L1 stops implicitly                </TD> <TD>L1 stops implicitly            </TD></TR>
 * <TR><TD>3G - 2G HHO                                             </TD> <TD>L1 stops implicitly                </TD> <TD>L1 stops implicitly                </TD> <TD>L1 stops implicitly            </TD></TR>
 * <TR><TD>3G - 2G reselection                                     </TD> <TD>L1 stops implicitly                </TD> <TD>L1 stops implicitly                </TD> <TD>L1 stops implicitly            </TD></TR>
 * <TR><TD>3G - 2G CCO                                             </TD> <TD>L1 stops implicitly                </TD> <TD>L1 stops implicitly                </TD> <TD>L1 stops implicitly            </TD></TR>
 * <TR><TD>3G - 2G fail                                            </TD> <TD>L1 restores                        </TD> <TD>L1 restores                        </TD> <TD>L1 restores                    </TD></TR>
 * <TR><TD>3G - 2G fail of fail                                    </TD> <TD>L1 stops implicitly                </TD> <TD>L1 stops implicitly                </TD> <TD>L1 stops implicitly            </TD></TR>
 * <TR><TD>Transition to NULL                                      </TD> <TD>Delete database                    </TD> <TD>Delete database                    </TD> <TD>Delete database                </TD></TR>
 * <TR><TD>Intra-cell freq switch  (TDD128 only)                   </TD> <TD>L1 stops implicitly                </TD> <TD>L1 stops implicitly                </TD> <TD>L1 continues                   </TD></TR>
 * <TR><TD>Intra-cell freq switch Failure (TDD128 only)            </TD> <TD>L1 restores                        </TD> <TD>L1 restores                        </TD> <TD>L1 continues                   </TD></TR>
 * </TABLE>
 *
 *
 * NOTE:above Abbreviations CPI is Cell parameters Id,PF is primary frequency.
 * Intra-cell freq switch is a working freq change in the same cell, i.e.
 * a working freq is changed between Primary freq and Secondary freq of the same cell,
 * or from one Secondary freq to another Secondary freq of the same cell.
 *
 *
 * Additionally it should be noted that cell-selection or reselection stops nCell
 * BCH and cancels any previous #CphyCellSearchReq.
 *
 * <B> Resource priorities in paging state </B>
 *
 *     -# Serving cell PCH and measurements
 *     -# Neighbour cell BCH
 *     -# Serving cell BCH
 *     -# Serving cell CTCH
 *     -# Neighbour cell measurements as defined in <tt> [3G TS 25.133] </tt>
 *
 * \note The protocol stack will not request simultaneous serving and
 * neighbour cell BCH decodes.
 *
 * @{
 */

/***************************************************************************
*   Manifest Constants
***************************************************************************/

/** Defines the maximum number of frequencies per RSSI scan supported by
 * the physical layer. It is used when a list of frequencies is given in
 * #CphyRssiScanReq primitive during initial cell selection or PLMN search.
 */
#if defined(UPGRADE_3G_FDD)
#define CPHY_MAX_FREQUENCIES_PER_RSSI_SCAN                      128
#else
#define CPHY_MAX_FREQUENCIES_PER_RSSI_SCAN                      20
#endif

/** Defines the maximum number of frequencies per RSSI scan supported by
 * the physical layer in FDD mode, see #CPHY_MAX_FREQUENCIES_PER_RSSI_SCAN*/
#define CPHY_MAX_FDD_FREQUENCIES_PER_RSSI_SCAN        CPHY_MAX_FREQUENCIES_PER_RSSI_SCAN

#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/**Defines the maximum number of frequencies per RSSI scan supported by
 * the physical layer in TDD mode, see #CPHY_MAX_FREQUENCIES_PER_RSSI_SCAN*/
#define CPHY_MAX_TDD128_FREQUENCIES_PER_RSSI_SCAN     CPHY_MAX_FREQUENCIES_PER_RSSI_SCAN
#if defined (TDD_UPCC_UL_DTX_B)
/**Default UL specialBurstScheduling is set to 1,mean the DTX period is 40ms*/
#define CPHY_DPCH_DEFAULT_SBGP  1
#endif
#endif

/** Defines the maximum number of cells for BSIC identification supported by
 * the physical layer.
 */
#define CPHY_MAX_CELLS_FOR_BSIC_IDENTIFICATION                      8

/** Defines the maximum number of CTFC entries given/configurable in a REQ
 * primitive.
 */
#define CPHY_MAX_CTFC_ENTRIES_PER_PRIMITIVE                         16

/** Defines the maximum number of frequency bands supported by UE.
 */
#define CPHY_MAX_FREQ_BANDS                                         2

/** Defines the maximum number of reports supported on this platform. */
#if defined (UPGRADE_MEASUREMENT_CAPABILITY_FROM_8CELL_TO_16CELL)
#define CPHY_MAX_REPORTS                                            16
#else
#define CPHY_MAX_REPORTS                                            8
#endif /* UPGRADE_MEASUREMENT_CAPABILITY_FROM_8CELL_TO_16CELL */

/** Defines the maximum number of RACH reports supported on this platform. */
#define CPHY_MAX_REPORT_ON_RACH                                     6

#ifdef ENABLE_INTER_FREQ_MEAS_ON_RACH
#if defined (UPGRADE_REL6)
/** Defines the maximum number of inter-freq measurement on RACH reports
 *  supported on this platform.
 */
#define CPHY_INTER_FREQ_MAX_REPORT_ON_RACH                          8
#endif
#endif /* ENABLE_INTER_FREQ_MEAS_ON_RACH */

/** Defines the maximum number of active Transmission Gap Patter Sequences
 * supported by the physical layer.
 */
#define CPHY_MAX_ACTIVE_TGPS                                        4

/** Defines the physical block size of GSM L1. */
#define GSM_L1_PHYSICAL_BLOCK_SIZE                                 23

/** Defines the maximum number of ARFCNs, which can be searched in GSM. */
#define GSM_ARFCN_LIST_SIZE                                       128

/** Defines the maximum number of search reports reported by the physical
 * layer.
 */
#define GSM_MAX_NUM_SEARCH_REPORTS                                 40

/** Defines a TFC that can no longer be selected by MAC though the TFCS and
 * TFCIs remain unchanged even if UTRAN removed transport channels and
 * associated TFCs.
 */
#define TFCI_NOT_USED                                             0xFFFFFFFF

/** Defines the scaling factors to be used for all dB and dBm measurements. */
#define MEASUREMENTS_TO_DB_SCALING_FACTOR          ((SignedInt8) 8)

/** Defines the BLER quality target. */
#define NO_BLER_QUALITY_TARGET                                    -128

 /* Note. The CSR unit tests use a limited range of 10550 to 10555 to test
 * band searching in a reasonable amount of time.
  */

#define CPHY_RSSI_SCAN_THRESHOLD                                (-100 * MEASUREMENTS_TO_DB_SCALING_FACTOR)
#define CPHY_RSSI_SCAN_THRESHOLD_FOR_SENSITIVITY_TEST           (-120 * MEASUREMENTS_TO_DB_SCALING_FACTOR)

#if defined (UPGRADE_3G_TDD128)
/** Define max num of find cell frequencies, Max num of rssi result is 33
 *  when A band search, 99 for F band search. */
#define CPHY_MAX_FREQUENCIES_PER_FIND_CELL                      193

/** Define following marcos for CphyRssiScanReq IE 'rssiScanThreshold' and
 *  'topNumOfRankedFreqs' during normal and sensitivity test.*/
#define CPHY_TOP_NUM_OF_RANKED_FREQS_FOR_A_BAND                         33
#define CPHY_TOP_NUM_OF_RANKED_FREQS_FOR_A_BAND_IN_SENSITIVITY_TEST     68

#define CPHY_TOP_NUM_OF_RANKED_FREQS_FOR_F_BAND                         99
#define CPHY_TOP_NUM_OF_RANKED_FREQS_FOR_F_BAND_IN_SENSITIVITY_TEST     193
#endif

#if !defined (UPGRADE_FDD_MULTIBAND)
/** Defines the minimum value of UARFCN for the 2.1 GHz band as defined in
 * <tt> [3G TS 25.101 section 5.4.4] </tt>.
 */
#define URRC_CSRR_MIN_UARFCN_IN_2_1_BAND                         10562

/** Defines the maximum value of UARFCN for the 2.1 GHz band as defined in
 * <tt> [3G TS 25.101 section 5.4.4] </tt>.
 */
#define URRC_CSRR_MAX_UARFCN_IN_2_1_BAND  10838

#define URRC_CSRR_MIN_UARFCN_IN_TDD128_BAND_A_UPPER         10054
#define URRC_CSRR_MAX_UARFCN_IN_TDD128_BAND_A_UPPER         10121

#define URRC_CSRR_MAX_FREQ_IN_BAND_A_UPPER      (URRC_CSRR_MAX_UARFCN_IN_TDD128_BAND_A_UPPER \
                                                -URRC_CSRR_MIN_UARFCN_IN_TDD128_BAND_A_UPPER+1)

#define URRC_CSRR_MIN_UARFCN_IN_TDD128_BAND_F              9404
#define URRC_CSRR_MAX_UARFCN_IN_TDD128_BAND_F              9596

#define URRC_CSRR_MAX_FREQ_IN_TDD128_BAND_F    (URRC_CSRR_MAX_UARFCN_IN_TDD128_BAND_F \
                                                -URRC_CSRR_MIN_UARFCN_IN_TDD128_BAND_F+1)

#define URRC_CSRR_MIN_UARFCN_IN_TDD128_BAND_E               11500
#define URRC_CSRR_MAX_UARFCN_IN_TDD128_BAND_E               11992

#define URRC_CSRR_MAX_FREQ_IN_TDD128_BAND_E    (URRC_CSRR_MAX_UARFCN_IN_TDD128_BAND_E \
                                                -URRC_CSRR_MIN_UARFCN_IN_TDD128_BAND_E+1)

/** Defines the maximum number of frequencies supported in the BandA Upper and  1.9 GHz band.
*/
#define URRC_CSRR_MAX_FREQS URRC_CSRR_MAX_FREQ_IN_TDD128_BAND_F

#else /* UPGRADE_FDD_MULTIBAND */
#define URRC_CSRR_MIN_UARFCN_IN_BAND_1                            10562

#define URRC_CSRR_MAX_UARFCN_IN_BAND_1                            10838

#define URRC_CSRR_MAX_FREQS_IN_BAND_1  (URRC_CSRR_MAX_UARFCN_IN_BAND_1 - \
                                        URRC_CSRR_MIN_UARFCN_IN_BAND_1 + \
                                        1)


#define URRC_CSRR_MIN_UARFCN_IN_BAND_2                             9662

#define URRC_CSRR_MAX_UARFCN_IN_BAND_2                             9938

#define URRC_CSRR_MAX_FREQS_IN_BAND_2_GENERAL  (URRC_CSRR_MAX_UARFCN_IN_BAND_2 - \
                                                URRC_CSRR_MIN_UARFCN_IN_BAND_2 + \
                                                1)

#define URRC_CSRR_MAX_FREQS_IN_BAND_2_ADDITIONAL                     12

#define URRC_CSRR_MAX_FREQS_IN_BAND_2 (URRC_CSRR_MAX_FREQS_IN_BAND_2_GENERAL + \
                                       URRC_CSRR_MAX_FREQS_IN_BAND_2_ADDITIONAL)


#define URRC_CSRR_MIN_UARFCN_IN_BAND_5                             4357

#define URRC_CSRR_MAX_UARFCN_IN_BAND_5                             4458

#define URRC_CSRR_MAX_FREQS_IN_BAND_5_GENERAL  (URRC_CSRR_MAX_UARFCN_IN_BAND_5 - \
                                                URRC_CSRR_MIN_UARFCN_IN_BAND_5 + \
                                                1)

#define URRC_CSRR_MAX_FREQS_IN_BAND_5_ADDITIONAL                      6

#define URRC_CSRR_MAX_FREQS_IN_BAND_5 (URRC_CSRR_MAX_FREQS_IN_BAND_5_GENERAL + \
                                       URRC_CSRR_MAX_FREQS_IN_BAND_5_ADDITIONAL)


#define URRC_CSRR_MAX_FREQS                 URRC_CSRR_MAX_FREQS_IN_BAND_2

#endif /* UPGRADE_FDD_MULTIBAND */


/** Defines the GCF test frequency for the middle of the 2100 band. */
#define URRC_CSRR_MID_UARFCN_GCF_TEST_IN_2_1_BAND  (UUARFCN) 10700


/** In IDLE state \c CPHY_CCTRCH_SCCPH_0 carries either only PCH or PCH and CTCH;
 * in CELL_FACH state, it carries FACH.
 */
#define CPHY_CCTRCH_SCCPH_0                   0

/** \c CPHY_CCTRCH_SCCPH_1  carries CTCH in IDLE state. */
#define CPHY_CCTRCH_SCCPH_1                   1

/** \c CPHY_CCTRCH_SCCPCH_2 is for DRAC operation which is for a future release.
 */
#define CPHY_CCTRCH_SCCPH_2                   2

#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
#if defined (UPGRADE_3G_MBMS)
/** \c CCTRCH_SCCPCH_FOR_MBMS_0 carries MCCH in all state. */
#define CCTRCH_SCCPCH_FOR_MBMS_0              3

/** \c CCTRCH_SCCPCH_FOR_MBMS_1 carries MTCH 1 in all state. */
#define CCTRCH_SCCPH_FOR_MBMS_1               4

/** \c CCTRCH_SCCPCH_FOR_MBMS_2 carries MTCH 2 in all state. */
#define CCTRCH_SCCPH_FOR_MBMS_2               5
#endif /* UPGRADE_3G_MBMS */
#endif /* UPGRADE_3G_TDD128  || UPGRADE_3G_DOUBLE_DUAL_MODE */

/** \c CPHY_CCTRCH_DPCH carries DCH transport channel in CELL_DCH state. */
#define CPHY_CCTRCH_DPCH                      0

/** Bit position 1 in bitmap used in #CphyUeCapabilityConfigCnf to indicate if
 * <tt>UL Frequency: 1920 - 1980 MHz</tt> and <tt>DL Frequency: 2110 - 2170 MHz
 * </tt> is supported in L1 for FDD.
 */
#define UMTS_BAND_1                     0x0001

/** Bit position 2 in bitmap used in #CphyUeCapabilityConfigCnf to indicate if
 * <tt>UL Frequency: 1850 - 1910 MHz</tt> and <tt>DL Frequency: 1930 - 1990 MHz
 * </tt> is supported in L1 for FDD.
 */
#define UMTS_BAND_2                     0x0002

/** Bit position 3 in bitmap used in #CphyUeCapabilityConfigCnf to indicate if
 * <tt>UL Frequency: 1710 - 1785 MHz</tt> and <tt>DL Frequency: 1805 - 1880 MHz
 * </tt> is supported in L1 for FDD.
 */
#define UMTS_BAND_3                     0x0004

/** Bit position 4 in bitmap used in #CphyUeCapabilityConfigCnf to indicate if
 * <tt>UL Frequency: 1710 - 1755 MHz</tt> and <tt>DL Frequency: 2110 - 2155 MHz
 * </tt> is supported in L1 for FDD.
 */
#define UMTS_BAND_4                     0x0008

/** Bit position 5 in bitmap used in #CphyUeCapabilityConfigCnf to indicate if
 * <tt>UL Frequency: 824 - 849 MHz</tt> and <tt>DL Frequency: 869 - 894 MHz
 * </tt> is supported in L1 for FDD.
 */
#define UMTS_BAND_5                     0x0010

/** Bit position 6 in bitmap used in #CphyUeCapabilityConfigCnf to indicate if
 * <tt>UL Frequency: 830 - 840 MHz</tt> and <tt>DL Frequency: 875 - 885 MHz
 * </tt> is supported in L1 for FDD.
 */
#define UMTS_BAND_6                     0x0020

/** Bit position 7 in bitmap used in #CphyUeCapabilityConfigCnf to indicate if
 * <tt>UL Frequency: 2500 - 2570 MHz</tt> and <tt>DL Frequency: 2620 - 2690 MHz
 * </tt> is supported in L1 for FDD.
 */
#define UMTS_BAND_7                     0x0040

/** Bit position 8 in bitmap used in #CphyUeCapabilityConfigCnf to indicate if
 * <tt>UL Frequency: 880 - 915 MHz</tt> and <tt>DL Frequency: 925 - 960 MHz
 * </tt> is supported in L1 for FDD.
 */
#define UMTS_BAND_8                     0x0080

/** Bit position 9 in bitmap used in #CphyUeCapabilityConfigCnf to indicate if
 * <tt>UL Frequency: 1749.9 - 1784.9 MHz</tt> and <tt>DL Frequency: 1844.9 -
 * 1879.9 MHz </tt> is supported in L1 for FDD.
 */
#define UMTS_BAND_9                     0x0100


#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/** Bit position 1 in bitmap used in #CphyUeCapabilityConfigCnf to indicate if
 * <tt>UL=DL Frequency: 1900 - 1920 MHz</tt> is supported in L1 for TDD128.
 */
#define UMTS_BAND_A_LOWER               0x0001

/** Bit position 2 in bitmap used in #CphyUeCapabilityConfigCnf to indicate if
 * <tt>UL=DL Frequency: 2010 - 2025 MHz</tt> is supported in L1 for TDD128.
 */
#define UMTS_BAND_A_UPPER               0x0002

/** Bit position 3 in bitmap used in #CphyUeCapabilityConfigCnf to indicate if
 * <tt>UL=DL Frequency: 1850 - 1910 MHz</tt> is supported in L1 for TDD128.
 */
#define UMTS_BAND_B_LOWER               0x0004

/** Bit position 4 in bitmap used in #CphyUeCapabilityConfigCnf to indicate if
 * <tt>UL=DL Frequency: 1930 - 1990 MHz</tt> is supported in L1 for TDD128.
 */
#define UMTS_BAND_B_UPPER               0x0008

/** Bit position 5 in bitmap used in #CphyUeCapabilityConfigCnf to indicate if
 * <tt>UL=DL Frequency: 1910 - 1930 MHz</tt> is supported in L1 for TDD128.
 */
#define UMTS_BAND_C                     0x0010

/** Bit position 6 in bitmap used in #CphyUeCapabilityConfigCnf to indicate if
 * <tt>UL=DL Frequency: 1880 - 1920 MHz</tt> is supported in L1 for TDD128.
 */
#define UMTS_BAND_F                     0x0020

/** Bit position 7 in bitmap used in #CphyUeCapabilityConfigCnf to indicate if
 * <tt>UL=DL Frequency: 2300 - 2400 MHz</tt> is supported in L1 for TDD128.
 */
#define UMTS_BAND_E                     0x0040

#endif /* UPGRADE_3G_TDD128  || UPGRADE_3G_DOUBLE_DUAL_MODE */

/** Defines the maximum number of MBMS services that L1 monitors MICH at the
 * same time.
 */
#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
#if defined (UPGRADE_3G_MBMS)
#define CPHY_MAX_NI_NUM_FOR_MBMS                 16

/** Defines the maximum number of TrChs in the S-CCPCH which carries MTCHs.
 */
#define CPHY_MAX_TRCH_NUM_FOR_MTCH_EACH_SCCPCH   2
#endif /* UPGRADE_3G_MBMS */
#endif /* UPGRADE_3G_TDD128  || UPGRADE_3G_DOUBLE_DUAL_MODE */


/* Defines the maximum number of scheduling info carried by CphyBchReq and CphyNcellBchReq */
#define CPHY_SCHEDULING_NUM           10

/* This definition is the same as URRC_SIR_MAX_UMTS_SIB_OFFSETS in Urrsityp.h
  * Since Urrsityp.h had already included this header file,
  * therefore a direct definition is given to avoid cross reference. */
#define CPHY_MAX_SIB_OFFSETS          15

#if defined (UPGRADE_LTE)
/** \def CPHY_MAX_EUTRA_INTER_FREQ_CARRIERS
 * Defines the maximum number of EUTRA inter-frequency carriers. */
#define ECPHY_MAX_EUTRA_INTER_FREQ_CARRIERS                 4
#endif /* UPGARDE_LTE */

/***************************************************************************
*   Macro Functions
***************************************************************************/

/***************************************************************************
*   Types
***************************************************************************/

/** Defines the types of transmission time interval*/
typedef enum TransmissionTimeIntervalTag
{
    /**not significant TTI*/
    TransmissionTimeInterval_not_significant = 0,
#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
    /**5ms TTI*/
    TransmissionTimeInterval_tti5 =  5,
#endif
    /**10ms TTI*/
    TransmissionTimeInterval_tti10 = UTransmissionTimeInterval_tti10,
    /**20ms TTI*/
    TransmissionTimeInterval_tti20 = UTransmissionTimeInterval_tti20,
    /**40ms TTI*/
    TransmissionTimeInterval_tti40 = UTransmissionTimeInterval_tti40,
    /**80ms TTI*/
    TransmissionTimeInterval_tti80 = UTransmissionTimeInterval_tti80,
    /**dynamic TTI, please refer to <tt> [3G TS 25.331 10.3.5.23] </tt>*/
    TransmissionTimeInterval_dynamic = 255
}
TransmissionTimeInterval;

/** Bitmap for cell identities.
 * For example: 0x00000002 indicates Cell ID 1.
 */
typedef Int32 CellIdMask;


/** Defines the type of TFCS configuration. */
typedef enum ExplicitTfcsModeTag
{
    /** Complete TFCS. */
    UTFC_COMPLETE                       = 0,

    /** Addition of TFC. */
    UTFC_ADDITION                       = 1,

    /** Removal of TFC. */
    UTFC_REMOVAL                        = 2
}
ExplicitTfcsMode;

/** Indicates the error codes returned from the physical layer in CNF
 * primitives.
 */
typedef enum UPhysicalLayerErrorCodeTag
{
    /** No error */
    PHY_ERR_NO_ERROR                    = 0,
    /** Invalid configuration */
    PHY_ERR_INVALID_CONFIGURATION       = 1,
    /** Option not supported */
    PHY_ERR_OPTION_NOT_SUPPORTED        = 2,
    /** Invalid state */
    PHY_ERR_INVALID_STATE               = 3,
    /** Undefined */
    PHY_ERR_UNDEFINED                   = 4
}
UPhysicalLayerErrorCode;

/** Indicates the reason for the #CphyRlCommonSetupReq primitive. */
typedef enum URadioLinkActionTag
{
    /** Establish radio link */
    CPHY_RADIO_LINK_ESTABLISH                    = 0,
    /** Timing maintained intra-frequency hard handover */
    CPHY_RADIO_LINK_INTRA_FREQ_HHO_MAINTAIN      = 1,
    /** Timing initialised intra-frequency hard handover */
    CPHY_RADIO_LINK_INTRA_FREQ_HHO_INITIALISE    = 2,
    /** Timing maintained inter-frequency hard handover */
    CPHY_RADIO_LINK_INTER_FREQ_HHO_MAINTAIN      = 3,
    /** Timing initialised inter-frequency hard handover */
    CPHY_RADIO_LINK_INTER_FREQ_HHO_INITIALISE    = 4,
    /** Inter system handover from GSM */
    CPHY_RADIO_LINK_HO_FROM_GSM                  = 5
#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
    /** Reconfiguration of radio link without SYNC A procedure for FDD only */
    ,CPHY_RADIO_LINK_RECONFIG_WITHOUT_SYNC_A      = 6,
    /** Active set update for FDD only */
    CPHY_RADIO_LINK_ACTIVE_SET_UPDATE            = 7
#endif
#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
    /** Reconfiguration of CCTCRH_DCH for TDD128 only.*/
    ,CPHY_RADIO_LINK_RECONFIG                     = 8,
    /** Baton Handover for TDD128 only.*/
    CPHY_RADIO_LINK_BATON_HO                     = 9

    ,CPHY_RADIO_LINK_BATON_HO_TX_TO_RX            = 10

    /* Add the new action for Uplink Physcial Channel Control message, in TDD only */
    ,CPHY_RADIO_LINK_RECONFIG_UPCC                = 11
#endif
}
URadioLinkAction;

/** Defines the reason for giving OutOfSync. */
typedef enum OutOfSyncCauseTag
{
    /** OutOfSync because of failure to establish physical channels. */
    PHYS_CHAN_ESTABLISH_FAILURE         = 0,

    /** OutOfSync because of Radio Link failure. */
    RADIO_LINK_FAILURE                  = 1
}
OutOfSyncCause;

/** Indicates the deactivation cause. */
typedef enum UDeactivationCauseTag
{
       /** Normal deactivation. */
       UDEACT_NORMAL = 0,

       /** Deactivation because of power-off. */
       UDEACT_POWER_OFF = 1
}
UDeactivationCause;

#if defined (UPGRADE_3G_MBMS)
/** Indicates the type of MTCH scheduling */
typedef enum MtchSchedulingTypeTag
{
    /** The MTCH_SCHEDULING_TYPE_DIRECT indicates L1 receives MTCH all the
     * time.
     */
    MTCH_SCHEDULING_TYPE_DIRECT = 0,

    /** The MTCH_SCHEDULING_TYPE_TDM indicates L1 receives MTCH according to
     * TDM scheduling information.
     */
    MTCH_SCHEDULING_TYPE_TDM = 1,

    /** The MTCH_SCHEDULING_TYPE_MSCH indicates L1 receives MTCH according to
     * the scheduling information received in MSCH.
     */
    MTCH_SCHEDULING_TYPE_MSCH = 2
}
MtchSchedulingType;
#endif /* UPGRADE_3G_MBMS */

/** Defines a list of GSM ARFCN's. */
typedef struct GsmArfcnListTag
{
    /** A list of ARFCN's.
     * This array is a bitmap. Bit x represent ARFCN x. Bit 0 in the bitmap
     * is bit 0 in \c list[0]. \c GSM_ARFCN_LIST_SIZE is set to 128.
     */
    Int8                         list [GSM_ARFCN_LIST_SIZE];
}
GsmArfcnList;

/** Defines the dynamic part of transport format.
 * The dynamic part of transport format can change every TTI, and it
 * mainly consists of \c numberOfTransportBlocks number of \c
 * transportBlockSize.
 */
typedef struct DynamicTfInfoTag
{
    /** Specifies the transport block size in bits. */
    Int16                               transportBlockSize;

    /** Specifies the number of transport blocks in a TTI. */
    Int16                               numberOfTransportBlocks;
#ifdef TMP_ENABLE_TDD128_TTI_DYNAMIC
    /** Specifies the transmission time interval*/
    TransmissionTimeInterval           tti;
#endif /* UPGRADE_TTI_DYNAMIC */
}
DynamicTfInfo;


/** Defines the transport format set. */
typedef struct DynamicTfDataTag
{
    /** Specifies the number of transport formats.
\assoc ARRAY \ref dynamicTfInfo */
    Int8                                numberOfTransportFormats;

    /** Specifies an array of transport formats. The number of transport
     * formats are given by \c numberofTransportFormats.
     */
    DynamicTfInfo                       dynamicTfInfo[maxTF];
}
DynamicTfData;


/** Defines the identity of a GSM cell. */
typedef struct UGsmCellTag
{
    /** Specifies the value of this particular GSM cell's ARFCN. */
    UBCCH_ARFCN                         bcch_arfcn;

    /** Indicates whether this cell is in the PCS or DCS band. */
    UFrequency_Band                     gsmBandIndicator;

    /** Specifies the Base transceiver Station Identity Code, which is divided
     * into the Network Colour Code (NCC) and the Base Station Colour Code
     * (BCC).
     */
    UBSIC                               bsic;
}
UGsmCell;


/** Defines the semi static part of the transport format. */
typedef struct SemiStaticTfDataTag
{
    /** Specifies the transmission timer interval for the transport channel.
     * TTI can be 5, 10, 20, 40 or 80ms.
     */
    TransmissionTimeInterval           tti;

    /** Specifies the channel coding of the transport channel. */
    UChannelCodingType                  channelCodingType;

    /** Specifies the rate-matching factor for the transport channel. Range is
     * 1 to 256.
     */
    URateMatchingAttribute              rateMatchingAttribute;

    /** Specifies the number of CRC bits for the transport channel. The values
     * can be 0, 8, 12, 16 or 24 bits.
     */
    UCRC_Size                           crc_Size;
}
SemiStaticTfData;


/** Allows adding Transport Format Combinations to the TFC database in the
 * physical layer.
 *
 * \note Any gaps in the current TFC database must be filled before adding
 * the remaining new TFC's to the end of the database.
 */
typedef struct TfcsAddTag
{
    /** Specifies the number of TFC's (1 to MAX_TFC_CONFIGS) to be added to the
     * database.

\assoc ARRAY \ref ctfc */
    Int8                        numberOfElements;

    /** Specifies an array of TFC's to be added, which are given as Calculated
     * Transport Format Combination. CTFC is defined in <tt>[3G TS 25.331
     * section 14.10]</tt>.
     */
    Int32                       ctfc [CPHY_MAX_CTFC_ENTRIES_PER_PRIMITIVE];
}
TfcsAdd;


/** Allows removing TFC's from the database.
 *
 * \note The removal process simply requires entries in the table to be
 * marked free so that the gaps can be reused by the next 'addition'.
 */
typedef struct TfcsRemovalListTag
{
    /** This defines the number of TFC's (1 to MAX_TFC_CONFIGS) to be removed
     * from the database.
     */
    Int8                        numberOfElements;

   /** List of indexes in the TFC table to be removed. */
    Int16                       tfcIndex [CPHY_MAX_CTFC_ENTRIES_PER_PRIMITIVE];
}
TfcsRemovalList;


/** Defines the type of TFCS configuration (complete/addition/removal).
 */
typedef union ExplicitTfcsUnionTag
{
    /** Specifies the complete TFCS given by RRC. */
    TfcsAdd                             complete;

    /** Specifies the addition of transport format combinations (TFC's). */
    TfcsAdd                             addition;

    /** Specifies the removal of transport format combinations (TFC's). */
    TfcsRemovalList                     removal;
}
ExplicitTfcsUnion;


/** Allows configuring the Transport Format Combination (TFC) database in the
 * physical layer when explicit TFCI signalling is used.
 *
 * \note To limit the size of the CPHY primitives the maximum number of
 * TFC's that can be configured (added or removed) in one go is limited to
 * \c MAX_TFC_CONFIGS (default value of 16). Therefore, for a complete
 * configuration of 31 entries the RRC would need to do a 'complete'
 * configuration for the first 16 entries followed by an 'addition' of the
 * remaining 15 entries. 'Addition' is only valid when configuring TFC using
 * more than one primitive in this way. 'Removal' is not valid.
 */
typedef struct ExplicitTfcsConfigTag
{
    /** Identifies whether this is a new TFC configuration, removal
     * or an addition.
\assoc UNION \ref explicitTfcsUnion */
    ExplicitTfcsMode                    explicitTfcsMode;

    /** Defines a Complete/Addition/Removal configuration of TFCS. */
    ExplicitTfcsUnion                   explicitTfcsUnion;
}
ExplicitTfcsConfig;



#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/** Defines a (FDD) cell. */
typedef struct UmtsCellFDDTag
{
    /** Indicates if transmit diversity is used in the cell or not. */
    Boolean                             txDiversityIndicator;

    /** Specifies the primary scrambling code of the cell. Primary scrambling
     * code is used by physical layer to synchronise to the cell.
     */
    UPrimaryScramblingCode              primaryScramblingCode;

    /** Specifies the SFN to be decoded from the target cell. */
    Boolean                             readSFN;

    /** Indicates whether the \c referenceTimeDifferenceToCell is semantically
     * meaningful (TRUE) or not (FALSE).

\assoc PRESENT \ref referenceTimeDifferenceToCell
\assoc PRESENT \ref referenceTimeDifferenceToCellType */
    Boolean                             referenceTimeDifferenceToCellPresent;

    /** Indicates an ASN.1 enumeration constant which defines the time
     * difference to cell accuracy in terms of chips.
     * Please refer to <tt> [3G TS 25.331 10.3.7.60] </tt>.
     */
    T_UReferenceTimeDifferenceToCell    referenceTimeDifferenceToCellType;

    /** Indicates the offset in chips from the current cell to the neighbour cell.
     * The presence of \c referenceTimeDifferenceToCell is optional over the air
     * interface but it is only absent for the serving cell.
     */
    Int16                               referenceTimeDifferenceToCell;
}
UmtsCellFDD;
#endif /* UPGRADE_3G_FDD  || UPGRADE_3G_DOUBLE_DUAL_MODE */

#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/** Defines a TDD128 cell. */
typedef struct UmtsCellTDD128Tag
{
    /** Specifies the cell parameters Id of the cell. */
    UCellParametersID                   cellParametersID;

    /** Specifies the SFN to be decoded from the target cell. */
    Boolean                             readSFN;

    /** Indicates whether the \c referenceTimeDifferenceToCell is semantically
     * meaningful (TRUE) or not (FALSE).

\assoc PRESENT \ref referenceTimeDifferenceToCell
\assoc PRESENT \ref referenceTimeDifferenceToCellType */
    Boolean                             referenceTimeDifferenceToCellPresent;

    /** Indicates an ASN.1 enumeration constant which defines the time
     * difference to cell accuracy in terms of chips.
     * Please refer to <tt> [3G TS 25.331 10.3.7.60] </tt>.
     */
    T_UReferenceTimeDifferenceToCell    referenceTimeDifferenceToCellType;

    /** Indicates the offset in chips from the current cell to the neighbour cell.
     * The presence of \c referenceTimeDifferenceToCell is optional over the air
     * interface but it is only absent for the serving cell.
     */
    Int16                               referenceTimeDifferenceToCell;

    /** Indicates the TS transmit diversity is used in the cell or not. */
    Boolean                             tstdIndicator;

    /** Indicates the SC transmit diversity is used in the cell or not. */
    Boolean                             sctdIndicator;

    /** Indicates the timeslot mapping: LSB correspond to TS0. */
    Int8                                timeslotInfo;

    /** Indicates the presence of primaryCCPCH_TX_Power.
\assoc PRESENT \ref primaryCCPCH_TX_Power */
    Boolean                             primaryCCPCH_TX_PowerPresent;

    /** indicates the Primary CCPCH Tx power of the cell.*/
    UPrimaryCCPCH_TX_Power              primaryCCPCH_TX_Power;

#if defined (UPGRADE_3G_HSUPA)
    /** Indicates that whether the inter frequency neighbour cell has a secondary frequency
     * which is the same as the current working frequency of UE*/
    Boolean                             intraSecondaryFreqIndicator;
#endif

}
UmtsCellTDD128;
#endif /* UPGRADE_3G_TDD128  || UPGRADE_3G_DOUBLE_DUAL_MODE */

/** Defines a generic 3G cell. */
typedef union UmtsCellUnionTag
{
#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
    /** Defines a FDD cell.*/
    UmtsCellFDD                         fdd;
#endif
#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
    /** Defines a TDD128 cell.*/
    UmtsCellTDD128                      tdd128;
#endif
}
UmtsCellUnion;

/** Defines the required 3GPP FDD intra- and inter-frequency measurements.
 * All the parameters in this structure are bit masks with bit 0 set to 1
 * indicate a measurement required on Cell ID 0 and bit 31 set to 1 indicate
 * a measurement required on Cell ID 31.
 * \note If a measurement quantity is set to 0xffffffff then this implies
 * all cells in the intra- and inter-frequency monitor list.
 */
typedef struct UCellMeasurementTypeTag
{
    /** Indicates a bit mask to define the SFN-to-SFN offset measurement
     * required for the corresponding cells.
     */
    Int32                               sfnToSfnOffset;

    /** Indicates a bit mask to define the CFN-to-SFN offset measurement
     * required for the corresponding cells.
     */
    Int32                               cfnToSfnOffset;

    /** Indicates a bit mask to define the CPICH Ec/No measurement required
     * for the corresponding cells.
     */
    Int32                               cpichEcNo;

    /** Indicates a bit mask to define the CPICH RSCP measurement required
     * for the corresponding cells.
     */
    Int32                               cpichRscp;
}
UCellMeasurementType;

#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
typedef UCellMeasurementType CellMeasurementTypeFDD;
#endif

#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/** Defines the required 3GPP TDD128 intra- and inter-frequency measurements.
 * All the parameters in this structure are bit masks with bit 0 set to 1
 * indicate a measurement required on Cell ID 0 and bit 31 set to 1 indicate
 * a measurement required on Cell ID 31.
 * \note If a measurement quantity is set to 0xffffffff then this implies
 * all cells in the intra- and inter-frequency monitor list.
 */
typedef struct CellMeasurementTypeTDD128Tag
{
    /** Indicates a bit mask to define the SFN-to-SFN offset measurement
     * required for the corresponding cells.
     */
    Int32                               sfnToSfnOffset;

    /** Indicates a bit mask to define the CFN-to-SFN offset measurement
     * required for the corresponding cells.
     */
    Int32                               cfnToSfnOffset;

    /** Indicates a bit mask to define the P-CCPCH RSCP measurement required
     * for the corresponding cells.
     */
    Int32                               p_ccpchRscp;

    /** Indicates a bit mask to define the Timeslot ISCP measurement required
     * for the corresponding cells.
     */
    Int32                               timeslotIscp;
}
CellMeasurementTypeTDD128;
#endif

/** Defines generic measurements required for intra- and inter-frequency cells */
typedef union IntraFreqCellMeasurementTypeUnionTag
{
#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
    /** Indicates the type of measurements required for the intra- and inter-frequency cells.
     * \note If a measurement quantity is set to 0xffffffff then this implies
     * all cells in the Inter-frequency monitor list.
     */
    CellMeasurementTypeFDD             fdd;
#endif
#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
    /** Indicates the type of measurements required for the intra- and inter- frequency cells.
     * \note If a measurement quantity is set to 0xffffffff then this implies
     * all cells in the Inter-frequency monitor list.
     */
    CellMeasurementTypeTDD128          tdd128;
#endif
}
IntraFreqCellMeasurementTypeUnion;

#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
typedef struct InterFreqCellMeasurementTypeFDDTag
{
    /** Indicates the type of measurements required for the inter-frequency cells.
     * \note If a measurement quantity is set to 0xffffffff then this implies
     * all cells in the Inter-frequency monitor list.
     */
    CellMeasurementTypeFDD             interFrequencyMeasType;

    /** Indicates if wideband UTRAN RSSI measurement is requested or not */
    Boolean                             measureRssi;
}
InterFreqCellMeasurementTypeFDD;
#endif

#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/**defines the type of measurements required for the inter-frequency cells*/
typedef struct InterFreqCellMeasurementTypeTDD128Tag
{
    /** Indicates the type of measurements required for the inter-frequency cells.
     * \note If a measurement quantity is set to 0xffffffff then this implies
     * all cells in the Inter-frequency monitor list.
     * \note timeslotIscp measurement in #interFrequencyMeasType can not be requested for
     * Inter-freq cells.
     */
    CellMeasurementTypeTDD128             interFrequencyMeasType;

    /** Indicates if wideband UTRAN RSSI measurement is requested or not */
    Boolean                             measureRssi;
}
InterFreqCellMeasurementTypeTDD128;
#endif

/** Defines generic type of measurements required for the inter-frequency cells */
typedef union InterFreqCellMeasurementTypeUnionTag
{
#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
    /** Indicates the type of measurements required for the inter-frequency cells.
     * \note If a measurement quantity is set to 0xffffffff then this implies
     * all cells in the Inter-frequency monitor list.
     */
    InterFreqCellMeasurementTypeFDD             fdd;
#endif
#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
    /** Indicates the type of measurements required for the inter-frequency cells.
     * \note If a measurement quantity is set to 0xffffffff then this implies
     * all cells in the Inter-frequency monitor list.
     */
    InterFreqCellMeasurementTypeTDD128          tdd128;
#endif
}
InterFreqCellMeasurementTypeUnion;


/** Defines the measurement results for intra-frequency cell results on RACH.
 */
typedef struct URachMeasurementReportTag
{
    /** Specifies the identity of the cell. */
    UPrimaryScramblingCode              primaryScramblingCode;

    /** Indicates if \c sfnToSfnFrameOffset and \c sfnToSfnChipOffset is used
     * or not.
     */
    Boolean                             sfnToSfnOffsetPresent;

    /** Specifies the frame offset (0 to 255) between the current cell and the
     * monitored cell.
     */
    Int8                                sfnToSfnFrameOffset;

    /** Specifies the chip offset between the current cell and the monitored
     * cell. It is in the range 0 to 38399 chips.
     */
    Int16                               sfnToSfnChipOffset;

    /** Indicates if \c cpich_Ec_No is present or not. */
    Boolean                             cpich_Ec_N0Present;

    /** Specifies the Ec/No measurement for the CPICH in 1/8 dB steps, \ref
     * example
     */
    SignedInt16                         cpich_Ec_N0;

    /** Indicates if \c cpich_RSCP is present or not. */
    Boolean                             cpich_RSCP_Present;

    /** Specifies the RSCP measurement for the CPICH in 1/8 dBm steps, \ref
     * example
     */
    SignedInt16                         cpich_RSCP;
}
URachMeasurementReport;


#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)

/** Defines the measurement results for intra- and inter-frequency monitored
 * cells.
 */
typedef struct CellMeasurementReportFDDTag
{
    /** Specifies the primary scrambling code of the cell measured. */
    UPrimaryScramblingCode              primaryScramblingCode;

    /** Indicates if \c cpich_Ec_N0 is present or not.
\assoc PRESENT \ref cpich_Ec_N0 */
    Boolean                             cpich_Ec_N0Present;

    /** Specifies the Ec/No measurement for the CPICH in 1/8 dB steps, \ref
     * example
     */
    SignedInt16                         cpich_Ec_N0;

    /** Indicates if \c cpich_RSCP is present or not.
\assoc PRESENT \ref cpich_RSCP */
    Boolean                             cpich_RSCP_Present;

    /** Specifies the RSCP measurement for the CPICH in 1/8 dBm steps, \ref
     * example
     */
    SignedInt16                         cpich_RSCP;

    /** Indicates if \c sfnToSfnFrameOffset and \c sfnToSfnChipOffset is present
     * or not.

\assoc PRESENT \ref sfnToSfnFrameOffset
\assoc PRESENT \ref sfnToSfnChipOffset */
    Boolean                             sfnToSfnOffsetPresent;

    /** Specifies the frame offset (0 to 255) from the current cell to the
     * monitored cell.
     */
    Int8                                sfnToSfnFrameOffset;

    /** Specifies the chip offset from the current cell to the monitored cell.
     * It is in the range 0 to 38399 chips.
     */
    Int16                               sfnToSfnChipOffset;

    /** Indicates if \c cfn and \c sfn used or not. Frame offset is calculated
     * by RRC since frame offset requires 4 LSBs of HFN with a 8 bit CFN in
     * order to do a 12 bit comparison.

\assoc PRESENT \ref cfn
\assoc PRESENT \ref sfn */
    Boolean                             cfnToSfnFrameOffsetPresent;

    /** Specifies the CFN of the reference cell. */
    Int8                                cfn;

    /** Specifies the SFN of the target cell (0 to 4095). */
    Int16                               sfn;

    /** Indicates if \c cfnToSfnChipOffset is present or not.

\assoc PRESENT \ref cfnToSfnChipOffset */
    Boolean                             cfnToSfnChipOffsetPresent;

    /** Specifies the chip offset (in the range 0 to 38399 chips)
     * between the reference cell and the target cell.
     */
    Int16                               cfnToSfnChipOffset;
}
CellMeasurementReportFDD;
#endif /* UPGRADE_3G_FDD  || UPGRADE_3G_DOUBLE_DUAL_MODE */


/** Defines the measurement results for inter system (GSM in this case)
 * monitored cells.
 */
typedef struct UGsmReportTag
{
#ifdef ENABLE_CIL_VERSION_CONTROL
    /** Specifies the value of this particular GSM cell's ARFCN. */
    UBCCH_ARFCN                         bcch_arfcn;
#endif /* ENABLE_CIL_VERSION_CONTROL */

    /** Indicates if \c gsm_CarrierRSSI is present or not.
\assoc PRESENT \ref gsm_CarrierRSSI */
    Boolean                             gsmCarrierPresent;

    /** RSSI level of a GSM cell in 1/8 dBm steps (e.g.\ -480 = -60 dBm).
     * The range is -880 to -384 (-110 dBm to -48 dBm).
     * Reference: <tt>[GSM TS 05.08]</tt>
     */
    SignedInt16                         gsm_CarrierRSSI;
}
UGsmReport;


/** Defines a structure which is used for the wideband RSSI measurement
 * reports.
 */
typedef struct UtraRssiReportTag
{
    /** The frequency on which the measurement was made. */
    UUARFCN                             frequency;

    /** The wideband RSSI measurement result for this frequency and is in
     * 1/8 dBm steps, for example -400 = -50 dBm.
     */
    SignedInt16                         utra_CarrierRSSI;
}
UtraRssiReport;

/** Defines the frequency and cell information to describe an inter-frequency
 * cell.
 */
typedef struct InterFrequencyCellTag
{
    /** Specifies the frequency of the cell. */
    UUARFCN                             uarfcn_dl;

    /** UmtsMode: FDD or TDD128.
\assoc UNION \ref mode */
    UmtsMode                            umtsMode;

    /** Specifies the identification of the cell on the above frequency.
     */
    UmtsCellUnion                       mode;
}
InterFrequencyCell;

/** Defines the GSM measurements required for each cell.
 * \c gsmCarrier is a 32 bit masks in which if bit 0 is set to 1 indicates a
 * corresponding measurement is required on Cell ID 0 and bit 31 is set to 1
 * indicates a corresponding measurement is required for Cell ID 31.
 * \note If a measurement quantity is set to 0xffffffff then this implies
 * all cells in the inter-system monitor list.
 */
typedef struct UGsmMeasTypeTag
{
    /** A 32 bit mask to indicate a BSIC identification measurement for a list
     * of cells.
     */
    Int32                               gsmCarrier;
}
UGsmMeasType;


/** Defines the constants and timers used during CELL_DCH channel
 * synchronisation.
 * The diagram below illustrates the usage of these parameters.
 *
 * \image html prd3gphychsync.png "Fig: Physical Channel Synchronisation"
 * \image latex prd3gphychsync.eps "Physical Channel Synchronisation" width=12cm
 */
typedef struct SyncParametersTag
{
    /** Constant used in CELL_DCH channel establishment. */
    Int16                               n_312;
    /** Constant used in determining a Radio Link failure. */
    UN_313                              n_313;
    /** Constant used in determining a Radio Link failure. */
    Int16                               n_315;
    /** Timer used in CELL_DCH channel establishment. */
    UT_312                              t_312;
    /** Timer used in determining a Radio Link failure. */
    UT_313                              t_313;
}
SyncParameters;


/** Allows configuring the uplink power control parameters when a DPCH is
 * established.
 */
typedef struct UCphy_UL_DPCH_PowerControlInfoTag
{
    /** Indicates a parameter used to calculate the DPCCH_Initial_power (in dB)
     * to start the inner loop power control.
     * It is calculated as:
     *     DPCCH_Initial_power = DPCCH_Power_offset - CPICH_RSCP.
     * Please refer to <tt> [3G TS 25.331 10.3.6.91] </tt>.
     */
    UDPCCH_PowerOffset                                 dpcch_PowerOffset;

    /** After establishment of the physical channel, send DPCCH and no DPDCH
     * during the number of frames indicated by the parameter.
     * Please refer to <tt> [3G TS 25.331 10.3.6.91] </tt>.
     */
    UPC_Preamble                                       pc_Preamble;

    /** Indicates the number of frames for which no data is transmitted on
     * signalling radio bearers after DPCH establishment.
     * Please refer to <tt> [3G TS 25.331 10.3.6.91] </tt>.
     */
    USRB_delay                                         sRB_delay;

    /** Power control algorithm used for generating the TPC commands in DPCH.
     * Please refer to <tt> [3G TS 25.331 10.3.6.91] </tt>.
     */
    UPowerControlAlgorithm                             powerControlAlgorithm;

}
UCphy_UL_DPCH_PowerControlInfo;

#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
typedef UCphy_UL_DPCH_PowerControlInfo UCphy_UL_DPCH_PowerControlInfoFDD;
#endif

#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/** Allows configuring the uplink power control parameters in DCH state for TDD mode
 */
typedef struct UCphy_UL_DPCH_PowerControlInfoTDD128Tag
{
    /** TPC step size: (1,2,3 dB).
    * Please refer to <tt> [3G TS 25.331 10.3.6.91] </tt>.
    */
    UTPC_StepSizeTDD                                   tpc_StepSize;

    /** UL target SIR.
    * It brepresents PRXDPCHdes with range Integer(-120 to -58 by step of 1) dBm;
    * this this expected rx power at Node-B.
    * Please refer to <tt> [3G TS 25.331 10.3.6.91] </tt>.
    */
    UUL_TargetSIR                                      ul_TargetSIR;

    /** TX_Power of primaryCCPCH at Node-B: used for pathloss estimation.
    * Please refer to <tt> [3G TS 25.331 10.3.6.59] </tt>.
    */
    UPrimaryCCPCH_TX_Power                             primaryCCPCH_TX_Power;

   /** beaconPLEst indicates that the UE may take into account path loss
    * estimated from beacon function physical channels if TRUE.
    * Please refer to <tt> [3G TS 25.331 10.3.6.61] </tt>
    */
    Boolean                                            beaconPLEst;

    /** Gain Factor for special burst.
    * Please refer to <tt> [3G TS 25.331 10.3.5.8] </tt>.
    */
    UGainFactor                                        gainFactor0;
}
UCphy_UL_DPCH_PowerControlInfoTDD128;

/**indicates up link DPCH parameters, include timing advanve and UL CCTrCH configuration*/
typedef struct UL_DPCHInfoTDD128Tag
{

    /**indicated whether timing advance information is present
\assoc PRESENT \ref ul_TimingAdvance */
    Boolean                                            ul_TimingAdvancePresent;
    /** ul_TimingAdvance contains the Timing advance information.
    * NOTE: if there is no UL Timing Advance Control information element,
    * then the Baton HO is to be peformed by L1 and this shall be in accordance
    * with the RlAction in CphyRlCommonSetupReq (CPHY_RADIO_LINK_BATON_HO).
    * Please refer to <tt> [3G TS 25.331 10.3.6.96] </tt>.
    */
    UUL_TimingAdvanceControl_r7                        ul_TimingAdvance;

#if defined (UPGRADE_3G_HSUPA)
    /**indicated whether UL CCTrCH is present*/
    Boolean                                            ul_CCTrCHPresent;
#endif /* UPGRADE_3G_HSUPA */

    /** ul_CCTrCH contains the configuration of UL CCTrCH dedicated type
    * Please refer to <tt> [3G TS 25.331 10.3.6.88] </tt>.
    */
    UUL_CCTrCH_r7                                      ul_CCTrCH;
} UL_DPCHInfoTDD128;

#endif /* UPGRADE_3G_TDD128  || UPGRADE_3G_DOUBLE_DUAL_MODE */

#if defined(UPGRADE_3G_HSUPA)
/** DlEdchRlParameters contains a configuration of AGCH and HICH.
 *  The RGCH is optional.
 */
typedef struct DlEdchRlParametersTag
{
    /** servingEDCH_RL_indicator when TRUE indicates this radio link is the
     * E-DCH serving radio link
     */
    Boolean                             servingEDCH_RL_indicator;

    /** eAGCHPresent AGCH is only present for the serving E-DCH radio link

\assoc PRESENT \ref eAGCHchannelisationCode */
    Boolean                             eAGCHPresent;
    /** eAGCHchannelisationCode
     */
    Int8                                eAGCHchannelisationCode;
    /** eHICHPresent

\assoc PRESENT \ref eHICHInfo */
    Boolean                             eHICHInfoPresent;
    /** eHICHinfo
     */
    UE_HICH_Information                 eHICHInfo;
    /** eRGCHinfoPresent

\assoc PRESENT \ref eRGCHInfo */
    Boolean                             eRGCHInfoPresent;
    /** eRGCHinfo
     */
    UE_RGCH_Information                 eRGCHInfo;
} DlEdchRlParameters;
#endif /* UPGRADE_3G_HSUPA */

#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)

/**This structure contains SIR measurement results*/
typedef struct SIR_MeasurementsTag
{
    /** tfcsId identifies the TFCS used in the measurement. */
    Int8                                tfcsId;
    /** noTimeslots is the number of timeslot used in the SIR measurement. */
    Int8                                noTimeslots;
    /** sir_TimeslotResults contains the result of SIR measurement in 1/8th of dB, range=[-46*8 .. +80*8] */
    SignedInt16                         sir_TimeslotResults[MAX_DL_TIMESLOT];
}
SIR_Measurements;
#endif /* UPGRADE_3G_TDD128  || UPGRADE_3G_DOUBLE_DUAL_MODE */

/**Indicates measurement types, todo: to checked whether it is needed or not*/
typedef enum CphyMeasurementTypeTag
{
    /** type of quality measurement*/
    CPHY_QUALITY_MEASUREMENT
}
CphyMeasurementType;

/**Indicate the BLER for a given transport channel.*/
typedef struct CphyQualityMeasurementTag
{
    /** transportChannelIdentity is the transport channel for which BLER is being reported.*/
    Int8                                transportChannelIdentity;

    /** dlTrChBler contains the BLER (quality) measurement on this transport channel.*/
    UDL_TransportChannelBLER            dlTrChBler;
#if defined (ENABLE_UPLANE_STATISTICS)
    /** Max TrCH Bler statistics */
    UDL_TransportChannelBLER            maxdlTrChBlerStats;
#endif /* ENABLE_UPLANE_STATISTICS */
}
CphyQualityMeasurement;

/** Quality measurement results.*/
typedef struct CphyAdditionalQualityMeasurementTag
{
    /** The identity number used by the UTRAN to identify the measurement.*/
    UMeasurementIdentity                measurementIdentity;

    /** SequenceNumber is an additional identifier used between URRC and UMAC
    * to avoid mis-synchronisation (particularly if a previously configured
    * measurement on this identity used periodic reporting).*/
    Int8                                sequenceNumber; /* same as in MeasReq */

    /** noQualityMeasurements defines the number of measurements to be made.
\assoc ARRAY \ref qualityMeasurements */
    Int8                                noQualityMeasurements;

    /** qualityMeasurements gives the BLER for a given transport channel.*/
    CphyQualityMeasurement              qualityMeasurements[UPS_MAX_DL_NO_OF_TRCH];
#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)

    /** sirTfcsIdMeasurementPresent indicates presence of sirTfcsIdMeasurementResults.
\assoc PRESENT \ref sirTfcsIdMeasurementResults */
    Boolean                             sirTfcsIdMeasurementPresent;

    /** sirTfcsIdMeasurementResults contains the identity of the CCTrCH (TFCS) measured
    * and the measurements for each TS.(Even if only 1 CCTRCH DCH is to be supported). */
    SIR_Measurements                    sirTfcsIdMeasurementResults;
#endif /* UPGRADE_3G_TDD128  || UPGRADE_3G_DOUBLE_DUAL_MODE */
}
CphyAdditionalQualityMeasurement;

/**Indicates any additional measurement results associated with this measurement identity, in meas.*/
typedef struct CphyAdditionalQualityMeasurementListTag
{
    /** number of measurements in the additional measurements array.
\assoc ARRAY \ref add */
    Int8                               numberOfMeasurements;
    /** add contains the measurement data for each additional measurement report passed to URRC.*/
    CphyAdditionalQualityMeasurement   add[maxAdditionalMeas];
}
CphyAdditionalQualityMeasurementList;

/** Contains a list of  quality-based measurement identities
 * to be reported when this measurement identity is reported.*/
typedef struct CphyAdditionalQualityIdentityListTag
{
    /**number of measurement
\assoc ARRAY \ref measurementIdentity */
    Int8                                numberOfMeasurementRequests;
    /** Identity number list used by the UTRAN to identify the measurement. */
    UMeasurementIdentity                measurementIdentity[maxAdditionalMeas];
}
CphyAdditionalQualityIdentityList;

/**Indicates the event (per transport channel) which trigger a quality measurement report.*/
typedef struct CphyQualityEventReportCriteriaTag
{
    /** The number of transport channels for which the event criteria are applied.
\assoc ARRAY \ref qualityEventCriteria */
    Int8                                numberOfTransportChannels;
    /** qualityEventCriteria define the events which trigger a quality measurement report
    * for each transport channel (total number of CRCs, the number of bad CRCs and
    * the number of CRCs between one trigger and the start of the next count.
    * Please refer to <tt> [3G TS 25.331 14.5.2.1] </tt>.
    */
    UQualityReportingCriteriaSingle     qualityEventCriteria[UPS_MAX_DL_NO_OF_TRCH];
}
CphyQualityEventReportCriteria;

/** Indicates the periodicity (for periodic reporting) or the event triggers (for event triggered reporting)
 * which will cause L1 to report measurements to URRC.
 * If "no reporting" is selected in reportCriteriaType, this parameter is not used.*/
typedef union CphyQualityCriteriaTag
{
    /** EventReportCriteria defines the event (per transport channel)
    * which trigger a quality measurement report.*/
    CphyQualityEventReportCriteria      eventReportCriteria;
    /** PeriodicalReportCriteria defines the period between, and
     * the number of measurement reports UMAC is to make to URRC.*/
    UPeriodicalReportingCriteria        periodicalReportCriteria;
    Int8                                dummy; /* for noReporting in Genie */
}
CphyQualityCriteria;

#if defined (UPGRADE_3G_TDD128)
/** uarfcn list for find cell.*/
typedef struct FindCellUarfcnListTag
{
    /** Specifies the number of uarfcns should to be found.
\assoc ARRAY \ref uarfcn */
    Int16                               numberOfUarfcn;

    /** Specifies frequencies to find and camp on cells, If PHY find one cell
     *  of requested frequency, stop find next frequency, report the result
     *  and send SIB to RRC. If not find, try next frequency until all uarfcns
     *  are tried. For One frequency, only synch on best cell of this frequency.
     */
    UUARFCN                             uarfcn[CPHY_MAX_FREQUENCIES_PER_FIND_CELL];
}
FindCellUarfcnList;
#endif /* UPGRADE_3G_TDD128*/

/** uarfcn union for find cell.*/
typedef union UarfcnUnionTag
{
#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
    /** Specifies the downlink frequency on which to find and
     * camp on the cell. */
    UUARFCN                             uarfcn_dl;
#endif

#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
    /** Specifies the frequency list on which to find and
     * camp on the cells. */
    FindCellUarfcnList                  uarfcnList;
#endif
}
UarfcnUnion;

/** Array of cell to find*/
typedef union FindCellArrayUnionTag
{
#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
    /** Specifies the primary scrambling code of the FDD cells.
     *  Up to MAX_STORED_SCRAMBLING_CODES=32 FDD cells can be configured
     */
    UPrimaryScramblingCode              primaryScramblingCode[MAX_STORED_SCRAMBLING_CODES];
#endif
#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
    /** Specifies the cell parameters Id of the TDD128 cells.
     *  Up to MAX_STORED_CELLPARAMETERSID=32 cells can be configured
     */
    UCellParametersID                   cellParametersID[MAX_STORED_CELLPARAMETERSID];
#endif
}
FindCellArrayUnion;

#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/** Indicates the FDD Cell Identifier and Results.*/
typedef struct FddCellInfoTag
{
    /** Specifies the primary scrambling code of the detected cell. */
    UPrimaryScramblingCode              primaryScramblingCode;

    /** Specifies the Ec/No measurement (in 1/8 dB steps) for the CPICH of the
     * detected cell, \ref example.
     */
    SignedInt16                         cpich_Ec_N0;

    /** Specifies the RSCP measurement (in 1/8 dBm steps) for the CPICH of the
     * detected cell, \ref example.
     */
    SignedInt16                         cpich_RSCP;
}
FddCellInfo;
#endif

#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/** Indicates the TDD128 Cell Identifier and Results.*/
typedef struct Tdd128CellInfoTag
{
    /** Specifies the primary scrambling code of the detected cell. */
    UCellParametersID                   cellParametersID;

    /** Specifies the P-CCPCH RSCP measurement (in 1/8 dB steps) for the
     * detected cell, \ref example.
     */
    SignedInt16                         p_ccpchRscp;
}
Tdd128CellInfo;
#endif

/** generic Cell Identifier and Results. */
typedef  union ModeCellInfoUnionTag
{
#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
    /** Indicates the FDD Cell Identifier and Results.*/
    FddCellInfo                         fdd;
#endif
#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
    /** Indicates the TDD128 Cell Identifier and Results.*/
    Tdd128CellInfo                      tdd128;
#endif
}
ModeCellInfoUnion;


#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/** FDD PICH Info. */
typedef struct PchConfigModeFDDTag
{
   /** Indicates if transmit diversity exists or not.*/
   Boolean                             txDiversityIndicator;

   /** Pich config. Please refer to <tt> [3G TS 25.331 10.3.6.49] </tt>.*/
   UPICH_Info_fdd                      pich_Info;
}
PchConfigModeFDD;
#endif

#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/** TDD128 PICH Info. */
typedef struct PchConfigModeTDD128Tag
{
   /** The primaryCCPCH_Info indicates if TSTD are used.
    * Please refer to <tt> [3G TS 25.331 10.3.6.85a] </tt>.*/
   Boolean                             tstdIndicator;
   /** The primaryCCPCH_Info indicates if SCTD are used.
    * Please refer to <tt> [3G TS 25.331 10.3.6.70a] </tt>.*/
   Boolean                             sctdIndicator;
   /** Pich config. Please refer to <tt> [3G TS 25.331 10.3.6.49] </tt>.*/
   UPICH_Info_LCR_r4                   pich_Info;
}
PchConfigModeTDD128;
#endif

/** generic PICH Info. */
typedef union PchConfigModeUnionTag
{
#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
   /** PICH Info for FDD*/
   PchConfigModeFDD                             fdd;
#endif
#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
   /** PICH Info for TDD128 */
   PchConfigModeTDD128                          tdd128;
#endif
}
PchConfigModeUnion;

#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/** FDD RACH config. */
typedef struct RachConfigModeFDDTag
{
    /**Primary CPICH Tx Power for FDD
     * Indicates a parameter used by the physical layer to calculate preamble
     * initial power. It is broadcast in SIB6/SIB5, and is in dBm.
     * Please refer to <tt> [3G TS 25.331 10.3.6.61] </tt>.
     */
    UPrimaryCPICH_TX_Power              primaryCPICH_TX_Power;

    /** Indicates a parameter used by the physical layer to calculate preamble
     * initial power.
     * It is broadcast in SIB7, and if the UE fails to read SIB7 because of
     * bad radio conditions then UE should use the last stored value.
     * Please refer to <tt> [3G TS 25.331 10.3.6.87] </tt>.
     */
    UUL_Interference                    ul_Interference;

    /** Indicates a parameter used by the physical layer to calculate the
     * initial output power on PRACH according to the Open loop power control
     * procedure. It is broadcast in SIB6/SIB5 for FDD.
     * Please refer to <tt> [3G TS 25.331 10.3.6.11] </tt>.
     */
    UConstantValue                      constantValue;

    /** Uplink frequency to be used for the RACH.*/
    UUARFCN                             uarfcn_ul;

    /** Defines the available spreading factors, scrambling code and
     * sub channels for all ASCs.
     * Please refer to <tt> [3G TS 25.331 10.3.6.52] </tt>.
     */
    UPRACH_RACH_Info_fdd                prach_RACH_Info;

    /** Defines the subset of available sub channels and pre-amble signatures
     * for each ASC.
     * Please refer to <tt> [3G TS 25.331 10.3.6.53] </tt>.
     */
    UPRACH_Partitioning_fdd             prach_Partitioning;

    /** Indicates the increment of power level for subsequent preambles for FDD
     * Please refer to <tt> [3G TS 25.331 10.3.6.54] </tt>.
     */
    UPRACH_PowerOffset                  prach_PowerOffset;

    /** Defines the channelisation code, scrambling code, timing of the AICH
     * and whether STTD is active on the AICH for FDD.
     * Please refer to <tt> [3G TS 25.331 10.3.6.2] </tt>.
     */
    UAICH_Info                          aich_Info;

    /** Power per transmitted Acquisition Indicator minus power of the Primary
     * CPICH and is in the range -22 to +5 dB for FDD
     * Please refer to <tt> [3G TS 25.331 10.3.6.3] </tt>.
     */
    UAICH_PowerOffset                   aich_PowerOffset;
}
RachConfigModeFDD;
#endif

#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/** TDD128 RACH config. */
typedef struct RachConfigModeTDD128Tag
{
    /**Primary CCPCH Tx Power for TDD128
     * Indicates a parameter used by the physical layer to calculate preamble
     * initial power. It is broadcast in SIB6/SIB5, and is in dBm.
     * Please refer to <tt> [3G TS 25.331 10.3.6.59] </tt>.
     */
    UPrimaryCCPCH_TX_Power              primaryCCPCH_TX_Power;

    /** Defines the available PRACHs and FPACHs
     * Please refer to <tt> [3G TS 25.331 10.3.6.52] </tt>.
     */
    UPRACH_RACH_Info_LCR_r4             prach_RACH_Info;

    /** Defines the UpPCH shifting position
\assoc PRESENT \ref upPCHpositionInfo */
    Boolean                             upPCHpositionInfoPresent;
    UUpPCHposition_LCR                  upPCHpositionInfo;

    /** Defines the subset of available sub channels and pre-amble signatures
     * for each ASC.
     * Please refer to <tt> [3G TS 25.331 10.3.6.53] </tt>.
     */
   UPRACH_Partitioning_LCR_r4           prach_Partitioning;
}
RachConfigModeTDD128;
#endif

/** generic RACH config Mode. */
typedef union RachConfigModeUnionTag
{
#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
    /** PRACH Partitioning for FDD*/
   RachConfigModeFDD                            fdd;
#endif /* UPGRADE_3G_FDD  || UPGRADE_3G_DOUBLE_DUAL_MODE */

#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
    /** PRACH Partitioning for TDD128*/
   RachConfigModeTDD128                         tdd128;
#endif /* UPGRADE_3G_TDD128  || UPGRADE_3G_DOUBLE_DUAL_MODE */
}
RachConfigModeUnion;

/** generic Secondary-CCPCH Info.
 * For further information refer to
 * <tt> [3G TS 25.331 section:10.3.6.71] </tt>
 */
typedef union SecondaryCCPCHConfigModeUnionTag
{
#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
    /** Secondary-CCPCH Info for FDD.*/
   USecondaryCCPCH_Info_r4_fdd                        fdd;
#endif /* UPGRADE_3G_FDD  || UPGRADE_3G_DOUBLE_DUAL_MODE */
#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
    /** Secondary-CCPCH Info for TDD128.*/
   USecondaryCCPCH_Info_r4_tdd                        tdd128;
#endif /* UPGRADE_3G_TDD128  || UPGRADE_3G_DOUBLE_DUAL_MODE */
}
SecondaryCCPCHConfigModeUnion;


/** SIBs scheduling info (number of segments,initial position, repetition for each SIB).*/
typedef struct SchedulingInfoTag
{
  /** sibId indicates the identity of the SIB.*/
  Int8               sibId;

  /** segmentCount indicates the nb of segments for this SIB
\assoc ARRAY \ref sibOffset */
  Int8               segmentCount;

  /** repetitionPeriod is the repetition period of the SIB.*/
  Int16              repetitionPeriod;

  /** sibPosition indicates initial SIB position in the repetition period.*/
  Int16              sibPosition;

  /** sibOffset contains the offset for each subsequent segment of the SIB
   * (thus, this does not except the 1st segment).*/
  Int8               sibOffset[CPHY_MAX_SIB_OFFSETS];
}
SchedulingInfo;


#ifdef UPGRADE_3G_HSDPA
#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
 /** HS-SCCH configuration for FDD*/
typedef struct HS_SCCHConfigModeFDDTag
{
   /** Specifies the primary scrambling code of the FDD cell. */
   UPrimaryScramblingCode              primaryScramblingCode;

   /** Set of HS-SCCH config
    * Please refer to <tt> [3G TS 25.331 10.3.6.36a] </tt>.
    */
   UHS_SCCH_Info_r7_fdd                scchConfig;

   /** Indicates the measurement feedback information that contains channel
    * quality indicator that may be used to select transport format and
    * resource by HS-DSCH serving Node-B. For FDD, the transmission rate of
    * the measurement report to the network is configured by higher layer
    * signalling.
    * Please refer to <tt> [3G TS 25.331 10.3.6.40a] </tt>.
    */
   UMeasurement_Feedback_Info_r7_fdd   measurementFeedback;

   /** Indicates the information to be used to control the uplink DPCH power. */
   Int8                                deltaAck;

   /** Indicates the information to be used to control the uplink DPCH power. */
   Int8                                deltaNack;

#if defined (UPGRADE_REL6)
   UHARQ_Preamble_Mode                 harq_Preamble_Mode;
#endif /*UPGRADE_REL6*/

   /** Indicates the repetition factor of ACK/NACK sent on the uplink HS-DPCCH
    * in support of error detection and correction on the downlink HS-DSCH.
    */
   Int8                                ackNackRepetitionFactor;
}
HS_SCCHConfigModeFDD;
#endif /* UPGRADE_3G_FDD  || UPGRADE_3G_DOUBLE_DUAL_MODE */

#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/** HS-SCCH configuration for TDD128*/
typedef struct HS_SCCHConfigModeTDD128Tag
{
   /** (up to 4)Set of HS-SCCH config and HS-SICH config
    * Please refer to <tt> [3G TS 25.331 10.3.6.36a] </tt>.
    */
    UHS_SCCH_Info_r7_tdd128             scchConfig;

    /** Contains midamble config for HS-PDSCH
    * Please refer to <tt> [3G TS 25.331 10.3.6.36oo] </tt>.
    */
    UHS_PDSCH_Midamble_Config_TDD128    hs_pdsch_MidambleConfig;
}
HS_SCCHConfigModeTDD128;
#endif /* UPGRADE_3G_TDD128  || UPGRADE_3G_DOUBLE_DUAL_MODE */

/** generic HS-SCCH configuration*/
typedef union HS_SCCHConfigModeUnionTag
{
#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
    /** HS-SCCH configuration for FDD*/
    HS_SCCHConfigModeFDD                          fdd;
#endif /* UPGRADE_3G_FDD  || UPGRADE_3G_DOUBLE_DUAL_MODE */
#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
    /** HS-SCCH configuration for TDD128*/
    HS_SCCHConfigModeTDD128                       tdd128;
#endif /* UPGRADE_3G_TDD128  || UPGRADE_3G_DOUBLE_DUAL_MODE */
}
HS_SCCHConfigModeUnion;
#endif /* UPGRADE_3G_HSDPA */

#if defined(UPGRADE_3G_HSUPA)
#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/** Configurations of E-RUCCH for TDD128.*/
typedef struct E_RUCCHConfigTag
{
   /** syncUlInfoForERucch contains parameters related to the SYNC_UL Tx
    * for E-RUCCH random access procedure: i.e Signatures bitmap,
    * PrxUpCHdes, PwrRampStep, MaxSyncUlTx.
    */
    USYNC_UL_InfoForE_RUCCH             syncUlInfoForERucch;

    /** Defines the UpPCH shifting position
\assoc PRESENT \ref upPCHpositionInfo */
    Boolean                             upPCHpositionInfoPresent;
    UUpPCHposition_LCR                  upPCHpositionInfo;

   /** eRUCCHInfo contains all E-RUCCH configurations: all the PRACHs
    * and FPACHs config (TS position, midambles, codes used).
    */
    UPRACH_Information_LCR_List         eRUCCH_PrachInfo;

   /** eRUCCH_Partitioning parameter defines the subset of available
    * sub channels and pre-amble signatures for each AccessServiceClass (ASC).
    */
    UPRACH_Partitioning_LCR_r4          eRUCCH_Partitioning;

   /** extendedEstimationWindow indicates the number of subframes for which
    * the L1 shall continue to monitor grant on E-AGCH, once it receives an invalid one.
    * It ranges from 2 to 5 subframes, if equal to 0 then it means not used. */
    Int8                                extendedEstimationWindow;
}
E_RUCCHConfig;

/** SchedTransInfoTDD128 contains E-AGCH set, E-HICH set and E-RUCCH parameters. */
typedef struct SchedTransInfoTDD128Tag
{
   /** eAGCHInfo contains the set of E-AGCHs  (up to 4) set configuration.*/
    UE_AGCH_Information_r7_tdd128   eAGCHInfo;

   /** eHICHInfo contains the set of E-HICHs  (up to 4) set configuration.*/
    UE_HICH_Information_TDD128      eHICHInfo;

   /** eRUCCHConfig contains E-RUCCH configuration.*/
    E_RUCCHConfig                   eRUCCHConfig;
}SchedTransInfoTDD128 ;

/** Configurations of E-PUCH for TDD128.*/
typedef struct E_PUCHConfigTag
{
    Boolean                                            snplPresent;

   /** snpl_ReportType indicates which methid to use for comutation of the
    * SNPL metric: type1 or type2.*/
    UE_PUCH_Info_snpl_ReportType                       snpl_ReportType;

   /** prxBASEdes indicates the reference desired power level for E-PUCH.
    * It ranges from -112 to -50 in dBm.*/
    SignedInt8                                         prxBASEdes;

   /** beaconPLEst indicates that the UE may take into account path loss
    * estimated from beacon function physical channels if TRUE.*/
    Boolean                                            beaconPLEst;

   /** tpc_StepSize indicates the TPC step size to use in dB: 1 to 3.*/
    UTPC_StepSizeTDD                                   tpc_StepSize;

   /** pebase_PowerControlGAP indicates the length of a power control gap.
    * The unit is in number of subframes. The default value is 1.
    * Value 255 represents infinite in which case closed loop power control
    * shall always be used.*/
    UPowerControlGap                                   pebase_PowerControlGAP;

   /** ul_SynchronisationParameters indicates Synchronisation step and
    * frequency to be used.*/
    UUL_SynchronisationParameters_r4                   ul_SynchronisationParameters;

   /** e_PUCH_TS_ConfigurationList indicates the midambles configuration for each
    * possible TS E-PUCH.*/
    UE_PUCH_Info_e_PUCH_TS_ConfigurationList_1         e_PUCH_TS_ConfigurationList;
}
E_PUCHConfig;

/** Configurations of physial channels associated with E-DCH for TDD128.*/
typedef struct EDCHConfigModeTDD128Tag
{
   /** rvConfiguration indicates if rv0 or rvTable is to be used for
    * the incremental redundancy version functionality. */
    UUL_AddReconfTransChInformation_r7_harq_Info   rvConfiguration;

   /**The E-RNTI field is used by Layer 1 to determine whether to start
    * encode E-DCH and then transmit.
    * (Note: only primary E-RNTI used in TDD128). */
    UE_RNTI                                   newPrimary_E_RNTI;

   /** schedTransInfoPresent indicates presence of schedTransInfo : used
    * in Scheduled transmission mode.
\assoc PRESENT \ref schedTransInfo */
    Boolean                                   schedTransInfoPresent;

   /** schedTransInfo contains E-AGCH set, E-HICH set and
    * E-RUCCH parameters. */
    SchedTransInfoTDD128                      schedTransInfo;

   /** nonSchedTransGrantInfoPresent indicates presence of nonSchedTransGrantInfo:
    * used in Non-Scheduled transmission mode.
\assoc PRESENT \ref nonSchedTransGrantInfo */
    Boolean                                   nonSchedTransGrantInfoPresent;

   /** nonSchedTransGrantInfo contains the resource grant of E-PUCH and the
    * configuration of its associated E-HICH. */
    UNon_ScheduledTransGrantInfoTDD_tdd128    nonSchedTransGrantInfo;

   /** ePUCHConfig contains the configuration of E-PUCH. */
    E_PUCHConfig                              ePUCHConfig;

    /** Indicates the E-DCH Physical Layer Category as defined in 25.306 table 5.1m */
    Int8                                      tdd_edch_PhysicalLayerCategory;
}
EDCHConfigModeTDD128;
#endif /* UPGRADE_3G_TDD128  || UPGRADE_3G_DOUBLE_DUAL_MODE */

#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/** Configurations of physial channels associated with E-DCH for FDD.*/
typedef struct EDCHConfigModeFDDTag
{
    /** rvConfigurationindicates the redundancy version used on E-DCH
     */
    UUL_AddReconfTransChInformation_r7_harq_Info rvConfiguration;
    /** resetMac_es_e indicates to L1 to flush all HARQ processes of data
     */
    Boolean                             resetMac_es_e;
    /** eDchTtiLength indicates whether a 2ms or a 10ms E-DCH TTI is configured.
     */
    UE_DCH_TTI                          eDchTtiLength;
    /** newPrimary_E_RNTIPresent

\assoc PRESENT \ref newPrimary_E_RNTI */
    Boolean                             newPrimary_E_RNTIPresent;
    /** newPrimary_E_RNTI
     */
    Int16                               newPrimary_E_RNTI;
    /** newSecondary_E_RNTIPresent

\assoc PRESENT \ref newSecondary_E_RNTI */
    Boolean                             newSecondary_E_RNTIPresent;
    /** newSecondary_E_RNTI
     */
    Int16                               newSecondary_E_RNTI;

    /** puncturingLimitNonMax
     */
    Int8                                puncturingLimitNonMax;
    /** maxChannelisationCode indicates the maximum number of channelisation
     * codes allowed to be used for E-DCH. This limites the E-TFCS (i.e. the
     * maximum E-DCH transport block size).
     */
    EdchSf                              maxChannelisationCode;

    /** threeIndexStepThreshold and twoIndexStepThreshold are required to
     * determine the serving grant according to TS 25.321 section 9.2.5.2.1.
     */
    Int8                                threeIndexStepThreshold;
    Int8                                twoIndexStepThreshold;

    /** schedTransmConfiguration is required by L1 in order to implement
     * the Serving Grant Update function.*/
    UE_DPDCH_SchedulingTransmConfiguration schedTransmConfiguration;
}
EDCHConfigModeFDD;
#endif /* UPGRADE_3G_FDD  || UPGRADE_3G_DOUBLE_DUAL_MODE */

/**generic configurations of physial channels associated with E-DCH.*/
typedef union EDCHConfigModeUnionTag
{
#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
    /** Configurations of physial channels associated with E-DCH for FDD.*/
    EDCHConfigModeFDD                             fdd;
#endif /* UPGRADE_3G_FDD  || UPGRADE_3G_DOUBLE_DUAL_MODE */
#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
    /** Configurations of physial channels associated with E-DCH for TDD128.*/
    EDCHConfigModeTDD128                          tdd128;
#endif /* UPGRADE_3G_TDD128  || UPGRADE_3G_DOUBLE_DUAL_MODE */
}
EDCHConfigModeUnion;
#endif /* UPGRADE_3G_HSUPA */

#if defined (UPGRADE_3G_MBMS)
/** Defines MTCH scheduling information*/
typedef struct MtchSchedulingInfoEachTrChTag
{
    /** Identity of the transport channel. */
    Int8                                transportChannelIdentity;

    /** Indicates whether the tdmInfoList is present or not. */
    Boolean                             tdmInfoPresent;

    /** Indicates the time division multiplexing information of the MTCH. */
    UMBSFN_TDM_Info_List                tdmInfoList;

    /** Indicates the scheduling information from MSCH is present or not. */
    Boolean                             schedulingInfoInMschPresent;

    /** One or more sets of scheduling information comprising of the beginning
     * and duration of an MBMS service transmission for one scheduling period.
     */
    UMBMS_ServiceSchedulingInfoList_r6  schedulingInfoInMsch;
}
MtchSchedulingInfoEachTrCh;
#endif /* UPGRADE_3G_MBMS */

#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)

#if defined (UPGRADE_REL6_FDPCH)
/* same as UDL_FDPCH_InfoCommon_r6 ASN.1 type but without cfnHanding field,
* to allow factorization with DPCHInfoCommon one.*/
typedef struct DlFdpchInfoCommonTag
{
/** \assoc PRESENT \ref dl_FDPCH_PowerControlInfo */
    Boolean                                            dl_FDPCH_PowerControlInfoPresent;
    UDL_DPCH_PowerControlInfo                          dl_FDPCH_PowerControlInfo;
/** \assoc PRESENT \ref dl_FDPCH_TPCcommandErrorRate */
    Boolean                                            dl_FDPCH_TPCcommandErrorRatePresent;
    Int8  /* 1 to 16 */                                dl_FDPCH_TPCcommandErrorRate;

} DlFdpchInfoCommon;
#endif /* UPGRADE_REL6_FDPCH */

/** Rl Common Setup Configuration for FDD. */
typedef struct RlCommonSetupConfigModeFDDTag
{
    /** Indicates the uplink frequency of the radio link. If this parameter is
     * not given in an IE then default duplex distance defined for the
     * operating frequency band shall be used.
     */
    UUARFCN                             uarfcn_ul;

    /** Indicates the \c sRB_delay and \c pc_Preamble parameters for the DCH
     * establishment procedure.*/
    UCphy_UL_DPCH_PowerControlInfoFDD   ul_DPCH_PowerControlInfo;

    /** Refer to <tt> [3G TS 25.331 section 10.3.6.88] </tt> for more
     * information.
     *
     * Indicates the parameters like (FDD):
     *      - Scrambling code type & Scrambling code number
     *      - Number of DPDCH
     *      - Spreading factor
     *      - TFCI existence
     *      - Number of FBI bits
     *      - Puncturing limit
     */
    UUL_DPCH_Info_r7_fdd                ul_DPCH_Info;

#if !defined (UPGRADE_REL6_FDPCH)
    UDL_DPCH_InfoCommon_r6_fdd            dl_DPCH_InfoCommon;
#if defined (UPGRADE_REL6)
    /** cfnHandling for DPCH of FDPCH.
    \assoc UNION \ref cfnHandling */
    union
    {
        UDL_DPCH_InfoCommon_r6_cfnHandling    dpchCfnHandling;
        Int8                                  dummy;
    } cfnHandling;
#endif /* UPGRADE_REL6 */

#else /* !UPGRADE_REL6_FDPCH */
    /** Defines DPCH mode.
    \assoc UNION \ref infoCommon
    \assoc UNION \ref cfnHandling */
    Boolean                             isDpchOrFdpch;  /** FALSE if DPDCH, TRUE if FDPCH*/

    union
    {
        UDL_DPCH_InfoCommon_r6_fdd       dl_DPCH_InfoCommon;
        DlFdpchInfoCommon                dl_FDPCH_InfoCommon;
    } infoCommon;

    /** cfnHandling for DPCH of FDPCH.*/
    union
    {
        UDL_DPCH_InfoCommon_r6_cfnHandling    dpchCfnHandling;
        UDL_FDPCH_InfoCommon_r6_cfnHandling   fdpchCfnHandling;
    } cfnHandling;
#endif /* UPGRADE_REL6_FDPCH */

   /** Refer to <tt> [3G TS 25.331 section 10.3.6.18] </tt> for more
     * information.*/
#if defined (UPGRADE_REL6)
    Boolean                                   postVerificationPeriod;
#endif /* UPGRADE_REL6 */

   /** Indicates the Default DPCH Offset Value, and it is used when the UE
     * enters CELL_DCH state or when a timing re-initialised hard handover is
     * performed. Valid range for defaultDPCH_OffsetValue: FDD 0..599.
     * Please refer to <tt> [3G TS 25.331 10.3.6.16] </tt>.
     */
    UDefaultDPCH_OffsetValueFDD         defaultDPCH_OffsetValue;

   /** Indicates the kind of diversity to be used.
     *
     * Different diversity modes can be:
     *     - STTD
     *     - Closed loop mode 1
     *     - Closed loop mode 2
     * Please refer to <tt> [3G TS 25.331 10.3.6.86] </tt>.
     */
    UTX_DiversityMode                   tx_DiversityMode;
}
RlCommonSetupConfigModeFDD;
#endif

#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/** Rl Common Setup Configuration for TDD128. */
typedef struct RlCommonSetupConfigModeTDD128Tag
{
/** \assoc PRESENT \ref secondaryfrequencyInfo */
    Boolean                             secondaryfrequencyInfoPresent;
    UUARFCN                             secondaryfrequencyInfo; /**< if present: this parameter substitutes uarfcn_dl provided in CphyRlCommonSetupReq */
/** \assoc PRESENT \ref fPachFrequencyInfo */
    Boolean                             fPachFrequencyInfoPresent;
    UUARFCN                             fPachFrequencyInfo;
/** \assoc PRESENT \ref upPCHpositionInfo */
    Boolean                             upPCHpositionInfoPresent;
    UUpPCHposition_LCR                  upPCHpositionInfo;

    UCphy_UL_DPCH_PowerControlInfoTDD128   ul_DPCH_PowerControlInfo;

    /** ul_DPCH_Info describes the UL CCTrCH allocated
     * Refer to <tt> [3G TS 25.331 section 10.3.6.88] </tt> for more
     * information.*/
    UL_DPCHInfoTDD128                   ul_DPCH_Info;

    /** Refer to <tt> [3G TS 25.331 section 10.3.6.18] </tt> for more
     * information.*/
    UDL_DPCH_InfoCommon_r6_tdd          dl_DPCH_InfoCommon;

    /** Refer to <tt> [3G TS 25.331 section 10.3.6.16] </tt> for more
     * information. */
    UDefaultDPCH_OffsetValueTDD         defaultDPCH_OffsetValue;

    /** Indicates usage of TSTD diverity in TDD128 only.
     * Please refer to <tt> [3G TS 25.331 10.3.6.85a] </tt>.
     */
    Boolean                             tstdIndicator;

    /** Indicates UL Special Burst generation period in radio frames.
     * Please refer to <tt> [3G TS 25.331 10.3.6.75a] </tt>. If
     * specialBurstSchedulingPresent is false, use the default value
     * for specialBurstScheduling. Once receive the UPCC message with
     * special burst scheduling , this Boolean will be set to TURE all
     * the time and use the new value for the specialBurstScheduling.
     */
    Boolean                             specialBurstSchedulingPresent;
    USpecialBurstScheduling             specialBurstScheduling;
}
RlCommonSetupConfigModeTDD128;
#endif

/**generic Rl Common Setup Configuration. */
typedef union RlCommonSetupConfigModeUnionTag
{
#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
        RlCommonSetupConfigModeFDD                               fdd;
#endif /* UPGRADE_3G_FDD  || UPGRADE_3G_DOUBLE_DUAL_MODE */
#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
        RlCommonSetupConfigModeTDD128                            tdd128;
#endif /* UPGRADE_3G_TDD128  || UPGRADE_3G_DOUBLE_DUAL_MODE */
}
RlCommonSetupConfigModeUnion;

#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
 /** RlSetup Configuration including all DL CCTrCHs description for FDD. */
typedef struct RlSetupConfigModeFDDTag
{
    /** Specifies the primary scrambling code of the FDD cell. */
    UPrimaryScramblingCode              primaryScramblingCode;

    /** downlink DPCH Info per RL.
    * It describes all the DL CCTrCHs allocated.
    * Please refer to <tt> [3G TS 25.331 10.3.6.21] </tt>.
    */
#if !defined (UPGRADE_REL6_FDPCH)
       UDL_DPCH_InfoPerRL_r7_fdd           dl_DPCH_InfoPerRL;
#else /* UPGRADE_REL6_FDPCH*/
/** Defines DPCH mode.
\assoc UNION \ref infoPerRL */
    Boolean                                isDpchOrFdpch;  /** FALSE if DPDCH, TRUE if FDPCH*/
    union
    {
       UDL_DPCH_InfoPerRL_r7_fdd           dl_DPCH_InfoPerRL;
       UDL_FDPCH_InfoPerRL_r7              fDPCH_InfoPerRL;
    } infoPerRL;
#endif /* UPGRADE_REL6_FDPCH*/

#if defined(UPGRADE_3G_HSUPA)
    /** Indicates this RL is part of the E-DCH active set.
\assoc PRESENT \ref dlEdchRlParameters */
    Boolean                             dlEdchRlParametersPresent;

    /** dlEdchRlParameters contains the parameters to configure the
     * AGCH, RGCH, and HICH */
    DlEdchRlParameters                  dlEdchRlParameters;
#endif /* UPGRADE_3G_HSUPA */
}
RlSetupConfigModeFDD;
#endif

#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
 /** RlSetup Configuration including all DL CCTrCHs description for TDD128. */
typedef struct RlSetupConfigModeTDD128Tag
{
    /** Specifies the cell parameters Id of the cell for TDD128 */
    UCellParametersID                   cellParametersID;

    /** Describe the configuration of the DL CCTrCH DCH
    * Please refer to <tt> [3G TS 25.331 10.3.6.21] </tt>.
    */
    UDL_CCTrCh_r7                       dl_CCTrCH_Info;
}
RlSetupConfigModeTDD128;
#endif /* UPGRADE_3G_TDD128  || UPGRADE_3G_DOUBLE_DUAL_MODE */

/** generic RlSetup Configuration */
typedef union RlSetupConfigModeUnionTag
{
#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
        RlSetupConfigModeFDD                               fdd;
#endif /* UPGRADE_3G_FDD  || UPGRADE_3G_DOUBLE_DUAL_MODE */
#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
        RlSetupConfigModeTDD128                            tdd128;
#endif /* UPGRADE_3G_TDD128  || UPGRADE_3G_DOUBLE_DUAL_MODE */
}
RlSetupConfigModeUnion;


/** Array of UMTS cell info*/
typedef union UmtsCellArrayUnionTag
{
#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
    /** Defines a FDD cell.*/
    UmtsCellFDD                         fdd [maxCellMeas];
#endif
#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
    /** Defines a TDD128 cell.*/
    UmtsCellTDD128                      tdd128 [maxCellMeas];
#endif
}
UmtsCellArrayUnion;

#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/**FDD cell measurement request, indicate whether CPICH Ec/No or CPICH RSCP are requested or not */
typedef struct CellMeasurementReqFDDTag
{
    /** Indicates if CPICH Ec/No measurements are requested or not. */
    Boolean                             cpichEcNo;

    /** Indicates if CPICH RSCP measurements are requested or not. */
    Boolean                             cpichRscp;
}
CellMeasurementReqFDD;
#endif

#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/**TDD128 cell measurement request, indicate whether P-CCPCH RSCP or timeslot ISCP are requested or not */
typedef struct CellMeasurementReqTDD128Tag
{
  /** Indicates if P-CCPCH RSCP measurements are requested or not. */
    Boolean                             p_ccpchRscp;

  /** Indicates if timeslot ISCP measurements are requested or not on serving.
    * \note timeslotIscp measurement can not be requested for detected cells.*/
    Boolean                             timeslotIscp;
}
CellMeasurementReqTDD128;
#endif

/**generic cell measurement request*/
typedef  union CellMeasurementReqUnionTag
{
#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/**FDD cell measurement request*/
    CellMeasurementReqFDD               fdd;
#endif
#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/**TDD128 cell measurement request*/
    CellMeasurementReqTDD128            tdd128;
#endif
}
CellMeasurementReqUnion;

/** generic UeInternalMeasurementQuantity. */
typedef union UeInternalMeasurementQuantityUnionTag
{
#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
    /** Indicates if Rx to Tx time difference measurements are requested for
     * all radio links in the active set or not.
     */
    Boolean                             ue_RX_TX_TimeDifference;
#endif
#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
    /** Indicates if Timing Advance measurements are requested or not
     */
    Boolean                             ue_T_ADVinfo;
#endif
}
UeInternalMeasurementQuantityUnion;

/** Array of detected cell currently monitored*/
typedef union DetectedCellArrayUnionTag
{
#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
    /** Specifies the primary scrambling code of the FDD cell.
     * UPS_MAX_MEASURED_INTRA_FREQ_CELLS is equal to 12.
     */
    UPrimaryScramblingCode              primaryScramblingCode[UPS_MAX_MEASURED_INTRA_FREQ_CELLS];
#endif
#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
    /** Specifies the cell parameters Id of the TDD128 cell.
    * UPS_MAX_MEASURED_INTRA_FREQ_CELLS is equal to 12.
    */
    UCellParametersID                   cellParametersID[UPS_MAX_MEASURED_INTRA_FREQ_CELLS];
#endif
}
DetectedCellArrayUnion;

#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/**Indicates FDD serving cell measurement result. */
typedef FddCellInfo           CellMeasurementReportFDDserving;
#endif

#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/**Indicates TDD128 serving cell measurement result. */
typedef struct CellMeasurementReportTDD128servingTag
{
    /** Specifies the cell parameters Id of the TDD128 cell. */
    UCellParametersID                   cellParametersID;

    /** Specifies whether P-CCPCH RSCP measured result is present or not
\assoc PRESENT \ref p_ccpchRscp */
    Boolean                             p_ccpchRscpPresent;

    /** Specifies the RSCP measurement for the P-CCPCH in 1/8 dBm steps, \ref
     * example
     */
    SignedInt16                         p_ccpchRscp;

    /** Specifies the ISCP measurement for the P-CCPCH in 1/8 dBm steps, \ref
     * example .(The invalid value is defined as 0x7FFF).
     */
    SignedInt16                         timeslotIscp[MAX_DL_TIMESLOT];
}
CellMeasurementReportTDD128serving;
#endif

/** generic serving cell measurement report. */
typedef  union ServingCellMeasurementReportUnionTag
{
#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/** FDD serving cell measurement report. */
    CellMeasurementReportFDDserving    fdd;
#endif
#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/** TDD128 serving cell measurement report. */
    CellMeasurementReportTDD128serving tdd128;
#endif
}
ServingCellMeasurementReportUnion;

#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/**Indicates TDD128 intra-frequency cells measurement report. */
typedef struct CellMeasurementReportTDD128Tag
{
    /** Specifies the cell parameters Id of the TDD128 cell. */
    UCellParametersID                   cellParametersID;

    /** Indicates if \c p_ccpchRscp is present or not.
\assoc PRESENT \ref p_ccpchRscp */
    Boolean                             p_ccpchRscp_Present;

    /** Specifies the RSCP measurement for the P-CCPCH in 1/8 dBm steps, \ref
     * example
     */
    SignedInt16                         p_ccpchRscp;

    /** Specifies the ISCP measurement for the P-CCPCH in 1/8 dBm steps, \ref
     * example .(The invalid value is defined as 0x7FFF).
     */
    SignedInt16                         timeslotIscp[MAX_DL_TIMESLOT];


    /** Indicates if \c sfnToSfnChipOffset is present
     * or not.

\assoc PRESENT \ref sfnToSfnChipOffset */
    Boolean                             sfnToSfnOffsetPresent;

    /** Specifies the chip offset from the current cell to the monitored cell.
     * It is in the range 0 to 38399 chips.
     */
    Int16                               sfnToSfnChipOffset;

    /** Indicates if \c cfn and \c sfn used or not. Frame offset is calculated
     * by RRC since frame offset requires 4 LSBs of HFN with a 8 bit CFN in
     * order to do a 12 bit comparison.

\assoc PRESENT \ref cfn
\assoc PRESENT \ref sfn */
    Boolean                             cfnToSfnFrameOffsetPresent;

    /** Specifies the CFN of the reference cell. */
    Int8                                cfn;

    /** Specifies the SFN of the target cell (0 to 4095). */
    Int16                               sfn;
}
CellMeasurementReportTDD128;
#endif /* UPGRADE_3G_TDD128  || UPGRADE_3G_DOUBLE_DUAL_MODE */

/** generic intra-frequency cells measurement report. */
typedef  union IntraFreqCellMeasurementReportUnionTag
{
#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/** FDD intra-frequency cells measurement report. */
    CellMeasurementReportFDD           fdd[CPHY_MAX_REPORTS];
#endif
#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/** TDD128 intra-frequency cells measurement report. */
    CellMeasurementReportTDD128        tdd128[CPHY_MAX_REPORTS];
#endif
}
IntraFreqCellMeasurementReportUnion;

#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/** Indicates RACH measurement reports used for FDD neighbour cells*/
typedef URachMeasurementReport CellMeasurementReportFDDonRach;

#ifdef ENABLE_INTER_FREQ_MEAS_ON_RACH
#ifdef UPGRADE_REL6
/** Defines the measurement results for inter-frequency cell results on RACH.
 */
typedef struct UInterFreqRachMeasurementReportTag
{
    /** Specifies the cell identity of the cell. */
    Int8                                cellId;

    /** Indicates if \c cpich_Ec_No is present or not. */
    Boolean                             cpich_Ec_N0Present;

    /** Specifies the Ec/No measurement for the CPICH in 1/8 dB steps, \ref
     * example
     */
    SignedInt16                         cpich_Ec_N0;

    /** Indicates if \c cpich_RSCP is present or not. */
    Boolean                             cpich_RSCP_Present;

    /** Specifies the RSCP measurement for the CPICH in 1/8 dBm steps, \ref
     * example
     */
    SignedInt16                         cpich_RSCP;
}
UInterFreqRachMeasurementReport;
#endif /* UPGRADE_REL6 */
#endif /* ENABLE_INTER_FREQ_MEAS_ON_RACH */
#endif

#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/** Indicates RACH measurement reports used for TDD128 neighbour cells*/
typedef struct CellMeasurementReportTDD128onRachTag
{
    /** Specifies the cell parameters Id of the TDD128 cell.*/
    UCellParametersID                   cellParametersID;

    /** Indicates if \c p_ccpchRscp is present or not.
\assoc PRESENT \ref p_ccpchRscp */
    Boolean                             p_ccpchRscp_Present;

    /** Specifies the RSCP measurement for the P-CCPCH in 1/8 dBm steps, \ref
     * example
     */
    SignedInt16                         p_ccpchRscp;
    /** Indicates if \c sfnToSfnChipOffset is present
     * or not.

\assoc PRESENT \ref sfnToSfnChipOffset */
    Boolean                             sfnToSfnOffsetPresent;

    /** Specifies the chip offset from the current cell to the monitored cell.
     * It is in the range 0 to 38399 chips.
     */
    Int16                               sfnToSfnChipOffset;
}
CellMeasurementReportTDD128onRach;
#endif /* UPGRADE_3G_TDD128  || UPGRADE_3G_DOUBLE_DUAL_MODE */

#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/** Results for RACH measuremnt for FDD cells */
typedef struct RachMeasurementReportFDDTag
{
    /** The Uarfcn of the current serving cell. */
    UUARFCN                             uarfcn;

    /** Indicates the report of the CPICH RSCP and Ec/No measurements for the
     * serving cell.
     */
    CellMeasurementReportFDDserving     servingCellRachReport;

    /** Indicates the number of neighbour cells for which the intra-frequency
     * measurement reports are reported in the parameter \c
     * neighbourCellRachReport.

\assoc ARRAY \ref neighbourCellRachReport */
    Int8                                numberOfNeighbourCells;

    /** Indicates an array of RACH measurement reports, and the number of
     * reports should be as given by the parameter \c numberOfNeighbourCells.
     */
    CellMeasurementReportFDDonRach      neighbourCellRachReport[CPHY_MAX_REPORT_ON_RACH];
}
RachMeasurementReportFDD;
#endif

#ifdef ENABLE_INTER_FREQ_MEAS_ON_RACH
#if defined (UPGRADE_3G_FDD) && defined (UPGRADE_REL6)
/** Results for inter-freq RACH measuremnt for FDD cells */
typedef struct InterFreqRachMeasurementReportFDDTag
{
    /** Indicates the number of neighbour cells for which the intra-frequency
     * measurement reports are reported in the parameter \c
     * neighbourCellRachReport.

\assoc ARRAY \ref neighbourCellRachReport */
    Int8                                numberOfNeighbourCells;

    /** Indicates an array of RACH measurement reports, and the number of
     * reports should be as given by the parameter \c numberOfNeighbourCells.
     */
    UInterFreqRachMeasurementReport     neighbourCellRachReport[CPHY_INTER_FREQ_MAX_REPORT_ON_RACH];
}
InterFreqRachMeasurementReportFDD;
#endif
#endif /* ENABLE_INTER_FREQ_MEAS_ON_RACH */

#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/** Results for RACH measuremnt for TDD128 cells */
typedef struct RachMeasurementReportTDD128Tag
{
    /** The Uarfcn of the current serving cell. */
    UUARFCN                             uarfcn;

    /** Indicates the report of the P-CCPCH RSCP and Timeslot ISCP measurements for the
     * serving cell.
     */
    CellMeasurementReportTDD128serving    servingCellRachReport;

    /** Indicates the number of neighbour cells for which the intra-frequency
     * measurement reports are reported in the parameter \c
     * neighbourCellRachReport.

\assoc ARRAY \ref neighbourCellRachReport */
    Int8                                numberOfNeighbourCells;

    /** Indicates an array of RACH measurement reports, and the number of
     * reports should be as given by the parameter \c numberOfNeighbourCells.
     */
    CellMeasurementReportTDD128onRach     neighbourCellRachReport[CPHY_MAX_REPORT_ON_RACH];
}
RachMeasurementReportTDD128;
#endif

/** generic RachMeasurementReport. */
typedef union RachMeasurementReportUnionTag
{
#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/** FDD rach measurement report. */
    RachMeasurementReportFDD           fdd;
#endif
#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/** TDD128 rach measurement report. */
    RachMeasurementReportTDD128        tdd128;
#endif
}
RachMeasurementReportUnion;

#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/** Results for Inter-frequency FDD cells */
typedef struct InterFreqCellMeasurementReportFDDTag
{
    /** The frequency on which the measurement was made. */
    UUARFCN                             uarfcn;

    /** Indicates if \c rssi is present or not
\assoc PRESENT \ref rssi */
    Boolean                             rssiPresent;

    /** Indicates wideband RSSI measurement reports */
    UtraRssiReport                      rssi;

    /* Other measurement data.
     *  Note: this is a larger structure and hence is better placed at the
     *        end of the structure such that allocated memory can be
     *        truncated if fewer than CPHY_MAX_REPORTS worth of cells
     *        need reporting.
     */
    /** Indicates the number of inter-frequency cells for which reports are
     * given by the parameter \c interFrequencyReport.

\assoc ARRAY \ref interFrequencyReport */
    Int8                                numberOfCells;

    /** Indicates an array of inter-frequency cell measurement reports, and the
     * number of reports should be as given by the parameter \c numberOfCells.
     */
    CellMeasurementReportFDD           interFrequencyReport [CPHY_MAX_REPORTS];
}
InterFreqCellMeasurementReportFDD;
#endif /* UPGRADE_3G_FDD  || UPGRADE_3G_DOUBLE_DUAL_MODE */

#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/** Results for Inter-frequency TDD128 cells */
typedef struct InterFreqCellMeasurementReportTDD128Tag
{
    /** The frequency on which the measurement was made. */
    UUARFCN                             frequency;

    /** Indicates if \c rssi is present or not */
    Boolean                             rssiPresent;

    /** The wideband RSSI measurement result for this frequency and is in
     * 1/8 dBm steps, for example -400 = -50 dBm.
     */
    SignedInt16                         utra_CarrierRSSI;

    /* Other measurement data.
     *  Note: this is a larger structure and hence is better placed at the
     *        end of the structure such that allocated memory can be
     *        truncated if fewer than CPHY_MAX_REPORTS worth of cells
     *        need reporting.
     */
    /** Indicates the number of inter-frequency cells for which reports are
     * given by the parameter \c interFrequencyReport.

\assoc ARRAY \ref interFrequencyReport */
    Int8                                numberOfCells;

    /** Specifies the RSCP measurement for Serving cell P-CCPCH in 1/8 dBm steps.
     * (The invalid value is defined as 0x8000)*/
    SignedInt16                         sCellPccpchRscp;

    /** Indicates an array of inter-frequency cell measurement reports, and the
     * number of reports should be as given by the parameter \c numberOfCells.
     */
    CellMeasurementReportTDD128           interFrequencyReport [CPHY_MAX_REPORTS];
}
InterFreqCellMeasurementReportTDD128;
#endif /* UPGRADE_3G_TDD128  || UPGRADE_3G_DOUBLE_DUAL_MODE */

/** generic inter-frequency cells measurement report. */
typedef union InterFreqCellMeasurementReportUnionTag
{
#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/** FDD inter-frequency cells measurement report. */
   InterFreqCellMeasurementReportFDD         fdd;
#endif
#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/** TDD128 inter-frequency cells measurement report. */
   InterFreqCellMeasurementReportTDD128      tdd128;
#endif
}
InterFreqCellMeasurementReportUnion;

#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/** Results for detected FDD cells measurement */
typedef CellMeasurementReportFDD CellMeasurementReportFDDdetected;
#endif /* UPGRADE_3G_FDD  || UPGRADE_3G_DOUBLE_DUAL_MODE */

#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/** Results for detected TDD128 cells measurement */
typedef struct CellMeasurementReportTDD128detectedTag
{
    /** Specifies the cell parameters Id of the TDD128 cell. */
    UCellParametersID                   cellParametersID;

    /** Specifies the RSCP measurement for the P-CCPCH in 1/8 dBm steps, \ref
     * example
     */
    SignedInt16                         p_ccpchRscp;
}
CellMeasurementReportTDD128detected;
#endif /* UPGRADE_3G_TDD128  || UPGRADE_3G_DOUBLE_DUAL_MODE */

/** generic DetectedMeasurementReport. */
typedef  union DetectedCellMeasurementReportArrayUnionTag
{
#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
    /**FDD detected cell measurement report*/
    CellMeasurementReportFDDdetected   fdd[CPHY_MAX_REPORTS];
#endif
#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
    /**FDD detected cell measurement report*/
    CellMeasurementReportTDD128detected   tdd128[CPHY_MAX_REPORTS];
#endif
}
DetectedCellMeasurementReportArrayUnion;

#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/** FDD specific UeInternalMeasurementResult. */
typedef struct UeInternalMeasurementResultFDDTag
{
    /** Indicates if \c ue_TransmittedPower is present or not.
\assoc PRESENT \ref ue_TransmittedPower */
    Boolean                             ue_TransmittedPowerPresent;

    /** Indicates the UE transmit power in the range -50 to 33 dBm in 1/8 dBm
     * steps, for example 264 = 33 dBm. The invalid value is defined as 0x7FFF.
     */
    SignedInt16                         ue_TransmittedPower;
    /** Indicates the number of radio links for which it reports the Rx to Tx
     * time difference for the cells identified by the associated PSC.

\assoc ARRAY \ref primaryScramblingCode
\assoc ARRAY \ref ue_RX_TX_TimeDiffResult */
    Int8                                numberOfRadioLinks;

    /** Indicates an array of PSCs for which Rx to Tx time differences are
     * given in \c ue_RX_TX_TimeDiffResult.
     */
    UPrimaryScramblingCode              primaryScramblingCode [UPS_MAX_RADIO_LINKS];

    /** Indicates the results of all the Rx to Tx time differences for the
     * cells identified by the associated PSC.
     * Please refer to <tt> [3G TS 25.331 10.3.7.83] </tt>.
     */
    UUE_RX_TX_TimeDifferenceType1        ue_RX_TX_TimeDiffResult [UPS_MAX_RADIO_LINKS];
}
UeInternalMeasurementResultFDD;
#endif

#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/** the Timing advance result for TDD128.*/
typedef struct T_ADVinfoTag
{
    /** Rx to Tx Timing advance in 1/8th of chips, range [0..8191] */
    Int16                               t_ADV;
    /** sfn of the frame in which measurement was done, range [0..4095] */
    Int16                               sfn;
}
T_ADVinfo;
#endif

#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/** TDD128 specific UeInternalMeasurementResult. */
typedef struct UeInternalMeasurementResultTDD128Tag
{
    /** Indicates the UE transmit power in the range -50 to 33 dBm in 1/8 dBm
     * steps, for example 264 = 33 dBm. The invalid value is defined as 0x7FFF.
     */
    SignedInt16                         ue_TransmittedPower[MAX_UL_TIMESLOT];

    /** Indicates if \c ue_T_ADVinfo is present or not.
\assoc PRESENT \ref ue_T_ADVinfo */
    Boolean                             ue_T_ADVinfo_Present;

    /** contains the Timing Advance measurement results */
    T_ADVinfo                           ue_T_ADVinfo;
}
UeInternalMeasurementResultTDD128;
#endif

/** generic UeInternalMeasurementResult. */
typedef union UeInternalMeasurementResultUnionTag
{
#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
    /** FDD specific UeInternalMeasurementResult. */
    UeInternalMeasurementResultFDD      fdd;
#endif
#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
    /** TDD128 specific UeInternalMeasurementResult. */
    UeInternalMeasurementResultTDD128   tdd128;
#endif
}
UeInternalMeasurementResultUnion;





/** \defgroup SigCphyUeCap UE Capability Primitives
 * This section defines the primitives exchanged between PHY and RRC to convey
 * UE capability information.
 *
 * @{
 */

/* Add this to avoid the compile error in 2G single mode, while cipher mode set to SOFTWARE */
#if defined (UPGRADE_3G)
/** Power class configuration request.
 * This primitive is sent from RRC to PHY to inform the physical layer of the
 * power class of the UE. The power class of the UE can change if the UE is
 * inserted or removed from an external power amplifier.
 *
 */
typedef struct CphyUeCapabilityConfigReqTag
{
#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)

/** TODO : UUE_PowerClass (single Band) should be removed, since agreed to keep
* UPGRADE_FDD_MULTIBAND code permanently */
#if !defined (UPGRADE_FDD_MULTIBAND)
    /** Indicates the power class of the UE and it is in the range 1 to 4.
     * However, only power class 3 and 4 are valid for Release 99.
     */
    UUE_PowerClass                      ue_PowerClass;
#else
    /** Indicates the power class of the UE for each supported FDD band.
     * MAX_NUM_FDD_BANDS=3 bands are configured.
     */
     FddBandInfo                         fddBandInfo;
#endif
#endif
#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
    /** Indicates the power class of the UE for each supported TDD128 band.
     * MAX_NUM_TDD128_BANDS=3 bands are configured.
     */
     Tdd128BandInfo                      tdd128BandInfo;
#endif
}
CphyUeCapabilityConfigReq;
#endif /* UPGRADE_3G */

/** Power class configuration confirmation.
 * This primitive is sent from PHY to RRC in response to a
 * #CphyUeCapabilityConfigReq primitive.
 */
typedef struct CphyUeCapabilityConfigCnfTag
{
#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
    /** This parameter indicates a bitmap of bands supported by the physical
     * layer. For example LSB position 1 in bitmap indicates if BAND_1 is
     * supported or not.
     * The table below lists all the UTRA FDD frequency bands. For more
     * information refer to <tt>[TS 25.101 Section 5.2 & 5.4.4]</tt>
     *
     * <TABLE>
     * <TR><TD><B>Operating Band</B></TD> <TD><B>UL Frequencies</B></TD> <TD><B>DL Frequencies</B></TD><TD><B>UL UARFCNs</B></TD><TD><B>DL UARFCNs</B></TD></TR>
     * <TR><TD>I                    </TD> <TD>1920 - 1980 MHz      </TD> <TD>2110 - 2170 MHz      </TD><TD>9612 - 9888  </TD>    <TD>10562 - 10838</TD></TR>
     * <TR><TD>II                   </TD> <TD>1850 - 1910 MHz      </TD> <TD>1930 - 1990 MHz      </TD><TD>9262 - 9538  </TD>    <TD>9662 - 9938  </TD></TR>
     * <TR><TD>III                  </TD> <TD>1710 - 1785 MHz      </TD> <TD>1805 - 1880 MHz      </TD><TD>937 - 1288   </TD>    <TD>1162 - 1513  </TD></TR>
     * <TR><TD>IV                   </TD> <TD>1710 - 1955 MHz      </TD> <TD>2110 - 2155 MHz      </TD><TD>1312 - 1513  </TD>    <TD>1537 - 1738  </TD></TR>
     * <TR><TD>V                    </TD> <TD>824 - 849  MHz       </TD> <TD>869 - 894 MHz        </TD><TD>4132 - 4233  </TD>    <TD>4357 - 4458  </TD></TR>
     * <TR><TD>VI                   </TD> <TD>830 - 840  MHz       </TD> <TD>875 - 885 MHz        </TD><TD>4162 - 4188  </TD>    <TD>4387 - 4413  </TD></TR>
     * <TR><TD>VII                  </TD> <TD>2500 - 2570 MHz      </TD> <TD>2620 - 2690 MHz      </TD><TD>2012 - 2338  </TD>    <TD>2237 - 2563  </TD></TR>
     * <TR><TD>VIII                 </TD> <TD>880 - 915 MHz        </TD> <TD>925 - 960 MHz        </TD><TD>2712 - 2863  </TD>    <TD>2937 - 3088  </TD></TR>
     * <TR><TD>IX                   </TD> <TD>1749.9 - 1784.9 MHz  </TD> <TD>1844.9 - 1879.9 MHz  </TD><TD>8762 - 8912  </TD>    <TD>9237 - 9387  </TD></TR>
     * </TABLE>
     *
     */
    Int16                               fddBandsSupported;
#endif

#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
    /** This parameter indicates a bitmap of bands supported by the physical
     * layer. For example LSB position 1 in bitmap indicates if TDD128_BAND_A_LOWER is
     * supported or not.
     * The table below lists all the UTRA TDD128 frequency bands. For more
     * information refer to <tt>[TS 25.102]</tt>
     *
     * <TABLE>
     * <TR><TD><B>Operating Band</B></TD> <TD><B>UL=DL Frequencies </B></TD> <TD><B>UL=DL UARFCNs </B></TD></TR>
     * <TR><TD>A_lower              </TD> <TD>1900 - 1920 MHz          </TD> <TD> 9504 - 9596         </TD></TR>
     * <TR><TD>A_upper              </TD> <TD>2010 - 2025 MHz          </TD> <TD>10054 - 10121        </TD></TR>
     * <TR><TD>B_lower              </TD> <TD>1850 - 1910 MHz          </TD> <TD> 9254 - 9546         </TD></TR>
     * <TR><TD>B_upper              </TD> <TD>1930 - 1990 MHz          </TD> <TD> 9654 - 9946         </TD></TR>
     * <TR><TD>C                    </TD> <TD>1910 - 1930 MHz          </TD> <TD> 9554 - 9646         </TD></TR>
     * <TR><TD>19                   </TD> <TD>1880 - 1920 MHz          </TD> <TD> 9404 - 9596         </TD></TR>
     * <TR><TD>23                   </TD> <TD>2300 - 2400 MHz          </TD> <TD>11504 - 11996        </TD></TR>
     * </TABLE>
     *
     */
    Int16                               tdd128BandsSupported;
#endif

    /** This parameter indicates whether L1 is capable of supporting the
     * simultaneous measurement of PCS and DCS band mode cells.
     * \note This msg is only used with a 3rd party PHY FDD.
     *        MEDIATEK PHY does not support it. PS need to be cleaned.
     */
    Boolean                             simultaneousDcsPcs;
}
CphyUeCapabilityConfigCnf;

/** @} */ /* End of SigCphyUeCap group */





/** \defgroup SigCphyCellSelResel Cell Selection and Re-selection Primitives
 * This section defines the primitives exchanged between PHY and RRC during
 * cell selection and re-selection.
 *
 * Both the serving cell BCH decode and neighbour cell BCH decode are never
 * simultaneously requested by RRC to L1.
 *
 * RRC will take into consideration the minimum skip time defined by
 * #UPS_MINIMUM_SKIP_FRAMES when issuing #CphyBchSkipReq and
 * #CphyNcellBchSkipReq primitives to L1. This is because, since BCH is
 * SFN modulo 2 and assuming L1 takes roughly a frame to decode the BCH data
 * and indicate it using #PhyDataInd primitive, it is fairly safe to assume that
 * RRC would not issue #CphyBchSkipReq and #CphyNcellBchSkipReq primitives for
 * a skip time less than what is defined by #UPS_MINIMUM_SKIP_FRAMES.
 *
 * \section SigCphyCellSelReselImpAct Implicit Actions
 * During cell selection process, the following implicit actions should be
 * executed in the physical layer upon the receipt of #CphyCellSelectReq,
 * #CphyFindCellReq or #CphyNextCellReq primitives:
 *     - serving cell measurements stopped
 *     - neighbour cell measurements stopped
 *     - neighbour cell decode stopped
 *     - serving cell decode started
 *
 * @{
 */

/** Find a 3GPP cell request.
 * This primitive is sent from RRC to PHY during the (initial) cell selection,
 * PLMN search and also upon recovery from a radio link failure. This primitive
 * requests the physical layer to synchronize to and camp onto the strongest
 * 3GPP cell for the requested downlink frequency and to receive SYSTEM
 * INFORMATION from that cell.
 *
 * If RRC has the primary scrambling code for the cells on the requested
 * \c uarfcn_dl then these are given to the physical layer to speed up the
 * synchronisation process and the physical layer selection process will
 * report the strongest cell matching one of the specified scrambling codes for FDD
 * and cellParametersIds for TDD128.
 * If the physical layer fails to find a cell matching one of these scrambling
 * codes for FDD or cellParameytersId for TDD128 then it will select the strongest
 * cell it can find.
 *
 * The physical layer then reports the BCH data decoded from the P-CCPCH for
 * this cell.
 *
 * Also see the \ref SigCphyCellSelReselImpAct taken in L1 upon receipt of this
 * primitive.
 *
 */
typedef struct CphyFindCellReqTag
{
    /** Specifies the frequency or frequencies on which to find and camp on the cells.
     *  There is a uarfcn List For TDD128 mode, one uarfcn_dl for FDD mode. */
    UarfcnUnion                         uarfcn;

    /** If IE "numberOfUarfcn" > 1, the following IE "numberOfCells" is invalid.*/
    /** Specifies the number of scrambling codes available for synchronisation
     * on \c uarfcn_dl.

\assoc ARRAY \ref mode.primaryScramblingCode \opt CONDITIONAL
\assoc ARRAY \ref mode.cellParametersID \opt CONDITIONAL */
    Int8                                numberOfCells;

    /** UmtsMode: FDD or TDD128.
\assoc UNION \ref mode
\assoc UNION \ref uarfcn */
    UmtsMode                            umtsMode;

    /** Specifies an array of primary scrambling codes to be used in synchronisation process.
     */
    FindCellArrayUnion                  mode;
    /** Specifies the CFN at which to leave CELL_DCH and start the cell search.
     * If set to other than \c now in CELL_DCH then L1 must leave CELL_DCH
     * at the given \c cfn.
     */
    ActivationTime                      activationTime;

   /** Allows PHY to indicate to higher layers which configuration activation has triggered */
   Int8                                activationTimeIndex;

   /** Tells PHY whether activationTime and activationTimeIndex are valid  */
   Boolean                             waitForActivationTime;

}
CphyFindCellReq;


/** Find a 3GPP cell request in IDLE state.
 * This primitive is sent from RRC to PHY during Idle state to request L1 to
 * detect a cell on a specified frequency.
 *
 * L1 must obey the DRX period and continue to listen to paging on the
 * current serving cell. L1 will not decode the serving cell BCH. This primitive
 * is used during background PLMN search procedures.
 *
 * The parameters are the same as for the #CphyFindCellReq primitive.
 * However, the only relevant parameter is \c uarfcn_dl.
 */
typedef CphyFindCellReq CphyDrxFindCellReq;

#if defined (UPGRADE_3G_TDD128)
/** Abort RSSI scan.
 * This primitive is sent from RRC to L1 during Rssi scan process.
 * After receive it, L1 should immediately response RRC confirm signal #CphyRssiScanCnf,
 * or #CphyDrxRssiScanCnf,Then RRC will send #CphyFindCellReq or #CphyRssiScanReq to L1,
 * This scenario only happen when RRC receive #rrcActReq in PLMN search process or RRC
 * receive #rrcPlmnListReq in ICS process.
 */
typedef EmptySignal CphyAbortRssiScanReq;

/** Abort find cell.
 * This primitive is sent from RRC to L1 during find cell process.
 * After receive it, L1 should immediately response RRC confirm signal #CphyFindCellCnf,
 * or #CphyDrxFindCellCnf,Then RRC will send #CphyFindCellReq or #CphyRssiScanReq to L1,
 * This scenario only happen when RRC receive #rrcActReq in PLMN search process or RRC
 * receive #rrcPlmnListReq in ICS process.
 */
typedef EmptySignal CphyAbortFindCellReq;
#endif

/** Find a cell confirmation.
 * This primitive is sent from PHY to RRC in response to a #CphyFindCellReq
 * primitive.
 * The physical layer indicates whether it successfully camped onto this
 * cell and if it was successful reports the primary scrambling code, RSCP and
 * Ec/No for FDD cells and cellParametersId and P-CPCH RSCP for TDD128 cells.
 */

typedef struct CphyFindCellCnfTag
{
    /** Indicates if the physical layer has detected one or more cells on the
     * requested frequency.

\assoc PRESENT \ref umtsMode
\assoc PRESENT \ref mode */
    Boolean                             cellDetected;

    /** UmtsMode: FDD or TDD128.
\assoc UNION \ref mode */
    UmtsMode                            umtsMode;

    /** Contains the cell identifier and results. */
    ModeCellInfoUnion                   mode;

#if defined (UPGRADE_3G_TDD128)
    /** Specifies uarfcn of current detected cell. */
    UUARFCN                             uarfcn;
#endif
}
CphyFindCellCnf;


/** Find a cell confirmation in IDLE state.
 * This primitive is sent from PHY to RRC in response to a #CphyDrxFindCellReq
 * primitive. The parameters are the same as for the #CphyFindCellCnf primitive.
 */
typedef CphyFindCellCnf CphyDrxFindCellCnf;


/** Next cell request.
 * This primitive is sent from RRC to PHY during initial cell selection
 * and is only sent after a CphyFindCellReq.
 * It requests the physical layer to synchronise to and camp onto the next
 * strongest 3GPP cell on the same frequency.
 * In FDD:
 * If the next strongest peak of the P-SCH scan is for a multipath component of
 * a previously reported cell, the physical layer will autonomously keep trying
 * the next peaks until either a new cell or no cell is detected.
 * In TDD128:
 *    If the next strongest peak of the DwPCH (SYNC_DL sequences) scan is for a
 *    multipath component of a previously reported cell, the physical layer will
 *    autonomously keep trying the next peaks until either a new cell or no cell is detected.

 * The physical layer sends RRC #CphyNextCellCnf primitive indicating whether
 * a cell was detected or not.
 *
 * If a cell is detected the physical layer reports
 * the BCH data decoded from the P-CCPCH for this new cell.
 *
 * Also see the \ref SigCphyCellSelReselImpAct taken in L1 upon receipt of this
 * primitive.
 */
typedef EmptySignal CphyNextCellReq;


/** Next cell confirmation.
 * This primitive is sent from PHY to RRC in response to a #CphyNextCellReq
 * primitive.
 * The physical layer indicates whether it successfully camped onto this cell
 * and if it was successful reports the primary scrambling code, RSCP and
 * Ec/No for FDD cells and cellParametersId and P-CPCH RSCP for TDD128 cells.
 */
typedef struct CphyNextCellCnfTag
{
    /** Value FALSE indicates that there are no more cells on the frequency
     * requested by the last #CphyFindCellReq primitive.

\assoc PRESENT \ref umtsMode
\assoc PRESENT \ref mode */
    Boolean                             cellDetected;

    /** UmtsMode: FDD or TDD128.
\assoc UNION \ref mode */
    UmtsMode                            umtsMode;

    /** Contains the cell identifier and results. */
    ModeCellInfoUnion                   mode;
}
CphyNextCellCnf;


/** Next best neighbour cell request.
 * This primitive is sent from RRC to PHY during Idle state to request L1 to
 * detect the next best neighbour cell. This primitive must only be used after
 * a #CphyDrxFindCellReq. L1 most obey the DRX period and continue to listen to
 * paging on the current serving cell. This primitive is used during background
 * PLMN search procedures and is used to see if there are multiple PLMNs on the
 * same frequency which could happen in border country situations. The physical
 * layer sends RRC #CphyDrxNextCellCnf primitive indicating whether a cell was
 * detected or not.
 */
typedef EmptySignal CphyDrxNextCellReq;


/** Next best neighbour cell confirmation.
 * This primitive is sent from PHY to RRC in response to a #CphyDrxNextCellReq
 * primitive. The parameters are the same as for the #CphyNextCellCnf primitive.
 */
typedef CphyNextCellCnf CphyDrxNextCellCnf;


/** Cell selection request.
 * This primitive is sent from RRC to PHY during cell selection or
 * re-selection.
 * This primitive requests the physical layer to synchronise to and camp onto
 * the specified cell.
 * \note During initial cell selection if RRC fails to find a suitable cell
 * using the #CphyFindCellReq or #CphyNextCellReq it will select an
 * acceptable cell that it previously detected using the #CphyCellSelectReq
 * primitive.
 * The physical layer sends a #CphyCellSelectCnf indicating if the cell was
 * detected. If a cell is detected the physical layer reports the BCH data
 * decoded from the P-CCPCH for this new cell.
 * Also see the \ref SigCphyCellSelReselImpAct taken in L1 upon receipt of this
 * primitive.
 */
typedef struct CphyCellSelectReqTag
{
    /** Specifies the downlink frequency to be used for camping on a cell. */
    UUARFCN                             uarfcn_dl;

    /** UmtsMode: FDD or TDD128.
\assoc UNION \ref mode */
    UmtsMode                            umtsMode;

    /** Specifies the identification of the cell to camp on.
     */
    UmtsCellUnion                       mode;

    /** Specifies the CFN at which to leave CELL_DCH and start the cell search.
     * It is set to other than \c now when leaving CELL_DCH state.
     */
    ActivationTime                      activationTime;

    /** Allows PHY to indicate to higher layers which configuration activation has triggered */
    Int8                                activationTimeIndex;

    /** Tells PHY whether activationTime and activationTimeIndex should be used  */
    Boolean                             waitForActivationTime;

}
CphyCellSelectReq;


/** Cell selection confirmation.
 * This primitive is sent from PHY to RRC in response to a #CphyCellSelectReq
 * primitive and indicates whether it successfully camped onto this cell. If it
 * has successfully camped it reports the primary scrambling code, downlink
 * frequency, RSCP and Ec/No for FDD or cellParametersId, frequency and
 * P-CCPCH RSCP.
 */
typedef struct CphyCellSelectCnfTag
{
    /** A value of FALSE indicates that physical layer failed to detect the
     * requested cell.

\assoc PRESENT \ref umtsMode
\assoc PRESENT \ref mode */
    Boolean                             cellDetected;

    /** Downlink frequency on which the cell was detected. */
    UUARFCN                             uarfcn_dl;

    /** UmtsMode: FDD or TDD128.
\assoc UNION \ref mode */
    UmtsMode                           umtsMode;

    /** Contains the cell identifier and results. */
    ModeCellInfoUnion                   mode;
}
CphyCellSelectCnf;


/** Return to CELL_FACH indication after CCO to GSM failure.
 * This primitive is sent from PHY to RRC to indicate the success or failure
 * of the PHY to return back to CELL_FACH after a failed attempt to perform an
 * Inter-RAT Cell Change to GSM from CELL_FACH state.
 * If successful the \c cellDetected parameter is TRUE, otherwise it is set
 * to FALSE.
 */
typedef CphyCellSelectCnf CphyCellSelectInd;


/** RSSI scan request.
 * This primitive is sent by RRC to PHY during initial cell selection
 * or PLMN search. This primitive is used to measure the RSSI level on either a
 * list or a range of frequencies.
 * note The RSSI scan is per frequency band: i.e the request will not span over multiple bands.
 * The results returned in the #CphyRssiScanCnf primitive are used to prioritise
 * the frequencies RRC tries to camp on using the #CphyFindCellReq primitive.
 */
typedef struct CphyRssiScanReqTag
{
    /** Number of frequencies to be scanned for RSSI measurements.
     * If \c numOfFreqs is greater than 0 then \c freqList is used for the
     * scan otherwise range of frequencies is used for scan.

\assoc ARRAY \ref freqList */
    Int16                               numOfFreqs;

    /** List of frequencies for which RRSI has to be measured.
     *  CPHY_MAX_FREQUENCIES_PER_RSSI_SCAN is 20.
     */
    UUARFCN                             freqList [CPHY_MAX_FREQUENCIES_PER_RSSI_SCAN];

    /** Lower edge of the frequency band. */
    UUARFCN                             minFreqInBand;

    /** Upper edge of the frequency band. */
    UUARFCN                             maxFreqInBand;

    /** The time (immediately or at a certain CFN) at which the physical
     * layer procedure is executed.
     */
    ActivationTime                      activationTime;

    /* Allows PHY to indicate to higher layers which configuration activation has triggered */
    Int8                                activationTimeIndex;

    /* Tells PHY whether activationTime and activationTimeIndex should be used  */
    Boolean                             waitForActivationTime;

#if defined (UPGRADE_3G_TDD128)
    /** Tells PHY only report scan result whose RSSI >= this threshold, the unit is dBm*/
    SignedInt16                         rssiScanThreshold;

    /** Tells PHY report max num of rssi result, When A or F band search. */
    Int16                               topNumOfRankedFreqs;
#endif

}
CphyRssiScanReq;


/** DRX RSSI scan request.
 * This primitive is sent from RRC to the PHY to request RSSI measurements for
 * the given list of frequencies.  L1 must obey the DRX period and continue to
 * receive to paging.  The parameters are the same as for the #CphyRssiScanReq
 * primitive with the physical layer using a #CphyDrxRssiScanCnf primitive to
 * report the measured results.
 *
 * \note This primitive implicitly stops any previously started DRX find cell
 *       operation that may have been started by the #CphyDrxFindCellReq
 *       primitive.
 *
 * \see CphyRssiScanReq
 */
typedef CphyRssiScanReq CphyDrxRssiScanReq;


/** RSSI scan confirmation.
 * This primitive is sent from PHY to RRC in response to a #CphyRssiScanReq
 * primitive.
 */
typedef struct CphyRssiScanCnfTag
{
    /** Total number of frequencies measured during the scan.
\assoc ARRAY \ref utra_RssiReport */
    Int16                               numOfFreqs;

    /** Number of frequencies that have been given a higher ranking by the PHY.
     * This can indicate that results \a utra_RssiReport[0] to
     * \a utra_RssiReport[numOfRankedFreqs - 1] have been identified as being
     * most likely to contain a UMTS carrier.  This is only applicable to band
     * scans where the corresponding \a CphyRssiScanReq.numOfFreqs is zero,
     * and then only if the PHY supports some filtering or carrier
     * identification scheme.  It should be zero if not used or not applicable
     * to the scan method.
     */
    Int16                               numOfRankedFreqs;

    /** An array of frequencies and their RSSI results.
     * This gives the measurement results for a RSSI scan.  For band scans,
     * when \a CphyRssiScanCnfReq.numOfFreqs or \a CphyDrxRssiScanCnf.numOfFreqs
     * is zero, this should be ordered such that the first \a numOfRankedFreqs
     * are those frequencies identified as being most likely to contain a UMTS
     * carrier, with subsequent results ordered by RSSI.  If should always
     * contain \a numOfFreqs valid results.
     */
    UtraRssiReport                      utra_RssiReport [URRC_CSRR_MAX_FREQS];
}
CphyRssiScanCnf;


/** DRX RSSI scan confirmation.
 * This primitive is sent from PHY to RRC in response to a #CphyDrxRssiScanReq
 * primitive.
 * The parameters are the same as for the #CphyRssiScanCnf primitive.
 */
typedef CphyRssiScanCnf CphyDrxRssiScanCnf;

/** @} */ /* End of SigCphyCellSelResel group */





/** \defgroup SigCphyCommChanCtrl Common Channel Control Primitives
 * This section defines the primitives exchanged between PHY and RRC for
 * common control channels.
 *
 * @{
 */

/** Switch on/off BCH request.
 * This primitive is sent from RRC to PHY to switch on/off the BCH channel
 * (i.e.\  the P-CCPCH physical channel). It can also be used to stop skipping.
 * There is no confirmation for this primitive.
 */
typedef struct CphyBchReqTag
{
    /** stopBch indicates if BCH listening has to be stopped.*/
    Boolean                             stopBch;

    /** schedulingCount indicates how many SIBs are in schedulingInfo table.
\assoc ARRAY \ref schedulingInfo */
    Int8                                schedulingCount;

    /** schedulingInfo contains the SIBs scheduling info (number of segments,
     * initial position, repetition for each SIB).*/
    SchedulingInfo                      schedulingInfo[CPHY_SCHEDULING_NUM];
}
CphyBchReq;


/** Neighbour cell BCH decode request.
 * This primitive is sent from RRC to the PHY to request a BCH decode from an
 * intra neighbour cell identified by \c UmtsCellFDD or \c UmtsCellTDD128.
 * After RRC has decoded the necessary data for neighbour cell, it must switch
 * neighbour cell decoding off using this primitive with \c bchOn set to FALSE.
 * Only one neighbour cell decode is allowed to be active at any one time,
 * including DRX find cell operation. A new request to switch BCH decode on
 * whilst it is already active implicitly stops decoding of any previous cell
 * and starts decoding of the new cell.
 *
 * The neighbour cell is indicated by physical
 * layer by #CphyNcellBchCnf message and the BCH data is reported in \c
 * PhyDataInd primitives.
 *
 * \note GSM neighbour cell BCH decode is not handled by this primitive; it
 *       is handled in GSM access layers.
 *
 * \note This signal may also be used to stop BCH reception resulting from a
 *       #CphyDrxFindCellReq or #CphyDrxNextCellReq.
 */

typedef struct CphyNcellBchReqTag
{
    /** stopBch indicates if BCH listening has to be stopped.*/
    Boolean                             stopBch;

    /** schedulingCount indicates how many SIBs are in schedulingInfo table.
\assoc ARRAY \ref schedulingInfo */
    Int8                                schedulingCount;

    /** schedulingInfo contains the SIBs scheduling info (number of segments,
     * initial position, repetition for each SIB).*/
    SchedulingInfo                      schedulingInfo[CPHY_SCHEDULING_NUM];

    /** Downlink frequency on which to decode BCH data.
     * Neighbour cell BCH will only be requested to start for intra-frequency
     * cells, so this will always match the serving cell ARFCN.
     *
     * \deprecated For \a bchOn = TRUE this will always match the
     *             intra-frequency.  When \a bchOn = FALSE, this is unused.
     */
    UUARFCN                             uarfcn_dl;

    /** UmtsMode: FDD or TDD128.
\assoc UNION \ref mode */
    UmtsMode                            umtsMode;

    /** UMTS cell on which BCH data has to be decoded.
     * Cell data for the cell on which BCH reception should start.  If
     * \a bchOn = FALSE, these parameters should be ignored.
     */
    UmtsCellUnion                       mode;
}
CphyNcellBchReq;

/** Neighbour cell BCH decode confirmation.
 * This primitive is sent from PHY to RRC to report the neighbour cell
 * measurement result in response to a #CphyNcellBchReq primitive. \note RRC
 * is responsible for only making this request in valid states.
 * For example, a UE with a single receiver cannot decode BCH for an
 * inter-frequency cell in CELL_FACH state.
 */
typedef struct CphyNcellBchCnfTag
{
    /** A value TRUE indicates that the neighbour cell is detected.

\assoc PRESENT \ref umtsMode
\assoc PRESENT \ref mode */
    Boolean                             cellDetected;

    /** Downlink frequency on which the neighbour cell was decoded. */
    UUARFCN                             uarfcn_dl;

    /** UmtsMode: FDD or TDD128.
\assoc UNION \ref mode */
    UmtsMode                            umtsMode;

    /** Contains the cell identifier and results. */
    ModeCellInfoUnion                   mode;
}
CphyNcellBchCnf;


/** PCH configuration request.
 * This primitive is sent from RRC to PHY to configure paging in IDLE,
 * CELL_PCH and URA_PCH states.
 */
typedef struct CphyPchConfigReqTag
{
    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configIdentifier;

    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configNoOfMessages;

    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configMessageNo;

    /** A value TRUE turns on monitoring of paging, and a value FALSE
     * indicates that the other parameters have no meaning.

\assoc PRESENT \ref umtsMode
\assoc PRESENT \ref mode
\assoc PRESENT \ref pich_PowerOffset
\assoc PRESENT \ref drxCycleLength
\assoc PRESENT \ref pagingOccasion
\assoc PRESENT \ref pagingIndicator
\assoc PRESENT \ref ccTrChIndex */
    Boolean                             pagingOn;

    /** UmtsMode: FDD or TDD128.
\assoc UNION \ref mode */
    UmtsMode                            umtsMode;

    /** Pch configuration union */
    PchConfigModeUnion                  mode;

    /** The power transmitted on the PICH minus power of the Primary CPICH
     * and is in the range -10 to +5 dB.
     * For further information refer to
     * <tt> [3G TS 25.331 section:10.3.6.50] </tt>
     */
    UPICH_PowerOffset                   pich_PowerOffset;

    /** Specifies the number of frames over which the UE must decode one paging
     * indicator.
     */
    Int16                               drxCycleLength;

    /** The \c pagingOccasion defines the first paging occasion of an hyperframe (Paging_Occasion for n=0)
     * A paging occasion every DRX is defined as:
     * Paging_Occasion = {(IMSI div K) mod (DRX cycle length div PBP)} * PBP
     *                       + n * DRX cycle length + Frame Offset
     * where n = 0, 1, 2 , as long as SFN is below its maximum value
     *       Frame offset (For FDD, Frame offset = 0; for TDD128, PICH frame offset values are given in system information)
     *       Refer to [3G TS 25.304 section 8.3] for further details.
     *       K is the number of  SCCPCH which carry a PCH
     *       The DRX cycle length shall be MAX(2k, PBP) frames, where k is an integer and PBP is the Paging Block Periodicity.
     *       PBP is only applicable for TDD128 and is equal to the PICH repetition period that
     *       is broadcast in system information. For FDD, PBP=1.
     * NOTE: in FDD, it is the SFN of the PICH frame where the UE monitors its paging indicator
     * (i.e. the SFN of the PCCPCH frame in which the PICH frame begins).
     * NOTE: in TDD128, it is the beginning of the paging block, which consists of several frames.
     * The value of Paging Occasion is equal to the first frame of the Paging Block.

     */
    Int16                               pagingOccasion;

    /** Indicates which bits of the PICH frame are used to determine a page for
     * this UE in L1.
     */
    Int8                                pagingIndicator;

    /** Identifies the S-CCPCH, TrCH and TFC to be used. */
    Int8                                ccTrChIndex;
}
CphyPchConfigReq;


/** FACH configuration request.
 * This primitive is sent from RRC to PHY to configure FACH when entering
 * CELL_FACH state.
 * This primitive is also used to pass <tt>FACH measurement occasion cycle
 * length coefficient</tt> and C_RNTI, which are used to calculate the FACH
 * measurement occasion. FACH measurement occasion is calculated as below:
 *
 * <em>SFN</em> <small>div</small> <em>N</em> =
 * <em>c_RNTI</em> <small>mod</small> <em>2<sup><small>k</small></sup></em> +
 * <em>n</em>*<em>2<sup><small>k</small></sup></em>
 *
 * where:
 *     - k is the FACH Measurement occasion cycle length coefficient signalled
 * in SIB11/SIB12
 *     - N is the TTI (in number of 10ms frames) of the FACH having the largest
 * TTI on the S-CCPCH monitored by UE.
 * For more information, refer to <tt>[3G TS 25.331 section 8.5.11]</tt>.
 */
typedef struct CphyFachConfigReqTag
{
    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configIdentifier;

    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configNoOfMessages;

    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configMessageNo;

    /** Identifies the S-CCPCH, TrCH and TFC to be used. */
    Int8                                ccTrChIndex;

    /** The time (immediately or at a certain CFN) at which the physical
     * layer procedure is executed.
     */
    ActivationTime                      activationTime;

    /** Allows PHY to indicate to higher layers which configuration activation has triggered */
    Int8                                activationTimeIndex;

    /** Tells PHY whether activationTime and activationTimeIndex should be used  */
    Boolean                             waitForActivationTime;

    /** Indicates if inter-frequency or inter-system measurements can be
     * performed or not.
     * This will be set to FALSE when the FACH measurement occasion
     * information element is not present in SIB11/12 or when the C-RNTI
     * is not valid.

\assoc PRESENT \ref fachMeasOccasionCoeff
\assoc PRESENT \ref c_RNTI */
    Boolean                             fachMeasOccasionPresent;

    /** FACH measurement occasion coefficient and c_RNTI are used to
     * define FACH measurement occasion in CELL_FACH state.
     */
    Int8                                fachMeasOccasionCoeff;

    /** Cell Radio Network Type Identifier.
     * This is a unique identifier for a UE in a cell in CELL_FACH state.
     */
    UC_RNTI                             c_RNTI;
}
CphyFachConfigReq;


/** RACH configuration request.
 * This primitive is sent from RRC to PHY to configure the static RACH
 * parameters.
 * The UMAC selects the Access Service Class (ASC) and Transport Format and
 * triggers the RACH sequence using the PhyAccessReq primitive.
 */
typedef struct CphyRachConfigReqTag
{
    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configIdentifier;

    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configNoOfMessages;

    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configMessageNo;

    /** The time (immediately or at a certain CFN) at which the physical
     * layer procedure is executed.
     */
    ActivationTime                      activationTime;

    /** Allows PHY to indicate to higher layers which configuration activation has triggered */
    Int8                                activationTimeIndex;

    /** Tells PHY whether activationTime and activationTimeIndex should be used  */
    Boolean                             waitForActivationTime;

    /** UmtsMode: FDD or TDD128.
\assoc UNION \ref mode */
    UmtsMode                            umtsMode;

    /** Rach configuration union */
    RachConfigModeUnion                 mode;

    /** Indicates the maximum uplink transmit power allowed on RACH and it ranges
     * from -50 to +33 dBm. <tt> [3G TS 25.331 10.3.6.39] </tt>.
     */
    UMaxAllowedUL_TX_Power              maxAllowedUL_TX_Power;
}
CphyRachConfigReq;


/** Cell broadcast and multicast configuration request.
 * This primitive is sent from RRC to PHY to configure the broadcast and
 * multicast decode in the physical layer.
 * The following diagram illustrates the use of the parameters used in this
 * primitive.
 *
 * \image html prd3gcbs.png "Fig: Example of CBS parameters"
 * \image latex prd3gcbs.eps "Example of CBS parameters" width=12cm
 */
typedef struct CphyCbsConfigReqTag
{
    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configIdentifier;

    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configNoOfMessages;

    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configMessageNo;

    /** Used to turn the cell broadcast service on or off.
     * If set to FALSE then the parameters \c cbs_DRX_Level1Information,
     * \c cbs_DRX_Level2Information and \c ccTrChIndex are not relevant.

\assoc PRESENT \ref cbs_DRX_Level1Information
\assoc PRESENT \ref cbs_DRX_Level2Information
\assoc PRESENT \ref ccTrChIndex */
    Boolean                             cbsOn;

    /** Indicates the Level 1 (higher-level) scheduling information, which mainly
     * contains the \c ctch_AllocationPeriod and the \c cbs_FrameOffset, which
     * are used by the physical layer to calculate the S-CCPCH frames it must
     * decode to receive the CTCH logical channel for broadcast service data.
     * This information is broadcast in system information.
     * The CTCH occasion is calculated as follows:
     *     CTCH occasion = (\c cbs_FrameOffset + m * \c ctch_AllocationPeriod)
     *     where m = 0, 1, 2 , (\c cbs_FrameOffset + m * \c ctch_AllocationPeriod) <= MaxSFN
     */
    UCBS_DRX_Level1Information          cbs_DRX_Level1Information;

    /** Indicates the Level 2 scheduling information, which is also referred to
     * as <tt> inband scheduling message </tt>, and it mainly contains the \c
     * referenceSfn, the \c numberOfReceptionPeriods and the \c
     * receptionPeriodOffset. The referenceSfn indicates the SFN of when the
     * last BMC schedule message was received and is used as a reference point
     * for the receptionPeriodOffset.
     * \note If the TTI used was > 10ms then the SFN corresponds to the first
     * frame number that this message was received in. The \c
     * receptionPeriodOffset defines an array of length \c
     * numberOfReceptionPeriods that defines the CTCH block sets that must be
     * decoded by physical layer. <tt> [3G TS 25.324] </tt> defines this offset
     * to be in the range 1 to 256. For compactness this is stored in an Int8
     * in the range 0 to 255. Therefore the CTCH occasion offset is: \c
     * receptionPeriodOffset + 1. If the \c numberOfReceptionPeriods is 0 then
     * all CTCH periods must be reported by the physical layer.
     */
    UCBS_DRX_Level2Information          cbs_DRX_Level2Information;

    /** Indicates the S-CCPCH, TrCH and TFC to be used. */
    Int8                                ccTrChIndex;
}
CphyCbsConfigReq;

/** S-CCPCH physical channel configuration request.
 * This primitive is sent from RRC to PHY to configure the S-CCPCH physical
 * channel.
 */
typedef struct CphySccpchConfigReqTag
{
    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configIdentifier;

    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configNoOfMessages;

    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configMessageNo;

    /** The time (immediately or at a certain CFN) at which the physical
     * layer procedure is executed.
     */
    ActivationTime                      activationTime;

    /** Allows PHY to indicate to higher layers which configuration activation has triggered */
    Int8                                activationTimeIndex;

    /** Tells PHY whether activationTime and activationTimeIndex should be used  */
    Boolean                             waitForActivationTime;

    /** Indicates each S-CCPCH, and is allocated by RRC and used to define which
     * S-CCPCH the FACH and/or PCH is mapped to.
     * This also maps to the TrCH and TFC configuration for the FACH and PCH.
     */
    Int8                                ccTrChIndex;

    /** UmtsMode: FDD or TDD128.
\assoc UNION \ref mode */
    UmtsMode                           umtsMode;

    /** mode is an union that contains the S-CCPCH configuration for FDD or TDD128,
     * depending on umtsMode.
     */
    SecondaryCCPCHConfigModeUnion      mode;
}
CphySccpchConfigReq;

#if defined (UPGRADE_3G_MBMS)
#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/** MBMS MCCH configuration request.
 * This primitive is sent from RRC to PHY to configure the MBMS MCCH channel.
 * The physical layer sends a CphyCcTrChConfigCnf signal to indicate success or
 * otherwise.  If success, L1 will begin reading MCCH immediately.
 */
typedef struct CphyMcchConfigReqTag
{
    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configIdentifier;

    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configNoOfMessages;

    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configMessageNo;

    /** The time (immediately or at a certain SFN) at which the physical
     * layer procedure is executed.
     */
    ActivationTimeForMbms               activationTimeForMbms;

    /** Allows PHY to indicate to higher layers which configuration activation has triggered */
    Int8                                activationTimeIndex;

    /** Tells PHY whether activationTime and activationTimeIndex should be used  */
    Boolean                             waitForActivationTime;

    /** Indicates each S-CCPCH, and is allocated by RRC and used to define which
     * S-CCPCH the FACH and/or PCH is mapped to.
     * This also maps to the TrCH and TFC configuration for the FACH and PCH.
     */
    Int8                                ccTrChIndex;

    /** Identity of the transport channel. */
    Int8                                transportChannelIdentity;

    /** Represents r, the repetition period coefficient. The number of
     * repetitions per modification period equals 2^r while the actual
     * repetition period, in number of frames, equals MP DIV 2^r.
     */
    Int8                                repetitionPeriodCoefficient;

    /** Represents m, the modification period coefficient. The actual
     * modification period(MP), in number of frames, equals 2^m.
     */
    Int8                                modificationPeriodCoefficient;
}
CphyMcchConfigReq;

/** MBMS pause MCCH reading request.
 * This primitive is sent from RRC to PHY to stop MCCH reading.
 * URRC will send this signal to the physical layer in a certain modification
 * period at the time that:
 * - URRC has gotten the whole MCCH messages;
 * - It is not necessary to continue reading (e.g. service info that UE is
 * interesting in is not changed in current modification period).
 * \note If L1 detects a TTI in which no MCCH information is transmitted
 * (CRC OK, tbnum or tbsize is 0), L1 will stop receiving MCCH automatically.
 * And start receiving MCCH in next repeat period or modification period
 * until RRC sends CphyMcchPauseReq, in order to avoid the situation that
 * RRC has not got enough MCCH information in this period.
 */
typedef struct CphyMcchPauseReqTag
{
    /** Indicates whether or not the UE should continue reading MCCH in the
     * next modification period.
     * Please refer to <tt> [3G TS 25.331 10.2.16j] </tt>
     */
    Boolean                             continueMcchReading;
}
CphyMcchPauseReq;

/** MBMS notification Indicator Channel configuration request.
 * This primitive signal is sent from URRC to L1 to configure the channel needed
 * to receive MICH.  There is no primitive signal to release MICH from URRC to
 * PHY.  The physical layer will release MICH internally at the time of the
 * relevant MCCH being released.
 */
typedef struct CphyMichConfigReqTag
{
    /** This is the power transmitted on the MICH minus power of the Primary
     * CPICH in FDD and Primary CCPCH Tx Power in TDD.
     * Refer to <tt> [3G TS 25.331 section:10.3.9a.15] </tt>
     */
    UMBMS_MICHPowerOffset               michPowerOffset;

    /** Includes information about MICH configuration.
     * Refer to <tt> [3G TS 25.331 section:10.3.9a.14] </tt>
     */
    UMBMS_MICHConfigurationInfo_r6_tdd128    michConfigInfo;

    /** Indicates the number of NI to be monitored.
\assoc ARRAY \ref niList */
    Int8                                niNum;

    /** Indicates the list of the NI to be monitored.
     * RRC calculates the niList according to TMGI list.
     * NI = (TMGI + TMGI/G) mod G, where G = 2^16.
     */
    Int16                               niList[CPHY_MAX_NI_NUM_FOR_MBMS];
}
CphyMichConfigReq;

/** MBMS MSCH configuration request.
 * This primitive is sent from RRC to L1 to configure the MSCH to schedule
 * the reception of MTCH.
 */
typedef struct CphyMschConfigReqTag
{
    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configIdentifier;

    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configNoOfMessages;

    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configMessageNo;

    /** The time (immediately or at a certain SFN) at which the physical
     * layer procedure is executed.
     */
    ActivationTimeForMbms               activationTimeForMbms;

    /** Allows PHY to indicate to higher layers which configuration activation has triggered */
    Int8                                activationTimeIndex;

    /** Tells PHY whether activationTime and activationTimeIndex should be used  */
    Boolean                             waitForActivationTime;

    /** Indicates each S-CCPCH, and is allocated by RRC and used to define which
     * S-CCPCH the FACH and/or PCH is mapped to.
     * This also maps to the TrCH and TFC configuration for the FACH and PCH.
     */
    Int8                                ccTrChIndex;

    /** Identity of the transport channel. */
    Int8                                transportChannelIdentity;

    /** Indicates the period, in number of frames, between MBMS scheduling
     * message (MSCH_REP)
     */
    Int16                               schedulingPeriod;

    /** Indicates the position of MBMS scheduling messages relative to timing
     * of the corresponding cell (MSCH_OFF).
     */
    Int16                               schedulingOffset;
}
CphyMschConfigReq;

/** MBMS pause MSCH reading request.
 * This primitive signal is sent from URRC to PHY indicating:
 * Stop reading MSCH in the pointed CCTrCH now in current scheduling period
 * and restart at the next occurrence of the MSCH message concerned with
 * a MBMS service.
 * URRC will send this signal to the physical layer in a certain scheduling
 * period when URRC has gotten the enough MSCH messages;
 * For multiple MBMS CCTrCHs with MSCH, this signal will be send separately for
 * each CCTrCH.
 * \note If L1 detects a TTI in which no MSCH information is transmitted
 * (CRC OK, tbnum or tbsize is 0), L1 will stop receiving MSCH automatically.
 */
typedef struct CphyMschPauseReqTag
{
    /** Indicates each S-CCPCH, and is allocated by RRC and used to define which
     * S-CCPCH the FACH and/or PCH is mapped to.
     * This also maps to the TrCH and TFC configuration for the FACH and PCH.
     */
    Int8                                ccTrChIndex;
}
CphyMschPauseReq;

/** MBMS MTCH configuration request.
 * This primitive signal is sent from URRC to PHY to configure MTCH.
 */
typedef struct CphyMtchConfigReqTag
{
    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configIdentifier;

    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configNoOfMessages;

    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configMessageNo;

    /** The time (immediately or at a certain SFN) at which the physical
     * layer procedure is executed.
     */
    ActivationTimeForMbms               activationTimeForMbms;

    /** Allows PHY to indicate to higher layers which configuration activation has triggered */
    Int8                                activationTimeIndex;

    /** Tells PHY whether activationTime and activationTimeIndex should be used  */
    Boolean                             waitForActivationTime;

    /** Indicates each S-CCPCH, and is allocated by RRC and used to define which
     * S-CCPCH the FACH and/or PCH is mapped to.
     * This also maps to the TrCH and TFC configuration for the FACH and PCH.
     */
    Int8                                ccTrChIndex;

    /** The schedulingType defines which kind of schedule method is applied
     * for MTCH. Only one of the three kinds of method could exist at the
     * same time.
     */
    MtchSchedulingType                  SchedulingType;

    /** Defines how many transport channels in the SCCPCH (with index equal
     * to ccTrChIndex) are used to carry MTCHs to be received.

\assoc ARRAY \ref mtchSchedulingInfoEachTrch */
    Int8                                trchNumForMtch;

    /** Defines MTCH scheduling information for each transport channel. */
    MtchSchedulingInfoEachTrCh          mtchSchedulingInfoEachTrch
                                        [CPHY_MAX_TRCH_NUM_FOR_MTCH_EACH_SCCPCH];
}
CphyMtchConfigReq;

/** S-CCPCH physical channel for MBMS configuration request.
 * This primitive is sent from RRC to PHY to configure the S-CCPCH physical
 * channel used for TDD128 MBMS.
 * \note The TFCS and TrCh configurations are sent by existing signals
 * (CphyDlTrchConfigReq and CphyDlTfcConfigReq).
 */
typedef struct CphySccpchConfigForMbmsReqTag
{
    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configIdentifier;

    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configNoOfMessages;

    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configMessageNo;

    /** The time (immediately or at a certain SFN) at which the physical
     * layer procedure is executed.
     */
    ActivationTimeForMbms               activationTimeForMbms;

    /** Allows PHY to indicate to higher layers which configuration activation has triggered */
    Int8                                activationTimeIndex;

    /** Tells PHY whether activationTime and activationTimeIndex should be used  */
    Boolean                             waitForActivationTime;

    /** Indicates each S-CCPCH, and is allocated by RRC and used to define which
     * S-CCPCH the FACH and/or PCH is mapped to.
     * This also maps to the TrCH and TFC configuration for the FACH and PCH.
     */
    Int8                                ccTrChIndex;

    /** Indicates the downlink frequency of the S-CCPCH.
     * When MCCH is carried in this S-CCPCH, uarfcn_dl is defined as the primary
     * frequency.
     * When MTCH and MSCH are received in this S-CCPCH, uarfcn_dl is defined
     * according to UTN information.
     */
    UUARFCN                             uarfcn_dl;

    /** Indicates the cellParametersID of the S-CCPCH.
     * When MCCH is carried in this S-CCPCH, cellParametersID is defined as
     * cellParametersID of the serving cell.
     * When MTCH and MSCH are received in this S-CCPCH, cellParametersID is
     * defined according to UTN information.
     */
    UCellParametersID                   cellParametersID;

    /** Indicates the physical channel information for S-CCPCH carried the MBMS
     */
    USecondaryCCPCHInfo_MBMS_r7_tdd128  sccpchConfigForMbms;
}
CphySccpchConfigForMbmsReq;

#endif /* UPGRADE_3G_TDD128  || UPGRADE_3G_DOUBLE_DUAL_MODE */
#endif /* UPGRADE_3G_MBMS */


/** @} */ /* End of SigCphyCommChanCtrl group */





/** \defgroup SigCphyTrch Transport Channel Primitives
 * This section defines the primitives exchanged between PHY and RRC for
 * establishing, re-configuring and releasing transport channels.
 *
 * @{
 */

/** Uplink transport channel configuration request.
 * This primitive is sent from RRC to PHY to configure the uplink transport
 * channels.
 * For each configuration set, this primitive is used to describe all uplink
 * transport channels present, even if they existed in the previous configuration.
 * Therefore, the absence of a transport channel in a given primitive set with
 * respect to a previous configuration indicates a transport channel has been
 * released.
 */
typedef struct CphyUlTrchConfigReqTag
{
    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configIdentifier;

    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configNoOfMessages;

    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configMessageNo;

    /** The time (immediately or at a certain CFN) at which the physical
     * layer procedure is executed.
     */
    ActivationTime                      activationTime;

    /** Allows PHY to indicate to higher layers which configuration activation has triggered */
    Int8                                activationTimeIndex;

    /** Tells PHY whether activationTime and activationTimeIndex should be used  */
    Boolean                             waitForActivationTime;

    /** Indicates the corresponding physical channel and TFC. */
    Int8                                ccTrChIndex;

    /** Type of transport channel. */
    UTransportChannelType               transportChannelType;

    /** Identity of the transport channel. */
    Int8                                transportChannelIdentity;

    /** Semi-static data of a transport channel.
     * It consists of:
     *     - TTI
     *     - Channel Coding
     *     - Rate Matching
     *     - CRC Size.
     */
    SemiStaticTfData                    semiStaticTfData;

    /** Dynamic data of a transport channel.
     * It consists of:
     *     - Transport Block Size
     *     - Number of Transport Blocks
     * The dynamic data of a transport channel can change every TTI.
     */
    DynamicTfData                       dynamicTfData;

#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
    /** Identifies individually a CCTrCH of dedicated type in TDD128. */
    Int8                                tfcsId;
#endif
}
CphyUlTrchConfigReq;


/** Downlink transport channel configuration request.
 * This primitive is sent from RRC to PHY to configure the downlink transport
 * channels.
 * For each configuration set, this primitive is used to describe all downlink
 * transport channels present, even if they existed in the previous configuration.
 * Therefore, the absence of a transport channel in a given primitive set with
 * respect to a previous configuration indicates a transport channel has been
 * released.
 */
typedef struct CphyDlTrchConfigReqTag
{
    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configIdentifier;

    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configNoOfMessages;

    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configMessageNo;

    /** The time (immediately or at a certain CFN) at which the physical
     * layer procedure is executed.
     */
    ActivationTime                      activationTime;

    /** Allows PHY to indicate to higher layers which configuration activation has triggered */
    Int8                                activationTimeIndex;

    /** Tells PHY whether activationTime and activationTimeIndex should be used  */
    Boolean                             waitForActivationTime;

    /** Indicates the corresponding physical channel and TFC. */
    Int8                                ccTrChIndex;

    /** Type of transport channel. */
    UTransportChannelType               transportChannelType;

    /** Identity of the transport channel.
     * Please refer to <tt> [3G TS 25.331 10.3.5.18] </tt>.
     */
    Int8                                transportChannelIdentity;

    /** Semi-static data of a transport channel.
     * It consists of:
     *     - TTI
     *     - Channel Coding
     *     - Rate Matching
     *     - CRC Size.
     */
    SemiStaticTfData                    semiStaticTfData;

    /** Dynamic data of a transport channel.
     * It consists of:
     *     - Transport Block Size
     *     - Number of Transport Blocks
     * The dynamic data of a transport channel can change every TTI.
     */
    DynamicTfData                       dynamicTfData;

    /** Represents Log10(Transport channel BLER quality target) and is in the
     * range 0 to -63. This parameter only applies to DCH transport channels. It
     * is up to L1 to calculate BLER.
     * Please refer to <tt> [3G TS 25.331 10.3.5.10] </tt>.
     */
    UBLER_QualityValue                  blerQualityValue;

#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
    /** Identifies individually a CCTrCH of dedicated type in TDD128. */
    Int8                                tfcsId;

    /** Indicates if this Trch carry the SRB. It also allows L1 to know which
     * CCTrCH (tfcs_id) convey the signalling Radio Bearer.
     */
     Boolean                            srbPresence;
#endif
}
CphyDlTrchConfigReq;


/** Downlink TFC configuration request.
 * This primitive is sent from RRC to PHY to configure the downlink Transport
 * Format Combination (TFC) database in the physical layer.
 * \note This primitive is sent for each downlink combined composite transport
 * channel (CcTrCh) and defines the valid combinations of transport formats
 * allowed on this channel.
 */
typedef struct CphyDlTfcConfigReqTag
{
    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configIdentifier;

    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configNoOfMessages;

    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configMessageNo;

    /** The time (immediately or at a certain CFN) at which the physical
     * layer procedure is executed.
     */
    ActivationTime                      activationTime;

    /** Allows PHY to indicate to higher layers which configuration activation has triggered */
    Int8                                activationTimeIndex;

    /** Tells PHY whether activationTime and activationTimeIndex should be used  */
    Boolean                             waitForActivationTime;

    /** Indicates the corresponding physical channel and TFC. */
    Int8                                ccTrChIndex;

    /** Indicates the TFCS to be configured on the downlink. */
    ExplicitTfcsConfig                  explicitTfcsConfig;

#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
    /** Identifies individually a CCTrCH of dedicated type in TDD128. */
    Int8                                tfcsId;
#endif
}
CphyDlTfcConfigReq;


/** Transport channel and physical channel configuration confirmation.
 * This primitive is sent from PHY to RRC to confirm transport channel and
 * physical channel configuration primitives.
 */
typedef struct CphyCcTrChConfigCnfTag
{
    /** Indicates the corresponding group of Req primitives. */
    Int8                                configIdentifier;

    /** This is set to \c configNoOfMessages from the corresponding group of
     * Req primitives if the configuration is successful, otherwise \c
     * configMessageNo indicates the first message in error from the
     * corresponding group of REQ primitives.
     */
    Int8                                configMessageNo;

    /** Indicates PHY_ERR_NO_ERROR if the configuration is successful otherwise
     * it will set to the appropriate enum defining the type of error.
     */
    UPhysicalLayerErrorCode             errorCode;

} CphyCcTrChConfigCnf;


/** Common Transport channel and Common physical channel release request.
 * This primitive is sent from RRC to PHY to release all transport channel and
 * physical channels associated with the parameter \c ccTrChIndex.
 */
typedef struct CphyCcTrChReleaseReqTag
{
    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configIdentifier;

    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configNoOfMessages;

    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configMessageNo;

    /** All transport channels and physical channels associated with this
     * parameter have to be released.
     */
    Int8                                ccTrChIndex;
}
CphyCcTrChReleaseReq;


/** Include CRC bits in transport blocks request.
 * This primitive is sent from RRC to PHY to inform L1 that it should also
 * include CRC bits in received transport blocks. Following reception of this
 * primitive, the bitLength indicated by layer 1 on these transport channels
 * is the transport block size in bits plus the CRC bit length. This primitive
 * is used only in loopback mode.
 */
typedef struct CphyDlTrchCrcReqTag
{
    /** Indicates how many transport channels for which CRC bits should be
     * included in their downlink transport blocks. If \c numberTrChs is zero
     * then it is a request to stop L1 including CRC bits in the received
     * transport blocks.

\assoc ARRAY \ref transportChannelIdentity */
    Int8                                numberTrChs;

    /** Indicates an array of transport channel identities for which CRC bits
     * should be included in their downlink transport blocks.
     */
    Int8                                transportChannelIdentity[UPS_MAX_LOOPBACK_BEARERS];
}
CphyDlTrchCrcReq;


/** Include CRC bits in transport blocks confirmation.
 * This primitive is sent from PHY to RRC to confirm a #CphyDlTrchCrcReq
 * primitive.
 */
typedef EmptySignal CphyDlTrchCrcCnf;


#if defined (UPGRADE_3G_HSDPA)

/** HS-DSCH transport channel configuration request.
 * This primitive is sent from RRC to PHY to configure the HS-DSCH transport
 * channel. It specifies the size and number of physical layer HARQ buffers.
 * The HS-DSCH is released if a primitive set is received from RRC that does
 * not include the #CphyDlHsDschTrChConfigReq primitive.
 */
typedef struct CphyDlHsDschTrChConfigReqTag
{
    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configIdentifier;

    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configNoOfMessages;

    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configMessageNo;

    /** The time (immediately or at a certain CFN) at which the physical
     * layer procedure is executed.
     */
    ActivationTime                      activationTime;

    /* Allows PHY to indicate to higher layers which configuration activation has triggered */
    Int8                                activationTimeIndex;

    /* Tells PHY whether activationTime and activationTimeIndex should be used  */
    Boolean                             waitForActivationTime;

    /** Indicates to the physical layer whether the HARQ buffers should be
     * reset during reconfiguration. \note This applies to all HARQ buffers.
     */
    Boolean                             macHsReset;

    /** Indicates the number of HARQ processes. If only HS-DSCH reset is sent
     * by the network this parameter is set to zero.

\assoc ARRAY \ref explicitHarqInfo */
    Int8                                numberOfProcesses;

    /** Indicates an array of HARQ processes to be configured for HS-DSCH. */
    Int32                               explicitHarqInfo[maxHProcesses];
}
CphyDlHsDschTrChConfigReq;


/** HS-DSCH transport channel release request.
 * This primitive is sent from RRC to PHY to release the HS-DSCH transport
 * channel and all associated physical channels (HS-SCCHs and HS-DPCCH in FDD
 * and HS-SICH in TDD128).
 */
typedef struct CphyHsDschReleaseReqTag
{
    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configIdentifier;

    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configNoOfMessages;

    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configMessageNo;

    /** The time (immediately or at a certain CFN) at which the physical
     * layer procedure is executed.
     */
    ActivationTime                      activationTime;

    /* Allows PHY to indicate to higher layers which configuration activation has triggered */
    Int8                                activationTimeIndex;

    /* Tells PHY whether activationTime and activationTimeIndex should be used  */
    Boolean                             waitForActivationTime;

}
CphyHsDschReleaseReq;

#endif /* UPGRADE_3G_HSDPA */

#if defined (UPGRADE_3G_HSUPA)
/** CphyEdchReleaseReq removes E-DCH configuration and E-RNTI from the Layer 1.
 *      Reception of HICH, AGCH, and RGCH should be stopped.
 *
 */
typedef struct CphyEdchReleaseReqTag
{
    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configIdentifier;

    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configNoOfMessages;

    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configMessageNo;
    /** activationTime defines the frame number/time at which the
     * operation/changes caused by the related message shall take effect.
     * Values between 0 and 255 indicate the absolute value of CFN
     * (Connection Frame Number) of that frame number/time.
     */
    ActivationTime                      activationTime;

    /** Allows PHY to indicate to higher layers which configuration activation has triggered */
    Int8                                activationTimeIndex;

    /** Tells PHY whether activationTime and activationTimeIndex should be used  */
    Boolean                             waitForActivationTime;

}
CphyEdchReleaseReq;
#endif /* UPGRADE_3G_HSUPA */

/** @} */ /* End of SigCphyTrch group */





/** \defgroup SigCphyRadioLink Radio Link Primitives
 * This section defines the primitives exchanged between PHY and RRC to
 * configure the dedicated physical channels in RRC CELL_DCH state.
 *
 * In soft handover, L1 receives the delta configuration whereas in the case of
 * hard handover L1 receives a complete configuration.
 *
 * @{
 */

/** Setup common information for radio links request.
 * This primitive is sent from RRC to PHY to set-up the common information
 * for all dedicated radio links.
 * When entering CELL_DCH state as a result of a #CphyRlCommonSetupReq the
 * following actions must be executed in the physical layer:
 *
 *     - serving cell measurements stopped
 *     - neighbour cell measurement stopped
 *     - neighbour cell decode stopped
 *     - serving cell decode stopped
 */
typedef struct CphyRlCommonSetupReqTag
{
    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configIdentifier;

    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configNoOfMessages;

    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configMessageNo;

    /** The time (immediately or at a certain CFN) at which the physical
     * layer procedure is executed.
     */
    ActivationTime                      activationTime;

    /* Allows PHY to indicate to higher layers which configuration activation has triggered */
    Int8                                activationTimeIndex;

    /* Tells PHY whether activationTime and activationTimeIndex should be used  */
    Boolean                             waitForActivationTime;

    /** Indicates the corresponding physical channel and TFC type. */
    Int8                                ccTrChIndex;

    /** Indicates the reason for this primitive. The setting affects the
     * meaning of the associated #CphyRlSetupReq, and #CphyRlReleaseReq
     * primitives respectively.
     * If \c radioLinkAction is set to CPHY_RADIO_LINK_HO_MAINTAIN the physical
     * layer must maintain the CFN during a hard handover.
     * If \c radioLinkAction is set to CPHY_RADIO_LINK_ESTABLISH or
     * CPHY_RADIO_LINK_HO_INITITIALISE the physical layer must calculate the
     * new CFN.
     * Note that the defaultDPCH_OffsetValue is:
     *    - For FDD: in the range 0 to 599 and must be multiplied by 512 to get the offset in chips.
     *    - For TDD: in the range 0 to 7.
     *
     * In FDD, all values of \c radioLinkAction other than CPHY_RADIO_LINK_RECONFIG_WITHOUT_SYNC_A
     * and CPHY_RADIO_LINK_ACTIVE_SET_UPDATE require the physical layer to perform
     * synchronisation procedure A as defined in <tt> [3G TS 25.214]. </tt> which will lead to the
     * sending of a CphySyncInd/CphyOutOfSyncInd depending on the success of such synchronisation.
     *
     * FFS: In TDD, all values of \c radioLinkAction other than CPHY_RADIO_LINK_RECONFIG and
     * CPHY_RADIO_LINK_BATON_HO require the physical layer to perform synchronisation procedure
     * phase 1: Initial establishment?as defined in <tt> [3G TS 25.224]. </tt> which will lead to
     * the sending of a CphySyncInd/CphyOutOfSyncInd depending on the success of such synchronisation.
     */
    URadioLinkAction                    radioLinkAction;

    /** This parameter defines the DL frequency for FDD or the UL/DL frequency for TDD128
     * on which the radio link is to be setup .
     */
    UUARFCN                             uarfcn_dl;

    /** UmtsMode: FDD or TDD128.
\assoc UNION \ref mode */
    UmtsMode                            umtsMode;

    /** RlCommonSetup Configuration. */
    RlCommonSetupConfigModeUnion        mode;

    /** Indicates the timers and constants needed by the physical layer to
     * determine DCH synchronisation.
     */
    SyncParameters                      syncParameters;

    /** Indicates the maximum uplink transmit power allowed on DCH and it reanges
     * from -50 to +33dBm). <tt> [3G TS 25.331 section 10.3.6.39] </tt>.
     */
    UMaxAllowedUL_TX_Power              maxAllowedUL_TX_Power;
}
CphyRlCommonSetupReq;


/** Setup dedicated information for radio links request.
 * This primitive is sent from RRC to PHY to establish a dedicated Radio link
 * to a node B at the specified activation time.
 * This primitive contains the radio link specific data and must be used with
 * the common information provided in the #CphyRlCommonSetupReq primitive.
 */
typedef struct CphyRlSetupReqTag
{
    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configIdentifier;

    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configNoOfMessages;

    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configMessageNo;

    /** The time (immediately or at a certain CFN) at which the radio link
     * has to be established.
     */
    ActivationTime                      activationTime;

    /* Allows PHY to indicate to higher layers which configuration activation has triggered */
    Int8                                activationTimeIndex;

    /* Tells PHY whether activationTime and activationTimeIndex should be used  */
    Boolean                             waitForActivationTime;

    /** Indicates the corresponding TrCH and TFC. */
    Int8                                ccTrChIndex;

    /** UmtsMode: FDD or TDD128.
\assoc UNION \ref mode */
    UmtsMode                            umtsMode;

    /** RlSetup Configuration including all DL CCTrCHs description. */
    RlSetupConfigModeUnion              mode;

}
CphyRlSetupReq;

#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/** Radio link release request.
 * This primitive is sent from RRC to PHY for release of a dedicated radio
 * link to a node B at the specified activation time. This primitive must
 * be used with the common information provided in the #CphyRlCommonSetupReq
 * primitive. This primitive is only used during soft-handover. The \c
 * radioLinkAction parameter in #CphyRlCommonSetupReq primitive must be set to
 * \c CPHY_RADIO_LINK_ACTIVE_SET_UPDATE.
 */
typedef struct CphyRlReleaseReqTag
{
    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configIdentifier;

    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configNoOfMessages;

    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configMessageNo;

    /** The time (immediately or at a certain CFN) at which the radio link
     * has to be released.
     */
    ActivationTime                      activationTime;

    /* Allows PHY to indicate to higher layers which configuration activation has triggered */
    Int8                                activationTimeIndex;

    /* Tells PHY whether activationTime and activationTimeIndex should be used  */
    Boolean                             waitForActivationTime;


    /** Indicates the corresponding TrCH and TFC. */
    Int8                                ccTrChIndex;

    /** Indicates the radio link uniquely. */
    UPrimaryScramblingCode              primaryScramblingCode;
}
CphyRlReleaseReq;
#endif


#if defined(UPGRADE_3G_HSDPA)
/** HS-SCCH physical channel configuration request.
 * This primitive is sent from RRC to PHY to configure the HS-SCCH physical
 * channel on the downlink. UE is provided with a HS-SCCH set for a two-step
 * signalling approach is used for indicating which UE has been scheduled and
 * for signalling the necessary information required for the UE to decode the
 * HS-PDSCHs.
 */
typedef struct CphyHsScchConfigReqTag
{
    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configIdentifier;

    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configNoOfMessages;

    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configMessageNo;

    /** The time (immediately or at a certain CFN) at which the physical
     * layer procedure is executed.
     */
    ActivationTime                      activationTime;

    /* Allows PHY to indicate to higher layers which configuration activation has triggered */
    Int8                                activationTimeIndex;

    /* Tells PHY whether activationTime and activationTimeIndex should be used  */
    Boolean                             waitForActivationTime;

    /** Indicates H-RNTI, which is the identifier for the UE while using
     * HS-DSCH transport channel.
     * Please refer to <tt> [3G TS 25.331 section 10.3.3.14a] </tt>.
     */
    UH_RNTI                             hRnti;

    /** UmtsMode: FDD or TDD128.
\assoc UNION \ref mode */
    UmtsMode                            umtsMode;

    /** Indicates the HS-SCCH configuration info, which has:
     *     - HS-SCCH Channelisation Codes, ...
     */
     HS_SCCHConfigModeUnion             mode;
}
CphyHsScchConfigReq;
#endif


#if defined(UPGRADE_3G_HSUPA)
/** There is only one CphyEdchConfigReq for a configuration of E-DCH
 */
typedef struct CphyEdchConfigReqTag
{
    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configIdentifier;

    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configNoOfMessages;

    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configMessageNo;
    /** activationTime defines the frame number/time at which the
     * operation/changes caused by the related message shall take effect.
     * Values between 0 and 255 indicate the absolute value of CFN
     * (Connection Frame Number) of that frame number/time.
     */
    ActivationTime                      activationTime;

    /* Allows PHY to indicate to higher layers which configuration activation has triggered */
    Int8                                activationTimeIndex;

    /* Tells PHY whether activationTime and activationTimeIndex should be used  */
    Boolean                             waitForActivationTime;

    /** UmtsMode: FDD or TDD128.
\assoc UNION \ref mode
\assoc UNION \ref mode */
    UmtsMode                            umtsMode;

    /** mode is an union that contains FDD or TDD128
     * configurations of physial channels associated with E-DCH.
     */
    EDCHConfigModeUnion                 mode;
}
CphyEdchConfigReq;
#endif

/** @} */ /* End of SigCphyRadioLink group */





/** \defgroup SigCphyShutdown Shutdown Primitives
 * This section defines the primitives exchanged between PHY and RRC for the
 * shutdown of the 3G physical layer.
 *
 * @{
 */

/** Shutdown 3G L1 request.
 * This primitive is sent from RRC to PHY to shutdown the 3G physical layer.
 * The physical layer sends a #CphyDeactivateCnf primitive for confirmation.
 */
typedef EmptySignal CphyDeactivateReq;

/** Shutdown 3G L1 confirmation.
 * This primitive is sent from PHY to RRC to confirm that the
 * physical layer has shutdown.
 */
typedef EmptySignal CphyDeactivateCnf;


/** Suspend 3G L1 indication.
 * This primitive is sent from PHY to RRC to indicate that the physical layer
 * is suspended. This is used during 3G to 2G reselection and handover.
 */
typedef EmptySignal CphySuspendInd;

/** Select either FDD or TDD128 Mode.
 * This signal is sent from RRC to 3G L1 to switch control to either FDD or TDD128.
 */

typedef struct CphyModeSelectReqTag
{
    /** Defines whether to ACTIVATE_RAT or to SWITCH_RAT. */
    RatActivateMode       ratActivateMode;

    /** Indicate whether to activate FDD or TDD128 mode. */
    NetworkModes          networkMode;

    /** Indicate the RATs the UE will operate over (includes networkMode). */
    NetworkModes          handsetModes;

#ifdef PRODUCT_CALIBRATION_TEST_SWITCH
    /** Sending task ID */
    TaskId                taskId;
#endif
}
CphyModeSelectReq;

/** Confirmation that FDD or TDD128 mode is selected.
 * This signal is sent from 3G L1 to RRC in response to a #CphyModeSelectReq to
 * confirm that control of L1 has been activated to either FDD or TDD128.
 */
typedef EmptySignal CphyModeSelectCnf;

/** @} */ /* End of SigCphyShutdown group */





/** \defgroup SigCphyCellDbMgmt Cell Database Maintenance Primitives
 * This section defines the primitives exchanged between PHY and RRC for the
 * maintenance of the cell database in the physical layer.

 * \section SigCphyCellDbMgmtImpAct Implicit Actions
 * The following table specifies the implicit actions carried out in 3G L1 as
 * regards deletion and maintenance of cell databases in 3G L1:
 *
 * <TABLE>
 * <TR><TD><B>UE Scenario                                 </B></TD> <TD><B>L1 Action          </B></TD> </TR>
 * <TR><TD>Foreground RSSI Scan                               </TD> <TD>Delete all cell databases </TD> </TR>
 * <TR><TD>Intra-frequency Cell Reselection                   </TD> <TD>Maintain cell database    </TD> </TR>
 * <TR><TD>Intra-frequency Entry and Exit from CELL_DCH state </TD> <TD>Maintain cell database    </TD> </TR>
 * <TR><TD>Intra-frequency Handover                           </TD> <TD>Maintain cell database    </TD> </TR>
 * <TR><TD>Inter-frequency Cell Reselection                   </TD> <TD>Delete all cell databases </TD> </TR>
 * <TR><TD>Inter-frequency Entry and Exit from CELL_DCH state </TD> <TD>Delete all cell databases </TD> </TR>
 * <TR><TD>Inter-frequency Handover success                   </TD> <TD>Delete all cell databases </TD> </TR>
 * <TR><TD>Inter-frequency Handover failure                   </TD> <TD>Maintain cell database    </TD> </TR>
 * <TR><TD>Inter-frequency switch cases (TDD128 only)         </TD> <TD>Delete all cell databases </TD> </TR>
 * </TABLE>
 *
 * NOTE: Intra-cell freq switch is a working freq change in the same cell, i.e.
 * a working freq is changed between Primary freq and Secondary freq of the same cell,
 * or from one Secondary freq to another Secondary freq of the same cell.
 *
 * <b>Usage of bit-mask (\c CellIdMask) and its corresponding cells in the
 * array:</b>
 *
 * In #CphyIntraFreqCellInfoReq, #CphyInterFreqCellInfoReq and
 * #CphyGsmCellInfoReq primitives a bit in \c CellIdMask relates to the
 * corresponding position in the \c UmtsCellFDD or \v UmtsCellTDD128 array.
 * For example: 0x00000011 in \c CellIdMask has the corresponding cells in the
 * position 0 and 4 of \c UmtsCellFDD or \c UmtsCellTDD128 array.
 *
 * @{
 */

/** Update intra-frequency cell information request.
 * This primitive is sent from RRC to PHY to update the cell database for
 * intra-frequency cells.
 * Also see the \ref SigCphyCellDbMgmtImpAct taken in L1 upon receipt of this
 * primitive.
 */
typedef struct CphyIntraFreqCellInfoReqTag
{
    /** Indicates the cells to be added as a 32-bit mask.
     */
    CellIdMask                          addcellIds;

    /** Number of cells to be added or replaced.
\assoc ARRAY \ref mode.fdd \opt CONDITIONAL
\assoc ARRAY \ref mode.tdd128 \opt CONDITIONAL */
    Int8                                numberOfCells;

    /** UmtsMode: FDD or TDD128.
\assoc UNION \ref mode */
    UmtsMode                            umtsMode;

    /** Array of intra-frequency UMTS cell info to be added or replaced. */
    UmtsCellArrayUnion                  mode;

    /** Indicates uniquely the identifier of this measurement request and is
     * only an internal reference used by RRC to tie up measurement requests
     * with measurement reports.
     * This is NOT the same as the measurement identity that is used on the
     * air interface.
     */
    Int8                                measurementIdentity;

#ifdef ENABLE_CIL_VERSION_CONTROL
    /** Indicates uniquely the identifier of this CIL request and is
     * only an internal reference used by RRC/L1 to identify with measurement
     * reports.
     * This is only used for Field Trial purpose.
     */
    Int8                                intraFreqSeqNo;
#endif /* ENABLE_CIL_VERSION_CONTROL */
}
CphyIntraFreqCellInfoReq;


/** Update inter-frequency cell information request.
 * This primitive is sent from RRC to PHY to update the cell database for
 * inter-frequency cells.
 * Also see the \ref SigCphyCellDbMgmtImpAct taken in L1 upon receipt of this
 * primitive.
 */
typedef struct CphyInterFreqCellInfoReqTag
{
    /** Indicates the cells to be added as a 32-bit mask. For example, if \c
     * addCellIds = 0x00000001 means add/replace Cell ID 0
     */
    CellIdMask                          addcellIds;

    /** Number of cells to be added or replaced.
\assoc ARRAY \ref interFrequencyCell */
    Int8                                numberOfCells;
    /** Array of inter-frequency UMTS cell info to be added are replaced. */
    InterFrequencyCell                 interFrequencyCell [maxCellMeas];

    /** Indicates uniquely the identifier of this measurement request and is
     * only an internal reference used by RRC to tie up measurement requests
     * with measurement reports.
     * This is NOT the same as the measurement identity that is used on the
     * air interface.
     */
    Int8                                measurementIdentity;

#ifdef ENABLE_CIL_VERSION_CONTROL
    /** Indicates uniquely the identifier of this CIL request and is
     * only an internal reference used by RRC/L1 to identify with measurement
     * reports.
     * This is only used for Field Trial purpose.
     */
    Int8                                interFreqSeqNo;
#endif /* ENABLE_CIL_VERSION_CONTROL */
}
CphyInterFreqCellInfoReq;


/** Update GSM cell information request.
 * This primitive is sent from RRC to PHY to update the cell database for
 * inter-RAT cells (i.e.\ GSM).
 * Also see the \ref SigCphyCellDbMgmtImpAct taken in L1 upon receipt of this
 * primitive.
 */
typedef struct CphyGsmCellInfoReqTag
{
    /** Indicates the cells to be added as a 32-bit mask.
     */
    CellIdMask                          addcellIds;

    /** Number of cells to be added or replaced.
\assoc ARRAY \ref cellInfo */
    Int8                                numberOfCells;

    /** Array of GSM cell info to be added are replaced. */
    UGsmCell                            cellInfo [maxCellMeas];

    /** Indicates whether the ARFCN is DCS or PCS. It is possible for the UE
     * to receive from the network a GSM neighbour cell list with the
     * band indicator specified per cell (thus permitting both DCS and PCS
     * cells in a single neighbour cell list). For L1 implementations that
     * do not support simultaneous measurement of DCS and PCS cells the band
     * indicator is used for all DCS or PCS cells.
     */
    UFrequency_Band                     gsmBandIndicator;

#ifdef ENABLE_CIL_VERSION_CONTROL
    /** Indicates uniquely the identifier of this CIL request and is
     * only an internal reference used by RRC/L1 to identify with measurement
     * reports.
     * This is only used for Field Trial purpose.
     */
    Int8                                interRatSeqNo;
#endif /* ENABLE_CIL_VERSION_CONTROL */
}
CphyGsmCellInfoReq;
/** @} */ /* End of SigCphyCellDbMgmt group */





/** \defgroup SigCphyMeas Measurement Primitives
 * This section defines the primitives exchanged between PHY and RRC for
 * performing measurements.
 *
 * If the UE is in any RRC state other than CELL_DCH it can request physical
 * layer to perform:
 *     - neighbour cell measurements for cell reselection using the
 *       #CphyMonitorIntraFreqCellReq, #CphyMonitorInterFreqCellReq or
 *       #CphyMonitorGsmCellReq primitives.
 *     - serving cell measurements using the dedicated
 *       #CphyServingCellMeasurementReq primitive.
 *
 * When the physical layer is in CELL_DCH state the network can request the UE to
 * report measurements from 3 types of cells:
 *     - Active cells
 *     - Monitored cells
 *     - Detected cells
 *
 * Active cells are cells with active radio links to the UE. The physical layer
 * knows which cells are active from the #CphyRlSetupReq primitives.
 * Measurements for active cells are requested using the
 * #CphyMonitorIntraFreqCellReq primitive.
 *
 * Monitored cells are cells that the physical layer knows about from the
 * #CphyIntraFreqCellInfoReq, #CphyInterFreqCellInfoReq and #CphyGsmCellInfoReq
 * primitives. Measurements for monitor cells are requested using the
 * #CphyMonitorIntraFreqCellReq, #CphyMonitorInterFreqCellReq and
 * #CphyMonitorGsmCellReq primitives.
 *
 * Detected cells are any other cells on the same frequency as the serving cell
 * that the physical layer detects autonomously. \note The physical layer
 * is only required to monitor detected cells when it receives a
 * #CphyDetectedCellMeasurementReq primitive from the protocol stack.
 *
 * UE Internal measurements for transmit power and RX to TX time differences
 * are requested using the #CphyUeInternalMeasurementReq primitive.
 *
 * The measurement model for the system is as defined in <tt>[3G TS 25.302]</tt>
 * with only layer 3 filtering and evaluation being performed in the protocol stack.
 * The physical layer is responsible for all other averaging and filtering
 * that is necessary to achieve the performance criteria defined in <tt>
 * [3G TS 25.133]</tt>.
 *
 * The Intra-Frequency CPICH measurement period in CELL_DCH state is defined in
 * <tt>[3G TS 25.133 section 8]</tt> as 50 ms. However, <tt>[3G TS 25.302]</tt>
 * does not define the rate of measurements between the physical layer and
 * protocol stack. Therefore, the protocol stack can be configured to accept
 * intra-frequency measurements every 10, 20, 40, 50 or 100 ms to suit different
 * physical layer implementations. Also it is possible to spread the cells
 * measured over a number of different measurement reports.
 *
 * This is best explained by the following example:
 *
 * If a physical layer solution can report up to 2 measurements every 10ms and
 * can report all visible cells (A, B, C, D, E, F and G in this example) within
 * 50ms the report would be as follows:
 *
 * <TABLE>
 * <TR><TD><B>Time (ms) </B></TD> <TD><B>Cells Reported </B></TD></TR>
 * <TR><TD>0                </TD> <TD>A, B                  </TD></TR>
 * <TR><TD>10               </TD> <TD>C, D                  </TD></TR>
 * <TR><TD>20               </TD> <TD>E, F                  </TD></TR>
 * <TR><TD>30               </TD> <TD>G, none               </TD></TR>
 * <TR><TD>40               </TD> <TD>no report             </TD></TR>
 * <TR><TD>50               </TD> <TD>A, B                  </TD></TR>
 * <TR><TD>60               </TD> <TD>C, D                  </TD></TR>
 * <TR><TD>70               </TD> <TD>E, F                  </TD></TR>
 * <TR><TD>80               </TD> <TD>G, none               </TD></TR>
 * <TR><TD>90               </TD> <TD>no report             </TD></TR>
 * <TR><TD>100              </TD> <TD>A, B                  </TD></TR>
 * <TR><TD>110              </TD> <TD>C, D                  </TD></TR>
 * <TR><TD>120              </TD> <TD>E, F                  </TD></TR>
 * <TR><TD>130              </TD> <TD>G, none               </TD></TR>
 * <TR><TD>140              </TD> <TD>no report             </TD></TR>
 * <TR><TD>150              </TD> <TD>A, B                  </TD></TR>
 * <TR><TD>160              </TD> <TD>C, D                  </TD></TR>
 * <TR><TD>170              </TD> <TD>E, F                  </TD></TR>
 * <TR><TD>180              </TD> <TD>G, none               </TD></TR>
 * <TR><TD>190              </TD> <TD>no report             </TD></TR>
 * </TABLE>
 *
 * The measurement period is configured using UPS_INTRA_FREQ_FILTER_COUNT
 * in uphy_cfg.h. This defines the number of measurements the physical layer
 * will send every 200 ms. For the above example this is set to 4 (i.e. one
 * measurement every 50 ms).
 *
 * The protocol stack needs to know which cells the physical layer is measuring
 * and so the physical layer must send a #CphyMeasureIntraFreqCellsInd
 * primitive whenever the measured cell list changes. Therefore, a #CphyMeasureIntraFreqCellsInd
 * must be sent following every #CphyMonitorInterFreqCellReq primitive.
 *
 * \note #CphyMeasureIntraFreqCellsInd primitive is only used in CELL_DCH
 * state.
 *
 * If a cell is replaced by another cell then the PHY must send
 * #CphyMeasureIntraFreqCellsInd primitive indicating that the old cell is no
 * longer measured. As soon as the PHY detects that it can measure the new
 * cell it must send another #CphyMeasureIntraFreqCellsInd primitive indicating
 * that the new cell is being measured.
 *
 * The physical layer may indicate that a cell is currently not found (i.e. it
 * is not being actively monitored) by returning minimum values for Ec/N0 and
 * RSCP of -32768.
 *
 * Ec/No and RSCP are always reported in 1/8 dB and 1/8 dBm steps respectively.
 * \section example example
 * For example:
 * for RSCP -80 = -10 dBm;
 * for Ec/No -24 = -3 dB
 *
 * For Inter-Frequency measurements the measurement period is typically 480ms
 * but this can be longer if the compressed gaps are not sufficient to make
 * all the required measurements.  The #CphyMeasureInterFreqCellsInd
 * primitive is used to let the protocol stack know which inter-frequency
 * cells are being measured in the same ways as they are indicated for
 * intra-frequency measurements. The physical layer can send one report for all
 * cells being measured or can split the cells reported across more than one
 * #CphyMonitorInterFreqCellInd primitives. The protocol stack will filter
 * the measurements when they arrive but will only evaluate events once it has
 * received a measurement report for all the cells being measured.
 *
 * There is a requirement in <tt>[3G TS 25.331]</tt> and <tt>[3G TS 25.133]</tt>
 * to report measurement results on RACH, which could require the physical layer
 * to make measurement in all states except CELL_DCH for subsequent reporting
 * on RACH. This requires the physical layer to determine the maximum neighbour
 * cells (the protocol stack will inform the physical layer of the number of
 * cells to be reported and this can be 0 to 6). The measurements are requested
 * using the #CphyIntraFreqMeasOnRachReq primitive.
 *
 * During GSM PLMN search, the following applies:
 *     - Immediately after sending a #CphyGsmListMeasInd to RRC, PHY starts
 *       to scan the Strong Cells List and attempts to do BSIC decoding for the
 *       next strongest ARFCN for which synchronisation is successful.
 *     - Once PHY has decoded the BSIC (see above) for an ARFCN and has sent a
 *       #CphyGsmListBsicInd to RRC, PHY retrieves system information
 *       message type 3 and 4 and passes them on to RRC in separate
 *       #CphyGsmBcchDecodeInd (s). PHY then listens for
 *       #CphyGsmListBcchReq (s) until either a #CphyGsmListNextReq or a
 *       #CphyGsmListStopReq is received.
 *
 * @{
 */

/** Serving cell measurement request.
 * This primitive is sent from RRC to PHY to request the CPICH Ec/N0 and
 * CPICH RSCP for FDD or P-CCPCH RSCP and optionally the Timeslot ISCP in TDD128
 * level of the serving cell in all states except CELL_DCH state. The physical
 * layer then reports the results in #CphyServingCellMeasurementInd primitives
 * at the appropriate reporting rate.
 * This primitive is not confirmed.
 */
typedef EmptySignal CphyServingCellMeasurementReq;

/** Stop serving cell measurements request.
 * This primitive is sent from RRC to PHY to request L1 to stop serving cell
 * measurements. This primitive is not confirmed.
 */
typedef EmptySignal CphyStopServingCellMeasReq;


/** Intra-frequency neighbour cells measurement request.
 * This primitive is sent from RRC to PHY to request neighbour cell
 * measurements on a subset of the intra-frequency cell database, and this
 * primitive is not confirmed.
 *
 * \note The physical layer is given a list of neighbour cells and the
 * measurement types required. It is then the responsibility of the physical
 * layer to schedule these measurements and report them back to RRC at the
 * appropriate rate in a #CphyMonitorIntraFreqCellInd primitive. \note The
 * rate of measurements will depend on state. Refer to <tt>[3G TS 25.133]</tt>
 * for details of the requirements.
 *
 * In practice it may not be possible for the physical layer to schedule all
 * the measurements due to resource conflicts (e.g.\ if BCH decoding clashes
 * with the measurement schedule).
 */

typedef struct CphyMonitorIntraFreqCellReqTag
{
    /** Indicates uniquely the identifier of this measurement request and is
     * only an internal reference used by RRC to tie up measurement requests
     * with measurement reports.
     * This is NOT the same as the measurement identity that is used on the
     * air interface.
     */
    Int8                                measurementIdentity;

    /** UmtsMode: FDD or TDD128.
\assoc UNION \ref mode */
    UmtsMode                            umtsMode;

    /** Indicates the type of measurements required for the intra-frequency cells.
     * \note If a measurement quantity is set to 0xffffffff then this implies
     * all cells in the Intra-frequency monitor list.
     */
    IntraFreqCellMeasurementTypeUnion   mode;
}
CphyMonitorIntraFreqCellReq;


/** Inter-frequency neighbour cells measurement request.
 * This primitive is sent from RRC to PHY to request neighbour cell
 * measurements on a subset of the inter-frequency cell database, and this
 * primitive is not confirmed.
 *
 * \note The physical layer is given a list of neighbour cells and the
 * measurement types required. It is then the responsibility of the physical
 * layer to schedule these measurements and report them back to RRC at the
 * appropriate rate in a #CphyMonitorInterFreqCellInd primitive. \note The
 * rate of measurements will depend on state. Refer to <tt>[3G TS 25.133]
 * </tt> for details of the requirements.
 *
 * In practice it may not be possible for the physical layer to schedule all
 * the measurements due to resource conflicts (e.g. if BCH decoding clashes
 * with the measurement schedule).
 */
typedef struct CphyMonitorInterFreqCellReqTag
{
    /** Indicates uniquely the identifier of this measurement request and is
     * only an internal reference used by RRC to tie up measurement requests
     * with measurement reports.
     * This is NOT the same as the measurement identity that is used on the
     * air interface.
     */
    Int8                                measurementIdentity;

    /** UmtsMode: FDD or TDD128.
\assoc UNION \ref mode */
    UmtsMode                            umtsMode;

    /* Contains the type of measurements required for the inter-frequency cells.*/
    InterFreqCellMeasurementTypeUnion   mode;

#if defined (UPGRADE_REL8)
    /** Defines if high priority search mode is active.
     *  In this mode the UTRA carriers are restricted to those which have a
     *  higher priority than the serving cell.
     */
    Boolean                             highPrioritySearchMode;
#endif
}
CphyMonitorInterFreqCellReq;

#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/** Compressed mode configuration request.
 * This primitive is sent from RRC to PHY to configure the compressed mode
 * parameters.
 */
typedef struct CphyCompressedModeConfigReqTag
{
    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configIdentifier;

    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configNoOfMessages;

    /** See the \ref SigCphyParDesc in the CPHY Signalling Interface section. */
    Int8                                configMessageNo;

    /** The time (immediately or at a certain CFN) at which the physical
     * layer procedure is executed.
     */
    ActivationTime                      activationTime;

    /** Allows PHY to indicate to higher layers which configuration activation has triggered */
    Int8                                activationTimeIndex;

    /** Tells PHY whether activationTime and activationTimeIndex should be used  */
    Boolean                             waitForActivationTime;

    /** Indicates the number of TGPS's to be activated, whose parameters are
     * given in \c activateTgps array.

\assoc ARRAY \ref activateTgps */
    Int8                                numberActiveTgps;

    /** Indicates an array of Transmission Gap Pattern Sequences, which
     * consists of:
     *     - CFN of the first frame of the first pattern within the TGPS
     *     - TGPSI
     *     - TGPS configuration parameters like:
     *           -# Transmission Gap Pattern sequence measurement purpose
     *           -# Number of TGP's within the TGPS
     *           -# Transmission gap starting slot number
     *           -# Length of Transmission Gap within the TGP (in slots)
     *           -# Length of second Transmission Gap within the TGP
     *           -# Transmission Gap distance
     *           -# For more parameters refer to [3G TS 25.331] section 10.3.6.33
     */
    UTGPSActiveConfigParams             activateTgps[CPHY_MAX_ACTIVE_TGPS];

    /** Indicates an array of TGPSI, which needs to be deactivated, which
     * consists of:
     *     - Indicates if CFN at which TGPS deactivated is present.
     *     - Indicates the CFN at which TGPS should be deactivated
     *     - TGPSI
     */
    UTGPSDeactiveConfigParams           deactivateTgpsi[CPHY_MAX_ACTIVE_TGPS];

    /** Indicates the number of TGPS's to be deactivated, whose identifiers are
     * given in \c deactivateTgpsi array.

\assoc ARRAY \ref deactivateTgpsi */
    Int8                                numberDeactivateTgps;
}
CphyCompressedModeConfigReq;


/** Compressed mode error indication.
 * This primitive is sent from PHY to RRC to indicate an overlap of the
 * compressed mode patterns at run time. The Physical layer is expected to
 * autonomously take the following actions:
 *     - delete the overlapping transmission gap pattern sequence which is
 *       associated with the highest value of IE "TGPSI"
 *     - terminate the inter-frequency and/or inter-RAT measurements
 *       corresponding to the deleted transmission gap pattern sequence
 *
 * Refer to <tt>[3G TS 25.331 section 8.2.11.2]</tt> for further details.
 */
typedef struct CphyCompressedModeErrorIndTag
{
    /** Indicates the identifier of the TGPS that was deleted. */
    UTGPSI                              tgpsi;

    /** Indicates the frame at which the error was detected. */
    Int8                                cfn;
}
CphyCompressedModeErrorInd;
#endif


/** GSM neighbour cells measurement request.
 * This primitive is sent from RRC to PHY to request neighbour cell
 * measurements on a subset of the GSM cell database, and this primitive is
 * not confirmed.
 *
 * \note The physical layer is given a list of neighbour cells and the
 * measurement types required. It is then the responsibility of the physical
 * layer to schedule these measurements and report them back to RRC one by
 * one at the appropriate rate in a #CphyMonitorGsmCellInd primitive. In
 * practice it may not be possible for the physical layer to schedule all the
 * measurements due to resource conflicts (e.g. if BCH decoding clashes with
 * the measurement schedule).
 */
typedef struct CphyMonitorGsmCellReqTag
{
    /** Indicates uniquely the identifier of this measurement request and is
     * only an internal reference used by RRC to tie up measurement requests
     * with measurement reports.
     * This is NOT the same as the measurement identity that is used on the
     * air interface.
     */
    Int8                                measurementIdentity;

    /** Indicates the type of measurement required for the GSM cell. */
    UGsmMeasType                        gsmMeasType;

#if defined (UPGRADE_REL8)
    /** Defines if high priority search mode is active.
     *  In this mode the GSM carriers are restricted to those which have a
     *  higher priority than the serving cell.
     */
    Boolean                             highPrioritySearchMode;
#endif
}
CphyMonitorGsmCellReq;


/** Intra-frequency measurements on RACH request.
 * This primitive is sent from RRC to PHY to request measurements to be made
 * for subsequent reporting on RACH.
 */
typedef struct CphyIntraFreqMeasOnRachReqTag
{
    /** Indicates uniquely the identifier of this measurement request. */
    Int8                                measurementIdentity;

    /** Indicates the maximum number of neighbour cells to be reported.
     */
    Int8                                numberOfNeighbourCells;

    /** Indicates if SFN to SFN measurements are requested or not. */
    Boolean                             sfnToSfnOffset;
    /** UmtsMode: FDD or TDD128.
\assoc UNION \ref mode */
    UmtsMode                            umtsMode;

    /** mode specific CellMeasurementReq. */
    CellMeasurementReqUnion             mode;
}
CphyIntraFreqMeasOnRachReq;

#ifdef ENABLE_INTER_FREQ_MEAS_ON_RACH
#if defined (UPGRADE_3G_FDD) && defined (UPGRADE_REL6)
/** Inter-frequency measurements on RACH request.
 * This primitive is sent from RRC to PHY to request measurements to be made
 * for subsequent reporting on RACH.
 */
typedef struct CphyInterFreqMeasOnRachReqTag
{
    /** Indicates uniquely the identifier of this measurement request. */
    Int8                                measurementIdentity;

    /** Indicates the maximum number of neighbour cells to be reported.
     */
    Int8                                numberOfNeighbourCells;

    /** UmtsMode: FDD or TDD128.
\assoc UNION \ref mode */
    UmtsMode                            umtsMode;

    /** mode specific CellMeasurementReq. */
    CellMeasurementReqUnion             mode;
}
CphyInterFreqMeasOnRachReq;
#endif
#endif /* ENABLE_INTER_FREQ_MEAS_ON_RACH */

/** Detected cells measurement request.
 * This primitive is sent from RRC to PHY to request measurements for
 * detected cells; and this primitive is not confirmed. The detected cells can
 * only be intra-frequency cells.
 * \note RRC only uses this is CELL_DCH state.
 * \note In TDD128, If L1 receives the CphyDetectedCellMeasurementReq and it is
 * on a secondary frequency it should try to identify Ncells whose primary frequency
 * is the same as the current Scell secondary frequency
 */
typedef struct CphyDetectedCellMeasurementReqTag
{
    /** Indicates uniquely the identifier of this measurement request and is
     * only an internal reference used by RRC to tie up measurement requests
     * with measurement reports. This is NOT the same as the measurement
     * identity that is used on the air interface.
     */
    Int8                                measurementIdentity;

#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
    /** Indicates if CPICH Ec/No and RSCP measurements in FDD mode are requested
     * or not.
     */
    CellMeasurementReqFDD               fdd;

    /** Indicates if CFN to SFN measurements are requested or not. **/
    Boolean                             cfnToSfnOffset;
#endif
}
CphyDetectedCellMeasurementReq;


/** UE internal measurements request.
 * This primitive is sent from RRC to PHY to request internal UE measurements
 * such as transmit power and the RX to TX time difference; and this primitive
 * is not confirmed.
 * The physical layer then reports the results in #CphyUeInternalMeasurementInd
 * primitives at the appropriate reporting rate (every 50 ms).
 */
typedef struct CphyUeInternalMeasurementReqTag
{
    /** Indicates uniquely the identifier of this measurement request and is
     * only an internal reference used by RRC to tie up measurement requests
     * with measurement reports. This is NOT the same as the measurement
     * identity that is used on the air interface.
     */
    Int8                                measurementIdentity;

    /** Indicates if the UTRA RSSI measurements for the serving cell are
     * requested or not.
     */
    Boolean                             utra_CarrierRSSI;

    /** Indicates if UE transmit power measurements are requested or not. */
    Boolean                             ue_TransmittedPower;

    /** UmtsMode: FDD or TDD128.
\assoc UNION \ref mode */
    UmtsMode                            umtsMode;

    /** mode specific UeInternalMeasurementQuantity. */
    UeInternalMeasurementQuantityUnion  mode;
}
CphyUeInternalMeasurementReq;


/** Stop intra-frequency measurements request.
 * This primitive is sent from RRC to PHY to request L1 to stop
 * Intra-Frequency cell measurements referenced by \c measurementIdentity
 * parameter. This primitive is not confirmed.
 */
typedef struct CphyStopIntraFreqCellMeasReqTag
{
    /** Indicates the identity of the measurement to be stopped. */
    Int8                                measurementIdentity;
}
CphyStopIntraFreqCellMeasReq;


/** Stop inter-frequency measurements request.
 * This primitive is sent from RRC to PHY to request L1 to stop
 * Inter-Frequency cell measurements referenced by \c measurementIdentity
 * parameter. This primitive is not confirmed.
 */
typedef struct CphyStopInterFreqCellMeasReqTag
{
    /** Indicates the identity of the measurement to be stopped. */
    Int8                                measurementIdentity;
}
CphyStopInterFreqCellMeasReq;


/** Stop GSM cell measurements request.
 * This primitive is sent from RRC to PHY to request L1 to stop
 * GSM cell measurements referenced by \c measurementIdentity parameter. This
 * primitive is not confirmed.
 */
typedef struct CphyStopGsmCellMeasReqTag
{
    /** Indicates the identity of the measurement to be stopped. */
    Int8                                measurementIdentity;
}
CphyStopGsmCellMeasReq;

/** Stop detected cell measurements request.
 * This primitive is sent from RRC to PHY to request L1 to stop
 * detected cell measurements referenced by \c measurementIdentity parameter.
 * This primitive is not confirmed.
 */
typedef struct CphyStopDetectedCellMeasReqTag
{
    /** Indicates the identity of the measurement to be stopped. */
    Int8                                measurementIdentity;
}
CphyStopDetectedCellMeasReq;


/** Stop UE internal measurements request.
 * This primitive is sent from RRC to PHY to request L1 to stop
 * UE internal measurements referenced by \c measurementIdentity parameter.
 * This primitive is not confirmed.
 */
typedef struct CphyStopUeInternalMeasReqTag
{
    /** Indicates the identity of the measurement to be stopped. */
    Int8                                measurementIdentity;
}
CphyStopUeInternalMeasReq;


/** Serving cell measurement indication.
 * This primitive is sent from PHY to RRC to indicate the measurement results
 * to the RRC for the serving cell.
 *
 * \note The \a psc and \a arfcn fields are informational only and should not
 *        be interpreted by the protocol stack.  They are provided to aid
 *        debugging of physical layers that populate this field only.
 */
typedef struct CphyServingCellMeasurementIndTag
{
    /** The Uarfcn of the current serving cell. */
    UUARFCN                             uarfcn;

    /** UmtsMode: FDD or TDD128.
\assoc UNION \ref mode */
    UmtsMode                            umtsMode;

    /** mode specific ServingCellMeasurementReport. */
    ServingCellMeasurementReportUnion  mode;
}
CphyServingCellMeasurementInd;


/** Intra-frequency cell measurements indication.
 * This primitive is sent from PHY to RRC to indicate the measurement results
 * to the RRC for up to #CPHY_MAX_REPORTS intra-frequency monitor cells.
 */
typedef struct CphyMonitorIntraFreqCellIndTag
{
    /** Indicates the identity of the corresponding Req primitive for which the
     * measurement results are reported in this primitive.
     */
    Int8                                measurementIdentity;

#ifdef ENABLE_CIL_VERSION_CONTROL
    /** Indicates uniquely the identifier of this CIL request and is
     * only an internal reference used by RRC/L1 to identify with measurement
     * reports.
     * This is only used for Field Trial purpose.
     */
    Int8                                intraFreqSeqNo;
#endif /* ENABLE_CIL_VERSION_CONTROL */

    /** Indicates the cells as a 32-bit mask for which the measurement results
     * are reported in this primitive. For example, if \c cellIds = 0x00000001
     * it means measurement report in this report is for Cell ID 0.
     */
    CellIdMask                          cellIds;

    /** Indicates the number of cells for which the measurements are reported
     * in this primitive.

\assoc ARRAY \ref mode.fdd \opt CONDITIONAL
\assoc ARRAY \ref mode.tdd128 \opt CONDITIONAL */
    Int8                                numberOfCells;

    /** UmtsMode: FDD or TDD128.
\assoc UNION \ref mode */
    UmtsMode                            umtsMode;

#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
    /** Specifies the RSCP measurement for Serving cell P-CCPCH in 1/8 dBm
     * steps. (The invalid value is defined as 0x8000)*/
    SignedInt16                        sCellPccpchRscp;
#endif

    /** Indicates an array of measurement reports for the cells indicated by
     * the parameter \c cellIds, and the number of such reports should be as
     * given by the parameter \c numberOfCells.
     */
    IntraFreqCellMeasurementReportUnion mode;
}
CphyMonitorIntraFreqCellInd;


/** Intra-frequency measurements on RACH confirmation.
 * This primitive is sent from PHY to RRC to report the measurement results
 * to the RRC for the serving cell and up to 6 intra-frequency neighbour cells
 * in signal strength order.
 * This primitive is sent in response to a #CphyIntraFreqMeasOnRachReq primitive
 * with its report flag set TRUE.
 */
typedef struct CphyIntraFreqMeasOnRachCnfTag
{
    /** Indicates the identity of the corresponding Req primitive for which the
     * measurement results are reported in this primitive.
     */
    Int8                                measurementIdentity;

    /** UmtsMode: FDD or TDD128.
\assoc UNION \ref mode */
    UmtsMode                            umtsMode;

    /** mode specific RachMeasurementReport. */
    RachMeasurementReportUnion          mode;
}
CphyIntraFreqMeasOnRachCnf;

#ifdef ENABLE_INTER_FREQ_MEAS_ON_RACH
#if defined (UPGRADE_3G_FDD) && defined (UPGRADE_REL6)
/** Inter-frequency measurements on RACH confirmation.
 * This primitive is sent from PHY to RRC to report the measurement results
 * to the RRC for  up to 8 inter-frequency neighbour cells in signal strength
 * order. This primitive is sent in response to a #CphyIntraFreqMeasOnRachReq
 * primitive with its report flag set TRUE.
 */
typedef struct CphyInterFreqMeasOnRachCnfTag
{
    /** Indicates the identity of the corresponding Req primitive for which the
     * measurement results are reported in this primitive.
     */
    Int8                                measurementIdentity;

    UmtsMode                            umtsMode;

    /** mode specific RachMeasurementReport. */
    InterFreqRachMeasurementReportFDD   mode;
}
CphyInterFreqMeasOnRachCnf;
#endif
#endif /* ENABLE_INTER_FREQ_MEAS_ON_RACH */

/** Inter-frequency cell measurements indication.
 * This primitive is sent from PHY to RRC to indicate the measurement results
 * to the RRC for up to #CPHY_MAX_REPORTS inter-frequency monitored cells.
 */
typedef struct CphyMonitorInterFreqCellIndTag
{
    /** Indicates the identity of the corresponding Req primitive for which the
     * measurement results are reported in this primitive.
     */
    Int8                                measurementIdentity;

#ifdef ENABLE_CIL_VERSION_CONTROL
    /** Indicates uniquely the identifier of this CIL request and is
     * only an internal reference used by RRC/L1 to identify with measurement
     * reports.
     * This is only used for Field Trial purpose.
     */
    Int8                                interFreqSeqNo;
#endif /* ENABLE_CIL_VERSION_CONTROL */

    /** Indicates the cells as a 32-bit mask for which the measurement results
     * are reported in this primitive. For example, if \c cellIds = 0x00000001
     * it means measurement report in this report is for Cell ID 0.
     */
    CellIdMask                          cellIds;

    /** UmtsMode: FDD or TDD128.
\assoc UNION \ref mode */
    UmtsMode                            umtsMode;

    /** Results for Inter Frequency FDD/TDD128 cells */
    InterFreqCellMeasurementReportUnion   mode;

}
CphyMonitorInterFreqCellInd;


/** Start extra cell searching due to failing S criteria.
 * \note This msg is only used with a 3rd party PHY FDD.
 *        MEDIATEK PHY does not support it. PS need to be cleaned.
 *
 * This informs that physical layer that the S criteria has either not been
 * met for Nserv consecutive DRX cycles and nCell searching is desired, or
 * that the S criteria is met and additional searching is not required.  Upon
 * receipt of this signal, the physical layer generally has 12 seconds in PCH
 * state or 4 in FACH before cell selection will start.
 *
 * This signal is only applicable to Cell_PCH and Cell_FACH states, and
 * cell reselection implicitly cancels any previous search request.
 *
 * \note This is an informational signal that may help some Phys reselect
 *        to a new cell before cell selection is ran in accordance with
 *        25.133, section 4.2.2.1 "Measurement and evaluation of cell
 *        selection criteria S of serving cell".  It is legitimate for the
 *        Phy to ignore this signal.
 */
typedef struct CphyCellSearchReqTag
{
    /** Whether or not cell searching is advised.
     * If TRUE, the Phy is advised that that S criteria is not being met
     * and additional cell searching maybe beneficial.  If FALSE, the S
     * criteria is being met.
     */
    Boolean                             search;

#if defined (UPGRADE_3G_TDD128)
    Boolean                             measInterReq;
    Int8                                numOfFreq;
    UUARFCN                             uarfcn[MAX_TDD128_INTER_FREQUENCY_CELLS];
#endif
}
CphyCellSearchReq;

#if defined(UPGRADE_NOT_FOR_RELEASE)
/* Shouldn't we have all this GSM-specific interface under EXCLUDE_GSM? */
#endif /* UPGRADE_NOT_FOR_RELEASE */


/** BSIC decode for a GSM cell request.
 * This primitive is sent from RRC to PHY to request BSIC cell measurements
 * on a subset of the GSM cell database.
 *
 * \note The physical layer is given a list of cells and that it is then the
 * responsibility of the physical layer to schedule these measurements and
 * report them back to RRC one by one at the appropriate rate in a
 * #CphyBsicDecodeInd primitive.
 *
 * This primitive is not confirmed.
 */
typedef struct CphyBsicDecodeReqTag
{
    /** Indicates the identifier of this measurement request and is only an
     * internal reference used by RRC to tie up measurement requests with
     * measurement reports. This is NOT the same as the measurement identity
     * that is used on the air interface.
     */
    Int8                       measurementIdentity;

    /** Indicates the number of cells for which BSIC measurement is requested.
     * The maximum number of cells requested depends on the UE state. In Idle
     * Mode, the maximum number of cells for BSIC measurement is 4, in
     * CELL_FACH state it is 6 and in CELL_DCH state it is 8.

\assoc ARRAY \ref arfcn */
    Int8                       numArfcn;

    /** Indicates an array of frequencies of the cells for which BSIC
     * measurement is requested.
     */
    UBCCH_ARFCN                arfcn [CPHY_MAX_CELLS_FOR_BSIC_IDENTIFICATION];
}
CphyBsicDecodeReq;


/** BSIC decode for a GSM cell indication.
 * This primitive is sent from PHY to RRC to indicate the BSIC measurement of
 * a GSM cell to the RRC.
 */
typedef struct CphyBsicDecodeIndTag
{
    /** Indicates the identity of the corresponding Req primitive for which the
     * measurement results are reported in this primitive.
     */
    Int8                            measurementIdentity;

    /** Indicates the frequency of a cell for which BSIC measurement has been
     * requested.
     */
    UBCCH_ARFCN                     arfcn;

    /** Indicates the Base transceiver Station Identity Code, which is divided
     * into the Network Colour Code (NCC) and the Base Station Colour Code
     * (BCC).
     */
    UBSIC                           bsic;
}
CphyBsicDecodeInd;

      /********************************************/
      /* START: signals used for GSM PLMN search. */


/** Start GSM PLMN search request.
 * This primitive is sent from RRC to PHY to trigger off a GSM PLMN search.
 * PHY measures all the channels in the frequency bands specified by the
 * parameter \c bandMode, then sorts them by signal strength according to the
 * arfcnList and sends a #CphyGsmListMeasInd primitive to RRC.
 */
typedef struct CphyGsmListBcchReqTag
{
    /** Indicates a bitmap specifying the channels to measure. When a GSM PLMN
     * search is started, all bits are set to 1.
     */
    GsmArfcnList                    arfcnList;

    /** Indicates the requested bands. */
    BandMode                        bandMode;

    /** Indicates if L1 should measure the RSSI on these channels or not. */
    Boolean                         measureRssi;

    Int8                            numCellsToReport;
}
CphyGsmListBcchReq;


/** Synchronise and decode BSIC of the next ARFCN.
 * This primitive is sent from RRC to PHY to request PHY to move on to the
 * next ARFCN in the Strong Cells List, attempt synchronisation and BSIC
 * decoding.
 * This primitive may be sent to PHY any time between #CphyGsmListMeasInd
 * has been received by RRC and #CphyGsmListStopReq is sent by RRC to PHY.
 */
typedef EmptySignal CphyGsmListNextReq;


/** Stop BSIC and BCCH decode request.
 * This primitive is sent from RRC to PHY to request PHY to stop BSIC and
 * BCCH decoding activity for the cells in the Strong Cells List.
 * This primitive is sent to PHY any time between #CphyGsmListMeasInd has been
 * received by RRC and #CphyGsmListStopReq is sent by RRC to PHY.
 */
typedef EmptySignal CphyGsmListStopReq;


/** List of GSM measurements indication.
 * This primitive is sent from PHY to RRC. The physical layer measures each
 * of the ARFCNs in the bands indicated in #CphyGsmListBcchReq primitive and
 * sorts up to 40 of them according to signal strength ("Strong Cells List").
 * Then, physical layer sends #CphyGsmListMeasInd primitive to RRC and
 * immediately attempts to synchronise to the next strongest ARFCN in \c arfcn.
 * In case of success, PHY decodes its BSIC and sends a #CphyGsmListBsicInd
 * primitive to RRC. In case of failure, physical layer proceeds to attempt to
 * synchronise and decode the BSIC of the next strongest cell in \c arfcn
 * (physical layer skips ARFCNs for which synchronisation fails).
 *
 * Should physical layer fail to synchronise to any ARFCN's in the list received
 * in #CphyGsmListBcchReq primitive, it sends a #CphyGsmListBsicInd primitive
 * in which the field \c bsic is set to INVALID_BSIC.
 */
typedef struct CphyGsmListMeasIndTag
{
    /** Indicates the number of entries in the parameters \c arfcn and \c
     * rssiLevel.

\assoc ARRAY \ref arfcn
\assoc ARRAY \ref rssiLevel */
    Int8                            numMeas;

    /** Indicates a sorted list of ARFCN's, with strongest ARFCN first in the
     * list.
     */
    UBCCH_ARFCN                     arfcn [GSM_MAX_NUM_SEARCH_REPORTS];

    /** Indicates an array of signal levels for the ARFCN's in \c
     * arfcn. Position x in this parameter corresponds to position x in \c
     * arfcn. Signal levels are reported in 1/8 dBm steps.
     */
    GsmRssiLevel                    rssiLevel [GSM_MAX_NUM_SEARCH_REPORTS];
}
CphyGsmListMeasInd;


/** BSIC decode indication.
 * This primitive is sent from PHY to RRC. Upon decoding the BSIC for an
 * ARFCN in the Strong Cells List, PHY sends this primitive and starts to
 * decode BCCH data, namely system information message types 3 and 4 from that
 * ARFCN. If PHY could not synchronise to any carriers in the Strong Cells List,
 * it will set bsic to INVALID_BSIC.
 */
typedef struct CphyGsmListBsicIndTag
{
    /** Indicates the ARFCN for which BSIC decoding has been carried out. */
    UBCCH_ARFCN                     arfcn;

    /** Indicates the decoded BSIC. */
    GsmBsic                         bsic;

    /** Indicates the RSSI level for the ARFCN on which BSIC was decoded.
     * Signal levels are reported in 1/8 dBm steps.
     */
    GsmRssiLevel                    rssiLevel;
}
CphyGsmListBsicInd;


/** Read GSM system information indication.
 * This primitive is sent from PHY to RRC to read system information message
 * type 3 and 4 and send it to RRC in \c frame.
 */
typedef struct CphyGsmBcchDecodeIndTag
{
    /** Indicates the ARFCN for which BSIC decoding has been carried out.
     */
    UBCCH_ARFCN                     arfcn;

    /** Indicates the GSM system information message type number. */
    GsmControlBlockType             controlBlock;

    /** Indicates a raw Layer 2 physical block. \note It includes the L2PO byte.
     */
    GsmPhysicalBlock                frame;

    /** Indicates whether the CRC check has been successful (TRUE) or not
     * (FALSE).
     */
    Boolean                         goodCrc;
}
CphyGsmBcchDecodeInd;

      /* END: signals used for GSM PLMN search. */
      /******************************************/


/** GSM cell measurements indication.
 * This primitive is sent from PHY to RRC to indicate the measurement results
 * to the RRC for up to #CPHY_MAX_REPORTS GSM cells.
 */
typedef struct CphyMonitorGsmCellIndTag
{
    /** Indicates the identity of the corresponding Req primitive for which the
     * measurement results are reported in this primitive.
     */
    Int8                                measurementIdentity;

#ifdef ENABLE_CIL_VERSION_CONTROL
    /** Indicates uniquely the identifier of this CIL request and is
     * only an internal reference used by RRC/L1 to identify with measurement
     * reports.
     * This is only used for Field Trial purpose.
     */
    Int8                                interRatSeqNo;
#endif /* ENABLE_CIL_VERSION_CONTROL */

    /** Indicates the cells as a 32-bit mask for which the measurement results
     * are reported in this primitive. For example, if \c cellIds = 0x00000001
     * it means measurement report in this report is for Cell ID 0.
     */
    CellIdMask                          cellIds;

    /** Indicates the number of GSM cells for which reports are given by the
     * parameter \c gsmReport.

\assoc ARRAY \ref gsmReport */
    Int8                                numberOfCells;

#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
    /** Specifies the RSCP measurement for Serving cell P-CCPCH in 1/8 dBm steps.
     * (The invalid value is defined as 0x8000)*/
    SignedInt16                         sCellPccpchRscp;
#endif

    /** Indicates an array of GSM reports of the cells for which measurements
     * are requested.
     */
    UGsmReport                          gsmReport [maxCellMeas];
}
CphyMonitorGsmCellInd;


/** Detected cells measurement indication.
 * This primitive is sent from PHY to RRC to indicate the measurement results
 * to the RRC for up to #CPHY_MAX_REPORTS detected cells.
 */
typedef struct CphyDetectedCellMeasurementIndTag
{
    /** Indicates the identity of the corresponding Req primitive for which the
     * measurement results are reported in this primitive.
     */
    Int8                            measurementIdentity;

    /** Indicates the number of detected cells for which the measurements are
     * given in \c primaryScramblingCode and \c intraFrequencyReport.

\assoc ARRAY \ref mode.fdd \opt CONDITIONAL
\assoc ARRAY \ref mode.tdd128 \opt CONDITIONAL */
    Int8                            numberOfCells;

    /** UmtsMode: FDD or TDD128.
\assoc UNION \ref mode */
    UmtsMode                            umtsMode;

    /** Indicates an array of measurement results of the detected cells. */
    DetectedCellMeasurementReportArrayUnion     mode;
}
CphyDetectedCellMeasurementInd;


/** UE internal measurements indication.
 * This primitive is sent from PHY to RRC to indicate the internal UE
 * measurement results.
 * Maximum number of radio links for which Rx to Tx time difference is sought
 * is configurable in uphy_cfg.h. This has a valid range of 6 to 8 and must be
 * configured to the capability of the physical layer.
 */
typedef struct CphyUeInternalMeasurementIndTag
{
    /** Indicates the identity of the corresponding Req primitive for which the
     * measurement results are reported in this primitive.
     */
    Int8                                measurementIdentity;


    /** Indicates if \c utra_CarrierRSSIPresent is present or not.
\assoc PRESENT \ref utra_CarrierRSSI */
    Boolean                             utra_CarrierRSSIPresent;

    /** Indicates the RSSI reading for this frequency and is in 1/8 dBm steps,
     * for example: -400 = -50 dBm.
     */
    SignedInt16                         utra_CarrierRSSI;

    /** UmtsMode: FDD or TDD128.
\assoc UNION \ref mode */
    UmtsMode                            umtsMode;

    /** mode specific UeInternalMeasurementResult. */
    UeInternalMeasurementResultUnion    mode;
}
CphyUeInternalMeasurementInd;


/** Quality measurements configuration*/
typedef struct CphyQualityMeasurementReqTag
{
   /** measurementIdentity is the identity number used by the UTRAN to identify the measurement.*/
    UMeasurementIdentity                measurementIdentity;

    /** sequenceNumber is an additional identifier used between URRC and L1 to avoid
    * mis-synchronisation (particularly if a previously configured measurement on this
    * identity used periodic reporting).
    */
    Int8                                sequenceNumber;

    /** measureAllTransportChannels defines whether measurements should include all
    * transport channels (note if this is TRUE, the following fields numberOfTransportChannels
    * and transportChannelIdentity are not used by L1).
    */
    Boolean                             measureAllTransportChannels;

    /**numberOfTransportChannels is the number of transport channels on which measurements should be made.

\assoc ARRAY \ref transportChannelIdentity */
    Int8                                numberOfTransportChannels;

    /** transportChannelIdentity defines the transport channel identities on which measurements should be made.
    */
    Int8                                transportChannelIdentity[UPS_MAX_DL_NO_OF_TRCH];

#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
    /** measureSirTfcsIdIsPresent indicates if a SIR measuremnent should be made.
    */
    Boolean                             measureSirTfcsIdIsPresent;

    /** sirTfcsId  identifies the TFCS_Id on which the SIR measurements should be made.
     */
    Int8                                sirTfcsId;
#endif /* UPGRADE_3G_TDD128  || UPGRADE_3G_DOUBLE_DUAL_MODE */

    /** reportCriteriaType indicates whether periodical, event triggered or no
    * reporting should be used. (If no reporting is selected, L1 still measures quality
    * on the specified transport channels, but waits for CphyGetAdditionalMeasurementReq
    * as the trigger to report measurements to URRC).

\assoc UNION \ref reportCriteria */
    T_UQualityReportCriteria            reportCriteriaType;

    /** reportCriteria defines the periodicity (for periodic reporting) or the event triggers
    * (for event triggered reporting) which will cause L1 to report measurements to URRC.
    * If no reporting is selected in reportCriteriaType, this parameter is not used.
    */
    CphyQualityCriteria                 reportCriteria;

    /** additional contains a list of (traffic volume or quality-based) measurement identities
    * to be reported when this measurement identity is reported.
    */
    CphyAdditionalQualityIdentityList   additional;
}
CphyQualityMeasurementReq;

/** Quality measurement results struct.*/
typedef struct CphyQualityMeasurementIndTag
{
    /** meas contains the measured quality results.*/
    CphyAdditionalQualityMeasurement        meas;

    /** numberOfEventTrChs defines the number of events which caused this measurement report
    * (zero if the report was due to periodic reporting or if in response to a
    * CphyGetAdditionalQualityMeasurementReq).

\assoc ARRAY \ref eventTrChIdentities */
    Int8                                numberOfEventTrChs; /* 0 if periodic
                                                               or as result of
                                                               GetAdditionalQualityMeasurement */

    /** eventTrChIdentities identifies the transport channels whose event criteria
    * were fulfilled to trigger this report. (This field is not used if the report
    * was due to periodic reporting or if in response to a CphyGetAdditionalMeasurementReq).
    */
    Int8                                    eventTrChIdentities[UPS_MAX_DL_NO_OF_TRCH];

    /** additional contains any additional measurement results associated with this
    * measurement identity, in meas.
    */
    CphyAdditionalQualityMeasurementList    additional;
}
CphyQualityMeasurementInd;


/** When a quality measurement is configured with "noReporting" (rather than periodic
 * or event triggered), it might need to be attached to a MAC or Cell measurement.
 * This primitive is used by RRC to request a quality measurement report for attachment
 * to a measurement report to UTRAN.*/
typedef struct CphyGetAdditionalQualityMeasurementReqTag
{
    /** getMeasReqId is an identity number used by RRC to ensure sequencing is consistent
    * between CphyGetAdditonalQualityMeasurementReq and CphyGetAdditionalQualityMeasurementCnf.*/
    Int8                                getMeasReqId;

    /** add holds an array of measurement identities which RRC is requesting to be reported back to URRC.*/
    CphyAdditionalQualityIdentityList   add;
}
CphyGetAdditionalQualityMeasurementReq;


/**This message encapsulates the L1 measurement report for attachment in cases
 * where triggering occurred from a different measurement type.*/
typedef struct CphyGetAdditionalQualityMeasurementCnfTag
{
    /** getMeasReqId is an identity number used by RRC to ensure sequencing
    * is consistent between CphyGetAdditonalQualityMeasurementReq
    * and CphyGetAdditionalQualityMeasurementCnf.
    */
    Int8                                         getMeasReqId;

    /** meas contains the measurement data for each measurement identity
    * requested for the quality measurement report from L1.
    */
    CphyAdditionalQualityMeasurementList         meas;
}
CphyGetAdditionalQualityMeasurementCnf;


/** This Request primitive is sent from URRC to PHY to request L1 to stop
 * Quality  measurements referenced by measurementIdentity.*/
typedef struct CphyQualityMeasurementStopReqTag
{
    /**The measurementIdentity identifies the measurement to be stopped.*/
    UMeasurementIdentity                measurementIdentity;

    /** sequenceNumber is an additional identifier used between URRC and L1
    * to avoid mis-synchronisation.*/
    Int8                                sequenceNumber; /* same as in MeasReq */
}
CphyQualityMeasurementStopReq;

#if defined (DEVELOPMENT_VERSION)
/** This Request primitive is sent to PHY to request L1 to appply
 * offsets in 1/8dB units to measurement report. This is to help force reselection
 * and handover scenarios on live networks in Development.
 * To revert to normal measurements the offsets must be set to 0. */

typedef struct CphyDebugMeasurementOffsetReqTag
{
    /** The serving cell Rscp Offset in 1/8dB settings */
    SignedInt16 servingCellRscpOffset;

    /** The Serving cell Ec/N0 Offset in 1/8dB settings */
    SignedInt16 servingCellEcN0Offset;

    /** The Intra-Freqeuncy cell Rscp Offset in 1/8dB settings */
    SignedInt16 intraFreqCellRscpOffset;

    /** The Intra-Freqeuncy cell Ec/N0 Offset in 1/8dB settings */
    SignedInt16 intraFreqCellEcN0Offset;

    /** The Inter-Freqeuncy cell Rscp Offset in 1/8dB settings */
    SignedInt16 interFreqCellRscpOffset;

    /** The Inter-Freqeuncy cell Ec/N0 Offset in 1/8dB settings */
    SignedInt16 interFreqCellEcN0Offset;

    /** The GSM cell Rssi Offset in 1/8dB settings */
    SignedInt16 gsmIn3gCellRssiOffset;

    /** The serving cell Rssi Offset in 1/8dB settings */
    SignedInt16 gsmScellRssiOffset;

    /** The neighbour cell Rssi Offset in 1/8dB settings */
    SignedInt16 gsmNcellRssiOffset;

    /** The neighbour cell Ec/N0 Offset in 1/8dB settings */
    SignedInt16 uigCellEcNoOffset;

    /** The neighbour cell Rscp Offset in 1/8dB settings */
    SignedInt16 uigCellRscpOffset;
}
CphyDebugMeasurementOffsetReq;
#endif /* DEVELOPMENT_VERSION */

/** @} */ /* End of SigCphyMeas group */





/** \defgroup SigCphyDpchSync DPCH Synchronisation Primitives
 * This section defines the primitives exchanged between PHY and RRC to aid in
 * DPCCH/DPDCH synchronisation process.
 * To reduce the bandwidth of primitives sent between the physical layer and the
 * protocol stack the handling of SYNC primitives is to be done in physical
 * layer or in the adaptation layer if this functionality is not supported by a
 * third party physical layer. The parameters needed to implement the following
 * state machine are provided to the physical layer in the #CphyRlCommonSetupReq
 * primitive.
 *
 * \image html prd3gphychsync.png "Fig: Physical Channel Synchronisation"
 * \image latex prd3gphychsync.eps "Physical Channel Synchronisation" width=12cm
 *
 * @{
 */

/** IN_SYNC indication.
 * This primitive is sent from PHY to RRC to indicate that synchronisation of
 * a dedicated physical channel has been achieved according to the rules
 * defined in <tt>[3G TS 25.331]</tt>.
 * Note for FDD: The physical layer sends only one result for all active cells.
 */
typedef EmptySignal CphySyncInd;


/** OUT_OF_SYNC indication.
 * This primitive is sent from PHY to RRC to indicate that synchronisation of
 * a dedicated physical channel has been lost according to the rules
 * defined in <tt>[3G TS 25.331]</tt>.
 * Note for FDD: The physical layer sends only one result for all active cells.
 */
typedef struct CphyOutOfSyncIndTag
{
    /** Indicates the cause for OUT_OF_SYNC. */
    OutOfSyncCause                      cause;
}
CphyOutOfSyncInd;


/** SIG_CPHY_TEST_MODE_CONFIG_REQ.
 *  This primitive is sent from RRC to PHY to request PHY enter/leave test mode.
 *  Currently only min Tx power control is involved.
 */
typedef struct CphyTestModeConfigReqTag
{
    /** Indicates whether to start/stop test mode */
    Boolean                             start;
}
CphyTestModeConfigReq;

#ifdef UPGRADE_3G_FDD
#ifdef ENABLE_UL_TX_CONTROL
/** SIG_CPHY_UL_TX_CONTROL_REQ.
 *  This primitive is sent from RRC to PHY to request PHY start/stop UL Tx.
 */
typedef struct CphyUlTxControlReqTag
{
    /** Indicates whether to start/stop UL Tx */
    Boolean                             ulTxNonStop;
}
CphyUlTxControlReq;
#endif /* ENABLE_UL_TX_CONTROL */
#endif /* UPGRADE_3G_FDD */

/*for integration testing in PC*/
#ifdef  PC_INTEGRATE_TDD128
/**This enumeration defines the case ID.
*/
typedef enum T1PcTestCaseIdTag
{
    TL1_FS_RA_SUCCESS,
    TL1_DS_RB_RAB_01_ID,/*TL1_DS_RB_RAB_01*/
    TL1_DS_RB_RAB_02_ID,/*TL1_DS_RB_RAB_02*/
    TL1_DS_RB_RAB_24_ID,/*TL1_DS_RB_RAB_24*/
    TL1IT_DS_RB_RAB_002_ID,
    /*...other test case.*/
    TL1_DS_RECONFIG_01_ID,
    TL1_DS_HANDOVER_01_ID, /*intra- hard handover*/
    TL1_DS_HANDOVER_02_ID, /*intra baton handover*/
    TL1_CSM_QUALITY_MEAS_001_ID, /*quality meas, add by toptian */
    /*Cases for T1 PHY PC Test*/
    T1_PHY_IT_FACH_001,
    /*cases for T1 PHY PC Test For RA*/
    T1_PHY_IT_RA_001,
    /*cases for T1 PHY PC Test For DPCH*/
    T1_PHY_IT_DPCH_001,
    TL1_DS_SYNC_01_ID,
    TL1_DS_HANDOVER_03_ID,
    TL1_DS_HANDOVER_04_ID,
    TL1_DS_MULTIPLEX_01_ID,
    TL1_DS_MULTIPLEX_02_ID,
    TL1_DPA_CAP_02_ID,
    TL1_PC_CASE_ID_NUM
} T1PcTestCaseId;

/** This is a structure with two members.
 */

typedef struct PcIntStartReqTag
{
  T1PcTestCaseId testCaseId;      /*the case ID */
} PcIntStartReq;

typedef EmptySignal PcIntStartCnf;

typedef struct PcIntStartIndTag
{
  Boolean tstRslt;/*TRUE --- means OK, FALSE --- means ERROR*/
  Int16 tstStatus;      /*test status, Only when testRslt is FALSE it is valid, 0 -- means the related probe point is error*/
} PcIntStartInd;
#endif
/** @} */ /* End of SigCphyDpchSync group */

# if defined (UPGRADE_LTE)

/** \defgroup SigCphyEutra EUTRA Multimode signals
 * This section defines the primitives exchanged between PHY and RRC for
 * PLMN Search of EUTRA, EUTRA Measurements and EUTRA BCH decode.
 *
 * @{
 */
/** Start or Stop Eutra measurements.
 *  This signal is used in both RRC_IDLE and RRC_CONNECTED states and is used to
 *  request EL1 to report to RRC the RSRP and RSRQ measurements for EUTRA
 *  cells.
 */
typedef struct CphyEutraMeasurementReqTag
{
    /** Defines the number of inter-frequency carriers to be measured.
     * Range is 0 to ECPHY_MAX_EUTRA_INTER_FREQ_CARRIERS.
     * A value of 0 implies that no inter-frequency carriers are to be measured.
\assoc ARRAY \ref eutraInterFreqCarrier */
    Int8                                numberOfCarriers;

    /** Defines the carrier frequency, allowedMeasBandwidth and presenceAntennaPort1
     * for each carrier. */
    EutraInterFreqCarrier               eutraInterFreqCarrier [ECPHY_MAX_EUTRA_INTER_FREQ_CARRIERS];

    /** Defines if high priority search mode is active.
     *  In this mode the #eutraInterFreqCarrier array is restricted to EUTRA carriers which
     *  have a higher priority than the serving cell.
     */
    Boolean                             highPrioritySearchMode;
}
CphyEutraMeasurementReq;

typedef EutraInterFrequencyMeasurementInd   CphyEutraMeasurementInd;
typedef EutraRssiScanReq                    CphyEutraRssiScanReq;
typedef EutraRssiScanCnf                    CphyEutraRssiScanCnf;
typedef EutraFindCellReq                    CphyEutraFindCellReq;
typedef EutraFindCellCnf                    CphyEutraFindCellCnf;
typedef EutraBcchDataInd                    CphyEutraBchDecodeInd;

/** Switch on/off serving cell BCH request.
 *  This primitive is sent from ERRC to EL1 request EL1 to decode the specified
 *  system information from the BCH transport channel on the serving cell.
 */
typedef struct CphyEutraBchDecodeReq
{
    /** decodeMibAndSi1 indicates if ERRC needs EL1 to decode the MIB and
     * SystemInformationBlockType1.
     * The SystemInformationBlockType1 uses a fixed schedule with a periodicity
     * of 80 ms and repetitions made within 80 ms. The first transmission of
     * SystemInformationBlockType1 is scheduled in subframe #5 of radio frames
     * for which the SFN mod 8 = 0, and repetitions are scheduled in subframe #5
     * of all other radio frames for which SFN mod 2 = 0.*/
    Boolean                             decodeMibAndSi1;

    /** Specifies the downlink frequency on which the BCCH data is decoded. */
    EutraArfcn                          dlFrequency;

    /** Indicates the physical cell id on which the BCCH data is decoded. */
    EutraPhysCellId                     physCellId;
}
CphyEutraBchDecodeReq;

/** @} */ /* End of SigCphyEutra group */
# endif


/** @} */ /* End of SigCphy group */

#endif /* CPHY_SIG_H */

/* END OF FILE */



