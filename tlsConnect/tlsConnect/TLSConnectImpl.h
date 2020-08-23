#ifndef TLSCONNECTIMPL_H
#define TLSCONNECTIMPL_H

#include "tlsConnect.h"

#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/ssl.h>
#include <openssl/x509v3.h>

#include <string>
#include <cstdio>
#include <Windows.h>
#include <winhttp.h>

class TLSConnect::Impl
{
public:
	Impl(wstring wsUrl)
	{
		ParseUrl(wsUrl);
	}

	void open(void)
	{
		SSL_library_init();
		SSL_load_error_strings();
		ERR_load_BIO_strings();
		SetupSslCtx();
		LoadCertificates();
		SetupBIO();
	}

	void connect(void)
	{
		OutputLog("connect");
		handshake();
		request();
	}

	void close(void)
	{
		OutputLog("close");

		SSL_set_shutdown(m_ssl, SSL_SENT_SHUTDOWN);
		SSL_shutdown(m_ssl);
		BIO_free_all(m_bio);
		SSL_CTX_free(m_ctx);
		ERR_free_strings();
	}

private:
	virtual void SetupSslCtx(void);
	virtual void LoadCertificates(void);
	virtual void SetupBIO(void);
	virtual void handshake(void);
	virtual void request(void);
	virtual void ParseUrl(wstring wsUrl);

	void TLSerror(const char* msg)
	{
		fprintf(stderr, "%s: %i: %s\n", __FILE__, __LINE__, msg);
		ERR_print_errors_fp(stderr);
		exit(-1);
	}

	void OutputLog(const char* msg)
	{
		perror(msg);
	}

private:
	SSL_CTX* m_ctx;
	SSL* m_ssl;
	BIO* m_bio;
	URL_COMPONENTS m_urlComp;
};

#endif