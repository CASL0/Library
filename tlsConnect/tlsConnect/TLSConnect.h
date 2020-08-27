#ifndef TLSCONNECT_H
#define TLSCONNECT_H

#include <string>

class CTLSConnect
{
public:

	CTLSConnect(std::wstring wsUrl);
	virtual ~CTLSConnect()=default;
	void open(void);
	void connect(void);
	void close(void);

private:
	class Impl;
	Impl* pimpl;
};

#endif