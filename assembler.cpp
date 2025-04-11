/*****************************************************************************
TITLE: Claims																																
AUTHOR: KOMMANA GOWTHAM
ROLL NO.: 2301AI12
Declaration of Authorship
This txt file, claims.txt, is part of the Mini project of CS2102
ARTIFICIAL INTELLIGENCE AND DATA SCIENCE, IIT Patna . 
*****************************************************************************/

#include <bits/stdc++.h>
using namespace std;

string trim(string s)       //Function for trimmation of the given string
{
    size_t first = s.find_first_not_of("\t\n\r\f\v ");      //finding the first non white-space in the string
    if (first == string::npos) 
    {
        return "";   // Return empty string when string doesn't contain non empty element
    }
    size_t last = s.find_last_not_of("\t\n\r\f\v ");      //finding the last non white-space in the string
    return s.substr(first, last - first + 1); // Return trimmed string by value
}

bool is_number(string s) // Function for checking if the given string is a valid number
{
    if (s.empty()) 
    {
        return false;   // Return false if the string is empty
    }

    int start = (s[0] == '-' || s[0] == '+') ? 1 : 0; // Skip optional sign

    // Hexadecimal number check (0x or 0X followed by valid hex digits)
    if ((s.find("0x", start) == start || s.find("0X", start) == start) && s.length() > start + 2)
    {
        for (int i = start + 2; i < s.length(); i++) 
        {
            if (!((s[i] >= '0' && s[i] <= '9') || (s[i] >= 'A' && s[i] <= 'F') || (s[i] >= 'a' && s[i] <= 'f')))
            {
                return false; // Invalid hexadecimal character
            }
        }
        return true;
    }

    // Octal number check (0 followed by valid octal digits)
    if (s[start] == '0' && s.length() > start + 1)
    {
        for (int i = start + 1; i < s.length(); i++) 
        {
            if (s[i] < '0' || s[i] > '7') // Valid octal digits are between 0 and 7
            {
                return false; // Invalid octal character
            }
        }
        return true;
    }

    // Decimal number check (only digits)
    for (int i = start; i < s.length(); i++) 
    {
        if (!(s[i] >= '0' && s[i] <= '9')) 
        {
            return false; // Invalid decimal character
        }
    }

    return true; // Return true if all characters are valid
}


int charToValue(char c) 
{
    if (std::isdigit(c)) 
    {
        return c - '0'; // Convert character '0'-'9' to 0-9
    } 
    else if (std::isalpha(c)) 
    {
        return std::toupper(c) - 'A' + 10; // Convert 'A'-'Z' to 10-35
    }
    return -1; // Invalid character
}

long long string_to_number(const std::string& str, int base = 10) 
{
    long long result = 0; // Variable to store the final number
    bool isNegative = false; // Flag to indicate if the number is negative
    size_t start = 0; // Position in string to start processing

    // Handle sign at the beginning of the string
    if (str[start] == '-') 
    {
        isNegative = true; // Mark as negative if '-' is found
        start++; // Move start position to the next character
    } 
    else if (str[start] == '+') 
    {
        start++; // Ignore '+' at the start and move to next character
    }

    // Detect base from prefix
    if (str.length() > start + 1 && str[start] == '0') 
    {
        if (str[start + 1] == 'x' || str[start + 1] == 'X') 
        {
            base = 16;  // Override to hexadecimal
            start += 2; // Skip "0x" or "0X" prefix
        } 
        else if (std::isdigit(str[start + 1])) 
        {
            base = 8;   // Override to octal
            start += 1; // Skip '0' prefix for octal
        }
    }

    // Convert each character to the corresponding value in the specified base
    for (size_t i = start; i < str.length(); ++i) 
    {
        int value = charToValue(str[i]); // Convert character to its base value

        // Check if character is valid for the given base
        if (value == -1 || value >= base) 
        {
            throw std::invalid_argument("Invalid character for base " + std::to_string(base) + ": " + str[i]);
        }

        // Update result by multiplying with the base and adding current character's value
        result = result * base + value; 
    }

    // Return result, making it negative if needed
    return isNegative ? -result : result;
}


struct symbol
{
    bool is_set;
    int set_value;
    string label_name;
    int label_address;
};

vector<symbol> symbol_table;


struct literal
{
	int literal;
	int literal_address;
};

vector<literal> literal_table;


bool contains(string name)
{
    for (int i = 0;i<symbol_table.size() ; i++) 
    {
        string temp = symbol_table[i].label_name;
        if (temp == name) 
        {
            return true; // Found the name in the symbol table
        }
    }
    return false; // Name not found
}


string int_to_oct(int i) // Converts an integer to an 8-character octal string
{
    char buffer[9]; // Buffer to hold 8 octal digits + null terminator
    snprintf(buffer, sizeof(buffer), "%08o", i); // Format as zero-padded 8-character octal

    return string(buffer); // Convert to string and return
}


