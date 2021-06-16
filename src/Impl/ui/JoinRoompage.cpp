//
// Created by 86191 on 2021/5/30.
//

#include "src/headers/ui/JoinRoompage.h"
JoinRoompage::JoinRoompage(QWidget *parent):
             QFrame(parent){
    enter = new JewelButton(this);
    lineEdit = new JewelLineEdit(this);
    returnButton = new JewelButton(this);
    enter->move(700,550);
    enter->setFixedSize(150,50);
    enter->setText("OK");
    returnButton->move(700,620);
    returnButton->setFixedSize(150,50);
    returnButton->setText("Exit");
    lineEdit->move(750,450);
    lineEdit->setPlaceholderText("Input Port Number");
    lineEdit->setStyleSheet("font-size:20px; color:rgb(100,100,100);");
    lineEdit->setFont(QFont("Microsoft YaHei", 10, 100));
    lineEdit->setStyleSheet("border:2px groove gray;border-radius:10px;padding:2px 4px;background-color:rgb(255,0,0,0);");
    lineEdit->setFixedSize(200,50);

}