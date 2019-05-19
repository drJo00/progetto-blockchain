#include "stdafx.h"
#include "Chain.h"
#include "Crypto.h"

Chain::Chain() { this->setTarget(); }

Chain::~Chain() {}

// генерация параметра target (bits в блоке)
void Chain::setTarget() { target = rand() * rand(); }

// функция добавление блока в цепь
void Chain::addBlock(Block newBlock)
{
	chain.push_back(newBlock);
	setTarget();							// target меняется после каждого добавления
}

// операция сохранения данных блоков в файл
void Chain::saveToFile()
{
	if (chain.size() > 0)
	{
		string filename;
		for (int i = 0; i < chain.size(); ++i)
		{
			filename = "block" + to_string(i) + ".blc";
			fstream blockFile(filename, ios::out);
			blockFile << "=========================Block Head=========================" << endl;
			blockFile << "Block SHA-256 hash: " << toHex(chain[i].getHash()) << endl;
			blockFile << "Block height: " << chain[i].getHeight() << endl;
			if (i)
				blockFile << "Previous block SHA-256 hash: " << toHex(chain[i].getPrevHash()) << endl;
			else blockFile << "Previous block SHA-256 hash: Genesis block" << endl;
			blockFile << "Merkle root: " << toHex(chain[i].getRoot()) << endl;
			time_t time = chain[i].getTimestamp();
			/*
			struct tm *buf = nullptr;
			localtime_s(buf, &time);
			*/
			blockFile << "Creation time: " << put_time(localtime(&time), "%e-%m-%Y %H:%M:%S") << endl;
			blockFile << "Bits: " << chain[i].getBits() << endl;
			blockFile << "Nonce: " << chain[i].getNonce() << endl;
			blockFile << "Transactions count: " << chain[i].getTxsCount() << endl;
			blockFile << "========================Transactions========================" << endl;

			vector<Transaction> txs = chain[i].getTxs();
			for (int i = 0; i < txs.size(); ++i)
			{
				blockFile << "Transaction id: " << txs[i].getTxid() << endl;
				blockFile << "SHA-256 hash: " << toHex(txs[i].getHash()) << endl;
				blockFile << "Input: " << txs[i].getInput() << endl;
				blockFile << "Output: " << txs[i].getOutput() << endl;
				blockFile << "Amount: " << txs[i].getAmount() << " BTC" << endl;
				blockFile << "Fee: " << txs[i].getFee() << " BTC" << endl;
				blockFile << "Locktime: " << txs[i].getLocktime() << endl;
				blockFile << "Signature: " << toHex(txs[i].getSignature()) << endl;
				blockFile << "------------------------------------------------------------" << endl;
			}
			blockFile.close();
		}
	}
}

unsigned int Chain::getTarget() { return target; }

// высота предыдущего блока в сети
unsigned int Chain::getPrevHeight()
{ 
	unsigned int res;
	chain.size() > 0 ? res = chain.back().getHeight() : res = 0;
	return res;
}

// хеш предыдущего блока в сети
string Chain::getPrevHash()
{ 
	string res;
	chain.size() > 0 ? res = chain.back().getHash() : res = "0";
	return res;
}