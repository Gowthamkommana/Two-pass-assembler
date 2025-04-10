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

void inset()
{
    // Display table headers
    cout << "Opcode |   Mnemonic  | HEX Opcode |   Operand     " << endl;

    // List each opcode, mnemonic, hexadecimal representation, and operand type
    cout << "  00   |     ldc     |     00     |    Value      " << endl;
    cout << "  01   |     adc     |     01     |    Value      " << endl;
    cout << "  02   |     ldl     |     02     |    offset     " << endl;
    cout << "  03   |     stl     |     03     |    offset     " << endl;
    cout << "  04   |     ldnl    |     04     |    offset     " << endl;
    cout << "  05   |     stnl    |     05     |    offset     " << endl;
    cout << "  06   |     add     |     06     |               " << endl;
    cout << "  07   |     sub     |     07     |               " << endl;
    cout << "  08   |     shl     |     08     |               " << endl;
    cout << "  09   |     shr     |     09     |               " << endl;
    cout << "  10   |     adj     |     0A     |    Value      " << endl;
    cout << "  11   |     a2sp    |     0B     |               " << endl;
    cout << "  12   |     sp2a    |     0C     |               " << endl;
    cout << "  13   |     call    |     0D     |    offset     " << endl;
    cout << "  14   |    return   |     0E     |               " << endl;
    cout << "  15   |     brz     |     0F     |    offset     " << endl;
    cout << "  16   |     brlz    |     10     |    offset     " << endl;
    cout << "  17   |      br     |     11     |    offset     " << endl;
    cout << "  18   |     HALT    |     12     |               " << endl;
    cout << "  19   |     SET     |     13     |    Value      " << endl;
    cout << "  20   |     data    |     14     |    Value      " << endl;
}


string int_to_hex(int i)
{
    char buffer[9]; // Buffer to hold 8 hex digits + null terminator
    snprintf(buffer, sizeof(buffer), "%08x", i); // Format as zero-padded 8-character hex

    return string(buffer); // Convert to string and return
}

// Array to represent memory with 2000 integer slots
int memory[100000];

// Registers and pointers for program execution
int program_counter, stack_pointer, A, B;

// Flag to indicate when to stop the program
bool stop = false;

// Vector to store the machine codes
vector<int> MACHINE_CODES;


// Structure to represent a line of read code with a value and an address
struct read_code_line
{
    int value;   // Value being read
    int address; // Address from which the value is read
};

// Vector to store instances of read_code_line
vector<read_code_line> R1;

// Structure to represent a line of write code with previous and current values and an address
struct write_code_line 
{
    int prev_value;  // Previous value at the address
    int pres_value;  // Current value to be written at the address
    int address;     // Address to which the value is written
};

// Vector to store instances of write_code_line
vector<write_code_line> W1;


// Function to load a constant value (operand) into the register A, and set B to the previous value of A
int ldc(int operand)
{
    B = A;        // Move the current value of A into B for backup
    A = operand;  // Load the operand value into A
    return 0;
}

// Function to add a constant operand to the current value of A
int adc(int operand)
{
    A = A + operand; // Add the operand to A
    return 0;
}

// Function to load a value from memory at stack_pointer + operand into A, while saving the previous A in B
int ldl(int operand)
{
    B = A; // Backup the current value of A into B
    A = memory[stack_pointer + operand]; // Load from memory at SP + offset into A
    R1.push_back({ (A, stack_pointer + operand) }); // Log the read
    return 0;
}

// Function to store A into memory at stack_pointer + operand, then restore the previous A from B
int stl(int operand)
{
    int temp = memory[stack_pointer + operand]; // Store current memory value
    memory[stack_pointer + operand] = A; // Write A's value to memory at SP + offset
    A = B; // Restore previous A from B
    W1.push_back({temp, memory[stack_pointer + operand], stack_pointer + operand}); // Log the write
    return 0;
}

// Function to load from memory at address A + operand into A
int ldnl(int operand)
{
    int temp = A; // Backup the current A for address calculation
    A = memory[A + operand]; // Load from memory at (A + offset) into A
    R1.push_back({ (A, temp + operand) }); // Log the read
    return 0;
}


