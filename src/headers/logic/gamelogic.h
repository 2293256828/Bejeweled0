#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <QObject>
#include <QTimer>
#include "src/headers/components/timer.h"

namespace Bejeweled {

// Should be used as interface
    class ModeLogic : public QObject {
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

        ModeLogic(const ModeLogic &) = delete;

        ModeLogic &operator=(const ModeLogic &) = delete;

    signals:

        void timeTick(int);

        void timeOut();

    protected:
        CountdownTimer *timer_;
    };

    class TimeOutMode_P1 : public ModeLogic {
    Q_OBJECT
    public:
        TimeOutMode_P1();

        int getGeneration() override;

        void FinishedOneMove() override {} // do nothing in TimeOutMode_P1

    };

    class TimeOutMode_P2 : public ModeLogic {
    Q_OBJECT
    public:
        TimeOutMode_P2();

        int getGeneration() override;

        void FinishedOneMove() override {} // do nothing in TimeOutMode_P1

    };

    class FastReactionMode_P1 : public ModeLogic {
    Q_OBJECT
    public:
        FastReactionMode_P1();

        int getGeneration() override;

        void FinishedOneMove();

    private:
        int diff;
        int count;
    };
    class FastReactionMode_P2 : public ModeLogic {
    Q_OBJECT
    public:
        FastReactionMode_P2();

        int getGeneration() override;

        void FinishedOneMove();

    private:
        int diff;
        int count;
    };
}

#endif // GAMELOGIC_H
