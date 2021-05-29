//
// Created by 86186 on 2021/5/24.
//

#ifndef BEJEWELED00_HOMEPAGE_H
#define BEJEWELED00_HOMEPAGE_H


#include <QFrame>
#include "jewelbutton.h"

class Homepage : public QFrame{
Q_OBJECT
public:
    explicit  Homepage(QWidget*parent=0);
    JewelButton *start_button;
    JewelButton*score_button ;
    JewelButton*join_button;
};


#endif //BEJEWELED00_HOMEPAGE_H
