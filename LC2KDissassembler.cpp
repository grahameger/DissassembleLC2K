#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

// first argument is a LC2K machine code file you'd like to disassemble
// second argument is the output file name. You may use any extension you like.

// If you type "stdout" and you filename, the program will print the output instead of putting it
// into a file

void print_offset(int instruction, std::ostream &output);
void print_dest_reg(int instruction, std::ostream &output);
void print_registers(int instruction, std::ostream &output);
void print(int instruction, std::ostream &output);
int signExtension(int instruction);

int main(int argc, char ** argv) {
    
    // error check for the number of arguments
    if (argc != 3)
    {
        std::cerr << "Incorrect number of arguments. Must have 3 arguments\n";
    }
    
    // get the files names into strings
    std::string in_file_name(argv[1]);
    std::string out_file_name(argv[2]);
    
    // get a stringstream for an output
    std::stringstream output;
    
    // setup the instreams
    std::ifstream in_file(in_file_name);
    
    int temp;
    std::vector<int> instructions;
    
    while (in_file >> temp) {
        instructions.push_back(temp);
    }
    
    for (int i = 0; i < instructions.size(); ++i) {
        print(instructions[i], output);
    }
    
    // either print it to stdout or a file
    if (in_file_name == "stdout")
    {
        std::cout << output.str();
    } else {
        std::ofstream out_file(out_file_name);
        out_file << output.str();
    }
    
    return 0;
    
}

void print(int instruction, std::ostream &output) {
    int opcode = (instruction & 0x01C00000) >> 22;
    switch (opcode) {
        case 0:
            output << "add\t";
            print_registers(instruction, output);
            print_dest_reg(instruction, output);
            output << '\n';
            break;
        case 1:
            output << "nor\t";
            print_registers(instruction, output);
            print_dest_reg(instruction, output);
            output << '\n';
            break;
        case 2:
            output << "lw\t";
            print_registers(instruction, output);
            print_offset(instruction, output);
            output << '\n';
            break;
        case 3:
            output << "sw\t";
            print_registers(instruction, output);
            print_offset(instruction, output);
            output << '\n';
            break;
        case 4:
            output << "beq\t";
            print_registers(instruction, output);
            print_offset(instruction, output);
            output << '\n';
            break;
        case 5:
            output << "jalr\t";
            print_registers(instruction, output);
            output << '\n';
            break;
        case 6:
            output << "halt\t";
            output << '\n';
            break;
        case 7:
            output << "noop\t";
            output << '\n';
            break;
    }
}

void print_registers(int instruction, std::ostream &output) {
    // get both the registers
    int regA = instruction >> 19;
    int regB = instruction >> 16;
    regA &= 0x0000007;
    regB &= 0x0000007;
    
    // print them
    output << regA << "\t" << regB << "\t";
}

void print_dest_reg(int instruction, std::ostream &output) {
    instruction &= 0x0000007;
    output << instruction << "\t";
}

void print_offset(int instruction, std::ostream &output) {
    // just get the least significant 16 bits
    // with a short cout will get the sign bit correct
    instruction &= 0x0000FFFF;
    instruction = signExtension(instruction);
    // ouput it
    output << instruction << "\t";
}

int signExtension(int instruction) {
    int value = (0x0000FFFF & instruction);
    int mask = 0x00008000;
    if (mask & instruction) {
        value += 0xFFFF0000;
    }
    return value;
}
