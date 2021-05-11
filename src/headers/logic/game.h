#ifndef GAME_H
#define GAME_H

#include <QObject>
#include "board.h"
#include "src/headers/ui/jewel.h"

class QWidget;
namespace Bejeweled {

class Board;
class ScoreSystem;
struct GameSettings;
class ModeLogic;

class Game : public QObject
{
	Q_OBJECT
public:
	explicit Game(const GameSettings &settings,QObject *parent = nullptr); // doesn't start a new game
	list<BoardEvent> Swap(JewelPos, Bejeweled::SwapDirection direction);
    list<BoardEvent> Swap2(JewelPos pos, Bejeweled::SwapDirection direction);//todo P2
	BoardEvent NewGame();
	void Pause();
	void Resume();
    void Punish(int);
    // copy inhibited
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;

signals:
	void scoreUpdated(int);
	void timeTick(int);
	void gameEnd(int score);
	void Hint(Bejeweled::JewelPos);

public slots:
	void endGame();

private:
	Board *board;
	ScoreSystem *scoreSystem;
    ScoreSystem *scoreSystem2;//todo P2
	ModeLogic *modeLogic;
};

}
#endif // GAME_H
