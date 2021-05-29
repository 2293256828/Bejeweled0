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
    QFont larger_font, smaller_font;
    larger_font.setPointSize(18);
    smaller_font.setPointSize(15);
    auto *layout = new QGridLayout(this);
    auto *label1 = new QLabel(tr("Time Limited Mode"), this);
    label1->setFont(larger_font);
    auto *label2 = new QLabel(tr("Fast Reaction Mode"), this);
    label2->setFont(larger_font);
    layout->addWidget(label1, 0, 0);
    layout->addWidget(label2, 0, 1);
    QLabel *tl_display[HighScoresStorage::kMaxRecord];
    QLabel *fr_display[HighScoresStorage::kMaxRecord];
    for (int i = 0; i != HighScoresStorage::kMaxRecord; ++i) {
        // read from GameState
        tl_display[i] = new QLabel(QString::number(state->GetScore(Mode::TIME_LIMIT, i)), this);
        tl_display[i]->setFont(smaller_font);
        fr_display[i] = new QLabel(QString::number(state->GetScore(Mode::FAST_REACTION, i)), this);
        fr_display[i]->setFont(smaller_font);
        layout->addWidget(tl_display[i], i + 1, 0);
        layout->addWidget(fr_display[i], i + 1, 1);
    }
    ret_button = new JewelButton(this);
    ret_button->setText(tr("Return"));
    layout->addWidget(ret_button, HighScoresStorage::kMaxRecord + 1, 1);
}

