/*
 * File: parser.cpp
 * ----------------
 * Implements the parser.h interface.
 */

#include <iostream>
#include <string>

#include "exp.h"
#include "parser.h"
#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/strlib.h"
#include "../StanfordCPPLib/tokenscanner.h"
#include "statement.h"

using namespace std;

/*
 * Implementation notes: parseExp
 * ------------------------------
 * This code just reads an expression and then checks for extra tokens.
 */
//process a expression
Expression *parseExp(TokenScanner & scanner) {
   Expression *exp = readE(scanner);
   if (scanner.hasMoreTokens()) {
      error("parseExp: Found extra token: " + scanner.nextToken());
   }
   return exp;
}
//process a statement

bool checkStatement(string token){
    if (token == "REM" || token == "LET" || token == "PRINT" || token == "INPUT" || token == "END" || token == "RUN" || token == "LIST" || token == "CLEAR" || token == "QUIT" || token == "HELP" || token == "GOTO" || token == "IF" || token == "THEN")return true;
    return false;
    //R(EM) L(ET)   P(RINT)  I(NPUT)  E(ND)  G(OTO) I(F) T(HEN)
    //R(UN) L(IST)  C(LEAR)  Q(UIT)   H(ELP)
}
Statement *parseStatement(TokenScanner & scanner, string line_message){//BUG 1
    //scanner.scanNumbers();
    //scanner.ignoreWhitespace();
    string token = scanner.nextToken();
    TokenType tokentype = scanner.getTokenType(token);
    int LINE_NUM;

    Expression *exp = nullptr;
    if (tokentype != WORD){
        error("SYNTAX ERROR");
    }
     else if (token == "REM"){
        return new REM();
    }else if (token == "LET"){
        if (!scanner.hasMoreTokens()){
            delete exp;
            error("SYNTAX ERROR");
        }
        exp = parseExp(scanner);
        if (exp->getType() != COMPOUND){
            delete exp;
            error("SYNTAX ERROR");
        }
        if (((CompoundExp *)exp)->getOp() != "=" ){
            delete exp;
            error("SYNTAX ERROR");
        }
        if (((CompoundExp*)exp)->getOp() != "="){
            delete exp;
            error("SYNTAX ERROR");
        }
        if (checkStatement(((IdentifierExp*)((CompoundExp*)exp)->getLHS())->getName())){
            delete exp;
            error("SYNTAX ERROR");
        }
        return new LET(exp);
    }else if (token == "PRINT"){//??why bug
        exp = parseExp(scanner);
        if (exp->getType() == COMPOUND){
            //复合表达式的特判
            if (((CompoundExp*)exp)->getOp() == "=" ||((CompoundExp*)exp)->getOp() == " > " || ((CompoundExp*)exp)->getOp() == "<"  ){
                delete exp;
                error("SYNTAX ERROR");
            }
        }
        return new PRINT(exp);
    }else if (token == "INPUT"){
        if (!scanner.hasMoreTokens())error("SYNTAX ERROR");
        token = scanner.nextToken();
        tokentype = scanner.getTokenType(token);
        if (tokentype != WORD)       error("SYNTAX ERROR");
        if (scanner.hasMoreTokens()) error("SYNTAX ERROR");
        return new INPUT(token);
    }else if (token == "IF"){
        if (!scanner.hasMoreTokens())error("SYNTAX ERROR");
        Expression *expleft;
        Expression *expright;
        string operate;
        GOTO *go_go_to;
        if (line_message.find('=') == string::npos){//赵一龙思路：没有等号
            expleft = readE(scanner);
            operate = scanner.nextToken();
            if (operate != ">" && operate != "<" && operate != "="){
                delete expleft;
                error("SYNTAX ERROR");
            } else{
                try {
                    expright = readE(scanner);
                } catch (...) {
                    delete expleft;
                    error("SYNTAX ERROR");
                }
                token = scanner.nextToken();
                if (token != "THEN"){
                    delete expleft;delete expright;
                    error("SYNTAX ERROR");
                } else{
                    token = scanner.nextToken();
                    tokentype = scanner.getTokenType(token);

                    if (tokentype != NUMBER)error("SYNTAX ERROR");
                    if (scanner.hasMoreTokens())error("SYNTAX ERROR");
                    try {LINE_NUM = stringToInteger(token);} catch (...) {
                        delete expleft;delete expright;
                        error("SYNTAX ERROR");
                    }
                    go_go_to = new GOTO(LINE_NUM);
                    return new IF(expleft, operate, expright, go_go_to);
                }
            }
        } else{//赵一龙思路
            operate = "=";//为什么不能直接return new IF(...,"=",...)?
            if (line_message.find_first_of('=') != line_message.find_last_of('='))error("SYNTAX ERROR");
            string left;
            while (scanner.hasMoreTokens()){
                token = scanner.nextToken();
                if (token == "=")break;
                left += (token + " ");
            }
            expright = readE(scanner);
            token = scanner.nextToken();
            if (token != "THEN"){
                delete expright;
                error("SYNTAX ERROR");
            }
            token = scanner.nextToken();
            tokentype = scanner.getTokenType(token);
            if (tokentype != NUMBER){
                delete expright;
                error("SYNTAX ERROR");
            }
            if (scanner.hasMoreTokens()){
                delete expright;
                error("SYNTAX ERROR");
            }
            try {
                scanner.setInput(left);
                expleft = readE(scanner);
            }catch (...){
                delete expleft;
                delete expright;
                error("SYNTAX ERROR");
            }
            try {
                LINE_NUM = stringToInteger(token);
            }catch (...){
                delete expright;
                error("SYNTAX ERROR");
            }
            go_go_to = new GOTO(LINE_NUM);
            return new IF(expleft, operate, expright, go_go_to);
        }
    }else if (token == "END"){
        if (scanner.hasMoreTokens())    error("SYNTAX ERROR");
        return new END();
    }else if (token == "GOTO"){

        if (!scanner.hasMoreTokens())error("SYNTAX ERROR");
        token = scanner.nextToken();
        tokentype = scanner.getTokenType(token);
        if (tokentype != NUMBER)        error("SYNTAX ERROR");
        if (scanner.hasMoreTokens())   error("SYNTAX ERROR");
        try {
            LINE_NUM = stringToInteger(token);
        }catch (...){
            error("SYNTAX ERROR");
        }
        return new GOTO(LINE_NUM);
    }
    error("SYNTAX ERROR");
}

