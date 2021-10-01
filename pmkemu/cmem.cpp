/*
 * Emulator Copyright (c) 2012 Felix Lazarev <me@felixl.com>
 *
 * ROM Content Copyright (c) 198x Trokhimenko Y.K. and others
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 
#include "cmem.h"

cMem::cMem()
{
    int i;
    for(i=0;i<MEM_BITLEN;i++)
        memarray[i]=false;
}

bool cMem::pretick()
{
    return memarray[0];
}

bool cMem::tick(bool rm)
{
    bool ret;
    int i;

    ret=memarray[0];

    for(i=0;i<(MEM_BITLEN-1);i++)
        memarray[i]=memarray[i+1];
    memarray[MEM_BITLEN-1]=rm;

    return ret;
}
