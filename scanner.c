/*************************************************************

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
* Author: Meech Lamirande Kabuasa                                   *
* studentID: lami0039                                               *
* Student Email : lami0039@algonquinlive.com                        *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
* File name: Scanner.c                                               *
* Compiler: GCC                                                     *
* Course: CST 8152 – Compilers, Lab Section: [012]                  *
* Assignment: A1.                                                   *
* Date: feb 6 2021                                                  *
* Professor: Paulo Sousa / Abdulah                                  *
* Purpose: This file is the main code for Buffer (A1)               *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *


***************************************************************
/* TODO_201: REPLACE THIS HEADER WITH YOUR HEADER */

/* The #define _CRT_SECURE_NO_WARNINGS should be used in MS Visual Studio projects
 * to suppress the warnings about using "unsafe" functions like fopen()
 * and standard sting library functions defined in string.h.
 * The define does not have any effect in Borland compiler projects.
 */
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>   /* standard input / output */
#include <ctype.h>   /* conversion functions */
#include <stdlib.h>  /* standard library functions and constants */
#include <string.h>  /* string functions */
#include <limits.h>  /* integer types constants */
#include <float.h>   /* floating-point types constants */

 /*#define NDEBUG        to suppress assert() call */
#include <assert.h>  /* assert() prototype */

/* project header files */
#include "buffer.h"
#include "token.h"
#include "table.h"

#define DEBUG  /* for conditional processing */
#undef  DEBUG

/* Global objects - variables */
/* This buffer is used as a repository for string literals.
   It is defined in platy_st.c */
extern bPointer stringLiteralTable;        /* String literal table */
int line;                                /* current line number of the source code */
extern int errorNumber;                    /* defined in platy_st.c - run-time error number */

static char debugMode = 0;                /* optional for debugging */

/* Local(file) global objects - variables */
static bPointer lexemeBuffer;            /* pointer to temporary lexeme buffer */
static bPointer sourceBuffer;            /* pointer to input source buffer */
/* No other global variable declarations/definitiond are allowed */

/* scanner.c static(local) function  prototypes */
static int nextClass(char c);            /* character class function */
static int nextState(int, char);        /* state machine function */
static int isKeyword(char* kw_lexeme);    /* keywords lookup function */

/* TODO_202: Follow the standard and adjust all function headers */

/*************************************************************
 * Intitializes scanner
 *		This function initializes the scanner using defensive programming.
 ************************************************************/
int startScanner(bPointer psourceBuffer) {
    if (bIsEmpty(psourceBuffer))
        return EXIT_FAILURE; /*1*/
    /* in case the buffer has been read previously  */
    bRewind(psourceBuffer);
    bClean(stringLiteralTable);
    line = 1;
    sourceBuffer = psourceBuffer;
    return EXIT_SUCCESS; /*0*/
}

/* TODO_203: Follow the standard and adjust all function headers */




/*************************************************************

 Function Name: tokenizer
  Purpose:  creates a buffer allocates the buffer on heap
  it sets the buffer struct variables that are given in the parameters
  it sets the mode, the increment, the size, and flags

  Author: Meech Lamirande Kabuasa
  History/Version: 1
  Called Functions: Tokenizer;
  Parameters: void
  return value: Token
  Algorithm: find the tokens, using the DFA to determine the type of the token that will be assigned this if
    find special car? :
        use the dfa to find the token type?
            the transition tables returns the type of the token depending on the character that is being fed through the
              return the token to be handleded by the parser.

 ************************************************************/
