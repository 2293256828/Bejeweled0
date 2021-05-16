#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <utility>
#include <QTcpServer>
#include "jewel.h"
#include "src/headers/logic/board.h"

namespace Bejeweled {
	class GameState;
}
using namespace std;
using namespace Bejeweled;
namespace Ui {
class MainWindow;
}

class QFrame;
class QButtonGroup;
class QColor;
class QGridLayout;
class QLabel;
class QPushButton;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
//!槽函数
private slots:
    //!点击事件
	void startClicked();
	void scoreClicked();
	void goClicked();
	void exitClicked();
	void pauseClicked();
    void hintClicked();
    void DoubleClicked();
    void joinClicked();
   //!点击事件

   //!交换事件
    void onSwap(Bejeweled::SwapDirection direction);

	void updateScore(int newScore);//更新成绩
	void updateTimeDisplay(int remain);//更新计时器Label

    void onSwap2(int x,int y,Bejeweled::SwapDirection direction);

	void gameEnd(bool highScore);//游戏结束
	void startHome();//主页
	void updateHint(Bejeweled::JewelPos pos);//更新提示
    void createRoom();

private:


	// Three StartXXX functions affects only GUI

	void startSelect();
	void startGame();
    void startGame2();
	void drawBoardEvent(const BoardEvent& event);
    void drawBoardEvent2(const BoardEvent& event);
	bool swapJewelInMap(int x, int y, SwapDirection direction);
    bool swapJewelInMap2(int x, int y, SwapDirection direction);



    //!UI组件
    QFrame *currentFrame;
	QButtonGroup *modeGroup;
	QButtonGroup *buttonGroup;
	QButtonGroup*pairGroup;
	QLabel *scoreDisplay;
	QLabel *timeDisplay;
	QPushButton *pauseButton;
	QPushButton *hintButton;
    Ui::MainWindow *ui;
    //!UI组件

    //!网络
    QTcpServer *server;
    QTcpSocket* P1socket;
    QTcpSocket*P2socket;
    //!成员
	GameState *gameState;
	pair<Color,Jewel*> map_[Board::kLargeSize][Board::kLargeSize];//二维数组数据结构,对应位置的宝石颜色与宝石对象
    int boardSize;//棋盘size
    JewelPos hintPos;//提示的坐标
    //!成员

    bool uiDrawing; // swap lock,当绘制UI时不能进行Hint,swap,pause操作
    bool animationDrawing; // internal drawing lock,某个动画绘制时其他动画不能绘制
    static const int kJewelWidgetSize = 50;//宝石size


    //todo 双人模式
    pair<Color,Jewel*> map2_[Board::kLargeSize][Board::kLargeSize];
    GameState *gameState2;
    bool animationDrawing2;
    bool uiDrawing2;
    bool ready;
    int port;
    void serverNewConnect();

    void joinRoom(int);

    void socketReadData();

    void socketDisconnected();

    void processReadStr(string s);

  string processStrToSend(JewelPos pos, SwapDirection direction);

    void roomPage();


};

#endif // MAINWINDOW_H
