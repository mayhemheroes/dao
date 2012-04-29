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

#ifndef DAO_STDTYPE_H
#define DAO_STDTYPE_H

#include"daoConst.h"
#include"daoBase.h"
#include"daoString.h"
#include"daoArray.h"
#include"daoMap.h"

#define DAO_DATA_CORE    uchar_t type, subtype, trait, marks; int refCount
#define DAO_DATA_COMMON  DAO_DATA_CORE; int cycRefCount

void DaoValue_Init( void *dbase, char type );

struct DaoNone
{
	DAO_DATA_CORE;
};
DAO_DLL extern DaoValue *dao_none_value;
DAO_DLL DaoNone* DaoNone_New();

struct DaoInteger
{
	DAO_DATA_CORE;

	daoint value;
};
struct DaoFloat
{
	DAO_DATA_CORE;

	float value;
};
struct DaoDouble
{
	DAO_DATA_CORE;

	double value;
};
struct DaoComplex
{
	DAO_DATA_CORE;

	complex16 value;
};
struct DaoLong
{
	DAO_DATA_CORE;

	DLong  *value;
};
DAO_DLL DaoLong* DaoLong_Copy( DaoLong *self );
DAO_DLL void DaoLong_Delete( DaoLong *self );

struct DaoString
{
	DAO_DATA_CORE;

	DString  *data;
};
DAO_DLL DaoString* DaoString_Copy( DaoString *self );
DAO_DLL void DaoString_Delete( DaoString *self );

/* Structure for symbol, enum and flag:
 * Storage modes:
 * Symbol: $AA => { type<$AA>, 0 }
 * Symbols: $AA + $BB => { type<$AA$BB>, 1|2 }
 * Enum: enum MyEnum{ AA=1, BB=2 }, MyEnum.AA => { type<MyEnum>, 1 }
 * Flag: enum MyFlag{ AA=1; BB=2 }, MyFlag.AA + MyFlag.BB => { type<MyFlag>, 1|2 }
 */
struct DaoEnum
{
	DAO_DATA_COMMON;

	int       value; /* value associated with the symbol(s) or flag(s) */
	DaoType  *etype; /* type information structure */
};

DAO_DLL DaoEnum* DaoEnum_New( DaoType *type, int value );
DAO_DLL DaoEnum* DaoEnum_Copy( DaoEnum *self, DaoType *type );
DAO_DLL void DaoEnum_Delete( DaoEnum *self );
DAO_DLL void DaoEnum_MakeName( DaoEnum *self, DString *name );
DAO_DLL void DaoEnum_SetType( DaoEnum *self, DaoType *type );
DAO_DLL int DaoEnum_SetSymbols( DaoEnum *self, const char *symbols );
DAO_DLL int DaoEnum_SetValue( DaoEnum *self, DaoEnum *other, DString *enames );
DAO_DLL int DaoEnum_AddValue( DaoEnum *self, DaoEnum *other, DString *enames );
DAO_DLL int DaoEnum_RemoveValue( DaoEnum *self, DaoEnum *other, DString *enames );
DAO_DLL int DaoEnum_AddSymbol( DaoEnum *self, DaoEnum *s1, DaoEnum *s2, DaoNamespace *ns );
DAO_DLL int DaoEnum_SubSymbol( DaoEnum *self, DaoEnum *s1, DaoEnum *s2, DaoNamespace *ns );

struct DaoList
{
	DAO_DATA_COMMON;

	DArray    items;
	DaoType  *unitype;
};

DAO_DLL DaoList* DaoList_New();
DAO_DLL void DaoList_Delete( DaoList *self );
DAO_DLL void DaoList_Clear( DaoList *self );

DAO_DLL void DaoList_Erase( DaoList *self, daoint id );
DAO_DLL int DaoList_SetItem( DaoList *self, DaoValue *it, daoint id );
DAO_DLL int DaoList_Append( DaoList *self, DaoValue *it );

