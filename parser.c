#include "parser.h"

void startParser(void) {
    lookahead = tokenizer();
    program();
    matchToken(SEOF_T, NO_ATTR);
    printf("%s\n", "PLATY: Source file parsed");
}

void matchToken(int tokenCode, int tokenAttribute) {
    int matchFlag = 1; /* dont know how to implement*/

    if (lookahead.code != tokenCode) {
        matchFlag = 0;
    }

    switch (lookahead.code) {
    case KW_T:case LOG_OP_T: case ART_OP_T: case REL_OP_T:
        if (lookahead.attribute.codeType != tokenAttribute) {
            matchFlag = 0;
        }
        break;
      /*
    default:
		if (tokenCode == lookahead.code && (lookahead.code >= SEOF_T && lookahead.code <= EOS_T)) {
			matchFlag = 1;
		}else{
			matchFlag = 0;
		}
		break;
      */
    }
    if (matchFlag && lookahead.code == SEOF_T)
        return;
    if (matchFlag) {
        lookahead = tokenizer();
        if (lookahead.code == ERR_T) {
            printError();
            lookahead = tokenizer();
            syntaxErrorNumber++;
            //return;
        }
    }
    else
        syncErrorHandler(tokenCode);

}

void syncErrorHandler(int syncTokenCode) {
    printError();
    syntaxErrorNumber++;
    while (lookahead.code != syncTokenCode) {
        if (lookahead.code == SEOF_T) {
            exit(syntaxErrorNumber);
        }
        lookahead = tokenizer();
    }
    if (lookahead.code != SEOF_T) {
        lookahead = tokenizer();
    }
}

void printError() {
    Token t = lookahead;
    printf("PLATY: Syntax error:  Line:%3d\n", line);
    printf("*****  Token code:%3d Attribute: ", t.code);
    switch (t.code) {
    case ERR_T: /*Error Token */
        printf("%s\n", t.attribute.errLexeme);
        break;
    case SEOF_T:/*Source End of File 0*/
        printf("NA\n");
        break;
    case AVID_T: case SVID_T:/*Arithmatic and String Variable Identifiers 1 ,2*/
        printf("%s\n", t.attribute.vidLexeme);
        break;
    case FPL_T: /*Floating point literal 3*/
        //printf("%5.1f\n", t.attribute.floatValue);
        printf("  %.1f\n", t.attribute.floatValue);
        break;
    case INL_T:/* Integer literal token 4*/
        printf("%d\n", t.attribute.intValue);
        break;
    case STR_T:/* String literal token 5*/
        printf("%s\n", bGetContent(stringLiteralTable, t.attribute.contentString));
        break;
    case SCC_OP_T: /* String concatenation operator token: ++ 6*/
        printf("NA\n");
        break;
    case ASS_OP_T: /* Assignment operator token 7*/
        printf("NA\n");
        break;
    case ART_OP_T:  /* Arithmetic operator token 8*/
        printf("%d\n", t.attribute.arithmeticOperator);
        break;
    case REL_OP_T: /* Relational operator token 9*/
        printf("%d\n", t.attribute.relationalOperator);
        break;
    case LOG_OP_T:/* Logical operator token 10*/
        printf("%d\n", t.attribute.logicalOperator);
        break;
    case LPR_T: /* Left parenthesis token 11*/
        printf("NA\n");
        break;
    case RPR_T: /* Right parenthesis token 12*/
        printf("NA\n");
        break;
    case LBR_T:/* Left brace token 13*/
        printf("NA\n");
        break;
    case RBR_T:  /* Right brace token 15*/
        printf("NA\n");
        break;
    case KW_T:  /* Keyword token 16*/
        printf("%s\n", keywordTable[t.attribute.keywordIndex]);
        break;
    case COM_T: /* Comma token 17*/
        printf("NA\n");
        break;
    case EOS_T:/* End of statement (semicolon) 18*/
        printf("NA\n");
        break;
    default:
        printf("PLATY: Scanner error: invalid token code: %d\n", t.code);
    }
}

void program(void) {
    matchToken(KW_T, MAIN);
    matchToken(LBR_T, NO_ATTR);
    optionalStatements();
    matchToken(RBR_T, NO_ATTR);
    printf("%s\n", "PLATY: Program parsed");
}

