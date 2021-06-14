//
// Created by 86186 on 2021/5/24.
//

#include <headers/ui/jewelbutton.h>
#include <headers/ui/selectpage.h>
#include <QRadioButton>
#include <QButtonGroup>

Selectpage::Selectpage(QWidget *parent) :
        QFrame(parent) {
    QPalette palette;  //创建一个调色板的对象
    QPixmap pixmap;
    pixmap.load("D:/Bejeweled00/res/pictures/img.png");
    palette.setBrush(backgroundRole(), QBrush(pixmap));
    setPalette(palette);
    this->update();
    goButton = new JewelButton(this);
    goButton->setText(tr("GO"));
    goButton->move(500, 600);
    QFont yahei("Microsoft YaHei", 15, 150);
    modeGroup = new QButtonGroup(this);
    difficultyGroup = new QButtonGroup(this);
    pairGroup = new QButtonGroup(this);
    time_radio = new QRadioButton("Time Limited Mode", this);
    time_radio->setFont(yahei);
    reaction_radio = new QRadioButton("Reaction Mode", this);
    reaction_radio->setFont(yahei);
    easy = new QRadioButton("EASY", this);
    easy->setFont(yahei);
    medium = new QRadioButton("MEDIUM", this);
    hard = new QRadioButton("HARD", this);
    single = new QRadioButton("SINGLE", this);
    pair = new QRadioButton("PAIR", this);
    medium->setFont(yahei);
    hard->setFont(yahei);
    single->setFont(yahei);
    pair->setFont(yahei);
    modeGroup->addButton(time_radio, 0);
    modeGroup->addButton(reaction_radio, 1);
    difficultyGroup->addButton(easy, 0);
    difficultyGroup->addButton(medium, 1);
    difficultyGroup->addButton(hard, 2);
    pairGroup->addButton(single, 0);
    pairGroup->addButton(pair, 1);
    time_radio->move(550, 250);
    reaction_radio->move(850, 250);
    easy->move(550, 350);
    medium->move(750, 350);
    hard->move(950, 350);
    single->move(550, 450);
    pair->move(800, 450);
    easy->setChecked(true);
    single->setChecked(true);
    time_radio->setChecked(true);
}

