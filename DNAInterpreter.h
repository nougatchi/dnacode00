#ifndef DNAINTERPRETER_H
#define DNAINTERPRETER_H

#include <stack>
#include <string>

#define CMP_EQL 0b00000001
#define CMP_NQL 0b00000010
#define CMP_GTH 0b00000100
#define CMP_LTH 0b00001000
#define CMP_ZER 0b00010000
#define CMP_NZR 0b00100000

class DNAInterpreter
{
    public:
        DNAInterpreter(bool silent = false);
        void Parse();
        void Load();
        void Load(std::string);
        std::string GetError();
    protected:
        std::stack<int> _stack;
        std::string code;
        bool ParsePair(std::string strpair);
        std::string lerror;
        int fc = 0;
        int ac = 0;
        int tmp = 0;
        int tmp2 = 0;
        int cmp = 0;
        int *_values;
        long long unsigned int posup = 0;
        bool silent;
    private:
};

#endif // DNAINTERPRETER_H
