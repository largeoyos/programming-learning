#include "library/KMPString.h"
#include <iostream>

int main() {
    SString S, T;
    StrAssign(S, "ababcabcacbab");
    StrAssign(T, "abcac");

    std::cout << "Main String: ";
    PrintString(S);
    std::cout << "Pattern String: ";
    PrintString(T);

    int next[MAXLEN + 1];
    GetNext(T, next);

    std::cout << "Next array: ";
    for (int i = 1; i <= T.length; i++) {
        std::cout << next[i] << " ";
    }
    std::cout << std::endl;

    int pos = Index_KMP(S, T, 1, next);
    if (pos != 0) {
        std::cout << "Pattern found at position: " << pos << std::endl;
    } else {
        std::cout << "Pattern not found." << std::endl;
    }

    // 测试 KMP 优化版本 nextval
    int nextval[MAXLEN + 1];
    GetNextVal(T, nextval);
    std::cout << "NextVal array: ";
    for (int i = 1; i <= T.length; i++) {
        std::cout << nextval[i] << " ";
    }
    std::cout << std::endl;

    pos = Index_KMP(S, T, 1, nextval);
    if (pos != 0) {
        std::cout << "Pattern found at position (using nextval): " << pos << std::endl;
    }

    return 0;
}
