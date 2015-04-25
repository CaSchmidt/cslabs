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

#include "internal/fpdf_util.h"

#include <csPDFium/csPDFiumUtil.h>

#include "internal/config.h"

#define TEXT_COMMIT()       \
  if( !text.isEmpty() ) {   \
    texts.push_back(text);  \
    text.clear();           \
  }

namespace util {

  namespace priv {

    bool touches(const QRectF& a, const QRectF& b, const qreal tol)
    {
      return csPDFium::overlapsH(a, b, tol) && csPDFium::overlapsV(a, b, tol);
    }

  }; // namespace priv

  csPDFiumTexts extractTexts(const FPDF_PAGE page, const QMatrix& ctm)
  {
    const FPDF_TEXTPAGE textPage = FPDFText_LoadPage(page);
    if( textPage == NULL ) {
      return csPDFiumTexts();
    }

    csPDFiumTexts texts;
    csPDFiumText text;
    for(int i = 0; i < FPDFText_CountChars(textPage); i++) {
      const QChar c = QChar(FPDFText_GetUnicode(textPage, i));
      if( c.isNull()  ||  c.isSpace() ) {
        TEXT_COMMIT();
        continue;
      }

      double left, top, right, bottom;
      FPDFText_GetCharBox(textPage, i, &left, &right, &bottom, &top);

      const QPointF topLeft     = QPointF(left,  top)   *ctm;
      const QPointF bottomRight = QPointF(right, bottom)*ctm;
      const QRectF r(topLeft, bottomRight);

#ifndef OLD_EXTRACT
      text.merge(r, c);
#else
      if( text.isEmpty() ) {
        text = r;
        text = c;
        continue;
      }

      if( priv::touches(text.rect(), r, MERGE_TOL) ) {
        text.merge(r, c);
      } else {
        TEXT_COMMIT();
        text = r;
        text = c;
      }
#endif
    }
    TEXT_COMMIT();

#ifdef OLD_EXTRACT
    qSort(texts);
#endif

    FPDFText_ClosePage(textPage);

    return texts;
  }

}; // namespace util
