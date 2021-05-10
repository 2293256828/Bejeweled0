#ifndef BOARD_H
#define BOARD_H
#include "src/headers/ui/jewel.h"
#include <list>
#include <utility>
#include <vector>
/**
 * board.h
 * 在命名空间Bejeweled中声明与棋盘相关的数据结构和方法
 */
using namespace std;
namespace Bejeweled {

    /**
     * 宝石坐标
     */
struct JewelPos {
	// (0,0) (0,1) (0,2) ...
	// (1,0) (1,1) (1,2) ...
	// ...
	JewelPos(int x,int y) {this->x=x,this->y=y;}
	int x;
	int y;
};

/**
 * 宝石坐标+颜色 构成JewelInfo
 */
typedef pair<JewelPos,int> JewelInfo;

class Board;
/**
 * 棋盘事件类
 */
class BoardEvent {
public:
	enum  EventType {NEW,FALL,DIE};//新生的宝石,下落的宝石,死去的宝石
	explicit BoardEvent(EventType etype);
	EventType type;
	list<JewelInfo> getNewPos() const {return info;}
	list<JewelPos> getDiePos() const {return pos;}
	list<pair<JewelPos,JewelPos>> getFallPos() const {return fall;}
	void setFallPos(list<pair<JewelPos,JewelPos>> list) { fall=list;}
	void setNewPos(list<JewelInfo> list) { info = list;}
	void setDiePos(list<JewelPos> list) { pos = list;}

	// default copy behaviors
	BoardEvent(const BoardEvent&) = default;
	BoardEvent& operator=(const BoardEvent&) = default;

private:
	list<pair<JewelPos,JewelPos>> fall;//fall list  第一个Pos是最开始的位置,第二个Pos是下落的位置
	list<JewelInfo> info;//info list
	list<JewelPos> pos;//pos list
};

class Board
{
typedef vector<vector<int>> IntTab;

public:
	static const int kSmallSize = 6;
	static const int kMediumSize = 8;
	static const int kLargeSize = 10;

	enum Size { SMALL = kSmallSize, MEDIUM = kMediumSize, LARGE = kLargeSize };

	explicit Board(Size);
	list<BoardEvent> Swap(JewelPos, Bejeweled::SwapDirection);
	void setGenerationFactor(int f) { generationFactor = f;}
	JewelPos getPossibleSwap();
	int getColorAt(int x, int y) {return board[x][y];}
	BoardEvent Init();
	// copy inhibited
	Board(const Board&) = delete;
	Board& operator=(const Board&) = delete;

private:
	list<JewelInfo> fullFill(IntTab &tab);
	int PossibleSwap(const IntTab&);
	list<JewelPos> Eliminatable(const IntTab&) const;

	list<pair<JewelPos,JewelPos>> Fall(IntTab&);

	JewelPos lastPossibleSwap;
	IntTab board;
	int generationFactor;
	const Size size;
};

}
#endif // BOARD_H
