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

#include <QtGui/QGuiApplication>
#include <QtGui/QClipboard>
#include <QtGui/QContextMenuEvent>
#include <QtGui/QFocusEvent>
#include <QtGui/QImage>
#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>
#include <QtGui/QPaintEvent>
#include <QtGui/QResizeEvent>
#include <QtWidgets/QMenu>

#include <SimPlot/SimPlotWidget.h>

#include "internal/SinglePlotImpl.h"

////// public ////////////////////////////////////////////////////////////////

SimPlotWidget::SimPlotWidget(QWidget *parent, Qt::WindowFlags f)
  : QWidget(parent, f)
  , _impl()
  , _contextMenu()
  , _panZoom(RectangularZoom)
  , _dragStart()
  , _zoomRect()
{
  _impl = new SinglePlotImpl(SimTheme::themeTextbook(), this);

  setMouseTracking(true);

  initializeContextMenu();
  initializeCursor();
}

SimPlotWidget::~SimPlotWidget()
{
  delete _impl;
}

bool SimPlotWidget::insert(ISimPlotSeriesData *data, const QColor& color)
{
  return _impl->insert(data, color);
}

bool SimPlotWidget::remove(const QString& seriesName)
{
  return _impl->remove(seriesName);
}

void SimPlotWidget::setActiveSeries(const QString& seriesName)
{
  _impl->setActiveSeries(seriesName);
}

void SimPlotWidget::setXTitle(const QString& title)
{
  _impl->setXTitle(title);
}

////// protected /////////////////////////////////////////////////////////////

void SimPlotWidget::changeEvent(QEvent *event)
{
  QWidget::changeEvent(event);
  if( event->type() == QEvent::FontChange ) {
    _impl->update();
  }
}

void SimPlotWidget::contextMenuEvent(QContextMenuEvent *event)
{
  _contextMenu->exec(event->globalPos());
  event->accept();
}

void SimPlotWidget::focusOutEvent(QFocusEvent *event)
{
  initializeCursor();
  QWidget::focusOutEvent(event);
}

void SimPlotWidget::leaveEvent(QEvent *event)
{
  initializeCursor();
  QWidget::leaveEvent(event);
}

void SimPlotWidget::mouseMoveEvent(QMouseEvent *event)
{
  if( event->buttons().testFlag(Qt::LeftButton) ) {
    if( _panZoom == Panning ) {
      const QPointF delta = QPointF(event->pos()) - _dragStart;
      _dragStart = event->pos();
      _impl->pan(delta);
    } else {
      _zoomRect = QRect(_dragStart, event->pos()).normalized();
      update();
    }
  }
  event->accept();
}

void SimPlotWidget::mousePressEvent(QMouseEvent *event)
{
  if( event->buttons() == Qt::LeftButton ) {
    _dragStart = event->pos();
    if( _panZoom == Panning ) {
      setCursor(Qt::ClosedHandCursor);
    }
  }
  event->accept();
}

void SimPlotWidget::mouseReleaseEvent(QMouseEvent *event)
{
  if(        _panZoom == RectangularZoom ) {
    _impl->rectangularZoom(_zoomRect);
  } else if( _panZoom == HorizontalZoom ) {
    _zoomRect.setTop(0);
    _zoomRect.setHeight(height());
    _impl->horizontalZoom(_zoomRect);
  } else if( _panZoom == VerticalZoom ) {
    _zoomRect.setLeft(0);
    _zoomRect.setWidth(width());
    _impl->verticalZoom(_zoomRect);
  } else if( _panZoom == Panning ) {
    initializeCursor();
  }
  _zoomRect = QRect();
  event->accept();
}

void SimPlotWidget::paintEvent(QPaintEvent *event)
{
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing, true);
  painter.setRenderHint(QPainter::TextAntialiasing, true);

  _impl->paint(&painter);

  painter.save();

  if( !_zoomRect.isNull() ) {
    const QRectF r(_zoomRect);

    painter.setPen(_impl->theme().rubberPen());
    if(        _panZoom == RectangularZoom ) {
      painter.drawRect(r.adjusted(0.5, 0.5, -0.5, -0.5));
    } else if( _panZoom == HorizontalZoom ) {
      const QPointF beginL(r.left() + 0.5, 0);
      const QPointF   endL(r.left() + 0.5, height());
      painter.drawLine(beginL, endL);

      const QPointF beginR(r.right() - 0.5, 0);
      const QPointF   endR(r.right() - 0.5, height());
      painter.drawLine(beginR, endR);
    } else if( _panZoom == VerticalZoom ) {
      const QPointF beginT(0,       r.top() + 0.5);
      const QPointF   endT(width(), r.top() + 0.5);
      painter.drawLine(beginT, endT);

      const QPointF beginB(0,       r.bottom() - 0.5);
      const QPointF   endB(width(), r.bottom() - 0.5);
      painter.drawLine(beginB, endB);
    }
  }

  painter.restore();

  event->accept();
}

void SimPlotWidget::resizeEvent(QResizeEvent *event)
{
  _impl->resize(QPointF(0, 0), event->size());

  event->accept();
}

////// public slots //////////////////////////////////////////////////////////

void SimPlotWidget::exportToClipboard()
{
  QImage image(size(), QImage::Format_ARGB32_Premultiplied);
  render(&image);
  QGuiApplication::clipboard()->setImage(image);
}

void SimPlotWidget::panning()
{
  _panZoom = Panning;
  initializeCursor();
}

void SimPlotWidget::horizontalZoom()
{
  _panZoom = HorizontalZoom;
  initializeCursor();
}

void SimPlotWidget::verticalZoom()
{
  _panZoom = VerticalZoom;
  initializeCursor();
}

void SimPlotWidget::rectangularZoom()
{
  _panZoom = RectangularZoom;
  initializeCursor();
}

void SimPlotWidget::reset()
{
  _impl->reset();
}

////// private ///////////////////////////////////////////////////////////////

void SimPlotWidget::initializeContextMenu()
{
  _contextMenu = new QMenu(this);

  // QAction *exportAction =
  _contextMenu->addAction(tr("Clipboard"), this, &SimPlotWidget::exportToClipboard);

  _contextMenu->addSeparator();

  QAction *panningAction =
      _contextMenu->addAction(tr("Pan"), this, &SimPlotWidget::panning,
                              Qt::Key_M);
  addAction(panningAction);

  _contextMenu->addSeparator();

  QAction *horizontalAction =
      _contextMenu->addAction(tr("Horizontal"), this, &SimPlotWidget::horizontalZoom,
                              Qt::Key_H);
  addAction(horizontalAction);

  QAction *verticalAction =
      _contextMenu->addAction(tr("Vertical"), this, &SimPlotWidget::verticalZoom,
                              Qt::Key_V);
  addAction(verticalAction);

  QAction *rectangularAction =
      _contextMenu->addAction(tr("Rectangular"), this, &SimPlotWidget::rectangularZoom,
                              Qt::Key_R);
  addAction(rectangularAction);

  _contextMenu->addSeparator();

  QAction *resetAction =
      _contextMenu->addAction(tr("Reset"), this, &SimPlotWidget::reset,
                              Qt::Key_Escape);
  addAction(resetAction);

  setContextMenuPolicy(Qt::DefaultContextMenu);
}

void SimPlotWidget::initializeCursor()
{
  if( _panZoom == Panning ) {
    setCursor(Qt::OpenHandCursor);
  } else {
    setCursor(Qt::CrossCursor);
  }
}