Token tokenizer(void) {
    Token currentToken = { 0 }; /* token to return after pattern recognition. Set all structure members to 0 */
    unsigned char c;	/* input symbol */
    int state = 0;		/* initial state of the FSM */
    short lexStart;		/* start offset of a lexeme in the input char buffer (array) */
    short lexEnd;		/* end offset of a lexeme in the input char buffer (array)*/

    int lexLength;		/* token length */
    int i;				/* counter */

    unsigned char newc;	/* new char */

    while (1) { /* endless loop broken by token returns it will generate a warning */

        c = bGetCh(sourceBuffer);
        switch (c) {
        case '!':
            bSetMarkOffset(sourceBuffer, bGetChOffset(sourceBuffer));
            newc = bGetCh(sourceBuffer);
            if (newc == '=') {
                currentToken.code = REL_OP_T;
                currentToken.attribute.relationalOperator = NE;
                return currentToken;
            }
            else {
                bRestore(sourceBuffer);
                currentToken.code = ERR_T;
                currentToken.attribute.errLexeme[0] = c;
                currentToken.attribute.errLexeme[1] = CHARSEOF0;
                return currentToken;;
            }
            break;
        case '=':
            newc = bGetCh(sourceBuffer);
            if (newc == '=') {
                currentToken.code = REL_OP_T;
                currentToken.attribute.relationalOperator = EQ;
                return currentToken;
            }

            //Retract buffer
            bRetract(sourceBuffer);
            currentToken.code = ASS_OP_T;
            return currentToken;
            break;
        case '<':
            currentToken.code = REL_OP_T;
            currentToken.attribute.relationalOperator = LT;
            return currentToken;
            break;
        case '>':
            currentToken.code = REL_OP_T;
            currentToken.attribute.relationalOperator = GT;
            return currentToken;
            break;

            //Case for Logical Operators
        case '.':
            bSetMarkOffset(sourceBuffer, bGetChOffset(sourceBuffer));
            newc = bGetCh(sourceBuffer);
            if (newc == 'A' && bGetCh(sourceBuffer) == 'N' && bGetCh(sourceBuffer) == 'D' && bGetCh(sourceBuffer) == '.') {
                currentToken.code = LOG_OP_T;
                currentToken.attribute.logicalOperator = AND;
                return currentToken;
            }
            else if (newc == 'O' && bGetCh(sourceBuffer) == 'R' && bGetCh(sourceBuffer) == '.') {
                currentToken.code = LOG_OP_T;
                currentToken.attribute.logicalOperator = OR;
                return currentToken;
            }
            else if (newc == 'N' && bGetCh(sourceBuffer) == 'O' && bGetCh(sourceBuffer) == 'T' && bGetCh(sourceBuffer) == '.') {
                currentToken.code = LOG_OP_T;
                currentToken.attribute.logicalOperator = NOT;
                return currentToken;

            }
            else {
                bRestore(sourceBuffer);
                currentToken.code = ERR_T;
                currentToken.attribute.errLexeme[0] = c;
                currentToken.attribute.errLexeme[1] = CHARSEOF0;
                return currentToken;
            }
            break;

            //Case for Arithmetic Operators
        case '+':
            bSetMarkOffset(sourceBuffer, bGetChOffset(sourceBuffer));
            newc = bGetCh(sourceBuffer);
            if (newc == '+') {
                currentToken.code = SCC_OP_T;
                return currentToken;
            }
            bRestore(sourceBuffer);
            currentToken.code = ART_OP_T;
            currentToken.attribute.arithmeticOperator = ADD;
            return currentToken;
            break;

        case '-':
            currentToken.code = ART_OP_T;
            currentToken.attribute.arithmeticOperator = SUB;
            return currentToken;
            break;

        case '*':
            currentToken.code = ART_OP_T;
            currentToken.attribute.arithmeticOperator = MUL;
            return currentToken;
            break;

        case '/':
            currentToken.code = ART_OP_T;
            currentToken.attribute.arithmeticOperator = DIV;
            return currentToken;
            break;

            //Bracket Seperators
        case '{':
            currentToken.code = LBR_T;
            return currentToken;
            break;

        case '}':
            currentToken.code = RBR_T;
            return currentToken;
            break;

        case '(':
            currentToken.code = LPR_T;
            return currentToken;
            break;

        case ')':
            currentToken.code = RPR_T;
            return currentToken;
            break;

        case ',':
            currentToken.code = COM_T;
            return currentToken;
            break;

        case ';':
            currentToken.code = EOS_T;
            return currentToken;
            break;

        case CHARSEOF0:
            currentToken.code = SEOF_T;
            currentToken.attribute.seofType = SEOF_0;
            return currentToken;
            break;

        case CHARSEOF255:
            currentToken.code = SEOF_T;
            currentToken.attribute.seofType = SEOF_EOF;
            return currentToken;
            break;

            //% sign
        case '%':
            newc = bGetCh(sourceBuffer);
            if (newc != '%') {
                bRetract(sourceBuffer);
                currentToken.code = ERR_T;
                currentToken.attribute.errLexeme[0] = c;
                currentToken.attribute.errLexeme[1] = CHARSEOF0;
                return currentToken;
            } do {
                c = bGetCh(sourceBuffer);
                if (c == CHARSEOF255 || c == CHARSEOF0) {
                    bRetract(sourceBuffer);
                }
            } while (c != '\n');
            line++;
            break;

        case '\n':
            line++;
            continue;

            //code to detect white space
        case ' ':
        case '\t':
        case '\v':
        case '\f':
        case '\r':

            continue;

            /* ------------------------------------------------------------------------
    Part 2: Implementation of Finite State Machine (DFA)
               or Transition Table driven Scanner
               Note: Part 2 must follow Part 1 to catch the illegal symbols
    -----------------------------------------------------------------------
*/

/* TODO_205: Transition driven scanner implementation inside default */

        default: // general case
            state = nextState(state, c);
            lexStart = bGetChOffset(sourceBuffer) - 1;
            bSetMarkOffset(sourceBuffer, lexStart);

            while (stateType[state] == NOAS) {
                c = bGetCh(sourceBuffer);
                state = nextState(state, c);
            }
            if (stateType[state] == ASWR) {
                bRetract(sourceBuffer);
            }
            lexEnd = bGetChOffset(sourceBuffer);
            lexLength = (lexEnd - lexStart); //+1
            lexemeBuffer = bCreate((short)lexLength + 2, 0, 'f');
            //lexemeBuffer = bCreate((short)lexLength, 0, 'f');

            if (!lexemeBuffer) {
                fprintf(stderr, "Scanner error: Can not create buffer\n");
                exit(1);
            }
            //Copy buffer content to Lexeme
            bRestore(sourceBuffer);
            for (i = 0; i < lexLength; i++) { //lexLength - 1
                bAddCh(lexemeBuffer, bGetCh(sourceBuffer));
            }
            bFinish(lexemeBuffer, '\0');
            //bAddCh(lexemeBuffer, CHARSEOF0);
            currentToken = (*finalStateTable[state])(bGetContent(lexemeBuffer, 0));
            //checkInt = bClean(lexemeBuffer);
            bFree(lexemeBuffer);
            return currentToken;
        } // switch

    } //while

} // tokenizer


