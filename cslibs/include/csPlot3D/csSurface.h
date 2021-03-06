/****************************************************************************
** Copyright (c) 2014-2015, Carsten Schmidt. All rights reserved.
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

#ifndef __CSSURFACE_H__
#define __CSSURFACE_H__

#include <QtCore/QVector>
#include <QtGui/QImage>
#include <QtGui/QOpenGLBuffer>
#include <QtGui/QOpenGLFunctions>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLTexture>

#include <csPlot3D/csplot3d_config.h>
#include <csPlot3D/csMeshInfo.h>

class CS_PLOT3D_EXPORT csSurface : protected QOpenGLFunctions {
public:
  csSurface();
  ~csSurface();

  void draw(QOpenGLShaderProgram& program);
  void drawMesh(QOpenGLShaderProgram& program);

  void setData(const QVector<float>& x,
               const QVector<float>& y,
               const QVector<float>& z);

private:
  void initialize();
  void updateColorImage();
  void updateModelMatrix();

  bool              _initRequired;
  QVector<float>    _paletteAxis;
  QVector<QColor>   _palette;
  csMeshInfo        _meshInfo;
  QVector<GLfloat>  _surfaceData;
  QOpenGLBuffer    *_surface;
  QVector<GLuint>   _stripData;
  QOpenGLBuffer    *_strip;
  QVector<GLuint>   _meshYData;
  QOpenGLBuffer    *_meshY;
  QMatrix4x4        _model;
  QImage            _colorImage;
  QOpenGLTexture   *_colorTexture;
};

#endif // __CSSURFACE_H__
