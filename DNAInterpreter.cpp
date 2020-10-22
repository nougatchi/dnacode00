#include "DNAInterpreter.h"

DNAInterpreter::DNAInterpreter(bool silent)
{
    _values = (int*)malloc(255);
    _stack = std::stack<int>();
    lerror = "unknown";
    code = "  ";
    this->silent = silent;
}

bool DNAInterpreter::ParsePair(std::string strpair) {
    if(strpair.length() != 2) {
            lerror = "not a pair";
            return false;
    }
    int x;
    switch(strpair[0]) {
    case 'A': // FC
        if(strpair[1] != 'T') {
            lerror = "A expected T";
            return false;
        }
        fc++;
        break;
    case 'C': // AC
        if(strpair[1] != 'G') {
            lerror = "C expected G";
            return false;
        }
        ac++;
        break;
    case 'T':
        if(strpair[1] != 'A') {
            lerror = "T expected A";
            return false;
        }
        fc++;
        break;
    case 'G':
        if(strpair[1] != 'C') {
            lerror = "G expected C";
            return false;
        }
        fc++;
        break;
    case ' ': // EOI
        switch(fc) {
        case 0: // EOP
            lerror = "success";
            return false;
            break;
        case 1: // STT
            if(!silent)
                printf("tmp = ac(%i)\n",ac);
            tmp = ac;
            break;
        case 2: // SMT
            _values[ac] = tmp;
            if(!silent)
                printf("memory[ac(%i)] = tmp(%i)\n",ac,tmp);
            break;
        case 3: // STM
            tmp = _values[ac];
            if(!silent)
                printf("tmp = memory[ac(%i)]\n",ac);
            break;
        case 4: // SM2
            tmp = _values[tmp2];
            if(!silent)
                printf("tmp = memory[tmp2(%i)]\n",tmp2);
            break;
        case 5: // S2T
            _values[tmp2] = tmp;
            if(!silent)
                printf("memory[tmp2(%i)] = tmp(%i)\n",tmp2,ac);
            break;
        case 6: // TT2
            tmp = tmp2;
            if(!silent)
                printf("tmp = tmp2\n");
            break;
        case 7: // T2T
            tmp2 = tmp;
            if(!silent)
                printf("tmp2 = tmp\n");
            break;
        case 8: // ST2
            x = tmp2;
            tmp2 = tmp;
            tmp = x;
            if(!silent)
                printf("x = tmp2\ntmp2 = tmp\ntmp = x\n");
            break;
        case 9: // PST
            _stack.push(tmp);
            if(!silent)
                printf("push tmp(%i)\n",tmp);
            break;
        case 10: // PSA
            _stack.push(ac);
            if(!silent)
                printf("push ac(%i)\n",ac);
            break;
        case 11: // PPT
            tmp = _stack.top();
            _stack.pop();
            if(!silent)
                printf("pop(%i) tmp\n",tmp);
            break;
        case 12: // ADD
            x = tmp;
            tmp = tmp + tmp2;
            if(!silent)
                printf("tmp(%i) = tmp(%i) + tmp2(%i)\n",tmp,x,tmp2);
            break;
        case 13: // SUB
            x = tmp;
            tmp = tmp - tmp2;
            if(!silent)
                printf("tmp(%i) = tmp(%i) - tmp2(%i)\n",tmp,x,tmp2);
            break;
        case 14: // MUL
            x = tmp;
            tmp = tmp * tmp2;
            if(!silent)
                printf("tmp(%i) = tmp(%i) * tmp2(%i)\n",tmp,x,tmp2);
            break;
        case 15: // DIV
            x = tmp;
            tmp = tmp / tmp2;
            if(!silent)
                printf("tmp(%i) = tmp(%i) / tmp2(%i)\n",tmp,x,tmp2);
            break;
        case 16: // JMP
            posup = ac;
            if(!silent)
                printf("i = ac(%i)\n",ac);
            break;
        case 17: // JMT
            posup = tmp;
            if(!silent)
                printf("i = tmp(%i)\n",tmp);
            break;
        case 18: // INC
            tmp++;
            if(!silent)
                printf("incrementing tmp\n");
            break;
        case 19: // DEC
            tmp--;
            if(!silent)
                printf("decrementing tmp\n");
            break;
        case 20: // JTF
            switch(ac) {
            case 0: // JE
                if(cmp & CMP_EQL) {
                    posup = tmp;
                    if(!silent)
                        printf("yes");
                } else {
                    if(!silent)
                        printf("no");
                }
                break;
            case 1: // JNE
                if(cmp & CMP_NQL) {
                    posup = tmp;
                    if(!silent)
                        printf("yes");
                } else {
                    if(!silent)
                        printf("no");
                }
                break;
            case 2: // JNZ
                if(cmp & CMP_NZR) {
                    posup = tmp;
                    if(!silent)
                        printf("yes");
                } else {
                    if(!silent)
                        printf("no");
                }
                break;
            case 3: // JZ
                if(cmp & CMP_ZER) {
                    posup = tmp;
                    if(!silent)
                        printf("yes");
                } else {
                    if(!silent)
                        printf("no");
                }
                break;
            }
            if(!silent)
                printf(" <-- i = tmp(%i) if CF(%i) is flagged with proper on m(%i)\n",tmp,cmp,ac);
            break;
        case 21: // CT2
            cmp = 0;
            if(tmp == tmp2) {
                cmp = cmp | CMP_EQL;
                if(!silent)
                    printf("EQL ");
            } else {
                cmp = cmp | CMP_NQL;
                if(!silent)
                    printf("NQL ");
            }
            if(tmp < tmp2) {
                cmp = cmp | CMP_LTH;
                if(!silent)
                    printf("LTH ");
            }
            if(tmp > tmp2) {
                cmp = cmp | CMP_GTH;
                if(!silent)
                    printf("GTH ");
            }
            if(tmp == 0) {
                cmp = cmp | CMP_ZER;
                if(!silent)
                    printf("ZER ");
            } else {
                cmp = cmp | CMP_NZR;
                if(!silent)
                    printf("NZR ");
            }
            if(!silent)
                printf("<-- comp(%i) tmp(%i) tmp2(%i)\n",cmp,tmp,tmp2);
            break;
        case 22: // CMP
            cmp = 0;
            if(tmp == ac) {
                cmp = cmp | CMP_EQL;
                if(!silent)
                    printf("EQL ");
            } else {
                cmp = cmp | CMP_NQL;
                if(!silent)
                    printf("NQL ");
            }
            if(tmp < ac) {
                cmp = cmp | CMP_LTH;
                if(!silent)
                    printf("LTH ");
            }
            if(tmp > ac) {
                cmp = cmp | CMP_GTH;
                if(!silent)
                    printf("GTH ");
            }
            if(tmp == 0) {
                cmp = cmp | CMP_ZER;
                if(!silent)
                printf("ZER ");
            } else {
                cmp = cmp | CMP_NZR;
                if(!silent)
                    printf("NZR ");
            }
            if(!silent)
                printf(" <-- comp(%i) tmp(%i) ac(%i)\n",cmp,tmp,ac);
            break;
        default:
            lerror = "unknown fc";
            return false;
        }
        fc = 0;
        ac = 0;
        break;
    default:
        lerror = "unknown pair";
        return false;
    }
    return true;
}

void DNAInterpreter::Load(std::string str) {
    code = str;
    posup = 0;
}

void DNAInterpreter::Parse() {
    for(long long unsigned int i = 0; i<code.length(); i+=2) {
        if(!silent)
            printf("\n%llu\t(%llu)\t:",i,posup);
        if(i%2!=0) {
            continue;
        }
        if(posup != i) {
            if(!silent)
                puts("jumping");
            i = posup;
            posup = i+2;
        } else {
            posup = i+2;
        }
        std::string cpair = code.substr(i,2);
        if(!silent)
            printf("p(%s)",cpair.c_str());
        if(!ParsePair(cpair)) {
            puts(lerror.c_str());
            puts(code.substr(i,2).c_str());
            break;
        }
    }
    printf("Memory: %s\n",(char*)_values);
    printf("TMP0: %i\n",tmp);
    printf("TMP1: %i\n",tmp2);
}
