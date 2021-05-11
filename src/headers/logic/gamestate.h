#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <QObject>
#include <vector>
#include "board.h"
#include "src/headers/ui/jewel.h"

using namespace std;
namespace Bejeweled {

enum  Difficulty { EASY, MEDIUM, HARD };
enum  Mode { TIME_LIMIT, FAST_REACTION,TIME_LIMIT_DOUBLE,FAST_REACTION_DOUBLE };
struct GameSettings {
	Mode mode;
	Difficulty difficulty;
};

class Game; // forward statement

class HighScoresStorage {
public:
	static const int kMaxRecord = 8;
	HighScoresStorage();
	bool NewScore(Mode mode,int score);
	int GetScore(Mode mode,int rank);
	void WriteToFile();
	~HighScoresStorage();

	// copy inhibited
	HighScoresStorage(const HighScoresStorage&) = delete;
	HighScoresStorage& operator=(const HighScoresStorage&) = delete;

private:
	vector<int> tl_scores_;
	vector<int> fr_scores_;
};

class GameState : public QObject
{
	Q_OBJECT



public:
	enum State { PAUSE, INGAME, ENDED };

	explicit GameState(QObject *parent = nullptr);
	~GameState() override;
	void SetDifficulty(Difficulty);
	void SetMode(Mode);

    GameSettings *getSettings() const;

    BoardEvent StartNewGame();
    BoardEvent StartNewGame2();
	void Pause();
	void Resume();
    void Punish(int);
	void Exit();
	int GetScore(Mode mode,int rank) {return highScores.GetScore(mode, rank);}
	list<BoardEvent> Swap(JewelPos, Bejeweled::SwapDirection direction);
    list<BoardEvent> Swap2(JewelPos pos, SwapDirection direction);
	// copy inhibited
	GameState(const GameState&) = delete;
	GameState& operator=(const GameState&) = delete;
	State state() const;

signals:
	void timeTick(int remain);
	void scoreUpdated(int new_score);
	void gameEnd(bool high_score);
	void Hint(Bejeweled::JewelPos pos);
    void scoreUpdated2(int new_score);

private slots:
	void GameEndProcessor_(int);

private:
	HighScoresStorage highScores;
	GameSettings *settings;
	State state_;
	Game *game;
	Game*game2;


};

}
#endif // GAMESTATE_H
