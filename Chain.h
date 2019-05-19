#pragma once
#include "Block.h"

class Chain
{
	vector<Block> chain;		// масив - цепь блоков
	unsigned int target;		// параметр target, определ€ющий сложность
	void setTarget();			// инициализацич€ и изменение target
public:
	/* объ€влени€ функций */
	Chain();
	~Chain();
	void addBlock(Block newBlock);
	void saveToFile();
	unsigned int getTarget();
	unsigned int getPrevHeight();
	string getPrevHash();
};