string int_to_hex(int i) // Converts an integer to an 8-character hex string
{
    char buffer[9]; // Buffer to hold 8 hex digits + null terminator
    snprintf(buffer, sizeof(buffer), "%08x", i); // Format as zero-padded 8-character hex

    return string(buffer); // Convert to string and return
}


bool is_valid_label(string label)
{
    // Check if the first character is a valid letter (A-Z or a-z)
    if (((label[0] < 'A' || label[0] > 'Z') && (label[0] < 'a' || label[0] > 'z')))
    {
        return false; // If the first character is not a letter, return false
    }

    // Loop through the remaining characters of the label
    for (int i = 1; i < label.length(); i++)
    {
        // Check if each character is a digit or a letter (A-Z or a-z)
        if (!(isdigit(label[i])) && (label[i] < 'A' && label[i] > 'Z') && (label[i] < 'a' && label[i] > 'z') && label[i]!='_')
        {
            return false; // If any character is not valid, return false
        }
    }

    return true; // If all characters are valid, return true
}



map<string, string> op_code;

void MOT_INIT()
{
	op_code["ldc"] = string("00");
	op_code["adc"] = string("01");
	op_code["ldl"] = string("02");
	op_code["stl"] = string("03");
	op_code["ldnl"] = string("04");
	op_code["stnl"] = string("05");
	op_code["add"] = string("06");
	op_code["sub"] = string("07");
	op_code["shl"] = string("08");
	op_code["shr"] = string("09");
	op_code["adj"] = string("0A");
	op_code["a2sp"] = string("0B");
	op_code["sp2a"] = string("0C");
	op_code["call"] = string("0D");
	op_code["return"] = string("0E");
	op_code["brz"] = string("0F");
	op_code["brlz"] = string("10");
	op_code["br"] = string("11");
	op_code["HALT"] = string("12");
    op_code["SET"] = string("13");
	op_code["data"] = string("14");
}


string instruction_to_tables_errors(int line ,string instruction, int* location )
{
    string code_errors = ""; // Stores error messages related to code
    int where = *location; // Holds the current program counter position
    instruction = trim(instruction); // Removes any leading/trailing whitespace from instruction
    
    // Check if the instruction has a label by finding ":"
    if(instruction.find(":") == string::npos)
    {
        int operand_start = instruction.find(" ", 0); // Finds start of operand in the instruction
        string operand = instruction.substr((operand_start + 1), (instruction.length())); // Extracts operand
        string _operand = trim(operand); // Removes any whitespace from operand

        // If operand is a number, add it to the literal table
        if(is_number(_operand))
        {
            int gow = string_to_number(_operand, 10); // Converts operand string to integer
            literal_table.push_back({gow, -1}); // Adds literal with default address of -1
        }
    }
    else // Instruction contains a label
    {
        int label_end = instruction.find(":", 0); // Finds the end of the label
        string label = instruction.substr(0, label_end); // Extracts the label
        string _label = trim(label); // Removes any whitespace from label

        // Checks for duplicate labels in the symbol table
        if(contains(_label))
        {
            cout << "ERROR : Duplicate Label at the line " << line << endl;
            code_errors = code_errors + ("ERROR : Duplicate Label at the line " + to_string(line) + "\n");
        }

        // Validates the label name format
        if (!(is_valid_label(_label)))
        {
            cout << "ERROR : Invalid Label Name at line " << line << endl;
            code_errors = code_errors + ("ERROR : Invalid Label Name at line " + to_string(line) + "\n");
        }

        // If there’s code after the label, process it as a real instruction
        if(label_end != instruction.length() - 1)
        {
            string real_instruction = instruction.substr(label_end + 1, (instruction.length()) - (label_end + 1)); // Extracts the real instruction after the label
            real_instruction = trim(real_instruction); // Removes any whitespace from the real instruction
            instruction_to_tables_errors(line, real_instruction, &where); // Recursively analyze the real instruction

            // Further process for operand and value within real instruction
            //code_errors = code_errors;
            string operation = real_instruction.substr(0, real_instruction.find(" ", 0)); // Extracts the operation part
            string operand = real_instruction.substr(real_instruction.find(" ", 0) + 1, real_instruction.length() - (real_instruction.find(" ", 0) + 1)); // Extracts the value part
            string _operation = trim(operation); // Trims whitespace from operand
            string _value = trim(operand); // Trims whitespace from value

            // Check if the operation is "SET" and update the symbol table accordingly
            if(_operation != "SET")
            {
                symbol_table.push_back({0, -1, _label, where}); // Adds label to symbol table with default value
            }
            else
            {
                symbol_table.push_back({1, stoi(_value), _label, where}); // Adds "SET" label with specified value
            }
        }
        else // Only label is present, no additional instruction
        {
            *location = *location - 1; // Adjust program counter
            symbol_table.push_back({0, -1, _label, where}); // Adds label to symbol table with default value
        }
    }

    return code_errors; // Returns any collected error messages
}

