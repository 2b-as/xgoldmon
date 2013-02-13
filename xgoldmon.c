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
#include <unistd.h>
#include <string.h>

#include "xgoldmon.h"

int dbglevel = 0;

struct phone2ltable p2t[] =
  {
    { "s3", s3_ltable, READLOG_UNESCAPE },
    { "note2", note2_ltable, READLOG_UNESCAPE },
    { "s2", s2_ltable, 0 },
  };

struct gsmtap_inst *init_gsmtap()
{
  struct gsmtap_inst *gti;

  gti = gsmtap_source_init("127.0.0.1", GSMTAP_UDP_PORT, 0);
  gsmtap_source_add_sink(gti);  

  return gti;
}

void usage(char *cmdname)
{
  printf("usage: %s [-t <phone type>] [-l] [-v] <logfile or device>\n"
         "  -t: select 'note2', 's2', or 's3' (default: '%s')\n"
         "  -l: print baseband log messages\n"
         "  -v: show debugging messages (more than once for more messages)\n",
         cmdname, p2t[0].ptype);
  exit(EXIT_SUCCESS);
}

void parse_cmdline(int argc, char *argv[],
                   int *printlog, struct phone2ltable **p2ltable, FILE **logfile)
{
  int ret, i = 0;
  extern char *optarg;
  extern int optind;

  *p2ltable = NULL;

  while((ret = getopt(argc, argv, "lvht:")) != -1) {
    switch(ret) {
    case 'l':
      *printlog = 1;
      break;
    case 'v':
      dbglevel++;
      break;
    case 't':
      for(i = 0; i < (sizeof(p2t) / sizeof(struct phone2ltable)); i++) {
        if(!strcmp(p2t[i].ptype, optarg)) {
          *p2ltable = &p2t[i];
          break;
        }
      }
      if(!*p2ltable)
        usage(argv[0]);
      break;
    case 'h':
    default:
      usage(argv[0]);
      break;
    }
  }
  if(!*p2ltable)
    *p2ltable = &p2t[0];

  if(argc <= optind)
    usage(argv[0]);

  *logfile = fopen(argv[optind], "r");
  if(*logfile == NULL) {
    perror(NULL);
    exit(EXIT_FAILURE);
  }
}

int main(int argc, char *argv[])
{
  int printlog = 0;
  struct phone2ltable *p2ltable = NULL;
  FILE *f;
  struct gsmtap_inst *gti;

  parse_cmdline(argc, argv, &printlog, &p2ltable, &f);

  gti = init_gsmtap();

  while(1)
    parse_logmsg(f, printlog, p2ltable, gti);
}
