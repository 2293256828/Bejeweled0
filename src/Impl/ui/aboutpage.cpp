#include <headers/ui/jewelbutton.h>
#include <iostream>
#include <headers/ui/aboutpage.h>


using namespace std;

Aboutpage::Aboutpage(QWidget *parent) :
        QFrame(parent) {

    label=new QLabel(QString::fromLocal8Bit("                                               ��Ϸ˵��\n\n�Ѷȷ�Ϊ3��:hard,medium,easy,ÿ���в�ͬ��С������\n\nģʽ��Ϊ2��:FAST_REACTION �� TIME_LIMIT,ǰ��ÿ��ֻ��2-4s�ķ�Ӧʱ��\n"
                        "������ˢ��ʱ��,��������ʱ������ģʽ\n������ս:��֧�־���������,��ҿ���ѡ��Pair���п���,������һ�����IP��ַ��\n�����"
                        "��һ�����������޸�IP����뷿�伴��\n\n���ڼ�ʱ:Ϊ����Ϸ�����빫ƽ,��������ʱ��ͣ��ʱ\n\n"
                        "������ʾ:��ҿ��Ե��Hint������ʾ,������ʱ�����2s\n\n�������ֺ���Ч:����������ҳ������������Ч����,����˫�˽Կ�\n\n����combo����:һ���ƶ�����������,ÿ������ʹcombo+1,����,���һ������\n������5������"
                        "��ʯ,Ҳ�ά��һ��combo,combo���ж���÷ֺ�������Ч�Ͷ���\n\n"),this);
    label->move(360,100);
    label->setFont(QFont("Microsoft YaHei", 15, 150));
    label->resize(900,800);
returnButton2=new JewelButton(this);
returnButton2->setText("I KNEW IT");
returnButton2->move(500,800);
}

