/**
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
* Author: Meech Lamirande Kabuasa                                   *
* studentID: lami0039                                               *
* Student Email : lami0039@algonquinlive.com                        *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
* File name: buffer.h                                               *
* Compiler: GCC                                                     *
* Course: CST 8152 – Compilers, Lab Section: [012]                  *
* Assignment: A1.                                                   *
* Date: feb 6 2021                                                  *
* Professor: Paulo Sousa / Abdulah                                  *
* Purpose: This file is the main code for Buffer (A1)               *
* Function list:                                                    *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
// TODO101: Adjust file header
#ifndef BUFFER_H_
#define BUFFER_H_
/*#pragma warning(1:4001) *//*to enforce C89 type comments  - to make //comments an warning */
/*#pragma warning(error:4001)*//* to enforce C89 comments - to make // comments an error */
/* standard header files */
#include <stdio.h>  /* standard input/output */
#include <malloc.h> /* for dynamic memory allocation*/
#include <limits.h> /* implementation-defined data type ranges and limits */
#include <string.h>
/* constant definitions */
#define RT_FAIL_1 (-1)			// TODO102
#define RT_FAIL_2 (-2)			// TODO103
#define LOAD_FAIL (-1)			// TODO104

#define DEFAULT_SIZE  (200)     // TODO105
#define DEFAULT_INCREMENT (15)  // TODO106

/* You should add your own constant definitions here */
#define MAX_SIZE SHRT_MAX-1  /* maximum capacity*/

/* Buffer Modes */
//the flags return buffer status
#define FIXMODE 1       // TODO107
#define ADDMODE 0       // TODO108
#define MULMODE (-1)      // TODO109

/* Add your bit-masks constant definitions here */
#define DEFAULT_FLAGS (0x3FFF) 	// 0011.1111 1111.1111

//to check these you have to set the feild
#define SET_EOB   0x8000         // TODO110                       OR
#define RESET_EOB 0x7FFF        // TODO111                      AND
#define CHECK_EOB 0x8000       // TODO112                       AND
#define SET_R_FLAG 0x4000      // TODO113
#define RESET_R_FLAG 0xBFFF   // TODO114
#define CHECK_R_FLAG 0x4000   // TODO115

/* Constants used in buffer */
#define MAX_INCREMENT 100	// TODO116

#define MAX_VALUE  SHRT_MAX-1		// TODO117

#define RT_INC_FAIL 0x100	// TODO118

/* user data type declarations */
typedef struct Buffer {
    char* content;         /* pointer to the beginning of character array (character buffer) */
    short size;            /* current dynamic memory size (in bytes) allocated to  buffer */
    char  increment;       /* character array increment factor */
    char  mode;            /* operational mode indicator*/
    short addCOffset;      /* the offset (in chars) to the add-character location */
    short getCOffset;      /* the offset (in chars) to the get-character location */
    short markOffset;      /* the offset (in chars) to the mark location */
    unsigned short flags;  /* contains character array reallocation and end-of-buffer flag */
} bStructure, * bPointer;

/* Function declarations */
bPointer bCreate(short size, char increment, char mode);
bPointer bAddCh(bPointer const b_ptr, char ch);
void displayBuffer(bPointer const b_ptr);
int bClean(bPointer const pBuffer);
int bLoad(bPointer const pBuffer, FILE* const fi);
size_t bGetIncrement(bPointer const pBuffer);
int bDisplay(bPointer const pBuffer, char nl);
int bFree(bPointer const pBuffer);
unsigned short bGetFlags(bPointer const pBuffer);
char* bGetContent(bPointer const pBuffer, short pos);
int bGetMode(bPointer const pBuffer);
short bGetSize(bPointer const pBuffer);
short bGetAddChOffset(bPointer const pBuffer);
int bIsFull(bPointer const pBuffer);
bPointer bFinish(bPointer const pBuffer, char ch);
short bGetChOffset(bPointer const pBuffer);
short bufferAddCposition(bPointer const pBuffer);
short bRestore(bPointer const pBuffer);
bPointer bRetract(bPointer const pBuffer);
int bRewind(bPointer const pBuffer);
char bGetCh(bPointer const pBuffer);
int bIsEmpty(bPointer const pBuffer);
short bSetMarkOffset(bPointer const pBuffer, short mark);

#endif
