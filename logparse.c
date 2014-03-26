/* (C) 2013 by Tobias Engel <tobias@sternraute.de>
 * All Rights Reserved
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#define _GNU_SOURCE
#include <string.h>
#include <arpa/inet.h>

#include "xgoldmon.h"

struct msgb *make_rrc_uplink_msg(uint8_t gsmtap_chan, uint8_t *msg_start, uint16_t msg_len, uint8_t **dummy);
struct msgb *make_rrc_downlink_msg(uint8_t gsmtap_chan, uint8_t *msg_start, uint16_t msg_len, uint8_t **dummy);
struct msgb *make_gsml2_msg(uint8_t gsmtap_chan, uint8_t *msg_start, uint16_t msg_len, uint8_t **dummy);
struct msgb *register_search_msg(uint8_t dummy1, uint8_t *msg_start, uint16_t dummy2, uint8_t **searchmsg_p);

struct rmsg_act note2_ltable[] =
  {
    { { 0x04, 0x01, 0x05,   -1 }, GSMTAP_RRC_SUB_UL_DCCH_Message, 28,  0, 26, make_rrc_uplink_msg },
    { { 0x04, 0x01, 0x06,   -1 }, GSMTAP_RRC_SUB_UL_CCCH_Message, 24,  0, 22, make_rrc_uplink_msg },
    { { 0x07, 0x01, 0x3f,   -1 }, GSMTAP_RRC_SUB_PCCH_Message,    20,  0, 18, make_rrc_downlink_msg },
    { { 0x08, 0x01, 0x59,   -1 }, GSMTAP_RRC_SUB_DL_DCCH_Message, 28,  0, 26, make_rrc_downlink_msg },
    { { 0x08, 0x01, 0x5a,   -1 }, GSMTAP_RRC_SUB_DL_CCCH_Message, 24,  0, 22, make_rrc_downlink_msg },
    { { 0x35,   -1, 0x0e,   -1 }, GSMTAP_CHANNEL_SDCCH,           31, 23,  0, make_gsml2_msg },
    { { 0x0d, 0x04, 0x0f,   -1 }, GSMTAP_CHANNEL_SDCCH,           10, 23,  0, make_gsml2_msg },
    { { 0x0d, 0x04, 0x04,   -1 }, 0,                              20,  0,  0, register_search_msg },
    { { 0x0d, 0x02, 0x09,   -1 }, 0,                              24,  0,  0, register_search_msg },
    { { 0x13,   -1,   -1, 0x06 }, GSMTAP_CHANNEL_BCCH,            18, 23,  0, make_gsml2_msg },
    { { 0x13, 0x0b,   0x5f, -1 }, GSMTAP_CHANNEL_PCH,             18, 23,  0, make_gsml2_msg },
    { { 0xf1, 0x01, 0x50,   -1 }, GSMTAP_CHANNEL_AGCH,            10, 23,  0, make_gsml2_msg },
    { { 0x13,   -1,   -1, 0x05 }, GSMTAP_CHANNEL_AGCH,            18, 23,  0, make_gsml2_msg },
    { { 0xf1, 0x01, 0x51,   -1 }, GSMTAP_CHANNEL_AGCH,            10, 23,  0, make_gsml2_msg },
    { {   -1,   -1,   -1,   -1 }, 0,                               0,  0,  0, NULL },
  };

struct rmsg_act s3_ltable[] =
  {
    { { 0x04, 0x01, 0x05,   -1 }, GSMTAP_RRC_SUB_UL_DCCH_Message, 28,  0, 26, make_rrc_uplink_msg },
    { { 0x04, 0x01, 0x06,   -1 }, GSMTAP_RRC_SUB_UL_CCCH_Message, 24,  0, 22, make_rrc_uplink_msg },
    { { 0x07, 0x01, 0x3f,   -1 }, GSMTAP_RRC_SUB_PCCH_Message,    20,  0, 18, make_rrc_downlink_msg },
    { { 0x08, 0x01, 0x59,   -1 }, GSMTAP_RRC_SUB_DL_DCCH_Message, 28,  0, 26, make_rrc_downlink_msg },
    { { 0x08, 0x01, 0x5a,   -1 }, GSMTAP_RRC_SUB_DL_CCCH_Message, 24,  0, 22, make_rrc_downlink_msg },
    { { 0x0d,   -1, 0x0f,   -1 }, GSMTAP_CHANNEL_SDCCH,           10, 23,  0, make_gsml2_msg },
    { { 0x0d, 0x04, 0x04,   -1 }, 0,                              20,  0,  0, register_search_msg },
    { { 0x0d, 0x02, 0x09,   -1 }, 0,                              24,  0,  0, register_search_msg },
    { { 0x13,   -1,   -1, 0x05 }, GSMTAP_CHANNEL_AGCH,            18, 23,  0, make_gsml2_msg },
    { { 0x13,   -1,   -1, 0x06 }, GSMTAP_CHANNEL_BCCH,            18, 23,  0, make_gsml2_msg },
    { { 0x13,   -1,   -1, 0x0a }, GSMTAP_CHANNEL_PCH,             18, 23,  0, make_gsml2_msg },
    { { 0xf1, 0x01, 0x50,   -1 }, GSMTAP_CHANNEL_AGCH,            10, 23,  0, make_gsml2_msg },
    { { 0xf1, 0x01, 0x51,   -1 }, GSMTAP_CHANNEL_AGCH,            10, 23,  0, make_gsml2_msg },
    { {   -1,   -1,   -1,   -1 }, 0,                               0,  0,  0, NULL },
  };

struct rmsg_act s2_ltable[] =
  {
    { { 0x04, 0x01, 0x05,   -1 }, GSMTAP_RRC_SUB_UL_DCCH_Message, 28,  0, 26, make_rrc_uplink_msg },
    { { 0x04, 0x01, 0x06,   -1 }, GSMTAP_RRC_SUB_UL_DCCH_Message, 24,  0, 22, make_rrc_uplink_msg },
    { { 0x04, 0x01, 0x07,   -1 }, GSMTAP_RRC_SUB_UL_CCCH_Message, 24,  0, 22, make_rrc_uplink_msg },
    { { 0x07, 0x01, 0x3d,   -1 }, GSMTAP_RRC_SUB_PCCH_Message,    20,  0, 18, make_rrc_downlink_msg },
    { { 0x08, 0x01, 0x52,   -1 }, GSMTAP_RRC_SUB_DL_DCCH_Message, 28,  0, 26, make_rrc_downlink_msg },
    { { 0x08, 0x02, 0x52,   -1 }, GSMTAP_RRC_SUB_DL_DCCH_Message, 28,  0, 26, make_rrc_downlink_msg },
    { { 0x08, 0x01, 0x53, 0x01 }, GSMTAP_RRC_SUB_DL_DCCH_Message, 24,  0, 22, make_rrc_downlink_msg },
    { { 0x08, 0x02, 0x53, 0x01 }, GSMTAP_RRC_SUB_DL_DCCH_Message, 24,  0, 22, make_rrc_downlink_msg }, 
    { { 0x08, 0x01, 0x53, 0x00 }, GSMTAP_RRC_SUB_DL_CCCH_Message, 24,  0, 22, make_rrc_downlink_msg },
    { { 0x08, 0x02, 0x53, 0x00 }, GSMTAP_RRC_SUB_DL_CCCH_Message, 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x0f, 0x39,   -1 }, GSMTAP_RRC_SUB_BCCH_BCH_Message,10,  36, 0, make_rrc_downlink_msg },
    { { 0x07, 0x0f, 0x01, 0x00 }, GSMTAP_RRC_SUB_MasterInformationBlock, 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x0f, 0x01, 0x01 }, GSMTAP_RRC_SUB_SysInfoType1          , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x0f, 0x01, 0x02 }, GSMTAP_RRC_SUB_SysInfoType2          , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x0f, 0x01, 0x03 }, GSMTAP_RRC_SUB_SysInfoType3          , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x0f, 0x01, 0x04 }, GSMTAP_RRC_SUB_SysInfoType4          , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x0f, 0x01, 0x05 }, GSMTAP_RRC_SUB_SysInfoType5          , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x0f, 0x01, 0x06 }, GSMTAP_RRC_SUB_SysInfoType6          , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x0f, 0x01, 0x07 }, GSMTAP_RRC_SUB_SysInfoType7          , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x0f, 0x01, 0x0b }, GSMTAP_RRC_SUB_SysInfoType11         , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x0f, 0x01, 0x0c }, GSMTAP_RRC_SUB_SysInfoType12         , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x0f, 0x01, 0x0f }, GSMTAP_RRC_SUB_SysInfoType15         , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x0f, 0x01, 0x1f }, GSMTAP_RRC_SUB_SysInfoType16         , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x0f, 0x01, 0x1a }, GSMTAP_RRC_SUB_SysInfoType18         , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x0f, 0x01, 0x1b }, GSMTAP_RRC_SUB_SysInfoTypeSB1        , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x0f, 0x01, 0x1c }, GSMTAP_RRC_SUB_SysInfoTypeSB2        , 24,  0, 22, make_rrc_downlink_msg }, 
    { { 0x07, 0x08, 0x39,   -1 }, GSMTAP_RRC_SUB_BCCH_BCH_Message,10,  36, 0, make_rrc_downlink_msg },
    { { 0x07, 0x08, 0x01, 0x00 }, GSMTAP_RRC_SUB_MasterInformationBlock, 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x08, 0x01, 0x01 }, GSMTAP_RRC_SUB_SysInfoType1          , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x08, 0x01, 0x02 }, GSMTAP_RRC_SUB_SysInfoType2          , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x08, 0x01, 0x03 }, GSMTAP_RRC_SUB_SysInfoType3          , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x08, 0x01, 0x04 }, GSMTAP_RRC_SUB_SysInfoType4          , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x08, 0x01, 0x05 }, GSMTAP_RRC_SUB_SysInfoType5          , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x08, 0x01, 0x06 }, GSMTAP_RRC_SUB_SysInfoType6          , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x08, 0x01, 0x07 }, GSMTAP_RRC_SUB_SysInfoType7          , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x08, 0x01, 0x0b }, GSMTAP_RRC_SUB_SysInfoType11         , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x08, 0x01, 0x0c }, GSMTAP_RRC_SUB_SysInfoType12         , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x08, 0x01, 0x0f }, GSMTAP_RRC_SUB_SysInfoType15         , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x08, 0x01, 0x1f }, GSMTAP_RRC_SUB_SysInfoType16         , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x08, 0x01, 0x1a }, GSMTAP_RRC_SUB_SysInfoType18         , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x08, 0x01, 0x1b }, GSMTAP_RRC_SUB_SysInfoTypeSB1        , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x08, 0x01, 0x1c }, GSMTAP_RRC_SUB_SysInfoTypeSB2        , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x06, 0x39,   -1 }, GSMTAP_RRC_SUB_BCCH_BCH_Message,10,  36, 0, make_rrc_downlink_msg },
    { { 0x07, 0x06, 0x01, 0x00 }, GSMTAP_RRC_SUB_MasterInformationBlock, 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x06, 0x01, 0x01 }, GSMTAP_RRC_SUB_SysInfoType1          , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x06, 0x01, 0x02 }, GSMTAP_RRC_SUB_SysInfoType2          , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x06, 0x01, 0x03 }, GSMTAP_RRC_SUB_SysInfoType3          , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x06, 0x01, 0x04 }, GSMTAP_RRC_SUB_SysInfoType4          , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x06, 0x01, 0x05 }, GSMTAP_RRC_SUB_SysInfoType5          , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x06, 0x01, 0x06 }, GSMTAP_RRC_SUB_SysInfoType6          , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x06, 0x01, 0x07 }, GSMTAP_RRC_SUB_SysInfoType7          , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x06, 0x01, 0x0b }, GSMTAP_RRC_SUB_SysInfoType11         , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x06, 0x01, 0x0c }, GSMTAP_RRC_SUB_SysInfoType12         , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x06, 0x01, 0x0f }, GSMTAP_RRC_SUB_SysInfoType15         , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x06, 0x01, 0x1f }, GSMTAP_RRC_SUB_SysInfoType16         , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x06, 0x01, 0x1a }, GSMTAP_RRC_SUB_SysInfoType18         , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x06, 0x01, 0x1b }, GSMTAP_RRC_SUB_SysInfoTypeSB1        , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x06, 0x01, 0x1c }, GSMTAP_RRC_SUB_SysInfoTypeSB2        , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x05, 0x39,   -1 }, GSMTAP_RRC_SUB_BCCH_BCH_Message,10,  36, 0, make_rrc_downlink_msg },
    { { 0x07, 0x05, 0x01, 0x00 }, GSMTAP_RRC_SUB_MasterInformationBlock, 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x05, 0x01, 0x01 }, GSMTAP_RRC_SUB_SysInfoType1          , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x05, 0x01, 0x02 }, GSMTAP_RRC_SUB_SysInfoType2          , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x05, 0x01, 0x03 }, GSMTAP_RRC_SUB_SysInfoType3          , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x05, 0x01, 0x04 }, GSMTAP_RRC_SUB_SysInfoType4          , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x05, 0x01, 0x05 }, GSMTAP_RRC_SUB_SysInfoType5          , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x05, 0x01, 0x06 }, GSMTAP_RRC_SUB_SysInfoType6          , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x05, 0x01, 0x07 }, GSMTAP_RRC_SUB_SysInfoType7          , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x05, 0x01, 0x0b }, GSMTAP_RRC_SUB_SysInfoType11         , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x05, 0x01, 0x0c }, GSMTAP_RRC_SUB_SysInfoType12         , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x05, 0x01, 0x0f }, GSMTAP_RRC_SUB_SysInfoType15         , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x05, 0x01, 0x1f }, GSMTAP_RRC_SUB_SysInfoType16         , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x05, 0x01, 0x1a }, GSMTAP_RRC_SUB_SysInfoType18         , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x05, 0x01, 0x1b }, GSMTAP_RRC_SUB_SysInfoTypeSB1        , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x05, 0x01, 0x1c }, GSMTAP_RRC_SUB_SysInfoTypeSB2        , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x04, 0x39,   -1 }, GSMTAP_RRC_SUB_BCCH_BCH_Message,10,  36, 0, make_rrc_downlink_msg },
    { { 0x07, 0x04, 0x01, 0x00 }, GSMTAP_RRC_SUB_MasterInformationBlock, 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x04, 0x01, 0x01 }, GSMTAP_RRC_SUB_SysInfoType1          , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x04, 0x01, 0x02 }, GSMTAP_RRC_SUB_SysInfoType2          , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x04, 0x01, 0x03 }, GSMTAP_RRC_SUB_SysInfoType3          , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x04, 0x01, 0x04 }, GSMTAP_RRC_SUB_SysInfoType4          , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x04, 0x01, 0x05 }, GSMTAP_RRC_SUB_SysInfoType5          , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x04, 0x01, 0x06 }, GSMTAP_RRC_SUB_SysInfoType6          , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x04, 0x01, 0x07 }, GSMTAP_RRC_SUB_SysInfoType7          , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x04, 0x01, 0x0b }, GSMTAP_RRC_SUB_SysInfoType11         , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x04, 0x01, 0x0c }, GSMTAP_RRC_SUB_SysInfoType12         , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x04, 0x01, 0x0f }, GSMTAP_RRC_SUB_SysInfoType15         , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x04, 0x01, 0x1f }, GSMTAP_RRC_SUB_SysInfoType16         , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x04, 0x01, 0x1a }, GSMTAP_RRC_SUB_SysInfoType18         , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x04, 0x01, 0x1b }, GSMTAP_RRC_SUB_SysInfoTypeSB1        , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x04, 0x01, 0x1c }, GSMTAP_RRC_SUB_SysInfoTypeSB2        , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x02, 0x39,   -1 }, GSMTAP_RRC_SUB_BCCH_BCH_Message,10,  36, 0, make_rrc_downlink_msg },
    { { 0x07, 0x02, 0x01, 0x00 }, GSMTAP_RRC_SUB_MasterInformationBlock, 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x02, 0x01, 0x01 }, GSMTAP_RRC_SUB_SysInfoType1          , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x02, 0x01, 0x02 }, GSMTAP_RRC_SUB_SysInfoType2          , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x02, 0x01, 0x03 }, GSMTAP_RRC_SUB_SysInfoType3          , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x02, 0x01, 0x04 }, GSMTAP_RRC_SUB_SysInfoType4          , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x02, 0x01, 0x05 }, GSMTAP_RRC_SUB_SysInfoType5          , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x02, 0x01, 0x06 }, GSMTAP_RRC_SUB_SysInfoType6          , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x02, 0x01, 0x07 }, GSMTAP_RRC_SUB_SysInfoType7          , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x02, 0x01, 0x0b }, GSMTAP_RRC_SUB_SysInfoType11         , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x02, 0x01, 0x0c }, GSMTAP_RRC_SUB_SysInfoType12         , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x02, 0x01, 0x0f }, GSMTAP_RRC_SUB_SysInfoType15         , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x02, 0x01, 0x1f }, GSMTAP_RRC_SUB_SysInfoType16         , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x02, 0x01, 0x1a }, GSMTAP_RRC_SUB_SysInfoType18         , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x02, 0x01, 0x1b }, GSMTAP_RRC_SUB_SysInfoTypeSB1        , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x02, 0x01, 0x1c }, GSMTAP_RRC_SUB_SysInfoTypeSB2        , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x01, 0x39,   -1 }, GSMTAP_RRC_SUB_BCCH_BCH_Message,10,  36, 0, make_rrc_downlink_msg },
    { { 0x07, 0x01, 0x01, 0x00 }, GSMTAP_RRC_SUB_MasterInformationBlock, 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x01, 0x01, 0x01 }, GSMTAP_RRC_SUB_SysInfoType1          , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x01, 0x01, 0x02 }, GSMTAP_RRC_SUB_SysInfoType2          , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x01, 0x01, 0x03 }, GSMTAP_RRC_SUB_SysInfoType3          , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x01, 0x01, 0x04 }, GSMTAP_RRC_SUB_SysInfoType4          , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x01, 0x01, 0x05 }, GSMTAP_RRC_SUB_SysInfoType5          , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x01, 0x01, 0x06 }, GSMTAP_RRC_SUB_SysInfoType6          , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x01, 0x01, 0x07 }, GSMTAP_RRC_SUB_SysInfoType7          , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x01, 0x01, 0x0b }, GSMTAP_RRC_SUB_SysInfoType11         , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x01, 0x01, 0x0c }, GSMTAP_RRC_SUB_SysInfoType12         , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x01, 0x01, 0x0f }, GSMTAP_RRC_SUB_SysInfoType15         , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x01, 0x01, 0x1f }, GSMTAP_RRC_SUB_SysInfoType16         , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x01, 0x01, 0x1a }, GSMTAP_RRC_SUB_SysInfoType18         , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x01, 0x01, 0x1b }, GSMTAP_RRC_SUB_SysInfoTypeSB1        , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x07, 0x01, 0x01, 0x1c }, GSMTAP_RRC_SUB_SysInfoTypeSB2        , 24,  0, 22, make_rrc_downlink_msg },
    { { 0x0d, 0x04, 0x0f,   -1 }, GSMTAP_CHANNEL_SDCCH,           10, 23,  0, make_gsml2_msg },
    { { 0x13, 0x0b, 0x5b,   -1 }, GSMTAP_CHANNEL_CCCH,            18, 23,  0, make_gsml2_msg },
    { { 0x13, 0x23, 0x59,   -1 }, GSMTAP_CHANNEL_BCCH,            18, 23,  0, make_gsml2_msg },
    { { 0x13, 0x23, 0x5b,   -1 }, GSMTAP_CHANNEL_BCCH,            18, 23,  0, make_gsml2_msg },
    { { 0xf1, 0x01, 0x28,   -1 }, GSMTAP_CHANNEL_AGCH,            10, 23,  0, make_gsml2_msg },
    { {   -1,   -1,   -1,   -1 }, 0,                               0,  0,  0, NULL },
  };

int guess_chantype(uint8_t *p)
{
  uint8_t l2len;

  // power control level can have values 30 and 31, but bits 3-5 in
  // lapdm octet 1 are sapi so they can only be 0 or 3
  if((p[0] & 0x1f) >= 30) return GSMTAP_CHANNEL_ACCH | GSMTAP_CHANNEL_SDCCH;
  // bit 1 in octet 1 of lapdm is always set
  if(!(p[0] & 1)) return GSMTAP_CHANNEL_ACCH | GSMTAP_CHANNEL_SDCCH;
  // octet 2 of sacch l1 header can only have values from 0-63 and 127
  if((p[1] >= 64 && p[1] <= 126) || (p[1] & 0x80)) return GSMTAP_CHANNEL_SDCCH;
  // this could either be octet 3 of lapdm or octet 1 of lapdm after
  // the l1 sacch header
  l2len = p[2] >> 2;
  // those are the only values that bits 3-8 of octet 1 could have
  if(l2len != 0 && l2len != 3 &&
     l2len != 8 && l2len != 11) return GSMTAP_CHANNEL_SDCCH;
  // if octet 3 == 1 and octet 4 == 2b, octet 3 could either be octet
  // 3 of lapdm and octet 4 the pd (non-call ss), or octets 3 and 4
  // could be octets 1 and 2 of lapdm. but if octets 5, 6 and 7 are
  // also == 2b, octet 7 cannot be the message type (value not defined
  // for that pd), so it must be padding, so the message started with
  // lapdm
  if(p[2] == 1 && p[3] == 0x2b && p[4] == 0x2b && p[5] == 0x2b && p[6] == 0x2b)
    return GSMTAP_CHANNEL_SDCCH;

  DBG(1,
      printf("** could not guess chantype\n");
      hexdump(p, 23));

  return GSMTAP_CHANNEL_SDCCH;
}

uint8_t *l3_addpadding(uint8_t *p) {
  static uint8_t l3wpad[SEARCHLEN];
  uint32_t l3len;

  // fixme: doesn't work for segmented messages

  l3len = *(uint32_t *)p;
  if(l3len > SEARCHLEN) return NULL;

  memcpy(l3wpad, p + 4, l3len);

  while(l3len < SEARCHLEN)
    l3wpad[l3len++] = '+';

  DBG(2,
      printf("** searching for:\n");
      hexdump(l3wpad, SEARCHLEN));

  return l3wpad;
}

struct msgb *make_rrc_uplink_msg(uint8_t gsmtap_chan, uint8_t *msg_start,
                                 uint16_t msg_len, uint8_t **dummy)
{
  return gsmtap_makemsg_ex(GSMTAP_TYPE_UMTS_RRC, GSMTAP_ARFCN_F_UPLINK, 0,
                           gsmtap_chan, 0, 0, 0, 0, msg_start, msg_len);
}

struct msgb *make_rrc_downlink_msg(uint8_t gsmtap_chan, uint8_t *msg_start,
                                   uint16_t msg_len, uint8_t **dummy)
{
  return gsmtap_makemsg_ex(GSMTAP_TYPE_UMTS_RRC, 0, 0,
                           gsmtap_chan, 0, 0, 0, 0, msg_start, msg_len);
}

struct msgb *make_gsml2_msg(uint8_t gsmtap_chan, uint8_t *msg_start,
                            uint16_t msg_len, uint8_t **dummy)
{
  return gsmtap_makemsg(0, 0, gsmtap_chan, 0, 0, 0, 0, msg_start, msg_len);
}

struct msgb *register_search_msg(uint8_t dummy1, uint8_t *msg_start,
                                 uint16_t dummy2, uint8_t **searchmsg_p)
{
  if(*searchmsg_p)
    DBG(1, printf("** old searchmsg not found\n"));
  if(!(*searchmsg_p = l3_addpadding(msg_start)))
    DBG(2, printf("** couldn't parse l3 search msg (e.g. segmented message)\n"));
  return NULL;
}

struct msgb *parse_radiomsg(uint8_t *data, uint16_t len_msg,
                            struct rmsg_act *ltable, uint8_t **searchmsg_p)
{
  int i = 0;
  uint16_t mlo, mso, len_rmsg;

  if(!(data[0] == 0xff && data[1] == 0xff && data[3] == 0xff)) {
    DBG(2, printf("** message damaged - ignoring\n"));
    return NULL;
  }

  do {

    if(IDMATCH(ltable[i].id[0], data[2]) &&
       IDMATCH(ltable[i].id[1], data[5]) &&
       IDMATCH(ltable[i].id[2], data[7]) &&
       IDMATCH(ltable[i].id[3], data[10])) {

      mso = ltable[i].msg_start_offset;
      mlo = ltable[i].msg_len_offset;

      len_rmsg = mlo ? *(uint16_t *)(data + mlo) : ltable[i].msg_len;

      if(len_rmsg && (mso + len_rmsg > len_msg)) {
        DBG(2, printf("** message too short - ignoring\n"));
        return NULL;
      }

      return ltable[i].action(ltable[i].gsmtap_chan,
                              data + mso,
                              len_rmsg,
                              searchmsg_p);
    }

  } while(ltable[++i].action);

  return NULL;
}

struct msgb *parse_rawmsg(uint8_t *data, uint16_t len_msg, uint8_t **searchmsg_p)
{
  struct msgb *msg = NULL;
  uint8_t *frame_p;

  if(*searchmsg_p &&
     (frame_p = memmem(data, len_msg, *searchmsg_p, SEARCHLEN))) {

    *searchmsg_p = NULL;

    msg = gsmtap_makemsg(0, 0, GSMTAP_CHANNEL_SDCCH,
                          0, 0, 0, 0, frame_p - 3, 23);

  } else if((frame_p = memmem(data, len_msg, "\x25\x06\x06", 3))) {

    msg = gsmtap_makemsg(0, 0, guess_chantype(frame_p + 0x0d),
                          0, 0, 0, 0, frame_p + 0x0d, 23);

  }

  return msg;
}

void send_rmsg(struct gsmtap_inst *gti, struct msgb *rmsg)
{
    DBG(3, printf("** sending gsmtap message\n"));

    if(gsmtap_sendmsg(gti, rmsg) < 0) {
      DBG(0, printf("** message sending failed\n"));
      msgb_free(rmsg);
    }
}

void parse_logmsg(FILE *f, int printlog,
                  struct phone2ltable *p2ltable, struct gsmtap_inst *gti)
{
  uint64_t pos;
  struct xglog_hdr *log_hdr;
  uint8_t *data;
  uint16_t len_msg;
  struct msgb *rmsg;
  static uint8_t *searchmsg = NULL;
  
  pos = ftell(f);

  if(!(log_hdr = read_logmsg(f, p2ltable->unescape)))
    return;

  data = log_hdr->msg.data;
  len_msg = ntohs(log_hdr->len) - sizeof(struct xglog_msg);

  rmsg = NULL;

  switch(log_hdr->type) {

  case 0x01:
    if(printlog) {
      data[len_msg] = 0;
      printf("LOG:>>%s<<\n", data);
    }
    break;

  case 0x04:
    rmsg = parse_radiomsg(data, len_msg, p2ltable->ltable, &searchmsg);
    break;

  case 0x0a:
    rmsg = parse_rawmsg(data, len_msg, &searchmsg);
    break;

  default:
    break;

  }

  if(rmsg) {

    if(searchmsg)
      DBG(2, printf("** logging new msg even though still searching for msg\n"));

    send_rmsg(gti, rmsg);
  }

  DBG(7,
      printf("--header:\n");
      hexdump_addr((uint8_t *) log_hdr, sizeof(struct xglog_hdr), pos);
      printf("--data:\n");
      hexdump_addr(data, ntohs(log_hdr->len), pos + sizeof(struct xglog_hdr)));
}
