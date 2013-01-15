// ============================================================================
//
// = CONTEXT
//    ITest Power Supplies - C++ Interface
//
// = FILENAME
//    ClassMagnet.cpp
//
// = AUTHOR
//    Sebastien Laurent - ITEST
//
// = VERSION
//	  2.00
//	
// ============================================================================
// V200
// first library for BE2811
// V140
// get_ctn for measurement : get 2 CTN measures per pole
// V130
// idata modified : return average value of voltage and current
// get_measre_current_aver and get_measure_voltage_aver added
// V120
// fdata modified : RMS value
// V110
// first library for BE549
// ============================================================================
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/time.h>
#include <fcntl.h>
#include <stdlib.h>

#include <ctime>
#include <cstdlib>

#include <netdb.h>

#include <iostream>
#include <sstream>

#include "ClassMagnet.h"
#include "Exception.h"

namespace itest{
//---------------------------------------------------------------------------
//#define __ITEST_DEBUG
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------CLASSE SOCKET----------------------------------
//---------------------------------------------------------------------------
//SOCKET
//Création de la socket, connexion au BILT 
Socket::Socket(const char *StrIP):init_device_done(false)
{
	struct sockaddr_in adresse;
	int resultat,flag;
	struct timeval Tv;
	fd_set myset;

#ifdef __ITEST_DEBUG
std::cout<<"CREATE SOCKET"<<std::endl;
#endif

	Sockfd=0;
	Sockfd=socket(AF_INET, SOCK_STREAM, 0);								//création socket pour client
//test si Sock ok
	if(Sockfd == -1)
	{					//ItestException e(SOCK_ERROR_STR,strerror(errno),"itest::Socket::Socket",1,errno);	
						//throw e;
		return;			//modif V202 07/06/07
	}
//Set non blocking
	flag=fcntl(Sockfd, F_GETFL, 0);
	fcntl(Sockfd, F_SETFL, O_NONBLOCK|flag);

	adresse.sin_family=AF_INET;
	adresse.sin_addr.s_addr=inet_addr(StrIP);
	adresse.sin_port=htons(SOCK_PORT);					

	resultat=connect(Sockfd, (struct sockaddr *)&adresse, sizeof(adresse));	//connexion au serveur

	if(resultat==-1)
	{	
#ifdef __ITEST_DEBUG
std::cout<<"ERR_CONNECT"<<std::endl;
#endif
		if(errno==EINPROGRESS)
		{	
#ifdef __ITEST_DEBUG
std::cout<<"ERR_INPROGRESS"<<std::endl;
#endif
			Tv.tv_sec=MAXTIME_CONNECT_READ;						
			Tv.tv_usec=0;
			FD_ZERO(&myset);
			FD_SET(Sockfd,&myset);
			if(select(FD_SETSIZE ,NULL ,&myset, NULL, &Tv)<1)				//attend MAXTIME_CONNECT_READ
			{	close(Sockfd);
						//ItestException e(SOCK_ERROR_STR,"Could not connect with peripheral","itest::Socket::Socket");
						//throw e;
				return;	//modif V202 07/06/07
			}
#ifdef __ITEST_DEBUG
std::cout<<"END ERR_INPROGRESS"<<std::endl;
#endif
		}	
		else
		{	close(Sockfd);
						//ItestException e(SOCK_ERROR_STR,strerror(errno),"itest::Socket::Socket",1,errno);	
						//throw e;
			return;		//modif V202 07/06/07
		}
	}
	
	this->init_device_done=true;	//modif V202 07/06/07	

						//Set blocking
						//	fcntl(Sockfd, F_SETFL, flag);
#ifdef __ITEST_DEBUG
std::cout<<"END CREATED: "<<Sockfd<<std::endl;
#endif
}
//---------------------------------------------
Socket::~Socket(void)
{
#ifdef __ITEST_DEBUG
std::cout<<"DESTRUCT SOCKET num: "<<Sockfd<<std::endl;
#endif
	close(Sockfd);
	Sockfd=0;

#ifdef __ITEST_DEBUG
std::cout<<"DESTRUCT SOCKET"<<std::endl;
#endif
}
//---------------------------------------------
//SOCKET
//Envoie commande et reçoit 
//---------------------------------- 
void Socket::write(const std::string& Str)
{
	if(!this->init_device_done)			//modif V202 07/06/07
	{
		std::ostringstream Desc;
		Desc<<ITEST_EXCEPT_TRY_STR<<"\""<<Str<<std::ends;
		ItestException e("Object constructor failed",Desc.str().c_str(),"itest::Socket::write",1,errno);
		throw e;	
	}


	if(::write(Sockfd, Str.c_str(), strlen(Str.c_str()))==-1)
	{	std::ostringstream Desc;
		Desc<<ITEST_EXCEPT_TRY_STR<<"\""<<Str<<"\"->"<<strerror(errno)<<std::ends;
		ItestException e(CONNECT_FAILED_STR,Desc.str().c_str(),"itest::Socket::write",1,errno);
		throw e;}
}
//----------------------------------
//SOCKET
//Envoie commande et reçoit 
//---------------------------------- 
char *Socket::query(const std::string& Str)
{
	char *Ptr;	

	if(!this->init_device_done)			//modif V202 07/06/07		
	{
		std::ostringstream Desc;
		Desc<<ITEST_EXCEPT_TRY_STR<<"\""<<Str<<std::ends;
		ItestException e("Object constructor failed",Desc.str().c_str(),"itest::Socket::query",1,errno);
		throw e;	
	}

	R_Buff[0]=0;

	if(::write(Sockfd, Str.c_str(), strlen(Str.c_str()))==-1)
	{	
		std::ostringstream Desc;
		Desc<<ITEST_EXCEPT_TRY_STR<<"\""<<Str<<"\"->"<<strerror(errno)<<std::ends;
		ItestException e(CONNECT_FAILED_STR,Desc.str().c_str(),"itest::Socket::query",1,errno);
		throw e;}
//-----------------------------------------------------------------------------------------------
	int resultat,flag;
	struct timeval Tv;
	fd_set readfds;


	Tv.tv_sec=MAXTIME_CONNECT_READ;
	Tv.tv_usec=0;
	FD_ZERO(&readfds);
	FD_SET(Sockfd,&readfds);

	if((resultat=select(FD_SETSIZE ,&readfds ,NULL, NULL, &Tv))<1)
	{	//close(Sockfd);
#ifdef __ITEST_DEBUG
std::cout<<"ERREUR: "<<strerror(errno)<<"\nRES: "<<resultat<<std::endl;
#endif
		ItestException e(SOCK_ERROR_STR,"Could not read data on peripheral","itest::Socket::query");
		throw e;}

	if(::read(Sockfd, &R_Buff[0], BUFFSIZE)==-1)
	{		
#ifdef __ITEST_DEBUG
std::cout<<"ERREUR: "<<strerror(errno)<<"\nRES: "<<resultat<<std::endl;
#endif
		std::ostringstream Desc;
		Desc<<ITEST_EXCEPT_TRY_STR<<"\""<<Str<<"\"->"<<strerror(errno)<<std::ends;
		ItestException e(CONNECT_FAILED_STR,Desc.str().c_str(),"itest::Socket::query",1,errno);
		throw e;}
//-----------------------------------------------------------------------------------------------
	if((R_Buff[0]==0)||(R_Buff[0]=='\n'))	//rien reçu modif V101
	{	std::ostringstream Desc;
		Desc<<ITEST_EXCEPT_TRY_STR<<"\""<<Str<<"\""<<std::ends;
		ItestException e(CONNECT_FAILED_STR,Desc.str().c_str(),"itest::Socket::query",1,errno);
		throw e;}

	if((Ptr=strchr(&R_Buff[0],'\n'))!=NULL)		//modif V101
		*Ptr=0;

	return &R_Buff[0];
}
//--------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------CLASSE BILTRW----------------------------------
//---------------------------------------------------------------------------
itBilt::itBilt(const char *StrIP):Socket(StrIP)
{
#ifdef __ITEST_DEBUG
std::cout<<"BILT CREATE"<<std::endl;
#endif
}
//----------------------------------
itBilt::~itBilt(void)
{
#ifdef __ITEST_DEBUG
std::cout<<"BILT DESTRUCT"<<std::endl;
#endif
}
//----------------------------------
void itBilt::write(const std::string& _Str) throw (itest::ItestException)
{
	std::ostringstream oss;
	char *Ptr;

	oss<<_Str<<std::endl; 			//modif V101 syst:err? à part

	Socket::write(oss.str());
	oss.str("");
	oss<<Q_SYSTERR<<std::endl;

	try{									//ajout try/catch modif V202 05/06/07
		Ptr=Socket::query(oss.str());
	}
	catch(itest::ItestException &e)
	{	
		e.push_error("Bilt write failed","Could not read data on peripheral","itest::TBilt::write");
		throw e;	
	}

	if((this->NumLastErr=atoi(Ptr))!=0)
	{
		std::ostringstream Desc;

		this->StrLastErr=strchr(Ptr,',');
		strcpy(&BuffLastErr[0],++this->StrLastErr);

		this->StrLastErr=&BuffLastErr[0];

		Desc<<ITEST_EXCEPT_TRY_STR<<"\""<<_Str<<"\"->"<<this->StrLastErr<<std::ends;
		
		ItestException e("Bilt command failed",Desc.str().c_str(),"itest::TBilt::write",1,this->NumLastErr);
		throw e;
	}
}
//----------------------------------
char *itBilt::query(const std::string& _Str) throw (itest::ItestException)
{
	std::ostringstream oss;

	oss<<_Str<<std::endl;

	try{									//ajout try/catch modif V202 05/06/07
		return Socket::query(oss.str());
	}
	catch(itest::ItestException &e)
	{	
		e.push_error("Bilt query failed","Could not read data on peripheral","itest::TBilt::query");
		throw e;	
	}
}

//--------------------------------------------------------------------------
//---------------------------------- 
int parse_datas(const char *str, std::string *Res,char car)
{
	int i=0;
	int start=0,end=0;

    if(*str=='\0')
        return 0;

	std::string data(str);

	while((end=data.find_first_of(car,start)) != std::string::npos)
	{
		*Res=data.substr(start,end-start);
		start=end+1;
		i++;
		Res++;
	}
	*Res=data.substr(start,data.length()-start);

    return i+1;
}
//----------------------------------
void parse_fill_idata(const char *str, struct idata * myidata,int _magnet)
{
	std::string TabRes[5];

	parse_datas(str,&TabRes[0],',');

	myidata->magnet=_magnet;
	int var=atoi(TabRes[0].c_str()+1);
	myidata->pole=(var>5?(var-7):(var-1))/2;
	myidata->state=atoi(TabRes[1].c_str());
	myidata->fail=TabRes[2];
	myidata->measvolt=atof(TabRes[3].c_str());
	myidata->meascurr=atof(TabRes[4].c_str());
}
//---------------------------------------------------------------------------
//--------------------------------UDP TRIG-----------------------------------
//---------------------------------------------------------------------------
void initiate_udp_trigger(const char *_mask)
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
}
//---------------------------------------------------------------------------
//-----------------------------CLASSE POLE-----------------------------------
//---------------------------------------------------------------------------
//Pole constructor
//----------------------------------
itPole2811::itPole2811(itBilt *_Bilt, int _NumMagnet, int _NumPole, int _NumInst):OwnBilt(false)
{
	NumMagnet=_NumMagnet;
	NumPole=_NumPole;
	Bilt=_Bilt;
	Inst=_NumInst;

    StrAddrIp="";
}
//----------------------------------
itPole2811::itPole2811(const char *_AddrIp, int _NumPole,int _NumInst):OwnBilt(true)
{
try{
	NumMagnet=0;
	NumPole=_NumPole;
	Inst=_NumInst;

    StrAddrIp=_AddrIp;
    //--------------------------------
    if(Inst<1 || Inst>13)
        throw ItestException("Error instrument position","exception caught on pole while trying to create one","itest::itPole2811::itPole2811");
    //--------------------------------
    //create
	Bilt=new itBilt(_AddrIp);

#ifdef __ITEST_DEBUG
std::cout<<"CREATE itMagnet2811"<<std::endl;
#endif
}
catch(...)
{
	throw ItestException("Unknown exception","Unknown exception caugth","itest::itPole2811::itPole2811");
}
}
//----------------------------------
itPole2811::~itPole2811(void)
{
    if(OwnBilt)
        delete Bilt;
}
//---------------------------------------------------------------------------
//INSTRUMENT BE2811
//RÃ©cupÃ¨re l'Ã©tat de l'instrument
//0=OFF;1=ON;2=WARNING;3=ALARM
//---------------------------------- 
int itPole2811::get_state(void) throw (itest::ItestException)
{
try{
	std::ostringstream oss;

	oss<<":i"<<this->Inst<<";"<<Q_STAT;

	return (atoi(Bilt->query(oss.str())));
}
catch(ItestException &e)
{
	std::ostringstream Desc;
	Desc<<ITEST_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	e.push_error("State could not be read",Desc.str(), "itest::itPole2811::get_state");
	throw e;
}
catch(...)
{
	std::ostringstream Desc;
	Desc<<UNKNOW_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	ItestException e("Unknown error",Desc.str(),"itest::itPole2811::get_state");
	throw e; 
}
}
//---------------------------------------------------------------------------
//INSTRUMENT BE2811
//Lecture de la mesure en courant 
//Lecture de la mesure en courant du pole
//---------------------------------- 
double itPole2811::get_measure_current(void) throw (itest::ItestException)
{
try{
	std::ostringstream oss;
	char *Ptr;

	oss<<":i"<<this->Inst<<Q_MCURR;

	Ptr=Bilt->query(oss.str());

	return atof(Ptr);
}
catch(ItestException &e)
{
	std::ostringstream Desc;
	Desc<<ITEST_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	e.push_error("Current measure could not be read",Desc.str(), "itest::itPole2811::get_measure_current");
	throw e;
}
catch(...)
{
	std::ostringstream Desc;
	Desc<<UNKNOW_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	ItestException e("Unknown error",Desc.str(),"itest::itPole2811::get_measure_current");
	throw e; 
}
}
//---------------------------------------------------------------------------
//INSTRUMENT BE2811
//Lecture de la mesure en tension
//Lecture de la mesure en tension de l'axe
//----------------------------------
double itPole2811::get_measure_voltage(void) throw (itest::ItestException)
{
try{
	std::ostringstream oss;
	char *Ptr;

	oss<<":i"<<this->Inst<<Q_MVOLT;

	Ptr=Bilt->query(oss.str());

	return atof(Ptr);
}
catch(ItestException &e)
{
	std::ostringstream Desc;
	Desc<<ITEST_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	e.push_error("Voltage measure could not be read",Desc.str(), "itest::itPole2811::get_measure_voltage");
	throw e;
}
catch(...)
{
	std::ostringstream Desc;
	Desc<<UNKNOW_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	ItestException e("Unknown error",Desc.str(),"itest::itPole2811::get_measure_voltage");
	throw e; 
}
}
//---------------------------------------------------------------------------
bool itPole2811::get_trigger_state(void) throw (itest::ItestException)
{
try{
	std::ostringstream oss;
	int syst_st,mod_st;

	oss<<":i"<<this->Inst<<Q_TRIG_STATE;

	return (atoi(Bilt->query(oss.str())) && (atoi(Bilt->query("syst:trig:sour?"))==3));
}
catch(ItestException &e)
{
	std::ostringstream Desc;
	Desc<<ITEST_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	e.push_error("Trigger state could not be read",Desc.str(), "itest::itPole2811::get_trigger_state");
	throw e;
}
catch(...)
{
	std::ostringstream Desc;
	Desc<<UNKNOW_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	ItestException e("Unknown error",Desc.str(),"itest::itPole2811::get_trigger_state");
	throw e; 
}
}
//---------------------------------------------------------------------------
bool itPole2811::get_trigger_edge(void) throw (itest::ItestException)
{
try{
	std::ostringstream oss;
	char *Ptr;

	oss<<":i"<<this->Inst<<Q_TRIG_EDGE;

	Ptr=Bilt->query(oss.str());

	return (atoi(Ptr)?1:0);
}
catch(ItestException &e)
{
	std::ostringstream Desc;
	Desc<<ITEST_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	e.push_error("Trigger state could not be read",Desc.str(), "itest::itPole2811::get_trigger_edge");
	throw e;
}
catch(...)
{
	std::ostringstream Desc;
	Desc<<UNKNOW_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	ItestException e("Unknown error",Desc.str(),"itest::itPole2811::get_trigger_edge");
	throw e; 
}
}
//---------------------------------------------------------------------------
//INSTRUMENT BE2811
//Lecture du delai avant déclenchement
//Lecture du delai avant déclenchement de l'axe
//----------------------------------
double itPole2811::get_trigger_delay(void) throw (itest::ItestException)
{
try{
	std::ostringstream oss;
	char *Ptr;

	oss<<":i"<<this->Inst<<Q_TRIG_DEL;

	Ptr=Bilt->query(oss.str());

	return atof(Ptr);
}
catch(ItestException &e)
{
	std::ostringstream Desc;
	Desc<<ITEST_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	e.push_error("Delay before trigger could not be read",Desc.str(), "itest::itPole2811::get_trigger_delay");
	throw e;
}
catch(...)
{
	std::ostringstream Desc;
	Desc<<UNKNOW_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	ItestException e("Unknown error",Desc.str(),"itest::itPole2811::get_trigger_delay");
	throw e; 
}
}
//---------------------------------------------------------------------------
//INSTRUMENT BE2811
//Lecture de idata : NoModule,State,Fail,MeasU,MeasI
//State : 0=OFF,1=ON,2=WARNING,3=ALARM
//Lecture de l'Ã©tat et des paramÃ¨tres de l'alimentation du pole
void itPole2811::get_idata(struct idata *myidata) throw (itest::ItestException)
{
try{
	std::ostringstream oss;
	char *Ptr;

	oss<<":i"<<this->Inst<<Q_IDATA;

	Ptr=Bilt->query(oss.str());
#ifdef __ITEST_DEBUG
std::cout<<"GET_IADATA: "<<Ptr<<std::endl;
#endif
	
	parse_fill_idata(Ptr, myidata,this->NumMagnet);

}
catch(ItestException &e)
{
	std::ostringstream Desc;
	Desc<<ITEST_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	e.push_error("Idata could not be read",Desc.str(), "itest::itPole2811::get_idata");
	throw e;
}
catch(...)
{
	std::ostringstream Desc;
	Desc<<UNKNOW_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	ItestException e("Unknown error",Desc.str(),"itest::itPole2811::get_idata");
	throw e; 
}
}
//---------------------------------------------------------------------------
//INSTRUMENT BE2811
//Consigne en courant sur instrument
//Lecture de la consigne en courant sur le pole de l'aimant
//valeur flottante
//---------------------------------- 
double itPole2811::get_current(void) throw (itest::ItestException)
{
try{
	std::ostringstream oss;
	char *Ptr;

	oss<<":i"<<this->Inst<<";"<<Q_CURR;

	Ptr=Bilt->query(oss.str());

	return atof(Ptr);
}
catch(ItestException &e)
{
	std::ostringstream Desc;
	Desc<<ITEST_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	e.push_error("Current value could not be read",Desc.str(), "itest::itPole2811::get_current");
	throw e;
}
catch(...)
{
	std::ostringstream Desc;
	Desc<<UNKNOW_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	ItestException e("Unknown error",Desc.str(),"itest::itPole2811::get_current");
	throw e; 
}
}
//---------------------------------------------------------------------------
//INSTRUMENT BE2811
//Consigne en courant latché sur instrument
//Lecture de la consigne en courant latché sur le pole de l'aimant
//valeur flottante
//---------------------------------- 
double itPole2811::get_current_latch(void) throw (itest::ItestException)
{
try{
	std::ostringstream oss;
	char *Ptr;

	oss<<":i"<<this->Inst<<";"<<Q_CURR_LATCH;

	Ptr=Bilt->query(oss.str());

	return atof(Ptr);
}
catch(ItestException &e)
{
	std::ostringstream Desc;
	Desc<<ITEST_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	e.push_error("Latched current value could not be read",Desc.str(), "itest::itPole2811::get_current_latch");
	throw e;
}
catch(...)
{
	std::ostringstream Desc;
	Desc<<UNKNOW_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	ItestException e("Unknown error",Desc.str(),"itest::itPole2811::get_current_latch");
	throw e; 
}
}
//---------------------------------------------------------------------------
//INSTRUMENT BE2811
//Consigne du niveau "track" en courant sur instrument
//Lecture du niveau du track en courant sur le pole de l'aimant
//valeur flottante
//---------------------------------- 
double itPole2811::get_current_tracking_level(void) throw (itest::ItestException)
{
try{
	std::ostringstream oss;
	char *Ptr;

	oss<<":i"<<this->Inst<<";"<<Q_CURR_TRACKING_LEVEL;

	Ptr=Bilt->query(oss.str());

	return atof(Ptr);
}
catch(ItestException &e)
{
	std::ostringstream Desc;
	Desc<<ITEST_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	e.push_error("Tracking level current value could not be read",Desc.str(), "itest::itPole2811::get_current_tracking_level");
	throw e;
}
catch(...)
{
	std::ostringstream Desc;
	Desc<<UNKNOW_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	ItestException e("Unknown error",Desc.str(),"itest::itPole2811::get_current_tracking_level");
	throw e;
}
}
//---------------------------------------------------------------------------
bool itPole2811::get_current_tracking_status(void) throw (itest::ItestException)
{
try{
	std::ostringstream oss;
	char *Ptr;

	oss<<":i"<<this->Inst<<";"<<Q_TRACK;

	Ptr=Bilt->query(oss.str());

	return atoi(Ptr);
}
catch(ItestException &e)
{
	std::ostringstream Desc;
	Desc<<ITEST_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	e.push_error("Tracking status could not be read",Desc.str(), "itest::itPole2811::get_current_tracking_status");
	throw e;
}
catch(...)
{
	std::ostringstream Desc;
	Desc<<UNKNOW_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	ItestException e("Unknown error",Desc.str(),"itest::itPole2811::get_current_tracking_status");
	throw e;
}
}
//---------------------------------------------------------------------------
bool itPole2811::get_current_slope_completion(void) throw (itest::ItestException)
{
try{
	std::ostringstream oss;
	char *Ptr;

	oss<<":i"<<this->Inst<<";"<<Q_COMPLETION;

	Ptr=Bilt->query(oss.str());

	return atoi(Ptr);
}
catch(ItestException &e)
{
	std::ostringstream Desc;
	Desc<<ITEST_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	e.push_error("Tracking status could not be read",Desc.str(), "itest::itPole2811::get_current_slope_completion");
	throw e;
}
catch(...)
{
	std::ostringstream Desc;
	Desc<<UNKNOW_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	ItestException e("Unknown error",Desc.str(),"itest::itPole2811::get_current_slope_completion");
	throw e;
}
}
//---------------------------------------------------------------------------
bool itPole2811::get_current_latch_source(void) throw (itest::ItestException)
{
try{
	std::ostringstream oss;
	char *Ptr;

	oss<<":i"<<this->Inst<<";"<<Q_LATCH_SOURCE;

	Ptr=Bilt->query(oss.str());

	return atoi(Ptr);
}
catch(ItestException &e)
{
	std::ostringstream Desc;
	Desc<<ITEST_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	e.push_error("Latch source could not be read",Desc.str(), "itest::itPole2811::get_current_latch_source");
	throw e;
}
catch(...)
{
	std::ostringstream Desc;
	Desc<<UNKNOW_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	ItestException e("Unknown error",Desc.str(),"itest::itPole2811::get_current_latch_source");
	throw e;
}
}
//---------------------------------------------------------------------------
void itPole2811::set_current_buffer(float *valF,int index,int count)
{
try{
	for(int i=0;i<count;i++)
	{
		std::ostringstream oss;
		oss<<":i"<<this->Inst<<";"<<BUFF_VAL<<index++<<","<<*valF++;
		Bilt->write(oss.str());
	}
}
catch(ItestException &e)
{
	std::ostringstream Desc;
	Desc<<ITEST_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	e.push_error("Buffer value could not be written",Desc.str(), "itest::itPole2811::set_current_buffer");
	throw e;
}
catch(...)
{
	std::ostringstream Desc;
	Desc<<UNKNOW_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	ItestException e("Unknown error",Desc.str(),"itest::itPole2811::set_current_buffer");
	throw e;
}
}
//---------------------------------------------------------------------------
void itPole2811::get_current_buffer(float *valF,int index,int count)
{
try{
	for(int i=0;i<count;i++)
	{
		std::ostringstream oss;
		oss<<":i"<<this->Inst<<";"<<BUFF_VAL<<index++<<"?";
		*valF=atof(Bilt->query(oss.str()));
		valF++;
	}
}
catch(ItestException &e)
{
	std::ostringstream Desc;
	Desc<<ITEST_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	e.push_error("Buffer value could not be written",Desc.str(), "itest::itPole2811::get_current_buffer");
	throw e;
}
catch(...)
{
	std::ostringstream Desc;
	Desc<<UNKNOW_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	ItestException e("Unknown error",Desc.str(),"itest::itPole2811::get_current_buffer");
	throw e;
}
}
//---------------------------------------------------------------------------
void itPole2811::set_current_buffer_loop(bool state)
{
try{
	std::ostringstream oss;

	oss<<":i"<<this->Inst<<";"<<BUFF_LOOP<<state;

	Bilt->write(oss.str());
}
catch(ItestException &e)
{
	std::ostringstream Desc;
	Desc<<ITEST_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	e.push_error("Buffer loop state could not be changed",Desc.str(), "itest::itPole2811::set_current_buffer_loop");
	throw e;
}
catch(...)
{
	std::ostringstream Desc;
	Desc<<UNKNOW_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	ItestException e("Unknown error",Desc.str(),"itest::itPole2811::set_current_buffer_loop");
	throw e;
}
}
//---------------------------------------------------------------------------
bool itPole2811::get_current_buffer_loop(void)
{
try{
	std::ostringstream oss;
	char *Ptr;

	oss<<":i"<<this->Inst<<";"<<Q_BUFF_LOOP;

	Ptr=Bilt->query(oss.str());

	return atoi(Ptr);
}
catch(ItestException &e)
{
	std::ostringstream Desc;
	Desc<<ITEST_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	e.push_error("Buffer looop state could not be read",Desc.str(), "itest::itPole2811::get_current_buffer_loop");
	throw e;
}
catch(...)
{
	std::ostringstream Desc;
	Desc<<UNKNOW_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	ItestException e("Unknown error",Desc.str(),"itest::itPole2811::get_current_buffer_loop");
	throw e;
}
}
//---------------------------------------------------------------------------
void itPole2811::set_current_buffer_sample(float valF)
{
try{
	std::ostringstream oss;

	oss<<":i"<<this->Inst<<BUFF_SAMP<<valF;

	Bilt->write(oss.str());
}
catch(ItestException &e)
{
	std::ostringstream Desc;
	Desc<<ITEST_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	e.push_error("Current could not be changed",Desc.str(), "itest::itPole2811::set_current_buffer_sample");
	throw e;
}
catch(...)
{
	std::ostringstream Desc;
	Desc<<UNKNOW_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	ItestException e("Unknown error",Desc.str(),"itest::itPole2811::set_current_buffer_sample");
	throw e; 
}
}
//---------------------------------------------------------------------------
float itPole2811::get_current_buffer_sample(void)
{
try{
	std::ostringstream oss;
	char *Ptr;

	oss<<":i"<<this->Inst<<";"<<Q_BUFF_SAMP;

	Ptr=Bilt->query(oss.str());

	return atof(Ptr);
}
catch(ItestException &e)
{
	std::ostringstream Desc;
	Desc<<ITEST_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	e.push_error("Tracking level current value could not be read",Desc.str(), "itest::itPole2811::get_current_buffer_sample");
	throw e;
}
catch(...)
{
	std::ostringstream Desc;
	Desc<<UNKNOW_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	ItestException e("Unknown error",Desc.str(),"itest::itPole2811::get_current_buffer_sample");
	throw e;
}
}
//---------------------------------------------------------------------------
void itPole2811::set_current_buffer_range(int index,int count)
{
try{
	std::ostringstream oss;

	oss<<":i"<<this->Inst<<BUFF_RANG<<index<<","<<count;

	Bilt->write(oss.str());
}
catch(ItestException &e)
{
	std::ostringstream Desc;
	Desc<<ITEST_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	e.push_error("Current could not be changed",Desc.str(), "itest::itPole2811::set_current_buffer_range");
	throw e;
}
catch(...)
{
	std::ostringstream Desc;
	Desc<<UNKNOW_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	ItestException e("Unknown error",Desc.str(),"itest::itPole2811::set_current_buffer_range");
	throw e; 
}
}
//---------------------------------------------------------------------------
void itPole2811::get_current_buffer_range(int *index,int *count)
{
try{
	std::ostringstream oss;

	oss<<":i"<<this->Inst<<Q_BUFF_RANG;

	*index = atoi(Bilt->query(oss.str()));
	*count = atoi(strchr(Bilt->query(oss.str()),',')+1);
}
catch(ItestException &e)
{
	std::ostringstream Desc;
	Desc<<ITEST_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	e.push_error("Current could not be changed",Desc.str(), "itest::itPole2811::get_current_buffer_range");
	throw e;
}
catch(...)
{
	std::ostringstream Desc;
	Desc<<UNKNOW_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	ItestException e("Unknown error",Desc.str(),"itest::itPole2811::get_current_buffer_range");
	throw e; 
}
}
//---------------------------------------------------------------------------
//INSTRUMENT BE2811
//Alarme sur instrument
//Lecture de l'alarme courante sur le pole de l'aimant
//valeur int
//---------------------------------- 
int itPole2811::get_num_alarm(void) throw (itest::ItestException)
{
try{
	std::ostringstream oss;
	char *Ptr;

	oss<<":i"<<this->Inst<<Q_FAIL; 

	Ptr=Bilt->query(oss.str());

	return atoi(Ptr);
}
catch(ItestException &e)
{
	std::ostringstream Desc;
	Desc<<ITEST_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	e.push_error("Alarm number could not be read",Desc.str(), "itest::itPole2811::get_num_alarm");
	throw e;
}
catch(...)
{
	std::ostringstream Desc;
	Desc<<UNKNOW_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	ItestException e("Unknown error",Desc.str(),"itest::itPole2811::get_num_alarm");
	throw e; 
}
}
//---------------------------------------------------------------------------
void itPole2811::clear_all_err(void) throw (itest::ItestException)
{
try{
	std::ostringstream oss;
	int NumBilt_Err;

	oss<<Q_SYSTERR;  	//modif v2.00 oss<<Q_SYSTERR<<std::endl;

	do
	{
		NumBilt_Err=atoi(Bilt->query(oss.str()));	
#ifdef __ITEST_DEBUG
std::cout<<"BOUCLE ERR"<<std::endl;
#endif
	}
	while(NumBilt_Err!=0);
}
catch(ItestException &e)
{
	std::ostringstream Desc;
	Desc<<ITEST_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	e.push_error("State could not be read",Desc.str(), "itest::itPole2811::clear_all_err");
	throw e;
}
catch(...)
{
	std::ostringstream Desc;
	Desc<<UNKNOW_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	ItestException e("Unknown error",Desc.str(),"itest::itPole2811::clear_all_err");
	throw e;
}
}
//---------------------------------------------------------------------------
//INSTRUMENT BE2811
//Alarme sur instrument
//Efface alarme pole de l'aimant
//----------------------------------
void itPole2811::clear_alarm(void) throw (itest::ItestException)
{
try{
	std::ostringstream oss;
	char *Ptr;

	oss<<":i"<<this->Inst<<";"<<STATCLEAR; 

	Bilt->write(oss.str());
}
catch(ItestException &e)
{
	std::ostringstream Desc;
	Desc<<ITEST_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	e.push_error("Alarm could not be cleared",Desc.str(), "itest::itPole2811::clear_alarm");
	throw e;
}
catch(...)
{
	std::ostringstream Desc;
	Desc<<UNKNOW_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	ItestException e("Unknown error",Desc.str(),"itest::itPole2811::clear_alarm");
	throw e; 
}
}
//---------------------------------------------------------------------------
void itPole2811::set_state(bool val) throw (itest::ItestException)
{
try{
	std::ostringstream oss;

	oss<<":i"<<this->Inst<<";"<<STAT<<val;

	Bilt->write(oss.str());
}
catch(ItestException &e)
{
	std::ostringstream Desc;
	Desc<<ITEST_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	e.push_error("State could not be changed",Desc.str(), "itest::itPole2811::set_state");
	throw e;
}
catch(...)
{
	std::ostringstream Desc;
	Desc<<UNKNOW_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	ItestException e("Unknown error",Desc.str(),"itest::itPole2811::set_state");
	throw e;
}
}
//---------------------------------------------------------------------------
//INSTRUMENT BE2811
//Consigne en courant sur instrument
//programme une consigne en courant sur le pole de l'aimant
//valeur flottante
//---------------------------------- 
void itPole2811::set_current(double ValF) throw (itest::ItestException)
{
try{
	std::ostringstream oss;

	oss<<":i"<<this->Inst<<CURR<<ValF;

	Bilt->write(oss.str());
}
catch(ItestException &e)
{
	std::ostringstream Desc;
	Desc<<ITEST_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	e.push_error("Current could not be changed",Desc.str(), "itest::itPole2811::set_current");
	throw e;
}
catch(...)
{
	std::ostringstream Desc;
	Desc<<UNKNOW_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	ItestException e("Unknown error",Desc.str(),"itest::itPole2811::set_current");
	throw e; 
}
}
//---------------------------------------------------------------------------
void itPole2811::set_current_latch(double ValF) throw (itest::ItestException)
{
try{
	std::ostringstream oss;

	oss<<":i"<<this->Inst<<CURR_LATCH<<ValF;

	Bilt->write(oss.str());
}
catch(ItestException &e)
{
	std::ostringstream Desc;
	Desc<<ITEST_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	e.push_error("Current latched value could  not be changed",Desc.str(), "itest::itPole2811::set_current_latch");
	throw e;
}
catch(...)
{
	std::ostringstream Desc;
	Desc<<UNKNOW_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	ItestException e("Unknown error",Desc.str(),"itest::itPole2811::set_current_latch");
	throw e;
}
}
//---------------------------------------------------------------------------
void itPole2811::set_current_tracking_level(double ValF) throw (itest::ItestException)
{
try{
	std::ostringstream oss;

	oss<<":i"<<this->Inst<<CURR_TRACKING_LEVEL<<ValF;

	Bilt->write(oss.str());
}
catch(ItestException &e)
{
	std::ostringstream Desc;
	Desc<<ITEST_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	e.push_error("Current tracking level could  not be changed",Desc.str(), "itest::itPole2811::set_current_tracking_level");
	throw e;
}
catch(...)
{
	std::ostringstream Desc;
	Desc<<UNKNOW_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	ItestException e("Unknown error",Desc.str(),"itest::itPole2811::set_current_tracking_level");
	throw e;
}
}
//---------------------------------------------------------------------------
void itPole2811::set_trigger_state(bool Val) throw (itest::ItestException)
{
try{
	std::ostringstream oss;

	oss<<":i"<<this->Inst<<TRIG_STATE<<Val<<";:trig:edge1;:syst:trig:sour 3";

	Bilt->write(oss.str());
}
catch(ItestException &e)
{
	std::ostringstream Desc;
	Desc<<ITEST_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	e.push_error("Trigger state could not be changed",Desc.str(), "itest::itPole2811::set_trigger_state");
	throw e;
}
catch(...)
{
	std::ostringstream Desc;
	Desc<<UNKNOW_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	ItestException e("Unknown error",Desc.str(),"itest::itPole2811::set_trigger_state");
	throw e;
}
}
//---------------------------------------------------------------------------
void itPole2811::set_trigger_delay(double ValF) throw (itest::ItestException)
{
try{
	std::ostringstream oss;

	oss<<":i"<<this->Inst<<TRIG_DEL<<ValF;

	Bilt->write(oss.str());
}
catch(ItestException &e)
{
	std::ostringstream Desc;
	Desc<<ITEST_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	e.push_error("Trigger delay could not be changed",Desc.str(), "itest::itPole2811::set_trigger_delay");
	throw e;
}
catch(...)
{
	std::ostringstream Desc;
	Desc<<UNKNOW_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	ItestException e("Unknown error",Desc.str(),"itest::itPole2811::set_trigger_delay");
	throw e;
}
}
//---------------------------------------------------------------------------
void itPole2811::set_current_latch_source(bool Val) throw (itest::ItestException)
{
try{
	std::ostringstream oss;

	oss<<":i"<<this->Inst<<CURR_LATCH_SOURCE<<Val;

	Bilt->write(oss.str());
}
catch(ItestException &e)
{
	std::ostringstream Desc;
	Desc<<ITEST_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	e.push_error("Latch source could not be changed",Desc.str(), "itest::itPole2811::set_current_latch_source");
	throw e;
}
catch(...)
{
	std::ostringstream Desc;
	Desc<<UNKNOW_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
	ItestException e("Unknown error",Desc.str(),"itest::itPole2811::set_current_latch_source");
	throw e;
}
}
//---------------------------------------------------------------------------
//INSTRUMENT BE2811
//mesure les 2 CTN du module
//---------------------------------- 
void itPole2811::get_ctn(int *ctn1,int *ctn2) throw (itest::ItestException)
{
try{
    std::ostringstream oss;
    char *Ptr;

    oss<<":i"<<this->Inst<<";:test:temp0?";

    Ptr=Bilt->query(oss.str());

    *ctn1=(int)(atof(Ptr)+0.5);


    oss.str("");

    oss<<":i"<<this->Inst<<";:test:temp1?";

    Ptr=Bilt->query(oss.str());

    *ctn2=(int)(atof(Ptr)+0.5);
}
catch(ItestException &e)
{
    std::ostringstream Desc;
	Desc<<ITEST_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
    e.push_error("CTN could not be read",Desc.str(), "itest::itPole2811::get_ctn");
    throw e;
}
catch(...)
{
    std::ostringstream Desc;
	Desc<<UNKNOW_EXCEPT_MAGN_STR<<this->NumMagnet<<"/Pole "<<this->NumPole<<"(I"<<this->Inst<<")"<<std::ends;
    ItestException e("Unknown error",Desc.str(),"itest::itPole2811::get_ctn");
    throw e;
}
}
//---------------------------------------------------------------------------
}	//end namespace
//---------------------------------------------------------------------------

