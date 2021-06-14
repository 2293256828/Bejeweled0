#include <QPainter>
#include <ctime>
#include <cstdlib>
#include "QEvent"
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
bool JewelButton::event(QEvent *e) {
    if(!isEnabled())return QPushButton::event(e);
    static QSound soundHover("D:\\Bejeweled00\\res\\sound_effect\\button_mouseover.wav");
    static QSound soundLeave("D:\\Bejeweled00\\res\\sound_effect\\button_mouseleave.wav");
    static QSound soundPress("D:\\Bejeweled00\\res\\sound_effect\\button_press.wav");
    static QSound soundRelease("D:\\Bejeweled00\\res\\sound_effect\\button_release.wav");
    switch(e->type()){
        case QEvent::Enter:
                soundHover.play();
            break;
        case QEvent::Leave:
                soundLeave.play();
            break;
        case QEvent::MouseButtonPress:
                soundPress.play();
            break;
        case QEvent::MouseButtonRelease:
                soundRelease.play();
            break;
        default:
            break;
    }
    return QPushButton::event(e);
}
void JewelButton::paintEvent(QPaintEvent* event)
{
	static const QIcon red (QPixmap("D:/Bejeweled00/res/pictures/red.png"));
	static const QIcon yellow (QPixmap("D:/Bejeweled00/res/pictures/yellow.png"));
	static const QIcon orange (QPixmap("D:/Bejeweled00/res/pictures/orange.png"));
	static const QIcon green (QPixmap("D:/Bejeweled00/res/pictures/green.png"));
	static const QIcon blue (QPixmap("D:/Bejeweled00/res/pictures/blue.png"));
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
