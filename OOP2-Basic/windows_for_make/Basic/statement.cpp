/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include <string>
#include "statement.h"
#include "parser.h"
#include "../StanfordCPPLib/simpio.h"
//Expression类：表达式
//EvalState类： 存储变量（变量名、变量值）。一个map类，存储由string 到 int的映射
//Statement类： 声明类型
using namespace std;

/* Implementation of the Statement class */

Statement::Statement() {
   /* Empty */
}

Statement::~Statement() {
   /* Empty */
}

REM::REM() = default;
REM::~REM() = default;
void REM::execute(EvalState &state) {
}

//LET:  compound expression
LET::LET(Expression *exp):expression(exp){//expression(exp)??
}
LET::~LET() {
    delete expression;
}
void LET::execute(EvalState &state) {
    expression->eval(state);
}

PRINT::PRINT(Expression *exp) :expression(exp){}
PRINT::~PRINT() {delete expression;}
void PRINT::execute(EvalState &state) {
    cout<<expression->eval(state)<<endl;
}

INPUT::INPUT(string &name) :name(name){}
INPUT::~INPUT() noexcept{}
void INPUT::execute(EvalState &state) {
    cout<<" ? ";
    string token;//语句块
    TokenScanner scanner;
    TokenType token_type;
    scanner.ignoreComments();
    scanner.ignoreWhitespace();
    while (true){
        int value;
        scanner.setInput(getLine());//输入
        if (!scanner.hasMoreTokens())continue;
        //读入下一个语块
        token = scanner.nextToken();
        token_type = scanner.getTokenType(token);

        if (token!="-"){//正数
            if (token_type != NUMBER) {
                cout << "INVALID NUMBER" << endl << " ? ";
                continue;
            }
            if (scanner.hasMoreTokens()){
                cout << "INVALID NUMBER" << endl << " ? ";
                continue;
            }
            try {
                value  = stringToInteger(token);
            } catch (...) {
                cout << "INVALID NUMBER" << endl << " ? ";
                continue;
            }
        } else{//负数
            token = scanner.nextToken();
            token_type = scanner.getTokenType(token);
            if (token_type != NUMBER) {
                cout << "INVALID NUMBER" << endl << " ? ";
                continue;
            }
            if (scanner.hasMoreTokens()){
                cout << "INVALID NUMBER" << endl << " ? ";
                continue;
            }
            try {
                value  = -stringToInteger(token);//-Integer
            } catch (...) {
                cout << "INVALID NUMBER" << endl << " ? ";
                continue;
            }
        }

        state.setValue(name,value);//储存变量
        break;
    }
}

END::END() = default;
END::~END() = default;
void END::execute(EvalState &state) {
    error("END");
}

GOTO::GOTO(int line_number):line_number(line_number) {}
GOTO::~GOTO() = default;
void GOTO::execute(EvalState &state) {
    error(integerToString(line_number));
}

IF::IF(Expression *exp1, string &op, Expression *exp2, GOTO *line):exp1(exp1),op(op),exp2(exp2),line(line){}
IF::~IF() {
    delete exp1;
    delete exp2;
    delete line;
}
void IF::execute(EvalState &state) {
    int cmp1 = exp1->eval(state);
    int cmp2 = exp2->eval(state);
    if (op == "="){
        if (cmp1 == cmp2)line->execute(state);
        else return;
    }
    if (op == "<"){
        if (cmp1 <  cmp2)line->execute(state);
        else return;
    }
    if (op == ">"){
        if (cmp1 >  cmp2)line->execute(state);
        else return;
    }
}

