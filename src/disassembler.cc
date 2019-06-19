#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#define I_SIZE 8

int loadBytes(std::vector<unsigned char>& bytes, std::string filepath) {

    std::ifstream file;
    file.open(filepath, std::ios::binary);

    if (file.fail()) {
        std::cout << "Failed to read from file" << std::endl; //I should learn exceptions :(
        return 1;
    }

    unsigned char byte;
    while (file >> byte) {
        bytes.push_back(byte);
    }

    return 0;
}


/* I should have just listed out the different combinations
 * On the brightside i reviewed my bitwise operators
 * */
int readBitsAt(unsigned char code, int start, int end) {
   code = code >> (I_SIZE - end);
   return code & ~( (unsigned) -1 << (end - start));
}

int readSource(unsigned char code) {
    return readBitsAt(code, 5, 8);
}

int readDest(unsigned char code) {
    return readBitsAt(code, 2, 5);
}

int readRP(unsigned char code) {
    return readBitsAt(code, 2, 4);
}

std::string getReg(int a) {

    switch(a) {
        case 0b111:
            return "A";
        case 0b000:
            return "B";
        case 0b001:
            return "C";
        case 0b010:
            return "D";
        case 0b011:
            return "E";
        case 0b100:
            return "H";
        case  0b101:
            return "L";
        default:
            std::cout << "Failed to find opcode(R)" << std::endl;
            return "";
    }
}

std::string getRegPair(int a) {
    switch(a) {
        case 0b00:
            return "B - C";
        case 0b01:
            return "D - E";
        case 0b10:
            return "H - L";
        case 0b11:
            return "SP";
        default:
            std::cout << "Failed to find opcode(RP)" << std::endl;
            return "";
    }
}

int disassembleOp(const std::vector<unsigned char>& buffer, int pc) {
    unsigned char code = buffer[pc];
    int op_size = 1;
    
    switch (code) {
        case 0b00110110:
            std::cout << "MVI M,data " << (int)buffer[pc + op_size++]; break;
        case 0b00111010:
            std::cout << "LDA addr " << (int)buffer[pc + op_size++] << (int)buffer[pc + op_size++]; break;
        case 0b00110010:
            std::cout << "STA addr " << (int)buffer[pc + op_size++] << (int)buffer[pc + op_size++]; break;
        case 0b00101010:
            std::cout << "LHLD addr " << (int)buffer[pc + op_size++] << (int)buffer[pc + op_size++]; break;
        case 0b00100010:
            std::cout << "SHLD addr "  << (int)buffer[pc + op_size++] << (int)buffer[pc + op_size++]; break;
        case 0b10000110:
            std::cout << "ADD M "; break;
        case 0b11000110:
            std::cout << "ADI data " << (int)buffer[pc + op_size++]; break;
        case 0b10001110:
            std::cout << "ADC M "; break;
        case 0b11001110:
            std::cout << "ACI data " << (int)buffer[pc + op_size++]; break;
        case 0b10010110:
            std::cout << "SUB M "; break;
        case 0b11010110:
            std::cout << "SUI data " << (int)buffer[pc + op_size++]; break;
        case 0b10011110:
            std::cout << "SBB M "; break;
        case 0b11011110:
            std::cout << "SBI data " << (int)buffer[pc + op_size++]; break;
        case 0b00110100:
            std::cout << "INR M "; break;
        case 0b00110101:
            std::cout << "DCR M "; break;
        case 0b00100111:
            std::cout << "DAA "; break;
        case 0b10100110:
            std::cout << "ANA M "; break;
        case 0b11100110:
            std::cout << "ANI data " << (int)buffer[pc + op_size++]; break;
        case 0b10101110:
            std::cout << "XRA M "; break;
        case 0b11101110:
            std::cout << "XRI data " << (int)buffer[pc + op_size++]; break;
        case 0b10110110:
            std::cout << "ORA M"; break;
        case 0b11110110:
            std::cout << "ORI data " << (int)buffer[pc + op_size++]; break;
        case 0b10111110:
            std::cout << "CMP M "; break;
        case 0b11111110:
            std::cout << "CPI data " << (int)buffer[pc + op_size++]; break;


    }
    if (readBitsAt(code, 0,2) == 0b01) {
        unsigned char dest = readDest(code);
        unsigned char source = readSource(code);
        if (source == 0b110) 
            std::cout << "MOV " << getReg(dest);
        else if (dest == 0b110) 
            std::cout << "MOV " << getReg(source);
        else
            std::cout << "MOV " << getReg(dest) << " " << getReg(source);
    }
    
    std::cout << std::endl;
    return op_size;
}

int main() {

    std::vector<unsigned char> buffer;
    loadBytes(buffer, "../rom/invaders.h");

    std::cout << std::hex; //I'm aware this is bad practice
    int pc = 0;
    while (pc < buffer.size()) {
        pc += disassembleOp(buffer, pc);
    }
    std::cout << std::dec;

    return 0;
}
