#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <utility>
#include <QTcpServer>
#include <QProgressBar>
#include <QMediaPlayer>
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
    void startGame2(int );
	void drawBoardEvent(const BoardEvent& event);
    void drawBoardEvent2(const BoardEvent& event);
	bool swapJewelInMap(int x, int y, SwapDirection direction);
    bool swapJewelInMap2(int x, int y, SwapDirection direction);



    //!UI���
    QLabel*combo1Label;
    QLabel*combo1Display;
    QLabel*combo2Label;
    QLabel*combo2Display;
    QPalette redPa;
    QPalette yellowPa;
    QPalette bluePa;
    QPalette greenPa;
    QPalette greyPa;
    QPalette blackPa;
    QFrame*currentFrame;
    Homepage*homepage;
    Selectpage*selectpage;
    Roompage*roompage;
    Scorepage*scorepage;
	QLabel *scoreDisplay;
    QLabel *scoreDisplay2;
    QLabel *deathLabel;
	QProgressBar* progressBar;
	QPushButton *pauseButton;
	QPushButton *hintButton;
    QLabel*excellentLabel;
    Ui::MainWindow *ui;
    //!UI���
    //!gif
    QMovie*movie1;
    QMovie*movie2;
    QMovie*movie3;
    QMovie*movie4;
    //!gif
    //!Music
    QMediaPlayer*goSound;
    QMediaPlayer*timeUpSound;
    QMediaPlayer*goodSound;
    QMediaPlayer*awesomeSound;
    QMediaPlayer*excellentSound;
    QMediaPlayer*unbelievableSound;
    QMediaPlayer*combo_2;
    QMediaPlayer*combo_3;
    QMediaPlayer*combo_4;
    QMediaPlayer*combo_5;
    QMediaPlayer*combo_6;
    QMediaPlayer*combo_7;
    QMediaPlayer*badMove;
    QMediaPlayer*tick;
    QMediaPlayer*hit;
    QMediaPlaylist *bgMusicList;
     QMediaPlayer*bgplayer;
     QMediaPlayer*gameBgPlayer;
     //!Music
    //!����
    QTcpServer *server;
    QTcpSocket* socket;
    //!��Ա
	GameState *gameState;
	pair<Color,Jewel*> map_[Board::kLargeSize][Board::kLargeSize];//��ά�������ݽṹ,��Ӧλ�õı�ʯ��ɫ�뱦ʯ����
    int boardSize;//����size
    JewelPos hintPos;//��ʾ������
    //!��Ա
    bool death1;
    bool death2;
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
