/*=========================================================================================
  This file is a part of a virtual machine for the Dao programming language.
  Copyright (C) 2006-2012, Fu Limin. Email: fu@daovm.net, limin.fu@yahoo.com

  This software is free software; you can redistribute it and/or modify it under the terms 
  of the GNU Lesser General Public License as published by the Free Software Foundation; 
  either version 2.1 of the License, or (at your option) any later version.

  This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
  See the GNU Lesser General Public License for more details.
  =========================================================================================*/

#include"daoVmcode.h"

typedef struct DaoVmCodeInfo DaoVmCodeInfo;

struct DaoVmCodeInfo
{
	const char     *name;
	unsigned short  base;
	unsigned char   type;
	unsigned char   perm; /* Used only by parser for compiling expression lists: */
};

static DaoVmCodeInfo dao_code_infolist[] = 
{
	{ "NOP",        DVM_NOP,        DAO_CODE_NOP,     1 },
	{ "DATA",       DVM_DATA,       DAO_CODE_GETC,    1 }, 
	{ "GETCL",      DVM_GETCL,      DAO_CODE_GETC,    1 },
	{ "GETCK",      DVM_GETCK,      DAO_CODE_GETC,    1 },
	{ "GETCG",      DVM_GETCG,      DAO_CODE_GETC,    1 }, 
	{ "GETVH",      DVM_GETVH,      DAO_CODE_GETU,    1 },
	{ "GETVL",      DVM_GETVL,      DAO_CODE_GETU,    1 },
	{ "GETVO",      DVM_GETVO,      DAO_CODE_GETG,    1 },
	{ "GETVK",      DVM_GETVK,      DAO_CODE_GETG,    1 },
	{ "GETVG",      DVM_GETVG,      DAO_CODE_GETG,    1 }, 
	{ "GETI",       DVM_GETI,       DAO_CODE_GETI,    0 },
	{ "GETDI",      DVM_GETDI,      DAO_CODE_GETF,    0 },
	{ "GETMI",      DVM_GETMI,      DAO_CODE_GETM,    0 },
	{ "GETF",       DVM_GETF,       DAO_CODE_GETF,    0 },
	{ "GETMF",      DVM_GETMF,      DAO_CODE_GETF,    0 },
	{ "SETVH",      DVM_SETVH,      DAO_CODE_SETU,    1 },
	{ "SETVL",      DVM_SETVL,      DAO_CODE_SETU,    1 },
	{ "SETVO",      DVM_SETVO,      DAO_CODE_SETG,    1 },
	{ "SETVK",      DVM_SETVK,      DAO_CODE_SETG,    1 },
	{ "SETVG",      DVM_SETVG,      DAO_CODE_SETG,    1 }, 
	{ "SETI",       DVM_SETI,       DAO_CODE_SETI,    0 },
	{ "SETDI",      DVM_SETDI,      DAO_CODE_SETF,    0 },
	{ "SETMI",      DVM_SETMI,      DAO_CODE_SETM,    0 },
	{ "SETF",       DVM_SETF,       DAO_CODE_SETF,    0 },
	{ "SETMF",      DVM_SETMF,      DAO_CODE_SETF,    0 },
	{ "LOAD",       DVM_LOAD,       DAO_CODE_MOVE,    1 },
	{ "CAST",       DVM_CAST,       DAO_CODE_MOVE,    0 },
	{ "MOVE",       DVM_MOVE,       DAO_CODE_MOVE,    0 },
	{ "NOT",        DVM_NOT,        DAO_CODE_UNARY,   0 },
	{ "UNMS",       DVM_UNMS,       DAO_CODE_UNARY,   0 },
	{ "BITREV",     DVM_BITREV,     DAO_CODE_BINARY,  0 },
	{ "ADD",        DVM_ADD,        DAO_CODE_BINARY,  0 },
	{ "SUB",        DVM_SUB,        DAO_CODE_BINARY,  0 },
	{ "MUL",        DVM_MUL,        DAO_CODE_BINARY,  0 },
	{ "DIV",        DVM_DIV,        DAO_CODE_BINARY,  0 },
	{ "MOD",        DVM_MOD,        DAO_CODE_BINARY,  0 },
	{ "POW",        DVM_POW,        DAO_CODE_BINARY,  0 },
	{ "AND",        DVM_AND,        DAO_CODE_BINARY,  0 },
	{ "OR",         DVM_OR,         DAO_CODE_BINARY,  0 },
	{ "LT",         DVM_LT,         DAO_CODE_BINARY,  0 },
	{ "LE",         DVM_LE,         DAO_CODE_BINARY,  0 },
	{ "EQ",         DVM_EQ,         DAO_CODE_BINARY,  0 },
	{ "NE",         DVM_NE,         DAO_CODE_BINARY,  0 },
	{ "IN",         DVM_IN,         DAO_CODE_BINARY,  1 },
	{ "BITAND",     DVM_BITAND,     DAO_CODE_BINARY,  0 },
	{ "BITOR",      DVM_BITOR,      DAO_CODE_BINARY,  0 },
	{ "BITXOR",     DVM_BITXOR,     DAO_CODE_BINARY,  0 },
	{ "BITLFT",     DVM_BITLFT,     DAO_CODE_BINARY,  0 },
	{ "BITRIT",     DVM_BITRIT,     DAO_CODE_BINARY,  0 },
	{ "CHECK",      DVM_CHECK,      DAO_CODE_BINARY,  1 },
	{ "NAMEVA",     DVM_NAMEVA,     DAO_CODE_UNARY2,  1 },
	{ "PAIR",       DVM_PAIR,       DAO_CODE_BINARY,  1 },
	{ "TUPLE",      DVM_TUPLE,      DAO_CODE_ENUM,    1 },
	{ "LIST",       DVM_LIST,       DAO_CODE_ENUM,    1 },
	{ "MAP",        DVM_MAP,        DAO_CODE_ENUM,    1 },
	{ "HASH",       DVM_HASH,       DAO_CODE_ENUM,    1 },
	{ "VECTOR",     DVM_VECTOR,     DAO_CODE_ENUM,    1 },
	{ "MATRIX",     DVM_MATRIX,     DAO_CODE_MATRIX,  1 },
	{ "APLIST",     DVM_APLIST,     DAO_CODE_ENUM,    1 },
	{ "APVECTOR",   DVM_APVECTOR,   DAO_CODE_ENUM,    1 },
	{ "CURRY",      DVM_CURRY,      DAO_CODE_ENUM2,   1 },
	{ "MCURRY",     DVM_MCURRY,     DAO_CODE_ENUM2,   1 },
	{ "ROUTINE",    DVM_ROUTINE,    DAO_CODE_ROUTINE, 1 },
	{ "CLASS",      DVM_CLASS,      DAO_CODE_CLASS,   1 },
	{ "GOTO",       DVM_GOTO,       DAO_CODE_JUMP,    0 },
	{ "SWITCH",     DVM_SWITCH,     DAO_CODE_BRANCH,  0 },
	{ "CASE",       DVM_CASE,       DAO_CODE_JUMP,    0 },
	{ "ITER",       DVM_ITER,       DAO_CODE_MOVE,    0 },
	{ "TEST",       DVM_TEST,       DAO_CODE_BRANCH,  0 }, 
	{ "MATH",       DVM_MATH,       DAO_CODE_UNARY2,  1 },
	{ "CALL",       DVM_CALL,       DAO_CODE_CALL,    0 },
	{ "MCALL",      DVM_MCALL,      DAO_CODE_CALL,    0 }, 
	{ "RETURN",     DVM_RETURN,     DAO_CODE_EXPLIST, 0 },
	{ "YIELD",      DVM_YIELD,      DAO_CODE_YIELD,   0 },
	{ "TRY",        DVM_TRY,        DAO_CODE_NOP,     0 },
	{ "RAISE",      DVM_RAISE,      DAO_CODE_EXPLIST, 0 },
	{ "CATCH",      DVM_CATCH,      DAO_CODE_EXPLIST, 0 },
	{ "DEBUG",      DVM_DEBUG,      DAO_CODE_NOP,     0 },
	{ "JITC",       DVM_JITC,       DAO_CODE_NOP,     0 },
	{ "SECT",       DVM_SECT,       DAO_CODE_EXPLIST, 0 },
	{ "DATA_I",     DVM_DATA_I,     DAO_CODE_GETC,    0 },
	{ "DATA_F",     DVM_DATA_F,     DAO_CODE_GETC,    0 },
	{ "DATA_D",     DVM_DATA_D,     DAO_CODE_GETC,    0 }, 
	{ "GETCL_I",    DVM_GETCL_I,    DAO_CODE_GETC,    0 },
	{ "GETCL_F",    DVM_GETCL_F,    DAO_CODE_GETC,    0 },
	{ "GETCL_D",    DVM_GETCL_D,    DAO_CODE_GETC,    0 }, 
	{ "GETCK_I",    DVM_GETCK_I,    DAO_CODE_GETC,    0 },
	{ "GETCK_F",    DVM_GETCK_F,    DAO_CODE_GETC,    0 },
	{ "GETCK_D",    DVM_GETCK_D,    DAO_CODE_GETC,    0 }, 
	{ "GETCG_I",    DVM_GETCG_I,    DAO_CODE_GETC,    0 },
	{ "GETCG_F",    DVM_GETCG_F,    DAO_CODE_GETC,    0 },
	{ "GETCG_D",    DVM_GETCG_D,    DAO_CODE_GETC,    0 }, 
	{ "GETVH_I",    DVM_GETVH_I,    DAO_CODE_GETU,    0 },
	{ "GETVH_F",    DVM_GETVH_F,    DAO_CODE_GETU,    0 },
	{ "GETVH_D",    DVM_GETVH_D,    DAO_CODE_GETU,    0 }, 
	{ "GETVL_I",    DVM_GETVL_I,    DAO_CODE_GETU,    0 },
	{ "GETVL_F",    DVM_GETVL_F,    DAO_CODE_GETU,    0 },
	{ "GETVL_D",    DVM_GETVL_D,    DAO_CODE_GETU,    0 }, 
	{ "GETVO_I",    DVM_GETVO_I,    DAO_CODE_GETG,    0 },
	{ "GETVO_F",    DVM_GETVO_F,    DAO_CODE_GETG,    0 },
	{ "GETVO_D",    DVM_GETVO_D,    DAO_CODE_GETG,    0 }, 
	{ "GETVK_I",    DVM_GETVK_I,    DAO_CODE_GETG,    0 },
	{ "GETVK_F",    DVM_GETVK_F,    DAO_CODE_GETG,    0 },
	{ "GETVK_D",    DVM_GETVK_D,    DAO_CODE_GETG,    0 }, 
	{ "GETVG_I",    DVM_GETVG_I,    DAO_CODE_GETG,    0 },
	{ "GETVG_F",    DVM_GETVG_F,    DAO_CODE_GETG,    0 },
	{ "GETVG_D",    DVM_GETVG_D,    DAO_CODE_GETG,    0 }, 
	{ "SETVH_II",   DVM_SETVH_II,   DAO_CODE_SETG,    0 },
	{ "SETVH_IF",   DVM_SETVH_IF,   DAO_CODE_SETG,    0 },
	{ "SETVH_ID",   DVM_SETVH_ID,   DAO_CODE_SETG,    0 }, 
	{ "SETVH_FI",   DVM_SETVH_FI,   DAO_CODE_SETG,    0 },
	{ "SETVH_FF",   DVM_SETVH_FF,   DAO_CODE_SETG,    0 },
	{ "SETVH_FD",   DVM_SETVH_FD,   DAO_CODE_SETG,    0 }, 
	{ "SETVH_DI",   DVM_SETVH_DI,   DAO_CODE_SETG,    0 },
	{ "SETVH_DF",   DVM_SETVH_DF,   DAO_CODE_SETG,    0 },
	{ "SETVH_DD",   DVM_SETVH_DD,   DAO_CODE_SETG,    0 },
	{ "SETVL_II",   DVM_SETVL_II,   DAO_CODE_SETG,    0 },
	{ "SETVL_IF",   DVM_SETVL_IF,   DAO_CODE_SETG,    0 },
	{ "SETVL_ID",   DVM_SETVL_ID,   DAO_CODE_SETG,    0 }, 
	{ "SETVL_FI",   DVM_SETVL_FI,   DAO_CODE_SETG,    0 },
	{ "SETVL_FF",   DVM_SETVL_FF,   DAO_CODE_SETG,    0 },
	{ "SETVL_FD",   DVM_SETVL_FD,   DAO_CODE_SETG,    0 }, 
	{ "SETVL_DI",   DVM_SETVL_DI,   DAO_CODE_SETG,    0 },
	{ "SETVL_DF",   DVM_SETVL_DF,   DAO_CODE_SETG,    0 },
	{ "SETVL_DD",   DVM_SETVL_DD,   DAO_CODE_SETG,    0 },
	{ "SETVO_II",   DVM_SETVO_II,   DAO_CODE_SETG,    0 },
	{ "SETVO_IF",   DVM_SETVO_IF,   DAO_CODE_SETG,    0 },
	{ "SETVO_ID",   DVM_SETVO_ID,   DAO_CODE_SETG,    0 }, 
	{ "SETVO_FI",   DVM_SETVO_FI,   DAO_CODE_SETG,    0 },
	{ "SETVO_FF",   DVM_SETVO_FF,   DAO_CODE_SETG,    0 },
	{ "SETVO_FD",   DVM_SETVO_FD,   DAO_CODE_SETG,    0 }, 
	{ "SETVO_DI",   DVM_SETVO_DI,   DAO_CODE_SETG,    0 },
	{ "SETVO_DF",   DVM_SETVO_DF,   DAO_CODE_SETG,    0 },
	{ "SETVO_DD",   DVM_SETVO_DD,   DAO_CODE_SETG,    0 },
	{ "SETVK_II",   DVM_SETVK_II,   DAO_CODE_SETG,    0 },
	{ "SETVK_IF",   DVM_SETVK_IF,   DAO_CODE_SETG,    0 },
	{ "SETVK_ID",   DVM_SETVK_ID,   DAO_CODE_SETG,    0 }, 
	{ "SETVK_FI",   DVM_SETVK_FI,   DAO_CODE_SETG,    0 },
	{ "SETVK_FF",   DVM_SETVK_FF,   DAO_CODE_SETG,    0 },
	{ "SETVK_FD",   DVM_SETVK_FD,   DAO_CODE_SETG,    0 }, 
	{ "SETVK_DI",   DVM_SETVK_DI,   DAO_CODE_SETG,    0 },
	{ "SETVK_DF",   DVM_SETVK_DF,   DAO_CODE_SETG,    0 },
	{ "SETVK_DD",   DVM_SETVK_DD,   DAO_CODE_SETG,    0 },
	{ "SETVG_II",   DVM_SETVG_II,   DAO_CODE_SETG,    0 },
	{ "SETVG_IF",   DVM_SETVG_IF,   DAO_CODE_SETG,    0 },
	{ "SETVG_ID",   DVM_SETVG_ID,   DAO_CODE_SETG,    0 }, 
	{ "SETVG_FI",   DVM_SETVG_FI,   DAO_CODE_SETG,    0 },
	{ "SETVG_FF",   DVM_SETVG_FF,   DAO_CODE_SETG,    0 },
	{ "SETVG_FD",   DVM_SETVG_FD,   DAO_CODE_SETG,    0 }, 
	{ "SETVG_DI",   DVM_SETVG_DI,   DAO_CODE_SETG,    0 },
	{ "SETVG_DF",   DVM_SETVG_DF,   DAO_CODE_SETG,    0 },
	{ "SETVG_DD",   DVM_SETVG_DD,   DAO_CODE_SETG,    0 },
	{ "MOVE_II",    DVM_MOVE_II,    DAO_CODE_MOVE,    0 },
	{ "MOVE_IF",    DVM_MOVE_IF,    DAO_CODE_MOVE,    0 },
	{ "MOVE_ID",    DVM_MOVE_ID,    DAO_CODE_MOVE,    0 },
	{ "MOVE_FI",    DVM_MOVE_FI,    DAO_CODE_MOVE,    0 },
	{ "MOVE_FF",    DVM_MOVE_FF,    DAO_CODE_MOVE,    0 },
	{ "MOVE_FD",    DVM_MOVE_FD,    DAO_CODE_MOVE,    0 },
	{ "MOVE_DI",    DVM_MOVE_DI,    DAO_CODE_MOVE,    0 },
	{ "MOVE_DF",    DVM_MOVE_DF,    DAO_CODE_MOVE,    0 },
	{ "MOVE_DD",    DVM_MOVE_DD,    DAO_CODE_MOVE,    0 },
	{ "MOVE_CC",    DVM_MOVE_CC,    DAO_CODE_MOVE,    0 },
	{ "MOVE_SS",    DVM_MOVE_SS,    DAO_CODE_MOVE,    0 },
	{ "MOVE_PP",    DVM_MOVE_PP,    DAO_CODE_MOVE,    0 },
	{ "MOVE_XX",    DVM_MOVE_XX,    DAO_CODE_MOVE,    0 },
	{ "NOT_I",      DVM_NOT_I,      DAO_CODE_UNARY,   0 },
	{ "NOT_F",      DVM_NOT_F,      DAO_CODE_UNARY,   0 },
	{ "NOT_D",      DVM_NOT_D,      DAO_CODE_UNARY,   0 }, 
	{ "UNMS_I",     DVM_UNMS_I,     DAO_CODE_UNARY,   0 },
	{ "UNMS_F",     DVM_UNMS_F,     DAO_CODE_UNARY,   0 },
	{ "UNMS_D",     DVM_UNMS_D,     DAO_CODE_UNARY,   0 },
	{ "BITREV_I",   DVM_BITREV_I,   DAO_CODE_BINARY,  0 },
	{ "BITREV_F",   DVM_BITREV_F,   DAO_CODE_BINARY,  0 },
	{ "BITREV_D",   DVM_BITREV_D,   DAO_CODE_BINARY,  0 },
	{ "UNMS_C",     DVM_UNMS_C,     DAO_CODE_BINARY,  0 },
	{ "ADD_III",    DVM_ADD_III,    DAO_CODE_BINARY,  0 },
	{ "SUB_III",    DVM_SUB_III,    DAO_CODE_BINARY,  0 },
	{ "MUL_III",    DVM_MUL_III,    DAO_CODE_BINARY,  0 },
	{ "DIV_III",    DVM_DIV_III,    DAO_CODE_BINARY,  0 },
	{ "MOD_III",    DVM_MOD_III,    DAO_CODE_BINARY,  0 },
	{ "POW_III",    DVM_POW_III,    DAO_CODE_BINARY,  0 },
	{ "AND_III",    DVM_AND_III,    DAO_CODE_BINARY,  0 },
	{ "OR_III",     DVM_OR_III,     DAO_CODE_BINARY,  0 },
	{ "LT_III",     DVM_LT_III,     DAO_CODE_BINARY,  0 },
	{ "LE_III",     DVM_LE_III,     DAO_CODE_BINARY,  0 },
	{ "EQ_III",     DVM_EQ_III,     DAO_CODE_BINARY,  0 },
	{ "NE_III",     DVM_NE_III,     DAO_CODE_BINARY,  0 }, 
	{ "BITAND_III", DVM_BITAND_III, DAO_CODE_BINARY,  0 },
	{ "BITOR_III",  DVM_BITOR_III,  DAO_CODE_BINARY,  0 },
	{ "BITXOR_III", DVM_BITXOR_III, DAO_CODE_BINARY,  0 },
	{ "BITLFT_III", DVM_BITLFT_III, DAO_CODE_BINARY,  0 },
	{ "BITRIT_III", DVM_BITRIT_III, DAO_CODE_BINARY,  0 }, 
	{ "ADD_FFF",    DVM_ADD_FFF,    DAO_CODE_BINARY,  0 },
	{ "SUB_FFF",    DVM_SUB_FFF,    DAO_CODE_BINARY,  0 },
	{ "MUL_FFF",    DVM_MUL_FFF,    DAO_CODE_BINARY,  0 },
	{ "DIV_FFF",    DVM_DIV_FFF,    DAO_CODE_BINARY,  0 },
	{ "MOD_FFF",    DVM_MOD_FFF,    DAO_CODE_BINARY,  0 },
	{ "POW_FFF",    DVM_POW_FFF,    DAO_CODE_BINARY,  0 },
	{ "AND_FFF",    DVM_AND_FFF,    DAO_CODE_BINARY,  0 },
	{ "OR_FFF",     DVM_OR_FFF,     DAO_CODE_BINARY,  0 },
	{ "LT_FFF",     DVM_LT_FFF,     DAO_CODE_BINARY,  0 },
	{ "LE_FFF",     DVM_LE_FFF,     DAO_CODE_BINARY,  0 },
	{ "EQ_FFF",     DVM_EQ_FFF,     DAO_CODE_BINARY,  0 },
	{ "NE_FFF",     DVM_NE_FFF,     DAO_CODE_BINARY,  0 }, 
	{ "BITAND_FFF", DVM_BITAND_FFF, DAO_CODE_BINARY,  0 },
	{ "BITOR_FFF",  DVM_BITOR_FFF,  DAO_CODE_BINARY,  0 },
	{ "BITXOR_FFF", DVM_BITXOR_FFF, DAO_CODE_BINARY,  0 },
	{ "BITLFT_FFF", DVM_BITLFT_FFF, DAO_CODE_BINARY,  0 },
	{ "BITRIT_FFF", DVM_BITRIT_FFF, DAO_CODE_BINARY,  0 }, 
	{ "ADD_DDD",    DVM_ADD_DDD,    DAO_CODE_BINARY,  0 },
	{ "SUB_DDD",    DVM_SUB_DDD,    DAO_CODE_BINARY,  0 },
	{ "MUL_DDD",    DVM_MUL_DDD,    DAO_CODE_BINARY,  0 },
	{ "DIV_DDD",    DVM_DIV_DDD,    DAO_CODE_BINARY,  0 },
	{ "MOD_DDD",    DVM_MOD_DDD,    DAO_CODE_BINARY,  0 },
	{ "POW_DDD",    DVM_POW_DDD,    DAO_CODE_BINARY,  0 },
	{ "AND_DDD",    DVM_AND_DDD,    DAO_CODE_BINARY,  0 },
	{ "OR_DDD",     DVM_OR_DDD,     DAO_CODE_BINARY,  0 },
	{ "LT_DDD",     DVM_LT_DDD,     DAO_CODE_BINARY,  0 },
	{ "LE_DDD",     DVM_LE_DDD,     DAO_CODE_BINARY,  0 },
	{ "EQ_DDD",     DVM_EQ_DDD,     DAO_CODE_BINARY,  0 },
	{ "NE_DDD",     DVM_NE_DDD,     DAO_CODE_BINARY,  0 }, 
	{ "BITAND_DDD", DVM_BITAND_DDD, DAO_CODE_BINARY,  0 },
	{ "BITOR_DDD",  DVM_BITOR_DDD,  DAO_CODE_BINARY,  0 },
	{ "BITXOR_DDD", DVM_BITXOR_DDD, DAO_CODE_BINARY,  0 },
	{ "BITLFT_DDD", DVM_BITLFT_DDD, DAO_CODE_BINARY,  0 },
	{ "BITRIT_DDD", DVM_BITRIT_DDD, DAO_CODE_BINARY,  0 }, 
	{ "ADD_FNN",    DVM_ADD_FNN,    DAO_CODE_BINARY,  0 },
	{ "SUB_FNN",    DVM_SUB_FNN,    DAO_CODE_BINARY,  0 },
	{ "MUL_FNN",    DVM_MUL_FNN,    DAO_CODE_BINARY,  0 },
	{ "DIV_FNN",    DVM_DIV_FNN,    DAO_CODE_BINARY,  0 },
	{ "MOD_FNN",    DVM_MOD_FNN,    DAO_CODE_BINARY,  0 },
	{ "POW_FNN",    DVM_POW_FNN,    DAO_CODE_BINARY,  0 },
	{ "AND_FNN",    DVM_AND_FNN,    DAO_CODE_BINARY,  0 },
	{ "OR_FNN",     DVM_OR_FNN,     DAO_CODE_BINARY,  0 },
	{ "LT_FNN",     DVM_LT_FNN,     DAO_CODE_BINARY,  0 },
	{ "LE_FNN",     DVM_LE_FNN,     DAO_CODE_BINARY,  0 },
	{ "EQ_FNN",     DVM_EQ_FNN,     DAO_CODE_BINARY,  0 },
	{ "NE_FNN",     DVM_NE_FNN,     DAO_CODE_BINARY,  0 }, 
	{ "BITLFT_FNN", DVM_BITLFT_FNN, DAO_CODE_BINARY,  0 },
	{ "BITRIT_FNN", DVM_BITRIT_FNN, DAO_CODE_BINARY,  0 }, 
	{ "ADD_DNN",    DVM_ADD_DNN,    DAO_CODE_BINARY,  0 },
	{ "SUB_DNN",    DVM_SUB_DNN,    DAO_CODE_BINARY,  0 },
	{ "MUL_DNN",    DVM_MUL_DNN,    DAO_CODE_BINARY,  0 },
	{ "DIV_DNN",    DVM_DIV_DNN,    DAO_CODE_BINARY,  0 },
	{ "MOD_DNN",    DVM_MOD_DNN,    DAO_CODE_BINARY,  0 },
	{ "POW_DNN",    DVM_POW_DNN,    DAO_CODE_BINARY,  0 },
	{ "AND_DNN",    DVM_AND_DNN,    DAO_CODE_BINARY,  0 },
	{ "OR_DNN",     DVM_OR_DNN,     DAO_CODE_BINARY,  0 },
	{ "LT_DNN",     DVM_LT_DNN,     DAO_CODE_BINARY,  0 },
	{ "LE_DNN",     DVM_LE_DNN,     DAO_CODE_BINARY,  0 },
	{ "EQ_DNN",     DVM_EQ_DNN,     DAO_CODE_BINARY,  0 },
	{ "NE_DNN",     DVM_NE_DNN,     DAO_CODE_BINARY,  0 }, 
	{ "BITLFT_DNN", DVM_BITLFT_DNN, DAO_CODE_BINARY,  0 },
	{ "BITRIT_DNN", DVM_BITRIT_DNN, DAO_CODE_BINARY,  0 }, 
	{ "ADD_CC",     DVM_ADD_CC,     DAO_CODE_BINARY,  0 },
	{ "SUB_CC",     DVM_SUB_CC,     DAO_CODE_BINARY,  0 },
	{ "MUL_CC",     DVM_MUL_CC,     DAO_CODE_BINARY,  0 },
	{ "DIV_CC",     DVM_DIV_CC,     DAO_CODE_BINARY,  0 }, 
	{ "ADD_SS",     DVM_ADD_SS,     DAO_CODE_BINARY,  0 },
	{ "LT_SS",      DVM_LT_SS,      DAO_CODE_BINARY,  0 },
	{ "LE_SS",      DVM_LE_SS,      DAO_CODE_BINARY,  0 },
	{ "EQ_SS",      DVM_EQ_SS,      DAO_CODE_BINARY,  0 },
	{ "NE_SS",      DVM_NE_SS,      DAO_CODE_BINARY,  0 },
	{ "GETI_LI",    DVM_GETI_LI,    DAO_CODE_GETI,    0 },
	{ "SETI_LI",    DVM_SETI_LI,    DAO_CODE_SETI,    0 },
	{ "GETI_SI",    DVM_GETI_SI,    DAO_CODE_GETI,    0 },
	{ "SETI_SII",   DVM_SETI_SII,   DAO_CODE_SETI,    0 },
	{ "GETI_LII",   DVM_GETI_LII,   DAO_CODE_GETI,    0 },
	{ "GETI_LFI",   DVM_GETI_LFI,   DAO_CODE_GETI,    0 },
	{ "GETI_LDI",   DVM_GETI_LDI,   DAO_CODE_GETI,    0 },
	{ "GETI_LSI",   DVM_GETI_LSI,   DAO_CODE_GETI,    0 },
	{ "SETI_LIII",  DVM_SETI_LIII,  DAO_CODE_SETI,    0 },
	{ "SETI_LIIF",  DVM_SETI_LIIF,  DAO_CODE_SETI,    0 },
	{ "SETI_LIID",  DVM_SETI_LIID,  DAO_CODE_SETI,    0 },
	{ "SETI_LFII",  DVM_SETI_LFII,  DAO_CODE_SETI,    0 },
	{ "SETI_LFIF",  DVM_SETI_LFIF,  DAO_CODE_SETI,    0 },
	{ "SETI_LFID",  DVM_SETI_LFID,  DAO_CODE_SETI,    0 },
	{ "SETI_LDII",  DVM_SETI_LDII,  DAO_CODE_SETI,    0 },
	{ "SETI_LDIF",  DVM_SETI_LDIF,  DAO_CODE_SETI,    0 },
	{ "SETI_LDID",  DVM_SETI_LDID,  DAO_CODE_SETI,    0 },
	{ "SETI_LSIS",  DVM_SETI_LSIS,  DAO_CODE_SETI,    0 },
	{ "GETI_AII",   DVM_GETI_AII,   DAO_CODE_GETI,    0 },
	{ "GETI_AFI",   DVM_GETI_AFI,   DAO_CODE_GETI,    0 },
	{ "GETI_ADI",   DVM_GETI_ADI,   DAO_CODE_GETI,    0 },
	{ "SETI_AIII",  DVM_SETI_AIII,  DAO_CODE_SETI,    0 },
	{ "SETI_AIIF",  DVM_SETI_AIIF,  DAO_CODE_SETI,    0 },
	{ "SETI_AIID",  DVM_SETI_AIID,  DAO_CODE_SETI,    0 },
	{ "SETI_AFII",  DVM_SETI_AFII,  DAO_CODE_SETI,    0 },
	{ "SETI_AFIF",  DVM_SETI_AFIF,  DAO_CODE_SETI,    0 },
	{ "SETI_AFID",  DVM_SETI_AFID,  DAO_CODE_SETI,    0 },
	{ "SETI_ADII",  DVM_SETI_ADII,  DAO_CODE_SETI,    0 },
	{ "SETI_ADIF",  DVM_SETI_ADIF,  DAO_CODE_SETI,    0 },
	{ "SETI_ADID",  DVM_SETI_ADID,  DAO_CODE_SETI,    0 },
	{ "GETI_TI",    DVM_GETI_TI,    DAO_CODE_GETI,    0 },
	{ "SETI_TI",    DVM_SETI_TI,    DAO_CODE_SETI,    0 }, 
	{ "GETF_TI",    DVM_GETF_TI,    DAO_CODE_GETF,    0 },
	{ "GETF_TF",    DVM_GETF_TF,    DAO_CODE_GETF,    0 },
	{ "GETF_TD",    DVM_GETF_TD,    DAO_CODE_GETF,    0 },
	{ "GETF_TX",    DVM_GETF_TX,    DAO_CODE_GETF,    0 },
	{ "SETF_TII",   DVM_SETF_TII,   DAO_CODE_SETF,    0 },
	{ "SETF_TIF",   DVM_SETF_TIF,   DAO_CODE_SETF,    0 },
	{ "SETF_TID",   DVM_SETF_TID,   DAO_CODE_SETF,    0 },
	{ "SETF_TFI",   DVM_SETF_TFI,   DAO_CODE_SETF,    0 },
	{ "SETF_TFF",   DVM_SETF_TFF,   DAO_CODE_SETF,    0 },
	{ "SETF_TFD",   DVM_SETF_TFD,   DAO_CODE_SETF,    0 },
	{ "SETF_TDI",   DVM_SETF_TDI,   DAO_CODE_SETF,    0 },
	{ "SETF_TDF",   DVM_SETF_TDF,   DAO_CODE_SETF,    0 },
	{ "SETF_TDD",   DVM_SETF_TDD,   DAO_CODE_SETF,    0 },
	{ "SETF_TSS",   DVM_SETF_TSS,   DAO_CODE_SETF,    0 },
	{ "SETF_TPP",   DVM_SETF_TPP,   DAO_CODE_SETF,    0 },
	{ "SETF_TXX",   DVM_SETF_TXX,   DAO_CODE_SETF,    0 },
	{ "GETI_ACI",   DVM_GETI_ACI,   DAO_CODE_GETI,    0 },
	{ "SETI_ACI",   DVM_SETI_ACI,   DAO_CODE_SETI,    0 },
	{ "GETMI_A",    DVM_GETMI_A,    DAO_CODE_GETM,    0 },
	{ "SETMI_A",    DVM_SETMI_A,    DAO_CODE_SETM,    0 },
	{ "GETF_KC",    DVM_GETF_KC,    DAO_CODE_GETF,    0 },
	{ "GETF_KG",    DVM_GETF_KG,    DAO_CODE_GETF,    0 },
	{ "GETF_OC",    DVM_GETF_OC,    DAO_CODE_GETF,    0 },
	{ "GETF_OG",    DVM_GETF_OG,    DAO_CODE_GETF,    0 },
	{ "GETF_OV",    DVM_GETF_OV,    DAO_CODE_GETF,    0 }, 
	{ "SETF_KG",    DVM_SETF_KG,    DAO_CODE_SETF,    0 },
	{ "SETF_OG",    DVM_SETF_OG,    DAO_CODE_SETF,    0 },
	{ "SETF_OV",    DVM_SETF_OV,    DAO_CODE_SETF,    0 },
	{ "GETF_KCI",   DVM_GETF_KCI,   DAO_CODE_GETF,    0 },
	{ "GETF_KCF",   DVM_GETF_KCF,   DAO_CODE_GETF,    0 },
	{ "GETF_KCD",   DVM_GETF_KCD,   DAO_CODE_GETF,    0 },
	{ "GETF_KGI",   DVM_GETF_KGI,   DAO_CODE_GETF,    0 },
	{ "GETF_KGF",   DVM_GETF_KGF,   DAO_CODE_GETF,    0 },
	{ "GETF_KGD",   DVM_GETF_KGD,   DAO_CODE_GETF,    0 },
	{ "GETF_OCI",   DVM_GETF_OCI,   DAO_CODE_GETF,    0 },
	{ "GETF_OCF",   DVM_GETF_OCF,   DAO_CODE_GETF,    0 },
	{ "GETF_OCD",   DVM_GETF_OCD,   DAO_CODE_GETF,    0 },
	{ "GETF_OGI",   DVM_GETF_OGI,   DAO_CODE_GETF,    0 },
	{ "GETF_OGF",   DVM_GETF_OGF,   DAO_CODE_GETF,    0 },
	{ "GETF_OGD",   DVM_GETF_OGD,   DAO_CODE_GETF,    0 },
	{ "GETF_OVI",   DVM_GETF_OVI,   DAO_CODE_GETF,    0 },
	{ "GETF_OVF",   DVM_GETF_OVF,   DAO_CODE_GETF,    0 },
	{ "GETF_OVD",   DVM_GETF_OVD,   DAO_CODE_GETF,    0 },
	{ "SETF_KGII",  DVM_SETF_KGII,  DAO_CODE_SETF,    0 },
	{ "SETF_KGIF",  DVM_SETF_KGIF,  DAO_CODE_SETF,    0 },
	{ "SETF_KGID",  DVM_SETF_KGID,  DAO_CODE_SETF,    0 }, 
	{ "SETF_KGFI",  DVM_SETF_KGFI,  DAO_CODE_SETF,    0 },
	{ "SETF_KGFF",  DVM_SETF_KGFF,  DAO_CODE_SETF,    0 },
	{ "SETF_KGFD",  DVM_SETF_KGFD,  DAO_CODE_SETF,    0 }, 
	{ "SETF_KGDI",  DVM_SETF_KGDI,  DAO_CODE_SETF,    0 },
	{ "SETF_KGDF",  DVM_SETF_KGDF,  DAO_CODE_SETF,    0 },
	{ "SETF_KGDD",  DVM_SETF_KGDD,  DAO_CODE_SETF,    0 }, 
	{ "SETF_OGII",  DVM_SETF_OGII,  DAO_CODE_SETF,    0 },
	{ "SETF_OGIF",  DVM_SETF_OGIF,  DAO_CODE_SETF,    0 },
	{ "SETF_OGID",  DVM_SETF_OGID,  DAO_CODE_SETF,    0 }, 
	{ "SETF_OGFI",  DVM_SETF_OGFI,  DAO_CODE_SETF,    0 },
	{ "SETF_OGFF",  DVM_SETF_OGFF,  DAO_CODE_SETF,    0 },
	{ "SETF_OGFD",  DVM_SETF_OGFD,  DAO_CODE_SETF,    0 }, 
	{ "SETF_OGDI",  DVM_SETF_OGDI,  DAO_CODE_SETF,    0 },
	{ "SETF_OGDF",  DVM_SETF_OGDF,  DAO_CODE_SETF,    0 },
	{ "SETF_OGDD",  DVM_SETF_OGDD,  DAO_CODE_SETF,    0 }, 
	{ "SETF_OVII",  DVM_SETF_OVII,  DAO_CODE_SETF,    0 },
	{ "SETF_OVIF",  DVM_SETF_OVIF,  DAO_CODE_SETF,    0 },
	{ "SETF_OVID",  DVM_SETF_OVID,  DAO_CODE_SETF,    0 },
	{ "SETF_OVFI",  DVM_SETF_OVFI,  DAO_CODE_SETF,    0 },
	{ "SETF_OVFF",  DVM_SETF_OVFF,  DAO_CODE_SETF,    0 },
	{ "SETF_OVFD",  DVM_SETF_OVFD,  DAO_CODE_SETF,    0 },
	{ "SETF_OVDI",  DVM_SETF_OVDI,  DAO_CODE_SETF,    0 },
	{ "SETF_OVDF",  DVM_SETF_OVDF,  DAO_CODE_SETF,    0 },
	{ "SETF_OVDD",  DVM_SETF_OVDD,  DAO_CODE_SETF,    0 },
	{ "TEST_I",     DVM_TEST_I,     DAO_CODE_BRANCH,  0 },
	{ "TEST_F",     DVM_TEST_F,     DAO_CODE_BRANCH,  0 },
	{ "TEST_D",     DVM_TEST_D,     DAO_CODE_BRANCH,  0 },
	{ "CHECK_ST",   DVM_CHECK_ST,   DAO_CODE_BINARY,  0 },
	{ "GOTO",       DVM_GOTO,       DAO_CODE_JUMP,    0 },
	{ "???",        DVM_UNUSED,     DAO_CODE_NOP,     0 },

	/* for compiling only */
	{ "label",      DVM_UNUSED,     DAO_CODE_NOP, 0 },
	{ "load2",      DVM_UNUSED,     DAO_CODE_NOP, 0 },
	{ "loop",       DVM_UNUSED,     DAO_CODE_NOP, 0 },
	{ "branch",     DVM_UNUSED,     DAO_CODE_NOP, 0 },
	{ "do",         DVM_UNUSED,     DAO_CODE_NOP, 0 },
	{ "lbra",       DVM_UNUSED,     DAO_CODE_NOP, 0 },
	{ "rbra",       DVM_UNUSED,     DAO_CODE_NOP, 0 },
	{ "try",        DVM_UNUSED,     DAO_CODE_NOP, 0 },
	{ "raise",      DVM_UNUSED,     DAO_CODE_NOP, 0 },
	{ "catch",      DVM_UNUSED,     DAO_CODE_NOP, 0 },
	{ "unused",     DVM_UNUSED,     DAO_CODE_NOP, 0 }
};