void optionalStatements(void) {
    switch (lookahead.code) {
    case AVID_T: case SVID_T:
        statements();
        break;
    case KW_T:
        if (lookahead.attribute.codeType == IF
            || lookahead.attribute.codeType == WHILE
            || lookahead.attribute.codeType == READ
            || lookahead.attribute.codeType == WRITE) {
            statements();
        }
        break;
    }
    printf("%s\n", "PLATY: Optional statements parsed");
}

void statements(void) {
    statement();
    statementsPrime();
    printf("%s\n", "PLATY: Statements parsed");
}

void statementsPrime(void) {
    switch (lookahead.code) {
    case AVID_T: case SVID_T:
        statement();
        statementsPrime();
        break;
    case KW_T:
        if (lookahead.attribute.codeType == IF || lookahead.attribute.codeType == WHILE ||
            lookahead.attribute.codeType == READ || lookahead.attribute.codeType == WRITE) {
            statement();
            statementsPrime();
        }
        break;
    }
}

void statement(void) {
    switch (lookahead.code) {
    case AVID_T: case SVID_T:
        assignmentStatement();
        break;
    case KW_T:
        switch (lookahead.attribute.codeType) {
        case IF: selectionStatement(); break;
        case WHILE: iterationStatement(); break;
        case READ: inputStatement(); break;
        case WRITE: outputStatement(); break;
        }
        break;
    default:
        printError();

    }
    printf("%s\n", "PLATY: Statement parsed");
}

void assignmentStatement(void) {
    assignmentExpression();
    matchToken(EOS_T, NO_ATTR);
    printf("%s\n", "PLATY: Assignment statement parsed");
}

void assignmentExpression(void) {
    switch (lookahead.code) {
    case AVID_T:
        matchToken(AVID_T, NO_ATTR);
        matchToken(ASS_OP_T, NO_ATTR);
        arithmeticExpression();
        printf("%s\n", "PLATY: Assignment expression parsed");
        break;
    case SVID_T:
        matchToken(SVID_T, NO_ATTR);
        matchToken(ASS_OP_T, NO_ATTR);
        stringExpression();
        printf("%s\n", "PLATY: Assignment expression parsed");
        break;
    default:
        printError();
    }
}

void selectionStatement(void) {
    /*Matching If*/
    matchToken(KW_T, IF);
    preCondition();
    matchToken(LPR_T, NO_ATTR);
    conditionalExpression();
    matchToken(RPR_T, NO_ATTR);
    /*Matching Then*/
    matchToken(KW_T, THEN);
    matchToken(LBR_T, NO_ATTR);
    optionalStatements();
    matchToken(RBR_T, NO_ATTR);
    /*Matching Else*/
    matchToken(KW_T, ELSE);
    matchToken(LBR_T, NO_ATTR);
    optionalStatements();
    matchToken(RBR_T, NO_ATTR);
    matchToken(EOS_T, NO_ATTR);
    printf("%s\n", "PLATY: Selection statement parsed");
}

void iterationStatement(void) {
    /*Matching keyword While*/
    matchToken(KW_T, WHILE);
    preCondition();
    matchToken(LPR_T, NO_ATTR);
    conditionalExpression();
    matchToken(RPR_T, NO_ATTR);
    /*Matching keyword Do*/
    matchToken(KW_T, DO);
    matchToken(LBR_T, NO_ATTR);
    statements();
    matchToken(RBR_T, NO_ATTR);
    matchToken(EOS_T, NO_ATTR);

    printf("%s\n", "PLATY: Iteration statement parsed");

}

//maybe change
void preCondition(void) {
    if (lookahead.code == KW_T) {
        //printf("hook *******************************************************");
        switch (lookahead.attribute.codeType) {
        case TRUE: case FALSE:
            matchToken(KW_T, lookahead.attribute.codeType);
            //why is this not showing up?
            printf("%s\n", "PLATY: Pre-condition parsed");
            break;
        default:
            printError();
        }
    }
    else
        printError();
}

void inputStatement(void) {
    matchToken(KW_T, READ);
    matchToken(LPR_T, NO_ATTR);
    variableList();
    matchToken(RPR_T, NO_ATTR);
    matchToken(EOS_T, NO_ATTR);
    printf("%s\n", "PLATY: Input statement parsed");
}

void variableList(void) {
    variableIdentifier();
    variableListPrime();
    printf("%s\n", "PLATY: Variable list parsed");
}

void variableListPrime(void) {
    if (lookahead.code == COM_T) {
        matchToken(COM_T, NO_ATTR);
        variableIdentifier();
        variableListPrime();
    }
}

