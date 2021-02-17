//
// Class to manage error reporting.
//
#ifndef _ERRORS_H
#define _ERRORS_H

#include <string>
#include <vector>
#include <iostream>
using namespace std;

class Errors {

public:

    // Initializes error reports.
    static void InitErrorReporting() {
        //call in the end of pass 1
        m_ErrorMsgs.clear();
    }

    // Records an error message.
    static void RecordError(const string a_emsg) {
        m_ErrorMsgs.push_back(a_emsg);
        m_WasErrorMessege = true;
    }
    // Displays the collected error message.
    static void DisplayErrors();
    static bool WasThereErrors() { return m_WasErrorMessege; }

    static string ERR_UsedLable;
    static string ERR_UndefinedLable;
    static string ERR_SintaxError;
    static string ERR_IllegalOpCode;
    static string ERR_InsufficientMem;
    static string ERR_MissEnd;
    static string ERR_ConstOverflow;
    static string ERR_MissOp;
private:
    static vector<string> m_ErrorMsgs;
    static bool m_WasErrorMessege;
};
#endif