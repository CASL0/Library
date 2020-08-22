#ifndef TLSCONNECT_H
#define TLSCONNECT_H

#include <string>

typedef std::basic_string<wchar_t> wstring;

class TLSConnect
{
public:

	TLSConnect(wstring wsUrl);
	virtual ~TLSConnect()=default;
	void open(void);
	void connect(void);
	void close(void);

private:
	class Impl;
	Impl* impl;
};

#endif