
#include <iostream>
#include "TLSConnect.h"

#pragma comment(lib,"tlsConnect.lib")


int main()
{
    CTLSConnect* connect = new CTLSConnect(std::wstring(L"https://bulbapedia.bulbagarden.net/wiki/List_of_Pok%C3%A9mon_by_base_stats_(Generation_VIII-present)"));
    connect->open();
    connect->connect();
    connect->close();

}
