
#ifndef BEJEWELED00_SCOREPAGE_H
#define BEJEWELED00_SCOREPAGE_H
#include <QFrame>
#include <QLabel>
#include "jewelbutton.h"
#include "src/headers/logic/gamestate.h"
class Scorepage : public QFrame{
Q_OBJECT
public:
    explicit Scorepage(QWidget*parent=0,GameState*state=0);
    JewelButton*ret_button;

};


#endif //BEJEWELED00_HOMEPAGE_H
