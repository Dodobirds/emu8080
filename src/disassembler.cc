#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#define I_SIZE 8

void loadBytes(std::vector<unsigned char>& bytes, std::string filepath) {

    std::ifstream file;
    file.open(filepath, std::ios::binary);

    if (file.fail()) {
        std::cout << "Failed to read from file" << std::endl; //I should learn exceptions :(
        return;
    }

    unsigned char byte;
    while (file >> byte) {
        bytes.push_back(byte);
    }

    return;
}


/* I should have just listed out the different combinations
 * On the brightside i reviewed my bitwise operators
 * */
int readBitsAt(unsigned char code, int start, int end) {
   code = code >> (I_SIZE - end);
   return code & ~( 0xFF << (end - start));
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

std::string getCond(int a) {
    switch(a) {
        case 0b000:
            return "cond_NZ";
        case 0b001:
            return "cond_Z";
        case 0b010:
            return "cond_NC";
        case 0b011:
            return "cond_C";
        case 0b100:
            return "cond_PO";
        case 0b101:
            return "cond_PE";
        case 0b110:
            return "cond_P";
        case 0b111:
            return "code_M";
        default:
            std::cout << "Failed to find condition" << std::endl;
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
        case 0b00000111:
            std::cout << "RLC "; break;
        case 0b00001111:
            std::cout << "RRC "; break;
        case 0b00010111:
            std::cout << "RAL "; break;
        case 0b00011111:
            std::cout << "RAR "; break;
        case 0b00101111:
            std::cout << "CMA "; break;
        case 0b00111111:
            std::cout << "CMC "; break;
        case 0b00110111:
            std::cout << "STC "; break;
        case 0b11000011:
            std::cout << "JMP addr " << (int)buffer[pc + op_size++] << (int)buffer[pc + op_size++]; break;
        case 0b11001101:
            std::cout << "CALL addr " << (int)buffer[pc + op_size++] << (int)buffer[pc + op_size++]; break;
        case 0b11001001:
            std::cout << "RET "; break;
        case 0b11101001:
            std::cout << "PCHL "; break;
        case 0b11110101:
            std::cout << "PUSH PSW "; break;
        case 0b11110001:
            std::cout << "POP PSW "; break;
        case 0b11101011:
            std::cout << "XCHG "; break;
        case 0b11100011:
            std::cout << "XTHL "; break;
        case 0b11111001:
            std::cout << "SPHL "; break;
        case 0b11011011:
            std::cout << "IN port " << (int)buffer[pc + op_size++]; break;
        case 0b11010011:
            std::cout << "OUT port " << (int)buffer[pc + op_size++]; break;
        case 0b11111011:
            std::cout << "EI "; break;
        case 0b11110011:
            std::cout << "DI "; break;
        case 0b01110110:
            std::cout << "HLT "; break;
        case 0x00:
            std::cout << "NOP "; break;
        default:



            if (readBitsAt(code, 0,2) == 0b01) {
                unsigned char dest = readDest(code);
                unsigned char source = readSource(code);
                if (source == 0b110) 
                    std::cout << "MOV r M " << getReg(dest);
                else if (dest == 0b110) 
                    std::cout << "MOV M r " << getReg(source);
                else
                    std::cout << "MOV r1 r2 " << getReg(dest) << " " << getReg(source);
            } else if (readBitsAt(code, 0, 2) == 0b00 ) {
                if (readSource(code) == 0b110) 
                    std::cout << "MVI r,data " << getReg(readDest(code)) << " " << (int)buffer[pc + op_size++];
                else if (readBitsAt(code, 4, 8) == 0b0001)
                    std::cout << "LXI rp,data " << getRegPair(readRP(code)) << " " << (int)buffer[pc + op_size++] << (int)buffer[pc + op_size++];
                else if (readBitsAt(code, 4, 8) == 0b1010)
                    std::cout << "LDAX rp " << getRegPair(readRP(code));
                else if (readBitsAt(code, 4, 8) == 0b0010)
                    std::cout << "STAX rp " << getRegPair(readRP(code));
                else if (readSource(code) == 0b100) 
                    std::cout << "INR r " << getReg(readDest(code));
                else if (readSource(code) == 0b101)
                    std::cout << "DCR r " << getReg(readDest(code));
                else if (readBitsAt(code, 4, 8) == 0b0011)
                    std::cout << "INX rp " << getRegPair(readRP(code));
                else if (readBitsAt(code, 4, 8) == 0b1011)
                    std::cout << "DCX rp " << getRegPair(readRP(code));
                else if (readBitsAt(code, 4, 8) == 0b1001)
                    std::cout << "DAD rp " << getRegPair(readRP(code));
            } else if (readBitsAt(code,0,2) == 0b11) {
                if (readSource(code) == 0b010)
                    std::cout << "Jcondition addr " << getCond(readDest(code)) << " " << (int)buffer[pc + op_size++] << (int)buffer[pc + op_size++];
                else if (readSource(code) == 0b100)
                    std::cout << "Ccondition addr " << getCond(readDest(code)) << " " << (int)buffer[pc + op_size++] << (int)buffer[pc + op_size++];
                else if (readSource(code) == 0b000)
                    std::cout << "Rcondition addr " << getCond(readDest(code));
                else if (readSource(code) == 0b111)
                    std::cout << "RST addr " << (readDest(code) << 3);
                else if (readBitsAt(code,4,8) == 0b0101)
                    std::cout << "PUSH rp " << getRegPair(readRP(code));
                else if (readBitsAt(code,4,8) == 0b0001)
                    std::cout << "POP rp " << getRegPair(readRP(code));
            }
            else if (readBitsAt(code,0,5) == 0b10000)
                std::cout << "ADD r " << getReg(readSource(code));
            else if (readBitsAt(code,0,5) == 0b10001) 
                std::cout << "ADC r " << getReg(readSource(code));
            else if (readBitsAt(code,0,5) == 0b10010)
                std::cout << "SUB r " << getReg(readSource(code));
            else if (readBitsAt(code,0,5) == 0b10011)
                std::cout << "SBB r " << getReg(readSource(code));
            else if (readBitsAt(code,0,5) == 0b10100)
                std::cout << "ANA r " << getReg(readSource(code));
            else if (readBitsAt(code,0,5) == 0b10101)
                std::cout << "XRA r " << getReg(readSource(code));
            else if (readBitsAt(code,0,5) == 0b10110)
                std::cout << "ORA r " << getReg(readSource(code));
            else if (readBitsAt(code,0,5) == 0b10111)
                std::cout << "CMP r " << getReg(readSource(code));
    }

    /* std::cout << "\t " << (int)code; */
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
