/****************************************************************************
** Copyright (c) 2013-2015, Carsten Schmidt. All rights reserved.
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

#ifndef __CSPDFCONTENTSNODE_H__
#define __CSPDFCONTENTSNODE_H__

#include <QtCore/QVariant>

#include <csPDF/cspdf_config.h>

class CS_PDF_EXPORT csPdfContentsNode {
public:
  csPdfContentsNode(const char *title,
                    const int destPage,
                    csPdfContentsNode *parent);
  ~csPdfContentsNode();

  QString title() const;
  int page() const;

  void appendChild(const char *title, const int destPage);
  void appendChild(const csPdfContentsNode *child);
  csPdfContentsNode *child(int row);
  int childCount() const;
  int columnCount() const;
  const csPdfContentsNode *constChild(int row) const;
  QVariant data(int column) const;
  csPdfContentsNode *parent();
  int row() const;

private:
  char *_title;
  csPdfContentsNode *_parent;
  csPdfContentsNode **_children;
  int _page;
  int _numChildren;
};

#endif // __CSPDFCONTENTSNODE_H__
