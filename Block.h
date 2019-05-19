
#pragma once
#include "stdafx.h"
#include "Transaction.h"

class Block
{
	unsigned int blockHeight;	// уникальный номер блока
	string dataHash;			// SHA-256 хеш данных
	string prevBlock;			// SHA-256 хеш предыдущего блока
	string merkleRoot;			// корень дерева ћеркла транзакций
	time_t timestamp;			// UNIX формат даты и времени создани€ блока
	unsigned int bits;			// число дл€ вычислени€ параметра nonce (определ€ет сложность)
	unsigned int nonce;			// случайное перебираемое число дл€ вычислени€ хеша
	unsigned int txsCount;		// количество транзакций в блоке
	vector<Transaction> txs;	// массив транзакций в блоке
public:
	/* объ€влени€ функций */
	Block(uint32_t blockHeight, string dataHash, string prevBlock, string merkleRoot, time_t timestamp, uint32_t bits, uint32_t nonce, vector<Transaction> txs);
	unsigned int getHeight();
	string getHash();
	string getPrevHash();
	string getRoot();
	time_t getTimestamp();
	unsigned int getBits();
	unsigned int getNonce();
	unsigned int getTxsCount();
	vector<Transaction> getTxs();
};