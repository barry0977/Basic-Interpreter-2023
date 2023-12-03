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
            std::cout<<" ? ";
            getline(std::cin,value);
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
        program.gotoline=-1;
        std::cout<<"LINE NUMBER ERROR\n";
    }
}

IF::IF(Expression* exp,int number,int op)
{
    expression=exp;
    linenumber=number;
    sign=op;
}

IF::~IF()
{
    delete expression;
}

void IF::execute(EvalState &state, Program &program)
{
    //std::cout<<"进入IF！\n";
    Expression *lhs,*rhs;
    lhs=((CompoundExp *) expression)->getLHS();
    rhs=((CompoundExp *) expression)->getRHS();
    int lvalue=lhs->eval(state);
    int rvalue=rhs->eval(state);
    bool istrue=false;
    if(sign==1)
    {
        if(lvalue>rvalue)
        {
            istrue=true;
        }
    }
    else if(sign==-1)
    {
        if(lvalue<rvalue)
        {
            istrue=true;
        }
    }
    else
    {
        if(lvalue==rvalue)
        {
            istrue=true;
        }
    }
    //std::cout<<"到达if！\n";
    if(istrue)
    {
        if(program.hasLineNumber(linenumber))
        {
            //std::cout<<"到达第一层！\n";
            program.gotoline=linenumber;
        }
        else
        {
            //std::cout<<"到达第二层！\n";
            program.gotoline=-1;
            std::cout<<"LINE NUMBER ERROR\n";
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
    program.gotoline=-1;
}


//todo
