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
//OVER:REM QUIT HELP LIST
//TODO:PRINT
void processLine(string line, Program & program, EvalState & state);

/* Main program */

int main() {
   EvalState state;//储存所有变量
   Program program;
   //cout << "Stub implementation of BASIC" << endl;
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
    } else return;//处理下一行
    if (token_type == WORD){
        if (token == "RUN"){
            if (scanner.hasMoreTokens()){
                cout<<"SYNTAX ERROR"<<endl;return;
            }
            try {
                program.model_run(state);
            }catch (ErrorException &err){
                if (err.getMessage() == "FAIL TO GOTO"){
                    cout<<"LINE NUMBER ERROR"<<endl;return;
                }
                if (err.getMessage() == "END")return;
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
            cout<<"\n"
                  "                   _ooOoo_\n"
                  "                  o8888888o\n"
                  "                  88\" . \"88\n"
                  "                  (| -_- |)\n"
                  "                  O\\  =  /O\n"
                  "               ____/`---'\\____\n"
                  "             .'  \\\\|     |//  `.\n"
                  "            /  \\\\|||  :  |||//  \\\n"
                  "           /  _||||| -:- |||||-  \\\n"
                  "           |   | \\\\\\  -  /// |   |\n"
                  "           | \\_|  ''\\---/''  |   |\n"
                  "           \\  .-\\__  `-`  ___/-. /\n"
                  "         ___`. .'  /--.--\\  `. . __\n"
                  "      .\"\" '<  `.___\\_<|>_/___.'  >'\"\".\n"
                  "     | | :  `- \\`.;`\\ _ /`;.`/ - ` : | |\n"
                  "     \\  \\ `-.   \\_ __\\ /__ _/   .-` /  /\n"
                  "======`-.____`-.___\\_____/___.-`____.-'======\n"
                  "                   `=---='\n"
                  "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n";
        } else if (token == "LET" || token == "INPUT" || token == "PRINT"){
            scanner.setInput(line);
            Statement *line_statement ;
            try {
                line_statement = parseStatement(scanner,line);//BUG 1
            } catch (...) {
                cout<<"SYNTAX ERROR"<<endl;
                return;
            }
            try {
                line_statement->execute(state);
                delete line_statement;
            } catch (ErrorException &err) {
                delete line_statement;
                if (err.getMessage() == "DIVIDE BY ZERO"){
                    cout<<"DIVIDE BY ZERO"<<endl;
                    return;
                }
                if (err.getMessage() == "INVALID NUMBER"){
                    cout<<"INVALID NUMBER"<<endl;
                    return;
                }
                if (err.getMessage() == "VARIABLE NOT DEFINED"){
                    cout<<"VARIABLE NOT DEFINED"<<endl;
                } else{
                    cout<<"VARIABLE NOT DEFINED"<<endl;
                    return;
                }
            }
        } else{
            cout<<"SYNTAX ERROR"<<endl;
        }
    } else if (token_type == NUMBER){//LINE NUMBER
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
        return;
    }
}
