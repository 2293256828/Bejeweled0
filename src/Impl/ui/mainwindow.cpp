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
#include "QMediaPlaylist"
#include "src/headers/ui/jewelbutton.h"
#include "src/headers/ui/mainwindow.h"
#include "src/headers/ui/ui_mainwindow.h"

#include "stdlib.h"

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
        combo2(0),
        death1(false),
        death2(false),
        sound_effect2(true),
        sound_effect1(true),
        bgMusic(true),
        gameBgMusic(true){
    bgplayer = new QMediaPlayer();
    ui->setupUi(this);
    setWindowIcon(QIcon(QPixmap("D:/Bejeweled00/res/green.png")));
    gameState = new GameState();
    gameState2 = new GameState();
    connect(gameState, SIGNAL(gameEnd(bool)), this, SLOT(gameEnd(bool)));
    //gameState->mainWindow purpose :游戏结束处理
    movie1 = new QMovie();
    movie2 = new QMovie();
    movie3 = new QMovie();
    movie4 = new QMovie();
    timeUpSound = new QMediaPlayer();
    timeUpSound->setMedia(QUrl::fromLocalFile("D:\\Bejeweled00\\res\\sound_effect\\voice_timeup.wav"));
    gameBgPlayer = new QMediaPlayer();
    gameBgPlayer->setMedia(QUrl::fromLocalFile("D:\\Bejeweled00\\res\\music\\Classic.mp3"));
    goSound = new QMediaPlayer();
    goSound->setMedia(QUrl::fromLocalFile("D:\\Bejeweled00\\res\\sound_effect\\voice_go.wav"));

    goodSound = new QMediaPlayer();
    goodSound->setMedia(QUrl::fromLocalFile("D:\\Bejeweled00\\res\\sound_effect\\voice_good.wav"));
    unbelievableSound = new QMediaPlayer();
    unbelievableSound->setMedia(QUrl::fromLocalFile("D:\\Bejeweled00\\res\\sound_effect\\voice_unbelievable.wav"));
    excellentSound = new QMediaPlayer();
    excellentSound->setMedia(QUrl::fromLocalFile("D:\\Bejeweled00\\res\\sound_effect\\voice_excellent.wav"));
    awesomeSound = new QMediaPlayer();
    awesomeSound->setMedia(QUrl::fromLocalFile("D:\\Bejeweled00\\res\\sound_effect\\voice_awesome.wav"));
    combo_2 = new QMediaPlayer();
    combo_2->setMedia(QUrl::fromLocalFile("D:\\Bejeweled00\\res\\sound_effect\\combo_2.wav"));
    combo_3 = new QMediaPlayer();
    combo_3->setMedia(QUrl::fromLocalFile("D:\\Bejeweled00\\res\\sound_effect\\combo_3.wav"));
    combo_4 = new QMediaPlayer();
    combo_4->setMedia(QUrl::fromLocalFile("D:\\Bejeweled00\\res\\sound_effect\\combo_4.wav"));
    combo_5 = new QMediaPlayer();
    combo_5->setMedia(QUrl::fromLocalFile("D:\\Bejeweled00\\res\\sound_effect\\combo_5.wav"));
    combo_6 = new QMediaPlayer();
    combo_6->setMedia(QUrl::fromLocalFile("D:\\Bejeweled00\\res\\sound_effect\\combo_6.wav"));
    combo_7 = new QMediaPlayer();
    combo_7->setMedia(QUrl::fromLocalFile("D:\\Bejeweled00\\res\\sound_effect\\combo_7.wav"));
    tick = new QMediaPlayer();
    tick->setMedia(QUrl::fromLocalFile("D:\\Bejeweled00\\res\\sound_effect\\tick.wav"));
    badMove = new QMediaPlayer();
    badMove->setMedia(QUrl::fromLocalFile("D:\\Bejeweled00\\res\\sound_effect\\badmove.wav"));
    bgMusicList = new QMediaPlaylist();
    bgMusicList->addMedia(QUrl::fromLocalFile("D:\\Bejeweled00\\res\\music\\Mainmenu.mp3"));
    bgMusicList->addMedia(QUrl::fromLocalFile("D:\\Bejeweled00\\res\\music\\Classic.mp3"));
    bgMusicList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    bgplayer->setPlaylist(bgMusicList);
    hit = new QMediaPlayer();
    redPa.setColor(QPalette::WindowText, Qt::red);
    greenPa.setColor(QPalette::WindowText, Qt::green);
    bluePa.setColor(QPalette::WindowText, Qt::blue);
    greyPa.setColor(QPalette::WindowText, Qt::darkCyan);
    blackPa.setColor(QPalette::WindowText, Qt::black);
    hit->setMedia(QUrl::fromLocalFile("D:\\Bejeweled00\\res\\sound_effect\\gem_hit.wav"));

    startHome();
}

MainWindow::~MainWindow() {
    if (!server) {
        server->close();
        server->deleteLater();
    }
    delete ui;
}
//todo combo动画 消除动画  music soundEffect double
/**
 * home页 todo 登陆注册
 */
