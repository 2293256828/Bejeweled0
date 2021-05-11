#include <QPropertyAnimation>
#include <QTimer>
#include <QMessageBox>
#include <QGridLayout>
#include <QPixmap>
#include <QRadioButton>
#include <QButtonGroup>
#include <QLabel>
#include <QFrame>
#include <QCoreApplication>
#include <QParallelAnimationGroup>
#include <iostream>
#include "src/headers/ui/jewelbutton.h"
#include "src/headers/ui/mainwindow.h"
#include "src/headers/ui/ui_mainwindow.h"
#include "src/headers/logic/gamestate.h"

using namespace Bejeweled;
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow),
        uiDrawing(false),
        animationDrawing(false),
        hintPos(0, 0)
{
	ui->setupUi(this);
	setWindowIcon(QIcon(QPixmap("D:/Bejeweled00/res/green.png")));
    gameState = new GameState();
	connect(gameState, SIGNAL(gameEnd(bool)), this, SLOT(gameEnd(bool)));

    currentFrame = nullptr;
    startHome();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::startHome()
{
    delete currentFrame;
    currentFrame = new QFrame(this);
    QPalette palette;  //创建一个调色板的对象
    QPixmap pixmap;
    pixmap.load("D:/Bejeweled00/res/bg.png");
    palette.setBrush(backgroundRole(), QBrush(pixmap));
    setPalette(palette);
	setCentralWidget(currentFrame);
	JewelButton *start_button,*score_button;
	start_button = new JewelButton(currentFrame);
	score_button = new JewelButton(currentFrame);


	start_button->setText(tr("START GAME"));
	score_button->setText(tr("SCORE BOARD"));
    start_button->move(500,750);
    score_button->move(500,690);

	currentFrame->show();
	QObject::connect(start_button,SIGNAL(clicked()),this,SLOT(startClicked()));
	QObject::connect(score_button,SIGNAL(clicked()),this,SLOT(scoreClicked()));
}

void MainWindow::updateHint(JewelPos pos)
{
    hintPos = pos;
}

void MainWindow::hintClicked()
{   if(gameState->state()!=GameState::INGAME){return;}
    uiDrawing= true;
    pauseButton->setEnabled(false);
    hintButton->setEnabled(false);
	Jewel *widget = map_[hintPos.x][hintPos.y].second;
	int ox=widget->x(), oy = widget->y();
	// the animation
    gameState->Punish(2);
	auto *animation = new QPropertyAnimation(widget,"geometry");
	animation->setDuration(200);
	// start from a bigger Jewel
	animation->setStartValue((QRect(ox-kJewelWidgetSize/2,oy-kJewelWidgetSize/2,kJewelWidgetSize*2,kJewelWidgetSize*2)));
	// resize to normal size
	animation->setEndValue((QRect(ox,oy,kJewelWidgetSize,kJewelWidgetSize)));
    animationDrawing = true;
	connect(animation,&QPropertyAnimation::finished,[=](){
        animationDrawing = false;
	});
	animation->start();
	while(animationDrawing)
		QCoreApplication::processEvents(QEventLoop::AllEvents);
    pauseButton->setEnabled(true);
    hintButton->setEnabled(true);
    uiDrawing = false;
}

void MainWindow::startClicked()
{     currentFrame->hide();
    startSelect();
}

void MainWindow::scoreClicked()
{
	// font setup
	QFont larger_font,smaller_font;
	larger_font.setPointSize(18);
	smaller_font.setPointSize(15);
	delete currentFrame;

	// set up new frame
	currentFrame = new QFrame(this);
	setCentralWidget(currentFrame);
	auto *layout = new QGridLayout(currentFrame);
	auto *label1 = new QLabel(tr("Time Limited Mode"), currentFrame);
	label1->setFont(larger_font);
	auto *label2 = new QLabel(tr("Fast Reaction Mode"), currentFrame);
	label2->setFont(larger_font);
	layout->addWidget(label1,0,0);
	layout->addWidget(label2,0,1);
	QLabel *tl_display[HighScoresStorage::kMaxRecord];
	QLabel *fr_display[HighScoresStorage::kMaxRecord];
	for(int i=0;i!=HighScoresStorage::kMaxRecord;++i) {
		// read from GameState
		tl_display[i] = new QLabel(QString::number(gameState->GetScore(Mode::TIME_LIMIT, i)), currentFrame);
		tl_display[i]->setFont(smaller_font);
		fr_display[i] = new QLabel(QString::number(gameState->GetScore(Mode::FAST_REACTION, i)), currentFrame);
		fr_display[i]->setFont(smaller_font);
		layout->addWidget(tl_display[i],i+1,0);
		layout->addWidget(fr_display[i],i+1,1);
	}

	auto *ret_button = new JewelButton(currentFrame);
	ret_button->setText(tr("Return"));
	layout->addWidget(ret_button,HighScoresStorage::kMaxRecord+1,1);
	connect(ret_button,SIGNAL(clicked()),this,SLOT(startHome()));
}

void MainWindow::startSelect()
{
		delete currentFrame;
	// set up new frame
	currentFrame = new QFrame(this);
    QPalette palette;  //创建一个调色板的对象
    QPixmap pixmap;
    pixmap.load("D:/Bejeweled00/res/background3.jpg");
    palette.setBrush(backgroundRole(), QBrush(pixmap));
    setPalette(palette);
	setCentralWidget(currentFrame);
	currentFrame->update();
	auto *go_button = new JewelButton(currentFrame);
	go_button->setText(tr("GO"));
	go_button->move(500,900);
	connect(go_button,SIGNAL(clicked()),this,SLOT(goClicked()));
	auto *mode_group = new QButtonGroup(currentFrame);
	auto *difficulty_group = new QButtonGroup(currentFrame);
    auto *pair_group = new QButtonGroup(currentFrame);
	auto *time_radio = new QRadioButton("Time Limited Mode", currentFrame);
	auto *reaction_radio = new QRadioButton("Reaction Mode", currentFrame);
	auto *easy = new QRadioButton("EASY", currentFrame);
	auto *medium = new QRadioButton("MEDIUM", currentFrame);
	auto *hard = new QRadioButton("HARD", currentFrame);
    auto *single=new QRadioButton("SINGLE",currentFrame);
    auto*pair=new QRadioButton("PAIR",currentFrame);
	mode_group->addButton(time_radio,0);
	mode_group->addButton(reaction_radio,1);
	difficulty_group->addButton(easy,0);
	difficulty_group->addButton(medium,1);
	difficulty_group->addButton(hard,2);
	pair_group->addButton(single,0);
	pair_group->addButton(pair,1);
	time_radio->move(500,750);
	reaction_radio->move(700,750);
	easy->move(500,700);
	medium->move(700,700);
	hard->move(900,700);
	single->move(500,800);
	pair->move(700,800);
    modeGroup = mode_group;
    buttonGroup = difficulty_group;
    pairGroup=pair_group;;
    easy->setChecked(true);
    single->setChecked(true);
    time_radio->setChecked(true);
	currentFrame->show();
    //!设置布局
}

/**
 * 点击GO时,根据选择设置模式和难度
 */
void MainWindow::goClicked()
{
	if(modeGroup->checkedId()==1&&pairGroup->checkedId() == 0) {
		gameState->SetMode(Mode::FAST_REACTION);
	}
    if(modeGroup->checkedId()==0&&pairGroup->checkedId() == 0) {
		gameState->SetMode(Mode::TIME_LIMIT);
	}
    if(modeGroup->checkedId()==0&&pairGroup->checkedId() == 1) {
        gameState->SetMode(Mode::TIME_LIMIT_DOUBLE);
    }
    if(modeGroup->checkedId()==1&&pairGroup->checkedId() == 1) {
        gameState->SetMode(Mode::FAST_REACTION_DOUBLE);
    }

	switch(buttonGroup->checkedId()) {
	case 0:
        boardSize = Board::kLargeSize;
		gameState->SetDifficulty(Difficulty::EASY);
		break;
	case 1:
        boardSize = Board::kMediumSize;
		gameState->SetDifficulty(Difficulty::MEDIUM);
		break;
	case 2:
        boardSize = Board::kSmallSize;
		gameState->SetDifficulty(Difficulty::HARD);
		break;
	default:
        boardSize = Board::kLargeSize;
		gameState->SetDifficulty(Difficulty::EASY);
		break;
	}

    startGame();
}

void MainWindow::exitClicked()
{
    gameState->Exit();
	// Set up initial frame
    startHome();
}

void MainWindow::pauseClicked()
{
	if(gameState->state() == GameState::PAUSE) {
		pauseButton->setText(tr("Pause"));
		gameState->Resume();
        hintButton->setEnabled(true);
	}
	else {
	    hintButton->setEnabled(false);
		pauseButton->setText(tr("Resume"));
		gameState->Pause();
	}
}

void MainWindow::updateTimeDisplay(int remain)
{
	timeDisplay->setText(QString::number(remain));
	update();
}

/**
 *交换中
 * @param direction
 */
void MainWindow::onSwap(SwapDirection direction)
{
	if(uiDrawing || gameState->state() != GameState::INGAME)
		return;

	gameState->Pause();
	pauseButton->setEnabled(false);
	hintButton->setEnabled(false);
	auto *sender = (Jewel*)this->sender();
	int x = sender->geometry().y()/50;
	int y = sender->geometry().x()/50;
	JewelPos pos(x,y);

    uiDrawing = true;
	//swap first
	if(!swapJewelInMap(x, y, direction))
		return;

	auto events = gameState->Swap(pos, direction);
	bool swaped = false;
	for(const BoardEvent& event : events) {
		swaped = true;
        drawBoardEvent(event);//
	}
	if(!swaped) {
        swapJewelInMap(x, y, direction);
		update();
	}

	gameState->Resume();
	pauseButton->setEnabled(true);
    hintButton->setEnabled(true);
    uiDrawing = false;
}
void MainWindow::onSwap2(SwapDirection direction)
{

    auto *sender = (Jewel*)this->sender();
    int x = sender->geometry().y()/50;
    int y = sender->geometry().x()/50;
    JewelPos pos(x,y);
    //swap first
    if(!swapJewelInMap2(x, y, direction))
        return;

    auto events = gameState->Swap2(pos, direction);
    bool swaped = false;
    for(const BoardEvent& event : events) {
        swaped = true;
        drawBoardEvent(event);//
    }
    if(!swaped) {
        swapJewelInMap2(x, y, direction);
        update();
    }
}
void MainWindow::gameEnd(bool highScore)
{
	timeDisplay->setText(QString::number(0));
	if(highScore) {
		QMessageBox info(this);
		info.setWindowTitle(tr("Congratulations!"));
		info.setText(tr("You have achieved a new high score!"));
		info.exec();
	}
	else {
		QMessageBox info(this);
		info.setWindowTitle(tr("Ooops!"));
		info.setText(tr("You didn't' do well this time."));
		info.exec();
	}
	pauseButton->setEnabled(false);
	hintButton->setEnabled(false);
}
/**
 * 事件的动画表示
 * @param event
 */
void MainWindow::drawBoardEvent(const BoardEvent& event)
{
	switch (event.type) {
	case BoardEvent::EventType::NEW: {
		for(JewelInfo info : event.getNewPos()) {
			Jewel *widget = map_[info.first.x][info.first.y].second;
			map_[info.first.x][info.first.y].first = static_cast<Bejeweled::Color>(info.second);
			map_[info.first.x][info.first.y].second -> SetColor(static_cast<Bejeweled::Color>(info.second));
			auto *animation = new QPropertyAnimation(widget,"geometry");
			animation->setDuration(200); // smaller than wait time
			animation->setStartValue(QRect(widget->x(),widget->y(),1,1));
			animation->setEndValue(QRect(widget->geometry())); // normal size
			animation->start();
			connect(animation,&QPropertyAnimation::finished,[=](){
				delete dynamic_cast<QPropertyAnimation*>(sender());
			});
		}
		break;
	}
	case BoardEvent::DIE: {
		for(JewelPos pos :event.getDiePos()) {
            map_[pos.x][pos.y].second->SetColor(Color::NONE);
            map_[pos.x][pos.y].first = Color::NONE;
		}

		// wait
		animationDrawing = true;
		QTimer timer;
		timer.setInterval(300);
		timer.setSingleShot(true);
		timer.start();
		connect(&timer,&QTimer::timeout,[=](){
            animationDrawing = false;
		});
		while(animationDrawing)
			QCoreApplication::processEvents(QEventLoop::AllEvents);
		break;
	}
	/**
	 * 关键:保证图像显示和内部map的一致性
	 */
	case BoardEvent::FALL: {
	    //alg
        //!动画交换
        animationDrawing = true;
        QTimer timer;
        for(pair<JewelPos,JewelPos> fallPos : event.getFallPos()) {
            const QRect tmp=map_[fallPos.first.x][fallPos.first.y].second->geometry();
            const QRect tmp2(tmp.x(),tmp.y()+tmp.width()*(fallPos.second.x-fallPos.first.x),tmp.width(),tmp.height());
            auto*animation2=new QPropertyAnimation(map_[fallPos.second.x][fallPos.second.y].second,"geometry",this);
            animation2->setDuration(300);
            animation2->setStartValue(tmp2);//新位置的
            animation2->setEndValue(QRect(tmp));
            animation2->setEasingCurve(QEasingCurve::InQuad);
            animation2->start();
            QTimer::singleShot(500, this, [=](){
                delete animation2;
            });

            auto*animation=new QPropertyAnimation(map_[fallPos.first.x][fallPos.first.y].second,"geometry",this);
            animation->setDuration(300);
            animation->setStartValue(tmp);
            animation->setEndValue(QRect(tmp2));
            animation->setEasingCurve(QEasingCurve::InQuad);
            animation->start();
            QTimer::singleShot(500, this, [=](){
                delete animation;
            });

//!逻辑交换
            map_[fallPos.second.x][fallPos.second.y].first =map_[fallPos.first.x][fallPos.first.y].first;
            Jewel*temp=map_[fallPos.first.x][fallPos.first.y].second;//temp=first;
            map_[fallPos.first.x][fallPos.first.y].second= map_[fallPos.second.x][fallPos.second.y].second;//first=second
            map_[fallPos.second.x][fallPos.second.y].second=temp;//second=temp
            map_[fallPos.first.x][fallPos.first.y].first = static_cast<Bejeweled::Color>(0);


		}
		// wait
		timer.setInterval(700);
		timer.setSingleShot(true);
		timer.start();
		connect(&timer,&QTimer::timeout,[=](){
            animationDrawing = false;
		});
		while(animationDrawing)
			QCoreApplication::processEvents(QEventLoop::AllEvents);
		break;
	}
	default:
		break;
	}
	update();
}
/**
 * 最上层的入口
 */
void MainWindow::startGame()
{
	// Set new UI frame
	delete currentFrame;

    currentFrame = new QFrame(this);

	setCentralWidget(currentFrame);
	auto *under_frame = new QFrame(currentFrame);
	auto *layout = new QGridLayout(currentFrame);
	auto *under_layout = new QGridLayout(under_frame);

	auto *board = new QLabel(currentFrame);
	board->setPixmap(QPixmap("D:/Bejeweled00/res/bg.png"));
	board->setMinimumHeight(kJewelWidgetSize*(boardSize));
	board->setMinimumWidth(kJewelWidgetSize*(boardSize));
	board->setMaximumHeight(kJewelWidgetSize*(boardSize));
	board->setMaximumWidth(kJewelWidgetSize*(boardSize));

	// set up appropriate fonts and colors
	QFont big_label_font,smaller_label_font;
	big_label_font.setPointSize(16);
	smaller_label_font.setPointSize(14);
	QPalette blue_pa,red_pa;
	blue_pa.setColor(QPalette::WindowText,Qt::blue);
	red_pa.setColor(QPalette::WindowText,Qt::red);


	auto *label1 = new QLabel(tr("Score"), under_frame);
	label1->setFont(big_label_font);
    scoreDisplay = new QLabel(under_frame);
	scoreDisplay->setFont(smaller_label_font);
	scoreDisplay->setPalette(blue_pa);

	auto *label3 = new QLabel(tr("Time Left"), under_frame);
	label3->setFont(big_label_font);
    timeDisplay =  new QLabel(under_frame);
	timeDisplay->setFont(smaller_label_font);
	timeDisplay->setPalette(red_pa);

	auto *abort_button = new JewelButton(under_frame);
	abort_button->setText(tr("Exit"));
	connect(abort_button,SIGNAL(clicked()),this,SLOT(exitClicked()));
    if(gameState->getSettings()->mode==FAST_REACTION_DOUBLE||gameState->getSettings()->mode==TIME_LIMIT_DOUBLE)
    {
        resize(1600,500);
        auto *board2 = new QLabel(currentFrame);
        board2->setPixmap(QPixmap("D:/Bejeweled00/res/bg.png"));
        board2->setMinimumHeight(kJewelWidgetSize*(boardSize));
        board2->setMinimumWidth(kJewelWidgetSize*(boardSize));
        board2->setMaximumHeight(kJewelWidgetSize*(boardSize));
        board2->setMaximumWidth(kJewelWidgetSize*(boardSize));
        for(int i=0; i != boardSize; ++i)
            for(int j=0; j != boardSize; ++j) {
                map2_[i][j].second = new Jewel(Color::NONE, board);
                map2_[i][j].second->setGeometry(kJewelWidgetSize*j,kJewelWidgetSize*i,kJewelWidgetSize,kJewelWidgetSize);
                connect(map2_[i][j].second, SIGNAL(Swap(Bejeweled::SwapDirection)), this, SLOT(
                        onSwap2(Bejeweled::SwapDirection)));
            }
        layout->addWidget(board2,0,1);
    }
    else resize(800,500);
    move(400,100);
    pauseButton = new JewelButton(under_frame);
    hintButton =new JewelButton(under_frame);
    hintButton->setText(tr("Hint"));
	pauseButton->setText(tr("Pause"));
	connect(pauseButton, SIGNAL(clicked()), this, SLOT(pauseClicked()));
    connect(hintButton, SIGNAL(clicked()), this, SLOT(hintClicked()));
	under_layout->addWidget(label1, 0, 0);
	under_layout->addWidget(scoreDisplay, 1, 0);
	under_layout->addWidget(label3, 2, 0);
	under_layout->addWidget(timeDisplay, 3, 0);
	under_layout->addWidget(hintButton,4,0);
	under_layout->addWidget(pauseButton, 5, 0);
	under_layout->addWidget(abort_button, 6, 0);
	layout->addWidget(board,0,0);
	layout->addWidget(under_frame, 1, 0);


//! /!初始化每个宝石的位置并赋予每个宝石发射信号的能力
	for(int i=0; i != boardSize; ++i)
		for(int j=0; j != boardSize; ++j) {
			map_[i][j].second = new Jewel(Color::NONE, board);
			map_[i][j].second->setGeometry(kJewelWidgetSize*j,kJewelWidgetSize*i,kJewelWidgetSize,kJewelWidgetSize);
			connect(map_[i][j].second, SIGNAL(Swap(Bejeweled::SwapDirection)), this, SLOT(
                    onSwap(Bejeweled::SwapDirection)));
		}
	currentFrame->show();
	connect(gameState, SIGNAL(Hint(Bejeweled::JewelPos)), this, SLOT(updateHint(Bejeweled::JewelPos)));
    drawBoardEvent(gameState->StartNewGame());
//    drawBoardEvent(gameState->StartNewGame2());
	connect(gameState, SIGNAL(timeTick(int)), this, SLOT(updateTimeDisplay(int)));
	connect(gameState, SIGNAL(scoreUpdated(int)), this, SLOT(updateScore(int)));

}

void MainWindow::updateScore(int newScore)
{
	scoreDisplay->setText(QString::number(newScore));
	update();
}
/**
 *交换宝石
 * @param x 宝石的横坐标
 * @param y  纵坐标
 * @param direction 交换方向
 * @return  true if success
 */
bool MainWindow::swapJewelInMap(int x, int y, SwapDirection direction)
{
	// TODO animate it
	switch (direction) {
	case DOWN: {
		if(x == boardSize - 1)
			return false;
		const pair<Bejeweled::Color,Jewel*> temp = map_[x][y];
		map_[x][y] = map_[x+1][y];
		map_[x+1][y] = temp;
		const QRect geometry_temp = map_[x+1][y].second->geometry();
		map_[x+1][y].second->setGeometry(map_[x][y].second->geometry());
		map_[x][y].second->setGeometry(geometry_temp);
		break;
	}
	case UP: {
		if(x == 0)
			return false;
		const pair<Bejeweled::Color,Jewel*> temp = map_[x][y];
		map_[x][y] = map_[x-1][y];
		map_[x-1][y] = temp;
		const QRect geometry_temp = map_[x-1][y].second->geometry();
		map_[x-1][y].second->setGeometry(map_[x][y].second->geometry());
		map_[x][y].second->setGeometry(geometry_temp);
		break;
	}
	case LEFT: {
		if(y == 0)
			return false;
		const pair<Bejeweled::Color,Jewel*> temp = map_[x][y];
		map_[x][y] = map_[x][y-1];
		map_[x][y-1] = temp;
		const QRect geometry_temp = map_[x][y-1].second->geometry();
		map_[x][y-1].second->setGeometry(map_[x][y].second->geometry());
		map_[x][y].second->setGeometry(geometry_temp);
		break;
	}
	case RIGHT: {
		if(y == boardSize - 1)
			return false;
		const pair<Bejeweled::Color,Jewel*> temp = map_[x][y];
		map_[x][y] = map_[x][y+1];
		map_[x][y+1] = temp;
		const QRect geometry_temp = map_[x][y+1].second->geometry();
		map_[x][y+1].second->setGeometry(map_[x][y].second->geometry());
		map_[x][y].second->setGeometry(geometry_temp);
		break;
	}
	default:
		return false;
		break;
	}
	// No special case happened where swap outside the board
	update();
	return true;
}
bool MainWindow::swapJewelInMap2(int x, int y, SwapDirection direction)
{
    // TODO animate it
    switch (direction) {
        case DOWN: {
            if(x == boardSize - 1)
                return false;
            const pair<Bejeweled::Color,Jewel*> temp = map2_[x][y];
            map2_[x][y] = map2_[x+1][y];
            map2_[x+1][y] = temp;
            const QRect geometry_temp = map2_[x+1][y].second->geometry();
            map2_[x+1][y].second->setGeometry(map2_[x][y].second->geometry());
            map2_[x][y].second->setGeometry(geometry_temp);
            break;
        }
        case UP: {
            if(x == 0)
                return false;
            const pair<Bejeweled::Color,Jewel*> temp = map2_[x][y];
            map2_[x][y] = map2_[x-1][y];
            map2_[x-1][y] = temp;
            const QRect geometry_temp = map2_[x-1][y].second->geometry();
            map2_[x-1][y].second->setGeometry(map2_[x][y].second->geometry());
            map2_[x][y].second->setGeometry(geometry_temp);
            break;
        }
        case LEFT: {
            if(y == 0)
                return false;
            const pair<Bejeweled::Color,Jewel*> temp = map2_[x][y];
            map2_[x][y] = map2_[x][y-1];
            map2_[x][y-1] = temp;
            const QRect geometry_temp = map2_[x][y-1].second->geometry();
            map2_[x][y-1].second->setGeometry(map2_[x][y].second->geometry());
            map2_[x][y].second->setGeometry(geometry_temp);
            break;
        }
        case RIGHT: {
            if(y == boardSize - 1)
                return false;
            const pair<Bejeweled::Color,Jewel*> temp = map2_[x][y];
            map2_[x][y] = map2_[x][y+1];
            map2_[x][y+1] = temp;
            const QRect geometry_temp = map2_[x][y+1].second->geometry();
            map2_[x][y+1].second->setGeometry(map2_[x][y].second->geometry());
            map2_[x][y].second->setGeometry(geometry_temp);
            break;
        }
        default:
            return false;
            break;
    }
    // No special case happened where swap outside the board
    update();
    return true;
}