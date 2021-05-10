#ifndef JEWEL_H
#define JEWEL_H

#include <QWidget>
#include <QPointF>
/**
 *
 */
namespace Bejeweled {
	enum Color { NONE = 0, RED = 1, ORANGE = 2, YELLOW = 3, GREEN = 4, BLUE = 5 };//
	enum SwapDirection { UP, RIGHT, DOWN, LEFT };
}
using namespace Bejeweled;
class Jewel : public QWidget
{
	Q_OBJECT
public:
	explicit Jewel(Color, QWidget *parent = nullptr);
	Color GetColor() const { return color; }
	void SetColor(Color c) { color = c;}

protected:
	void paintEvent(QPaintEvent *) override;
	void mousePressEvent(QMouseEvent *) override;
	void mouseMoveEvent(QMouseEvent *) override;
	void mouseReleaseEvent(QMouseEvent *) override;

signals:
	void Swap(Bejeweled::SwapDirection);
	

	
private:
	Color color;
	bool mouseDown;
	QPointF mouseDownPos;
	bool selected;
};

#endif // JEWEL_H
