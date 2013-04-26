#include "DeltaPS.h"
#include <string>
#include <cmath>

#define LF "\10"

PSC_ETH::PSC_ETH(const std::string ip_address, int ps_group) {
	this->ip_address = ip_address;
        this->ps_group = ps_group;
    yat::Socket::init();

    //- set some socket option
    sock.set_option(yat::Socket::SOCK_OPT_KEEP_ALIVE, 1);
    sock.set_option(yat::Socket::SOCK_OPT_NO_DELAY, 1);
    sock.set_option(yat::Socket::SOCK_OPT_OTIMEOUT, 1000);
    sock.set_option(yat::Socket::SOCK_OPT_ITIMEOUT, 1000);

    yat::Address addr(ip_address, SOCK_PORT);
try{
    sock.connect(addr);
}
catch(yat::Exception &e)
{
	std::ostringstream Desc;
	Desc<<"YAT exception caught on magnet with ip: "<<this->ip_address<<std::ends;
	e.push_error("Failed to connect",Desc.str(), "PSC_ETH::Constructor");
	throw e;
}
catch(...)
{
	std::ostringstream Desc;
	Desc<<"Unknown exception caught on magnet with ip: "<<this->ip_address<<std::ends;
	yat::Exception e("Unknown error",Desc.str(),"PSC_ETH::Constructor");
	throw e; 
        
}           
}

PSC_ETH::~PSC_ETH() {
	sock.disconnect();
    yat::Socket::terminate();
}

std::string PSC_ETH::send_query(std::string query) {
	std::string result;
	std::ostringstream oss;
try{
	oss << query << "\n";
	sock << oss.str();
	sock >> result;

	return result;
}
catch(yat::Exception &e)
{
	std::ostringstream Desc;
	Desc<<"YAT exception caught on magnet with ip: "<<this->ip_address<<std::ends;
	e.push_error("Query could not be sent",Desc.str(), "PSC_ETH::send_query");
	throw e;
}
catch(...)
{
	std::ostringstream Desc;
	Desc<<"Unknown exception caught on magnet with ip: "<<this->ip_address<<std::ends;
	yat::Exception e("Unknown error",Desc.str(),"PSC_ETH::send_query");
	throw e; 
}
}

std::string PSC_ETH::idn() {
	return id;
}

int PSC_ETH::get_ps_group()
{
    return ps_group;
}

std::string PSC_ETH::addrIP()
{
	return ip_address;
}

double PSC_ETH::get_max_voltage() {
	std::string reply;
	double d;

	sock << "source:voltage:maximum?\n";
	sock >> reply;

	std::istringstream i(reply);
	i >> d;
	return d;
}

void PSC_ETH::set_max_voltage(double v) {
try{	
	std::ostringstream oss;

	oss<< "source:voltage:maximum " << v << "\n";
	sock << oss.str();
}
catch(yat::Exception &e)
{
	std::ostringstream Desc;
	Desc<<"YAT exception caught on magnet with ip: "<<this->ip_address<<std::ends;
	e.push_error("Max voltage could not be set",Desc.str(), "PSC_ETH::set_max_voltage");
	throw e;
}
catch(...)
{
	std::ostringstream Desc;
	Desc<<"Unknown exception caught on magnet with ip: "<<this->ip_address<<std::ends;
	yat::Exception e("Unknown error",Desc.str(),"PSC_ETH::set_max_voltage");
	throw e; 
}
}
double PSC_ETH::get_max_current() {
	std::string reply;
	double d;

	sock << "source:current:maximum?\n";
	sock >> reply;

	std::istringstream i(reply);
	i >> d;
	return d;
}