const char* DaoVmCode_GetOpcodeName( int code )
{
	if( code >= 0 && code <= DVM_UNUSED ) return dao_code_infolist[ code ].name;
	return "???";
}
uchar_t DaoVmCode_GetOpcodeType( int code )
{
	if( code >= 0 && code <= DVM_UNUSED ) return dao_code_infolist[ code ].type;
	return DAO_CODE_NOP;
}
uchar_t DaoVmCode_CheckPermutable( int code )
{
	if( code >= 0 && code <= DVM_UNUSED ) return dao_code_infolist[ code ].perm;
	return 0;
}

void DaoVmCode_Print( DaoVmCode self, char *buffer )
{
	const char *name = DaoVmCode_GetOpcodeName( self.code );
	static const char *fmt = "%-11s : %6i , %6i , %6i ;\n";
	if( buffer == NULL )
		printf( fmt, name, self.a, self.b, self.c );
	else
		sprintf( buffer, fmt, name, self.a, self.b, self.c );
}
void DaoVmCodeX_Print( DaoVmCodeX self, char *annot )
{
	const char *name = DaoVmCode_GetOpcodeName( self.code );
	static const char *fmt = "%-11s : %6i , %6i , %6i ;  %4i,  %s\n";
	printf( fmt, name, self.a, self.b, self.c, self.line, annot ? annot : "" );
}
