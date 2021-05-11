#ifndef COUNTDOWNTIMER_H
#define COUNTDOWNTIMER_H

#include <QObject>

class QTimer;

class CountdownTimer : public QObject
{
	Q_OBJECT
public:
	explicit CountdownTimer(int sec,QObject *parent = nullptr);
	void Start();
	void Pause();
	void Resume();
    void Punish(int sec);
	void SetTimeRemained(int sec);
	int GetTimeRemained() const {return remainSec;}

	// copy inhibited
	CountdownTimer(const CountdownTimer&) = delete;
	CountdownTimer& operator=(const CountdownTimer&) = delete;

signals:
	void TimeOut();
	void Tick();
	
private slots:
	void InternalTimerTimeout_();

private:
	int remainSec;
	bool paused;
	bool dead;
	QTimer *tick;

};

#endif // COUNTDOWNTIMER_H
