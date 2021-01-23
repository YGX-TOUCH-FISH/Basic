/*
 * File: evalstate.h
 * -----------------
 * This interface exports a class called EvalState, which keeps track
 * of additional information required by the evaluator, most notably
 * the values of variables.
 * 该接口导出一个名为EvalState的类，该类跟踪求值器所需的附加信息，尤其是变量的值。
 */

#ifndef _evalstate_h
#define _evalstate_h

#include <string>
#include "../StanfordCPPLib/map.h"

/*
 * Class: EvalState
 * ----------------
 * This class is passed by reference through the recursive levels
 * of the evaluator and contains information from the evaluation
 * environment that the evaluator may need to know.  In this
 * version, the only information maintained by the EvalState class
 * is a symbol table that maps variable names into their values.
 * Several of the exercises, however, require you to include
 * additional information in the EvalState class.
 * *这个类通过递归级别通过引用传递，
 * 包含评估环境中评估器可能需要知道的信息。
 * 在这个版本中，EvalState类维护的唯一信息是一个将变量名映射到其值的符号表。
 * 然而，有几个练习要求您在EvalState类中包含额外的信息。
 */

class EvalState {

public:

/*
 * Constructor: EvalState
 * Usage: EvalState state;
 * -----------------------
 * Creates a new EvalState object with no variable bindings.
 * 创建一个没有变量绑定的新EvalState对象。
 */

   EvalState();

/*
 * Destructor: ~EvalState
 * Usage: usually implicit
 * -----------------------
 * Frees all heap storage associated with this object.
 */

   ~EvalState();

/*
 * Method: setValue
 * Usage: state.setValue(var, value);
 * ----------------------------------
 * Sets the value associated with the specified var.
 * 实现变量的存储
 */

   void setValue(std::string var, int value);

/*
 * Method: getValue
 * Usage: int value = state.getValue(var);
 * ---------------------------------------
 * Returns the value associated with the specified variable.
 * 用变量名访问数据
 */

   int getValue(std::string var);

/*
 * Method: isDefined
 * Usage: if (state.isDefined(var)) . . .
 * --------------------------------------
 * Returns true if the specified variable is defined.
 * 判断是否定义了这个变量
 */

   bool isDefined(std::string var);

   void clear(){
       symbolTable.clear();
   }
private:

   Map<std::string,int> symbolTable;

};

#endif
