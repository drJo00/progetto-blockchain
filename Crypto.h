#pragma once
#include "stdafx.h"
#include "crypto\sha.h"
#include "crypto\hex.h"
#include "crypto\eccrypto.h"
#include "crypto\osrng.h"
#include "crypto\oids.h"
using namespace CryptoPP;

// ���������� ������ data � ����������������� ����
static string toHex(string data)
{
	string encoded;									// ������ ��� ���������
	byte const* decoded = (byte*)data.data();		// �������� ������������� ������� ������
	StringSource ss(decoded, data.size(), true,		// ����������� � HEX ������
		new HexEncoder(new StringSink(encoded)));	 
	return encoded;
}

// ���������� SHA-256 ��� ������ data
static string sha256(string data)
{
	byte const* input = (byte*)data.data();				// �������� ������������� ������� ������
	unsigned int inputLen = data.size();				// ����� ������� ������
	byte digest[SHA256::DIGESTSIZE];					// ������ ��� ���

	SHA256().CalculateDigest(digest, input, inputLen);	// ���������� ����
	/*
	HexEncoder encoder;
	string output;
	encoder.Attach(new StringSink(output));
	encoder.Put(digest, sizeof(digest));
	enoder.MessageEnd();
	*/
	return string((char*)digest, 32);
}

// ���������� ������� ��������� message, ������������ ������ privateKey �� ��������� ECDSA
static string signMess(string message, ECDSA<ECP, SHA256>::PrivateKey privateKey)
{
	ECDSA<ECP, SHA256>::Signer signer(privateKey);		// ������������� �������

	AutoSeededRandomPool prng;							// �������� ������������ �����
	string signature;									// ������ ��� ���������

	// ����������������� ������� ������
	StringSource s(message, true, new SignerFilter(prng, signer, new StringSink(signature)));
	return signature;
}

// ���������� true, ���� ������� �����, � false � �������� ������
static bool verifyMess(string message, string signature, ECDSA<ECP, SHA256>::PublicKey publicKey)
{
	ECDSA<ECP, SHA256>::Verifier verifier(publicKey);	// ������������� �������-������������
	bool result = false;								// ��������� ��������

	StringSource ss(signature + message, true,			// �������� ���������; ��������� � result
		new SignatureVerificationFilter(verifier, new ArraySink((byte*)&result, sizeof(result))));

	return result;
}