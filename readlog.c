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

#include <stdlib.h>
#include <arpa/inet.h>

#include "xgoldmon.h"

int readlog_unescape;

uint8_t read_byte(FILE *f)
{
  uint8_t byte;
  int rc = fread(&byte, 1, 1, f);

  if(rc != 1) {
    if(feof(f)) {
      DBG(2, printf("** EOF\n"));
      exit(0);
    }
    if(ferror(f)) {
      DBG(0, perror(NULL));
      exit(1);
    }
    exit(0);
  }

  return byte;
}

void read_log(FILE *f, uint8_t *buf, uint16_t size)
{
  while(size--) {

    *buf = read_byte(f);

    if(readlog_unescape && (*buf == 0x7d))
      *buf = (read_byte(f) & 0x0f) | 0x70;

    buf++;
  }
}

int find_pattern(uint8_t *pat, int len_pat, FILE *f)
{
  uint8_t *p = pat;
  uint8_t byte;
  int c_read = 0;

  while(1) {

    read_log(f, &byte, 1);
    c_read++;

    if(byte == *p) {

      p++;

      if(p - pat == len_pat)
        return c_read;

    } else if(p != pat) {

      p = pat;
      if(byte == *p)
        p++;

    }
  }
}

struct xglog_hdr *read_logmsg(FILE *f, int unescape)
{
  uint8_t magic_word[] = { 0xf9, 0x02 };
  char *log_err = NULL;
  int ebytes, llvl;
  static int counter = 0;
  static uint8_t buf[sizeof(struct xglog_hdr) + 0xffff + 1];
  struct xglog_hdr *log_hdr = (struct xglog_hdr *) buf;
  uint16_t len;
  static uint8_t seqno_prev = 0;

  readlog_unescape = unescape;

  ebytes = find_pattern(magic_word, sizeof(magic_word), f);
  counter++;

  read_log(f, buf, sizeof(struct xglog_hdr));

  len = ntohs(log_hdr->len);

  if(len != ntohs(log_hdr->msg.len))
    log_err = "length fields mismatch";
  else if(len > 10000)
    log_err = "too long";
  else if(len <= sizeof(struct xglog_msg))
    log_err = "too short";
  else if(log_hdr->seqno != log_hdr->msg.seqno)
    log_err = "seqno mismatch";
  else {
    read_log(f, log_hdr->msg.data, len - sizeof(struct xglog_msg));
  }

  llvl = (log_err ? 2 : 5);
  DBG(llvl, printf("#%6d type: %3d, seq#: %3d, len: %5d",
                   counter, log_hdr->type, log_hdr->seqno, len));

  if(ebytes - sizeof(magic_word))
    DBG(llvl, printf("  ** %d extraneous bytes before msg", (int)(ebytes - sizeof(magic_word))));

  if(++seqno_prev != log_hdr->seqno)
    DBG(llvl, printf("  ** msg out of sequence"));
  seqno_prev = log_hdr->seqno;

  if(log_err)
    DBG(llvl, printf("  ** %s - ignoring msg", log_err));

  DBG(llvl, printf("\n"));

  return (log_err ? NULL : log_hdr);
}

