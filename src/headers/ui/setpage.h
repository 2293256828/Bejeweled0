
#ifndef BEJEWELED00_SETPAGE_H
#define BEJEWELED00_SETPAGE_H
#include <QFrame>
#include <QLabel>
#include <QSlider>
#include <QSpinBox>
#include "jewelbutton.h"
#include "qiplineedit.h"
class Setpage : public QFrame{
Q_OBJECT
public:
    explicit Setpage(QWidget*parent=nullptr);
    JewelButton*modifyIpButton;
    QLabel*label;
    QIPLineEdit*lineEdit;
    QSlider *musicSlider ;
    QSpinBox *musicSpinBox;
    QSlider *soundEffectSlider ;
    QSpinBox*soundEffectSpinBox;
    JewelButton*musicButton;
    JewelButton*soundButton;
    JewelButton*soundButton2;
    JewelButton*returnButton;
    JewelButton*returnButton2;
    QString getText();
    signals:
    void changeMusic();
    void changeSoundEffect();
    void changeSoundEffect2();
private slots:
    void musicButtonClicked();
    void soundEffectButtonClicked();
    void soundEffect2ButtonClicked();
};


#endif //BEJEWELED00_SETPAGE_H
