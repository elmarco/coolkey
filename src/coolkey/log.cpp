/* ***** BEGIN COPYRIGHT BLOCK *****
 * Copyright (C) 2005 Red Hat, Inc.
 * All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation version
 * 2.1 of the License.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 * ***** END COPYRIGHT BLOCK *****/

#include <string>
#include "mypkcs11.h"
#include <assert.h>
#include <stdio.h>
#include "log.h"
#include <cstdarg>
#include "PKCS11Exception.h"


FileLog::FileLog(const char *filename )
{
    file = fopen(filename, "at");
    if( file == NULL ) {
        throw PKCS11Exception(CKR_GENERAL_ERROR, "Failed to open logfile");
    }
}

void
FileLog::log(const char *msg, ...)
{
    va_list ap;

    va_start(ap, msg);

    vfprintf(file, msg, ap);
    fflush(file);

    va_end(ap);
}

#define ROW_LENGTH 16 
// dump the contents of an CKYBuffer, both in hex and  'ascii'.
// change ROW_LENGTH above to change how many bytes are printed on a single
// line
void
Log::dump(CKYBuffer *buf)
{
    CKYSize i;
    CKYSize size = CKYBuffer_Size(buf);
    char string[ROW_LENGTH+1];
    char *bp = &string[0];
    CKYByte c;

    for (i=0; i < size; i++) {
	if (i && ((i % (ROW_LENGTH-1)) == 0) ) {
	    *bp = 0;
	    log(" %s\n",string);
	    bp = &string[0];
	}
	c = CKYBuffer_GetChar(buf, i);
	log("%02x ",c);
	*bp++ =  (c < ' ') ? '.' : ((c & 0x80) ? '*' : c);
    }
    *bp = 0;
    for (i= (i % (ROW_LENGTH-1)); i && (i < ROW_LENGTH); i++) {
	log("   ");
    }
    log(" %s\n",string);
}

FileLog::~FileLog()
{
    fclose(file);
}
