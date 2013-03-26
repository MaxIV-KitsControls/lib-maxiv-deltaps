#include "DeltaPS.h"

#define LF "\10"

PSC_ETH::PSC_ETH(const std::string ip_address) {
    yat::Socket::init();

    //- set some socket option
    sock.set_option(yat::Socket::SOCK_OPT_KEEP_ALIVE, 1);
    sock.set_option(yat::Socket::SOCK_OPT_NO_DELAY, 1);
    sock.set_option(yat::Socket::SOCK_OPT_OTIMEOUT, 1000);
    sock.set_option(yat::Socket::SOCK_OPT_ITIMEOUT, 1000);

    yat::Address addr(ip_address, SOCK_PORT);
    sock.connect(addr);
}

PSC_ETH::~PSC_ETH() {
	sock.disconnect();
    yat::Socket::terminate();
}

std::string PSC_ETH::idn() {
	std::string result;

	sock << "*idn?\n";
	sock >> result;

	return result;
}

double PSC_ETH::max_voltage() {
	std::string reply;
	double d;

	sock << "source:voltage:maximum?\n";
	sock >> reply;

	std::istringstream i(reply);
	i >> d;
	return d;
}
