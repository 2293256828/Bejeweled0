#ifndef JEWELBUTTON_H
#define JEWELBUTTON_H

#include <QPushButton>
#include <QSound>

class JewelButton : public QPushButton
{
	Q_OBJECT
public:

	explicit JewelButton(QWidget *parent = 0);

signals:
	
public slots:

protected:
	void paintEvent(QPaintEvent *event) override;
    bool event(QEvent *e) override;

private:
    bool sound_effect;

};

#endif // JEWELBUTTON_H
