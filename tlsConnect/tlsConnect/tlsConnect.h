#ifndef TLSCONNECT_H
#define TLSCONNECT_H

class TLSConnect
{
public:

	TLSConnect(void);
	virtual ~TLSConnect(void) {};
	void open(void);
	void connect(void);
	void close(void);

private:
	class Impl;
	Impl* impl;
};

#endif