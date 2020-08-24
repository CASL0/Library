
#include <iostream>
#include "tlsConnect.h"

#pragma comment(lib,"tlsConnect.lib")


int main()
{
    std::cout << "Hello World!\n";
    CTLSConnect* connect = new CTLSConnect(wstring(L"http://www.microsoft.com/ja-jp"));
    connect->open();
    connect->connect();
    connect->close();

}
