#include "Errors.h"

vector<string> Errors::m_ErrorMsgs;
bool Errors::m_WasErrorMessege;

string Errors::ERR_UsedLable = "Multiply defined labels.";
string Errors::ERR_UndefinedLable = "Undefined label.  Namely, a symbolic operand does not match a label.";
string Errors::ERR_SintaxError = "Syntax error in construction of the label or operands.";
string Errors::ERR_IllegalOpCode = "Illegal operation code.";
string Errors::ERR_InsufficientMem = "Insufficient memory for the translation.";
string Errors::ERR_MissEnd = "Missing end statement or the end statement is not the last one in the program."; //DONE
string Errors::ERR_ConstOverflow = "Constant too large for Quack3200 memory.";
string Errors::ERR_MissOp = "Missing or extra operands";
// 
/*
NAME

    DisplayErrors - Displays the collected error message.

SYNOPSIS

    void DisplayErrors();

DESCRIPTION
    This function will display and label each error by the sequence of occurence.
*/
void Errors::DisplayErrors() {
    for (size_t i = 0; i < m_ErrorMsgs.size(); i++) {
        cerr << "Error number " << i << " :" << m_ErrorMsgs.at(i) << endl;
    }
    m_ErrorMsgs.clear();
    m_WasErrorMessege = false;
}