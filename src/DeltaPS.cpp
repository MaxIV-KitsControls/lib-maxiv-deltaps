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
//--------------------------------UDP TRIG-----------------------------------
//---------------------------------------------------------------------------
/*void initiate_udp_trigger(const char *_mask)
{
#ifdef __ITEST_DEBUG
std::cout<<"UDP TRIGGER"<<std::endl;
#endif
try{
	static unsigned char counter=0;

	int sock,status,sinlen;
	struct sockaddr_in sock_in;
	int yes=1;
	
	sinlen=sizeof(struct sockaddr_in);
	memset(&sock_in, 0, sinlen);

	sock=socket(PF_INET,SOCK_DGRAM,IPPROTO_UDP);
	if(sock == -1)
	{					
		std::ostringstream Desc;	
		Desc<<ITEST_EXCEPT_TRY_STR<<"\""<<"Socket could not be created"<<"\"->"<<strerror(errno)<<std::ends;
		ItestException e("UDP trigger failed",Desc.str().c_str(),"itest::initiate_udp_trigger",1,errno);
		throw e;			
	}
	sock_in.sin_family = PF_INET;
	sock_in.sin_port = htons(5044);
	sock_in.sin_addr.s_addr = htonl(INADDR_ANY);

	status = bind(sock, (struct sockaddr *)&sock_in,sinlen);
	if(status == -1)
	{					
		std::ostringstream Desc;	
		Desc<<ITEST_EXCEPT_TRY_STR<<"\""<<"bind error"<<"\"->"<<strerror(errno)<<std::ends;
		ItestException e("UDP trigger failed",Desc.str().c_str(),"itest::initiate_udp_trigger",1,errno);
		throw e;			
	}
	
	status = setsockopt(sock,SOL_SOCKET,SO_BROADCAST,&yes,sizeof(int));
	if(status == -1)
	{					
		std::ostringstream Desc;	
		Desc<<ITEST_EXCEPT_TRY_STR<<"\""<<"bind error"<<"\"->"<<strerror(errno)<<std::ends;
		ItestException e("UDP trigger failed",Desc.str().c_str(),"itest::initiate_udp_trigger",1,errno);
		throw e;			
	}
	
	sock_in.sin_family = PF_INET;
	sock_in.sin_port = htons(5044);
	sock_in.sin_addr.s_addr = inet_addr(_mask);//htonl(-1);

	srand(time(0));

	unsigned char tab[]={'L','X','I',0,'L','A','N','0',0,0,0,0,0,0,0,0,0,0,0,0,0,0,(unsigned char)(rand()%255),(unsigned char)(rand()%255)+counter++,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0};
#ifdef __ITEST_DEBUG
for(int i=0;i<40;i++)
printf("%hu",tab[i]);
std::cout<<std::endl;
#endif

	status=sendto(sock,tab,40,0,(struct sockaddr *)&sock_in,sinlen);
	if(status == -1)
	{					
		std::ostringstream Desc;	
		Desc<<ITEST_EXCEPT_TRY_STR<<"\""<<"bind error"<<"\"->"<<strerror(errno)<<std::ends;
		ItestException e("UDP trigger failed",Desc.str().c_str(),"itest::initiate_udp_trigger",1,errno);
		throw e;			
	}
	shutdown(sock,2);
	close(sock);
}
catch(ItestException &e)
{
	throw e;
}
catch(...)
{
	ItestException e("Unknown error","","itest::initiate_udp_trigger");
	throw e; 
}
}*/
