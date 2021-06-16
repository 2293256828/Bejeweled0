//
// Created by 86191 on 2021/5/29.
//

#include "src/headers/ui/JewelLineEdit.h"


JewelLineEdit::JewelLineEdit(QWidget *parent):
        QLineEdit(parent)
{
    setMinimumHeight(50);
    resize(600, 50);
    setFont(QFont("Timers", 10, QFont::Bold));
    //setStyleSheet("border:2px groove gray;border-radius:10px;padding:2px 4px");
}