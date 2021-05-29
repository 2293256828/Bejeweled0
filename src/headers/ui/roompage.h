
#ifndef BEJEWELED00_ROOMPAGE_H
#define BEJEWELED00_ROOMPAGE_H
#include <QFrame>
#include <QLabel>
#include "jewelbutton.h"

class Roompage : public QFrame{
Q_OBJECT
public:
    explicit Roompage(QWidget*parent=0,int port=0);
    JewelButton*double_go_button;
    QLabel*label;
};


#endif //BEJEWELED00_HOMEPAGE_H
