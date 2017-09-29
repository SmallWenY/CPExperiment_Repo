/**
 * @author TripleZ
 * @date 2017-09-27
 * @brief A simple grammar analysis program written by C++.
 */

#include "GA.h"
using namespace std;

Unit unit;
int errorStack[100] = {-1};
int *errorType = errorStack;
bool error = false;
fstream gaSource;
fstream gaOutput;


/**
 * Read file line by line.
 * @return Global variables: unit.value & unit.key.
 */
string line;
string::iterator itLine;

void ReadLine() {
// Remember ERROR procedure.
    getline(gaSource, line);
//    cout << line << endl;
    itLine = line.begin();
    while (*itLine == '#' || line.empty()) {// Jump annotations & empty lines
        getline(gaSource, line);
//        cout << line << endl;
        itLine = line.begin();
    }

    istringstream iss(line);

    if (*itLine == '^'){ // Lexical error
        char ch;
        iss >> ch; // '^'
        iss >> unit.value;
        iss >> unit.key;
        iss >> unit.line;
        iss >> unit.column;
//        unit.print();
        getline(gaSource, line);
        cout << line << endl;
    } else { // No lexical error
        iss >> unit.value;
        iss >> unit.key;
        iss >> unit.line;
        iss >> unit.column;
//        unit.print();
    }
}

/**
 * Throw an error.
 * @param type Error type.
 */
void ThrowError(int type){
    error = true;
    *errorType = type;
    errorType++;
    switch(type){
        case 0:
            cout << "[Grammar ERROR]" << " [" << unit.line << "," << unit.column << "] " << "Missing word \"program\"" << endl;
            break;
        case 1:
            cout << "[Grammar ERROR]" << " [" << unit.line << "," << unit.column << "] " << "Missing identifier after \"program\"" << endl;
            break;
        case 2:
            cout << "[Grammar ERROR] "<< " [" << unit.line << "," << unit.column << "] " <<"Missing end character \";\"" << endl;
            break;
        case 3:
            cout << "[Grammar ERROR] "<< " [" << unit.line << "," << unit.column << "] " <<"Missing identifier after \"const\"" << endl;
            break;
        case 4:
            cout << "[Grammar ERROR] "<< " [" << unit.line << "," << unit.column << "] " <<"Missing assignment operation" << endl;
            break;
        case 5:
            cout << "[Grammar ERROR] "<< " [" << unit.line << "," << unit.column << "] " <<"Missing assignment integer" << endl;
            break;
        case 6:
            cout << "[Grammar ERROR] "<< " [" << unit.line << "," << unit.column << "] " <<"Missing identifier after \"var\"" << endl;
            break;
        case 7:
            cout << "[Grammar ERROR] "<< " [" << unit.line << "," << unit.column << "] " <<"Missing identifier after \",\"" << endl;
            break;
        case 8:
            cout << "[Grammar ERROR] "<< " [" << unit.line << "," << unit.column << "] " <<"Cannot resolve type \"" << unit.value << "\"" << endl;
            break;
        case 9:
            cout << "[Grammar ERROR] "<< " [" << unit.line << "," << unit.column << "] " <<"Missing identifier after \"procedure\"" << endl;
            break;
        case 10:
            cout << "[Grammar ERROR] "<< " [" << unit.line << "," << unit.column << "] " <<"Missing parentheses in \"procedure\"" << endl;
            break;
        case 11:
            cout << "[Grammar ERROR] "<< " [" << unit.line << "," << unit.column << "] " <<"Missing identifier in \"procedure\"" << endl;
            break;
        default:
            cout << "[Grammar ERROR] "<< " [" << unit.line << "," << unit.column << "] " <<"Unknown error" << endl;
            break;
    }
}

/**
 * <statement> → <id> := <exp>
               |if <lexp> then <statement>[else <statement>]
               |while <lexp> do <statement>
               |call <id>[（<exp>{,<exp>}）]
               |<body>
               |read (<id>{，<id>})
               |write (<exp>{,<exp>})
 */
void Statement() {

}

