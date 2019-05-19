#pragma once
#include "Block.h"

class Chain
{
	vector<Block> chain;		// ����� - ���� ������
	unsigned int target;		// �������� target, ������������ ���������
	void setTarget();			// �������������� � ��������� target
public:
	/* ���������� ������� */
	Chain();
	~Chain();
	void addBlock(Block newBlock);
	void saveToFile();
	unsigned int getTarget();
	unsigned int getPrevHeight();
	string getPrevHash();
};

