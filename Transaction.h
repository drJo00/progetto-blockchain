#pragma once
#include "stdafx.h"

class Transaction
{
	unsigned int txid;			// id транзакции
	string input;				// адрес кошелька, с которого переведены средства
	string output;				// адрес кошелька, на который переведены средства
	double amount;				// сумма перевода
	double fee;					// комисси€ за перевод
	unsigned int locktime;		// номер блока (< 5000000), начина€ с которого можно использовать (0 - сразу)
	string signature;			// криптографическа€ подпись транзакции

public:
	/* объ€влени€ функций */
	Transaction(uint32_t txid, string input, string output, double amount, double fee, uint32_t locktime, string signature);
	unsigned int getTxid();
	string getInput();
	string getOutput();
	double getAmount();
	double getFee();
	unsigned int getLocktime();
	string getSignature();
	string getHash();
};