void variableIdentifier(void) {
    switch (lookahead.code) {
    case AVID_T:
        matchToken(AVID_T, NO_ATTR);
        break;
    case SVID_T:
        matchToken(SVID_T, NO_ATTR);
        break;
    default:
        printError();
    }
    printf("%s\n", "PLATY: Variable identifier parsed");
}

void outputStatement(void) {
    matchToken(KW_T, WRITE);
    matchToken(LPR_T, NO_ATTR);
    //outputStatementPrime();
    OutputvariableList();
    matchToken(RPR_T, NO_ATTR);
    matchToken(EOS_T, NO_ATTR);
    printf("%s\n", "PLATY: Output statement parsed");
}

//does not exist ->important note: yeet
void outputStatementPrime(void) {
    switch (lookahead.code) {
    case STR_T:
        matchToken(STR_T, NO_ATTR);
        printf("%s\n", "PLATY: Output variable list parsed");
        break;
    default:
        variableList();
    }
}

void OutputvariableList(void) {
    switch (lookahead.code) {
    case AVID_T: case SVID_T:
        variableList();
        break;
    case STR_T:
        matchToken(STR_T, NO_ATTR);
        break;
    default:
        break;
    }
    printf("%s\n", "PLATY: Output variable list parsed");
}

void arithmeticExpression(void) {
    switch (lookahead.code) {
    case AVID_T: case FPL_T: case INL_T: case LPR_T:
        additiveArithmeticExpression();
        printf("%s\n", "PLATY: Arithmetic expression parsed");
        break;
    case ART_OP_T:
        switch (lookahead.attribute.arithmeticOperator) {
        case ADD: case SUB:
            unaryArithmeticExpression();
            printf("%s\n", "PLATY: Arithmetic expression parsed");
            break;
        default:
            printError();
        }
    }
}

void unaryArithmeticExpression(void) {
    switch (lookahead.code) {
    case ART_OP_T:
        switch (lookahead.attribute.arithmeticOperator) {
        case ADD:
            matchToken(ART_OP_T, ADD);
            primaryArithmeticExpression();
            break;
        case SUB:
            matchToken(ART_OP_T, SUB);
            primaryArithmeticExpression();
            break;
        default:
            printError();
        }
        break;
    default:
        printError();
    }
}

void additiveArithmeticExpression(void) {
    multiplicativeArithmeticExpression();
    additiveArithmeticExpressionPrime();
    printf("%s\n", "PLATY: Additive arithmetic expression parsed");

}

void additiveArithmeticExpressionPrime(void) {

    //maybe change back to hardcode -
    switch (lookahead.code) {
    case ART_OP_T:
        switch (lookahead.attribute.arithmeticOperator) {
        case ADD:
            matchToken(ART_OP_T, lookahead.attribute.arithmeticOperator);
            multiplicativeArithmeticExpression();
            additiveArithmeticExpressionPrime();
            break;
        case SUB:
            matchToken(ART_OP_T, lookahead.attribute.arithmeticOperator);
            multiplicativeArithmeticExpression();
            additiveArithmeticExpressionPrime();
            break;

        }
    }
}

void multiplicativeArithmeticExpression(void) {
    primaryArithmeticExpression();
    multiplicativeArithmeticExpressionPrime();
    printf("%s\n", "PLATY: Multiplicative arithmetic expression parsed");
}

void multiplicativeArithmeticExpressionPrime(void) {
    switch (lookahead.code) {
    case ART_OP_T:
        switch (lookahead.attribute.arithmeticOperator) {
        case MUL:
            matchToken(ART_OP_T, MUL);
            primaryArithmeticExpression();
            multiplicativeArithmeticExpressionPrime();
            break;
        case DIV:
            matchToken(ART_OP_T, DIV);
            primaryArithmeticExpression();
            multiplicativeArithmeticExpressionPrime();
            break;
        }
    }
}

void primaryArithmeticExpression(void) {
    switch (lookahead.code) {
    case AVID_T: case FPL_T: case INL_T:
        matchToken(lookahead.code, NO_ATTR);
        printf("%s\n", "PLATY: Primary arithmetic expression parsed");
        break;
    case LPR_T:
        matchToken(LPR_T, NO_ATTR);
        arithmeticExpression();
        matchToken(RPR_T, NO_ATTR);
        printf("%s\n", "PLATY: Primary arithmetic expression parsed");
        break;
    default:
        printError();
    }
}

