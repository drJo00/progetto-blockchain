// ProgettoBlockchain.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "Crypto.h"
#include "Mainer.h"
#include "Chain.h"

/* загрузка списка транзакций из файла */
vector<Transaction> loadFromFile(Mainer &miner)
{
	string filename = "transactions.txt", str;
	unsigned int pos = 0;
	vector<Transaction> txs;						// массив для хранения считанных транзакций
	ifstream txFile(filename);						// открытие файла

	if (!txFile.is_open())							// если файл не найден, вернуть пустой массив
		return txs;

	// считывание данных построчно, занесение во временные переменные и формирование транзакции
	while (getline(txFile, str))
	{
		unsigned int txid = stoi(str.substr(0, pos = str.find(';')));
		string input = str.substr(++pos, str.find(';', pos) - pos);
		string output = str.substr(pos += input.size() + 1, str.find(';', pos) - pos);
		string amount = str.substr(pos += output.size() + 1, str.find(';', pos) - pos);
		string fee = str.substr(pos += amount.size() + 1, str.find(';', pos) - pos);
		unsigned int locktime = stoi(str.substr(pos += fee.size() + 1));

		txs.push_back(miner.createTx(txid, input, output, stof(amount), stof(fee), locktime));
	}
	txFile.close();									// закрытие файла
	return txs;
}

int main()
{
	Chain ch;											// цепь блоков
	Mainer miner;										// майнер - управляющий  объект

	cout << "Parsing transactions..." << endl;
	vector<Transaction> txPool = loadFromFile(miner);	// загрузка транзакций в пул
	cout << "Done." << endl;

	cout << "Calculating blocks..." << endl;
	while (txPool.size() > 0)							// пока в пуле есть транзакции, формирование блоков и запись в цепь
		ch.addBlock(miner.createBlock(ch.getPrevHeight() + 1, ch.getPrevHash(), time(nullptr), ch.getTarget(), miner.getTxs(txPool)));
	cout << "Done." << endl;

	cout << "Saving blocks into file..." << endl;
	ch.saveToFile();									// сохранение цепочки в файл
	cout << "Done." << endl;

	system("PAUSE");
    return 0;
}

