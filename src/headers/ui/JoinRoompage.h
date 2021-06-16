//
// Created by 86191 on 2021/5/30.
//

#ifndef BEJEWELED00_JOINROOMPAGE_H
#define BEJEWELED00_JOINROOMPAGE_H

#include <QFrame>
#include "JewelLineEdit.h"
#include "jewelbutton.h"

class JoinRoompage : public QFrame{
public:
    JoinRoompage(QWidget *parent=0);
    JewelLineEdit * getLineEdit(){return lineEdit;}
    JewelButton * getButton(){return enter;}
    JewelButton * getReturnButton(){return returnButton;}

private:
    JewelButton *enter,*returnButton;
    JewelLineEdit *lineEdit;
};


#endif //BEJEWELED00_JOINROOMPAGE_H
