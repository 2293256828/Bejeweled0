#include <cstdlib>
#include <ctime>
#include "src/headers/logic/board.h"
using namespace Bejeweled;
using namespace std;

Board::Board(Size size) :
        board(size, vector<int>(size, 0)),
        generationFactor(1),
        size(size),
        lastPossibleSwap(0, 0)
{
	srand(time(nullptr));
}
/**
 *进行一次尝试交换产生的事件
 * @param pos
 * @param direction
 * @return
 */
list<BoardEvent> Board::Swap(JewelPos pos, SwapDirection direction)
{
	list<BoardEvent> events;
	IntTab tab = board;
	int i = pos.x, j = pos.y;

	switch(direction) {
	case UP:
		if(i == 0)
			return events;
		tab[i][j] = tab[i-1][j];
		tab[i-1][j] = board[i][j];
		break;
	case DOWN:
		if(i == size - 1)
			return events;
		tab[i][j] = tab[i+1][j];
		tab[i+1][j] = board[i][j];
		break;
	case LEFT:
		if(j == 0)
			return events;
		tab[i][j] = tab[i][j-1];
		tab[i][j-1] = board[i][j];
		break;
	case RIGHT:
		if(j == size - 1)
			return events;
		tab[i][j] = tab[i][j+1];
		tab[i][j+1] = board[i][j];
		break;
	default:
		break;
	}
	// get list of to-be-eliminated jewelpos
	list<JewelPos> dieList = Eliminatable(tab);
	// if not a valid swap, return blank list
	if(dieList.empty())
		return events;

	// a valid swap
	board = tab;
	BoardEvent newEvent(BoardEvent::DIE);
    newEvent.setDiePos(dieList);
	events.push_back(newEvent);
	// first elimination
	for(JewelPos pos :dieList)
        board[pos.x][pos.y] = 0;
	// fall->eliminate->fall->eliminate-> ...
	do {
		//fall
		newEvent = BoardEvent(BoardEvent::FALL);
        newEvent.setFallPos(Fall(board));
		events.push_back(newEvent);
        dieList = Eliminatable(board);
		if(!dieList.empty()) {
            newEvent = BoardEvent(BoardEvent::DIE);
            newEvent.setDiePos(dieList);
			events.push_back(newEvent);
			// 消除
			for(JewelPos diePos : dieList )
                board[diePos.x][diePos.y] = 0;
		}
		else
			break;
	} while(true);
	// generate
	newEvent = BoardEvent(BoardEvent::NEW);
    newEvent.setNewPos(fullFill(board));
	events.push_back(newEvent);
	return events;
}
/**
 * 获得可行的交换
 * @return JewelPos
 */
JewelPos Board::getPossibleSwap()
{
	return lastPossibleSwap;
}

/**
 *初始化棋盘
 * @return
 */
BoardEvent Board::Init()
{
	BoardEvent event(BoardEvent::NEW);
    event.setNewPos(fullFill(board));
	return event;
}
/**
 * 填充棋盘
 * @param tab
 * @return List<JewelInfo> infolist 存放宝石信息的list
 */
list<JewelInfo> Board::fullFill(IntTab &tab)
{
	IntTab tmptab = tab;
	list<JewelInfo> infolist;
	do {
		infolist.clear();
		for(int i=0; i != size; ++i)
			for(int j=0; j != size; ++j)
				if(board[i][j] == 0) {
					// random between 1 and 5
					tmptab[i][j] = rand() % 5+1;
					infolist.emplace_back(JewelPos(i,j),tmptab[i][j]);
				}
	} while(!Eliminatable(tmptab).empty() || PossibleSwap(tmptab) < generationFactor);
	// update
	tab = tmptab;
	return infolist;
}
/**
 *
 * @param tab 棋盘
 * @return count 有多少可行交换
 */
int Board::PossibleSwap(const IntTab& tab)
{
	IntTab tab2 = tab;
	int count = 0;
	// swap right
	for(int i=0; i != size; ++i)
		for(int j=0; j != size - 1; ++j) {
			// swap
			tab2[i][j] = tab2[i][j+1];
			tab2[i][j+1] = tab[i][j];
			if(!Eliminatable(tab2).empty()) {
				// record it for hint use
				lastPossibleSwap.x = i;
                lastPossibleSwap.y = j;
				++count;
			}
			// revert changes
			tab2[i][j] = tab[i][j];
			tab2[i][j+1] = tab[i][j+1];
		}
	// swap down
	for(int j=0; j != size; ++j)
		for(int i=0; i != size - 1; ++i) {
			// swap
			tab2[i][j] = tab2[i+1][j];
			tab2[i+1][j] = tab[i][j];
			if(!Eliminatable(tab2).empty()) {
				// record it for hint use
				lastPossibleSwap.x = i;
                lastPossibleSwap.y = j;
				++count;
			}
			// revert changes
			tab2[i][j] = tab[i][j];
			tab2[i+1][j] = tab[i+1][j];
		}
	return count;
}
/**
 * 检查如果进行了交换,那么能否产生消除
 * @param tab 一次尝试交换后的board
 * @return dieList 将被消除的位置组成的list
 */
list<JewelPos> Board::Eliminatable(const Board::IntTab& tab) const
{
	bool eliminated[kLargeSize][kLargeSize] = {false};
	list<JewelPos> dieList;
	// check horizontally
	for(int i=0; i != size; ++i)
		for(int j=0; j != size; ++j) {
			if(tab[i][j] == 0)
				continue;

			int current = tab[i][j];
			int k;
			for(k=j+1; k != size; ++k)
				if(tab[i][k] != current)
					break;
			if(k-j >=3) {
				// add them to list
				for(int l = j;l<=k-1;++l) {
					dieList.emplace_back(i, l);
					eliminated[i][l] = true;
				}
				j = k-1;
			}
		}

	// check vertically
	for(int j=0; j != size; ++j)
		for(int i=0; i != size; ++i) {
			if(tab[i][j] == 0)
				continue;

			int current = tab[i][j];
			int k;
			for(k=i; k != size; ++k)
				if(tab[k][j] != current)
					break;
			if(k-i >=3) {
				// add them to list
				for(int l = i;l<=k-1;++l)
					// 避免重复
					if(!eliminated[l][j])
						dieList.emplace_back(l, j);
				i = k-1;
			}
		}
	return dieList;
}
/**
 *
 * @param tab 棋盘二维数组
 * @return list<pair<JewelPos,JewelPos>> res 将要落下的宝石的原本位置和更新后位置组成的list
 */
 //alg
list<pair<JewelPos,JewelPos>> Board::Fall(Board::IntTab& tab)
{
	// record how many blocks a jewel has to fall
	list<pair<JewelPos,JewelPos>> res;
	for(int j=0; j != size; ++j) {
		int count = 0;
		for(int i= size - 1; i >= 0; --i) {
			if(tab[i][j] == 0) {
				++count;
				continue;
			}
			if(count > 0) {
				res.emplace_back(JewelPos(i, j), JewelPos(i + count, j));
				tab[i+count][j] = tab[i][j];
				tab[i][j] = 0;
			}
		}
	}
	return res;
}


BoardEvent::BoardEvent(const BoardEvent::EventType etype):
	type(etype)
{
}
