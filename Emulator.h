//
//		Emulator class - supports the emulation of Quack3200 programs
//
#ifndef _EMULATOR_H 
#define _EMULATOR_H

class emulator {

public:

    const static int MEMSZ = 100000;	// The size of the memory of the Quack3200.
    emulator() {

        memset(m_memory, 0, MEMSZ * sizeof(int));
    }
    // Records instructions and data into Quack3200 memory.
    bool insertMemory(int a_location, int a_contents)
    {
        if (a_location >= 0 && a_location < MEMSZ)
        {
            m_memory[a_location] = a_contents;
            return true;
        }
        else
        {
            Errors::RecordError(Errors::ERR_InsufficientMem);
            cerr << "Grumble gumble" << endl;
            return false;
        }
    }

    // Runs the Quack3200 program recorded in memory.
    bool runProgram()
    {
        //TODO(nvalev) Use the location given by org.
        int loc = 100;
        while (true)
        {
            int contents = m_memory[loc];  // 01455555
            int opcode = contents / 1000000;
            int reg = contents / 100000 % 10; // 14
            int address = contents % 100000;
            /*
            Symbolic Operation Codes:
                01 ADD        02 SUB     03 MULT     04 DIV     05 LOAD
                06 STORE    07 READ  08 WRITE     09 B        10 BM
                11 BZ           12 BP        13 HALT
            */
            switch (opcode)
            {
            case 1:// 01 ADD
                m_reg[reg] += m_memory[address];
                loc += 1;
                continue;
            case 2://02 SUB
                m_reg[reg] -= m_memory[address];
                loc += 1;
                continue;
            case 3://03 MULT
                cout << "m_reg[reg]" << m_reg[reg] << "m_memory[address]" << m_memory[address] << endl;
                if (m_reg[reg] == 0) { m_reg[reg] = 1; }
                m_reg[reg] *= m_memory[address];
                cout << "m_reg[reg]" << m_reg[reg] << "m_memory[address]" << m_memory[address] << endl;
                loc += 1;
                continue;
            case 4://04 DIV
                m_reg[reg] /= m_memory[address];
                loc += 1;
                continue;
            case 5://05 LOAD
                m_reg[reg] = m_memory[address];
                loc += 1;
                continue;
            case 6://06 STORE
                //TODO(check if the storing is correct)
                m_memory[address] = m_reg[reg];
                loc += 1;
                continue;
            case 7://07 READ
                cout << "?" << " ";
                int x;
                cin >> x;
                //if (isdigit(x) == false) {continue; }
                m_memory[address] = x;
                loc += 1;
                continue;
            case 8://08 WRITE
                cout << m_memory[address] << endl;
                loc += 1;
                continue;
            case 9://09 B (BRANCH)
                loc = address;
                continue;
            case 10://10 BM (BRANCH MINUS)
                if (m_reg[reg] < 0) {
                    loc = address;
                    continue;
                }
                loc++;
                continue;
            case 11://11 BZ (BRANCH ZERO)
                if (m_reg[reg] == 0) {
                    loc = address;
                    continue;
                }
                loc++;
                continue;
            case 12://12 BP (BRANCH POSITIVE)
                if (m_reg[reg] > 0) {
                    loc = address;
                    continue;
                }
                loc++;
                continue;
            case 13://13 HALT
                exit(1);
            default:// if an illegal opcode is tried
                //TODO(nvalev) Implement the Errors.cpp error
                Errors::RecordError(Errors::ERR_IllegalOpCode);
                Errors::DisplayErrors();
                cerr << "Illegal opcode" << endl;
                exit(1);
            }
        }
    }

private:

    int m_memory[MEMSZ];       // The memory of the Quack3200.
    int m_reg[10];					// The accumulator for the Quack3200
};
#endif

