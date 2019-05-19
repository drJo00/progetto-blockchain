
#pragma once
#include "stdafx.h"
#include "Transaction.h"

class Block
{
	unsigned int blockHeight;	// ���������� ����� �����
	string dataHash;			// SHA-256 ��� ������
	string prevBlock;			// SHA-256 ��� ����������� �����
	string merkleRoot;			// ������ ������ ������ ����������
	time_t timestamp;			// UNIX ������ ���� � ������� �������� �����
	unsigned int bits;			// ����� ��� ���������� ��������� nonce (���������� ���������)
	unsigned int nonce;			// ��������� ������������ ����� ��� ���������� ����
	unsigned int txsCount;		// ���������� ���������� � �����
	vector<Transaction> txs;	// ������ ���������� � �����
public:
	/* ���������� ������� */
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