void MainWindow::startHome() {
    bgMusicList->setCurrentIndex(1);
    if(bgMusic) bgplayer->play();
    homepage = new Homepage(this);
    QPalette palette;  //创建一个调色板的对象
    QPixmap pixmap;
    pixmap.load("D:/Bejeweled00/res/bg.png");
    palette.setBrush(backgroundRole(), QBrush(pixmap));
    setPalette(palette);
    setCentralWidget(homepage);
    move(100, 20);
    resize(1600, 1000);
    homepage->show();
    QObject::connect(homepage->start_button, SIGNAL(clicked()), this, SLOT(startClicked()));
    QObject::connect(homepage->score_button, SIGNAL(clicked()), this, SLOT(scoreClicked()));
    QObject::connect(homepage->join_button, SIGNAL(clicked()), this, SLOT(joinClicked()));

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
    scorepage = new Scorepage(this, gameState);
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
    pixmap.load("D:/Bejeweled00/res/img.png");
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
        delete selectpage;
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
        int seed = rand();
        startSignal += to_string(seed);
        socket->write(startSignal.data());
        startGame2(seed);
    } else QMessageBox::warning(this, "warning", "The other man is not ready yet!");
}

//todo 传入随机端口号作为房间号,
void MainWindow::roomPage() {
    createRoom();//创建监听服务
    roompage = new Roompage(this, port);
    QPalette palette;  //创建一个调色板的对象
    QPixmap pixmap;
    pixmap.load("D:/Bejeweled00/res/img.png");
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
    if (remain <= 90)
        progressBar->setStyleSheet("QProgressBar{color:grey;} QProgressBar::chunk{background-color: rgb(0,255,255)}");
    if (remain < 60)
        progressBar->setStyleSheet("QProgressBar{color:grey;} QProgressBar::chunk{background-color: rgb(255,255,0)}");
    if (remain < 20)
        progressBar->setStyleSheet("QProgressBar{color:grey;} QProgressBar::chunk{background-color: rgb(255,99,71)}");
    progressBar->setValue((120 - remain) / 1.2);
    update();
}
/**
 *slot  交换
 * @param direction
 */