/*
 * Implementation notes: readE
 * Usage: exp = readE(scanner, prec);
 * ----------------------------------
 * This version of readE uses precedence to resolve the ambiguity in
 * the grammar.  At each recursive level, the parser reads operators and
 * subexpressions until it finds an operator whose precedence is greater
 * than the prevailing one.  When a higher-precedence operator is found,
 * readE calls itself recursively to read in that subexpression as a unit.
 */
//不太懂
Expression *readE(TokenScanner & scanner, int prec) {
   Expression *exp = readT(scanner);
   string token;
   while (true) {
      token = scanner.nextToken();
      int newPrec = precedence(token);
      if (newPrec <= prec) break;
      Expression *rhs = readE(scanner, newPrec);
      exp = new CompoundExp(token, exp, rhs);
   }
   scanner.saveToken(token);
   return exp;
}

/*
 * Implementation notes: readT
 * ---------------------------
 * This function scans a term, which is either an integer, an identifier,
 * or a parenthesized subexpression.
 */

Expression *readT(TokenScanner & scanner) {
   string token = scanner.nextToken();
   TokenType type = scanner.getTokenType(token);
   if (type == WORD) return new IdentifierExp(token);
   if (type == NUMBER) return new ConstantExp(stringToInteger(token));
   if (token != "(") error("Illegal term in expression");
   Expression *exp = readE(scanner);
   if (scanner.nextToken() != ")") {
      error("Unbalanced parentheses in expression");
   }
   return exp;
}

/*
 * Implementation notes: precedence
 * --------------------------------
 * This function checks the token against each of the defined operators
 * and returns the appropriate precedence value.
 */

int precedence(string token) {
   if (token == "=") return 1;
   if (token == "+" || token == "-") return 2;
   if (token == "*" || token == "/") return 3;
   return 0;
}
