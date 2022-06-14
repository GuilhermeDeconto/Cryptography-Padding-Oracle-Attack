//
// Created by Guilherme Deconto on 5/31/22.
//

#ifndef ORACLE_PADDING_ATTACK_NETWORK_H
#define ORACLE_PADDING_ATTACK_NETWORK_H

#include <iostream>
#include <cpr/cpr.h>
#include <string>

using namespace std;

class network {

public:
    static bool queryUrl(const string& cipheredText);

};

#endif //ORACLE_PADDING_ATTACK_NETWORK_H
