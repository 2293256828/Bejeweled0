#include "src/headers/logic/gamestate.h"
#include "src/headers/logic/game.h"
#include "src/headers/logic/gamelogic.h"
#include "src/headers/components/scoresystem.h"
/**
 * game.cpp �̳�QObject�Ĵ���,������ϼǷ�ϵͳ,ģʽ���ú��Ѷ����á����̵ĳ�ʼ��
 */
using namespace Bejeweled;
using namespace std;
Game::Game(const GameSettings &settings,QObject *parent) :
	QObject(parent)
{
	Board::Size boardSize;
	switch(settings.difficulty) {
	case EASY:
        boardSize = Board::LARGE;
		break;
	case MEDIUM:
        boardSize = Board::MEDIUM;
		break;
	case HARD:
        boardSize = Board::SMALL;
		break;
	default:
		break;
	}
    board = new Board(boardSize);
	// Create appropriate GameLogic
	switch (settings.mode) {
	case TIME_LIMIT:
        modeLogic = new TimeOutMode();

		break;
	case FAST_REACTION:
        modeLogic = new FastReactionMode();
		break;
	case FAST_REACTION_DOUBLE:
	        modeLogic=new FastReactionMode();
            break;
    case TIME_LIMIT_DOUBLE:
	        modeLogic=new TimeOutMode();
            break;
	}
    scoreSystem = new ScoreSystem;
    board->setGenerationFactor(modeLogic->getGeneration());
}


list<BoardEvent> Game::Swap(JewelPos pos, Bejeweled::SwapDirection direction)
{
	// update generation factor
    board->setGenerationFactor(modeLogic->getGeneration());
	list<BoardEvent> events = board->Swap(pos, direction);
	bool first = true; //Processing First BoardEvent
	for(const BoardEvent& event : events) {
		if(first && event.type == BoardEvent::DIE) {
            scoreSystem->firstGain(event.getDiePos().size());
			emit(scoreUpdated(scoreSystem->getScore()));
			first = false;
		}
		else if(!first && event.type == BoardEvent::DIE) {
            scoreSystem->comboGain(event.getDiePos().size());
			emit(scoreUpdated(scoreSystem->getScore()));
		}
	}

	if(!events.empty()) { //not a fail swap
		modeLogic->FinishedOneMove();
        scoreSystem->finishMove();
	}
    emit(Hint(board->getPossibleSwap()));

	return events;
}
list<BoardEvent> Game::Swap2(JewelPos pos, Bejeweled::SwapDirection direction)
{
    // update generation factor
    board->setGenerationFactor(modeLogic->getGeneration());
    list<BoardEvent> events = board->Swap(pos, direction);
    bool first = true; //Processing First BoardEvent
    for(const BoardEvent& event : events) {
        if(first && event.type == BoardEvent::DIE) {
            scoreSystem2->firstGain(event.getDiePos().size());
            emit(scoreUpdated(scoreSystem2->getScore()));
            first = false;
        }
        else if(!first && event.type == BoardEvent::DIE) {
            scoreSystem2->comboGain(event.getDiePos().size());
            emit(scoreUpdated(scoreSystem2->getScore()));
        }
    }

    if(!events.empty()) { //not a fail swap
        modeLogic->FinishedOneMove();
        scoreSystem->finishMove();
    }
            emit(Hint(board->getPossibleSwap()));

    return events;
}
/**
 * ����һ����Ϸ,����board��Init()����
 * @see board.h Init()
 * @return
 */
BoardEvent Game::NewGame()
{
	connect(modeLogic, SIGNAL(timeOut()), this, SLOT(endGame()));
	connect(modeLogic, SIGNAL(timeTick(int)), this, SIGNAL(timeTick(int)));

	BoardEvent ret = board->Init();
	return ret;
}
void Game::Punish(int a) {
    modeLogic->Punish(a);
}
void Game::Pause()
{
	modeLogic->Pause();
}

void Game::Resume()
{
	modeLogic->Resume();
}

void Game::endGame()
{
	emit(gameEnd(scoreSystem->getScore()));
}
