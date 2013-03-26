#include <iostream>
#include "DeltaPS.h"

int main()
{
	try
	{
		PSC_ETH ps("130.235.95.231");

		std::cout << ps.idn();
		std::cout << ps.max_voltage() << std::endl;
	}
	catch (const yat::SocketException &se)
	{
		std::cout << "*** yat::SocketException caught ***" << std::endl;

	    for (size_t err = 0; err  < se.errors.size(); err++)
	    {
	      std::cout << "Err-" << err << "::reason..." << se.errors[err].reason << std::endl;
	      std::cout << "Err-" << err << "::desc....." << se.errors[err].desc << std::endl;
	      std::cout << "Err-" << err << "::origin..." << se.errors[err].origin << std::endl;
	      std::cout << "Err-" << err << "::code....." << se.errors[err].code << std::endl;
	    }
	    return -1;
	}
	catch (...)
	{
		std::cout << "Unknown exception caught" << std::endl;
		return -1;
	}
	return 0;
}
