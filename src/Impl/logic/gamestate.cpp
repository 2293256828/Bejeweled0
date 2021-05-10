#include <algorithm>
#include <fstream>
#include "src/headers/logic/gamestate.h"
#include "src/headers/components/scoresystem.h"
#include "src/headers/logic/game.h"


using namespace Bejeweled;
using namespace std;

GameState::GameState(QObject *parent) :
	QObject(parent),
	state_(ENDED)
{
    settings = new GameSettings;
}

void GameState::GameEndProcessor_(int score)
{
	delete game;
    game = nullptr;
	state_ = ENDED;
	if(highScores.NewScore(settings->mode, score))
		emit(gameEnd(true)); // high score
	else
		emit(gameEnd(false));
}
GameState::State GameState::state() const
{
	return state_;
}

void GameState::SetDifficulty(Difficulty diff)
{
    settings->difficulty = diff;
}

void GameState::SetMode(Mode mode)
{
    settings->mode = mode;
}

GameState::~GameState()
{

		delete game;
	delete settings;
}

/**
 * 开启一局游戏 连接信号与槽 调用game的NewGame()
 * @see game.cpp NewGame()
 * @return
 */
BoardEvent GameState::StartNewGame()
{
    game = new Game(*settings);
	connect(game, SIGNAL(timeTick(int)), this, SIGNAL(timeTick(int)));
	connect(game, SIGNAL(scoreUpdated(int)), this, SIGNAL(scoreUpdated(int)));
	connect(game, SIGNAL(gameEnd(int)), this, SLOT(GameEndProcessor_(int)));
	connect(game, SIGNAL(Hint(Bejeweled::JewelPos)), this, SIGNAL(Hint(Bejeweled::JewelPos)));

	state_ = INGAME;
	return game->NewGame();
}

void GameState::Pause()
{
	game->Pause();
	state_ = PAUSE;
}

void GameState::Resume()
{
	game->Resume();
	state_ = INGAME;
}

void GameState::Exit()
{
	delete game;
    game = nullptr;
	state_ = ENDED;
}
/**
 * 调用game的Swap方法
 * @param pos
 * @param direction
 * @return
 */
list<BoardEvent> GameState::Swap(JewelPos pos, SwapDirection direction)
{
	return game->Swap(pos, direction);
}


HighScoresStorage::HighScoresStorage() :
	tl_scores_(kMaxRecord,0),
	fr_scores_(kMaxRecord,0)
{
	fstream file("hs.dat",ios::in | ios::binary);
	if(file) {
		for(int i=0;i!=kMaxRecord;++i)
			file.read((char*)&(tl_scores_[i]),sizeof(int));
		for(int i=0;i!=kMaxRecord;++i)
			file.read((char*)&(fr_scores_[i]),sizeof(int));
	}
	file.close();
}

bool HighScoresStorage::NewScore(Mode mode,int new_score)
{
	bool ret = false;
	// Determine mode
	vector<int> *target = mode==Mode::TIME_LIMIT? &tl_scores_:&fr_scores_;
	for(int score: *target)
		// is_not_smallest
		if(score < new_score) {
			ret = true;
			break;
		}

	if(ret) {
		// get the new score to correct position
		target->push_back(new_score);
		sort(target->begin(),target->end(),[=](int a,int b){
			// descending sort
			return a>=b;
		});
		target->pop_back();
	}
	WriteToFile();
	return ret;
}

int HighScoresStorage::GetScore(Mode mode, int rank)
{
	if(mode == Mode::TIME_LIMIT)
		return tl_scores_[rank];
	else
		return fr_scores_[rank];
}

void HighScoresStorage::WriteToFile()
{
	fstream file("hs.dat",ios::out | ios::binary);
	if(file) {
		for(int i=0;i!=kMaxRecord;++i)
			file.write((char*)&(tl_scores_[i]),sizeof(int));
		for(int i=0;i!=kMaxRecord;++i)
			file.write((char*)&(fr_scores_[i]),sizeof(int));
	}
	file.close();
}

HighScoresStorage::~HighScoresStorage()
{
	WriteToFile(); // write again to ensure synchronization, not necessary for now though
}
