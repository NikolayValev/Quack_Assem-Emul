
#include "stdafx.h"
#include <sstream>

using namespace std;
#include "Instruction.h"
#include "Errors.h"
const std::string Instruction::AssemblyLabels[numAssemblyInstructions] = { "ORG", "DC", "DS" };
const std::string Instruction::MachineLabels[numMachineInstructions] = { "ADD", "SUB", "MULT", "DIV", "LOAD", "STORE", "READ", "WRITE", "B", "BM", "BZ", "BP", "HALT" };
std::unordered_map<string, int> AssemblyLabels, MachineLabels;

/**/
/*
Instruction::ParseInstruction() Instruction::ParseInstruction()

NAME

        Instruction::ParseInstruction - Parses a line of code.

SYNOPSIS


        Instruction::InstructionType Instruction::ParseInstruction(string a_buff);

            a_buff              --> string buffer containing the line of code.

DESCRIPTION

        The function takes the line of code in the form of a string,
        breaking it into the elements of an instruction and saving.

RETURNS

        Returns Instruction::InstructionType,the type of instruction being passed in the line.
        ST_MachineLanguage	--> A machine language instruction.
        ST_AssemblerInstr  	--> Assembler Language instruction.
        ST_Comment          --> Comment or blank line
        ST_End              --> End instruction.

AUTHOR

        Nikolay Valev

*/
/**/
Instruction::InstructionType Instruction::ParseInstruction(string a_buff)
{
    // Make copy of original instruction
    m_instruction = a_buff;

    // Get rid of everything after the semicolon if it exists.  "aaa;"
    size_t isemi = a_buff.find(';');
    if (isemi != string::npos)
    {
        a_buff = a_buff.substr(0, isemi);  // This was an error 
    }
    // Parse instruction into basic elements.
    GetLabelOpcodeEtc(a_buff);
    if (IsBlankOrComment()) { return ST_Comment; }
    if (IsEnd()) { return ST_End; }
    if (IsMachineLanguage()) { return ST_MachineLanguage; }
    if (IsAssemblerInstr()) { return ST_AssemblerInstr; }
    Errors::RecordError(Errors::ERR_IllegalOpCode);
}

/**/
/*
Instruction::IsMachineLanguage() Instruction::IsMachineLanguage()

NAME

        Instruction::IsMachineLanguage -Checks if the given instruction is machine language.

SYNOPSIS

        bool Instruction::IsMachineLanguage();

DESCRIPTION

        The function checks the operation code of the instruction against an array containg all legal operation codes.

RETURNS

        Returns a boolean value, to be placed in an if statement.
        TRUE	--> A machine language instruction.
        FALSE  	--> Not a machine language instruction.

AUTHOR

        Nikolay Valev

*/
/**/
bool Instruction::IsMachineLanguage() {
    for (int i = 0; i < numMachineInstructions; i++) {
        if (m_OpCode == MachineLabels[i]) { return true; }
    }
    return false;
}

/**/
/*
Instruction::IsAssemblerInstr() Instruction::IsAssemblerInstr()

NAME

        Instruction::IsAssemblerInstr -Checks if the given instruction is assembly instruction.

SYNOPSIS

        bool Instruction::IsAssemblerInstr();

DESCRIPTION

        The function checks the operation code of the instruction against an array containg all legal operation codes.

RETURNS

        Returns a boolean value, to be placed in an if statement.
        TRUE	--> An assembly instruction.
        FALSE  	--> Not an assembly instruction.

AUTHOR

        Nikolay Valev

*/
/**/
bool Instruction::IsAssemblerInstr() {
    for (int i = 0; i < numAssemblyInstructions; i++) {
        if (m_OpCode == AssemblyLabels[i]) { return true; }
    }
    return false;
}

/**/
/*
Instruction::GetLabelOpcodeEtc() Instruction::GetLabelOpcodeEtc()

NAME

        Instruction::GetLabelOpcodeEtc -Breaks up the line into the elements present.

SYNOPSIS

        void Instruction::GetLabelOpcodeEtc(const string& a_buff);
        const string& a_buff --> The buffer containing the line of code.

DESCRIPTION

        The function sets the values to multiple class member varaiables.
        string m_Label;         --> The label.
        string m_OpCode;        --> The symbolic op code.
        string m_Register;      --> The register value.
        string m_Operand;       --> The operand.

AUTHOR

        Nikolay Valev

*/
/**/
void Instruction::GetLabelOpcodeEtc(const string& a_buff)
{
    m_Label = m_OpCode = m_Register = m_Operand = "";
    m_IsBlankOrComment = false;
    istringstream ins(a_buff);

    string a1, a2, a3, a4;
    ins >> a1 >> a2 >> a3 >> a4;

    // If there is no data, this line must have been empty or just had a comment.
    if (a1.empty())
    {
        m_IsBlankOrComment = true;
        return;
    }
    if (a_buff[0] != ' ' && a_buff[0] != '\t')
    {
        m_Label = a1;
        m_OpCode = a2;
        ChangeOpcodeToCaps(m_OpCode);
        m_Operand = a3;
        if (!isalpha(m_Label.at(0)) || m_Label.size() > 10) {
            Errors::RecordError(Errors::ERR_SintaxError);
        }
        //TODO(nvalev) Write a function to check the Label and operand
        if (a4 != "") {
            Errors::RecordError(Errors::ERR_MissOp);
        }
    }
    else
    {
        m_OpCode = a1;
        ChangeOpcodeToCaps(m_OpCode);
        m_Operand = a2;
        if (a3 != "") {
            Errors::RecordError(Errors::ERR_MissOp);
        }
    }
    // Check operand for comma and parse accordingly 
    size_t icomma = m_Operand.find(',');
    if (icomma != string::npos)
    {
        m_Register = m_Operand.substr(0, icomma);
        m_Operand = m_Operand.substr(icomma + 1);
    }
    else
    {
        m_Register = "9";
    }
}