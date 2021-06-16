#include <headers/ui/jewelbutton.h>
#include <iostream>
#include <headers/ui/aboutpage.h>


using namespace std;

Aboutpage::Aboutpage(QWidget *parent) :
        QFrame(parent) {

    label=new QLabel(QString::fromLocal8Bit("                                               游戏说明\n\n难度分为3种:hard,medium,easy,每种有不同大小的棋盘\n\n模式分为2种:FAST_REACTION 与 TIME_LIMIT,前者每次只有2-4s的反应时间\n"
                        "消除后刷新时间,后者是总时间限制模式\n联机对战:仅支持局域网联机,玩家可以选择Pair进行开房,告诉另一人你的IP地址和\n房间号"
                        "另一人在设置中修改IP后加入房间即可\n\n关于计时:为了游戏体验与公平,动画绘制时暂停计时\n\n"
                        "关于提示:玩家可以点击Hint进行提示,代价是时间减少2s\n\n关于音乐和音效:可以在设置页面设置音乐音效音量,单人双人皆可\n\n关于combo机制:一次移动引起多次消除,每次消除使combo+1,此外,如果一次消除\n消除了5个以上"
                        "宝石,也会奖励一次combo,combo会有额外得分和特殊音效和动画\n\n"),this);
    label->move(360,100);
    label->setFont(QFont("Microsoft YaHei", 15, 150));
    label->resize(900,800);
returnButton2=new JewelButton(this);
returnButton2->setText("I KNEW IT");
returnButton2->move(500,800);
}