void analyse(string code, ofstream& logfile)
{
    int program_counter = 0; // Program counter to track current location
    int no_of_line = 1; // Line counter for error logging
    string line; // Holds each line of code to process

    ifstream code_file(code); // Opens the code file for reading

    // Reads each line from the code file
    while(getline(code_file, line))
    {
        line = trim(line); // Removes whitespace from line
        string instruction;
        int semi_column = line.find(";", 0); // Finds the start of comment
        instruction = line.substr(0, semi_column); // Extracts the code part, ignoring comment
        instruction = trim(instruction); // Removes any whitespace from instruction

        // If line is empty, continue to the next line
        if(instruction.empty())
        {
            no_of_line++;
            continue;
        }

        logfile << instruction_to_tables_errors(no_of_line++, instruction, &program_counter); // Logs any errors to logfile
        program_counter++; // Increment program counter for the next instruction
    }

    // Process literals after parsing instructions
    int size = literal_table.size();
    for(int i = 0; i < size; i++)
    {
        // Assign addresses to literals without addresses
        if(literal_table[i].literal_address == -1)
        {
            literal_table[i].literal_address = program_counter; // Assign current program counter as address
            program_counter++; // Increment program counter for next literal
        }
    }

    code_file.close(); // Close the code file
}


tuple<string , string , string> instruction_to_code( int line , string instruction , int* location )
{
    int program_counter = *location;

    string ENCODING = "";
    string ERRORS = "";
    string MACHINE_CODE = "";
    string temp_encoding = "";

    ENCODING = int_to_hex(program_counter) + " ";
    if(instruction.find(":", 0) == string::npos)  // No label
    {
        instruction = trim(instruction);

        string operation_instruction = instruction.substr(0 , instruction.find(" ", 0));
        string _operation_instruction = trim(operation_instruction);
        string operand_instruction = instruction.substr(instruction.find(" ", 0) + 1, instruction.length());
        string _operand_instruction = trim(operand_instruction);
        
        if(op_code[_operation_instruction] == "")  // Check if operation exists in op_code
        {
            cout << "ERROR : Undefined Mnemonic is used in the line " << line << endl;
            ERRORS = ERRORS + "ERROR: Undefined Mnemonic is used in the line " + to_string(line) + "\n";
        }
        else if(op_code[_operation_instruction] == "06" || op_code[_operation_instruction] == "07" || op_code[_operation_instruction] == "08" || op_code[_operation_instruction] == "09" || op_code[_operation_instruction] == "0B" || op_code[_operation_instruction] == "0C" || op_code[_operation_instruction] == "0E" || op_code[_operation_instruction] == "12")
        {
            // ENCODING = ENCODING +"000000" + op_code[_operation_instruction] + " ";
            // MACHINE_CODE = MACHINE_CODE + "000000" + op_code[_operation_instruction] + " ";
            if( _operation_instruction .length() != instruction.length())
            {
                ERRORS += "ERROR: Operand does not placed at a line " + to_string(line) + "\n";
                cout << "ERROR: Oprand does not placed at a line " << line << endl;
            }
            else
            {
                ENCODING = ENCODING +"000000" + op_code[_operation_instruction] + " ";
                MACHINE_CODE = MACHINE_CODE + "000000" + op_code[_operation_instruction] + " ";
            }
        
        }
        else if(is_number(_operand_instruction))
        {
            string hexadecimal = int_to_hex( string_to_number(_operand_instruction , 10));
            
            if(op_code[_operation_instruction] == "13" || op_code[_operation_instruction] == "14")
            {
                ENCODING = ENCODING +hexadecimal + " ";
                MACHINE_CODE = MACHINE_CODE + hexadecimal+ " ";
            }
            else
            {
                ENCODING = ENCODING +hexadecimal.substr( 2 , hexadecimal.length()-2) + op_code[_operation_instruction] + " ";
                MACHINE_CODE = MACHINE_CODE + hexadecimal.substr(2 , hexadecimal.length()-2) + op_code[_operation_instruction] + " ";
            }
        }
        else
        {
            bool check =0;
            int size = symbol_table.size();
            for(int i = 0; i <size; i++)
            {
                string hexadecimal;
                if(((symbol_table[i].label_name.compare(_operand_instruction) == 0)))
                {

                    check = 1;
                    if(symbol_table[i].is_set == true)
                    {
                        hexadecimal = int_to_hex(symbol_table[i].set_value);
                        ENCODING  = ENCODING +  hexadecimal.substr( 2 , hexadecimal.length()-2) + op_code[_operation_instruction] + " ";
                        MACHINE_CODE = MACHINE_CODE + hexadecimal.substr(2 , hexadecimal.length()-2) + op_code[_operation_instruction] + " ";
                        break;
                    }
                    else if(op_code[_operation_instruction] == "0D" || op_code[_operation_instruction] == "0F" ||
                            op_code[_operation_instruction] == "10" || op_code[_operation_instruction] == "11"   )
                    {
                        hexadecimal = int_to_hex(symbol_table[i].label_address - program_counter - 1);
                        ENCODING  = ENCODING + hexadecimal.substr( 2 , hexadecimal.length()-2) + op_code[_operation_instruction] + " ";
                        MACHINE_CODE = MACHINE_CODE + hexadecimal.substr(2 , hexadecimal.length()-2) + op_code[_operation_instruction] + " ";
                        break;
                    }
                    else
                    {
                        hexadecimal = int_to_hex(symbol_table[i].label_address);
                        ENCODING  = ENCODING + hexadecimal.substr( 2 , hexadecimal.length()-2) + op_code[_operation_instruction] + " ";
                        MACHINE_CODE = MACHINE_CODE + hexadecimal.substr(2 , hexadecimal.length()-2) + op_code[_operation_instruction] + " ";
                        break;
                    }
                }
            }

            if(operation_instruction.length() == instruction.length())
            {
                cout << "ERROR: Operand must be placed at line " << line << endl;
                ERRORS += "ERROR: Operand must be placed at line " + to_string(line) + "\n"; 
            }
            else if(!(check) && (_operation_instruction.length() != instruction.length()))
            {
                cout << "ERROR : Undefined OPerand is used in the line " << line << endl;
                ERRORS += "ERROR : Undefined OPerand is used in the line "+to_string(line)+"\n";
            }
        }
        ENCODING = ENCODING + instruction + "\n";
    }
    else
    {
        instruction = trim(instruction);
        if((instruction.find(":", 0) + 1) == instruction.length())
        {
            ENCODING = ENCODING + "         ";
            *location = *location - 1; 
        }
        else
        {
            string real_instruction = instruction.substr(instruction.find(":", 0) + 1, instruction.length());
            real_instruction = trim(real_instruction);

            tuple<string, string, string> result;
            result = (instruction_to_code( line , real_instruction, &program_counter));
            temp_encoding = get<0>(result);
            ERRORS = get<1>(result);
            MACHINE_CODE = get<2>(result);

            string temp = temp_encoding;
            string line_machine_code = temp.substr(9, 9);
            ENCODING += line_machine_code;
        }
        ENCODING += instruction + "\n";
    }

    return make_tuple(ENCODING, ERRORS, MACHINE_CODE);
}