// Function to store B in memory at address A + operand
int stnl(int operand)
{
    int temp = memory[A + operand]; // Store current memory value at (A + offset)
    memory[A + operand] = B; // Write B to memory
    W1.push_back({B, A + operand, temp});// Log the write
    return 0;
}

// Function to add values of A and B and store result in A
int add()
{
    A = B + A; // Perform addition of A and B
    return 0;
}

// Function to subtract value of A from B and store result in A
int sub()
{
    A = B - A; // Perform subtraction of A from B
    return 0;
}

// Function to shift B left by the number of bits in A
int shl()
{
    A = (B << A); // Perform left shift of B by A
    return 0;
}

// Function to shift B right by the number of bits in A
int shr()
{
    A = (B >> A); // Perform right shift of B by A
    return 0;
}

// Function to adjust stack pointer by a specified operand
int adj(int operand)
{
    stack_pointer = stack_pointer + operand; // Adjust stack pointer by operand
    return 0;
}

// Function to set stack pointer to value in A, and restore previous A from B
int a2sp()
{
    stack_pointer = A; // Move A's value to stack pointer
    A = B; // Restore previous A from B
    return 0;
}

// Function to move stack pointer's value to A, and previous A to B
int sp2a()
{
    B = A; // Backup A in B
    A = stack_pointer; // Move stack pointer's value into A
    return 0;
}

// Function to call a subroutine by saving program counter into A and setting program counter to operand offset
int call(int operand)
{
    B = A; // Backup A in B
    A = program_counter; // Save current program counter in A
    program_counter = program_counter + operand; // Adjust program counter by operand
    return 0;
}

// Function to return from subroutine by setting program counter to A and restoring A from B
int _return()
{
    program_counter = A; // Restore program counter from A
    A = B; // Restore A from B
    return 0;
}

// Function to branch if A equals zero
int brz(int operand)
{
    if (A == 0) // Check if A is zero
    {
        program_counter = program_counter + operand; // Adjust program counter by operand
    }
    return 0;
}

// Function to branch if A is less than zero
int brlz(int operand)
{
    if (A < 0) // Check if A is negative
    {
        program_counter = program_counter + operand; // Adjust program counter by operand
    }
    return 0;
}

// Function for an unconditional branch by adjusting program counter by operand
int br(int operand)
{
    program_counter = program_counter + operand; // Adjust program counter by operand
    return 0;
}



void GET_TRACE(int size)
{
    program_counter = 0;
    int total_no_of_instructions = 0;

    while(program_counter< size)
    {
        int old_program_counter = program_counter;

        int instr_MACHINE_CODE = MACHINE_CODES[program_counter];

        int opcode_value =  bitset<8>(instr_MACHINE_CODE).to_ulong();
        // cout<<opcode_value<<endl;

        int operand = (instr_MACHINE_CODE >> 8) & 0x7FFFFF;
        operand =  (instr_MACHINE_CODE & (1 << 31)) ? operand - (1 << 23) : operand;

        if(opcode_value == 0)
        {
            ldc(operand);
        }
        else if(opcode_value == 1)
        {
            adc(operand);
        }
        else if(opcode_value == 2)
        {
            ldl(operand);
        }
        else if(opcode_value == 3)
        {
            stl(operand);
        }
        else if(opcode_value == 4)
        {
            ldnl(operand);
        }
        else if(opcode_value == 5)
        {
            stnl(operand);
        }
        else if(opcode_value == 6)
        {
            add();
        }
        else if(opcode_value == 7)
        {
            sub();
        }
        else if(opcode_value == 8)
        {
            shl();
        }
        else if(opcode_value == 9)
        {
            shr();
        }
        else if(opcode_value == 10)
        {
            adj(operand);
        }
        else if(opcode_value == 11)
        {
            a2sp();
        }
        else if(opcode_value == 12)
        {
            sp2a();
        }
        else if(opcode_value == 13)
        {
            call(operand);
        }
        else if(opcode_value == 14)
        {
            _return();
        }
        else if(opcode_value == 15)
        {
            brz(operand);
        }
        else if(opcode_value == 16)
        {
            brlz(operand);
        }
        else if(opcode_value == 17)
        {
            br(operand);
        }
        else if(opcode_value == 18)
        {
            cout << total_no_of_instructions<< " number of instructions executed" << endl << endl;
            return ;
        }
        program_counter = program_counter + 1 ;

        if(old_program_counter == program_counter)
        {
            cout<<"infinite loop detected in this code"<<endl;
            break;
        }

        total_no_of_instructions++;

        cout<<"program counter = "<<int_to_hex(program_counter)<<" A = "<<int_to_hex(A)<<" B = "<<int_to_hex(B)<<" stack pointer = "<<int_to_hex(stack_pointer)<<" "<<total_no_of_instructions<<endl;

    }
    cout<<"Number of instructions ran in the given code is "<<total_no_of_instructions<<endl;
}