/* DO NOT MODIFY THE CODE / COMMENT OF THIS FUNCTION */



/*************************************************************

 Function Name: nextstate
  Purpose:  creates a buffer allocates the buffer on heap
  it sets the buffer struct variables that are given in the parameters
  it sets the mode, the increment, the size, and flags

  Author: Meech Lamirande Kabuasa
  History/Version: 1
  Called Functions:  nextstate;
  Parameters:
    short size (range dependent on mode)
    char increment (range dependent on size in bytes )
  return value: Token
  Algorithm: find the tokens, using the DFA to determine the type of the token that will be assigned this if
    find special car? :
        use the dfa to find the token type?
            the transition tables returns the type of the token depending on the character that is being fed through the
              return the token to be handleded by the parser.

 ************************************************************/


 /*************************************************************
  * Get Next State
     The assert(int test) macro can be used to add run-time diagnostic to programs
     and to "defend" from producing unexpected results.
     - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     (*) assert() is a macro that expands to an if statement;
     if test evaluates to false (zero) , assert aborts the program
     (by calling abort()) and sends the following message on stderr:
     (*) Assertion failed: test, file filename, line linenum.
     The filename and linenum listed in the message are the source file name
     and line number where the assert macro appears.
     - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     If you place the #define NDEBUG directive ("no debugging")
     in the source code before the #include <assert.h> directive,
     the effect is to comment out the assert statement.
     - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     The other way to include diagnostics in a program is to use
     conditional preprocessing as shown bellow. It allows the programmer
     to send more details describing the run-time problem.
     Once the program is tested thoroughly #define DEBUG is commented out
     or #undef DEBUF is used - see the top of the file.
  ************************************************************/

int nextState(int state, char c) {
    int col;
    int next;
    col = nextClass(c);
    next = transitionTable[state][col];
#ifdef DEBUG
    printf("Input symbol: %c Row: %d Column: %d Next: %d \n", c, state, col, next);
#endif
    assert(next != IS);
#ifdef DEBUG
    if (next == IS) {
        printf("Scanner Error: Illegal state:\n");
        printf("Input symbol: %c Row: %d Column: %d\n", c, state, col);
        exit(1);
    }
#endif
    return next;
}

/* TODO_206: Comment this function header */


