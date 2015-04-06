/****************************************************************************
** Copyright (c) 2015, Carsten Schmidt. All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions
** are met:
**
** 1. Redistributions of source code must retain the above copyright
**    notice, this list of conditions and the following disclaimer.
**
** 2. Redistributions in binary form must reproduce the above copyright
**    notice, this list of conditions and the following disclaimer in the
**    documentation and/or other materials provided with the distribution.
**
** 3. Neither the name of the copyright holder nor the names of its
**    contributors may be used to endorse or promote products derived from
**    this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************/

#include "command.h"

#include <csCore/csAlloc.h>
#include "clipboard.h"
#include "reg.h"
#include "util.hpp"

bool executeCommand(const UINT cmd, const csStringList& files)
{
  const DWORD flags   = regReadFlags();
  const bool  isBatch = testFlags(flags, CMD_FLAG_BATCH);
  const bool  isUnc   = testFlags(flags, CMD_FLAG_UNC)  &&  cmd != Cmd_List;
  const bool  isUnix  = testFlags(flags, CMD_FLAG_UNIX);

  if(        cmd == Cmd_List                ||
             cmd == Cmd_ListWithPath        ||
             cmd == Cmd_ListWithPathTabular ) {
    int size = 0;
    for(int i = 0; i < files.count(); i++) {
      wchar_t *uncName = 0;
      if( isUnc  &&  (uncName = resolveUNC(files[i])) != 0 ) {
        size += lenFN(csString(uncName, false), cmd);
      } else {
        size += lenFN(files[i], cmd);
      }
    }

    wchar_t *text = (wchar_t*)csAlloc((size+1)*sizeof(wchar_t));
    if( text == 0 ) {
      return false;
    }

    int pos = 0;
    for(int i = 0; i < files.count(); i++) {
      wchar_t *uncName = 0;
      if( isUnc  &&  (uncName = resolveUNC(files[i])) != 0 ) {
        catFN(text, pos, csString(uncName, false), cmd);
      } else {
        catFN(text, pos, files[i], cmd);
      }
    }
    text[size] = L'\0';

    if( isUnix ) {
      replace(text, size, L'\\', L'/');
    }

    setClipboardText(text);
    csFree(text);

    return true;

  } else if( cmd == Cmd_CheckBatchProcessing ) {
    regWriteFlags(flags ^ CMD_FLAG_BATCH);
    return true;

  } else if( cmd == Cmd_CheckResolveUncPaths ) {
    regWriteFlags(flags ^ CMD_FLAG_UNC);
    return true;

  } else if( cmd == Cmd_CheckUnixPathSeparators ) {
    regWriteFlags(flags ^ CMD_FLAG_UNIX);
    return true;
  }

  return false;
}