void welcome_msg()
{
        cout<<"                       WELCOME TO THIS EMULATOR                         "<<endl;
        cout<<"       These are the Actions can be implemented in this emulator        "<<endl;
        cout<<" To get trace of a program                                     : -trace "<<endl;
        cout<<" show memory before code excution (please use before excution) : -before"<<endl;
        cout<<" show memory after code excution  (please use after excution)  : -after "<<endl;
        cout<<" Show memory reads                                             : -read  "<<endl;
        cout<<" show memory writes                                            : -write "<<endl; 
        cout<<" Show Operands with opcode's                                   : -isa   "<<endl;
        cout<<" Show registers and SP values at that time                     : -reg   "<<endl; 
}

void show_memory()
{
    int temp = MACHINE_CODES.size();
    for(int i=0 ; i<temp ; i=i+4)
    {
        cout<<int_to_hex(i)<<"  ";
        int bound = min( temp , i + 4 );
        for(int j = i ; j < bound ; j++)
        {
            cout<<int_to_hex(memory[j])<<" ";
        }
        cout<<endl;
    }
}

void show_reg()
{
   cout<<"program counter = "<<int_to_hex(program_counter)<<" A = "<<int_to_hex(A)<<" B = "<<int_to_hex(B)<<" stack pointer = "<<int_to_hex(stack_pointer)<<endl;
}

int main()
{
    cout<<"Enter the name of the file (with file extension) : "<<endl;
    string name_of_a_file;
    cin>>name_of_a_file;

    ifstream file;
    file.open( name_of_a_file , ios::in | ios::binary );

    int memory_address = 0;
    for (int i; file.read(reinterpret_cast<char*>(&i), sizeof(int)); ) 
    { 
        MACHINE_CODES.emplace_back(i);
        memory[memory_address++] = i;
    }

    int machine_code_size = MACHINE_CODES.size();
    while(true)
    {
        welcome_msg();
        cout<<"Enter the instruction do you want to implement(ENTER 0 to exit):";
        string s;
        cin>>s;
        if(s=="-trace")
        {
            GET_TRACE(machine_code_size);
        }
        else if(s == "-before")
        {
            cout<<"Memory layout before code excution :"<<endl;
            show_memory();
        }
        else if(s == "-after")
        {
            cout<<"Memory layout after code excution :"<<endl;
            show_memory();
        }
        else if(s == "-isa")
        {
            inset();
        }
        else if(s == "-reg")
        {
            show_reg();
        }
        else if(s == "0")
        {
            exit(0);
        }
        else if(s == "-read")
        {
            if(R1.empty())
            {
                cout<<"There is no read instructions in this code"<<endl;
            }
            else
            {
                for(int i=0;i<R1.size();i++)
                {
                    cout<<"Read memory at "<<int_to_hex(R1[i].address)<<" found "<<int_to_hex(R1[i].value)<<endl;
                }
            }
        }
        else if(s == "-write")
        {
            if(W1.empty())
            {
                cout<<"There is no Write instructions in this code"<<endl;
            }
            else
            {
                for(int i=0;i<W1.size();i++)
                {
                    cout<<"Wrote memory at "<<int_to_hex(W1[i].address)<<" changed from "<<W1[i].prev_value<<" to "<<W1[i].pres_value<<endl;
                }
            }
        }
        else
        {
            cout<<"write correct instruction which was shown"<<endl;
        }
    }
    return 0;
}