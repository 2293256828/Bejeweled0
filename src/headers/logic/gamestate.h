#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <QObject>
#include <vector>
#include "board.h"
#include "src/headers/ui/jewel.h"
#include "Server/Connect.h"

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
        void updateRankScore();
        bool NewScore(Mode mode,int score);
        int GetScore(Mode mode,int rank);
        ~HighScoresStorage();

        // copy inhibited
        HighScoresStorage(const HighScoresStorage&) = delete;
        HighScoresStorage& operator=(const HighScoresStorage&) = delete;

    private:
        Connect *server;
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

        void setID(string s){ID = s;}
        GameSettings *getSettings() const;

        BoardEvent StartNewGame(int);

        void Pause();
        void Resume();
        void Punish(int);
        void Exit();


        int GetScore(Mode mode,int rank) {return highScores.GetScore(mode, rank);}
        list<BoardEvent> Swap(JewelPos, Bejeweled::SwapDirection direction);

        // copy inhibited
        State state() const;

    signals:
        void timeTick(int remain);//send to mainWindow, purpose :更新计时器显示Label
        void scoreUpdated(int new_score);//send to mainWindow, purpose :更新成绩显示Label
        void gameEnd(bool high_score);//send to mainWindow ,purpose :游戏结束处理
        void Hint(Bejeweled::JewelPos pos);//     send to mainWindow ,purpose :更新提示位置


        /**
         * 槽函数
         * 游戏结束时
         */
    private slots:
        void GameEndProcessor_(int);

    private:
        HighScoresStorage highScores;
        GameSettings *settings;
        State state_;
        Game * game;

        Connect *server;
        string ID;
    };

}
#endif // GAMESTATE_H