void MainWindow::onSwap(SwapDirection direction) {
    if (uiDrawing || gameState->state() != GameState::INGAME)
        return;
    if(sound_effect1)tick->play();
    gameState->Pause();
    pauseButton->setEnabled(false);
    hintButton->setEnabled(false);

    auto *sender = (Jewel *) this->sender();
    int x = sender->geometry().y() / 50;
    int y = sender->geometry().x() / 50;
    JewelPos pos(x, y);
    uiDrawing = true;
    //swap first

    if (!swapJewelInMap(x, y, direction)) { return; }

    auto events = gameState->Swap(pos, direction);//gameState->Swap
    string str = processStrToSend(pos, direction);
    if (readyDouble) socket->write(str.data());
    bool swaped = false;
    for (const BoardEvent &event : events) {
        swaped = true;
        drawBoardEvent(event);//
    }
    if (!swaped) {
        swapJewelInMap(x, y, direction);//如果此次交换无效,那么再换回来
        if(sound_effect1)badMove->play();
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
    string end;//end 结束进行的通知代号
    if(bgMusic)bgplayer->play();
    if(sound_effect1)timeUpSound->play();
    gameBgPlayer->stop();
    if (highScore) {
        QMessageBox::information(this, "Congratulations!", "You have achieved a new high score!");
    } else {
        QMessageBox::information(this, "Sorry!", "You didn't do well this time.");
    }
    pauseButton->setEnabled(false);
    hintButton->setEnabled(false);
    if (readyDouble) {
        death1 = true;
        if (death2) {
            readyDouble = false;
            if (scoreDisplay->text().toInt() > scoreDisplay2->text().toInt()) {
                QMessageBox::information(this, "CONGRATULATIONS", "YOU WON THIS TIME!");
                end = "16666";

            } else if (scoreDisplay->text().toInt() == scoreDisplay2->text().toInt()) {
                QMessageBox::information(this, "OOPS", "YOU HAVE A DRAW");
                end = "36666";
            } else {
                QMessageBox::information(this, "SORRY", "YOU LOSE THIS TIME!");
                end = "26666";
            }
            socket->write(end.data());
            socket->close();
            if (!server) {
                server->close();
                server->deleteLater();
            }
        } else {
            end = "66666";
            socket->write(end.data());
        }
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
            startGame2(atoi(s.substr(5, s.length() - 5).c_str()));//correspondence
            return;
        case '6':
            QPalette redPa;
            switch (s[0]) {
                case '6':
                    redPa.setColor(QPalette::WindowText, Qt::red);
                    scoreDisplay2->setPalette(redPa);
                    deathLabel->setPalette(redPa);
                    scoreDisplay2->setText(scoreDisplay2->text());
                    deathLabel->setFont(QFont("Microsoft YaHei", 15, 150));
                    deathLabel->move(805, 130);
                    deathLabel->show();
                    gameState2->Exit();
                    death2 = true;
                    return;
                case '1':
                    QMessageBox::information(this, "SORRY", "YOU LOSE THIS TIME!");
                    readyDouble = false;
                    socket->close();
                    if (!server) {
                        server->close();
                        server->deleteLater();
                    }
                    return;
                case '2':
                    QMessageBox::information(this, "CONGRATULATIONS", "YOU WON THIS TIME!");
                    readyDouble = false;
                    socket->close();
                    if (!server) {
                        server->close();
                        server->deleteLater();
                    }
                    return;
                case '3':
                    QMessageBox::information(this, "OOPS", "YOU HAVE A DRAW");
                    readyDouble = false;
                    socket->close();
                    if (!server) {

                        server->close();
                        server->deleteLater();
                    }
                    return;
            }
    }
    onSwap2(x, y, direction);
};
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
                animation->setEndValue(QRect(widget->x(), widget->y(), 50, 50)); // normal size
                animation->start();
                connect(animation, &QPropertyAnimation::finished, [=]() {
                    delete dynamic_cast<QPropertyAnimation *>(sender());
                });
                combo1 = 0;
                combo1Display->setText(QString::number(combo1));
                combo1Display->setPalette(bluePa);
            }
            break;
        }
        case BoardEvent::DIE: {
            int count = 0;
            for (JewelPos pos :event.getDiePos()) {
                //todo gif:消除
                Jewel *widget = map_[pos.x][pos.y].second;
                auto *animation = new QPropertyAnimation(widget, "geometry");
                animation->setDuration(200); // smaller than wait time
                animation->setEndValue(QRect(widget->x(), widget->y(), 1, 1));
                animation->setStartValue(QRect(widget->geometry())); // normal size
                animation->start();
                count++;
                connect(animation, &QPropertyAnimation::finished, [=]() {
                    delete dynamic_cast<QPropertyAnimation *>(sender());
                });
                map_[pos.x][pos.y].second->SetColor(Color::NONE);
                map_[pos.x][pos.y].first = Color::NONE;
            }
            if (count >= 6) {
                combo1++;
                combo1Display->setText(QString::number(combo1));
            }
            switch (combo1) {
                case 0:
                    if(sound_effect1)combo_2->play();
                    break;
                case 1:
                    combo1Display->setPalette(greenPa);
                    if(sound_effect1)combo_3->play();
                    break;
                case 2:
                    combo1Display->setPalette(yellowPa);
                    if(sound_effect1)combo_4->play();
                    break;
                case 3:
                    combo1Display->setPalette(bluePa);
                    if(sound_effect1)combo_5->play();
                    break;
                case 4:
                    combo1Display->setPalette(greyPa);
                    if(sound_effect1)combo_6->play();
                    break;
                case 5:
                    combo1Display->setPalette(redPa);
                    if(sound_effect1)combo_7->play();
                    break;
                default:
                    combo1Display->setPalette(redPa);
                    if(sound_effect1)combo_7->play();
            }
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
            if(sound_effect1)hit->play();
            switch (combo1) {
                case 0:
                    break;
                case 1:
                    if(sound_effect1)goodSound->play();
                    goodLabel->show();
                    goodAnimation->start();
                    QTimer::singleShot(1000, this, [=]() {
                        goodLabel->hide();
                    });
                    break;
                case 2:
                    if(sound_effect1)awesomeSound->play();
                    awesomeLabel->show();
                    awesomeAnimation->start();
                    QTimer::singleShot(1200, this, [=]() {
                        awesomeLabel->hide();
                    });
                    break;
                case 3:
                    if(sound_effect1)excellentSound->play();
                    excellentLabel->show();
                    excellentAnimation->start();
                    QTimer::singleShot(1400, this, [=]() {
                        excellentLabel->hide();
                    });
                    break;
                case 4:
                    if(sound_effect1)unbelievableSound->play();
                    unbelievableLabel->show();
                    unbelievableAnimation->start();
                    QTimer::singleShot(1600, this, [=]() {
                        unbelievableLabel->hide();
                    });
                    break;
                default:
                    if(sound_effect1)unbelievableSound->play();
                    unbelievableLabel->show();
                    unbelievableAnimation->start();
                    QTimer::singleShot(1600, this, [=]() {
                        unbelievableLabel->hide();
                    });

            }
            combo1++;
            combo1Display->setText(QString::number(combo1));
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
                combo2 = 0;
                combo2Display->setText(QString::number(combo2));
                combo2Display->setPalette(redPa);
            }
            break;
        }
        case BoardEvent::DIE: {
            int count = 0;
            for (JewelPos pos :event.getDiePos()) {
                map2_[pos.x][pos.y].second->SetColor(Color::NONE);
                map2_[pos.x][pos.y].first = Color::NONE;
                count++;
            }
            if (count >= 6) {
                combo2++;
                combo2Display->setText(QString::number(combo2));
            }
            switch (combo2) {
                case 0:
                    break;
                case 1:
                    combo2Display->setPalette(greenPa);
                    break;
                case 2:
                    combo2Display->setPalette(yellowPa);
                    break;
                case 3:
                    combo2Display->setPalette(bluePa);
                    break;
                case 4:
                    combo2Display->setPalette(greyPa);
                    break;
                case 5:
                    combo2Display->setPalette(redPa);
                    break;
                default:
                    combo2Display->setPalette(redPa);
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
                auto *animation2 = new QPropertyAnimation(map2_[fallPos.second.x][fallPos.second.y].second, "geometry",
                                                          this);
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
            switch (combo2) {
                case 0:
                    break;
                case 1:
                    if(sound_effect2)goodSound->play();
                    goodLabel2->show();
                    goodAnimation2->start();
                    QTimer::singleShot(1000, this, [=]() {
                        goodLabel2->hide();
                    });
                    break;
                case 2:
                    if(sound_effect2)awesomeSound->play();
                    awesomeLabel2->show();
                    awesomeAnimation2->start();
                    QTimer::singleShot(1200, this, [=]() {
                        awesomeLabel2->hide();
                    });
                    break;
                case 3:
                    if(sound_effect2)excellentSound->play();
                    excellentLabel2->show();
                    excellentAnimation2->start();
                    QTimer::singleShot(1400, this, [=]() {
                        excellentLabel2->hide();
                    });
                    break;
                case 4:
                    if(sound_effect2)unbelievableSound->play();
                    unbelievableLabel2->show();
                    unbelievableAnimation2->start();
                    QTimer::singleShot(1600, this, [=]() {
                        unbelievableLabel2->hide();
                    });
                    break;
                default:
                    if(sound_effect2)unbelievableSound->play();
                    unbelievableLabel2->show();
                    unbelievableAnimation2->start();
                    QTimer::singleShot(1600, this, [=]() {
                        unbelievableLabel2->hide();
                    });

            }
            combo2++;
            combo2Display->setText(QString::number(combo2));
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
    currentFrame = new QFrame(this);
    excellentLabel = new QLabel(currentFrame);
    goodLabel=new QLabel(currentFrame);
    unbelievableLabel=new QLabel(currentFrame);
    awesomeLabel=new QLabel(currentFrame);
    excellentLabel->resize(206, 150);
    goodLabel->resize(216, 192);
    unbelievableLabel->resize(231, 248);
    awesomeLabel->resize(216, 193);
    goodLabel->setPixmap(QPixmap("D:\\Bejeweled00\\res\\goodjob.png"));
    excellentLabel->setPixmap(QPixmap("D:\\Bejeweled00\\res\\EXCELLENT.png"));
    awesomeLabel->setPixmap(QPixmap("D:\\Bejeweled00\\res\\awesome.png"));
    unbelievableLabel->setPixmap(QPixmap("D:\\Bejeweled00\\res\\unbelievable.png"));
    excellentLabel->move(550, 260);
    awesomeLabel->move(550, 260);
    goodLabel->move(550, 260);
    unbelievableLabel->move(550, 260);
    unbelievableLabel->hide();
    awesomeLabel->hide();
    goodLabel->hide();
    excellentLabel->hide();
    goodAnimation = new QPropertyAnimation(goodLabel, "geometry");
    goodAnimation->setEasingCurve(QEasingCurve::InOutCubic);
    goodAnimation->setDuration(1000);
    goodAnimation->setStartValue(QRect(goodLabel->x(), goodLabel->y(), 1, 1));
    goodAnimation->setEndValue(QRect(goodLabel->x(), goodLabel->y(), 216, 192));

    awesomeAnimation = new QPropertyAnimation(awesomeLabel, "geometry");
    awesomeAnimation->setEasingCurve(QEasingCurve::InOutCubic);
    awesomeAnimation->setDuration(1200);
    awesomeAnimation->setStartValue(QRect(awesomeLabel->x(), awesomeLabel->y(), 1, 1));
    awesomeAnimation->setEndValue(QRect(awesomeLabel->x(), awesomeLabel->y(), 216, 193));

    unbelievableAnimation = new QPropertyAnimation(unbelievableLabel, "geometry");
    unbelievableAnimation->setEasingCurve(QEasingCurve::InOutCubic);
    unbelievableAnimation->setDuration(1600);
    unbelievableAnimation->setStartValue(QRect(unbelievableLabel->x(), unbelievableLabel->y(), 1, 1));
    unbelievableAnimation->setEndValue(QRect(unbelievableLabel->x(), unbelievableLabel->y(), 231, 248));

    excellentAnimation = new QPropertyAnimation(excellentLabel, "geometry");
    excellentAnimation->setEasingCurve(QEasingCurve::InOutCubic);
    excellentAnimation->setDuration(1400);
    excellentAnimation->setStartValue(QRect(excellentLabel->x(), excellentLabel->y(), 1, 1));
    excellentAnimation->setEndValue(QRect(excellentLabel->x(), excellentLabel->y(), 206, 150)); // normal size
    bgplayer->stop();
    if(gameBgMusic)gameBgPlayer->play();
    setCentralWidget(currentFrame);
    auto *under_frame = new QFrame(currentFrame);
    auto *layout = new QGridLayout(currentFrame);
    auto *under_layout = new QGridLayout(under_frame);
    auto *board = new QLabel(currentFrame);
    combo1Label = new QLabel("COMBO", currentFrame);
    combo1Display = new QLabel("", currentFrame);
    board->setPixmap(QPixmap("D:/Bejeweled00/res/img.png"));
    board->setMinimumHeight(kJewelWidgetSize * (boardSize));
    board->setMinimumWidth(kJewelWidgetSize * (boardSize));
    board->setMaximumHeight(kJewelWidgetSize * (boardSize));
    board->setMaximumWidth(kJewelWidgetSize * (boardSize));
    // set up appropriate fonts and colors
    auto *scoreLable = new QLabel(tr("SCORE"), currentFrame);
    scoreLable->setFont(QFont("Microsoft YaHei", 15, 150));
    scoreDisplay = new QLabel(currentFrame);
    scoreDisplay->setFont(QFont("Microsoft YaHei", 15, 150));
    scoreDisplay->setPalette(bluePa);
    auto *timeUsedLabel = new QLabel(tr("TIME USED"), currentFrame);
    timeUsedLabel->setFont(QFont("Microsoft YaHei", 15, 150));
    timeUsedLabel->resize(200, 100);
    timeUsedLabel->move(350, 500);
    progressBar = new QProgressBar(this);
    int len = this->width() / 100 * 6;
    progressBar->setGeometry(this->width() / 20 * 7 + 20, this->height() - len, 10 * len - 40, len * 7 / 10 + 15);
    progressBar->setRange(0, 99);
    progressBar->setValue(0);
    progressBar->setFormat("%p%");
    progressBar->setAlignment(Qt::AlignCenter);
    progressBar->setFont(QFont("Microsoft YaHei", 12, 100));
    progressBar->setStyleSheet("QProgressBar{color:grey;} QProgressBar::chunk{background-color: rgb(60,179,113)}");
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
    scoreLable->move(550, 130);
    scoreDisplay->move(550, 90);
    combo1Label->move(550, 240);
    combo1Label->setFont(QFont("Microsoft YaHei", 15, 150));
    combo1Display->move(550, 200);
    combo1Display->setFont(QFont("Microsoft YaHei", 15, 150));
    under_layout->addWidget(new QLabel("", under_frame), 2, 0);
    under_layout->addWidget(progressBar, 3, 0);
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
    if(sound_effect1)goSound->play();
}
/**
 * 双人模式入口
 */
void MainWindow::startGame2(int seed) {
    // Set new UI frame
    bgplayer->stop();
    if(gameBgMusic)gameBgPlayer->play();
    death2 = false;
    death1 = false;
    currentFrame = new QFrame(this);
    excellentLabel = new QLabel(currentFrame);
    goodLabel=new QLabel(currentFrame);
    unbelievableLabel=new QLabel(currentFrame);
    awesomeLabel=new QLabel(currentFrame);
    excellentLabel->resize(206, 150);
    goodLabel->resize(216, 192);
    unbelievableLabel->resize(231, 248);
    awesomeLabel->resize(216, 193);
    goodLabel->setPixmap(QPixmap("D:\\Bejeweled00\\res\\goodjob.png"));
    excellentLabel->setPixmap(QPixmap("D:\\Bejeweled00\\res\\EXCELLENT.png"));
    awesomeLabel->setPixmap(QPixmap("D:\\Bejeweled00\\res\\awesome.png"));
    unbelievableLabel->setPixmap(QPixmap("D:\\Bejeweled00\\res\\unbelievable.png"));
    excellentLabel->move(540, 260);
    awesomeLabel->move(540, 260);
    goodLabel->move(540, 260);
    unbelievableLabel->move(540, 260);
    unbelievableLabel->hide();
    awesomeLabel->hide();
    goodLabel->hide();
    excellentLabel->hide();
    goodAnimation = new QPropertyAnimation(goodLabel, "geometry");
    goodAnimation->setEasingCurve(QEasingCurve::InOutCubic);
    goodAnimation->setDuration(1000);
    goodAnimation->setStartValue(QRect(goodLabel->x()-216, goodLabel->y(), 1, 1));
    goodAnimation->setEndValue(QRect(goodLabel->x(), goodLabel->y(), 216, 192));

    awesomeAnimation = new QPropertyAnimation(awesomeLabel, "geometry");
    awesomeAnimation->setEasingCurve(QEasingCurve::InOutCubic);
    awesomeAnimation->setDuration(1200);
    awesomeAnimation->setStartValue(QRect(awesomeLabel->x()-216, awesomeLabel->y(), 1, 1));
    awesomeAnimation->setEndValue(QRect(awesomeLabel->x(), awesomeLabel->y(), 216, 193));

    unbelievableAnimation = new QPropertyAnimation(unbelievableLabel, "geometry");
    unbelievableAnimation->setEasingCurve(QEasingCurve::InOutCubic);
    unbelievableAnimation->setDuration(1600);
    unbelievableAnimation->setStartValue(QRect(unbelievableLabel->x()-231, unbelievableLabel->y(), 1, 1));
    unbelievableAnimation->setEndValue(QRect(unbelievableLabel->x(), unbelievableLabel->y(), 231, 248));


    excellentAnimation = new QPropertyAnimation(excellentLabel, "geometry");
    excellentAnimation->setEasingCurve(QEasingCurve::InOutCubic);
    excellentAnimation->setDuration(1400);
    excellentAnimation->setStartValue(QRect(excellentLabel->x()-206, excellentLabel->y(), 1, 1));
    excellentAnimation->setEndValue(QRect(excellentLabel->x(), excellentLabel->y(), 206, 150)); // normal size


    excellentLabel2 = new QLabel(currentFrame);
    goodLabel2=new QLabel(currentFrame);
    unbelievableLabel2=new QLabel(currentFrame);
    awesomeLabel2=new QLabel(currentFrame);
    excellentLabel2->resize(206, 150);
    goodLabel2->resize(216, 192);
    unbelievableLabel2->resize(231, 248);
    awesomeLabel2->resize(216, 193);
    goodLabel2->setPixmap(QPixmap("D:\\Bejeweled00\\res\\goodjob2.png"));
    excellentLabel2->setPixmap(QPixmap("D:\\Bejeweled00\\res\\EXCELLENT2.png"));
    awesomeLabel2->setPixmap(QPixmap("D:\\Bejeweled00\\res\\awesome2.png"));
    unbelievableLabel2->setPixmap(QPixmap("D:\\Bejeweled00\\res\\unbelievable2.png"));
    excellentLabel2->move(600, 260);
    awesomeLabel2->move(600, 260);
    goodLabel2->move(600, 260);
    unbelievableLabel2->move(600, 260);
    unbelievableLabel2->hide();
    awesomeLabel2->hide();
    goodLabel2->hide();
    excellentLabel2->hide();
    goodAnimation2 = new QPropertyAnimation(goodLabel2, "geometry");
    goodAnimation2->setEasingCurve(QEasingCurve::InOutCubic);
    goodAnimation2->setDuration(1000);
    goodAnimation2->setStartValue(QRect(goodLabel2->x()+216, goodLabel2->y(), 1, 1));
    goodAnimation2->setEndValue(QRect(goodLabel2->x(), goodLabel2->y(), 216, 192));

    awesomeAnimation2 = new QPropertyAnimation(awesomeLabel2, "geometry");
    awesomeAnimation2->setEasingCurve(QEasingCurve::InOutCubic);
    awesomeAnimation2->setDuration(1200);
    awesomeAnimation2->setStartValue(QRect(awesomeLabel2->x()+216, awesomeLabel2->y(), 1, 1));
    awesomeAnimation2->setEndValue(QRect(awesomeLabel2->x(), awesomeLabel2->y(), 216, 193));

    unbelievableAnimation2 = new QPropertyAnimation(unbelievableLabel2, "geometry");
    unbelievableAnimation2->setEasingCurve(QEasingCurve::InOutCubic);
    unbelievableAnimation2->setDuration(1600);
    unbelievableAnimation2->setStartValue(QRect(unbelievableLabel2->x()+231, unbelievableLabel2->y(), 1, 1));
    unbelievableAnimation2->setEndValue(QRect(unbelievableLabel2->x(), unbelievableLabel2->y(), 231, 248));

    excellentAnimation2 = new QPropertyAnimation(excellentLabel2, "geometry");
    excellentAnimation2->setEasingCurve(QEasingCurve::InOutCubic);
    excellentAnimation2->setDuration(1400);
    excellentAnimation2->setStartValue(QRect(excellentLabel2->x()+206, excellentLabel2->y(), 1, 1));
    excellentAnimation2->setEndValue(QRect(excellentLabel2->x(), excellentLabel2->y(), 206, 150)); // normal size

    QPalette palette;  //创建一个调色板的对象
    QPixmap pixmap;
    pixmap.load("D:/Bejeweled00/res/img.png");
    palette.setBrush(backgroundRole(), QBrush(pixmap));
    setPalette(palette);
    pauseButton = new JewelButton();
    deathLabel = new QLabel("DEATH", currentFrame);
    deathLabel->setPalette(Qt::red);
    deathLabel->hide();
    setCentralWidget(currentFrame);
    auto *under_frame = new QFrame(currentFrame);
    auto *under_layout = new QGridLayout(under_frame);
    auto *board = new QLabel(currentFrame);
    auto *P1 = new QLabel(currentFrame);
    P1->setPixmap(QPixmap("D:/Bejeweled00/res/P1.png"));
    P1->move(0, 650);
    auto *P2 = new QLabel(currentFrame);
    P2->setPixmap(QPixmap("D:/Bejeweled00/res/P2.png"));
    P2->move(1240, 650);
    board->setPixmap(QPixmap("D:/Bejeweled00/res/img.png"));
    board->setMinimumHeight(kJewelWidgetSize * (boardSize));
    board->setMinimumWidth(kJewelWidgetSize * (boardSize));
    board->setMaximumHeight(kJewelWidgetSize * (boardSize));
    board->setMaximumWidth(kJewelWidgetSize * (boardSize));
    auto *board2 = new QLabel(currentFrame);
    board2->setMinimumHeight(kJewelWidgetSize * (boardSize));
    board2->setMinimumWidth(kJewelWidgetSize * (boardSize));
    board2->setMaximumHeight(kJewelWidgetSize * (boardSize));
    board2->setMaximumWidth(kJewelWidgetSize * (boardSize));
    board2->setPixmap(QPixmap("D:/Bejeweled00/res/img.png"));
    QPalette blue_pa;
    blue_pa.setColor(QPalette::WindowText, Qt::blue);
    QPalette RED_pa;
    RED_pa.setColor(QPalette::WindowText, Qt::red);
    scoreDisplay2 = new QLabel(currentFrame);
    scoreDisplay2->setFont(QFont("Microsoft YaHei", 15, 150));
    scoreDisplay2->setPalette(RED_pa);
    auto *label1 = new QLabel(tr("SCORE"), currentFrame);
    auto *label2 = new QLabel(tr("SCORE"), currentFrame);
    label1->setFont(QFont("Microsoft YaHei", 15, 150));
    label1->setPalette(blue_pa);
    label2->setFont(QFont("Microsoft YaHei", 15, 150));
    label2->setPalette(RED_pa);
    scoreDisplay = new QLabel(currentFrame);
    scoreDisplay->resize(200, 100);
    scoreDisplay->setFont(QFont("Microsoft YaHei", 15, 150));
    scoreDisplay->setPalette(blue_pa);
    auto *timeUsedLabel = new QLabel(tr("YOUR TIMEBAR"), currentFrame);
    timeUsedLabel->setFont(QFont("Microsoft YaHei", 15, 150));
    auto *exitButton = new JewelButton(under_frame);
    exitButton->setText(tr("Exit"));
    connect(exitButton, SIGNAL(clicked()), this, SLOT(exitClicked()));
    resize(1400, 500);
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
    hintButton = new JewelButton(under_frame);
    hintButton->setText(tr("Hint"));
    connect(hintButton, SIGNAL(clicked()), this, SLOT(hintClicked()));
    progressBar = new QProgressBar(this);
    int len = this->width() / 100 * 6;
    progressBar->setGeometry(this->width() / 20 * 7 + 20, this->height() - len, 10 * len - 40, len * 7 / 10 + 30);
    progressBar->setRange(0, 99);
    progressBar->setValue(0);
    progressBar->setFormat("%p%");
    progressBar->setAlignment(Qt::AlignCenter);
    progressBar->setFont(QFont("Microsoft YaHei", 12, 100));
    progressBar->setStyleSheet("QProgressBar{color:grey;} QProgressBar::chunk{background-color: rgb(60,179,113)}");
    timeUsedLabel->resize(200, 100);
    timeUsedLabel->move(580, 570);
    under_layout->addWidget(new QLabel("", under_frame), 2, 0);
    under_layout->addWidget(progressBar, 3, 0);
    under_layout->addWidget(hintButton, 4, 0);
    under_layout->addWidget(exitButton, 5, 0);
    label1->move(590, 50);
    label2->move(700, 50);
    scoreDisplay->move(632, 100);
    scoreDisplay->setAlignment(Qt::AlignRight);
    auto *VS = new QLabel(currentFrame);
    VS->setPixmap(QPixmap("D:/Bejeweled00/res/vs2.png"));
    VS->move(530, 380);
    scoreDisplay2->move(710, 65);
    scoreDisplay2->resize(200, 100);
    scoreDisplay->setAlignment(Qt::AlignLeft);
    combo1Label = new QLabel("COMBO", currentFrame);
    combo2Label = new QLabel("COMBO", currentFrame);
    combo1Label->setPalette(bluePa);
    combo2Label->setPalette(redPa);
    combo1Label->setFont(QFont("Microsoft YaHei", 15, 150));
    combo2Label->setFont(QFont("Microsoft YaHei", 15, 150));
    combo1Display = new QLabel("", currentFrame);
    combo2Display = new QLabel("", currentFrame);
    combo1Display->setFont(QFont("Microsoft YaHei", 15, 150));
    combo2Display->setFont(QFont("Microsoft YaHei", 15, 150));
    combo1Label->move(590, 130);
    combo1Display->move(610, 180);
    combo1Display->resize(20, 20);
    combo2Label->move(700, 130);
    combo2Display->move(720, 180);
    combo2Display->resize(20, 20);
    board->move(10, 50);
    board2->move(900, 50);
    under_frame->move(200, 500);
    under_frame->resize(1000, 300);
    currentFrame->show();
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
    if(sound_effect1)goSound->play();
}
//!网络Qt通信
/**创建房间
 *
 */
void MainWindow::createRoom() {
    server = new QTcpServer();
    port = rand() % 64511 + 1024;//todo:random
    while (!server->listen(QHostAddress::Any, port)) {//监听开始
        qDebug() << server->errorString();
        QMessageBox::warning(this, "DEFEAT", "OCCUPIED PORT:" + QString::number(port) + "\nFINDING ANOTHER...");
        port = rand() % 64511 + 1024;
    };
    QMessageBox::information(this, "SUCCESS", "PORT HAVE OPENED ON :" + QString::number(port));
    connect(server, &QTcpServer::newConnection, this, &MainWindow::serverNewConnect);
}
void MainWindow::joinRoom(int roomNumber) {
    socket = new QTcpSocket();
    QObject::connect(socket, &QTcpSocket::readyRead, this, &MainWindow::socketReadData);
    QObject::connect(socket, &QTcpSocket::disconnected, this, &MainWindow::socketDisconnected);
//    QString IP = tr("127.0.0.1");//todo:设置局域网地址
    port = roomNumber;
    socket->abort();
    socket->connectToHost(IP, port);
    if (!socket->waitForConnected(3000)) { QMessageBox::warning(this, "DEFEAT", "JOIN DEFEAT!"); }
    else {
        QMessageBox::information(this, "SUCCESS", "JOIN SUCCESS!");
        readyDouble = true;
    }

}
/**
 * 读取传输的数据
 */
void MainWindow::socketReadData() {
    QByteArray buffer;
    buffer = socket->readAll();
    if (!buffer.isEmpty()) {
        string s = tr(buffer).toStdString();
        processReadStr(s);
    }
}
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
    QMessageBox::warning(this, "HINT", "The other man has disconnected!");
    readyDouble = false;
}
/**
 * 被连接
 */