/*************************************************************

 Function Name: nextstate
  Purpose:  creates a buffer allocates the buffer on heap
  it sets the buffer struct variables that are given in the parameters
  it sets the mode, the increment, the size, and flags

  Author: Meech Lamirande Kabuasa
  History/Version: 1
  Called Functions:  nextstate;
  Parameters:
    short size (range dependent on mode)
    char increment (range dependent on size in bytes )
  return value: Token
  Algorithm: find the tokens, using the DFA to determine the type of the token that will be assigned this if
    find special car? :
        use the dfa to find the token type?
            the transition tables returns the type of the token depending on the character that is being fed through the
              return the token to be handleded by the parser.

 ************************************************************/




 /*************************************************************
  * Get Next Token Class
     * Create a function to return the column number in the transition table:
     * Considering an input char c, you can identify the "class".
     * For instance, a letter should return the column for letters, etc.
 ************************************************************/
int nextClass(char c) {
    int val = -1;
    /* TODO_207: Adjust the logic to return next column in TT */
    /*				[A-z](0),	[0-9](1),	.(2),	$(3),	'(4),	SEOF(5),	other(6) */
    //how to implement DFA.


/*

    if(isdigit(c))
    {
        while(!isdigit(bGetCh(sourceBuffer)));
        if(bGetCh(sourceBuffer)=='.'){
            while(!isdigit(bGetCh(sourceBuffer)));

        }
    }*/
    if (isalpha(c))return 0;
    if (isdigit(c))return 1;

    switch (c) {
    case '.':
        return 2;
    case '$':
        return 3;
    case '\'':
        return 4;
    case CHARSEOF0:
        return 5;
    default:

        return 6;
    }


 //   return val;
}




/* TODO_208: Comment this function header */
/*************************************************************

 Function Name: funcAVID
  Purpose:  this function recgnozing a arithmetic variable identifer


  Author: Meech Lamirande Kabuasa
  History/Version: 1
  Called Functions:  funcAVID;
  Parameters: lexeme the lexeme being passed the function.
  return value: Token
  Algorithm: find the tokens, using the DFA to determine the type of the token that will be assigned this if
    find special car? :
        use the dfa to find the token type?
            the transition tables returns the type of the token depending on the character that is being fed through the
              return the token to be handleded by the parser.

 ************************************************************/

 /* TODO_209: Continue the code... */
Token funcAVID(char lexeme[]) {
    Token currentToken = { 0 };
    int size;
    size = strlen(lexeme);
    int check = isKeyword(lexeme);
    if (check == (-1)) {
        currentToken.code = AVID_T;
        strncpy(currentToken.attribute.vidLexeme, lexeme, VID_LEN);
        currentToken.attribute.vidLexeme[VID_LEN] = CHARSEOF0;
    }
    else {
        currentToken.code = KW_T;
        currentToken.attribute.keywordIndex = check;
    }
    return currentToken;
}

/* TODO_210: Comment this function header */

/*************************************************************

 Function Name: funcSVID
  Purpose:  this function recognizing a arithmetic variable identifer


  Author: Meech Lamirande Kabuasa
  History/Version: 1
  Called Functions:  funcSVID;
  Parameters: lexeme the lexeme being passed the function.
  return value: Token
  Algorithm:
        for the length of the lexeme, copy the attribute length.

 ************************************************************/


 /* TODO_211: Continue the code... */
Token funcSVID(char lexeme[]) {
    Token currentToken = { 0 };
    int size = strlen(lexeme);

    currentToken.code = SVID_T;
    if (size > VID_LEN) {
        lexeme[VID_LEN - 1] = '$';
    }
    strncpy(currentToken.attribute.vidLexeme, lexeme, VID_LEN);
    currentToken.attribute.vidLexeme[VID_LEN] = CHARSEOF0;
    return currentToken;
}

/* TODO_212: Comment this function header */
/*************************************************************

 Function Name: funcIL
  Purpose:  this function recognizing a Integer Literals


  Author: Meech Lamirande Kabuasa
  History/Version: 1
  Called Functions:  funcIL;
  Parameters: lexeme the lexeme being passed the function.
  return value: Token
  Algorithm:
        for the length check if the lexeme is from the alaphabet;
                if return error
          convert it the integer literal;

          check the bounds of the integer



 ************************************************************/

 /* TODO_213: Continue the code... */
Token funcIL(char lexeme[]) {
    Token currentToken = { 0 };
    int num;
    num = atoi(lexeme);

    if (num < SHRT_MIN || num > SHRT_MAX) {
        strcat(currentToken.attribute.errLexeme, "...");
        currentToken = funcErr(lexeme);
    }
    else {
        currentToken.code = INL_T;
        currentToken.attribute.intValue = num;
    }
    return currentToken;
}

