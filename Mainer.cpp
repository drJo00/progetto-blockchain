#include "stdafx.h"
#include "Mainer.h"

Mainer::Mainer() { this->genKeyPair(); }

// ��������� ���� ������
void Mainer::genKeyPair()
{
	AutoSeededRandomPool prng;
	ECDSA<ECP, SHA256>::PrivateKey privateKey;		// ��������� ����
	ECDSA<ECP, SHA256>::PublicKey publicKey;		// ��������� ����

	privateKey.Initialize(prng, ASN1::secp256k1());	// ������������� ���������� ����� �� ������ secp256k1
	bool result = privateKey.Validate(prng, 3);		// ��������

	privateKey.MakePublicKey(publicKey);			// ��������� ���������� ����� �� ����������
	result = publicKey.Validate(prng, 3);			// ��������

	this->keys.privateKey = privateKey;
	this->keys.publicKey = publicKey;
}

// �������� �������� ���������� � � ����������������� �������
Transaction Mainer::createTx(uint32_t txid, string input, string output, double amount, double fee, uint32_t locktime)
{
	string signature;																	// ���������� ��� �������
	signature = signMess(sha256(to_string(txid) + input + output + 
		to_string(amount) + to_string(fee) + to_string(locktime)), keys.privateKey);	// ������� ���� �� ������ ����������
	Transaction tx(txid, input, output, amount, fee, locktime, signature);				// �������� ����������
	return tx;
}

// �������� �������� �����
Block Mainer::createBlock(uint32_t height, string prevBlock, time_t timestamp, uint32_t target, vector<Transaction> txs)
{
	unsigned int nonce = 0;																	// ������������ �������� ��� ���������� ����������� ����
	string merkleRoot = getMerkleRoot(txs);													// ��������� ����� ������ ������ ������ �����������
	string blockHash = sha256(to_string(height) + prevBlock + merkleRoot +					// ���������� ���� �����������
		to_string(timestamp) + to_string(target) + to_string(nonce) + to_string(txs.size()));

	// ���������� ����, ���������������� �������: ������ ��� ����� (4 ������� � hex ������) - ����
	while (blockHash[0] != 0 || blockHash[1] != 0)
	{
		++nonce;
		blockHash = sha256(to_string(height) + prevBlock + merkleRoot + 
			to_string(timestamp) + to_string(target) + to_string(nonce) + to_string(txs.size()));
	}
	Block block(height, blockHash, prevBlock, merkleRoot, timestamp, target, nonce, txs);	//�������� �����
	return block;
}

// ���������� ������ ������ �� ������ ���������� � ��������� ��� �����
string Mainer::getMerkleRoot(vector<Transaction> &txs)
{
	vector<string> txsHash(txs.size());			// ������, ���������� ���� ����������
	vector<string>::iterator it;				// �������� �� �������

	for (int i = 0; i < txsHash.size(); ++i)
		txsHash[i] = txs[i].getHash();			// ����������� ������ ���������� � ��������� � ������

	// �������� ���������� ������, ��������� ������
	while (txsHash.size() > 1)
	{
		it = txsHash.begin();
		while (it < txsHash.end())
		{
			if (it == txsHash.end() - 1)		// ���� �������� ����� �������
				*it = sha256(*it + *it);		// ������ ��������������� ���� � ����� � ���������� SHA-256
			else
			{
				*it = sha256(*it + *(it + 1));	// ������������ ���� ������� � ����������� ���������� SHA-256; ��������� � ������ �� ����
				txsHash.erase(it + 1);			// �������� ������ ������
			}
			it++;
		}
	}
	return txsHash.front();						// � ����� ��������� ��������� � ������ �������� �������
}

// ����� ���� ���������� �� ����
vector<Transaction> Mainer::getTxs(vector<Transaction> &txPool)
{
	vector<Transaction> txs;						// ������ ��������� ����������
	if (txPool.size() >= 5)							// ���� � ���� �������� �� ����� ����
		for (int i = 0; i < 5; ++i)
			txs.push_back(txPool.at(i));			// ��������� �� ���� ���� ����������
	else
		for (int i = 0; i < txPool.size(); ++i)		// ���� ����� ���� - ��������� �������
			txs.push_back(txPool.at(i));

	auto it = txPool.begin();
	txPool.erase(it, it + txs.size());				// �������� ��������� ���������� �� ����

	return txs;
}