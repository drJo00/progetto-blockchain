#include "stdafx.h"
#include "Mainer.h"

Mainer::Mainer() { this->genKeyPair(); }

// генерация пары ключей
void Mainer::genKeyPair()
{
	AutoSeededRandomPool prng;
	ECDSA<ECP, SHA256>::PrivateKey privateKey;		// приватный ключ
	ECDSA<ECP, SHA256>::PublicKey publicKey;		// публичный ключ

	privateKey.Initialize(prng, ASN1::secp256k1());	// инициализация приватного ключа по набору secp256k1
	bool result = privateKey.Validate(prng, 3);		// проверка

	privateKey.MakePublicKey(publicKey);			// получение публичного ключа из приватного
	result = publicKey.Validate(prng, 3);			// проверка

	this->keys.privateKey = privateKey;
	this->keys.publicKey = publicKey;
}

// алгоритм создания транзакции и её криптографической подписи
Transaction Mainer::createTx(uint32_t txid, string input, string output, double amount, double fee, uint32_t locktime)
{
	string signature;																	// переменная под подпись
	signature = signMess(sha256(to_string(txid) + input + output + 
		to_string(amount) + to_string(fee) + to_string(locktime)), keys.privateKey);	// подпись хеша от данных транзакции
	Transaction tx(txid, input, output, amount, fee, locktime, signature);				// создание транзакции
	return tx;
}

// алгоритм создания блока
Block Mainer::createBlock(uint32_t height, string prevBlock, time_t timestamp, uint32_t target, vector<Transaction> txs)
{
	unsigned int nonce = 0;																	// перебираемый параметр для вычисления корректного хеша
	string merkleRoot = getMerkleRoot(txs);													// получение корня дерева Меркла списка транзакцицй
	string blockHash = sha256(to_string(height) + prevBlock + merkleRoot +					// вычисление хеша содержимого
		to_string(timestamp) + to_string(target) + to_string(nonce) + to_string(txs.size()));

	// вычисление хеша, соответствующего условию: первые два байта (4 символа в hex записи) - нули
	while (blockHash[0] != 0 || blockHash[1] != 0)
	{
		++nonce;
		blockHash = sha256(to_string(height) + prevBlock + merkleRoot + 
			to_string(timestamp) + to_string(target) + to_string(nonce) + to_string(txs.size()));
	}
	Block block(height, blockHash, prevBlock, merkleRoot, timestamp, target, nonce, txs);	//создание блока
	return block;
}

// построение дерева Меркла от списка транзакций и получение его корня
string Mainer::getMerkleRoot(vector<Transaction> &txs)
{
	vector<string> txsHash(txs.size());			// массив, содержащий хеши транзакций
	vector<string>::iterator it;				// итератор по массиву

	for (int i = 0; i < txsHash.size(); ++i)
		txsHash[i] = txs[i].getHash();			// хеширование данных транзакции и занесение в массив

	// алгоритм построения дерева, используя массив
	while (txsHash.size() > 1)
	{
		it = txsHash.begin();
		while (it < txsHash.end())
		{
			if (it == txsHash.end() - 1)		// если встречен конца массива
				*it = sha256(*it + *it);		// запись конкатенируется сама с собой и хешируется SHA-256
			else
			{
				*it = sha256(*it + *(it + 1));	// конкатенация двух записей и хеширование алгоритмом SHA-256; результат в первую из двух
				txsHash.erase(it + 1);			// удаление второй записи
			}
			it++;
		}
	}
	return txsHash.front();						// в конце алгоритма результат в первом элементе массива
}

// выбор пяти транзакций из пула
vector<Transaction> Mainer::getTxs(vector<Transaction> &txPool)
{
	vector<Transaction> txs;						// массив выбранных транзакций
	if (txPool.size() >= 5)							// если в пуле осталось не менее пяти
		for (int i = 0; i < 5; ++i)
			txs.push_back(txPool.at(i));			// получение из пула пяти транзакций
	else
		for (int i = 0; i < txPool.size(); ++i)		// если менее пяти - получение остатка
			txs.push_back(txPool.at(i));

	auto it = txPool.begin();
	txPool.erase(it, it + txs.size());				// удаление выбранных транзакций из пула

	return txs;
}