/*
 * File: program.h
 * ---------------
 * This interface exports a Program class for storing a BASIC
 * program.
 */

#ifndef _program_h
#define _program_h

#include <string>
#include "statement.h"
using namespace std;

/*
 * This class stores the lines in a BASIC program.  Each line
 * in the program is stored in order according to its line number.
 * Moreover, each line in the program is associated with two
 * components:
 *
 * 1. The source line, which is the complete line (including the
 *    line number) that was entered by the user.
 *
 * 2. The parsed representation of that statement, which is a
 *    pointer to a Statement.
 *    *这个类存储基本程序中的行。程序中的每一行都按照其行号的顺序存储。
*此外，程序中的每一行都与两个组件相关联:
*
* 1。源行，即用户输入的完整行(包括行号)。
* 2。该语句的解析表示，它是一个指向Statement的指针。
 */

class Program {

public:

/*
 * Constructor: Program
 * Usage: Program program;
 * -----------------------
 * Constructs an empty BASIC program.
 */

   Program();

/*
 * Destructor: ~Program
 * Usage: usually implicit
 * -----------------------
 * Frees any heap storage associated with the program.
 */

   ~Program();

/*
 * Method: clear
 * Usage: program.clear();
 * -----------------------
 * Removes all lines from the program.
 */

   void clear();

/*
 * Method: addSourceLine
 * Usage: program.addSourceLine(lineNumber, line);
 * -----------------------------------------------
 * Adds a source line to the program with the specified line number.
 * If that line already exists, the text of the line replaces
 * the text of any existing line and the parsed representation//parse：解析
 * (if any) is deleted.  If the line is new, it is added to the
 * program in the correct sequence.
 */

   void addSourceLine(int lineNumber, std::string line);

/*
 * Method: removeSourceLine
 * Usage: program.removeSourceLine(lineNumber);
 * --------------------------------------------
 * Removes the line with the specified number from the program,
 * freeing the memory associated with any parsed representation.
 * If no such line exists, this method simply returns without
 * performing any action.
 */

   void removeSourceLine(int lineNumber);

/*
 * Method: getSourceLine
 * Usage: string line = program.getSourceLine(lineNumber);
 * -------------------------------------------------------
 * Returns the program line with the specified line number.
 * If no such line exists, this method returns the empty string.
 */
//
//    / *
//    *方法:getSourceLine
//        *使用:string line = program.getSourceLine(lineNumber);
//    * -------------------------------------------------------
//    *返回具有指定行号的程序行。
//    *如果不存在这样的行，该方法返回空字符串。
//    * /
   std::string getSourceLine(int lineNumber);

/*
 * Method: setParsedStatement
 * Usage: program.setParsedStatement(lineNumber, stmt);
 * ----------------------------------------------------
 * Adds the parsed representation of the statement to the statement
 * at the specified line number.  If no such line exists, this
 * method raises an error.  If a previous parsed representation
 * exists, the memory for that statement is reclaimed.
 */
//    *方法:setParsedStatement
//    * ----------------------------------------------------
//    *将已解析的语句表示形式添加到指定行号的语句中。如果不存在这样的行，则
//    *方法引发错误。如果是先前解析的表示
//    *存在时，该语句的内存将被回收。
//    * /
   void setParsedStatement(int lineNumber, Statement *stmt);

/*
 * Method: getParsedStatement
 * Usage: Statement *stmt = program.getParsedStatement(lineNumber);
 * ----------------------------------------------------------------
 * Retrieves the parsed representation of the statement at the
 * specified line number.  If no value has been set, this method
 * returns NULL.
 */
//检索语句在指定行号处的解析表达式。如果没有设置值，该方法将返回NULL。
   Statement *getParsedStatement(int lineNumber);

/*
 * Method: getFirstLineNumber
 * Usage: int lineNumber = program.getFirstLineNumber();
 * -----------------------------------------------------
 * Returns the line number of the first line in the program.
 * If the program has no lines, this method returns -1.
 */

   int getFirstLineNumber();

/*
 * Method: getNextLineNumber
 * Usage: int nextLine = program.getNextLineNumber(lineNumber);
 * ------------------------------------------------------------
 * Returns the line number of the first line in the program whose
 * number is larger than the specified one, which must already exist
 * in the program.  If no more lines remain, this method returns -1.
 */

   int getNextLineNumber(int lineNumber);

   bool Search(int lineNumber);

   void model_list();

   void model_run(EvalState &state);
private:

// Fill this in with whatever types and instance variables you need
    struct line_msg{
        string message;
        Statement *exp = nullptr;
        line_msg(){
            message = "";
        }
        line_msg(string &information,Statement *stmt){
            message = information;
            exp = stmt;
        }
        line_msg(string &information){
            message = information;
        }

    };
    map<int , line_msg> program_map;//行号-行信息
};

#endif
