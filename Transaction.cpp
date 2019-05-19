#include "stdafx.h"
#include "Transaction.h"
#include "Crypto.h"

Transaction::Transaction(uint32_t txid, string input, string output, double amount, double fee, uint32_t locktime, string signature)
	: txid(txid), input(input), output(output), amount(amount), fee(fee), locktime(locktime), signature(signature) {}

/* ������� ��������� ��������� ������ ���������� */
unsigned int Transaction::getTxid() { return txid; }

string Transaction::getInput() { return input; }

string Transaction::getOutput() { return output; }

double Transaction::getAmount() { return amount; }

double Transaction::getFee() { return fee; }

unsigned int Transaction::getLocktime() { return locktime; }

string Transaction::getSignature() { return signature; }

// ��������� ���� ������ ����������
string Transaction::getHash()
{
	return sha256(to_string(txid) + input + output +
		to_string(amount) + to_string(fee) + to_string(locktime));
}