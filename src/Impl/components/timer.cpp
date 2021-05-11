#include "src/headers/components/timer.h"
#include <QTimer>
#include <cassert>
/**
 * QT¼ÆÊ±Æ÷
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
	tick->setSingleShot(false); // repeat ticks
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

void CountdownTimer::Pause()
{
    paused = true;
}

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
