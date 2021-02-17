//
//      Implementation of the Assembler class.
//
#include "stdafx.h"
#include "Assembler.h"
#include "Errors.h"
#include "SymTab.h"

// Constructor for the assembler.  Note: we are passing argc and argv to the file access constructor.
// See main program.  
Assembler::Assembler(int argc, char* argv[])
    : m_facc(argc, argv)
{
    loc = 0;
    // Nothing else to do here at this point.
}

// Destructor currently does nothing.  You might need to add something as you develope this project.
Assembler::~Assembler()
{
}

/**/
/*
        Assembler::ReturnMemory()  Assembler::ReturnMemory()

NAME

        Assembler::ReturnMemory - Returns The integer value representing the translation.

SYNOPSIS

        int Assembler::ReturnMemory(int opc)
        int opc     --> The intiger value of the operation code.

DESCRIPTION

        The function returns an int value representing the translation.
        The value is fromated once the function returns, to save the int value in memory.

AUTHOR

        Nikolay Valev

*/
/**/
int Assembler::ReturnMemory(int a_opc) {
    int a_reg = stoi(m_inst.GetRegister());
    int a_adress = m_symtab.ReturnAdress(m_inst.GetOperand());
    int a_memory = a_opc * 1000000;
    a_memory += a_reg * 100000;
    a_memory += a_adress;
    if (!m_emul.insertMemory(loc, a_memory)) {
        Errors::DisplayErrors();
        return 0;
    }
    return a_memory;
}

/**/
/*
        Assembler::TranslateMachineLanguageInstruction()  Assembler::TranslateMachineLanguageInstruction()

NAME

        Assembler::TranslateMachineLanguageInstruction - translates machine language instructions to an int value.

SYNOPSIS

        int Assembler::TranslateMachineLanguageInstruction();

DESCRIPTION

        The function returns an int value representing the translation.
        The value is fromated once the function returns, to save the int value in memory.
        The function calls Assembler::ReturnMemory in a series of if statements inserting a hardcoded value.
        Some operation codes are handles internally,
        in order to keep Assembler::ReturnMemory simple and not having overloaded or default values.

AUTHOR

        Nikolay Valev

*/
/**/
int Assembler::TranslateMachineLanguageInstruction() {
    string a_opCode = m_inst.GetOpCode();;
    if (a_opCode == "ADD") {
        return ReturnMemory(1);
    }
    if (a_opCode == "SUB") {
        return ReturnMemory(2);
    }
    if (a_opCode == "MULT") {
        return ReturnMemory(3);
    }
    if (a_opCode == "DIV") {
        return ReturnMemory(4);
    }
    if (a_opCode == "LOAD") {
        return ReturnMemory(5);
    }
    if (a_opCode == "STORE") {
        return ReturnMemory(6);
    }
    if (a_opCode == "READ") {
        return ReturnMemory(7);
    }
    if (a_opCode == "WRITE") {
        return ReturnMemory(8);
    }
    if (a_opCode == "B") {
        return ReturnMemory(9);
    }
    if (a_opCode == "BM") {
        return ReturnMemory(10);
    }
    if (a_opCode == "BZ") {
        return ReturnMemory(11);
    }
    if (a_opCode == "BP") {
        return ReturnMemory(12);
    }
    if (a_opCode == "HALT") {
        int a_reg = stoi(m_inst.GetRegister());
        int a_memory = 13 * 1000000;
        a_memory += a_reg * 100000;
        if (!m_emul.insertMemory(loc, a_memory)) {
            Errors::DisplayErrors();
            return 0;
        }
        return a_memory;
    }
    Errors::RecordError(Errors::ERR_IllegalOpCode);
    return 0;
}

/**/
/*
        Assembler::TranslateAssemblerInstrInstruction()  Assembler::TranslateAssemblerInstrInstruction()

NAME

        Assembler::TranslateAssemblerInstrInstruction - translates Assembler instructions to an int value.

SYNOPSIS

        int Assembler::TranslateAssemblerInstrInstruction();

DESCRIPTION

        The function returns an int value representing the translation,
        only for (opCode == "DC"), in all other cases the function returns 0,
        because the operations consern location handling and is done in memory.
        The value is fromated once the function returns, to save the int value in memory.


AUTHOR

        Nikolay Valev

*/
/**/
int Assembler::TranslateAssemblerInstrInstruction() {
    string a_opCode = m_inst.GetOpCode();
    if (a_opCode == "ORG") {
        loc = stoi(m_inst.GetOperand());
        return 0;
    }
    if (a_opCode == "DC") {
        if (!m_emul.insertMemory(loc, stoi(m_inst.GetOperand()))) {
            Errors::DisplayErrors();
            loc++;
            return 0;
        }
        return stoi(m_inst.GetOperand());
    }
    if (a_opCode == "DS") {
        loc += stoi(m_inst.GetOperand());
        return 0;
    }
    Errors::RecordError(Errors::ERR_IllegalOpCode);
}

