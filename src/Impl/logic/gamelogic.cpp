#include "src/headers/logic/gamelogic.h"

using namespace Bejeweled;
/**
 * gamelogic.cpp 设置不同的模式逻辑
 */
TimeOutMode::TimeOutMode() :
	ModeLogic(nullptr)
{
	timer_->SetTimeRemained(120);
	timer_->Start();
}

ModeLogic::ModeLogic(QObject *parent) :
	QObject(parent)
{
	timer_= new CountdownTimer(0,this);
	connect(timer_, SIGNAL(TimeOut()), this, SIGNAL(timeOut()));
	connect(timer_,&CountdownTimer::Tick,[=](){
		emit(timeTick(timer_->GetTimeRemained()));});
}

void ModeLogic::Punish(int a){
    timer_->Punish(a);
}

void ModeLogic::Pause()
{
	timer_->Pause();
}

void ModeLogic::Resume()
{
	timer_->Resume();
}

int TimeOutMode::getGeneration()
{
    if(timer_->GetTimeRemained() >= 60)
        return 3;
    else
        return 4;
}
FastReactionMode::FastReactionMode() :
	ModeLogic(nullptr)
{
	timer_->SetTimeRemained(4);
	timer_->Start();
    diff = 4;
    count = 0;
}

int FastReactionMode::getGeneration()
{
	return diff;
}


void FastReactionMode::FinishedOneMove()
{
	int ntime = 4;
	if(count > 10)
		ntime = 3;
	if(count > 25)
		ntime = 2;
	timer_->SetTimeRemained(4);
	emit(timeTick(ntime)); // refresh
	++count;
}
