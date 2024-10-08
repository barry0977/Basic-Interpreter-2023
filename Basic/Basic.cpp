/*
 * File: Basic.cpp
 * ---------------
 * This file is the starter project for the BASIC interpreter.
 */

#include <cctype>
#include <iostream>
#include <string>
#include "exp.hpp"
#include "parser.hpp"
#include "program.hpp"
#include "Utils/error.hpp"
#include "Utils/tokenScanner.hpp"
#include "Utils/strlib.hpp"


/* Function prototypes */

void processLine(std::string line, Program& program, EvalState& state);
void run(Program& program, EvalState& state);
void list(Program& program, EvalState& state);
void clear(Program& program, EvalState& state);
/* Main program */

int main() {
    EvalState state;
    Program program;
    //cout << "Stub implementation of BASIC" << endl;
    while (true) {
        try {
            //std::cout<<"进入该次循环\n";
            std::string input;
            //std::cout<<"point 1\n";
            getline(std::cin, input);
            //std::cout<<"point 2\n";
            if (input.empty())
                return 0;
            processLine(input, program, state);
            //std::cout<<"离开该次循环\n";
        }
        catch (ErrorException& ex) {
            std::cout << ex.getMessage() << std::endl;
        }
    }

    return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version of
 * implementation, the program reads a line, parses it as an expression,
 * and then prints the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */

void processLine(std::string line, Program& program, EvalState& state) {
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.setInput(line);
    Statement* statement = nullptr;
    std::string token = scanner.nextToken();
    if (scanner.getTokenType(token) == NUMBER)
    {
        int n = stringToInteger(token);
        if (scanner.hasMoreTokens()) //如果行数后还有别的，则添加该行,并且该行不会立即执行
        {
            int pos = scanner.getPosition();
            std::string addline = trim(line.substr(abs(pos)));
            program.addSourceLine(n, addline);
            if (program.hasLineNumber(n))//如果该行已经存在，应该先把该行清除，否则会内存泄漏
            {
                program.eraseParsedStatement(n);
            }
            token = scanner.nextToken();
            if (token == "INPUT")
            {
                token = scanner.nextToken();
                statement = new INPUT(token);
            }
            else if (token == "LET")
            {
                //Expression* exp= readE(scanner);
                Expression* exp = parseExp(scanner);
                statement = new LET(exp);
            }
            else if (token == "PRINT")
            {
                Expression* exp = parseExp(scanner);
                statement = new PRINT(exp);
            }
            else if (token == "IF")
            {
                int op = 0;
                int pos1 = abs(scanner.getPosition());
                while (token != "THEN")
                {
                    token = scanner.nextToken();
                }
                int pos2 = abs(scanner.getPosition());
                std::string expr = trim(line.substr(pos1, pos2 - pos1 - 5));
                //std::cout<<"得到expr:"<<expr<<"\n";
                for (int i = 0; i < expr.size(); i++)
                {
                    if (expr[i] == '<')
                    {
                        op = -1;
                        expr[i] = '=';
                        break;
                    }
                    if (expr[i] == '>')
                    {
                        op = 1;
                        expr[i] = '=';
                    }
                }
                TokenScanner scanner1;
                scanner1.ignoreWhitespace();
                scanner1.scanNumbers();
                scanner1.setInput(expr);
                Expression* exp = parseExp(scanner1);
                token = scanner.nextToken();
                int number = stringToInteger(token);
                statement = new IF(exp, number, op);
            }
            else if (token == "GOTO")
            {
                token = scanner.nextToken();
                int number = stringToInteger(token);
                statement = new GOTO(number);
            }
            else if (token == "REM")
            {
                //std::cout<<"进入REM！\n";
                statement = new REM();
            }
            else if (token == "END")
            {
                //std::cout << "in IF!" << std::endl;
                if (!scanner.hasMoreTokens())
                {
                    //std::cout << "new IN!" << std::endl;
                    statement = new END();
                    //std::cout << "new OK!" << std::endl;
                }
                else
                {
                    throw error;//格式错误
                }
            }
            program.setParsedStatement(n, statement);
        }
        else//如果只有行数，则删除这一行
        {
            program.removeSourceLine(n);
        }
    }
    else if (token == "INPUT")
    {
        //std::cout<<"进入\n";
        std::string name = scanner.nextToken();
        std::string value;
        while (true)
        {
            try
            {
                std::cout << " ? ";
                getline(std::cin, value);
                int intvalue = stringToInteger(value);
                state.setValue(name, intvalue);
            }
            catch (ErrorException& ex)
            {
                std::cout << "INVALID NUMBER\n";
                continue;
            }
            break;
        }
        //std::cout<<"退出\n";
    }
    else if (token == "LET")
    {
        Expression* exp = parseExp(scanner);
        LET tmp(exp);
        tmp.execute(state, program);
    }
    else if (token == "PRINT")
    {
        Expression* exp = parseExp(scanner);
        PRINT tmp(exp);
        tmp.execute(state, program);
    }
    else if (token == "RUN")
    {
        run(program, state);
    }
    else if (token == "LIST")
    {
        list(program, state);
    }
    else if (token == "CLEAR")
    {
        clear(program, state);
    }
    else if (token == "QUIT")
    {
        clear(program, state);
        exit(0);
    }
    else if (token == "HELP")
    {
    }
}

void run(Program& program, EvalState& state)
{
    int linenumber = program.getFirstLineNumber();
    while (linenumber != -1)
    {
        Statement* statement = program.getParsedStatement(linenumber);
        statement->execute(state, program);
        linenumber = program.getNextLineNumber(linenumber);
    }
}

void list(Program& program, EvalState& state)
{
    int linenumber = program.getFirstLineNumber();
    while (linenumber != -1)
    {
        std::cout << linenumber << " " << program.getSourceLine(linenumber) << "\n";
        linenumber = program.getNextLineNumber(linenumber);
    }
}

void clear(Program& program, EvalState& state)
{
    //std::cout<<"enter\n";
    program.clear();
    //std::cout<<"program clear over\n";
    state.Clear();
    //std::cout<<"state clear over\n";
}



