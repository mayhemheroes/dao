/*
// This file is part of the virtual machine for the Dao programming language.
// Copyright (C) 2006-2012, Limin Fu. Email: daokoder@gmail.com
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of this 
// software and associated documentation files (the "Software"), to deal in the Software 
// without restriction, including without limitation the rights to use, copy, modify, merge, 
// publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons 
// to whom the Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies or 
// substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING 
// BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef DAO_STREAM_H
#define DAO_STREAM_H

#include<stdio.h>
#include<wchar.h>

#include"daoType.h"

#define IO_BUF_SIZE  512

enum
{
	DAO_IO_FILE = 1 ,
	DAO_IO_PIPE = 2 ,
	DAO_IO_STRING = 4 ,
	DAO_IO_READ = 1 ,
	DAO_IO_WRITE = 2
};

typedef struct DFile
{
	FILE   *fd;
	int     rc;
} DFile;

struct DaoStream
{
	DAO_DATA_COMMON;

	char        attribs;
	int         mode;
	int         useQuote;
	char       *format;
	DFile      *file;
	DString    *streamString;
	DString    *fname;

	DaoUserStream *redirect;
};

DAO_DLL DaoStream* DaoStream_New();
DAO_DLL void DaoStream_Delete( DaoStream *self );
DAO_DLL void DaoStream_Close( DaoStream *self );
DAO_DLL void DaoStream_Flush( DaoStream *self );

DAO_DLL void DaoStream_SetColor( DaoStream *self, const char *fgcolor, const char *bgcolor );

DAO_DLL void DaoStream_WriteChar( DaoStream *self, char val );
DAO_DLL void DaoStream_WriteInt( DaoStream *self, daoint val );
DAO_DLL void DaoStream_WriteFloat( DaoStream *self, double val );
DAO_DLL void DaoStream_WriteString( DaoStream *self, DString *val );
DAO_DLL void DaoStream_WriteMBS( DaoStream *self, const char *val );
DAO_DLL void DaoStream_WriteWCS( DaoStream *self, const wchar_t *val );
DAO_DLL void DaoStream_WritePointer( DaoStream *self, void *val );
DAO_DLL void DaoStream_WriteFormatedInt( DaoStream *self, daoint val, const char *format );
DAO_DLL void DaoStream_WriteNewLine( DaoStream *self );

DAO_DLL int DaoStream_ReadLine( DaoStream *self, DString *buf );
DAO_DLL int DaoFile_ReadLine( FILE *fin, DString *line );
DAO_DLL int DaoFile_ReadAll( FILE *fin, DString *all, int close );

DAO_DLL int Dao_IsFile( const char *file );
DAO_DLL int Dao_IsDir( const char *file );

#endif
