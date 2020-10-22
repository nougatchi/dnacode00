#ifndef DNACOMPILER_H
#define DNACOMPILER_H
#include <string>
#include <array>

class DNACompiler
{
    public:
        DNACompiler(bool silent = false);
        std::string Compile(std::string in);
    protected:
        bool mm;
        int x = 0;
        bool silent;
    private:
        std::string out_fc_for_int(int n);
        std::string out_ac_for_int(int n);
};

#endif // DNACOMPILER_H
