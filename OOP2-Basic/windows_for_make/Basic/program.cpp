/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include <string>
#include "program.h"
#include "statement.h"
#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/tokenscanner.h"
using namespace std;

Program::Program() {
   // Replace this stub with your own code
   //line_msg();
}

Program::~Program() {
   // Replace this stub with your own code
   this->clear();

}

void Program::clear() {
   // Replace this stub with your own code
   for(auto & i : program_map){
       delete i.second.exp ;
       i.second.exp = nullptr;
   }
   program_map.clear();
}

bool Program::Search(int lineNumber){
    for(auto & i : program_map){
        if (lineNumber == i.first)return true;
    }return false;
}

void Program::addSourceLine(int lineNumber, string line_) {
   // Replace this stub with your own code
    if (!this->Search(lineNumber)){//不存在该条代码
        program_map.insert(make_pair(lineNumber,line_msg(line_)));
    } else{
        program_map[lineNumber].message = line_;
        if (program_map[lineNumber].exp != nullptr)program_map[lineNumber].exp = nullptr;
    }
}

void Program::removeSourceLine(int lineNumber) {
   // Replace this stub with your own code
   //  * If no such line exists, this method simply returns without
   // * performing any action.
    if (!this->Search(lineNumber)){
        error("SYNTAX ERROR");
    } else{
        delete program_map[lineNumber].exp;
        program_map[lineNumber].exp = nullptr;
        program_map.erase(lineNumber);
    }
}

string Program::getSourceLine(int lineNumber) {
    // Replace this stub with your own code
    if (!this->Search(lineNumber)){
        return "";
    } else{
        return program_map[lineNumber].message;
    }
}

void Program::setParsedStatement(int lineNumber, Statement *stmt) {
   // Replace this stub with your own code
    if (!this->Search(lineNumber)){
        error("SYNTAX ERROR");
    } else{
        if (program_map[lineNumber].exp!= nullptr)delete program_map[lineNumber].exp;
        program_map[lineNumber].exp = stmt;
    }
}

Statement *Program::getParsedStatement(int lineNumber) {
   // Replace this stub with your own code
    if (!this->Search(lineNumber)){
        error("SYNTAX ERROR");
    } else{
        if (program_map[lineNumber].exp!= nullptr)return program_map[lineNumber].exp;
        else return nullptr;
    }
}

int Program::getFirstLineNumber() {
    if (program_map.empty())error("SYNTAX ERROR");
    else{
        auto i = program_map.begin();
        return i->first;
    }
    return 0;     // Replace this stub with your own code
}

int Program::getNextLineNumber(int lineNumber) {
    for (auto i : program_map){
        if (i.first > lineNumber)return i.first;
    }
    error("SYNTAX ERROR");
    return 0;     // Replace this stub with your own code
}

void Program::model_list() {
    for(auto  i : program_map){
        cout<<i.second.message<<endl;
    }
}

void Program::model_run(EvalState &state) {//模拟RUN
    if (program_map.empty()){
        return;
    }
    auto i = program_map.begin();
    while (i != program_map.end()){
        try {
            i->second.exp->execute(state);
            i++;
        }catch (ErrorException & err){
            TokenScanner scan;
            if (scan.getTokenType(err.getMessage()) == NUMBER){
                int line = stringToInteger(err.getMessage());
                if (!this->Search(line)){
                    error("FAIL TO GOTO");
                    return;
                } else{
                    i = this->program_map.find(line);
                    continue;
                }
            } else{
                error(err.getMessage());
            }
        }
    }
}
