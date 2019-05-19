#include "stdafx.h"
#include "Block.h"

Block::Block(uint32_t blockHeight, string dataHash, string prevBlock, string merkleRoot, time_t timestamp, uint32_t bits, uint32_t nonce, vector<Transaction> txs)
	: blockHeight(blockHeight), dataHash(dataHash), prevBlock(prevBlock), merkleRoot(merkleRoot), 
		timestamp(timestamp), bits(bits), nonce(nonce), txsCount(txs.size()), txs(txs) {}

/* функции получения различных данных блока */
unsigned int Block::getHeight() { return blockHeight; }

string Block::getHash() { return dataHash; }

string Block::getPrevHash() { return prevBlock; }

string Block::getRoot() { return merkleRoot; }

time_t Block::getTimestamp() { return timestamp; }

unsigned int Block::getBits() { return bits; }

unsigned int Block::getNonce() { return nonce; }

unsigned int Block::getTxsCount() { return txsCount; }

vector<Transaction> Block::getTxs() { return txs; }