void MainWindow::serverNewConnect() {
    QMessageBox::warning(this, "HINT", "READY!");
    readyDouble = true;
    socket = server->nextPendingConnection();
    QObject::connect(socket, &QTcpSocket::readyRead, this, &MainWindow::socketReadData);
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
            if (x >= boardSize - 1)
                return false;
            const QRect tmp = map_[x][y].second->geometry();
            const QRect tmp2 = map_[x + 1][y].second->geometry();
            auto *animation2 = new QPropertyAnimation(map_[x + 1][y].second, "geometry",
                                                      this);
            animationDrawing = true;
            auto *animation = new QPropertyAnimation(map_[x][y].second, "geometry", this);
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
            Color tmpColor = map_[x + 1][y].first;
            map_[x + 1][y].first = map_[x][y].first;
            Jewel *temp = map_[x][y].second;//temp=first;
            map_[x][y].second = map_[x + 1][y].second;//first=second
            map_[x + 1][y].second = temp;//second=temp
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
            animationDrawing = true;
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
            Color tmpColor = map_[x - 1][y].first;
            map_[x - 1][y].first = map_[x][y].first;
            Jewel *temp = map_[x][y].second;//temp=first;
            map_[x][y].second = map_[x - 1][y].second;//first=second
            map_[x - 1][y].second = temp;//second=temp
            map_[x][y].first = tmpColor;
            break;
        }
        case LEFT: {
            if (y <= 0)
                return false;
            const QRect tmp = map_[x][y].second->geometry();
            const QRect tmp2 = map_[x][y - 1].second->geometry();
            animationDrawing = true;
            auto *animation = new QPropertyAnimation(map_[x][y].second, "geometry", this);
            animation->setDuration(300);
            animation->setStartValue(tmp);
            animation->setEndValue(QRect(tmp2));
            animation->setEasingCurve(QEasingCurve::InQuad);
            animation->start();
            QTimer::singleShot(400, this, [=]() {
                delete animation;
            });
            auto *animation2 = new QPropertyAnimation(map_[x][y - 1].second, "geometry", this);
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
            Color tmpColor = map_[x][y - 1].first;
            map_[x][y - 1].first = map_[x][y].first;
            Jewel *temp = map_[x][y].second;//temp=first;
            map_[x][y].second = map_[x][y - 1].second;//first=second
            map_[x][y - 1].second = temp;//second=temp
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
            animationDrawing = true;
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
            Color tmpColor = map_[x][y + 1].first;
            map_[x][y + 1].first = map_[x][y].first;
            Jewel *temp = map_[x][y].second;//temp=first;
            map_[x][y].second = map_[x][y + 1].second;//first=second
            map_[x][y + 1].second = temp;//second=temp
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
            if (x >= boardSize - 1)
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
            Color tmpColor = map2_[x + 1][y].first;
            map2_[x + 1][y].first = map2_[x][y].first;
            Jewel *temp = map2_[x][y].second;//temp=first;
            map2_[x][y].second = map2_[x + 1][y].second;//first=second
            map2_[x + 1][y].second = temp;//second=temp
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
            auto *animation2 = new QPropertyAnimation(map2_[x - 1][y].second, "geometry", this);
            animationDrawing = true;
            animation2->setDuration(300);
            animation2->setStartValue(tmp2);//新位置的
            animation2->setEndValue(QRect(tmp));
            animation2->setEasingCurve(QEasingCurve::InQuad);
            animation2->start();
            QTimer::singleShot(400, this, [=]() {
                delete animation2;
            });
            auto *animation = new QPropertyAnimation(map2_[x][y].second, "geometry", this);
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
            Color tmpColor = map2_[x - 1][y].first;
            map2_[x - 1][y].first = map2_[x][y].first;
            Jewel *temp = map2_[x][y].second;//temp=first;
            map2_[x][y].second = map2_[x - 1][y].second;//first=second
            map2_[x - 1][y].second = temp;//second=temp
            map2_[x][y].first = tmpColor;
            break;
        }
        case LEFT: {
            if (y <= 0)
                return false;
            const QRect tmp = map2_[x][y].second->geometry();
            const QRect tmp2 = map2_[x][y - 1].second->geometry();
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
            Color tmpColor = map2_[x][y - 1].first;
            map2_[x][y - 1].first = map2_[x][y].first;
            Jewel *temp = map2_[x][y].second;//temp=first;
            map2_[x][y].second = map2_[x][y - 1].second;//first=second
            map2_[x][y - 1].second = temp;//second=temp
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
            animationDrawing = true;
            animation2->setDuration(300);
            animation2->setStartValue(tmp2);//新位置的
            animation2->setEndValue(QRect(tmp));
            animation2->setEasingCurve(QEasingCurve::InQuad);
            animation2->start();
            QTimer::singleShot(400, this, [=]() {
                delete animation2;
            });
            auto *animation = new QPropertyAnimation(map2_[x][y].second, "geometry", this);
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
            Color tmpColor = map2_[x][y + 1].first;
            map2_[x][y + 1].first = map2_[x][y].first;
            Jewel *temp = map2_[x][y].second;//temp=first;
            map2_[x][y].second = map2_[x][y + 1].second;//first=second
            map2_[x][y + 1].second = temp;//second=temp
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