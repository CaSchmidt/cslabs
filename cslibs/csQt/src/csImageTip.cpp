/****************************************************************************
** Copyright (c) 2013-2014, Carsten Schmidt. All rights reserved.
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

#include <QtWidgets/QApplication>
#include <QtWidgets/QDesktopWidget>
#include <QtGui/QImage>
#include <QtGui/QPainter>
#include <QtGui/QPaintEvent>
#include <QtGui/QPixmap>
#include <QtCore/QPoint>
#include <QtWidgets/QWidget>
#include <QtWidgets/5.2.1/QtWidgets/private/qeffects_p.h>

#include "csQt/csImageTip.h"

////// Private ///////////////////////////////////////////////////////////////

class csTipWidget : public QWidget {
  Q_OBJECT
public:
  csTipWidget(const QImage& image,
              const csImageTip::Flags flags = csImageTip::NoFlags,
              QWidget *parent = 0, Qt::WindowFlags winFlags = 0);
  ~csTipWidget();

  static csTipWidget *instance;

  bool eventFilter(QObject *watched, QEvent *event);
  void hideTip();
  void placeTip(const QPoint& globalPos, QWidget *widget);
  static int tipScreen(const QPoint& globalPos, QWidget *widget);

protected:
  void paintEvent(QPaintEvent *event);

private:
  QPixmap pixmap;
};

csTipWidget *csTipWidget::instance = 0;

csTipWidget::csTipWidget(const QImage& image,
                         const csImageTip::Flags flags,
                         QWidget *parent, Qt::WindowFlags)
  : QWidget(parent,
            Qt::ToolTip | Qt::BypassGraphicsProxyWidget | Qt::FramelessWindowHint),
    pixmap()
{
  delete instance;
  instance = this;

  setAttribute(Qt::WA_TranslucentBackground, true);

  pixmap = QPixmap::fromImage(image);
  if( flags.testFlag(csImageTip::DrawBorder) ) {
    QPainter painter(&pixmap);
    painter.setBackgroundMode(Qt::TransparentMode);
    painter.setBackground(QBrush(Qt::NoBrush));
    painter.setBrush(QBrush(Qt::NoBrush));
    painter.setPen(QPen(QBrush(Qt::black, Qt::SolidPattern),
                        0.0, Qt::SolidLine));
    painter.drawRect(0, 0, image.width()-1, image.height()-1);
  }

  resize(pixmap.width(), pixmap.height());

  setMouseTracking(true);

  QApplication::instance()->installEventFilter(this);
}

csTipWidget::~csTipWidget()
{
  instance = 0;
}

bool csTipWidget::eventFilter(QObject *, QEvent *event)
{
  switch( event->type() ) {
  // NOTE: If the Image ToolTip is Placed with ZERO Offset to the
  //       Mouse's Cursor, the Image Will be immediately Hidden
  //       Due to FocusIn/Out, Leave, WindowActivate/Deactivate
  //       Events Passed to This Widget.
  case QEvent::FocusIn:
  case QEvent::FocusOut:
  case QEvent::KeyPress:
  case QEvent::KeyRelease:
  case QEvent::Leave:
  case QEvent::MouseButtonDblClick:
  case QEvent::MouseButtonPress:
  case QEvent::MouseButtonRelease:
  case QEvent::MouseMove:
  case QEvent::Wheel:
  case QEvent::WindowActivate:
  case QEvent::WindowDeactivate:
    hideTip();
    break;
  }

  return false;
}

void csTipWidget::hideTip()
{
  close();
  deleteLater();
}

void csTipWidget::placeTip(const QPoint& globalPos, QWidget *widget)
{
  const QPoint offset(8, 8);

  QPoint placePos(offset+globalPos);

  const QRect screenRect =
      QApplication::desktop()->screenGeometry(tipScreen(globalPos, widget));

  if( placePos.x() + width() > screenRect.right()  &&
      width() < screenRect.width() ) {
    placePos.rx() = globalPos.x() - offset.x() - width();
  }

  if( placePos.y() + height() > screenRect.bottom()  &&
      height() < screenRect.height() ) {
    placePos.ry() = globalPos.y() - offset.y() - height();
  }

  move(placePos);
}

void csTipWidget::paintEvent(QPaintEvent *event)
{
  QPainter painter(this);
  painter.setCompositionMode(QPainter::CompositionMode_Source);
  painter.drawPixmap(0, 0, pixmap);

  event->accept();
}

int csTipWidget::tipScreen(const QPoint& globalPos, QWidget *widget)
{
  if( QApplication::desktop()->isVirtualDesktop() ) {
    return QApplication::desktop()->screenNumber(globalPos);
  }

  return QApplication::desktop()->screenNumber(widget);
}

////// Public ////////////////////////////////////////////////////////////////

void csImageTip::showImage(const QPoint& globalPos, const QImage& image,
                           QWidget *widget,
                           const csImageTip::Flags flags)
{
#ifndef Q_WS_WIN
  new csTipWidget(image, flags, widget);
#else
  new csTipWidget(image, flags,
                  QApplication::desktop()->screen(csTipWidget::tipScreen(globalPos,
                                                                         widget)));
#endif

  if( flags.testFlag(ForcePosition) ) {
    csTipWidget::instance->move(globalPos);
  } else {
    csTipWidget::instance->placeTip(globalPos, widget);
  }
  csTipWidget::instance->setObjectName(QLatin1String("csTipWidget"));

#if !defined(QT_NO_EFFECTS) && !defined(Q_WS_MAC)
# if 1
  qFadeEffect(csTipWidget::instance);
# else
  if (QApplication::isEffectEnabled(Qt::UI_FadeTooltip)) {
    qFadeEffect(csTipWidget::instance);
  } else if (QApplication::isEffectEnabled(Qt::UI_AnimateTooltip)) {
    qScrollEffect(csTipWidget::instance);
  } else {
    csTipWidget::instance->show();
  }
# endif
#else
  csTipWidget::instance->show();
#endif
}

#include "csImageTip.moc"
