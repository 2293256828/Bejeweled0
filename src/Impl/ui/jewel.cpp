#include <QPainter>
#include <QMouseEvent>
#include "src/headers/ui/jewel.h"



/**
 * ����һ����ʯ
 * @param color
 * @param parent
 */
Jewel::Jewel(Bejeweled::Color color, QWidget *parent) :
        QWidget(parent),
        color(color),
        mouseDown(false),
        selected(false) {

}

/**
 * �����¼�,����QLabel��ʹ��drawPixmap,����GPU���������CPUѹ��,��������Ϸ����
 * @param event
 */
void Jewel::paintEvent(QPaintEvent *event) {
    static const QPixmap redPix("D:/Bejeweled00/res/red.png");
    static const QPixmap orangePix("D:/Bejeweled00/res/orange.png");
    static const QPixmap yellowPix("D:/Bejeweled00/res/yellow.png");
    static const QPixmap greenPix("D:/Bejeweled00/res/green.png");
    static const QPixmap bluePix("D:/Bejeweled00/res/blue.png");
    static const QPixmap transPix("D:/Bejeweled00/res/trans2.png");
    QPainter painter(this);
    QPixmap const *pixmap;
    //FIXME paint pixmap according to color
    switch (color) {
        case Bejeweled::Color::RED:
            pixmap = &redPix;
            break;
        case Bejeweled::Color::ORANGE:
            pixmap = &orangePix;
            break;
        case Bejeweled::Color::YELLOW:
            pixmap = &yellowPix;
            break;
        case Bejeweled::Color::GREEN:
            pixmap = &greenPix;
            break;
        case Bejeweled::Color::BLUE:
            pixmap = &bluePix;
            break;
        default:
            pixmap = &transPix;
            break;
    }
    painter.drawPixmap(0, 0, size().width(), size().height(), *pixmap);
    QPen pen;
    pen.setWidth(3);
    pen.setColor(Qt::white);
    painter.setPen(pen);
    //�����ѡ��,���ڵ�ǰλ�û���һ����ɫ����
    if (selected)
        painter.drawRect(0, 0, geometry().width(), geometry().height());
}

/**
 * ��갴���¼�
 * @param event
 */
void Jewel::mousePressEvent(QMouseEvent *event) {
    mouseDown = true;
    mouseDownPos = event->pos();//��ȡ��갴��λ��
    selected = true;
    update(); //�ػ�
}

/**
 * ��껬���¼�
 * @param event
 */
void Jewel::mouseMoveEvent(QMouseEvent *event) {
    if (mouseDown) {
        const QPointF pos = event->pos();//������λ��
        const qreal ox = mouseDownPos.x();//����ʱ��λ��
        const qreal oy = mouseDownPos.y();
        const qreal dx = pos.x() - ox;
        const qreal dy = pos.y() - oy;

        //alg
        static const double kMarginFactor = 1.15;
        if (abs(dx) > abs(dy)) {//���򻬶��ж�
            if (dx > 0 && ox + dx > (this->size().width()) * kMarginFactor)
                    emit(Swap(RIGHT));//emit signal
            else if (dx < 0 && ox + dx < -(this->size().width() * (kMarginFactor - 1.0)))
                    emit(Swap(LEFT));
            else
                return;
        } else {//���򻬶��ж�
            if (dy > 0 && oy + dy > (double) (this->size().height()) * kMarginFactor)
                    emit(Swap(DOWN));
            else if (dy < 0 && oy + dy < -(this->size().width() * (kMarginFactor - 1.0)))
                    emit(Swap(UP));
            else
                return;
        }
        selected = false;
        update(); // �ػ�
        mouseDown = false;

    }
}

/**
 * ����ͷ�,�ָ���ʼ״̬
 */
void Jewel::mouseReleaseEvent(QMouseEvent *) {
    mouseDown = false;
    selected = false;
}
