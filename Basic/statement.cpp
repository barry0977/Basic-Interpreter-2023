/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include "statement.hpp"


/* Implementation of the Statement class */

int stringToInt(std::string str);

Statement::Statement() = default;

Statement::~Statement() = default;

INPUT::INPUT(std::string name)
{
    variable=name;
}

INPUT::~INPUT()=default;

void INPUT::execute(EvalState &state, Program &program)
{
    std::string value;
    while(true)
    {
        try
        {
            std::cout<<"?\n";
            std::cin>>value;
            int intvalue= stringToInteger(value);
            state.setValue(variable,intvalue);
        }
        catch(ErrorException& ex)
        {
            std::cout<<"INVALID NUMBER\n";
            continue;
        }
        break;
    }
}

REM::REM()=default;

REM::~REM()=default;

void REM::execute(EvalState &state, Program &program)
{
    return;
}

LET::LET(Expression* exp)
{
    expression=exp;
}

LET::~LET()
{
    delete expression;
}

void LET::execute(EvalState &state, Program &program)
{
    int value=expression->eval(state);
}

GOTO::GOTO(int num)
{
    linenumber=num;
}

GOTO::~GOTO()=default;

void GOTO::execute(EvalState &state, Program &program)
{
    if(program.hasLineNumber(linenumber))
    {
        program.gotoline=linenumber;
    }
    else
    {
        throw error;
    }
}

IF::IF(Expression* exp,int number)
{
    expression=exp;
    linenumber=number;
}

IF::~IF()
{
    delete expression;
}

void IF::execute(EvalState &state, Program &program)
{
    int value=expression->eval(state);
    if(value==1)
    {
        if(program.hasLineNumber(linenumber))
        {
            program.gotoline=linenumber;
        }
        else
        {
            throw error;
        }
    }
}

PRINT::PRINT(Expression *exp)
{
    expression=exp;
}

PRINT::~PRINT()
{
    delete expression;
}

void PRINT::execute(EvalState &state, Program &program)
{
    int value=expression->eval(state);
    std::cout<<value<<"\n";
}

END::END()=default;

END::~END()=default;

void END::execute(EvalState &state, Program &program)
{
    exit(0);
}


//todo
