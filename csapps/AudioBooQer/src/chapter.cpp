/****************************************************************************
** Copyright (c) 2014, Carsten Schmidt. All rights reserved.
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

#include <QtCore/QFileInfo>

#include "chapter.h"

////// Global ////////////////////////////////////////////////////////////////

bool isRoot(csITreeItem *item)
{
  return dynamic_cast<ChapterRoot*>(item) != 0;
}

bool isNode(csITreeItem *item)
{
  return dynamic_cast<ChapterNode*>(item) != 0;
}

bool isFile(csITreeItem *item)
{
  return dynamic_cast<ChapterFile*>(item) != 0;
}

////// ChapterRoot ///////////////////////////////////////////////////////////

ChapterRoot::ChapterRoot()
  : csITreeItem(0)
{
}

ChapterRoot::~ChapterRoot()
{
  qDeleteAll(_children);
}

bool ChapterRoot::insert(ChapterNode *node)
{
  if(         _children.isEmpty()  &&  !node->isSource() ) {
    return false;
  } else if( !_children.isEmpty()  &&   node->isSource() ) {
    return false;
  }

  if( node->isSource() ) {
    _children.push_back(node);
  } else {
    _children.insert(_children.size()-1, node);
  }

  return true;
}

int ChapterRoot::columnCount() const
{
  return 1;
}

QVariant ChapterRoot::data(int /*column*/) const
{
  return QVariant();
}

////// ChapterNode ///////////////////////////////////////////////////////////

ChapterNode::ChapterNode(csITreeItem *parent, const bool isSource)
  : csITreeItem(parent)
  , _isSource(isSource)
  , _title()
{
}

ChapterNode::~ChapterNode()
{
  qDeleteAll(_children);
}

bool ChapterNode::isSource() const
{
  return _isSource;
}

QStringList ChapterNode::files(const int count) const
{
  QStringList list;
  for(int i = 0; i < qMin(count, _children.size()); i++) {
    ChapterFile *file = dynamic_cast<ChapterFile*>(_children[i]);
    list.push_back(file->fileName());
  }
  return list;
}

int ChapterNode::remove(const int count)
{
  if( !_isSource ) {
    return 0;
  }

  int cntRemoved(0);
  const int number = qMin(count, _children.size());
  for(int i = 0; i < number; i++) {
    delete _children.takeFirst();
    cntRemoved++;
  }
  return cntRemoved;
}

int ChapterNode::setFiles(const QStringList& fileNames)
{
  QStringList names = fileNames;
  qSort(names);

  int cntAdded(0);
  foreach (const QString& name, names) {
    ChapterFile *file = new ChapterFile(this);
    _children.push_back(file);
    file->setFileName(name);
  }
  return cntAdded;
}

const QString& ChapterNode::title() const
{
  return _title;
}

void ChapterNode::setTitle(const QString& title)
{
  _title = title;
}

int ChapterNode::columnCount() const
{
  return 1;
}

QVariant ChapterNode::data(int /*column*/) const
{
  return _title;
}

////// ChapterItem ///////////////////////////////////////////////////////////

ChapterFile::ChapterFile(csITreeItem *parent)
  : csITreeItem(parent)
  , _fileName()
{
}

ChapterFile::~ChapterFile()
{
  qDeleteAll(_children);
}

const QString& ChapterFile::fileName() const
{
  return _fileName;
}

void ChapterFile::setFileName(const QString& fileName)
{
  _fileName = fileName;
}

int ChapterFile::columnCount() const
{
  return 1;
}

QVariant ChapterFile::data(int /*column*/) const
{
  return QFileInfo(_fileName).fileName();
}
