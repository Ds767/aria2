/* <!-- copyright */
/*
 * aria2 - a simple utility for downloading files faster
 *
 * Copyright (C) 2006 Tatsuhiro Tsujikawa
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
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
/* copyright --> */
#include "PreAllocationDiskWriter.h"
#include "DlAbortEx.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

PreAllocationDiskWriter::PreAllocationDiskWriter(long long int totalLength)
  :AbstractDiskWriter(),totalLength(totalLength) {}

PreAllocationDiskWriter::~PreAllocationDiskWriter() {}

void PreAllocationDiskWriter::initAndOpenFile(string filename) {
  createFile(filename);
  
  int bufSize = 4096;
  char buf[4096];

  memset(buf, 0, bufSize);
  long long int x = totalLength/bufSize;
  int r = totalLength%bufSize;
  for(long long int i = 0; i < x; i++) {
    if(write(fd, buf, bufSize) < 0) {
      throw new DlAbortEx(strerror(errno));
    }
  }
  if(r > 0) {
    seek(totalLength-r);
    if(write(fd, buf, r) < 0) {
	throw new DlAbortEx(strerror(errno));
    }
  }
}
