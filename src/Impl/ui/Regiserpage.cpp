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
    //设置字体大小，颜色
    lineEdit->setStyleSheet("font-size:20px; color:rgb(100,100,100);");
    //设置圆角边框，透明背景
    lineEdit->setStyleSheet("border:2px groove gray;border-radius:10px;padding:2px 4px;background-color:rgb(255,0,0,0);");
    lineEdit1->move(550,650);
    lineEdit1->setFixedSize(500,50);
    lineEdit1->setPlaceholderText("Password (6 digits)");
    lineEdit1->setStyleSheet("font-size:20px; color:rgb(100,100,100);");
    lineEdit1->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    lineEdit1->setStyleSheet("border:2px groove gray;border-radius:10px;padding:2px 4px;background-color:rgb(255,0,0,0);");
    //可以设置边框为空白
    //lineEdit->setStyleSheet("background:transparent;border-width:0;border-style:outset");
    server=new Connect();
}

/**
 * 访问服务器，获得数据库查询结果
 * @param s1 用户名
 * @param s2 密码
 * @return
 */
bool Registerpage::loginDB()
{
    //获得用户名及密码
    QString qString1 = lineEdit->text();
    QString qString2 = lineEdit1->text();
    ID = qString1.toStdString();
    password = qString2.toStdString();
    cout << "Register:" << ID << endl;
    cout << "Register:" << password << endl;

    //设置server中的ID、password，用于向服务器发送当前登录或注册的ID和password
    server->setID(ID);
    server->setPassword(password);
    //调用ServerClient，告诉服务器当前是登录操作，并将ID和password发送给服务器
    bool result = server->ServerClient("login");
    return result;
}

/**
 * 访问服务器，增加数据库记录
 * @param s1 用户名
 * @param s2 密码
 * @return 是否注册成功
 */
bool Registerpage::registerDB()
{
    //获得用户名及密码
    QString qString1 = lineEdit->text();
    QString qString2 = lineEdit1->text();
    ID = qString1.toStdString();
    password = qString2.toStdString();

    //设置server中的ID、password，用于向服务器发送当前登录或注册的ID和password
    server->setID(ID);
    server->setPassword(password);

    //调用ServerClient，告诉服务器当前是注册操作，并将ID和password发送给服务器
    bool result = server->ServerClient("register");
    return result;
}