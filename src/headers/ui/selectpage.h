

#ifndef BEJEWELED00_SELECT_PAGE_H
#define BEJEWELED00_SELECT_PAGE_H


#include <QFrame>
#include <QRadioButton>
#include "jewelbutton.h"

class Selectpage : public QFrame{
Q_OBJECT
public:
    explicit  Selectpage(QWidget*parent=0);
    QButtonGroup *modeGroup;
    QButtonGroup *difficultyGroup;
    QButtonGroup*pairGroup;
    QRadioButton*time_radio;
    QRadioButton*reaction_radio;
    QRadioButton*easy;
    QRadioButton*medium;
    QRadioButton*hard;
    QRadioButton*single;
    QRadioButton*pair;
    JewelButton*goButton;

};


#endif //BEJEWELED00_SelectPAGE_H
