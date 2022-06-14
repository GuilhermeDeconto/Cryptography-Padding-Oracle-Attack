//
// Created by Guilherme Deconto on 5/31/22.
//

#include "network.h"

bool network::queryUrl(const string& cipheredText) {
    const static string URL = "https://crypto-class.appspot.com/po?er=";

    //Make http request
    cpr::Response r = cpr::Get(cpr::Url{URL + cipheredText});
//    cout << URL + cipheredText << endl;
//    cout << r.elapsed << endl;

    //Check if padding error
    if (r.status_code == 404 || r.status_code == 200) {
        return true;
    } else {
        return false;
    }
}
