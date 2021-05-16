#include "src/headers/components/timer.h"
#include <QTimer>
#include <cassert>
/**
 * QT计时器
 *
 * @param sec
 * @param parent
 */
CountdownTimer::CountdownTimer(int sec,QObject *parent) :
        QObject(parent),
        remainSec(0),
        paused(false),
        dead(true)
{
    tick = new QTimer(this);
	tick->setInterval(1000);
	tick->setSingleShot(false); // 每隔1s调用一次InternalTimerTimout_
	connect(tick, SIGNAL(timeout()), this, SLOT(InternalTimerTimeout_()));
	SetTimeRemained(sec);
}

void CountdownTimer::Start()
{
	if(dead) {
        dead = false;
		tick->start();
	}
}
/**
 * mainWindow.pauseClicked->gameState.Pause->game.Pause->modeLogic.Pause->timer.Pause
 */
void CountdownTimer::Pause()
{
    paused = true;
}

/**
 * mainWindow.pauseClicked->gameState.Resume->game.Resume->modeLogic.Resume->timer.Resume
 */
void CountdownTimer::Resume()
{
    paused = false;
}

void CountdownTimer::InternalTimerTimeout_()
{
	if(!dead && !paused) {
		if(--remainSec == 0) {
			emit(TimeOut());
            dead = true;
		}
		emit(Tick());
	}
}
/**
 * mainWindow.hintClicked->gameState.Punish->game.Punish->modeLogic.Punish->timer.Punish
 * @param a second
 */
void CountdownTimer::Punish(int sec)
{
    if(!dead && !paused) {
        remainSec-=sec;
        if(remainSec <= 0) {
            remainSec=0;
            emit(TimeOut());
            dead = true;
        }
        emit(Tick());
    }
}
void CountdownTimer::SetTimeRemained(int sec)
{
	if(sec > 0)
        remainSec = sec;
}
