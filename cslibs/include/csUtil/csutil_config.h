/****************************************************************************
** Copyright (c) 2018, Carsten Schmidt. All rights reserved.
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

#ifndef __CSUTIL_CONFIG_H__
#define __CSUTIL_CONFIG_H__

////// Compiler Configuration ////////////////////////////////////////////////

#ifdef _MSC_VER
# define CS_DECL_EXPORT  __declspec(dllexport)
# define CS_DECL_IMPORT  __declspec(dllimport)
#else
# define CS_DECL_EXPORT
# define CS_DECL_IMPORT
#endif

////// Build Configuration ///////////////////////////////////////////////////

#if defined(CS_BUILD_UTIL_LIB)
# define CS_UTIL_EXPORT  CS_DECL_EXPORT
#else
# define CS_UTIL_EXPORT  CS_DECL_IMPORT
#endif

////// Feature Configuration /////////////////////////////////////////////////

#undef CS_FLAGS_HAVE_VALUE_OPERATORS

#undef CS_FROM_CHARS_HAVE_CAPS
#undef CS_FROM_CHARS_HAVE_PLUS

#define CS_TO_CHARS_HAVE_REMOVE_TRAILING_ZEROS

#endif // __CSUTIL_CONFIG_H__
