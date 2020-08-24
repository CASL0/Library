#ifndef TLSCONNECT_H
#define TLSCONNECT_H

#include <string>
#include <wchar.h>

typedef std::basic_string<wchar_t> wstring;

class CTLSConnect
{
public:

	CTLSConnect(wstring wsUrl);
	virtual ~CTLSConnect()=default;
	void open(void);
	void connect(void);
	void close(void);

private:
	class Impl;
	Impl* pimpl;
};

#endif