/**/
/*
        Assembler::PassI()  Assembler::PassI()

NAME

        void Assembler::PassI - Handles pass one of the two pass design assembler.

SYNOPSIS

        void Assembler::PassI();

DESCRIPTION

        Pass I establishes the location of the labels, putting them in the symbol table.
        Lines having only coments and Machine and Assembler instructions are ignored.
        Does error chacking.

AUTHOR

        Nikolay Valev

*/
/**/
void Assembler::PassI() {
    int loc = 0;        // Tracks the location of the instructions to be generated.

   // Successively process each line of source code.
    while (true) {

        // Read the next line from the source file.
        string line;
        if (!m_facc.GetNextLine(line)) {

            // If there are no more lines, we are missing an end statement.
            // We will let this error be reported by Pass II.
            return;
        }
        // Parse the line and get the instruction type.
        Instruction::InstructionType st = m_inst.ParseInstruction(line);
        //Check for origin and set the location.
        if (m_inst.GetOpCode() == "ORG") {
            loc = stoi(m_inst.GetOperand());
            continue;
        }

        // If this is an end statement, there is nothing left to do in pass I.
        // Pass II will determine if the end is the last statement.
        if (st == Instruction::ST_End) return;

        //if (st == Instruction::ST_Comment) continue;

        // Labels can only be on machine language and assembler language
        // instructions.  So, skip other instruction types.
        if (st != Instruction::ST_MachineLanguage && st != Instruction::ST_AssemblerInstr)
        {
            continue;
        }
        // If the instruction has a label, record it and its location in the
        // symbol table.
        if (m_inst.isLabel()) {

            m_symtab.AddSymbol(m_inst.GetLabel(), loc);
            if (m_inst.GetOpCode() == "DS") {
                loc += stoi(m_inst.GetOperand());
                continue;
            }
        }
        // Compute the location of the next instruction.
        loc = m_inst.LocationNextInstruction(loc);
    }
}

/**/
/*
        Assembler::PassII()  Assembler::PassII()

NAME

        void Assembler::PassII - Handles pass two of the two pass design assembler.

SYNOPSIS

        void Assembler::PassII();

DESCRIPTION

        Pass II encapsulates the functionality of translating and handling errors.
        The function takes one line at a time, appending an error to a vector,
        printing the errors at each exit point.If more than one error occurs,
        they are all printed after the offending statement. Translation does not
        stop if an error is found. Serious errors are not handled well,
        and if an error causes a crash, there is not yet exception handling.

AUTHOR

        Nikolay Valev

*/
/**/
void Assembler::PassII() {
    //Rewind the source file to line 0. 
    m_facc.rewind();
    while (true)
    {
        // Read the next line from the source file.
        string line;
        if (!m_facc.GetNextLine(line)) {
            // If there are no more lines, we are missing an end statement. 
            Errors::RecordError(Errors::ERR_MissEnd);
            if (Errors::WasThereErrors()) { Errors::DisplayErrors(); }
            return;
        }
        //generate the instruction type and parse the line.
        Instruction::InstructionType st = m_inst.ParseInstruction(line);
        if (m_inst.GetOpCode() == "ORG") {
            cout << loc << setw(30) << line << endl;
            loc = stoi(m_inst.GetOperand());
            if (Errors::WasThereErrors()) { Errors::DisplayErrors(); }
            continue;
        }
        if (m_inst.IsBlankOrComment()) {
            //TODO(nvalev) Find a way to fix this.
            cout << setw(30) << line << endl;
            if (Errors::WasThereErrors()) { Errors::DisplayErrors(); }
            continue;
        }
        if (st == Instruction::ST_End) {
            cout << setw(30) << line << endl;
            if (Errors::WasThereErrors()) { Errors::DisplayErrors(); }
            return;
        }
        if (st == Instruction::ST_MachineLanguage) {
            int translation = TranslateMachineLanguageInstruction();
            cout << loc << "    " << std::setfill('0') << setw(8) << translation << setfill('\0') << line << endl;
            if (Errors::WasThereErrors()) { Errors::DisplayErrors(); }
        }
        if (st == Instruction::ST_AssemblerInstr) {
            int translation = TranslateAssemblerInstrInstruction();
            cout << loc << "    " << std::setfill('0') << setw(8) << translation << "    " << setw(10) << line << endl << setfill('\0');
            if (Errors::WasThereErrors()) { Errors::DisplayErrors(); }

        }
        loc = m_inst.LocationNextInstruction(loc);
    }
}
