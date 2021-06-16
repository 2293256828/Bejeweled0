//
// Created by 86191 on 2021/6/3.
//

#include "Connect.h"

bool Connect::ServerClient(string func)
{
    cout << "ServerInformation:" << ID << endl;
    cout << "ServerInformation:" << password <<endl;
    //初始化DLL
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    //创建套接字
    SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    //向服务器发起请求
    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
    sockAddr.sin_family = PF_INET;
    sockAddr.sin_addr.s_addr = inet_addr(ip);
    sockAddr.sin_port = htons(port);
    connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));

    send(sock, func.c_str(), strlen(func.c_str())+sizeof(char), 0);
    //接收服务器传回的数据
    char szBuffer[MAXBYTE] = {0};
    recv(sock, szBuffer, MAXBYTE, 0);

    //输出接收到的数据
    std::cout << szBuffer << std::endl;
    send(sock, ID.c_str(), strlen(ID.c_str())+sizeof(char), 0);

    recv(sock, szBuffer, MAXBYTE, 0);
    std::cout << szBuffer << std::endl;

    send(sock, password.c_str(), strlen(password.c_str())+sizeof(char), 0);

    recv(sock, szBuffer, MAXBYTE, 0);

    std::cout << szBuffer << std::endl;
    string s = szBuffer;
    bool result = false;
    if (s == "yes"){
        result = true;
    }else{
        if (s == "no"){
            result = false;
        }
    }
    cout << "!@#$%^" <<endl;
    //关闭套接字
    closesocket(sock);
    //终止使用 DLL
    WSACleanup();
    // system("pause");
    return result;
}

bool Connect::ServerScore(int score,int mode)
{
    cout << "ServerInformation:" << ID << endl;

    //初始化DLL
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    //创建套接字
    SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    //向服务器发起请求
    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
    sockAddr.sin_family = PF_INET;
    sockAddr.sin_addr.s_addr = inet_addr(ip);
    sockAddr.sin_port = htons(port);
    connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));

    string func = "score";
    send(sock, func.c_str(), strlen(func.c_str())+sizeof(char), 0);

    //接收服务器传回的数据
    char szBuffer[MAXBYTE] = {0};
    recv(sock, szBuffer, MAXBYTE, 0);
    std::cout << szBuffer << std::endl;

    //将ID发到服务器
    send(sock, ID.c_str(), strlen(ID.c_str())+sizeof(char), 0);

    recv(sock, szBuffer, MAXBYTE, 0);
    std::cout << szBuffer << std::endl;

    //将游戏模式发给服务器
    string s_mode = to_string(mode);
    send(sock, s_mode.c_str(), strlen(s_mode.c_str())+sizeof(char), 0);

    recv(sock, szBuffer, MAXBYTE, 0);
    std::cout << szBuffer << std::endl;

    //将分数发到服务器
    string s_score = to_string(score);
    send(sock, s_score.c_str(), strlen(s_score.c_str())+sizeof(char), 0);

    recv(sock, szBuffer, MAXBYTE, 0);
    std::cout << szBuffer << std::endl;
    string s = szBuffer;
    bool result = false;
    if (s == "yes"){
        result = true;
    }else{
        if (s == "no"){
            result = false;
        }
    }
    cout << "!@#$%^" <<endl;
    //关闭套接字
    closesocket(sock);
    //终止使用 DLL
    WSACleanup();
    // system("pause");
    return result;
}

vector<int> Connect::ServerRank()
{
    //初始化DLL
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    //创建套接字
    SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    //向服务器发起请求
    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
    sockAddr.sin_family = PF_INET;
    sockAddr.sin_addr.s_addr = inet_addr(ip);
    sockAddr.sin_port = htons(port);
    connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));

    string func = "ranking";
    send(sock, func.c_str(), strlen(func.c_str())+sizeof(char), 0);

    //接收服务器传回的数据
    char szBuffer[MAXBYTE] = {0};
    recv(sock, szBuffer, MAXBYTE, 0);
    int n = stoi(szBuffer);
    std::cout << "Rank:number:"<< szBuffer << std::endl;

    send(sock, szBuffer, strlen(szBuffer)+sizeof(char), 0);
    vector<int> result;
    for (int i = 0;i < n;i++){
        for (int j = 1;j < 3;j++){
            //接收分数（一个Time，一个Fast,依次接收）
            char szBuffer1[MAXBYTE] = {0};
            recv(sock, szBuffer1, MAXBYTE, 0);
            cout << i << "+" << j << ":" << szBuffer1 << endl;
            result.push_back(stoi(szBuffer1));

            string s = "0";
            send(sock, s.c_str(), strlen(s.c_str())+sizeof(char), 0);
        }
    }
    if (n < 8){
        for (int i = n;i < 8;i++){
            result.push_back(0);
            result.push_back(0);
        }
    }
    cout << "!@#$%^" <<endl;
    //关闭套接字
    closesocket(sock);
    //终止使用 DLL
    WSACleanup();
    return result;
}
