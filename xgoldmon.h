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

#ifndef XGOLDMON_H
#define XGOLDMON_H

#include <stdint.h>
#include <stdio.h>

#include <osmocom/core/msgb.h>
#include <osmocom/core/gsmtap.h>
#include <osmocom/core/gsmtap_util.h>

struct xglog_msg {
  uint16_t len;
  uint8_t unknown[14];
  uint8_t seqno;
  uint8_t data[];
} __attribute__((packed));

struct xglog_hdr {
  uint8_t type;
  uint8_t seqno;
  uint16_t len;
  uint8_t unknown;
  struct xglog_msg msg;
} __attribute__((packed));

struct rmsg_act {
  // id contains values for data indices 2, 5, 7 and 12. -1 indicates
  // "don't care"
  int16_t id[4];
  uint8_t gsmtap_chan;
  uint16_t msg_start_offset;
  uint16_t msg_len;
  uint16_t msg_len_offset;
  struct msgb *(*action)(uint8_t gsmtap_chan,
                         uint8_t *msg_start,
                         uint16_t msg_len,
                         uint8_t **searchmsg_p);
};

struct phone2ltable {
  char *ptype;
  struct rmsg_act *ltable;
  int unescape;
};

#define hexdump(buf, len) hexdump_addr(buf, len, 0)
#define DBG(level, statement) do { if(dbglevel >= level) { statement; } } while(0)
#define IDMATCH(vt, vd) ((vt) == -1 || (vt) == (vd))

// fixme: doesn't work for SACCH messages (too long)
#define SEARCHLEN 20 // 23 - 3 bytes lapdm header

#define READLOG_UNESCAPE 1

extern struct rmsg_act s3_ltable[];
extern struct rmsg_act s2_ltable[];
extern struct phone2ltable p2t[];
extern int dbglevel;

void hexdump_addr(uint8_t *buf, int len, uint32_t addr_start);
void parse_logmsg(FILE *f, int printlog, struct phone2ltable *ltable, struct gsmtap_inst *gti);
struct xglog_hdr *read_logmsg(FILE *f, int unescape);

#endif