/**
 * <lexp> → <exp> <lop> <exp>|odd <exp>
 */
void Lexp() {

}


/**
 * <exp> → [+|-]<term>{<aop><term>}
 */
void Exp() {

}

/**
 * <term> → <factor>{<mop><factor>}
 */
void Term() {

}

/**
 * <factor>→<id>|<integer>|(<exp>)
 */
void Factor() {

}

/**
 * <body> → begin <statement>{;<statement>}end
 */
void Body() {

}

/**
 * <proc> → procedure <id>（<id>{,<id>}）;<block>{;<proc>}
 */
void Proc() {

    void Block();

    ReadLine();
    if(unit.key == "ID" || *errorType == 9) {
        if(*errorType == 9 && unit.key != "ID") errorType++;
        ReadLine();
        if (unit.key == "SOP" && unit.value == "(" || *errorType == 10){
            if (*errorType == 10 && unit.value != "(") errorType++;
            ReadLine();
            if (unit.key == "ID" || *errorType == 11) {
                if (*errorType == 11 && unit.key != "ID") errorType++;
                ReadLine();

                while (unit.value == "," && unit.key == "SOP"){

                    ReadLine();
                    if (unit.key == "ID"|| *errorType == 11){
                        if(unit.key != "ID") errorType++;
                        ReadLine();

                    } else{
                        ThrowError(11); // Missing id
                    }
                    if(error) break;
                }

                if (unit.key == "SOP" && unit.value == ")" || *errorType == 10){
                    if (*errorType == 10 && unit.value != ")") errorType++;
                    ReadLine();

                    if (unit.key == "EOP" && unit.value == ";" || *errorType == 2) {
                        if (*errorType == 2 && unit.value != ";") errorType++;
                        Block();

                        while (unit.key == "EOP" && unit.value == ";") {
                            Proc();
                        }
                    } else {
                        ThrowError(2);
                    }
                } else {
                    ThrowError(10);
                }

            } else {
                ThrowError(11);
            }
        } else {
            ThrowError(10);
        }
    } else {
        ThrowError(9);
    }
}

/**
 * <vardecl> → var <id>{,<id>};
 * <id> → l{l|d}
 * l represent letter.
 */
void Vardecl() {
    ReadLine();
    if (unit.key == "ID") {
        ReadLine();
        while (unit.value == ",", unit.key == "SOP") {
            ReadLine();
            if (unit.key == "ID") {
                ReadLine();
            } else {
                ThrowError(7);// Missing ID
            }
        }
        if (unit.value == ";" && unit.key == "EOP") {
            ReadLine();
            // Finish var declaration.
        } else {
            ThrowError(2); // Missing EOP
        }
    } else {
        ThrowError(6); // Missing ID
    }
}

/**
 * Const variables declaration.
 * <condecl> → const <const>{,<const>};
 * <const> → <id>:=<integer>
 * <id> → l{l|d}
 * l represent letter.
 */
void Condecl() {
    ReadLine();
    if (unit.key == "ID" || *errorType == 3){
        if(unit.key != "ID") errorType++;
        ReadLine();
        if (unit.value == ":=" && unit.key == "AOP"|| *errorType == 4) {
            if(!(unit.value == ":=" && unit.key == "AOP")) errorType++;
            ReadLine();
            if (unit.key == "INT"|| *errorType == 5) {
                if(unit.key != "INT") errorType++;
                ReadLine();
                while (unit.value == "," && unit.key == "SOP"){

                    ReadLine();
                    if (unit.key == "ID"|| *errorType == 3){
                        if(unit.key != "ID") errorType++;
                        ReadLine();
                        if (unit.value == ":=" && unit.key == "AOP"|| *errorType == 4){
                            if(!(unit.value == ":=" && unit.key == "AOP")) errorType++;
                            ReadLine();
                            if (unit.key == "INT"|| *errorType == 5) {
                                if(unit.key != "INT") errorType++;
                                ReadLine();
                            } else {
                                ThrowError(5); // Missing INT
                            }
                        } else {
                            ThrowError(4); // Missing AOP
                        }
                    } else{
                        ThrowError(3); // Missing id
                    }
                    if(error) break;
                }
                if (unit.value == ";" && unit.key == "EOP"|| *errorType == 2 || error) {
                    if(!(unit.value == ";" && unit.key == "EOP")) errorType++;
                    ReadLine();
                    // Finish const declaration.
                } else {
                    ThrowError(2); // Missing EOP
                }
            } else {
                ThrowError(5); // Missing INT
            }
        } else {
            ThrowError(4); // Missing AOP
        }
    } else{
        ThrowError(3); // Missing id
    }
}




