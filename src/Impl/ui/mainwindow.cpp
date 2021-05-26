#include <QPropertyAnimation>
#include <QTimer>
#include <QMessageBox>
#include <QGridLayout>
#include <QRadioButton>
#include <QButtonGroup>
#include <QLabel>
#include <QCoreApplication>
#include <QParallelAnimationGroup>
#include <QTcpSocket>
#include <iostream>
#include <QInputDialog>
#include <QMovie>
#include "src/headers/ui/jewelbutton.h"
#include "src/headers/ui/mainwindow.h"
#include "src/headers/ui/ui_mainwindow.h"
using namespace Bejeweled;
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow),
        uiDrawing(false),
        uiDrawing2(false),
        animationDrawing2(false),
        animationDrawing(false),
        hintPos(0, 0),
        readyDouble(false),
        boardSize(1),
        startSignal("55555"),
        port(8000),
        IP(tr("127.0.0.1")),
        combo1(0),
        combo2(0) {
    ui->setupUi(this);
    setWindowIcon(QIcon(QPixmap("D:/Bejeweled00/res/green.png")));
    gameState = new GameState();
    gameState2 = new GameState();
    connect(gameState, SIGNAL(gameEnd(bool)), this, SLOT(gameEnd(bool)));
    //gameState->mainWindow purpose :游戏结束处理
    movie1=new QMovie();
    movie2=new QMovie();
    movie3=new QMovie();
    movie4=new QMovie();
    startHome();
}
MainWindow::~MainWindow() {
   if(!server) {server->close();
    server->deleteLater();}
    delete ui;
}
//todo combo动画 消除动画
/**
 * home页 todo 登陆注册
 */
void MainWindow::startHome() {

     homepage = new Homepage(this);
    QPalette palette;  //创建一个调色板的对象
    QPixmap pixmap;
    pixmap.load("D:/Bejeweled00/res/bg.png");
    palette.setBrush(backgroundRole(), QBrush(pixmap));
    setPalette(palette);
    setCentralWidget(homepage);
    homepage->show();
    QObject::connect(homepage->start_button, SIGNAL(clicked()), this, SLOT(startClicked()));
    QObject::connect(homepage->score_button, SIGNAL(clicked()), this, SLOT(scoreClicked()));
    QObject::connect(homepage->join_button , SIGNAL(clicked()), this, SLOT(joinClicked()));
//    QObject::connect(homepage->returnButton, SIGNAL(clicked()), this, SLOT(returnClicked()));
}
void MainWindow::returnClicked() {
    startHome();
}
void MainWindow::updateHint(JewelPos pos) {
    hintPos = pos;
}
void MainWindow::joinClicked() {
    bool ok;
    QString text = QInputDialog::getText(this, tr("INPUT"), tr("INPUT ROOM NUMBER"), QLineEdit::Normal, nullptr, &ok);
    if (ok && text.length() > 0 && text.length() < 6)joinRoom(text.toInt());

}
void MainWindow::registerClicked() {
    //todo by pts UI
    delete currentFrame;
    currentFrame = new QFrame(this);
    JewelButton *returnButton;
    returnButton = new JewelButton(currentFrame);
    returnButton->setText("RETURN");
    returnButton->move(500, 600);
    QObject::connect(returnButton, SIGNAL(clicked()), this, SLOT(returnClicked()));
}
void MainWindow::loginClicked() {
    delete currentFrame;
    //todo by pts UI
}
void MainWindow::hintClicked() {
    if (gameState->state() != GameState::INGAME) { return; }
    uiDrawing = true;
    pauseButton->setEnabled(false);
    hintButton->setEnabled(false);
    Jewel *widget = map_[hintPos.x][hintPos.y].second;
    int ox = widget->x(), oy = widget->y();
    // the animation
    gameState->Punish(2);
    auto *animation = new QPropertyAnimation(widget, "geometry");
    animation->setDuration(200);
    // start from a bigger Jewel
    animation->setStartValue(
            (QRect(ox - kJewelWidgetSize / 2, oy - kJewelWidgetSize / 2, kJewelWidgetSize * 2, kJewelWidgetSize * 2)));
    // resize to normal size
    animation->setEndValue((QRect(ox, oy, kJewelWidgetSize, kJewelWidgetSize)));
    animationDrawing = true;
    connect(animation, &QPropertyAnimation::finished, [=]() {
        animationDrawing = false;
    });
    animation->start();
    while (animationDrawing)
        QCoreApplication::processEvents(QEventLoop::AllEvents);
    pauseButton->setEnabled(true);
    hintButton->setEnabled(true);
    uiDrawing = false;
}
void MainWindow::startClicked() {
    homepage->hide();
    startSelect();
}
void MainWindow::scoreClicked() {
    homepage->hide();
    scorepage = new Scorepage(this,gameState);
    setCentralWidget(scorepage);
    connect(scorepage->ret_button, SIGNAL(clicked()), this, SLOT(startHome()));
}
/**
 * 进入选择游戏模式页面
 */
void MainWindow::startSelect() {
    // set up new frame

    selectpage = new Selectpage(this);
    QPalette palette;  //创建一个调色板的对象
    QPixmap pixmap;
    pixmap.load("D:/Bejeweled00/res/background3.jpg");
    palette.setBrush(backgroundRole(), QBrush(pixmap));
    setPalette(palette);
    setCentralWidget(selectpage);
    connect(selectpage->goButton, SIGNAL(clicked()), this, SLOT(goClicked()));
    selectpage->show();
    //!设置布局
}
/**
 * 根据设置开始游戏
 */
