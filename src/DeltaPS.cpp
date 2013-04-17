#include "DeltaPS.h"

#define LF "\10"

PSC_ETH::PSC_ETH(const std::string ip_address) {
	this->ip_address = ip_address;
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

std::string PSC_ETH::addrIP()
{
	return ip_address;
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
//---------------------------------------------------------------------------
//Read the Status:Operation:Regulating:Condition register
//bit 1 = Constant current
//---------------------------------- 
bool PSC_ETH::get_current_state(void) throw (yat::Exception)
{
try{
	std::string reply;
	int d;
	bool state = false;

	sock << "status:operation:regulating:condition?\n";
	sock >> reply;

	std::istringstream i(reply);
	i >> d;
	if (d > 1)
		state = true;
	return state;
}
catch(yat::Exception &e)
{
	std::ostringstream Desc;
	Desc<<"YAT exception caught on magnet with ip: "<<this->ip_address<<std::ends;
	e.push_error("State could not be read",Desc.str(), "PSC_ETH::get_current_state");
	throw e;
}
catch(...)
{
	std::ostringstream Desc;
	Desc<<"Unknown exception caught on magnet with ip: "<<this->ip_address<<std::ends;
	yat::Exception e("Unknown error",Desc.str(),"PSC_ETH::get_current_state");
	throw e; 
}
}
//---------------------------------------------------------------------------
//Read the Status:Operation:Regulating:Condition register
//bit 0 = Constant voltage
//---------------------------------- 
bool PSC_ETH::get_voltage_state(void) throw (yat::Exception)
{
try{
	std::string reply;
	int d;
	bool state = false;

	sock << "status:operation:regulating:condition?\n";
	sock >> reply;

	std::istringstream i(reply);
	i >> d;
	if(d == 1 || d == 3)
		state = true;
	
	return state;
}
catch(yat::Exception &e)
{
	std::ostringstream Desc;
	Desc<<"YAT exception caught on magnet with ip: "<<this->ip_address<<std::ends;
	e.push_error("State could not be read",Desc.str(), "PSC_ETH::get_voltage_state");
	throw e;
}
catch(...)
{
	std::ostringstream Desc;
	Desc<<"Unknown exception caught on magnet with ip: "<<this->ip_address<<std::ends;
	yat::Exception e("Unknown error",Desc.str(),"PSC_ETH::get_voltage_state");
	throw e; 
}
}
//---------------------------------------------------------------------------
//Read the Status:Operation:Shutdown:Condition register
//bit 0 = protection summary
//bit 1 = interlock
//bit 2 = RSD
//bit 3 = Output On/Off
//---------------------------------- 
int PSC_ETH::get_state(void) throw (yat::Exception)
{
try{
	std::string reply;
	int d;

	sock << "status:operation:shutdown:condition?\n";
	sock >> reply;

	std::istringstream i(reply);
	i >> d;
	//TODO: split reply bitwise
	return d;
}
catch(yat::Exception &e)
{
	std::ostringstream Desc;
	Desc<<"YAT exception caught on magnet with ip: "<<this->ip_address<<std::ends;
	e.push_error("State could not be read",Desc.str(), "PSC_ETH::get_state");
	throw e;
}
catch(...)
{
	std::ostringstream Desc;
	Desc<<"Unknown exception caught on magnet with ip: "<<this->ip_address<<std::ends;
	yat::Exception e("Unknown error",Desc.str(),"PSC_ETH::get_state");
	throw e; 
}
}
//---------------------------------------------------------------------------
//Set Output state to On or Off
//------------------------------
void PSC_ETH::set_state(bool val) throw (yat::Exception)
{
try{
	std::ostringstream oss;

	oss<<"output" << val << "\n"; 
	sock << oss.str();
}
catch(yat::Exception &e)
{
	std::ostringstream Desc;
	Desc<<"YAT exception caught on magnet with ip: "<<this->ip_address<<std::ends;
	e.push_error("State could not be set",Desc.str(), "PSC_ETH::set_state");
	throw e;
}
catch(...)
{
	std::ostringstream Desc;
	Desc<<"Unknown exception caught on magnet with ip: "<<this->ip_address<<std::ends;
	yat::Exception e("Unknown error",Desc.str(),"PSC_ETH::set_state");
	throw e; 
}
}
//---------------------------------------------------------------------------------
std::string PSC_ETH::read_error(void) throw (yat::Exception)
{
try{
	std::ostringstream oss;

	oss<<"system:error?\n";

	for(int i=0;i<5;i++)
	{
		sock << oss.str();	
	}
	std::string result;

	sock << "system:error?\n";
	sock >> result;

	return result;
}
catch(yat::Exception &e)
{
	std::ostringstream Desc;
	Desc<<"YAT exception caught on magnet with ip: "<<this->ip_address<<std::ends;
	e.push_error("Error could not be read",Desc.str(), "PSC_ETH::clear_all_err");
	throw e;
}
catch(...)
{
	std::ostringstream Desc;
	Desc<<"Unknown exception caught on magnet with ip: "<<this->ip_address<<std::ends;
	yat::Exception e("Unknown error",Desc.str(),"PSC_ETH::clear_all_err");
	throw e; 
}
}
//---------------------------------------------------------------------------
//Empty error queue (has 5 slots)
//----------------------------------
void PSC_ETH::clear_all_err(void) throw (yat::Exception)
{
try{
	
	for(int i=0;i<5;i++)
	{
		sock << "system:error?\n";	
	}
}
catch(yat::Exception &e)
{
	std::ostringstream Desc;
	Desc<<"YAT exception caught on magnet with ip: "<<this->ip_address<<std::ends;
	e.push_error("Error could not be read",Desc.str(), "PSC_ETH::clear_all_err");
	throw e;
}
catch(...)
{
	std::ostringstream Desc;
	Desc<<"Unknown exception caught on magnet with ip: "<<this->ip_address<<std::ends;
	yat::Exception e("Unknown error",Desc.str(),"PSC_ETH::clear_all_err");
	throw e; 
}
}
//---------------------------------------------------------------------------
//Get the measured current
//----------------------------------
double PSC_ETH::get_measure_current(void) throw (yat::Exception)
{
try{
	std::string reply;
	double d;

	sock << "measure:current?\n";
	sock >> reply;

	std::istringstream i(reply);
	i >> d;
	return d;
}
catch(yat::Exception &e)
{
	std::ostringstream Desc;
	Desc<<"YAT exception caught on magnet with ip: "<<this->ip_address<<std::ends;
	e.push_error("Measured current could not be read",Desc.str(), "PSC_ETH::get_measure_current");
	throw e;
}
catch(...)
{
	std::ostringstream Desc;
	Desc<<"Unknown exception caught on magnet with ip: "<<this->ip_address<<std::ends;
	yat::Exception e("Unknown error",Desc.str(),"PSC_ETH::get_measure_current");
	throw e; 
}
}
//---------------------------------------------------------------------------
//Get the measured voltage
//----------------------------------
double PSC_ETH::get_measure_voltage(void) throw (yat::Exception)
{
try{
	std::string reply;
	double d;

	sock << "measure:voltage?\n";
	sock >> reply;

	std::istringstream i(reply);
	i >> d;
	return d;
}
catch(yat::Exception &e)
{
	std::ostringstream Desc;
	Desc<<"YAT exception caught on magnet with ip: "<<this->ip_address<<std::ends;
	e.push_error("Measured voltage could not be read",Desc.str(), "PSC_ETH::get_measure_voltage");
	throw e;
}
catch(...)
{
	std::ostringstream Desc;
	Desc<<"Unknown exception caught on magnet with ip: "<<this->ip_address<<std::ends;
	yat::Exception e("Unknown error",Desc.str(),"PSC_ETH::get_measure_voltage");
	throw e; 
}
}
//---------------------------------------------------------------------------
//Set the current
//---------------------------------- 
void PSC_ETH::set_current(double ValF) throw (yat::Exception)
{
try{
	std::ostringstream oss;

	oss<<"source:current "<<ValF<<"\n";

	sock << oss.str();
}
catch(yat::Exception &e)
{
	std::ostringstream Desc;
	Desc<<"YAT exception caught on magnet with ip: "<<this->ip_address<<std::ends;
	e.push_error("Current could not be set",Desc.str(), "PSC_ETH::set_current");
	throw e;
}
catch(...)
{
	std::ostringstream Desc;
	Desc<<"Unknown exception caught on magnet with ip: "<<this->ip_address<<std::ends;
	yat::Exception e("Unknown error",Desc.str(),"PSC_ETH::set_current");
	throw e; 
}
}

void PSC_ETH::set_current_buffer(float *valF,int index,int count)
{
//TODO: Implement a sequence in the ps controller
}


void PSC_ETH::send_software_trigger()
{
//TODO:Send TRIGger:IMMediate command to trig a sequence in the ps controller

}

