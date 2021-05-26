#include <cstdlib>
#include <ctime>
#include <iostream>
#include "src/headers/logic/board.h"
using namespace Bejeweled;
using namespace std;
/**
 * board.cpp 对棋盘
 * @param size
 */
Board::Board(Size size) :
        board(size, vector<int>(size, 0)),
        generationFactor(1),
        size(size),
        lastPossibleSwap(0, 0),
        count(0)
{

}
/**鼠标滑动滑动宝石触发->mainWindow.onSwap(JewelPos pos, SwapDirection direction)->gameState.Swap()->game.Swap()->board.Swap()
 *进行一次尝试交换产生的事件
 * @param pos 被点击的宝石的位置
 * @param direction 方向
 * @return
 */
list<BoardEvent> Board::Swap(JewelPos pos, SwapDirection direction)
{
	list<BoardEvent> events;
	IntTab tab = board;
	int i = pos.x, j = pos.y;

	//根据交换方向尝试交换
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
	// get list of to-be-eliminated jewel pos
	list<JewelPos> dieList = Eliminatable(tab);
	// if not a valid swap, return blank list
	if(dieList.empty())
		return events;//a invalid swap

	// a valid swap
	board = tab;
	BoardEvent newEvent(BoardEvent::DIE);
    newEvent.setDiePos(dieList);
	events.push_back(newEvent);
	// first elimination
	for(JewelPos pos :dieList)
        board[pos.x][pos.y] = 0;

	do {
		//fall
		newEvent = BoardEvent(BoardEvent::FALL);
        newEvent.setFallPos(Fall(board));
		events.push_back(newEvent);
        dieList = Eliminatable(board);// fall->eliminate->fall->eliminate-> ...
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
void Board::seedToArray(){
    for(int i=0;i<1000000;i++){
       arr[i]=rand()%5+1;
    }

}
/**
 *初始化棋盘
 * @return
 */
BoardEvent Board::Init(int seed)
{   srand(seed);
    seedToArray();
	BoardEvent event(BoardEvent::NEW);
    event.setNewPos(fullFill(board));
	return event;
}

/**
 * 填充棋盘:0->random 1~5
 * 保证有可以消除的位置,且可以自己调整最少可消除的数量
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
//				    tmptab[i][j]=rand()%5+1;
//                  int rand=abs(13*count*count-7*count+3*(count*count-78))%1000;
					tmptab[i][j] = arr[count++];
//					count++;
					if(count==1000000)count=0;//循环使用
					infolist.emplace_back(JewelPos(i,j),tmptab[i][j]);
				}
	} while(!Eliminatable(tmptab).empty()|| PossibleSwap(tmptab) < generationFactor);
	// update
	tab = tmptab;
	return infolist;
}
/**
 *
 * @param tab 棋盘
 * @return int count 有多少可行交换
 */
int Board::PossibleSwap(const IntTab& tab)
{
	IntTab tab2 = tab;//tab2:用于假想交换
	int cnt = 0;
	// swap right
	for(int i=0; i != size; ++i)
		for(int j=0; j != size - 1; ++j) {
			//左右交换
			tab2[i][j] = tab2[i][j+1];
			tab2[i][j+1] = tab[i][j];
			if(!Eliminatable(tab2).empty()) {//如果存在可消除的位置
				//记录
				lastPossibleSwap.x = i;
                lastPossibleSwap.y = j;
				++cnt;
			}
			// backtrack
			tab2[i][j] = tab[i][j];
			tab2[i][j+1] = tab[i][j+1];
		}
	//上下交换
	for(int j=0; j != size; ++j)
		for(int i=0; i != size - 1; ++i) {
			// swap
			tab2[i][j] = tab2[i+1][j];
			tab2[i+1][j] = tab[i][j];
			if(!Eliminatable(tab2).empty()) {
				//record for hint
				lastPossibleSwap.x = i;
                lastPossibleSwap.y = j;
				++cnt;
			}
			// revert changes
			tab2[i][j] = tab[i][j];
			tab2[i+1][j] = tab[i+1][j];
		}
	return cnt;
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