void synthesize(string code, ofstream& outfile, ofstream& logfile, ofstream& objfile)
{
    ifstream code_file(code);
    string code_line;
    bool is_success = true;
    int no_of_line = 1;
    int Program_Counter = 0;
    while (true)
    {
        is_success = bool(getline(code_file, code_line));  // Read the next line

        if (is_success) 
        {
            code_line = trim(code_line);

            string real_instruction;

            int operand = code_line.find(";", 0);
            real_instruction = code_line.substr(0, operand);
            real_instruction = trim(real_instruction);

            if (real_instruction.empty()) 
            {
                no_of_line++;
                continue;
            }

            // Call the function to process the instruction
            tuple<string, string, string> result;
            result = instruction_to_code(no_of_line++, real_instruction, &Program_Counter);

            // Extract the results from the tuple
            string ENCODING = get<0>(result);
            string ERRORS = get<1>(result);
            string MACHINE_CODE = get<2>(result);

            // Write the results to the log and output files
            logfile << ERRORS;
            outfile << ENCODING;

            if (MACHINE_CODE != "")
            {
                uint32_t hexa_decimal = stoul("0x" + trim(MACHINE_CODE), nullptr, 16);
                objfile.write((char *)&hexa_decimal, sizeof(hexa_decimal));
            }
            Program_Counter++;
        }
        else 
        {
            break;
        }
    }
}

int main(int argc, char* argv[])
{
	
	MOT_INIT();

	string in_file = argv[1];


	string LOG_File = in_file.substr(0, in_file.find(".", 0)) + ".log";
    ofstream  _LOG_File( LOG_File);

	string OUT_File = in_file.substr(0, in_file.find(".", 0)) + ".L";
    ofstream _OUT_File(OUT_File);

	string OBJ_File = in_file.substr(0, in_file.find(".", 0)) + ".o";
    ofstream _OBJ_File(OBJ_File,ios::out | ios::binary);

    analyse(in_file,_LOG_File);
    
    synthesize(in_file, _OUT_File,_LOG_File,_OBJ_File);

    _OUT_File.close();
    _LOG_File.close();
    _OBJ_File.close();
}