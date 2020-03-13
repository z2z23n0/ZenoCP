#pragma once

#include <iostream>
#include <string>

using namespace std;

inline void LOG(string str)
{
    cout << __FILE__ << ":" << __LINE__ << " " <<
         __TIMESTAMP__ << ":" << str << endl;
}
