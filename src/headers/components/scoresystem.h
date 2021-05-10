#ifndef SCORESYSTEM_H
#define SCORESYSTEM_H

namespace Bejeweled {
class ScoreSystem
{
public:
	ScoreSystem();
	void firstGain(int gain);
	void comboGain(int gain);
	void finishMove();
	int getScore() const  { return totalScore; }

	// copy inhibited
	ScoreSystem(const ScoreSystem&) = delete;
	ScoreSystem& operator=(const ScoreSystem&) = delete;

private:
	int combo;
	int totalScore;
};

}
#endif // SCORESYSTEM_H
