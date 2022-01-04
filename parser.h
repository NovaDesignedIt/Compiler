
/*************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Winter, 2021
*************************************************************
* File name: parser.h
* Compiler: MS Visual Studio 2019
* Author: Svillen Ranev - Paulo Sousa - Abdulah
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013, or 014]
* Assignment: A3.
* Date: Sep 01 2020
* Purpose: This file is the main header for Parser (.h)
* Function list: (...).
*************************************************************/

/* TODO_101: Adjust the function header */

/* Inclusion section */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "token.h"
#include "buffer.h"

/* Global vars */
static Token lookahead;
int syntaxErrorNumber = 0;

extern bStructure* stringLiteralTable;
extern int line;
extern Token tokenizer();
extern char* keywordTable[];

/* Mock code: remove this UNKNOWN when variables defined */
#define UNKNOWN 0

/* TODO_102: Create ALL constants for keywords (sequence given in table.h) */
/* Constants */
#define	NO_ATTR	-1
#define MAIN 	0
#define IF		1
#define THEN	2
#define	ELSE	3
#define WHILE	4
#define DO		5
#define READ	6
#define WRITE	7
#define TRUE	8
#define FALSE	9
// Continue the code

/* Function definitions */
void startParser(void);
void matchToken(int, int);
void syncErrorHandler(int);
void printError();
//void printMessage(char*);

/* TODO_103: Place ALL non-terminal function declarations */

//put those you finished at the top
void program(void);
void optionalStatements(void);
void statements(void);
void variableList(void);
void variableListPrime(void);
void variableIdentifier(void);
void iterationStatement(void);
void outputStatement(void);
void outputStatementPrime(void);
void arithmeticExpression(void);
void additiveArithmeticExpression(void);
void additiveArithmeticExpressionPrime(void);
void assignmentExpression(void);
void assignmentStatement(void);
void conditionalExpression(void);
void inputStatement(void);
void logicalAndExpression(void);
void logicalAndExpressionPrime(void);
void primaryArithmeticExpression(void);
void primaryRelationalArithmeticExpression(void);
void multiplicativeArithmeticExpression(void);
void multiplicativeArithmeticExpressionPrime(void);
void primaryRelationalStringExpression(void);
void primaryStringExpression(void);
void preCondition(void);
void selectionStatement(void);
void stringExpression(void);
void stringExpressionPrime(void);
void statementsPrime(void);
void statement(void);
void logicalOrExpressionPrime(void);
void relationalExpression(void);
void relationalArithmeticOperator(void);
void relationalStringOperator(void);
void outputVariableList(void);
void unaryArithmeticExpression(void);


void relationalArithmeticExpression(void);
void relationalStringExpression(void);
void logicalOrExpression(void);
void selection_statement(void);

void relationalStringExpressionPrime(void);
void relationalArithmeticExpressionPrime(void);
void OutputvariableList(void);
void logicalNotExpression(void);







// Continue the code
