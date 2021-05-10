#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <utility>
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
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
//槽函数
private slots:
	void startClicked();
	void scoreClicked();
	void goClicked();
	void exitClicked();
	void pauseClicked();
	void updateScore(int newScore);
	void updateTimeDisplay(int remain);
	void onSwap(Bejeweled::SwapDirection direction);
	void gameEnd(bool highScore);
	void startHome();
	void hintProcessor(Bejeweled::JewelPos pos);

private:
	void showHint();
	bool uiDrawing; // swap lock
	bool animationDrawing; // internal drawing lock

	static const int kJewelWidgetSize = 50;
	// Three StartXXX functions affects only GUI
	void startSelect();
	void startGame();
	void drawBoardEvent(const BoardEvent& event);
	bool swapJewelInMap(int x, int y, SwapDirection direction);

	QFrame *currentFrame;
	QButtonGroup *modeGroup;
	QButtonGroup *buttonGroup;
	QButtonGroup*pairGroup;
	QLabel *scoreDisplay;
	QLabel *timeDisplay;
	QPushButton *pushButton;
    Ui::MainWindow *ui;
	GameState *gameState;
	pair<Color,Jewel*> map_[Board::kLargeSize][Board::kLargeSize];//二维数组,存放对应位置的宝石颜色与宝石对象
	int boardSize;
	bool hint; // whether to show hint
	JewelPos hintPos;
};

#endif // MAINWINDOW_H
