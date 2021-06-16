#include <headers/ui/jewelbutton.h>
#include <headers/ui/setpage.h>
#include <iostream>


using namespace std;

Setpage::Setpage(QWidget *parent) :
        QFrame(parent) {
    auto*settingLabel3=new QLabel(this);
    settingLabel3->resize(150,150);
    settingLabel3->move(1460,850);
    auto*settingLabel4=new QLabel(this);
    settingLabel4->resize(150,150);
    settingLabel4->move(0,850);
    
    settingLabel3->setPixmap(QPixmap("D:\\Bejeweled00\\res\\pictures\\settings.png"));
    settingLabel4->setPixmap(QPixmap("D:\\Bejeweled00\\res\\pictures\\settings.png"));
    musicButton=new JewelButton(this);
    soundButton=new JewelButton(this);
    soundButton2=new JewelButton(this);
    musicButton->setText("TURN OFF MUSIC");
    soundButton->setText("TURN OFF SOUND EFFECT P1");
    soundButton2->setText("TURN OFF SOUND EFFECT P2");
    connect(musicButton, SIGNAL(clicked()), this, SLOT(musicButtonClicked()));
    connect(soundButton, SIGNAL(clicked()),this, SLOT(soundEffectButtonClicked()));
    connect(soundButton2, SIGNAL(clicked()),this, SLOT(soundEffect2ButtonClicked()));
    soundButton->move(480,800);soundButton2->move(780,800);
    soundButton->resize(280,50);soundButton2->resize(280,50);
    musicButton->move(470,650);
    musicSpinBox=new QSpinBox(this);
    musicSpinBox->setMinimum(0);  // 最小值
    musicSpinBox->setMaximum(100);  // 最大值
    musicSpinBox->setSingleStep(1);  // 步长
    musicSlider = new QSlider(this);

    musicSlider->setOrientation(Qt::Horizontal);  // 水平方向
    musicSlider->setMinimum(0);  // 最小值
    musicSlider->setMaximum(100);  // 最大值
    musicSlider->setSingleStep(1);
    connect(musicSpinBox, SIGNAL(valueChanged(int)), musicSlider, SLOT(setValue(int)));
    connect(musicSlider, SIGNAL(valueChanged(int)), musicSpinBox, SLOT(setValue(int)));
    musicSlider->move(470,600);
    musicSlider->resize(600,50);
    musicSlider->setValue(50);
    musicSpinBox->move(750,580);
    musicSlider->setTickPosition(QSlider::TicksAbove);
    musicSlider->setTracking(true);
    
    soundEffectSpinBox=new QSpinBox(this);
    soundEffectSpinBox->setMinimum(0);  // 最小值
    soundEffectSpinBox->setMaximum(100);  // 最大值
    soundEffectSpinBox->setSingleStep(1);  // 步长
    soundEffectSlider = new QSlider(this);
    soundEffectSlider->setTickPosition(QSlider::TicksAbove);
    soundEffectSlider->setOrientation(Qt::Horizontal);  // 水平方向
    soundEffectSlider->setMinimum(0);  // 最小值
    soundEffectSlider->setMaximum(100);  // 最大值
    soundEffectSlider->setSingleStep(1);
    connect(soundEffectSpinBox, SIGNAL(valueChanged(int)), soundEffectSlider, SLOT(setValue(int)));
    connect(soundEffectSlider, SIGNAL(valueChanged(int)), soundEffectSpinBox, SLOT(setValue(int)));
    soundEffectSlider->move(470,770);

    soundEffectSlider->setValue(50);
    soundEffectSlider->resize(600,30);
    soundEffectSpinBox->move(750,750);


    lineEdit = new QIPLineEdit(this);
    lineEdit->resize(160, 50);
    modifyIpButton = new JewelButton(this);
    modifyIpButton->setText(tr("MODIFY GOAL IP"));
    modifyIpButton->resize(300, 50);

    label = new QLabel(QString::fromStdString("IP ADDRESS"), this);
    label->setFont(QFont("Microsoft YaHei", 15, 150));
    QPalette gray_pa;
    gray_pa.setColor(QPalette::WindowText, Qt::black);
    label->setPalette(gray_pa);
    label->resize(300, 50);
    returnButton=new JewelButton(this);
    returnButton->resize(250,100);
    returnButton->move(1360,0);
    returnButton->setText("RETURN HOME");
    returnButton->setFont(QFont("Microsoft YaHei", 15, 150));

    returnButton2=new JewelButton(this);
    returnButton2->resize(250,100);
    returnButton2->move(0,0);
    returnButton2->setText("RETURN HOME");
    returnButton2->setFont(QFont("Microsoft YaHei", 15, 150));
    modifyIpButton->move(630, 430);
    label->move(720, 280);
    lineEdit->move(700, 330);


}

void Setpage::  musicButtonClicked(){
     if(musicButton->text()=="TURN OFF MUSIC")musicButton->setText("TURN ON MUSIC");
     else musicButton->setText("TURN OFF MUSIC");
    emit(changeMusic());
}
void Setpage::soundEffectButtonClicked() {
      if(soundButton->text()=="TURN OFF SOUND EFFECT P1")soundButton->setText("TURN ON SOUND EFFECT P1");
      else soundButton->setText("TURN OFF SOUND EFFECT P1");
      emit(changeSoundEffect());
}
void Setpage::soundEffect2ButtonClicked() {
    if(soundButton2->text()=="TURN OFF SOUND EFFECT P2")soundButton2->setText("TURN ON SOUND EFFECT P2");
    else soundButton2->setText("TURN OFF SOUND EFFECT P2");
    emit(changeSoundEffect2());
}
QString Setpage::getText() {
    return lineEdit->text();

}
