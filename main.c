/*************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2020
* This Code is EXCLUSIVE for professors (must not be shared)
* Author: Svillen Ranev - Paulo Sousa - Abdulah.
*************************************************************
* File Name: main.c
* Compiler: MS Visual Studio 2019
* Author: Tyrel Remus ID# 040566685, Anoushka Gourisunkur ID# 040963614
* Course: CST 8152 – Compilers, Lab Section: 013
* Assignment: 2
* Date: 5 February 2021
* Professor: Adbulah Kadri, Paulo Sousa
* Purpose: A character buffer utility with three modes of self-incrementation through dynamic memory allocation, and ability to set a mark flag.
* Function list: From buffer:
				bCreate(), bAddCh(), bClean(), bFree(), bIsFull(), bGetAddChOffset(), bGetSize(), bGetMode(), bGetMarkOffset(), bSetMarkOffset(), bFinish(),
				bDisplay(), bLoad(), bIsEmpty(), bGetCh(), bRewind(), bRetract(), bRestore(), bGetChOffset(), bGetIncrement(), bGetContent(), bufferAddCPosition()

				From scanner:
				startScanner(), tokenizer(), processToken(), nextClass(), nextState(), funcAVID(), funcSVID(), funcIL(), funcFPL(), funcSL(), funcErr(), isKeyword()
*************************************************************/
#define _CRT_SECURE_NO_WARNINGS 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

#define ANSI_C 0
#if defined(__STDC__)
#undef ANSI_C
#define ANSI_C 1
#endif

int mainParser(int, char**);
/************************************************************
Function name: main()
Purpose: This function contains the main function of the buffer where the program starts executing
Author: Tyrel Remus ID# 040566685, Anoushka Gourisunkur ID# 040963614
History/Versions: 5 February 2021
Called functions: mainBuffer()
Parameters: int argc, char** argv
Return Value: 0 (succesful execution)
Algorithm: Nothing
**************************************************************/


int main(int argc, char** argv)
{
	mainParser(argc, argv);

	return 0;
}

