#include "src/headers/components/scoresystem.h"
using namespace Bejeweled;
//alg
ScoreSystem::ScoreSystem() :
        combo(0),
        totalScore(0)
{
}


void ScoreSystem::firstGain(int gain)
{
    totalScore += gain + (gain - 3);
	++combo;
}

void ScoreSystem::comboGain(int gain)
{
    totalScore += gain + (gain - 3) + combo * 2;
	++combo;
}

void ScoreSystem::finishMove()
{
    combo = 0;
}
