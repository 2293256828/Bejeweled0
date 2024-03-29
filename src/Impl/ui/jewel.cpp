#include <QPainter>
#include <QMouseEvent>
#include "src/headers/ui/jewel.h"



/**
 * 代表一个宝石
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
 * 绘制事件,不用QLabel而使用drawPixmap,利用GPU处理画面减轻CPU压力,提高玩家游戏体验
 * @param event
 */
void Jewel::paintEvent(QPaintEvent *event) {
    static const QPixmap redPix("D:/Bejeweled00/res/pictures/red.png");
    static const QPixmap orangePix("D:/Bejeweled00/res/pictures/orange.png");
    static const QPixmap yellowPix("D:/Bejeweled00/res/pictures/yellow.png");
    static const QPixmap greenPix("D:/Bejeweled00/res/pictures/green.png");
    static const QPixmap bluePix("D:/Bejeweled00/res/pictures/blue.png");
    static const QPixmap transPix("D:/Bejeweled00/res/pictures/trans2.png");
    QPainter painter(this);
    QPixmap const *pixmap;

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
    //如果被选中,则在当前位置绘制一个白色矩形
    if (selected)
        painter.drawRect(0, 0, geometry().width(), geometry().height());
}

/**
 * 鼠标按下事件
 * @param event
 */
void Jewel::mousePressEvent(QMouseEvent *event) {
    mouseDown = true;
    mouseDownPos = event->pos();//获取鼠标按下位置
    selected = true;
    update(); //重绘
}

/**
 * 鼠标滑动事件
 * @param event
 */
void Jewel::mouseMoveEvent(QMouseEvent *event) {
    if (mouseDown) {
        const QPointF pos = event->pos();//滑动的位置
        const qreal ox = mouseDownPos.x();//按下时的位置
        const qreal oy = mouseDownPos.y();
        const qreal dx = pos.x() - ox;
        const qreal dy = pos.y() - oy;

        //alg
        static const double kMarginFactor = 1.15;
        if (abs(dx) > abs(dy)) {//横向滑动判定
            if (dx > 0 && ox + dx > (this->size().width()) * kMarginFactor)
                    emit(Swap(RIGHT));//emit signal
            else if (dx < 0 && ox + dx < -(this->size().width() * (kMarginFactor - 1.0)))
                    emit(Swap(LEFT));
            else
                return;
        } else {//纵向滑动判定
            if (dy > 0 && oy + dy > (double) (this->size().height()) * kMarginFactor)
                    emit(Swap(DOWN));
            else if (dy < 0 && oy + dy < -(this->size().width() * (kMarginFactor - 1.0)))
                    emit(Swap(UP));
            else
                return;
        }
        selected = false;
        update(); // 重绘
        mouseDown = false;

    }
}

/**
 * 鼠标释放,恢复初始状态
 */
void Jewel::mouseReleaseEvent(QMouseEvent *) {
    mouseDown = false;
    selected = false;
}
