﻿
#include <iostream>
#include "TLSConnect.h"

#pragma comment(lib,"tlsConnect.lib")


int main()
{
    std::cout << "Hello World!\n";
    CTLSConnect* connect = new CTLSConnect(wstring(L"https://bulbapedia.bulbagarden.net/wiki/List_of_Pok%C3%A9mon_by_base_stats_(Generation_VIII-present)"));
    connect->open();
    connect->connect();
    connect->close();

}
