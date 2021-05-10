#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <QObject>
#include <QTimer>
#include "src/headers/components/countdowntimer.h"

namespace Bejeweled {

// Should be used as interface
class ModeLogic : public QObject
{
	Q_OBJECT
public:
	explicit ModeLogic(QObject *parent = nullptr);
	virtual int NextGeneration() = 0;
	void Pause();
	void Resume();
	virtual void FinishedOneMove() = 0;

	// copy inhibited
	ModeLogic() = delete;
	ModeLogic(const ModeLogic&) = delete;
	ModeLogic& operator=(const ModeLogic&) = delete;

signals:
	void timeTick(int);
	void timeOut();
protected:
	CountdownTimer *timer_;
};

class TimeOutMode : public ModeLogic
{
	Q_OBJECT
public:
	TimeOutMode();
	int NextGeneration() override;
	void FinishedOneMove() override {} // do nothing in TimeOutMode

};

class FastReactionMode : public ModeLogic
{
	Q_OBJECT
public:
	FastReactionMode();
	int NextGeneration() override;
	void IncreaseDifficulty();
	void ReduceDifficulty();
	void FinishedOneMove();
private:
	int diff;
	int count;
};

}

#endif // GAMELOGIC_H