/*
 * File: parser.h
 * --------------
 * This file acts as the interface to the parser module.
 */

#ifndef _parser_h
#define _parser_h

#include <string>
#include "exp.h"

#include "../StanfordCPPLib/tokenscanner.h"
#include "statement.h"

/*
 * Function: parseExp
 * Usage: Expression *exp = parseExp(scanner);
 * -------------------------------------------
 * Parses an expression by reading tokens from the scanner, which must
 * be provided by the client.  The scanner should be set to ignore
 * whitespace and to scan numbers.
 */

Expression *parseExp(TokenScanner & scanner);

/*
 * Function: readE
 * Usage: Expression *exp = readE(scanner, prec);
 * ----------------------------------------------
 * Returns the next expression from the scanner involving only operators
 * whose precedence is at least prec.  The prec argument is optional and
 * defaults to 0, which means that the function reads the entire expression.
 */

Expression *readE(TokenScanner & scanner, int prec = 0);

/*
 * Function: readT
 * Usage: Expression *exp = readT(scanner);
 * ----------------------------------------
 * Returns the next individual term, which is either a constant, an
 * identifier, or a parenthesized subexpression.
 */
//*返回下一个单独的项，它可以是一个常量，一个标识符，或者一个括起来的子表达式。
Expression *readT(TokenScanner & scanner);

/*
 * Function: precedence
 * Usage: int prec = precedence(token);
 * ------------------------------------
 * Returns the precedence of the specified operator token.  If the token
 * is not an operator, precedence returns 0.
 */
//*返回指定操作符的优先级。如果标记不是操作符，优先级返回0。
int precedence(std::string token);

bool checkStatement(string & token);

Statement *parseStatement(TokenScanner & scanner, string line_message);

#endif
