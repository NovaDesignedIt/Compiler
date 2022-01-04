/*************************************************************

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
* Author: Meech Lamirande Kabuasa                                   *
* studentID: lami0039                                               *
* Student Email : lami0039@algonquinlive.com                        *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
* File name: table.c                                               *
* Compiler: GCC                                                     *
* Course: CST 8152 – Compilers, Lab Section: [012]                  *
* Assignment: A1.                                                   *
* Date: feb 6 2021                                                  *
* Professor: Paulo Sousa / Abdulah                                  *
* Purpose: This file is the main code for Buffer (A1)               *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *


***************************************************************
/* TODO_201: REPLACE THIS HEADER WITH YOUR HEADER */
// TODO_101: Adjust the file header

/* Macro section */
#ifndef  TABLE_H_
#define  TABLE_H_ 

#ifndef BUFFER_H_
#include "buffer.h"
#endif

#ifndef TOKEN_H_
#include "token.h"
#endif

#ifndef NULL
#include <_null.h> /* NULL pointer constant is defined there */
#endif

 /*  Source end-of-file (SEOF) sentinel symbol
  *    '\0' or one of 255,0xFF,EOF
  */

  /* TODO_102: EOF definitions */
#define CHARSEOF0 '\0'
#define CHARSEOF255 255

/*  Special case tokens processed separately one by one
 *  in the token-driven part of the scanner
 *  '=' , ' ' , '(' , ')' , '{' , '}' , == , <> , '>' , '<' , ';',
 *  white space
 *  !!comment , ',' , ';' , '-' , '+' , '*' , '/', ## ,
 *  .AND., .OR. , .NOT., SEOF,
 */

 /* TODO_103: Error states and illegal state */
#define ES  (-2)		/* Error state  with no retract */
#define ER  (-1)		/* Error state  with retract */
#define IS  0		    /* Illegal state */

#define MAX_INT_P 32767

 /* State transition table definition */
#define TABLE_COLUMNS 8

/* Transition table - type of states defined in separate table */
/* TODO_104: Define Transition table missing values */
int  transitionTable[][TABLE_COLUMNS] = {
	/*				[A-z],		[0-9],		.,			$,			",			SEOF,		other	*/
	/*				L(0),		D(1),		P(2),		S(3),		Q(4),		E(5),		O(6)	*/
	/* State 00 */  {1, 	    5,	        11,	        3,	        9,	        12, 	    11},
	/* State 01 */  {1,	        1,	        2,	        2,  	    2,	        2,	        2},
	/* State 02 */  {IS,		IS,			IS,			IS,			IS,			IS,			IS},
	/* State 03 */  {3,	        3,	        11,	        4,	        11,	        11,     	11},
	/* State 04 */  {IS,		IS,			IS,			IS,			IS,			IS,			IS},
	/* State 05 */  {6,	        5,	        7,	        6,	        6,	        11,	        6},
	/* State 06 */  {IS,		IS,			IS,			IS,			IS,			IS,			IS},
	/* State 07 */  {8,	        7,	        8,	        8,	        8,         11,	        8},
	/* State 08 */  {IS,		IS,			IS,			IS,			IS,			IS,			IS},
	/* State 09 */  {9,	        9,	        9,      	9,      	10,     	11,         9},
	/* State 10 */  {IS,		IS,			IS,			IS,			IS,			IS,			IS},
	/* State 11 */  {IS,		IS,			IS,			IS,			IS,			IS,			IS},
	/* State 12 */  {IS,		IS,			IS,			IS,			IS,			IS,			IS}
};

/* Accepting state table definition */
/* TODO_105: Define accepting states types */
#define ASWR 1	/* accepting state with retract */
#define ASNR 0	/* accepting state with no retract */
#define NOAS (-1) /* not accepting state */

/* List of acceptable states */
/* TODO_106: Define list of acceptable states */
int stateType[] = {
	NOAS, 		/* 00 */
	NOAS, 		/* 01 */
	ASWR, 	/* 02 */
	NOAS, 		/* 03 */
	ASNR, //ASWR 	/* 04 */
	NOAS, 		/* 05 */
	ASWR, 	    /* 06 */
	NOAS, 		/* 07 */
	ASWR, 	    /* 08 */
	NOAS, 		/* 09 */
	ASNR, //ASWR 	    /* 10 */
	ASWR,//ASNR, 	    /* 11 */
	ASNR  	    /* 12 */
};

/* TODO_107: Declare accepting states functions */

/* Accepting action function declarations */
Token funcAVID(char* lexeme);
Token funcSVID(char* lexeme);
Token funcIL(char* lexeme);
Token funcFPL(char* lexeme);
Token funcSL(char* lexeme);

Token funcErr(char* lexeme);
/* Defining a new type: pointer to function (of one char * argument)
   returning Token
*/
typedef Token(*PTR_ACCFUN)(char* lexeme);

/* Accepting function (action) callback table (array) definition */
/* If you do not want to use the typedef, the equvalent declaration is:
 * Token (*finalStateTable[])(char lexeme[]) = {
 */
 /* TODO_108: Define final state table */
PTR_ACCFUN finalStateTable[] = {
	NULL,		/* 00 */
	NULL,		/* 01 */
	funcAVID,		/* 02 */
	NULL,		/* 03 */
	funcSVID,		/* 04 */
	NULL,		/* 05 */
	funcIL,		/* 06 */
	NULL,		/* 07 */
	funcFPL,		/* 08 */
	NULL,		/* 09 */
	funcSL,		/* 10 */
	funcErr,		/* 11 */
	funcErr			/* 12 */
};

/* TODO_09: Define the number of Keywords from the language */
/* Keyword lookup table (.AND., .OR. and .NOT. are not keywords) */
#define KWT_SIZE 10

/* Keyword list */
/* TODO_10: Define the list of keywords */
char* keywordTable[] = {
	"MAIN",
	"IF",
	"THEN",
	"ELSE",
	"WHILE",
	"DO",
	"READ",
	"WRITE",
	"TRUE",
	"FALSE",
};

#endif