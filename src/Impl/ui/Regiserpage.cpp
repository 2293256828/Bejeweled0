//
// Created by 86191 on 2021/5/29.
//

#include "src/headers/ui/Regiserpage.h"

Registerpage::Registerpage(QWidget *parent) :
        QFrame(parent){
    registerButton = new JewelButton(this);
    registerButton->setText("register");
    registerButton->move(550,720);
    registerButton->setFixedSize(200,50);
    loginButton = new JewelButton(this);
    loginButton->setText("login");
    loginButton->move(800,720);
    loginButton->setFixedSize(200,50);

    lineEdit = new JewelLineEdit(this);
    lineEdit1 = new JewelLineEdit(this);
    lineEdit->move(550,580);
    lineEdit->setFixedSize(500,50);
    lineEdit->setPlaceholderText("User name (10 digits)");
    //���������С����ɫ
    lineEdit->setStyleSheet("font-size:20px; color:rgb(100,100,100);");
    //����Բ�Ǳ߿�͸������
    lineEdit->setStyleSheet("border:2px groove gray;border-radius:10px;padding:2px 4px;background-color:rgb(255,0,0,0);");
    lineEdit1->move(550,650);
    lineEdit1->setFixedSize(500,50);
    lineEdit1->setPlaceholderText("Password (6 digits)");
    lineEdit1->setStyleSheet("font-size:20px; color:rgb(100,100,100);");
    lineEdit1->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    lineEdit1->setStyleSheet("border:2px groove gray;border-radius:10px;padding:2px 4px;background-color:rgb(255,0,0,0);");
    //�������ñ߿�Ϊ�հ�
    //lineEdit->setStyleSheet("background:transparent;border-width:0;border-style:outset");
    server=new Connect();
}

/**
 * ���ʷ�������������ݿ��ѯ���
 * @param s1 �û���
 * @param s2 ����
 * @return
 */
bool Registerpage::loginDB()
{
    //����û���������
    QString qString1 = lineEdit->text();
    QString qString2 = lineEdit1->text();
    ID = qString1.toStdString();
    password = qString2.toStdString();
    cout << "Register:" << ID << endl;
    cout << "Register:" << password << endl;

    //����server�е�ID��password����������������͵�ǰ��¼��ע���ID��password
    server->setID(ID);
    server->setPassword(password);
    //����ServerClient�����߷�������ǰ�ǵ�¼����������ID��password���͸�������
    bool result = server->ServerClient("login");
    return result;
}

/**
 * ���ʷ��������������ݿ��¼
 * @param s1 �û���
 * @param s2 ����
 * @return �Ƿ�ע��ɹ�
 */
bool Registerpage::registerDB()
{
    //����û���������
    QString qString1 = lineEdit->text();
    QString qString2 = lineEdit1->text();
    ID = qString1.toStdString();
    password = qString2.toStdString();

    //����server�е�ID��password����������������͵�ǰ��¼��ע���ID��password
    server->setID(ID);
    server->setPassword(password);

    //����ServerClient�����߷�������ǰ��ע�����������ID��password���͸�������
    bool result = server->ServerClient("register");
    return result;
}