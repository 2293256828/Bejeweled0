#include <headers/ui/jewelbutton.h>
#include <headers/ui/roompage.h>

Roompage::Roompage(QWidget *parent, int port) :
        QFrame(parent) {
    double_go_button = new JewelButton(this);
    double_go_button->setText(tr("GO"));
    double_go_button->resize(300, 100);
    double_go_button->move(700, 550);
    label = new QLabel(QString::fromStdString("RoomNumber: " + std::to_string(port)), this);
    label->setFont(QFont("Microsoft YaHei", 15, 150));
    QPalette gray_pa;
    gray_pa.setColor(QPalette::WindowText, Qt::darkGray);
    label->setPalette(gray_pa);
    label->resize(600, 300);
    label->move(500, 300);
}

