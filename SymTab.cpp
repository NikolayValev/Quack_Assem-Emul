//
//		Implementation of the symbol table class.  This is the format I want for commenting functions.
//
#include "stdafx.h"
#include "SymTab.h"

/**/
/*
NAME

    AddSymbol - adds a new symbol to the symbol table.

SYNOPSIS

    void AddSymbol( char *a_symbol, int a_loc );

DESCRIPTION

    This function will place the symbol "a_symbol" and its location "a_loc"
    in the symbol table.

AUTHOR

        Nikolay Valev

*/
/**/
void SymbolTable::AddSymbol(string& a_symbol, int a_loc)
{
    // If the symbol is already in the symbol table, record it as multiply defined.
    map<string, int>::iterator st = m_symbolTable.find(a_symbol);
    if (st != m_symbolTable.end()) {

        st->second = multiplyDefinedSymbol;
        return;
    }
    // Record a the  location in the symbol table.
    m_symbolTable[a_symbol] = a_loc;
}

/**/
/*
NAME

    DisplaySymbolTable - displays the symbols stored in the symbol table.

SYNOPSIS

    void DisplaySymbolTable();

DESCRIPTION

    This function will iterate though the stored symbols and display them in a formated way.

AUTHOR

        Nikolay Valev

*/
/**/
void SymbolTable::DisplaySymbolTable() {
    if (m_symbolTable.begin() == m_symbolTable.end()) {
        return;
        //TODO(nvalev) Add some comprihensive error
    }
    cout << "Symbol #     Symbol     Location" << endl;
    //TODO(nvalev) Fix this ugly for loop
    int i = 0;
    for (auto st : m_symbolTable) {
        //TODO(nvalev) Format better
        cout << i++ << "                 " << st.first << "              " << st.second << endl;
    }
}

/**/
/*
NAME

    LookupSymbol - Check if a symbol is in the symbol table.

SYNOPSIS

    bool LookupSymbol(string& a_symbol, int& a_loc);
        string& a_symbol --> The symbol to be looked up in the symbol table.
        int& a_loc //TODO(nvalev) IDK yet

DESCRIPTION

    This function will determine wheter a symbol is already in the table or not.

AUTHOR

    Nikolay Valev

*/
/**/
bool SymbolTable::LookupSymbol(string& a_symbol, int& a_loc) {
    auto st = m_symbolTable.find(a_symbol);
    if (st->second == multiplyDefinedSymbol) {
        Errors::RecordError(Errors::ERR_UsedLable);
        return false;
    }
    if (st == m_symbolTable.end()) {
        Errors::RecordError(Errors::ERR_UndefinedLable);
        return false;
    }
    return true;
}

int SymbolTable::ReturnAdress(string a_symbol) {
    auto st = m_symbolTable.find(a_symbol);
    if (st == m_symbolTable.end()) {
        Errors::RecordError(Errors::ERR_UndefinedLable);
        return -1;
        //TODO(nvlaev) Throw an exception maybe, time will show.
    }
    return st->second;
}