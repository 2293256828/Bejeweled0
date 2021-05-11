#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <QObject>
#include <QTimer>
#include "src/headers/components/timer.h"

namespace Bejeweled {

// Should be used as interface
class ModeLogic : public QObject
{
	Q_OBJECT
public:
	explicit ModeLogic(QObject *parent = nullptr);
	virtual int getGeneration() = 0;
	void Pause();
	void Resume();
	void Punish(int);
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
	int getGeneration() override;
	void FinishedOneMove() override {} // do nothing in TimeOutMode

};

class FastReactionMode : public ModeLogic
{
	Q_OBJECT
public:
	FastReactionMode();
	int getGeneration() override;
	void FinishedOneMove();
private:
	int diff;
	int count;
};

}

#endif // GAMELOGIC_H
