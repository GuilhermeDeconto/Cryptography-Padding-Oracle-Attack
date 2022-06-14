//
// Created by Guilherme Deconto on 5/30/22.
//

#ifndef ORACLE_PADDING_ATTACK_MAIN_H
#define ORACLE_PADDING_ATTACK_MAIN_H

#include <iostream>
#include <vector>
#include <chrono>
#include <string>

using namespace std;

class main {

public:
    static string byteToHexStr(vector<char> const & vector);
    static vector<char> hexToBytes(const string& hex);
    static vector<int> getPossibleValues();
    static vector<char> decryptBlock(const vector<char>& previousBlock, const vector<char>& blockToDecrypt, int blockSize);
    static void printProgress(int p, int total, int width = 80);
    static void vectorIntoChunks(const vector<char>& cipheredText, vector<vector<char>>& cipherBlocks);

};

#endif //ORACLE_PADDING_ATTACK_MAIN_H
