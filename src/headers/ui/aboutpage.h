//
// Created by 86186 on 2021/6/12.
//

#ifndef BEJEWELED00_ABOUTPAGE_H
#define BEJEWELED00_ABOUTPAGE_H


#include <QFrame>
#include <QLabel>
#include <QSlider>
#include <QSpinBox>
#include "jewelbutton.h"
#include "qiplineedit.h"

class Aboutpage : public QFrame{
    Q_OBJECT
public:
    explicit Aboutpage(QWidget*parent=nullptr);
    QLabel*label;
    JewelButton*returnButton2;

};
#endif //BEJEWELED00_ABOUTPAGE_H