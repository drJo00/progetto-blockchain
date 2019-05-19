#pragma once
#include "stdafx.h"

class Transaction
{
	unsigned int txid;			// id ����������
	string input;				// ����� ��������, � �������� ���������� ��������
	string output;				// ����� ��������, �� ������� ���������� ��������
	double amount;				// ����� ��������
	double fee;					// �������� �� �������
	unsigned int locktime;		// ����� ����� (< 5000000), ������� � �������� ����� ������������ (0 - �����)
	string signature;			// ����������������� ������� ����������

public:
	/* ���������� ������� */
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