#pragma once
#include "Crypto.h"
#include "Transaction.h"
#include "Block.h"

class Mainer
{
	struct keyPair									// ���� ����������������� ������ ��������� ECDSA
	{
		ECDSA<ECP, SHA256>::PrivateKey privateKey;	// ��������� ����
		ECDSA<ECP, SHA256>::PublicKey publicKey;	// ��������� ����
	} keys;
	void genKeyPair();
public:
	/*���������� �������*/
	Mainer();
	Transaction createTx(uint32_t txid, string input, string output, double amount, double fee, uint32_t locktime);
	Block createBlock(uint32_t height, string prevBlock, time_t timestamp, uint32_t target, vector<Transaction> txs);
	string getMerkleRoot(vector<Transaction> &txs);
	vector<Transaction> getTxs(vector<Transaction> &txPool);
};