void stringExpression(void) {
    primaryStringExpression();
    stringExpressionPrime();
    printf("%s\n", "PLATY: String expression parsed");
}

void stringExpressionPrime(void) {
    switch (lookahead.code) {
    case SCC_OP_T:
        matchToken(SCC_OP_T, NO_ATTR);
        primaryStringExpression();
        stringExpressionPrime();
        break;
    }
}

void primaryStringExpression(void) {
    switch (lookahead.code) {
    case SVID_T: case STR_T:
        matchToken(lookahead.code, NO_ATTR);
        printf("%s\n", "PLATY: Primary string expression parsed");
        break;
    default:
        printError();
    }
}

void conditionalExpression(void) {
    logicalOrExpression();
    printf("%s\n", "PLATY: Conditional expression parsed");
}

void logicalOrExpression(void) {
    logicalAndExpression();
    logicalOrExpressionPrime();
    printf("%s\n", "PLATY: Logical Or Expression parsed");
}

void logicalOrExpressionPrime(void) {
    switch (lookahead.code) {
    case LOG_OP_T:
        switch (lookahead.attribute.logicalOperator) {
        case OR:
            matchToken(LOG_OP_T, OR);
            logicalAndExpression();
            logicalOrExpressionPrime();
            printf("%s\n", "PLATY: Logical OR expression parsed");
            break;
        }
        break;
    }
}

void logicalAndExpression(void) {
    logicalNotExpression();
    logicalAndExpressionPrime();
    printf("%s\n", "PLATY: Logical And Expression parsed");
}

void logicalAndExpressionPrime(void) {
    switch (lookahead.code) {
    case LOG_OP_T:
        switch (lookahead.attribute.logicalOperator) {
        case AND:
            matchToken(LOG_OP_T, AND);
            logicalNotExpression();
            logicalAndExpressionPrime();
            printf("%s\n", "PLATY: Logical AND expression parsed");
            break;
        }
        break;
    }
}

void logicalNotExpression(void) {
    //printf("hooooooooooooooooooooooooooooooooooooooook am i making it here?");
    switch (lookahead.code) {
    case LOG_OP_T:
        matchToken(LOG_OP_T, NOT);
        relationalExpression();
        break;
    case AVID_T: case SVID_T: case FPL_T: case INL_T: case STR_T:
        //printf("hoooooooooooooooook am i making it here?");
        relationalExpression();
        break;
    default:
        printError();
        printf("%s\n", "PLATY: Relational expression parsed");
    }
    printf("%s\n", "PLATY: Logical Not Expression parsed");

}

void relationalExpression(void) {
    switch (lookahead.code) {
    case AVID_T: case FPL_T: case INL_T:
        relationalArithmeticExpression();
        printf("%s\n", "PLATY: Relational expression parsed");
        break;
    case SVID_T: case STR_T:
        relationalStringExpression();
        //	primaryRelationalStringExpression();
        printf("%s\n", "PLATY: Relational expression parsed");
        break;
    default:
        printError();
    }
}

void relationalArithmeticExpression(void) {
    primaryRelationalArithmeticExpression();
    relationalArithmeticExpressionPrime();
    primaryRelationalArithmeticExpression();
    printf("%s\n", "PLATY: Relational arithmetic operator parsed");
    printf("%s\n", "PLATY: Relational arithmetic expression parsed");
}

void relationalArithmeticExpressionPrime(void) {
    if (lookahead.code == REL_OP_T) {
        matchToken(REL_OP_T, lookahead.attribute.relationalOperator);
    }
    else {
        printError();
    }
}

void primaryRelationalArithmeticExpression(void) {
    switch (lookahead.code) {
    case AVID_T: case FPL_T: case INL_T:
        matchToken(lookahead.code, NO_ATTR);
        printf("%s\n", "PLATY: Primary relational arithmetic expression parsed");
        break;
    }
}

void relationalStringExpression(void) {
    primaryRelationalStringExpression();
    relationalStringExpressionPrime();
    primaryRelationalStringExpression();
    printf("%s\n", "PLATY: Relational string operator parsed");
    printf("%s\n", "PLATY: Relational string expression parsed");
}

void relationalStringExpressionPrime(void) {
    if (lookahead.code == REL_OP_T) {
        matchToken(REL_OP_T, lookahead.attribute.relationalOperator);
    }
    else {
        printError();
    }
}

void primaryRelationalStringExpression(void) {
    primaryStringExpression();
    printf("%s\n", "PLATY: Primary relational string expression parsed");
}
