#include "TLSConnect.h"
#include "TLSConnectImpl.h"


#pragma comment(lib,"libssl.lib")
#pragma comment(lib,"libcrypto.lib")
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"crypt32.lib")
#pragma comment(lib,"winhttp.lib")

#define URL_LENGTH 256


void CTLSConnect::Impl::SetupSslCtx(void)
{
	OutputLog("SetupSslCtx");
	if ((m_ctx = SSL_CTX_new(TLS_client_method())) == nullptr)
	{
		TLSerror(__FILE__,__LINE__,"  failed in SSL_CTX_new");
	}
}

void CTLSConnect::Impl::LoadCertificates(void)
{
	OutputLog("LoadCertificates");
	HCERTSTORE hStore = CertOpenSystemStore(NULL, L"Root");
	if (hStore == nullptr)
	{
		TLSerror(__FILE__,__LINE__,"  failed in CertOpenSystemStore");
	}
	X509* x509=nullptr;
	PCCERT_CONTEXT pCertCtx = nullptr;
	while (pCertCtx = CertEnumCertificatesInStore(hStore, pCertCtx))
	{
		x509 = d2i_X509(nullptr, (const unsigned char**)&pCertCtx->pbCertEncoded, pCertCtx->cbCertEncoded);
		
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
		TLSerror(__FILE__,__LINE__,"  failed in CertCloseStore");
	}

}

void CTLSConnect::Impl::SetupBIO(void)
{
	OutputLog("SetupBIO");
	if ((m_bio = BIO_new_ssl_connect(m_ctx)) == nullptr)
	{
		TLSerror(__FILE__,__LINE__,"  failed in BIO_new_ssl_connect");
	}
	BIO_get_ssl(m_bio, &m_ssl);
	if (m_ssl == nullptr)
	{
		TLSerror(__FILE__,__LINE__,"  failed in BIO_get_ssl");
	}

	SSL_set_mode(m_ssl, SSL_MODE_AUTO_RETRY);
	char buff[URL_LENGTH] = { 0 };
	sprintf_s(buff, sizeof(buff), "%ls:443", m_urlComp.lpszHostName);
	BIO_set_conn_hostname(m_bio,buff);
}

void CTLSConnect::Impl::SetTlsExtensions(void)
{
	char host[URL_LENGTH] = { 0 };
	sprintf_s(host, sizeof(host), "%ls", m_urlComp.lpszHostName);
	SSL_set_tlsext_host_name(m_ssl, host);
}

void CTLSConnect::Impl::handshake(void)
{
	if (SSL_connect(m_ssl) < 1)
	{
		TLSerror(__FILE__,__LINE__,"  failed in SSL_connect");
	}
	int certVerifyResult = SSL_get_verify_result(m_ssl);
	if (certVerifyResult != X509_V_OK)
	{
		std::cerr << "  invalid certificate: " << X509_verify_cert_error_string(certVerifyResult) << std::endl;
		TLSerror(__FILE__,__LINE__,"  verify error");
	}
	
}

void CTLSConnect::Impl::request(void)
{
	char msg[256] = { 0 };
	sprintf_s(msg, 256, "GET %ls HTTP/1.1\r\nHost: %ls\r\nConnection: Close\r\n\r\n",m_urlComp.lpszUrlPath,m_urlComp.lpszHostName);
	int ret = SSL_write(m_ssl, msg, strlen(msg));
	if (ret < 1)
	{
		TLSerror(__FILE__,__LINE__,"  failed in SSL_write");
	}

	char buf[1024];
	int read_size=0;

	const char* outFileName = "file.html";
	std::ofstream ofStream(outFileName);
	while ((read_size = SSL_read(m_ssl, buf, 1024 - 1)) > 0)
	{
		ofStream << buf;
		memset(buf, 0, sizeof(buf));
	}
}

void CTLSConnect::Impl::ParseUrl(std::wstring wsUrl)
{
	ZeroMemory(&m_urlComp, sizeof(m_urlComp));
	m_urlComp.dwStructSize = sizeof(m_urlComp);

	static wchar_t schema[URL_LENGTH];
	static wchar_t hostName[URL_LENGTH];
	static wchar_t path[URL_LENGTH];
	static wchar_t parameter[URL_LENGTH];

	m_urlComp.dwSchemeLength = (DWORD)-1;
	m_urlComp.dwHostNameLength = (DWORD)-1;
	m_urlComp.dwUrlPathLength = (DWORD)-1;
	m_urlComp.dwExtraInfoLength = (DWORD)-1;

	m_urlComp.lpszScheme = schema;
	m_urlComp.lpszHostName = hostName;
	m_urlComp.lpszUrlPath = path;
	m_urlComp.lpszExtraInfo = parameter;

	if (!WinHttpCrackUrl(wsUrl.c_str(), wsUrl.length(), 0, &m_urlComp))
	{
		fprintf(stderr,"Error %u in WinHttpCrackUrl.\n", GetLastError());
		exit(1);
	}
}

