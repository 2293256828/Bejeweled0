//
// Created by 86186 on 2021/5/29.
//

#ifndef BEJEWELED00_QIPLINEEDIT_H
#define BEJEWELED00_QIPLINEEDIT_H

#include <QLineEdit>
#include <QEvent>
class QIPLineEdit : public QLineEdit
{
Q_OBJECT

public:
    QIPLineEdit(QWidget *parent = 0);
    ~QIPLineEdit();

    void setText(const QString &strIP);
    QString text() const;
protected:
    void paintEvent(QPaintEvent *event);
    bool eventFilter(QObject *obj, QEvent *ev);

    int getIndex(QLineEdit *pEdit);
    bool isTextValid(const QString &strIP);
private:
    QLineEdit *m_lineEidt[4];
};



#endif //BEJEWELED00_QIPLINEEDIT_H