DAO_DLL DaoList* DaoList_Copy( DaoList *self, DMap *cycdata );

struct DaoMap
{
	DAO_DATA_COMMON;

	DMap     *items;
	DaoType  *unitype;
};

DAO_DLL DaoMap* DaoMap_New( unsigned int hashing );
DAO_DLL void DaoMap_Delete( DaoMap *self );
DAO_DLL void DaoMap_Clear( DaoMap *self );
DAO_DLL void DaoMap_Reset( DaoMap *self );

DAO_DLL int DaoMap_Insert( DaoMap *self, DaoValue *key, DaoValue *value );
DAO_DLL void DaoMap_Erase( DaoMap *self, DaoValue *key );


#define DAO_TUPLE_ITEMS 2
/* 2 is used instead of 1, for two reasons:
 * A. most often used tuples have at least two items;
 * B. some builtin tuples have at least two items, and are accessed by
 *    constant sub index, compilers such Clang may complain if 1 is used. */

struct DaoTuple
{
	DAO_DATA_COMMON;

	int         size; /* packed with the previous field in 64-bits system; */
	DaoType    *unitype;
	DaoValue   *items[DAO_TUPLE_ITEMS]; /* the actual number of items is in ::size; */
};

DAO_DLL DaoTuple* DaoTuple_Create( DaoType *type, int size, int init );
DAO_DLL void DaoTuple_Delete( DaoTuple *self );
DAO_DLL void DaoTuple_SetItem( DaoTuple *self, DaoValue *it, int pos );
DAO_DLL int DaoTuple_GetIndex( DaoTuple *self, DString *name );


/* DaoNameValue is not data type for general use, it is mainly used for 
 * passing named parameters and fields: */
struct DaoNameValue
{
	DAO_DATA_COMMON;

	DString   *name;
	DaoValue  *value;
	DaoType   *unitype;
};
DaoNameValue* DaoNameValue_New( DString *name, DaoValue *value );


/* Customized/extended Dao data or opaque C/C++ data: */
/* DaoCdata sub-types: */
enum DaoCdataType
{
	DAO_CDATA_PTR , /* opaque C/C++ data, not owned by the wrapper */
	DAO_CDATA_CXX , /* opaque C/C++ data, owned by the wrapper */
	DAO_CDATA_DAO   /* customized Dao data */
};

#define DAO_CDATA_COMMON \
	DAO_DATA_COMMON; DaoTypeBase *typer; DaoType *ctype; DaoObject *object; void *data

struct DaoCdata
{
	DAO_CDATA_COMMON;
};

DAO_DLL void DaoCdata_InitCommon( DaoCdata *self, DaoType *type );
DAO_DLL void DaoCdata_FreeCommon( DaoCdata *self );
DAO_DLL void DaoCdata_Delete( DaoCdata *self );

DAO_DLL extern DaoTypeBase defaultCdataTyper;
DAO_DLL extern DaoCdata dao_default_cdata;


/* In analog to DaoClass, a DaoCtype is created for each cdata type: */
struct DaoCtype
{
	DAO_CDATA_COMMON;

	DaoType *cdtype;
};
DAO_DLL DaoCtype* DaoCtype_New( DaoType *cttype, DaoType *cdtype );
DAO_DLL void DaoCtype_Delete( DaoCtype *self );



struct DaoException
{
	DAO_CDATA_COMMON;

	int         fromLine;
	int         toLine;
	DaoRoutine *routine;
	DArray     *callers;
	DArray     *lines;

	DString    *name;
	DString    *info;
	DaoValue   *edata;
};

DaoException* DaoException_New( DaoType *type );
DaoException* DaoException_New2( DaoType *type, DaoValue *v );
void DaoException_Delete( DaoException *self );
void DaoException_Setup( DaoNamespace *ns );

DaoType* DaoException_GetType( int type );

extern DaoTypeBase dao_Exception_Typer;


#endif
