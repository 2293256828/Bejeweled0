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
    //��ǰ��Ҫ��¼����ע����û���
    string ID;
    //��ǰ��Ҫ��¼����ע����û�������
    string password;
    JewelButton *registerButton,*loginButton;
    JewelLineEdit *lineEdit,*lineEdit1;
    //�������ӷ�����
    Connect *server;

public:
    explicit Registerpage(QWidget *parent = 0);
    //ע��ʱ���ӷ�����
    bool registerDB();
    //��¼ʱ���ӷ�����
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
