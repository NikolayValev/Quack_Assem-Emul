//
//		File access to source file.
//
#ifndef _FILEACCESS_H  // This is the way that multiple inclusions are defended against often used in UNIX
#define _FILEACCESS_H // We use pramas in Visual Studio.  See other include files

#include <fstream>
#include <stdlib.h>
#include <string>

class FileAccess {

public:

    // Opens the file.
    FileAccess(int argc, char* argv[]);

    // Closes the file.
    ~FileAccess() {
        m_sfile.close();
    }

    // Get the next line from the source file.
    bool GetNextLine(string& a_buff);

    // Put the file pointer back to the beginning of the file.
    // Clean all file flags and go back to the beginning of the file.
    inline void rewind() {
        m_sfile.clear();
        m_sfile.seekg(0, ios::beg);
    }

private:

    ifstream m_sfile;		// Source file object.
};
#endif
