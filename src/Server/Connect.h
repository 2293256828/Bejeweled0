//
// Created by 86191 on 2021/6/3.
//

#ifndef BEJEWELED00_CONNECT_H
#define BEJEWELED00_CONNECT_H
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>//引用头文件
#pragma comment(lib,"ws2_32.lib")//引用库文件
#include <iostream>
#include <string>
#include <utility>
#include <vector>

using namespace std;
class Connect {
public:
    const char *ip = "8.140.41.224";
    const int port = 9876;;
    string ID;
    string password;
//    DBConnection dbConnection;
//    QTcpServer tcpServer;
//    QTcpSocket tcpSocket;
public:
    Connect(){

    }
    ~Connect(){

    }
    void setID(string s){ID = std::move(s);}
    void setPassword(string s){password = std::move(s);}

    bool ServerClient(string func);
    bool ServerScore(int score,int mode);
    vector<int> ServerRank();
};
#endif //BEJEWELED00_CONNECT_H
