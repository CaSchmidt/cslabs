/****************************************************************************
** Copyright (c) 2017, Carsten Schmidt. All rights reserved.
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

#ifndef __IPLOTIMPLEMENTATION_H__
#define __IPLOTIMPLEMENTATION_H__

#include <SimPlot/ISimPlotSeriesData.h>
#include <SimPlot/SimTheme.h>

#include "internal/IPlotElement.h"

class IPlotImplementation : public IPlotElement {
public:
  IPlotImplementation(const SimTheme& theme);
  ~IPlotImplementation();

  SimTheme& theme();
  const SimTheme& theme() const;

  virtual bool insert(ISimPlotSeriesData *data, const QColor& color) = 0;

  virtual bool remove(const QString& seriesName) = 0;

  virtual const Widget *widget() const = 0;

  virtual void reset() = 0;

  virtual void update() = 0;

  virtual void setActiveSeries(const QString& seriesName) = 0;

  virtual void setXTitle(const QString& title) = 0;

  virtual SimRange rangeX() const = 0;

  virtual QTransform mapToScreen() const = 0;

  virtual void rectangularZoom(const QRectF& zoomRect) = 0;

  virtual void horizontalZoom(const QRectF& zoomRect) = 0;

  virtual void verticalZoom(const QRectF& zoomRect) = 0;

  virtual void pan(const QPointF& delta) = 0;

private:
  SimTheme _theme;
};

#endif // __IPLOTIMPLEMENTATION_H__