/**
 * <block> → [<condecl>][<vardecl>][<proc>]<body>
 */
void Block() {
    ReadLine();

    if (unit.value == "const" && unit.key == "RESERVED") {
        Condecl();
    } else if (unit.key == "ID" && *errorType != 8){
        ThrowError(8); // Cannot resolve type
        while(unit.key != "EOP" && unit.key != "RESERVED"){
            ReadLine();
        }
        ReadLine();
    } else if (*errorType == 8) {
        errorType++;
        while(unit.key != "EOP" && unit.key != "RESERVED"){
            ReadLine();
        }
        ReadLine();
    }

    if (unit.value == "var" && unit.key == "RESERVED") {
        Vardecl();
    } else if (unit.key == "ID" && *errorType != 8){
        ThrowError(8); // Cannot resolve type
        while(unit.key != "EOP" && unit.key != "RESERVED"){
            ReadLine();
        }
        ReadLine();
    } else if (*errorType == 8) {
        errorType++;
        while(unit.key != "EOP" && unit.key != "RESERVED"){
            ReadLine();
        }
        ReadLine();
    }

    if (unit.value == "procedure" && unit.key == "RESERVED") {
        Proc();
    } else if (unit.key == "ID" && *errorType != 8){
        ThrowError(8); // Cannot resolve type
        while(unit.key != "EOP" && unit.key != "RESERVED"){
            ReadLine();
        }
        ReadLine();
    } else if (*errorType == 8) {
        errorType++;
        while(unit.key != "EOP" && unit.key != "RESERVED"){
            ReadLine();
        }
        ReadLine();
    }

    Body();
}


/**
 * <prog> → program <id>; <block>
 */
void Prog() {
    ReadLine();
    if ((unit.value == "program" && unit.key == "RESERVED") || *errorType == 0){
        if (*errorType == 0) errorType++;

        ReadLine();
        if (unit.key == "ID" || *errorType == 1) {
            if (*errorType == 1) errorType++;
            ReadLine();
            if ((unit.value == ";" && unit.key == "EOP") || *errorType == 2) {
                if (*errorType == 2) errorType++;
                Block();
            } else {
                ThrowError(2);
            }
        } else {
            ThrowError(1);
        }
    } else {
        ThrowError(0);
    }

}


void OpenFile() {
    gaSource.open("la_output", ios::in); // Read file
    gaOutput.open("ga_output", ios::out | ios::trunc); // Write file

    if (!gaSource.is_open()) {
        cout << "Cannot open the gaSource file!\a" << endl;
        exit(1);
    }
    if (!gaOutput.is_open()) {
        cout << "Cannot open the gaOutput file!\a" << endl;
    } else {
        // Header of the file (DateTime & File name & Lang set)

        time_t rawtime;
        struct tm * timeinfo;

        time (&rawtime);
        timeinfo = localtime (&rawtime);

        gaOutput << "# Grammar Analysis Result" << endl;
        gaOutput << "# Generated Time: " << asctime(timeinfo);
        gaOutput << "# Language Set: PL/0" << endl;
        gaOutput << endl;
    }
}

void CloseFile() {
    gaSource.close();
    gaOutput.close();
}

/**
 * Grammar analysis main program
 * @return
 */
int GA() {

    OpenFile();
    Prog();
    CloseFile();
    while(error){
        error = false;
        errorType = errorStack;
        OpenFile();
        Prog();
        CloseFile();
    }
    return 0;
}