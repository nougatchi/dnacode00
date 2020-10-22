#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <sstream>
#include <fstream>
#include <time.h>

#include "DNAInterpreter.h"
#include "DNACompiler.h"
#include "version.h"

int main(int argc,char** args)
{
    printf("dnacode00 interpreter/compiler %s\nv%i.%i.%i.%i_%s build %i\n",STATUS,MAJOR,MINOR,BUILD,REVISION,STATUS_SHORT,BUILDS_COUNT);
    printf("built: %s/%s/%s\n",DATE,MONTH,YEAR);
    if(argc == 1) {
        puts("usage:");
        puts("\t-c <file> [output file]: compiles file as DNACode Direct Machine Language and outputs to out.dna or [output file]");
        puts("\t-r <file>              : runs file as DNACode");
        puts("\t-rc <file>             : compiles and runs a file");
        return 0;
    }
    if(argc == 2) {
        puts("specify file");
        return 0;
    }
    if(argc >= 3) {
        if(strcmp(args[1],"-c") == 0) {
            puts("dnacode00 compiler");
            DNACompiler cmp = DNACompiler();
            std::ifstream t(args[2]);
            std::stringstream buffer;
            buffer << t.rdbuf();

            std::string data = cmp.Compile(buffer.str());
            std::string outfile = "out.dna";
            if(argc >= 4) {
                outfile = std::string(args[3]);
            }
            std::ofstream t2(outfile.c_str());
            t2.write(data.c_str(),data.length());

            t2.close();
        } else if(strcmp(args[1],"-r") == 0) {
            puts("dnacode00 interpreter");
            DNAInterpreter dna = DNAInterpreter();
            std::ifstream t(args[2]);
            std::stringstream buffer;
            buffer << t.rdbuf();

            dna.Load(buffer.str());
            time_t start = time(0);
            dna.Parse();
            float seconds_since_start = (float)difftime( time(0), start);
            printf("\n%fs elapsed",seconds_since_start);
        } else if(strcmp(args[1],"-rc") == 0) {
            puts("dnacode00 interpreter/compiler");
            bool silent = false;
            if(argc >= 4) {
                if(strcmp(args[3],"-silent") == 0) {
                    silent = true;
                }
            }
            DNACompiler cmp = DNACompiler(silent);
            DNAInterpreter dna = DNAInterpreter(silent);
            std::ifstream t(args[2]);
            std::stringstream buffer;
            buffer << t.rdbuf();


            std::string data = cmp.Compile(buffer.str());
            dna.Load(data);
            time_t start = time(0);
            dna.Parse();
            float seconds_since_start = (float)difftime( time(0), start);
            printf("\n%fs elapsed",seconds_since_start);
        } else {
            puts("bad argument");

        }
        return 0;
    }
    return 0;
}
