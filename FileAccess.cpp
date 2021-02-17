//
//  Implementation of file access class.
//
#include "stdafx.h"
#include "FileAccess.h"
#include <iostream>
using namespace std;

/**/
/*
FileAccess::FileAccess() FileAccess::FileAccess()

NAME

        FileAccess::FileAccess -Constructor for the file access class.

SYNOPSIS

        FileAccess::FileAccess(int argc, char* argv[])
        int argc     --> An iniger value representing the number of arguments passed.
        char* argv[] --> The array of comand line arguments.

DESCRIPTION

        The constructor will take the comand line argumens and open a file with the same name as specified.
        The constructor does basic error checking and exits the program if anything goes wrong.
        No need to invoke Errors.h because if a file cant be open there is nothing we can do.

AUTHOR

        Nikolay Valev

*/
/**/
FileAccess::FileAccess(int argc, char* argv[])
{
    // Check that there is exactly one run time parameter.
    if (argc != 2) {
        cerr << "Usage: Assem <FileName>" << endl;
        exit(1);
    }

    // Open the file.
    m_sfile.open(argv[1], ios::in);

    // If the open failed, report the error and terminate.
    if (!m_sfile) {
        cerr << "Source file could not be opened, assembler terminated."
            << endl;
        exit(1);
    }
}

/**/
/*
        FileAccess::GetNextLine()  FileAccess::GetNextLine()

NAME

        FileAccess::GetNextLine - Get the next line from the input file.

SYNOPSIS

        bool FileAccess::GetNextLine(string& a_buff)
        string& a_buff     --> A string value that the line is passed to.

DESCRIPTION

        The function returns a boolian value.
        The function will return false if the file opening fails and willrturn true if the file can be opened.

AUTHOR

        Nikolay Valev

*/
/**/
bool FileAccess::GetNextLine(string& a_buff)
{
    // If there is no more data, return false.
    if (m_sfile.eof()) {

        return false;
    }
    getline(m_sfile, a_buff);

    // Return indicating success.
    return true;
}

