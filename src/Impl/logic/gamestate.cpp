#include <algorithm>
#include <fstream>
#include "src/headers/logic/gamestate.h"
#include "src/headers/components/scoresystem.h"
#include "src/headers/logic/game.h"

/**
 * �̳�QWidget��ΪQT���,�Ӷ����Խ����ź�
 */
using namespace Bejeweled;
using namespace std;

GameState::GameState(QObject *parent) :
        QObject(parent),
        state_(ENDED)
{
    settings = new GameSettings;
    server = new Connect();
}

void GameState::GameEndProcessor_(int score)
{
    delete game;
    game = nullptr;
    state_ = ENDED;

    //����server��ID���Ա㱣�浱ǰ����ڵ�ǰģʽ����߷�
    server->setID(ID);
    server->ServerScore(score,settings->mode);

    if(highScores.NewScore(settings->mode, score)) {
        emit(gameEnd(true)); // high score
    }
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
 * ����һ����Ϸ �����ź���� ����game��NewGame()
 * @see game.cpp NewGame()
 * @return ��õ�NEW�¼����ظ�mainWindow��startGame()��ȥ����
 */
BoardEvent GameState::StartNewGame(int seed)
{
    game = new Game(*settings);
    connect(game, SIGNAL(timeTick(int)), this, SIGNAL(timeTick(int)));
    //game->gameState->mainWindow, purpose:����ʱ��Label
    connect(game, SIGNAL(scoreUpdated(int)), this, SIGNAL(scoreUpdated(int)));
    //game->gameState->mainWindow, purpose :���³ɼ�Label

    connect(game, SIGNAL(gameEnd(int)), this, SLOT(GameEndProcessor_(int)));
    //game->gameState->mainWindow ,purpose:��Ϸ��������, (int)�ǽ���ʱ�ĳɼ�

    connect(game, SIGNAL(Hint(Bejeweled::JewelPos)), this, SIGNAL(Hint(Bejeweled::JewelPos)));
    //game->gameState->mainWindow , purpose: ÿ�ν����������ʾ��λ�� (JewelPos)����mainWindow��¼����
    state_ = INGAME;
    return game->NewGame(seed);
}

/**
 * mainWindow.pauseClicked->gameState.Pause->game.Pause->modeLogic.Pause->timer.Pause
 */
void GameState::Pause()
{
    game->Pause();
    state_ = PAUSE;
}
/**
 * mainWindow.hintClicked->gameState.Punish->game.Punish->modeLogic.Punish->timer.Punish
 * @param a second
 */
void GameState::Punish(int a) {
    game->Punish(a);
}

/**
 * mainWindow.pauseClicked->gameState.Resume->game.Resume->modeLogic.Resume->timer.Resume
 */
void GameState::Resume()
{
    game->Resume();
    state_ = INGAME;
}

/**
 * mainWindow.exitClicked->exit()
 */
void GameState::Exit()
{
    delete game;
    game = nullptr;
    state_ = ENDED;
}
/**
 * ����game��Swap����
 * @param pos
 * @param direction
 * @return
 */
list<BoardEvent> GameState::Swap(JewelPos pos, SwapDirection direction)
{
    return game->Swap(pos, direction);
}
GameSettings *GameState::getSettings() const {
    return settings;
}

HighScoresStorage::HighScoresStorage() :
        tl_scores_(kMaxRecord,0),
        fr_scores_(kMaxRecord,0)
{
    server = new Connect();
    updateRankScore();

}
void HighScoresStorage::updateRankScore()
{
    vector<int> ranking;
    ranking = server->ServerRank();
    for (int i = 0;i < kMaxRecord*2;i++){
        tl_scores_[i/2] = ranking[i];
        i++;
        fr_scores_[i/2] = ranking[i];
    }
    sort(tl_scores_.begin(),tl_scores_.end());
    sort(fr_scores_.begin(),fr_scores_.end());
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
    //WriteToFile();
    return ret;
}

int HighScoresStorage::GetScore(Mode mode, int rank)
{
    if(mode == Mode::TIME_LIMIT)
        return tl_scores_[rank];
    else
        return fr_scores_[rank];
}

HighScoresStorage::~HighScoresStorage()
{

}
