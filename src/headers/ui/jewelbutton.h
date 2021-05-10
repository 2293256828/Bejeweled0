#ifndef JEWELBUTTON_H
#define JEWELBUTTON_H

#include <QPushButton>

class JewelButton : public QPushButton
{
	Q_OBJECT
public:
	explicit JewelButton(QWidget *parent = 0);
	
signals:
	
public slots:

protected:
	void paintEvent(QPaintEvent *event) override;
};

#endif // JEWELBUTTON_H