/* TODO_214: Comment this function header */
/*************************************************************

 Function Name: funcFPL
  Purpose:  this function recognizing a Floating Point Literals.


  Author: Meech Lamirande Kabuasa
  History/Version: 1
  Called Functions:  funcFPL;
  Parameters: lexeme the lexeme being passed the function.
  return value: Token
  Algorithm:
        check the legitmacy of the floating point literal
              check the bound.
               if not? return error token


 ************************************************************/

 /* TODO_215: Continue the code... */
Token funcFPL(char lexeme[]) {
    Token currentToken = { 0 };
    double newNum = atof(lexeme);

    if ((strlen(lexeme) > 7) && (newNum < FLT_MIN || newNum > FLT_MAX || newNum < 0)) {
        strcat(currentToken.attribute.errLexeme, "...");
        currentToken = funcErr(lexeme);
    }
    else {
        currentToken.code = FPL_T;
        currentToken.attribute.floatValue = newNum;
    }
    return currentToken;
}

/* TODO_216: Comment this function header */
/*************************************************************

 Function Name: funcSL
  Purpose:  this function recognizing a String literals

  Author: Meech Lamirande Kabuasa
  History/Version: 1
  Called Functions:  funcSL;
  Parameters: lexeme the lexeme being passed the function.
  return value: Token
  Algorithm:
  if the tokens are in between single brackets,
    add the lexeme to the string literals
       if an illegal character like terminators are encountered return the error token


 ************************************************************/

 /* TODO_217: Continue the code... */
Token funcSL(char lexeme[]) {
    Token currentToken = { 0 };
    int size = strlen(lexeme);
    currentToken.attribute.contentString = bGetAddChOffset(stringLiteralTable);
    //for (int i = 1; i < size - 1; i++) {
    for (int i = 0; i < size - 1; i++) {
        //if (lexeme[i] == '\n') {
        if (lexeme[i+1] == '\n') {
            bAddCh(stringLiteralTable, lexeme[i]); //new
            line++;
        }
        else if (lexeme[i+1] == '\'') { //new
            /*skip*/
        }
        else { //new
            bAddCh(stringLiteralTable, lexeme[i]);
        }
        //bAddCh(stringLiteralTable, lexeme[i]);
    }
    if (!bAddCh(stringLiteralTable, CHARSEOF0)) {
        currentToken.code = RTE_T;
        strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
        errorNumber = 3;
        return currentToken;
    }
    currentToken.code = STR_T;
    return currentToken;
}

/* TODO_218: Comment this function header */
/*************************************************************
 Function Name: funcErr
  Purpose:  this function recognizing a arithmetic variable identifer

  Author: Meech Lamirande Kabuasa
  History/Version: 1
  Called Functions:  funcSL;
  Parameters: lexeme the lexeme being passed the function.
  return value: Token
  Algorithm:
    this finds input the error lexeme into it's attribute and returns the Error Token.

 ************************************************************/

 /* TODO_219: Continue the code... */
Token funcErr(char lexeme[]) {
    Token currentToken = { 0 };
    int size = strlen(lexeme);
    currentToken.code = ERR_T;
    if (size > ERR_LEN) {
        currentToken.code = ERR_T;
        strncpy(currentToken.attribute.errLexeme, lexeme, ERR_LEN - 3);
        currentToken.attribute.errLexeme[ERR_LEN - 3] = '\0';
        strcat(currentToken.attribute.errLexeme, "...");
    }
    else {
        strcpy(currentToken.attribute.errLexeme, lexeme);
    }
    for (int i = 0; i < size - 1; i++) {
        if (lexeme[i] == '\n') {
            line++;
        }
    }
    return currentToken;
}

/* TODO_220: Comment this function header */
/*************************************************************
 Function Name: isKeyword
  Purpose: this function recongnizes a keyword token.

  Author: Meech Lamirande Kabuasa
  History/Version: 1
  Called Functions:  funcSL;
  Parameters: lexeme the lexeme being passed the function.
  return value: Token
  Algorithm:
   return the comparable keyword.

 ************************************************************/
 /* TODO_221: Continue the code... */

int isKeyword(char lexeme[]) {
    int i = -1;
    for (int x = 0; x < KWT_SIZE; x++) {
        if (strcmp(lexeme, keywordTable[x]) == 0) {
            i = x;
        }
    }
    return i;
}


/*
TODO_222: (If necessary): HERE YOU WRITE YOUR ADDITIONAL FUNCTIONS (IF ANY).
*/
