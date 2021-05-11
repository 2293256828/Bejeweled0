#include <QPainter>
#include <ctime>
#include <cstdlib>
#include "src/headers/ui/jewelbutton.h"
/**
 * ¶¯Ì¬Button
 * @param parent
 */
JewelButton::JewelButton(QWidget *parent) :
	QPushButton(parent)
{
    setMinimumHeight(50);
    resize(600, 50);
    setFont(QFont("Timers", 10, QFont::Bold));
    setStyleSheet("QPushButton:hover{background-color:rgb(255,215,0);border: 1px solid grey;border-radius:8px;}"
                  "QPushButton{background-color:rgb(252,230,202);border: 1px solid grey;border-radius:8px;}");
}

void JewelButton::paintEvent(QPaintEvent* event)
{
	static const QIcon red (QPixmap("D:/Bejeweled00/res/red.png"));
	static const QIcon yellow (QPixmap("D:/Bejeweled00/res/yellow.png"));
	static const QIcon orange (QPixmap("D:/Bejeweled00/res/orange.png"));
	static const QIcon green (QPixmap("D:/Bejeweled00/res/green.png"));
	static const QIcon blue (QPixmap("D:/Bejeweled00/res/blue.png"));
	QPushButton::paintEvent(event);
	// simple implementation by randomizing icon
	srand(time(nullptr));
	switch (rand()%5) {
	case 0:
		setIcon(red);
		break;
	case 1:
		setIcon(yellow);
		break;
	case 2:
		setIcon(orange);
		break;
	case 3:
		setIcon(green);
		break;
	case 4:
		setIcon(blue);
		break;
	}
}