void PSC_ETH::set_max_current(double v) {
try{	
	std::ostringstream oss;

	oss<< "source:current:maximum " << v << "\n";
	sock << oss.str();
}
catch(yat::Exception &e)
{
	std::ostringstream Desc;
	Desc<<"YAT exception caught on magnet with ip: "<<this->ip_address<<std::ends;
	e.push_error("Max voltage could not be set",Desc.str(), "PSC_ETH::set_max_current");
	throw e;
}
catch(...)
{
	std::ostringstream Desc;
	Desc<<"Unknown exception caught on magnet with ip: "<<this->ip_address<<std::ends;
	yat::Exception e("Unknown error",Desc.str(),"PSC_ETH::set_max_current");
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
int PSC_ETH::get_output_state(void) throw (yat::Exception)
{
try{
	std::string reply;
	int d= 0;
	if(ps_group == PSGROUP_1)
        {
            return -1; //not supported
        }
        else if(ps_group == PSGROUP_2)
        {
            sock << "status:operation:shutdown:condition\n";
            sock >> reply;

            std::istringstream i(reply);
            i >> d;
            if (d == 8)
                d = 1;
        }
        else if(ps_group == PSGROUP_3)
        {
            sock << "OUTPUT?\n";
            sock >> reply;
            std::istringstream i(reply);
            i >> d;
        }
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
void PSC_ETH::set_output_state(bool val) throw (yat::Exception)
{
try{
        if(ps_group > 1)
{
            std::ostringstream oss;
            oss << "output " << val << "\n";
            sock << oss.str();
        }
        //group 1 lacks on/off output setting. In case of state= 0 (OFF) set
        //current to zero.
        else if((ps_group == 1) && (val == MAGNET_OFF))
        {
            std::ostringstream oss;
            oss << "source:current " << 0.0 << "\n";
            sock << oss.str();
        }
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
//Empty the error queue (has 5 slots)
//----------------------------------
void PSC_ETH::clear_all_err(void) throw (yat::Exception)
{
try{
	
	for(int i=0;i<5;i++)
	{
		this->read_error();
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
//---------------------------------------------------------------------------
//Set the voltage
//---------------------------------- 
void PSC_ETH::set_voltage(double ValF) throw (yat::Exception)
{
try{
	std::ostringstream oss;

	oss<<"source:voltage "<<ValF<<"\n";

	sock << oss.str();
}
catch(yat::Exception &e)
{
	std::ostringstream Desc;
	Desc<<"YAT exception caught on magnet with ip: "<<this->ip_address<<std::ends;
	e.push_error("Voltage could not be set",Desc.str(), "PSC_ETH::set_voltage");
	throw e;
}
catch(...)
{
	std::ostringstream Desc;
	Desc<<"Unknown exception caught on magnet with ip: "<<this->ip_address<<std::ends;
	yat::Exception e("Unknown error",Desc.str(),"PSC_ETH::set_voltage");
	throw e; 
}
}

//---------------------------------------------------------------------------
//Read the set current
//---------------------------------- 
double PSC_ETH::get_source_current(void) throw (yat::Exception)
{
 try{
    double result;
    std::string reply;
    
	sock<<"source:current?\n";
        sock >> reply;

	std::istringstream i(reply);
	i >> result;
        return result;
}
catch(yat::Exception &e)
{
	std::ostringstream Desc;
	Desc<<"YAT exception caught on magnet with ip: "<<this->ip_address<<std::ends;
	e.push_error("Source current could not be read",Desc.str(), "PSC_ETH::get_source_current");
	throw e;
}
catch(...)
{
	std::ostringstream Desc;
	Desc<<"Unknown exception caught on magnet with ip: "<<this->ip_address<<std::ends;
	yat::Exception e("Unknown error",Desc.str(),"PSC_ETH::get_source_current");
	throw e; 
}  
}
//---------------------------------------------------------------------------
//Read the set voltage
//---------------------------------- 
double PSC_ETH::get_source_voltage(void) throw (yat::Exception)
{
try{
    double result;
    std::string reply;
    
	sock<<"source:voltage?\n";
        sock >> reply;

	std::istringstream i(reply);
	i >> result;
        return result;
}
catch(yat::Exception &e)
{
	std::ostringstream Desc;
	Desc<<"YAT exception caught on magnet with ip: "<<this->ip_address<<std::ends;
	e.push_error("Source voltage could not be read",Desc.str(), "PSC_ETH::get_source_voltage");
	throw e;
}
catch(...)
{
	std::ostringstream Desc;
	Desc<<"Unknown exception caught on magnet with ip: "<<this->ip_address<<std::ends;
	yat::Exception e("Unknown error",Desc.str(),"PSC_ETH::get_source_voltage");
	throw e; 
}
}

//---------------------------------------------------------------------------
//Poll to see if requested current value is reached and stabilized
//---------------------------------- 
bool PSC_ETH::is_current_moving()
{

	double meas_i, set_i;
	bool state = false;

	//compare measured current and set current
        set_i = get_source_current();
        //sleep(0.5);
        meas_i = get_measure_current();
        
        state = std::abs(set_i - meas_i) > CURRENT_TOLERANCE;
	
	return state;
}


void PSC_ETH::send_software_trigger()
{
//TODO:Send TRIGger:IMMediate command to trig a sequence in the ps controller

}

