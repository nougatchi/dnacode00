#include "DNACompiler.h"
#include <sstream>

DNACompiler::DNACompiler(bool silent)
{
    x = 0;
    this->silent = silent;
}

int lbln = 0;

//
// Instructions:
// TMP (num)
// MT0
// MT1
// MTA
// SWP
// S01 tmp -> tmp2
// S10 tmp2 -> tmp


std::string DNACompiler::out_ac_for_int(int n) {
    std::string out = "";
    for(int i = 0; i<n; i++) {
        out += "CG";
        x += 2;
    }
    return out;
}

std::string DNACompiler::out_fc_for_int(int n) {
    std::string out = "";
    for(int i = 0; i<n; i++) {
        out += "AT";
        x += 2;
    }
    return out;
}

void rm_nl(std::string &s) {
    for (int p = s.find("\n"); p != (int) std::string::npos; p = s.find("\n"))
    s.erase(p,1);
}

std::string DNACompiler::Compile(std::string in) {
    if(!silent)
        printf("compiling\n");
    rm_nl(in);
    std::istringstream iss(in);
    std::string out = "";
    std::string wrd;
    bool argin = false;
    bool comment = false;
    while(iss >> wrd) {
        if(!silent)
            printf("%p: %s\n",(int*)(long long unsigned int)x,wrd.c_str());
        if(comment) {
            if(wrd == ";") {
                comment = false;
            }
            continue;
        }
        if(argin) {
            if(mm) {
                out += out_ac_for_int(std::stoi(wrd,nullptr,0));
                out += "  ";
                x += 2;
                mm = false;
            }
            argin = false;
        } else {
            if(wrd == "STT") {
                out += out_fc_for_int(1);
                argin = true;
                mm = true;
            } else if(wrd == "SMT") {
                out += out_fc_for_int(2);
                argin = true;
                mm = true;
            } else if(wrd == "STM") {
                out += out_fc_for_int(3);
                argin = true;
                mm = true;
            } else if(wrd == "SM2") {
                out += out_fc_for_int(4);
                out += "  ";
                x += 2;
            } else if(wrd == "S2T") {
                out += out_fc_for_int(5);
                out += "  ";
                x += 2;
            } else if(wrd == "TT2") {
                out += out_fc_for_int(6);
                out += "  ";
                x += 2;
            } else if(wrd == "T2T") {
                out += out_fc_for_int(7);
                out += "  ";
                x += 2;
            } else if(wrd == "ST2") {
                out += out_fc_for_int(8);
                out += "  ";
                x += 2;
            } else if(wrd == "EOP") {
                out += "  ";
                x += 2;
            } else if(wrd == "PST") {
                out += out_fc_for_int(9);
                out += "  ";
                x += 2;
            } else if(wrd == "PSA") {
                out += out_fc_for_int(10);
                argin = true;
                mm = true;
            } else if(wrd == "PPT") {
                out += out_fc_for_int(11);
                out += "  ";
                x += 2;
            } else if(wrd == ";") {
                comment = true;
            } else if(wrd == "ADD") {
                out += out_fc_for_int(12);
                out += "  ";
                x += 2;
            } else if(wrd == "SUB") {
                out += out_fc_for_int(13);
                out += "  ";
                x += 2;
            } else if(wrd == "MUL") {
                out += out_fc_for_int(14);
                out += "  ";
                x += 2;
            } else if(wrd == "DIV") {
                out += out_fc_for_int(15);
                out += "  ";
                x += 2;
            } else if(wrd == "JMP") {
                out += out_fc_for_int(16);
                argin = true;
                mm = true;
            } else if(wrd == "JMT") {
                out += out_fc_for_int(17);
                out += "  ";
                x += 2;
            } else if(wrd == "INC") {
                out += out_fc_for_int(18);
                out += "  ";
                x += 2;
            } else if(wrd == "DEC") {
                out += out_fc_for_int(19);
                out += "  ";
                x += 2;
            } else if(wrd == "CMP") {
                out += out_fc_for_int(22);
                argin = true;
                mm = true;
            } else if(wrd == "JTF") {
                out += out_fc_for_int(20);
                argin = true;
                mm = true;
            } else {
                printf("warning: unknwn \"%s\"\n",wrd.c_str());
                out += out_fc_for_int(std::stoi(wrd,nullptr,0));
                argin = true;
                mm = true;
            }
        }
    }
    return out;
}
