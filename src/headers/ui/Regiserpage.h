//
// Created by 86191 on 2021/5/29.
//

#ifndef BEJEWELED00_REGISERPAGE_H
#define BEJEWELED00_REGISERPAGE_H

#include <iostream>
#include <string>
#include <QFrame>
#include <QWidget>
#include "JewelLineEdit.h"
#include "jewelbutton.h"
#include "Server/Connect.h"

using namespace  std;
class Registerpage :public QFrame{
public:
    //当前需要登录或者注册的用户名
    string ID;
    //当前需要登录或者注册的用户的密码
    string password;
    JewelButton *registerButton,*loginButton;
    JewelLineEdit *lineEdit,*lineEdit1;
    //用于连接服务器
    Connect *server;

public:
    explicit Registerpage(QWidget *parent = 0);
    //注册时连接服务器
    bool registerDB();
    //登录时连接服务器
    bool loginDB();

    JewelButton* getRegisterButton(){return registerButton;}
    JewelButton* getLoginButton(){return loginButton;}

    string getID(){return ID;}

    void setLineEdit(){
        lineEdit->setText("");
        lineEdit1->setText("");
    }
};




#endif //BEJEWELED00_REGISERPAGE_H
