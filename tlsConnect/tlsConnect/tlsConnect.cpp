#include "tlsConnect.h"

#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/ssl.h>
#include <openssl/x509v3.h>

#include <string>
#include <cstdio>
#include <Windows.h>

#pragma comment(lib,"libssl.lib")
#pragma comment(lib,"libcrypto.lib")
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"crypt32.lib")


class TLSConnect::Impl 
{
public:
	void open(void)
	{
		SSL_library_init();
		SSL_load_error_strings();
		ERR_load_BIO_strings();
		SetupSslCtx();
		LoadCertificates();
		SetupBIO();
	}

	virtual void connect(void)
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

protected:
	void TLSerror(const char *msg)
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
};

TLSConnect::TLSConnect(void) :impl(new TLSConnect::Impl) {}

void TLSConnect::Impl::SetupSslCtx(void)
{
	OutputLog("SetupSslCtx");
	if ((m_ctx = SSL_CTX_new(TLS_client_method())) == nullptr)
	{
		TLSerror("  failed in SSL_CTX_new");
	}
	SSL_CTX_set_min_proto_version(m_ctx, TLS1_1_VERSION);
}

void TLSConnect::Impl::LoadCertificates(void)
{
	OutputLog("LoadCertificates");
	HCERTSTORE hStore = CertOpenSystemStore(NULL, L"Root");
	if (hStore == nullptr)
	{
		TLSerror("  failed in CertOpenSystemStore");
	}
	X509* x509=nullptr;
	PCCERT_CONTEXT pCertCtx = nullptr;
	while (pCertCtx = CertEnumCertificatesInStore(hStore, pCertCtx))
	{
		x509 = d2i_X509(NULL, (const unsigned char**)&pCertCtx->pbCertEncoded, pCertCtx->cbCertEncoded);
		
		if (x509)
		{
			int certAddingResult = X509_STORE_add_cert(SSL_CTX_get_cert_store(m_ctx), x509);
			if (certAddingResult == 1)
			{
				OutputLog("  adding certificate");
			}
			X509_free(x509);
		}
	}

	CertFreeCertificateContext(pCertCtx);
	if (!CertCloseStore(hStore, 0))
	{
		TLSerror("  failed in CertCloseStore");
	}

}

void TLSConnect::Impl::SetupBIO(void)
{
	OutputLog("SetupBIO");
	if ((m_bio = BIO_new_ssl_connect(m_ctx)) == nullptr)
	{
		TLSerror("  failed in BIO_new_ssl_connect");
	}
	BIO_get_ssl(m_bio, &m_ssl);
	if (m_ssl == nullptr)
	{
		TLSerror("  failed in BIO_get_ssl");
	}

	SSL_set_mode(m_ssl, SSL_MODE_AUTO_RETRY);
	BIO_set_conn_hostname(m_bio, "www.microsoft.com:443");
}

void TLSConnect::Impl::handshake(void)
{
	if (SSL_connect(m_ssl) <= 0)
	{
		TLSerror("  failed in SSL_connect");
	}
	int certVerifyResult = SSL_get_verify_result(m_ssl);
	if (certVerifyResult != X509_V_OK)
	{
		fprintf(stderr, "  invalid certificate: %s\n", X509_verify_cert_error_string(certVerifyResult));
		TLSerror("  verify error");
	}
	
}

void TLSConnect::Impl::request(void)
{
	char msg[100] = { 0 };
	sprintf_s(msg, 100, "GET /ja-jp HTTP/1.1\r\nHost: www.microsoft.com\r\nConnection: Close\r\n\r\n");
	int ret = SSL_write(m_ssl, msg, strlen(msg));
	if (ret < 1)
	{
		TLSerror("  failed in SSL_write");
	}
	int buf_size = 1024;
	char buf[1024];
	int read_size;

	while ((read_size = SSL_read(m_ssl, buf, 1024 - 1)) > 0)
	{
		fprintf(stderr, "%s", buf);
		memset(buf, 0, sizeof(buf));
	}
}

void TLSConnect::open(void)
{
	impl->open();
}

void TLSConnect::connect(void)
{
	impl->connect();
}

void TLSConnect::close(void)
{
	impl->close();
}