void MainWindow::goClicked() {
    selectpage->hide();
    switch (selectpage->difficultyGroup->checkedId()) {
        case 0:
            boardSize = Board::kLargeSize;
            gameState->SetDifficulty(Difficulty::EASY);
            gameState2->SetDifficulty(Difficulty::EASY);
            startSignal[0] = '1';
            break;
        case 1:
            boardSize = Board::kMediumSize;
            gameState->SetDifficulty(Difficulty::MEDIUM);
            gameState2->SetDifficulty(Difficulty::MEDIUM);
            startSignal[0] = '2';
            break;
        case 2:
            boardSize = Board::kSmallSize;
            gameState->SetDifficulty(Difficulty::HARD);
            gameState2->SetDifficulty(Difficulty::HARD);
            startSignal[0] = '3';
            break;
        default:
            boardSize = Board::kLargeSize;
            gameState->SetDifficulty(Difficulty::EASY);
            gameState2->SetDifficulty(Difficulty::EASY);
            startSignal[0] = '1';
            break;
    }
    if (selectpage->modeGroup->checkedId() == 1 && selectpage->pairGroup->checkedId() == 0) {
        gameState->SetMode(Mode::FAST_REACTION);
        startGame();
    } else if (selectpage->modeGroup->checkedId() == 0 && selectpage->pairGroup->checkedId() == 0) {
        gameState->SetMode(Mode::TIME_LIMIT);
        startGame();
    } else if (selectpage->modeGroup->checkedId() == 0 && selectpage->pairGroup->checkedId() == 1) {
        gameState->SetMode(Mode::TIME_LIMIT);
        gameState2->SetMode(Mode::TIME_LIMIT_DOUBLE);
        startSignal[1] = '1';
        roomPage();
    } else if (selectpage->modeGroup->checkedId() == 1 && selectpage->pairGroup->checkedId() == 1) {
        gameState->SetMode(Mode::FAST_REACTION);
        gameState2->SetMode(Mode::FAST_REACTION_DOUBLE);
        startSignal[1] = '2';
        roomPage();
    }

}
void MainWindow::doubleClicked() {

    if (readyDouble) {
        P1socket->write(startSignal.data());
        startGame2();
    }
    else QMessageBox::warning(this, "warning", "The other man is not readyDouble yet!");
}
//todo 传入随机端口号作为房间号,
void MainWindow::roomPage() {

    createRoom();//创建监听服务
    roompage = new Roompage(this,port);
    QPalette palette;  //创建一个调色板的对象
    QPixmap pixmap;
    pixmap.load("D:/Bejeweled00/res/bg.png");
    palette.setBrush(backgroundRole(), QBrush(pixmap));
    setPalette(palette);
    setCentralWidget(roompage);
    roompage->show();
    QObject::connect(roompage->double_go_button, SIGNAL(clicked()), this, SLOT(doubleClicked()));
}
/**
 *
 */
void MainWindow::exitClicked() {
    gameState->Exit();
    // Set up initial frame
    startHome();
    resize(800,500);
}
void MainWindow::pauseClicked() {
    if (gameState->state() == GameState::PAUSE) {
        pauseButton->setText(tr("Pause"));
        gameState->Resume();
        hintButton->setEnabled(true);
    } else {
        hintButton->setEnabled(false);
        pauseButton->setText(tr("Resume"));
        gameState->Pause();

    }
}
void MainWindow::updateTimeDisplay(int remain) {
    progressBar->setValue((120-remain)/1.2);
    update();
}
/**
 *slot  交换
 * @param direction
 */
void MainWindow::onSwap(SwapDirection direction) {
    if (uiDrawing || gameState->state() != GameState::INGAME)
        return;
    gameState->Pause();
    pauseButton->setEnabled(false);
    hintButton->setEnabled(false);
    auto *sender = (Jewel *) this->sender();
    int x = sender->geometry().y() / 50;
    int y = sender->geometry().x() / 50;
    JewelPos pos(x, y);
    uiDrawing = true;
    //swap first
    if (!swapJewelInMap(x, y, direction))
    {return;}

    auto events = gameState->Swap(pos, direction);//gameState->Swap
    string str = processStrToSend(pos, direction);
    if (readyDouble) P1socket->write(str.data());
    bool swaped = false;
    for (const BoardEvent &event : events) {
        swaped = true;
        drawBoardEvent(event);//
    }
    if (!swaped) {
        swapJewelInMap(x, y, direction);//如果此次交换无效,那么再换回来
        update();
    }

    gameState->Resume();
    pauseButton->setEnabled(true);
    hintButton->setEnabled(true);
    uiDrawing = false;
}
void MainWindow::onSwap2(int x, int y, SwapDirection direction) {
    if (uiDrawing2 || gameState2->state() != GameState::INGAME)
        return;
    JewelPos pos(x, y);//获得发送信号的宝石所在的位置
    uiDrawing2 = true;
    //swap first
    if (!swapJewelInMap2(x, y, direction))
        return;

    auto events = gameState2->Swap(pos, direction);
    bool swaped = false;
    for (const BoardEvent &event : events) {
        swaped = true;
        drawBoardEvent2(event);//
    }
    if (!swaped) {
        swapJewelInMap2(x, y, direction);
        update();
    }
    uiDrawing2 = false;
}
void MainWindow::gameEnd(bool highScore) {
    timeDisplay->setText(QString::number(0));
    if (highScore) {
        QMessageBox info(this);
        info.setWindowTitle(tr("Congratulations!"));
        info.setText(tr("You have achieved a new high score!"));
        info.exec();
    } else {
        QMessageBox info(this);
        info.setWindowTitle(tr("Ooops!"));
        info.setText(tr("You didn't do well this time."));
        info.exec();
    }
    pauseButton->setEnabled(false);
    hintButton->setEnabled(false);
    if (readyDouble) {
        readyDouble = false;
        string end = "66666";
        P1socket->write(end.data());
    }
}
/**
 * 事件的动画表示
 * @param event
 */
