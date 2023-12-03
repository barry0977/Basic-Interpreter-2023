/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include "program.hpp"



Program::Program() = default;

Program::~Program() = default;

void Program::clear() {
    list.clear();
    for(auto it:statelist)
    {
        Statement* tmp=it.second;
        tmp->~Statement();
        delete tmp;
    }
    statelist.clear();
}

void Program::addSourceLine(int lineNumber, const std::string &line) {
    list[lineNumber]=line;
}

void Program::removeSourceLine(int lineNumber) {
    list.erase(lineNumber);
}

std::string Program::getSourceLine(int lineNumber) {
    return list[lineNumber];
}

void Program::setParsedStatement(int lineNumber, Statement *stmt) {
    statelist[lineNumber]=stmt;
}


Statement *Program::getParsedStatement(int lineNumber) {
   return statelist[lineNumber];
}

int Program::getFirstLineNumber() {
    int number;
    if(list.empty())
    {
        number=-1;
    }
    else
    {
        number = list.begin()->first;
    }
    return number;
}

int Program::getNextLineNumber(int lineNumber) {
    if(gotoline!=-2)
    {
        int tmp=gotoline;
        gotoline=-2;
        return tmp;
    }
    else
    {
        int number;
        if(list.upper_bound(lineNumber)!=list.end())
        {
            number = list.upper_bound(lineNumber)->first;
        }
        else
        {
            number=-1;
        }
        return number;
    }
}

bool Program::hasLineNumber(int lineNumber)
{
    if(list.count(lineNumber)>0)
    {
        return true;
    }
    else
    {
        return false;
    }
}


//more func to add
//todo


