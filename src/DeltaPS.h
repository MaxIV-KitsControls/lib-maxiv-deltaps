#ifndef _DELTAPS_H_
#define _DELTAPS_H_

#include <yat/network/ClientSocket.h>

#define SOCK_PORT 8462

class PSC_ETH
{
public:
	PSC_ETH(const std::string ip_address);

	virtual ~PSC_ETH();

	std::string idn();
	double max_voltage();

private:
	yat::ClientSocket sock;
};

#endif // _DELTAPS_H_