void MainWindow::drawBoardEvent(const BoardEvent &event) {
    switch (event.type) {
        case BoardEvent::NEW: {
            for (JewelInfo info : event.getNewPos()) {
                Jewel *widget = map_[info.first.x][info.first.y].second;
                map_[info.first.x][info.first.y].first = static_cast<Bejeweled::Color>(info.second);
                map_[info.first.x][info.first.y].second->SetColor(static_cast<Bejeweled::Color>(info.second));
                auto *animation = new QPropertyAnimation(widget, "geometry");
                animation->setDuration(200); // smaller than wait time
                animation->setStartValue(QRect(widget->x(), widget->y(), 1, 1));
                animation->setEndValue(QRect(widget->x(),widget->y(),50,50)); // normal size
                animation->start();
                connect(animation, &QPropertyAnimation::finished, [=]() {
                    delete dynamic_cast<QPropertyAnimation *>(sender());
                });
                combo1 = 0;
            }
            break;
        }
        case BoardEvent::DIE: {
            for (JewelPos pos :event.getDiePos()) {
                //todo gif:消除
                Jewel *widget = map_[pos.x][pos.y].second;
                auto *animation = new QPropertyAnimation(widget, "geometry");
                animation->setDuration(200); // smaller than wait time
                animation->setEndValue(QRect(widget->x(), widget->y(), 1, 1));
                animation->setStartValue(QRect(widget->geometry())); // normal size
                animation->start();
                connect(animation, &QPropertyAnimation::finished, [=]() {
                    delete dynamic_cast<QPropertyAnimation *>(sender());
                });
                map_[pos.x][pos.y].second->SetColor(Color::NONE);
                map_[pos.x][pos.y].first = Color::NONE;
            }
            combo1++;
            // wait
            QTimer timer;
            timer.setInterval(400);
            timer.setSingleShot(true);
            timer.start();
            connect(&timer, &QTimer::timeout, [=]() {
                animationDrawing = false;
            });
            while (animationDrawing)
                QCoreApplication::processEvents(QEventLoop::AllEvents);
            break;
        }
            /**
             * 关键:保证图像显示和内部map的一致性
             */
        case BoardEvent::FALL: {
            //!动画交换
            animationDrawing = true;
            QTimer timer;
            for (pair<JewelPos, JewelPos> fallPos : event.getFallPos()) {
                const QRect tmp = map_[fallPos.first.x][fallPos.first.y].second->geometry();
                const QRect tmp2(tmp.x(), tmp.y() + tmp.width() * (fallPos.second.x - fallPos.first.x), tmp.width(),
                                 tmp.height());
                auto *animation2 = new QPropertyAnimation(map_[fallPos.second.x][fallPos.second.y].second, "geometry",
                                                          this);
                animation2->setDuration(300);
                animation2->setStartValue(tmp2);//新位置的
                animation2->setEndValue(QRect(tmp));
                animation2->setEasingCurve(QEasingCurve::InOutCubic);
                animation2->start();
                QTimer::singleShot(400, this, [=]() {
                    delete animation2;
                });

                auto *animation = new QPropertyAnimation(map_[fallPos.first.x][fallPos.first.y].second, "geometry",
                                                         this);
                animation->setDuration(300);
                animation->setStartValue(tmp);
                animation->setEndValue(QRect(tmp2));
                animation->setEasingCurve(QEasingCurve::InOutCubic);
                animation->start();
                QTimer::singleShot(400, this, [=]() {
                    delete animation;
                });

//!逻辑交换
                map_[fallPos.second.x][fallPos.second.y].first = map_[fallPos.first.x][fallPos.first.y].first;
                Jewel *temp = map_[fallPos.first.x][fallPos.first.y].second;//temp=first;
                map_[fallPos.first.x][fallPos.first.y].second = map_[fallPos.second.x][fallPos.second.y].second;//first=second
                map_[fallPos.second.x][fallPos.second.y].second = temp;//second=temp
                map_[fallPos.first.x][fallPos.first.y].first = static_cast<Bejeweled::Color>(0);
            }
            // wait
            timer.setInterval(800);
            timer.setSingleShot(true);
            timer.start();
            connect(&timer, &QTimer::timeout, [=]() {
                animationDrawing = false;
            });
            while (animationDrawing)
                QCoreApplication::processEvents(QEventLoop::AllEvents);
            break;
        }
        default:
            break;
    }
    update();
}
void MainWindow::drawBoardEvent2(const BoardEvent &event) {
    switch (event.type) {
        case BoardEvent::NEW: {
            for (JewelInfo info : event.getNewPos()) {
                Jewel *widget = map2_[info.first.x][info.first.y].second;
                map2_[info.first.x][info.first.y].first = static_cast<Color>(info.second);
                map2_[info.first.x][info.first.y].second->SetColor(static_cast<Color>(info.second));
                auto *animation = new QPropertyAnimation(widget, "geometry");
                animation->setDuration(200); // smaller than wait time
                animation->setStartValue(QRect(widget->x(), widget->y(), 1, 1));
                animation->setEndValue(QRect(widget->geometry())); // normal size
                animation->start();
                connect(animation, &QPropertyAnimation::finished, [=]() {
                    delete dynamic_cast<QPropertyAnimation *>(sender());
                });
            }
            break;
        }
        case BoardEvent::DIE: {
            for (JewelPos pos :event.getDiePos()) {
                map2_[pos.x][pos.y].second->SetColor(Color::NONE);
                map2_[pos.x][pos.y].first = Color::NONE;
            }

            // wait
            animationDrawing2 = true;
            QTimer timer;
            timer.setInterval(300);
            timer.setSingleShot(true);
            timer.start();
            connect(&timer, &QTimer::timeout, [=]() {
                animationDrawing2 = false;
            });
            while (animationDrawing2)
                QCoreApplication::processEvents(QEventLoop::AllEvents);
            break;
        }
            /**
             * 关键:保证图像显示和内部map的一致性
             */
        case BoardEvent::FALL: {
            //!动画交换
            animationDrawing2 = true;
            QTimer timer;
            for (pair<JewelPos, JewelPos> fallPos : event.getFallPos()) {
                const QRect tmp = map2_[fallPos.first.x][fallPos.first.y].second->geometry();
                const QRect tmp2(tmp.x(), tmp.y() + tmp.width() * (fallPos.second.x - fallPos.first.x), tmp.width(),
                                 tmp.height());
                auto *animation2 = new QPropertyAnimation(map2_[fallPos.second.x][fallPos.second.y].second, "geometry",this);
                animation2->setDuration(300);
                animation2->setStartValue(tmp2);//新位置的
                animation2->setEndValue(QRect(tmp));
                animation2->setEasingCurve(QEasingCurve::InQuad);
                animation2->start();
                QTimer::singleShot(400, this, [=]() {
                    delete animation2;
                });

                auto *animation = new QPropertyAnimation(map2_[fallPos.first.x][fallPos.first.y].second, "geometry",
                                                         this);
                animation->setDuration(300);
                animation->setStartValue(tmp);
                animation->setEndValue(QRect(tmp2));
                animation->setEasingCurve(QEasingCurve::InQuad);
                animation->start();
                QTimer::singleShot(400, this, [=]() {
                    delete animation;
                });

//!逻辑交换
                map2_[fallPos.second.x][fallPos.second.y].first = map2_[fallPos.first.x][fallPos.first.y].first;
                Jewel *temp = map2_[fallPos.first.x][fallPos.first.y].second;//temp=first;
                map2_[fallPos.first.x][fallPos.first.y].second = map2_[fallPos.second.x][fallPos.second.y].second;//first=second
                map2_[fallPos.second.x][fallPos.second.y].second = temp;//second=temp
                map2_[fallPos.first.x][fallPos.first.y].first = static_cast<Bejeweled::Color>(0);


            }
            // wait
            timer.setInterval(800);
            timer.setSingleShot(true);
            timer.start();
            connect(&timer, &QTimer::timeout, [=]() {
                animationDrawing2 = false;
            });
            while (animationDrawing2)
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
void MainWindow::startGame() {
    // Set new UI frame
    currentFrame = new QFrame(this);
    setCentralWidget(currentFrame);
    auto *under_frame = new QFrame(currentFrame);
    auto *layout = new QGridLayout(currentFrame);
    auto *under_layout = new QGridLayout(under_frame);
    auto *board = new QLabel(currentFrame);
    board->setPixmap(QPixmap("D:/Bejeweled00/res/bg.png"));
    board->setMinimumHeight(kJewelWidgetSize * (boardSize));
    board->setMinimumWidth(kJewelWidgetSize * (boardSize));
    board->setMaximumHeight(kJewelWidgetSize * (boardSize));
    board->setMaximumWidth(kJewelWidgetSize * (boardSize));
    // set up appropriate fonts and colors
    QFont big_label_font, smaller_label_font;
    big_label_font.setPointSize(16);
    smaller_label_font.setPointSize(14);
    QPalette blue_pa, red_pa;
    blue_pa.setColor(QPalette::WindowText, Qt::blue);
    red_pa.setColor(QPalette::WindowText, Qt::red);
    auto *label1 = new QLabel(tr("Score"), under_frame);
    label1->setFont(big_label_font);
    scoreDisplay = new QLabel(under_frame);
    scoreDisplay->setFont(smaller_label_font);
    scoreDisplay->setPalette(blue_pa);
    auto *label3 = new QLabel(tr("Time Used"), under_frame);
    label3->setFont(big_label_font);
    timeDisplay = new QLabel(under_frame);
    timeDisplay->setFont(smaller_label_font);
    timeDisplay->setPalette(red_pa);
    progressBar=new QProgressBar(this);
    int len = this->width()/100*6;
    progressBar->setGeometry(this->width()/20*7 + 20, this->height() - len, 10*len - 40, len*7/10 - 15);
    progressBar->setRange(0,99);
    progressBar->setValue(0);
    progressBar->setFormat("%p%");
    progressBar->setAlignment(Qt::AlignCenter);
    progressBar->setFont(QFont("Microsoft YaHei", 12, 100));
    progressBar->setStyleSheet("QProgressBar{color:grey;} QProgressBar::chunk{background-color:#24247e}");
    auto *abort_button = new JewelButton(under_frame);
    abort_button->setText(tr("Exit"));
    connect(abort_button, SIGNAL(clicked()), this, SLOT(exitClicked()));
    resize(800, 500);
    move(400, 100);
    pauseButton = new JewelButton(under_frame);
    hintButton = new JewelButton(under_frame);
    hintButton->setText(tr("Hint"));
    pauseButton->setText(tr("Pause"));
    connect(pauseButton, SIGNAL(clicked()), this, SLOT(pauseClicked()));
    connect(hintButton, SIGNAL(clicked()), this, SLOT(hintClicked()));
    under_layout->addWidget(label1, 0, 1);
    under_layout->addWidget(scoreDisplay, 1, 1);
    under_layout->addWidget(label3, 2, 0);
    under_layout->addWidget(progressBar,3,0);
    under_layout->addWidget(hintButton, 4, 0);
    under_layout->addWidget(pauseButton, 5, 0);
    under_layout->addWidget(abort_button, 6, 0);
    layout->addWidget(board, 0, 0);
    layout->addWidget(under_frame, 1, 0);
//! /!初始化每个宝石的位置并赋予每个宝石发射信号的能力
    for (int i = 0; i != boardSize; ++i)
        for (int j = 0; j != boardSize; ++j) {
            map_[i][j].second = new Jewel(Color::NONE, board);
            map_[i][j].second->setGeometry(kJewelWidgetSize * j, kJewelWidgetSize * i, kJewelWidgetSize,
                                           kJewelWidgetSize);
            connect(map_[i][j].second, SIGNAL(Swap(Bejeweled::SwapDirection)), this, SLOT(
                    onSwap(Bejeweled::SwapDirection)));
        }
    currentFrame->show();
    drawBoardEvent(gameState->StartNewGame(rand()));
//!连接信号和槽
    connect(gameState, SIGNAL(Hint(Bejeweled::JewelPos)), this, SLOT(updateHint(Bejeweled::JewelPos)));
    //game emit->gameState->mainWindow , purpose:更新可行的宝石位置updateHint()
    connect(gameState, SIGNAL(timeTick(int)), this, SLOT(updateTimeDisplay(int)));
    //gameState->mainWindow , purpose:更新计时器QLabel updateTimeDisplay()
    connect(gameState, SIGNAL(scoreUpdated(int)), this, SLOT(updateScore(int)));
    //gameState->mainWindow, purpose:更新成绩QLabel updateScore()
//!连接信号和槽

}

/**
 * 双人模式入口
 */
void MainWindow::startGame2() {
    // Set new UI frame
    delete currentFrame;
    readyDouble = true;
    currentFrame = new QFrame(this);
    setCentralWidget(currentFrame);
    auto *under_frame = new QFrame(currentFrame);
    auto *under_frame2 = new QFrame(currentFrame);
    auto *layout = new QGridLayout(currentFrame);
    auto *under_layout = new QGridLayout(under_frame);
    auto *under_layout2 = new QGridLayout(under_frame2);
    auto *board = new QLabel(currentFrame);
    board->setPixmap(QPixmap("D:/Bejeweled00/res/bg.png"));
    board->setMinimumHeight(kJewelWidgetSize * (boardSize));
    board->setMinimumWidth(kJewelWidgetSize * (boardSize));
    board->setMaximumHeight(kJewelWidgetSize * (boardSize));
    board->setMaximumWidth(kJewelWidgetSize * (boardSize));
    auto *board2 = new QLabel(currentFrame);
    board2->setPixmap(QPixmap("D:/Bejeweled00/res/bg.png"));
    board2->setMinimumHeight(kJewelWidgetSize * (boardSize));
    board2->setMinimumWidth(kJewelWidgetSize * (boardSize));
    board2->setMaximumHeight(kJewelWidgetSize * (boardSize));
    board2->setMaximumWidth(kJewelWidgetSize * (boardSize));
    // set up appropriate fonts and colors
    QFont big_label_font, smaller_label_font;
    big_label_font.setPointSize(16);
    smaller_label_font.setPointSize(14);
    QPalette blue_pa, red_pa;
    blue_pa.setColor(QPalette::WindowText, Qt::blue);
    red_pa.setColor(QPalette::WindowText, Qt::red);
    scoreDisplay2 = new QLabel(under_frame2);
    scoreDisplay2->setFont(smaller_label_font);
    scoreDisplay2->setPalette(blue_pa);
    auto *label1 = new QLabel(tr("Score"), under_frame);
    label1->setFont(big_label_font);
    scoreDisplay = new QLabel(under_frame);
    scoreDisplay->setFont(smaller_label_font);
    scoreDisplay->setPalette(blue_pa);
    auto *label3 = new QLabel(tr("Time Used"), under_frame);
    label3->setFont(big_label_font);
    timeDisplay = new QLabel(under_frame);
    timeDisplay->setFont(smaller_label_font);
    timeDisplay->setPalette(red_pa);
    auto *abort_button = new JewelButton(under_frame);
    abort_button->setText(tr("Exit"));
    connect(abort_button, SIGNAL(clicked()), this, SLOT(exitClicked()));
    resize(1600, 500);
    //! /!初始化每个宝石的位置并赋予每个宝石发射信号的能力
    for (int i = 0; i != boardSize; ++i)
        for (int j = 0; j != boardSize; ++j) {
            map_[i][j].second = new Jewel(Color::NONE, board);
            map_[i][j].second->setGeometry(kJewelWidgetSize * j, kJewelWidgetSize * i, kJewelWidgetSize,
                                           kJewelWidgetSize);
            connect(map_[i][j].second, SIGNAL(Swap(Bejeweled::SwapDirection)), this, SLOT(
                    onSwap(Bejeweled::SwapDirection)));
            map2_[i][j].second = new Jewel(Color::NONE, board2);
            map2_[i][j].second->setGeometry(kJewelWidgetSize * j, kJewelWidgetSize * i, kJewelWidgetSize,
                                            kJewelWidgetSize);//P2不需发射信号
        }


    move(400, 100);
    pauseButton = new JewelButton(under_frame);
    hintButton = new JewelButton(under_frame);
    hintButton->setText(tr("Hint"));
    pauseButton->setText(tr("Pause"));
    connect(pauseButton, SIGNAL(clicked()), this, SLOT(pauseClicked()));
    connect(hintButton, SIGNAL(clicked()), this, SLOT(hintClicked()));
    progressBar=new QProgressBar(this);
    int len = this->width()/100*6;
    progressBar->setGeometry(this->width()/20*7 + 20, this->height() - len, 10*len - 40, len*7/10 - 15);
    progressBar->setRange(0,99);
    progressBar->setValue(0);
    progressBar->setFormat("%p%");
    progressBar->setAlignment(Qt::AlignCenter);
    progressBar->setFont(QFont("Microsoft YaHei", 12, 100));
    progressBar->setStyleSheet("QProgressBar{color:grey;} QProgressBar::chunk{background-color:#24247e}");
    under_layout->addWidget(label1, 0, 0);
    under_layout->addWidget(scoreDisplay, 1, 0);
    under_layout->addWidget(label3, 2, 0);
    under_layout->addWidget(progressBar, 3, 0);
    under_layout->addWidget(hintButton, 4, 0);
    under_layout->addWidget(abort_button, 6, 0);

    under_layout2->addWidget(label1, 0, 0);
    under_layout2->addWidget(scoreDisplay2, 1, 0);

    layout->addWidget(board, 0, 0);
    layout->addWidget(board2, 0, 1);
    layout->addWidget(under_frame, 1, 0);
    layout->addWidget(under_frame2, 1, 1);
    currentFrame->show();
    int seed = rand();
    drawBoardEvent(gameState->StartNewGame(seed));
    drawBoardEvent2(gameState2->StartNewGame(seed));

//!连接信号和槽
    connect(gameState, SIGNAL(Hint(Bejeweled::JewelPos)), this, SLOT(updateHint(Bejeweled::JewelPos)));
    //game emit->gameState->mainWindow , purpose:更新可行的宝石位置updateHint()
    connect(gameState, SIGNAL(timeTick(int)), this, SLOT(updateTimeDisplay(int)));
    //gameState->mainWindow , purpose:更新计时器QLabel updateTimeDisplay()
    connect(gameState, SIGNAL(scoreUpdated(int)), this, SLOT(updateScore(int)));
    //gameState->mainWindow, purpose:更新成绩QLabel updateScore()
    connect(gameState2, SIGNAL(scoreUpdated(int)), this, SLOT(updateScore2(int)));
//!连接信号和槽

}

//!网络QT通信
/**创建房间
 *
 */
void MainWindow::createRoom() {
    server = new QTcpServer();
//   port=rand()%65535
    port = 8000;//todo:random
    if (!server->listen(QHostAddress::Any, port)) {//监听开始
        qDebug() << server->errorString();
        QMessageBox::warning(this, "warning", "defeat in building the server!");
        port = -1;
        return;
    };
    connect(server, &QTcpServer::newConnection, this, &MainWindow::serverNewConnect);
}
void MainWindow::joinRoom(int roomNumber) {
    //todo by pts roompage
    P1socket = new QTcpSocket();
    QObject::connect(P1socket, &QTcpSocket::readyRead, this, &MainWindow::socketReadData);
    QObject::connect(P1socket, &QTcpSocket::disconnected, this, &MainWindow::socketDisconnected);
//    QString IP = tr("127.0.0.1");//todo:设置局域网地址
    port = roomNumber;
    P1socket->abort();
    P1socket->connectToHost(IP, port);
    if (!P1socket->waitForConnected(3000)) { cout << "JOIN DEFEAT"; }
    else { QMessageBox::warning(this, "warning", "JOIN SUCCESS!"); }

}
/**
 * 读取传输的数据
 */
void MainWindow::socketReadData() {
    QByteArray buffer;
    buffer = P1socket->readAll();
    if (!buffer.isEmpty()) {
        string s = tr(buffer).toStdString();
        processReadStr(s);
    }
}
/**
 * 处理收到的数据
 * @param s
 */
void MainWindow::processReadStr(string s) {
    int x = (s[0] - '0') * 10 + (s[1] - '0');
    int y = (s[2] - '0') * 10 + (s[3] - '0');
    Bejeweled::SwapDirection direction;
    switch (s[4]) {
        case '1':
            direction = LEFT;
            break;
        case '2':
            direction = RIGHT;
            break;
        case '3':
            direction = UP;
            break;
        case '4':
            direction = DOWN;
            break;
        case '5':
            switch (s[0]) {
                case '1':
                    gameState->SetDifficulty(EASY);
                    gameState2->SetDifficulty(EASY);
                    boardSize = Board::kLargeSize;
                    break;
                case '2':
                    gameState->SetDifficulty(MEDIUM);
                    gameState2->SetDifficulty(MEDIUM);
                    boardSize = Board::kMediumSize;
                    break;
                case '3':
                    gameState->SetDifficulty(HARD);
                    gameState2->SetDifficulty(HARD);
                    boardSize = Board::kSmallSize;
                    break;
            }
            switch (s[1]) {
                case '1':
                    gameState2->SetMode(TIME_LIMIT_DOUBLE);
                    gameState->SetMode(TIME_LIMIT);
                    break;
                case '2':
                    gameState2->SetMode(FAST_REACTION_DOUBLE);
                    gameState->SetMode(FAST_REACTION);
                    break;
            }
            startGame2();
            return;
        case '6':
            scoreDisplay2->setText("DEATH IN " + scoreDisplay2->text());
            gameState2->Exit();
            readyDouble = false;
            return;
    }
    onSwap2(x, y, direction);
};
/**
 * 处理将要发送的数据
 */
string MainWindow::processStrToSend(JewelPos pos, SwapDirection direction) {
    int x = pos.x, y = pos.y;
    string str;
    if (x >= 10)str += to_string(x);
    else str += "0" + to_string(x);
    if (y >= 10)str += to_string(y);
    else str += "0" + to_string(y);
    switch (direction) {
        case UP:
            str += to_string(3);
            break;
        case RIGHT:
            str += to_string(2);
            break;
        case DOWN:
            str += to_string(4);
            break;
        case LEFT:
            str += to_string(1);
            break;
    }
    return str;
}
void MainWindow::socketDisconnected() {
    QMessageBox::warning(this, "warning", "The other man has disconnected!");
}
/**
 * 被连接
 */
void MainWindow::serverNewConnect() {

    QMessageBox::warning(this, "warning", "The other man is already readyDouble!please start the game!");
    readyDouble = true;
    P1socket = server->nextPendingConnection();
    QObject::connect(P1socket, &QTcpSocket::readyRead, this, &MainWindow::socketReadData);
}
/**
 * 加入房间
 */
/**
 * slot
 * @param newScore
 */
void MainWindow::updateScore(int newScore) {
    scoreDisplay->setText(QString::number(newScore));
    update();
}
void MainWindow::updateScore2(int newScore) {
    scoreDisplay2->setText(QString::number(newScore));
    update();
}
/**
 *数据结构上交换宝石
 * @param x 宝石的横坐标
 * @param y  纵坐标
 * @param direction 交换方向
 * @return  TRUE if success
 */
bool MainWindow::swapJewelInMap(int x, int y, SwapDirection direction) {
    // TODO animate it
    switch (direction) {
        case DOWN: {
            if (x >= boardSize-1)
                return false;
            const QRect tmp = map_[x][y].second->geometry();
            const QRect tmp2 = map_[x + 1][y].second->geometry();
            auto *animation2 = new QPropertyAnimation(map_[x + 1][y].second, "geometry",
                                                      this);
            animationDrawing = true;
            auto *animation = new QPropertyAnimation(map_[x][y].second, "geometry",this);
            animation->setDuration(300);
            animation->setStartValue(tmp);
            animation->setEndValue(QRect(tmp2));
            animation->setEasingCurve(QEasingCurve::InQuad);
            animation->start();
            QTimer::singleShot(400, this, [=]() {
                delete animation;
            });
            animation2->setDuration(300);
            animation2->setStartValue(tmp2);//新位置的
            animation2->setEndValue(QRect(tmp));
            animation2->setEasingCurve(QEasingCurve::InQuad);
            animation2->start();
            QTimer::singleShot(400, this, [=]() {
                delete animation2;
            });
            QTimer timer;
            timer.setInterval(800);
            timer.setSingleShot(true);
            timer.start();
            connect(&timer, &QTimer::timeout, [=]() {
                animationDrawing = false;
            });
            while (animationDrawing)
                QCoreApplication::processEvents(QEventLoop::AllEvents);
            Color tmpColor=map_[x+1][y].first;
            map_[x+1][y].first = map_[x][y].first;
            Jewel *temp = map_[x][y].second;//temp=first;
            map_[x][y].second = map_[x+1][y].second;//first=second
            map_[x+1][y].second = temp;//second=temp
            map_[x][y].first = tmpColor;
            break;
        }
        case UP: {
            if (x <= 0)
                return false;
            const QRect tmp = map_[x][y].second->geometry();
            const QRect tmp2 = map_[x - 1][y].second->geometry();
            auto *animation2 = new QPropertyAnimation(map_[x - 1][y].second, "geometry",
                                                      this);
            animationDrawing= true;
            animation2->setDuration(300);
            animation2->setStartValue(tmp2);//新位置的
            animation2->setEndValue(QRect(tmp));
            animation2->setEasingCurve(QEasingCurve::InQuad);
            animation2->start();
            QTimer::singleShot(400, this, [=]() {
                delete animation2;
            });
            auto *animation = new QPropertyAnimation(map_[x][y].second, "geometry",
                                                     this);
            animation->setDuration(300);
            animation->setStartValue(tmp);
            animation->setEndValue(QRect(tmp2));
            animation->setEasingCurve(QEasingCurve::InQuad);
            animation->start();
            QTimer::singleShot(400, this, [=]() {
                delete animation;
            });
            QTimer timer;
            timer.setInterval(800);
            timer.setSingleShot(true);
            timer.start();
            connect(&timer, &QTimer::timeout, [=]() {
                animationDrawing = false;
            });
            while (animationDrawing)
                QCoreApplication::processEvents(QEventLoop::AllEvents);
            Color tmpColor=map_[x-1][y].first;
            map_[x-1][y].first = map_[x][y].first;
            Jewel *temp = map_[x][y].second;//temp=first;
            map_[x][y].second = map_[x-1][y].second;//first=second
            map_[x-1][y].second = temp;//second=temp
            map_[x][y].first = tmpColor;
            break;
        }
        case LEFT: {
            if (y <= 0)
                return false;
            const QRect tmp = map_[x][y].second->geometry();
            const QRect tmp2 = map_[x][y - 1].second->geometry();
            animationDrawing=true;
            auto *animation = new QPropertyAnimation(map_[x][y].second, "geometry",
                                                     this);
            animation->setDuration(300);
            animation->setStartValue(tmp);
            animation->setEndValue(QRect(tmp2));
            animation->setEasingCurve(QEasingCurve::InQuad);
            animation->start();
            QTimer::singleShot(400, this, [=]() {
                delete animation;
            });
            auto *animation2 = new QPropertyAnimation(map_[x][y - 1].second, "geometry",
                                                      this);
            animation2->setDuration(300);
            animation2->setStartValue(tmp2);//新位置的
            animation2->setEndValue(QRect(tmp));
            animation2->setEasingCurve(QEasingCurve::InQuad);
            animation2->start();
            QTimer::singleShot(400, this, [=]() {
                delete animation2;
            });
            QTimer timer;
            timer.setInterval(800);
            timer.setSingleShot(true);
            timer.start();
            connect(&timer, &QTimer::timeout, [=]() {
                animationDrawing = false;
            });
            while (animationDrawing)
                QCoreApplication::processEvents(QEventLoop::AllEvents);
            Color tmpColor=map_[x][y-1].first;
            map_[x][y-1].first = map_[x][y].first;
            Jewel *temp = map_[x][y].second;//temp=first;
            map_[x][y].second = map_[x][y-1].second;//first=second
            map_[x][y-1].second = temp;//second=temp
            map_[x][y].first = tmpColor;
            break;
        }
        case RIGHT: {
            if (y >= boardSize - 1)
                return false;
            const QRect tmp = map_[x][y].second->geometry();
            const QRect tmp2 = map_[x][y + 1].second->geometry();
            auto *animation2 = new QPropertyAnimation(map_[x][y + 1].second, "geometry",
                                                      this);
            animationDrawing=true;
            animation2->setDuration(300);
            animation2->setStartValue(tmp2);//新位置的
            animation2->setEndValue(QRect(tmp));
            animation2->setEasingCurve(QEasingCurve::InQuad);
            animation2->start();
            QTimer::singleShot(400, this, [=]() {
                delete animation2;
            });

            auto *animation = new QPropertyAnimation(map_[x][y].second, "geometry",
                                                     this);
            animation->setDuration(300);
            animation->setStartValue(tmp);
            animation->setEndValue(QRect(tmp2));
            animation->setEasingCurve(QEasingCurve::InQuad);
            animation->start();
            QTimer::singleShot(400, this, [=]() {
                delete animation;
            });
            QTimer timer;
            timer.setInterval(800);
            timer.setSingleShot(true);
            timer.start();
            connect(&timer, &QTimer::timeout, [=]() {
                animationDrawing = false;
            });
            while (animationDrawing)
                QCoreApplication::processEvents(QEventLoop::AllEvents);
            Color tmpColor=map_[x][y+1].first;
            map_[x][y+1].first = map_[x][y].first;
            Jewel *temp = map_[x][y].second;//temp=first;
            map_[x][y].second = map_[x][y+1].second;//first=second
            map_[x][y+1].second = temp;//second=temp
            map_[x][y].first = tmpColor;
            break;
        }
        default:
            return false;
            break;
    }
    update();
    return true;
}
bool MainWindow::swapJewelInMap2(int x, int y, SwapDirection direction) {
    // TODO animate it
    switch (direction) {
        case DOWN: {
            if(x>=boardSize-1)
                return false;
            const QRect tmp = map2_[x][y].second->geometry();
            const QRect tmp2 = map2_[x + 1][y].second->geometry();
            auto *animation2 = new QPropertyAnimation(map2_[x + 1][y].second, "geometry",
                                                      this);
            animationDrawing = true;

            auto *animation = new QPropertyAnimation(map2_[x][y].second, "geometry",
                                                     this);
            animation->setDuration(300);
            animation->setStartValue(tmp);
            animation->setEndValue(QRect(tmp2));
            animation->setEasingCurve(QEasingCurve::InQuad);
            animation->start();
            QTimer::singleShot(400, this, [=]() {
                delete animation;
            });
            animation2->setDuration(300);
            animation2->setStartValue(tmp2);//新位置的
            animation2->setEndValue(QRect(tmp));
            animation2->setEasingCurve(QEasingCurve::InQuad);
            animation2->start();
            QTimer::singleShot(400, this, [=]() {
                delete animation2;
            });
            QTimer timer;
            timer.setInterval(800);
            timer.setSingleShot(true);
            timer.start();
            connect(&timer, &QTimer::timeout, [=]() {
                animationDrawing = false;
            });
            Color tmpColor=map2_[x+1][y].first;
            map2_[x+1][y].first = map2_[x][y].first;
            Jewel *temp = map2_[x][y].second;//temp=first;
            map2_[x][y].second = map2_[x+1][y].second;//first=second
            map2_[x+1][y].second = temp;//second=temp
            map2_[x][y].first = tmpColor;
            while (animationDrawing)
                QCoreApplication::processEvents(QEventLoop::AllEvents);
            break;
        }
        case UP: {
            if (x <= 0)
                return false;
            const QRect tmp = map2_[x][y].second->geometry();
            const QRect tmp2 = map2_[x - 1][y].second->geometry();
            auto *animation2 = new QPropertyAnimation(map2_[x - 1][y].second, "geometry",this);
            animationDrawing= true;
            animation2->setDuration(300);
            animation2->setStartValue(tmp2);//新位置的
            animation2->setEndValue(QRect(tmp));
            animation2->setEasingCurve(QEasingCurve::InQuad);
            animation2->start();
            QTimer::singleShot(400, this, [=]() {
                delete animation2;
            });
            auto *animation = new QPropertyAnimation(map2_[x][y].second, "geometry",this);
            animation->setDuration(300);
            animation->setStartValue(tmp);
            animation->setEndValue(QRect(tmp2));
            animation->setEasingCurve(QEasingCurve::InQuad);
            animation->start();
            QTimer::singleShot(400, this, [=]() {
                delete animation;
            });
            QTimer timer;
            timer.setInterval(800);
            timer.setSingleShot(true);
            timer.start();
            connect(&timer, &QTimer::timeout, [=]() {
                animationDrawing = false;
            });
            while (animationDrawing)
                QCoreApplication::processEvents(QEventLoop::AllEvents);
            Color tmpColor=map2_[x-1][y].first;
            map2_[x-1][y].first = map2_[x][y].first;
            Jewel *temp = map2_[x][y].second;//temp=first;
            map2_[x][y].second = map2_[x-1][y].second;//first=second
            map2_[x-1][y].second = temp;//second=temp
            map2_[x][y].first = tmpColor;
            break;
        }
        case LEFT: {
            if (y <= 0)
                return false;
            const QRect tmp = map2_[x][y].second->geometry();
            const QRect tmp2 = map2_[x][y - 1].second->geometry();
            animationDrawing=true;
            auto *animation = new QPropertyAnimation(map2_[x][y].second, "geometry",
                                                     this);
            animation->setDuration(300);
            animation->setStartValue(tmp);
            animation->setEndValue(QRect(tmp2));
            animation->setEasingCurve(QEasingCurve::InQuad);
            animation->start();
            QTimer::singleShot(400, this, [=]() {
                delete animation;
            });
            auto *animation2 = new QPropertyAnimation(map2_[x][y - 1].second, "geometry",
                                                      this);
            animation2->setDuration(300);
            animation2->setStartValue(tmp2);//新位置的
            animation2->setEndValue(QRect(tmp));
            animation2->setEasingCurve(QEasingCurve::InQuad);
            animation2->start();
            QTimer::singleShot(400, this, [=]() {
                delete animation2;
            });
            QTimer timer;
            timer.setInterval(800);
            timer.setSingleShot(true);
            timer.start();
            connect(&timer, &QTimer::timeout, [=]() {
                animationDrawing = false;
            });
            while (animationDrawing)
                QCoreApplication::processEvents(QEventLoop::AllEvents);
            Color tmpColor=map2_[x][y-1].first;
            map2_[x][y-1].first = map2_[x][y].first;
            Jewel *temp = map2_[x][y].second;//temp=first;
            map2_[x][y].second = map2_[x][y-1].second;//first=second
            map2_[x][y-1].second = temp;//second=temp
            map2_[x][y].first = tmpColor;
            break;
        }
        case RIGHT: {
            if (y >= boardSize - 1)
                return false;
            const QRect tmp = map2_[x][y].second->geometry();
            const QRect tmp2 = map2_[x][y + 1].second->geometry();
            auto *animation2 = new QPropertyAnimation(map2_[x][y + 1].second, "geometry",
                                                      this);
            animationDrawing=true;
            animation2->setDuration(300);
            animation2->setStartValue(tmp2);//新位置的
            animation2->setEndValue(QRect(tmp));
            animation2->setEasingCurve(QEasingCurve::InQuad);
            animation2->start();
            QTimer::singleShot(400, this, [=]() {
                delete animation2;
            });
            auto *animation = new QPropertyAnimation(map2_[x][y].second, "geometry",this);
            animation->setDuration(300);
            animation->setStartValue(tmp);
            animation->setEndValue(QRect(tmp2));
            animation->setEasingCurve(QEasingCurve::InQuad);
            animation->start();
            QTimer::singleShot(400, this, [=]() {
                delete animation;
            });
            QTimer timer;
            timer.setInterval(800);
            timer.setSingleShot(true);
            timer.start();
            connect(&timer, &QTimer::timeout, [=]() {
                animationDrawing = false;
            });
            while (animationDrawing)
                QCoreApplication::processEvents(QEventLoop::AllEvents);
            Color tmpColor=map2_[x][y+1].first;
            map2_[x][y+1].first = map2_[x][y].first;
            Jewel *temp = map2_[x][y].second;//temp=first;
            map2_[x][y].second = map2_[x][y+1].second;//first=second
            map2_[x][y+1].second = temp;//second=temp
            map2_[x][y].first = tmpColor;
            break;
        }
        default:
            return false;
            break;
    }
    update();
    return true;
}