//
// Created by 86191 on 2021/6/3.
//

#include "Connect.h"

bool Connect::ServerClient(string func)
{
    cout << "ServerInformation:" << ID << endl;
    cout << "ServerInformation:" << password <<endl;
    //��ʼ��DLL
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    //�����׽���
    SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    //���������������
    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));  //ÿ���ֽڶ���0���
    sockAddr.sin_family = PF_INET;
    sockAddr.sin_addr.s_addr = inet_addr(ip);
    sockAddr.sin_port = htons(port);
    connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));

    send(sock, func.c_str(), strlen(func.c_str())+sizeof(char), 0);
    //���շ��������ص�����
    char szBuffer[MAXBYTE] = {0};
    recv(sock, szBuffer, MAXBYTE, 0);

    //������յ�������
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
    //�ر��׽���
    closesocket(sock);
    //��ֹʹ�� DLL
    WSACleanup();
    // system("pause");
    return result;
}

bool Connect::ServerScore(int score,int mode)
{
    cout << "ServerInformation:" << ID << endl;

    //��ʼ��DLL
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    //�����׽���
    SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    //���������������
    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));  //ÿ���ֽڶ���0���
    sockAddr.sin_family = PF_INET;
    sockAddr.sin_addr.s_addr = inet_addr(ip);
    sockAddr.sin_port = htons(port);
    connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));

    string func = "score";
    send(sock, func.c_str(), strlen(func.c_str())+sizeof(char), 0);

    //���շ��������ص�����
    char szBuffer[MAXBYTE] = {0};
    recv(sock, szBuffer, MAXBYTE, 0);
    std::cout << szBuffer << std::endl;

    //��ID����������
    send(sock, ID.c_str(), strlen(ID.c_str())+sizeof(char), 0);

    recv(sock, szBuffer, MAXBYTE, 0);
    std::cout << szBuffer << std::endl;

    //����Ϸģʽ����������
    string s_mode = to_string(mode);
    send(sock, s_mode.c_str(), strlen(s_mode.c_str())+sizeof(char), 0);

    recv(sock, szBuffer, MAXBYTE, 0);
    std::cout << szBuffer << std::endl;

    //����������������
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
    //�ر��׽���
    closesocket(sock);
    //��ֹʹ�� DLL
    WSACleanup();
    // system("pause");
    return result;
}

vector<int> Connect::ServerRank()
{
    //��ʼ��DLL
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    //�����׽���
    SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    //���������������
    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));  //ÿ���ֽڶ���0���
    sockAddr.sin_family = PF_INET;
    sockAddr.sin_addr.s_addr = inet_addr(ip);
    sockAddr.sin_port = htons(port);
    connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));

    string func = "ranking";
    send(sock, func.c_str(), strlen(func.c_str())+sizeof(char), 0);

    //���շ��������ص�����
    char szBuffer[MAXBYTE] = {0};
    recv(sock, szBuffer, MAXBYTE, 0);
    int n = stoi(szBuffer);
    std::cout << "Rank:number:"<< szBuffer << std::endl;

    send(sock, szBuffer, strlen(szBuffer)+sizeof(char), 0);
    vector<int> result;
    for (int i = 0;i < n;i++){
        for (int j = 1;j < 3;j++){
            //���շ�����һ��Time��һ��Fast,���ν��գ�
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
    //�ر��׽���
    closesocket(sock);
    //��ֹʹ�� DLL
    WSACleanup();
    return result;
}
