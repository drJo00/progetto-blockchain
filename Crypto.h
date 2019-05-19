#pragma once
#include "stdafx.h"
#include "crypto\sha.h"
#include "crypto\hex.h"
#include "crypto\eccrypto.h"
#include "crypto\osrng.h"
#include "crypto\oids.h"
using namespace CryptoPP;

// возвращает строку data в шестнадцатеричном виде
static string toHex(string data)
{
	string encoded;									// строка под результат
	byte const* decoded = (byte*)data.data();		// байтовое представление входной строки
	StringSource ss(decoded, data.size(), true,		// кодирование в HEX формат
		new HexEncoder(new StringSink(encoded)));	 
	return encoded;
}

// возвращает SHA-256 хеш строки data
static string sha256(string data)
{
	byte const* input = (byte*)data.data();				// байтовое представление входной строки
	unsigned int inputLen = data.size();				// длина входной строки
	byte digest[SHA256::DIGESTSIZE];					// массив под хеш

	SHA256().CalculateDigest(digest, input, inputLen);	// вычисление хеша
	/*
	HexEncoder encoder;
	string output;
	encoder.Attach(new StringSink(output));
	encoder.Put(digest, sizeof(digest));
	enoder.MessageEnd();
	*/
	return string((char*)digest, 32);
}

// возвращает подпись сообщения message, подписанного ключом privateKey по алгоритму ECDSA
static string signMess(string message, ECDSA<ECP, SHA256>::PrivateKey privateKey)
{
	ECDSA<ECP, SHA256>::Signer signer(privateKey);		// инициализация подписи

	AutoSeededRandomPool prng;							// случайно генерируемое число
	string signature;									// строка под результат

	// криптографическая подпись данных
	StringSource s(message, true, new SignerFilter(prng, signer, new StringSink(signature)));
	return signature;
}

// возвращает true, если подпись верна, и false в обратном случае
static bool verifyMess(string message, string signature, ECDSA<ECP, SHA256>::PublicKey publicKey)
{
	ECDSA<ECP, SHA256>::Verifier verifier(publicKey);	// инициализация объекта-верификатора
	bool result = false;								// результат проверки

	StringSource ss(signature + message, true,			// проверка сообщения; результат в result
		new SignatureVerificationFilter(verifier, new ArraySink((byte*)&result, sizeof(result))));

	return result;
}