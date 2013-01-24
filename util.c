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

#include <stdint.h>
#include <stdio.h>

void hexdump_addr(uint8_t *buf, int len, uint32_t addr_start)
{
  int i;
  uint8_t str[17];
  str[16] = 0;
  
  for(i = 0; i < len; i++) {

    if(!(i % 16)) printf("%08x  ", i + addr_start);

    str[i % 16] = ((buf[i] >= 32 && buf[i] <= 126) ? buf[i] : '.');
    printf("%02x ", buf[i]);

    if(!((i+1) % 8)) {
      if(!((i+1) % 16))
        printf(" %s\n", str);
      else
        printf(" ");
    }
  }

  i = 16 - (i % 16);
  str[16 - i] = 0;
  if(i > 0 && i < 16) {
    if(i > 8) printf(" ");
    while(i--)
      printf("   ");
    printf(" %s\n", str);
  }
}
