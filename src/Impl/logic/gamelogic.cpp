#include "src/headers/logic/gamelogic.h"

using namespace Bejeweled;
/**
 * gamelogic.cpp 接口,设置不同的模式逻辑
 */


ModeLogic::ModeLogic(QObject *parent) :
	QObject(parent)
{
	timer_= new CountdownTimer(0,this);
	connect(timer_, SIGNAL(TimeOut()), this, SIGNAL(timeOut()));
	//timer_internalTimeOut emit TimeOut()->modeLogic->gameState.gameEnd()
	connect(timer_,&CountdownTimer::Tick,[=](){
		emit(timeTick(timer_->GetTimeRemained()));});
	//timer_internalTimeOut emit TimeTick(int sec)->modeLogic->gameState->mainWindow.updateTimeDisplay(sec)
}
/**
 * mainWindow.hintClicked->gameState.Punish->game.Punish->modeLogic.Punish->timer.Punish
 * @param a second
 */
void ModeLogic::Punish(int a){
    timer_->Punish(a);
}
/**
 * mainWindow.pauseClicked->gameState.Pause->game.Pause->modeLogic.Pause->timer.Pause
 */
void ModeLogic::Pause()
{
	timer_->Pause();
}

/**
 * mainWindow.pauseClicked->gameState.Resume->game.Resume->modeLogic.Resume->timer.Resume
 */
void ModeLogic::Resume()
{
	timer_->Resume();
}
//!P1
TimeOutMode_P1::TimeOutMode_P1() :
        ModeLogic(nullptr)
{
    timer_->SetTimeRemained(120);
    timer_->Start();
}
int TimeOutMode_P1::getGeneration()
{
    if(timer_->GetTimeRemained() >= 60)
        return 3;
    else
        return 4;
}

//!P2
TimeOutMode_P2::TimeOutMode_P2() :
        ModeLogic(nullptr)
{
    timer_->SetTimeRemained(3600);
    timer_->Start();
}
int TimeOutMode_P2::getGeneration()
{
    if(timer_->GetTimeRemained() >= 60)
        return 3;
    else
        return 4;
}



//!P1
FastReactionMode_P1::FastReactionMode_P1() :
	ModeLogic(nullptr)
{
	timer_->SetTimeRemained(4);
	timer_->Start();
    diff = 4;
    count = 0;
}

int FastReactionMode_P1::getGeneration()
{
	return diff;
}


void FastReactionMode_P1::FinishedOneMove()
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
//!P2
FastReactionMode_P2::FastReactionMode_P2() :
        ModeLogic(nullptr)
{
    timer_->SetTimeRemained(3600);
    timer_->Start();
    diff = 4;
    count = 0;
}

int FastReactionMode_P2::getGeneration()
{
    return diff;
}


void FastReactionMode_P2::FinishedOneMove()
{
//    int ntime = 4;
//    if(count > 10)
//        ntime = 3;
//    if(count > 25)
//        ntime = 2;
//    timer_->SetTimeRemained(4);
//    emit(timeTick(ntime)); // refresh
    ++count;
}
