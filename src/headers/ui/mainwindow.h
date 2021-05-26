#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <utility>
#include <QTcpServer>
#include <QProgressBar>
#include "jewel.h"
#include "src/headers/logic/board.h"
#include "homepage.h"
#include "selectpage.h"
#include "roompage.h"
#include "scorepage.h"

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
//!�ۺ���
private slots:
    //!����¼�
	void startClicked();
	void scoreClicked();
	void goClicked();
	void exitClicked();
	void pauseClicked();
    void hintClicked();
    void doubleClicked();
    void joinClicked();
   //!����¼�

   //!�����¼�
    void onSwap(Bejeweled::SwapDirection direction);
	void updateScore(int newScore);//���³ɼ�
    void updateScore2(int newScore);//���³ɼ�
	void updateTimeDisplay(int remain);//���¼�ʱ��Label
    void onSwap2(int x,int y,Bejeweled::SwapDirection direction);
	void gameEnd(bool highScore);//��Ϸ����
	void startHome();//��ҳ
	void updateHint(Bejeweled::JewelPos pos);//������ʾ
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



    //!UI���
    QMovie*movie1;
    QMovie*movie2;
    QMovie*movie3;
    QMovie*movie4;
    QFrame*currentFrame;
    Homepage*homepage;
    Selectpage*selectpage;
    Roompage*roompage;
    Scorepage*scorepage;
	QLabel *scoreDisplay;
    QLabel *scoreDisplay2;
	QLabel *timeDisplay;
	QProgressBar* progressBar;
	QPushButton *pauseButton;
	QPushButton *hintButton;
    Ui::MainWindow *ui;
    //!UI���

    //!����
    QTcpServer *server;
    QTcpSocket* P1socket;

    //!��Ա
	GameState *gameState;
	pair<Color,Jewel*> map_[Board::kLargeSize][Board::kLargeSize];//��ά�������ݽṹ,��Ӧλ�õı�ʯ��ɫ�뱦ʯ����
    int boardSize;//����size
    JewelPos hintPos;//��ʾ������
    //!��Ա

    bool uiDrawing; // swap lock,������UIʱ���ܽ���Hint,swap,pause����
    bool animationDrawing; // internal drawing lock,ĳ����������ʱ�����������ܻ���
    static const int kJewelWidgetSize = 50;//��ʯsize

    int combo1;
    int combo2;
    //todo ˫��ģʽ
    pair<Color,Jewel*> map2_[Board::kLargeSize][Board::kLargeSize];
    GameState *gameState2;
    bool animationDrawing2;
    bool uiDrawing2;
    bool readyDouble;
    int port;
    string startSignal;
    QString IP;
    void serverNewConnect();
    void joinRoom(int);
    void socketReadData();
    void socketDisconnected();
    void processReadStr(string s);
  static string processStrToSend(JewelPos pos, SwapDirection direction);
    void roomPage();
    void registerClicked();
    void loginClicked();
    void returnClicked();
};

#endif // MAINWINDOW_H
