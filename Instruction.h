//
// Class to parse and provide information about instructions.  Note: you will be adding more functionality.
//
#pragma once
#include <unordered_map>
#include <algorithm>
#include "Errors.h"
constexpr int numAssemblyInstructions = 3;
constexpr int numMachineInstructions = 13;
// The elements of an instruction.
class Instruction {

public:

    Instruction() {};
    ~Instruction() { };

    // Codes to indicate the type of instruction we are processing.  Why is this inside the
    // class?

    enum InstructionType {
        ST_MachineLanguage, 		// A machine language instruction.
        ST_AssemblerInstr,  		// Assembler Language instruction.
        ST_Comment,          		// Comment or blank line
        ST_End                   	// end instruction.
    };
    // Parse the Instruction.
    InstructionType ParseInstruction(string a_buff);

    // Compute the location of the next instruction.
    inline int LocationNextInstruction(int a_loc) { return a_loc + 1; }

    // To determine if a label is blank.
    inline bool isLabel() {
        return !m_Label.empty();
    };

    // To access the label
    inline string& GetLabel() {

        return m_Label;
    };

    // To access the operation code
    inline string& GetOpCode() {
        return m_OpCode;
    };

    // To access the register
    inline string& GetRegister() {
        return m_Register;
    };

    // To access the operand
    inline string& GetOperand() {
        if (m_Operand.size() > 5) { Errors::RecordError(Errors::ERR_ConstOverflow); }
        return m_Operand;
    };

    inline bool& IsBlankOrComment() {
        return m_IsBlankOrComment;
    };

    // Determine if the instruction is Machine Language
    bool IsMachineLanguage();

    // Determine if the instruction is Assembler Language
    bool IsAssemblerInstr();

    // Determine if the instruction is an end statement 
    inline bool IsEnd() {
        if (m_OpCode == "END") { return true; }
        return false;
    }


private:

    void GetLabelOpcodeEtc(const string& a_buff);
    inline void ChangeOpcodeToCaps(string& m_opcode) {
        std::for_each(m_opcode.begin(), m_opcode.end(), [](char& c) {
            c = ::toupper(c);
            });
    }


    // The elemements of a instruction
    string m_Label;         // The label.
    string m_OpCode;        // The symbolic op code.
    string m_Register;      // The register value.
    string m_Operand;       // The operand.


    string m_instruction;   // The original instruction.

    // Derived values.
    int m_NumOpCode;        // The numerical value of the op code.
    int m_NumRegister;	    // the numberic value for the register.
    InstructionType m_type; // The type of instruction.

    bool m_IsNumericOperand;// == true if the operand is numeric.
    int m_OperandValue;     // The value of the operand if it is numeric.
    bool m_IsBlankOrComment;// == true if the line is blank or a comment.

    static const std::string AssemblyLabels[numAssemblyInstructions];// = { "ORG", "DC", "DS", "END" };
    static const std::string MachineLabels[numMachineInstructions];// = { "ADD", "SUB", "MULT", "DIV", "LOAD", "STORE", "READ", "WRITE", "B", "BM", "BZ", "BP", "HALT" };
};

