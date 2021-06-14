//
// Created by 86186 on 2021/5/24.
//

#include <headers/ui/jewelbutton.h>
#include <headers/ui/scorepage.h>
#include <QGridLayout>
#include <headers/logic/gamestate.h>

using namespace Bejeweled;
Scorepage::Scorepage(QWidget *parent,GameState*state) :
        QFrame(parent) {
    QFont larger_font("Microsoft YaHei", 15, 150);
    larger_font.setPointSize(18);

    auto *label1 = new QLabel(tr("Time Limited Mode"), this);
    label1->setFont(larger_font);
    auto *label2 = new QLabel(tr("Fast Reaction Mode"), this);
    label2->setFont(larger_font);
    label2->setFont(larger_font);
    label1->move(500,200);
    label2->move(800,200);
    QLabel *tl_display[HighScoresStorage::kMaxRecord];
    QFont yahei("Microsoft YaHei", 10, 100);
    QLabel *fr_display[HighScoresStorage::kMaxRecord];

    for (int i = 0; i != HighScoresStorage::kMaxRecord; ++i) {
        // read from GameState
        tl_display[i] = new QLabel(QString::number(state->GetScore(Mode::TIME_LIMIT, i)), this);
        tl_display[i]->setFont(yahei);
        fr_display[i] = new QLabel(QString::number(state->GetScore(Mode::FAST_REACTION, i)), this);
        fr_display[i]->setFont(yahei);
        tl_display[i]->move(500,250+50*(HighScoresStorage::kMaxRecord-i));
        fr_display[i]->move(800,250+50*(HighScoresStorage::kMaxRecord-i));
    }
    ret_button = new JewelButton(this);
    ret_button->setText(tr("Return"));
    ret_button->move(500,900);
//todo

}

