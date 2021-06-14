
#include <headers/ui/jewelbutton.h>
#include <headers/ui/homepage.h>
Homepage::Homepage(QWidget *parent) :
        QFrame(parent) {
    QPalette palette;  //创建一个调色板的对象
    QPixmap pixmap;
    pixmap.load("D:/Bejeweled00/res/pictures/bg.png");
    palette.setBrush(backgroundRole(), QBrush(pixmap));
    setPalette(palette);
    start_button = new JewelButton(this);
    score_button = new JewelButton(this);
    join_button = new JewelButton(this);
    setting_button=new JewelButton(this);
    about_button=new JewelButton(this);
    start_button->setText(tr("START GAME"));
    score_button->setText(tr("SCORE BOARD"));
    join_button->setText(tr("JOIN ROOM"));
    setting_button->setText(tr("SETTINGS"));
    about_button->setText(tr("ABOUT AND INSTRUCTIONS"));
    start_button->move(500, 570);
    score_button->move(500, 690);
    join_button->move(500,630);
    setting_button->move(500,750);
    about_button->move(500,810);
}

