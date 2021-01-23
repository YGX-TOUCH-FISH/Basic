/*
 * File: Basic.cpp
 * ---------------
 * Name: [TODO: YGX]
 * Section: [TODO: enter section leader here]
 * This file is the starter project for the BASIC interpreter from
 * Assignment #6.
 * [TODO: extend and correct the documentation]
 */

#include <cctype>
#include <iostream>
#include <string>
#include "exp.h"
#include "parser.h"
#include "program.h"
#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/tokenscanner.h"

#include "../StanfordCPPLib/simpio.h"
#include "../StanfordCPPLib/strlib.h"
using namespace std;

/* Function prototypes */

void processLine(string line, Program & program, EvalState & state);

/* Main program */

int main() {
   EvalState state;//储存所有变量
   Program program;
   cout << "Stub implementation of BASIC" << endl;
   while (true) {
      try {
         processLine(getLine(), program, state);
      } catch (ErrorException & ex) {
         cerr << "Error: " << ex.getMessage() << endl;
      }
   }//error.cpp
   return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version,
 * the implementation does exactly what the interpreter program
 * does in Chapter 19: read a line, parse it as an expression,
 * and then print the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */

//void processLine(string line, Program & program, EvalState & state) {
//    //program.cpp;evalstate.cpp;tokenscanner.cpp;statement.cpp
//   TokenScanner scanner;
//   scanner.ignoreWhitespace();
//   scanner.scanNumbers();
//   scanner.setInput(line);
//   Expression *exp = parseExp(scanner);//parser.cpp
//   int value = exp->eval(state);
//   cout << value << endl;
//   delete exp;
//}
void processLine(string line, Program  & program , EvalState & state){
    string token;
    TokenType token_type;
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.setInput(line);
    //* Sets the token stream for this scanner to the specified string or
    //* input stream.  Any previous token stream is discarded.
    if (scanner.hasMoreTokens()){
        token = scanner.nextToken();
        token_type = scanner.getTokenType(token);
    } else return;

    if (token_type == WORD){
        if (token == "RUN"){
            if (scanner.hasMoreTokens()){
                cout<<"SYNTAX ERROR"<<endl;return;
            }
            try {
                program.model_run(state);
            }catch (ErrorException &err){
                if (err.getMessage() == "FAIL TO GOTO"){
                    cout<<"FAIL TO GOTO"<<endl;return;
                }
                if (err.getMessage() == "DIVIDE BY ZERO"){
                    cout<<"DIVIDE BY ZERO"<<endl;return;
                } else{
                    cout<<"VARIABLE NOT DEFINED"<<endl;return;
                }
            }
        } else if (token == "LIST"){
            if (scanner.hasMoreTokens()){
                cout<<"SYNTAX ERROR"<<endl;return;
            }
            try {
                program.model_list();
            }catch (...){
                cout<<"SYNTAX ERROR";
            }
        } else if (token == "CLEAR"){
            if (scanner.hasMoreTokens()){
                cout<<"SYNTAX ERROR"<<endl;return;
            }
            program.clear();
            state.clear();
        } else if (token == "QUIT"){
            exit(0);
        } else if (token == "HELP"){
            cout<<"RUN:     This command starts program execution beginning at the lowest-numbered line."<<endl;
            cout<<"LIST:    This command lists the steps in the program in numerical sequence."<<endl;
            cout<<"CLEAR:   This command deletes all program and variables."<<endl;
            cout<<"QUIT:    This command exits from the BASIC interpreter by calling exit(0)."<<endl;
            cout<<"HELP:    This command provides a simple help message describing your interpreter."<<endl;
        } else if (token == "LET" || token == "INPUT" || token == "PRINT"){

        } else{
            cout<<"SYNTAX ERROR"<<endl;
        }
    } else if (token_type == NUMBER){
        int line_number;
        try {
            line_number = stringToInteger(token);
        }catch (...){
            cout<<"SYNTAX ERROR"<<endl;
        }
        //Note that this operation actually deleted the line and did not simply replace it with a
        //blank line that would appear in program listings.
        if (!scanner.hasMoreTokens()){
            program.removeSourceLine(line_number);
            return;
        }
        try {
            Statement *statement_pointer = parseStatement(scanner,line);//指针定位
            program.addSourceLine(line_number,line);//添加信息
            program.setParsedStatement(line_number,statement_pointer);//指针链接
        }catch (...){
            cout<<"SYNTAX ERROR"<<endl;
        }
    } else{
        cout<<"SYNTAX ERROR"<<endl;
    }
}
