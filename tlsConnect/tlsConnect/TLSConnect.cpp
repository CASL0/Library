#include "TLSConnect.h"
#include "TLSConnectImpl.h"

CTLSConnect::CTLSConnect(std::wstring wsUrl) :pimpl(new Impl(wsUrl)) {}

void CTLSConnect::open(void)
{
	pimpl->open();
}

void CTLSConnect::connect(void)
{
	pimpl->connect();
}

void CTLSConnect::close(void)
{
	pimpl->close();
}