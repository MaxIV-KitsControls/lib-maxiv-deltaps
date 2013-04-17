#include <sstream>
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <math.h>

#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "DeltaPS.h"

//g++ -o TestClass TestClass.cpp -L./ -lMagnet
//--------
//g++ -D_REENTRANT -o TestClass TestClass.cpp -L./ -lMagnet -lpthread
//--------

using namespace std;
PSC_ETH *Magnet=0;
struct idata myDataX_Magnet1;


/* Return 1 if the difference is negative, otherwise 0.  */
long int timeval_subtract(struct timeval *result, struct timeval *t2, struct timeval *t1)
{
    long int diff = (t2->tv_usec + 1000000 * t2->tv_sec) - (t1->tv_usec + 1000000 * t1->tv_sec);
//    result->tv_sec = diff / 1000000;
//    result->tv_usec = diff % 1000000;

    return diff;
}

void test0()
{

	try{
		std::cout<<"---------------TEST0---------------"<<std::endl;

		/** Test the software trigger and the ramping of the current**/

		Magnet = new PSC_ETH("192.168.150.101");
		
		std::cout<<Magnet->addrIP()<<std::endl;
		float tab[]={1.,2.,2.001,1.,-0.5,-3};
		Magnet->set_current_buffer(tab,0,6);

		Magnet->set_current_buffer_sample(3);		
		Magnet->set_current_buffer_loop(1);		
		Magnet->set_current_buffer_range(0,6);	

		Magnet->set_trigger_state(TRIGGER_STATE_ON);
		Magnet->set_current_latch_source(CURR_LATCH_SOURCE_BUFFER);
		Magnet->set_state(MAGNET_ON);
		sleep(1);
		Magnet->send_software_trigger();
		
		while(getchar() != 'q')
			std::cout<<"\r"<<"MEASCURR1 :"<<Magnet->get_measure_current();
		std::cout<<std::endl;
		Magnet->set_current_buffer_loop(1);
		Magnet->set_current_latch_source(CURR_LATCH_SOURCE_VALUE);
		Magnet->set_state(MAGNET_OFF);

		delete Magnet;
	}
	catch(yat::Exception &e)
	{
		for(int i=0;i<e.errors.size();i++)		//gestion des exceptions empilées dans le vecteur d'erreur
       		{
			std::cout<<e.errors[i].reason<<" , "<<e.errors[i].desc<<" , "<<e.errors[i].origin<<" , "<<
					e.errors[i].code<<std::endl;
		}
		delete Magnet;
	}
}
void test1()
{

	try{
		std::cout<<"---------------TEST1---------------"<<std::endl;

		/** 	Set current
		//	Set state to ON
		//	Print state
		//	Set state to OFF
		//	Print the current, voltage and state, (fail etc) 	
		**/

		Magnet = new PSC_ETH("192.168.150.101");
		std::cout<<Magnet->addrIP()<<std::endl;
		Magnet->set_current(1);
		std::cout<<"CURRENT SETTING:"<<1<<std::endl;
		Magnet->clear_all_err();
		Magnet->set_state(1);

		sleep(2);

		std::cout<<"STATE :"<<Magnet->get_state()<<std::endl;

		Magnet->get_idata(&myDataX_Magnet1);

		Magnet->set_state(0);

		delete Magnet;

		std::cout<<"---------------IDATA---------------"<<std::endl;
		std::cout<<"MAGNET :"<<myDataX_Magnet1.magnet<<std::endl;
		std::cout<<"POLE :"<<myDataX_Magnet1.pole<<std::endl;
		std::cout<<"STATE :"<<myDataX_Magnet1.state<<std::endl;
		std::cout<<"FAIL :"<<myDataX_Magnet1.fail<<std::endl;
		std::cout<<"MEASVOLT :"<<myDataX_Magnet1.measvolt<<std::endl;
		std::cout<<"MEASCURR :"<<myDataX_Magnet1.meascurr<<std::endl;
		std::cout<<"-----------------------------------"<<std::endl;

	}
	catch(yat::Exception &e)
	{
		for(int i=0;i<e.errors.size();i++)		//gestion des exceptions empilées dans le vecteur d'erreur
       		{
			std::cout<<e.errors[i].reason<<" , "<<e.errors[i].desc<<" , "<<e.errors[i].origin<<" , "<<
					e.errors[i].code<<std::endl;
		}
		delete Magnet;
	}	
}

bool test2()
{
	bool result=false;
	try{
		std::cout<<"---------------TEST2---------------"<<std::endl;
		/**	Software trigger timing test 
		//	Enable trigger
		// 	Set state to ON
		//	Set current
		//	Print set current
		//	Set current tracking level (stabilisation range)
		// 	Print the read current until the set value is reached
		//	Print the read current until it is stabilized
		//	Print the current, voltage and state, (fail etc) 
		//	Latch the current set point
		//	Set trigger delay 3
		//	Send software trigger 
		// 	Print the read current until the set value is reached
		//	Print the read current until it is stabilized
		//	Print the current, voltage and state, (fail etc) 
		//	Latch the current set point
		//	Set trigger delay 0
		//	Send software trigger 
		// 	Print the read current until the set value is reached
		//	Print the read current until it is stabilized
		//	Print the current, voltage and state, (fail etc) 
		//	Set state to OFF
		*/
		Magnet = new PSC_ETH("192.168.150.101");
		Magnet->clear_all_err();
		Magnet->set_trigger_state(TRIGGER_STATE_ON);
		Magnet->set_state(MAGNET_ON);

		Magnet->set_current(1.12345);
		std::cout<<"CURRENT SETTING:"<<1.12345<<std::endl;
		Magnet->set_current_tracking_level(50e-6);	
		sleep(2);
		//-------------------------------------------------------------------	
		do{
			std::cout<<"\r"<<"MEASCURR1 :"<<Magnet->get_measure_current();
		}while(!Magnet->get_current_slope_completion());
		std::cout<<std::endl;
		do{
			std::cout<<"\r"<<"MEASCURR2 :"<<Magnet->get_measure_current();
		}while(!Magnet->get_current_tracking_status());
		std::cout<<std::endl;
		//-------------------------------------------------------------------
		Magnet->get_idata(&myDataX_Magnet1);

		std::cout<<"---------------IDATA---------------"<<std::endl;
		std::cout<<"MAGNET :"<<myDataX_Magnet1.magnet<<std::endl;
		std::cout<<"POLE :"<<myDataX_Magnet1.pole<<std::endl;
		std::cout<<"STATE :"<<myDataX_Magnet1.state<<std::endl;
		std::cout<<"FAIL :"<<myDataX_Magnet1.fail<<std::endl;
		std::cout<<"MEASVOLT :"<<myDataX_Magnet1.measvolt<<std::endl;
		std::cout<<"MEASCURR :"<<myDataX_Magnet1.meascurr<<std::endl;
		std::cout<<"MEASCURR :"<<Magnet->get_measure_current()<<std::endl;
		std::cout<<"-----------------------------------"<<std::endl;

		Magnet->set_current_latch(-2.65432);
		std::cout<<"CURRENT TRIGGER SETTING:"<<-2.65432<<std::endl;
		Magnet->set_trigger_delay(3);

		sleep(10);
		//initiate_udp_trigger("192.168.150.255");
		//-------------------------------------------------------------------
		do{
			std::cout<<"\r"<<"MEASCURR1 :"<<Magnet->get_measure_current();
		}while(!Magnet->get_current_slope_completion());
		std::cout<<std::endl;
		do{
			std::cout<<"\r"<<"MEASCURR2 :"<<Magnet->get_measure_current();
		}while(!Magnet->get_current_tracking_status());
		std::cout<<std::endl;
		//-------------------------------------------------------------------
		Magnet->get_idata(&myDataX_Magnet1);

		std::cout<<"---------------IDATA---------------"<<std::endl;
		std::cout<<"MAGNET :"<<myDataX_Magnet1.magnet<<std::endl;
		std::cout<<"POLE :"<<myDataX_Magnet1.pole<<std::endl;
		std::cout<<"STATE :"<<myDataX_Magnet1.state<<std::endl;
		std::cout<<"FAIL :"<<myDataX_Magnet1.fail<<std::endl;
		std::cout<<"MEASVOLT :"<<myDataX_Magnet1.measvolt<<std::endl;
		std::cout<<"MEASCURR :"<<myDataX_Magnet1.meascurr<<std::endl;
		std::cout<<"MEASCURR :"<<Magnet->get_measure_current()<<std::endl;
		std::cout<<"-----------------------------------"<<std::endl;

		Magnet->set_current_latch(5);
		std::cout<<"CURRENT TRIGGER SETTING:"<<5<<std::endl;
		Magnet->set_trigger_delay(0);
		//initiate_udp_trigger("192.168.150.255");
		//-------------------------------------------------------------------
		do{
			std::cout<<"\r"<<"MEASCURR1 :"<<Magnet->get_measure_current();
		}while(!Magnet->get_current_slope_completion());
		std::cout<<std::endl;
		do{
			std::cout<<"\r"<<"MEASCURR2 :"<<Magnet->get_measure_current();
		}while(!Magnet->get_current_tracking_status());
		std::cout<<std::endl;
		//-------------------------------------------------------------------
		Magnet->get_idata(&myDataX_Magnet1);

		std::cout<<"---------------IDATA---------------"<<std::endl;
		std::cout<<"MAGNET :"<<myDataX_Magnet1.magnet<<std::endl;
		std::cout<<"POLE :"<<myDataX_Magnet1.pole<<std::endl;
		std::cout<<"STATE :"<<myDataX_Magnet1.state<<std::endl;
		std::cout<<"FAIL :"<<myDataX_Magnet1.fail<<std::endl;
		std::cout<<"MEASVOLT :"<<myDataX_Magnet1.measvolt<<std::endl;
		std::cout<<"MEASCURR :"<<myDataX_Magnet1.meascurr<<std::endl;
		std::cout<<"MEASCURR :"<<Magnet->get_measure_current()<<std::endl;
		std::cout<<"-----------------------------------"<<std::endl;

		Magnet->set_state(MAGNET_OFF);	

		delete Magnet;
	}
	catch(yat::Exception &e)
	{
		for(int i=0;i<e.errors.size();i++)		//gestion des exceptions empilées dans le vecteur d'erreur
       		{
			std::cout<<e.errors[i].reason<<" , "<<e.errors[i].desc<<" , "<<e.errors[i].origin<<" , "<<
					e.errors[i].code<<std::endl;
		}
		delete Magnet;
	}	
	return result;
}

bool test3()
{
	bool result=false;
	try{
		std::cout<<"---------------TEST3---------------"<<std::endl;

		/**	Hardware trigger timing test
		//	Enable trigger
		// 	Set state to ON
		//	Set current
		//	Print set current
		//	Set current tracking level (stabilisation range)
		// 	Print the read current until the set value is reached
		//	Print the read current until it is stabilized
		//	Print the current, voltage and state, (fail etc) 
		//	Latch the current set point
		//	Set trigger delay 3
		//	Send hardware trigger
		//	Wait for current to start changing 
		// 	Print the read current until the set value is reached
		//	Print the read current until it is stabilized
		//	Print the current, voltage and state, (fail etc) 
		//	Set state to OFF
		*/

		Magnet = new PSC_ETH("192.168.150.101");
		Magnet->clear_all_err();
		Magnet->set_trigger_state(TRIGGER_STATE_ON);
		Magnet->set_state(MAGNET_ON);

		Magnet->set_current(1.12345);
		std::cout<<"CURRENT SETTING:"<<1.12345<<std::endl;
		Magnet->set_current_tracking_level(50e-6);	
		//-------------------------------------------------------------------	
		do{
			std::cout<<"\r"<<"MEASCURR1 :"<<Magnet->get_measure_current();
		}while(!Magnet->get_current_slope_completion());
		std::cout<<std::endl;
		do{
			std::cout<<"\r"<<"MEASCURR2 :"<<Magnet->get_measure_current();
		}while(!Magnet->get_current_tracking_status());
		std::cout<<std::endl;
		//-------------------------------------------------------------------
		Magnet->get_idata(&myDataX_Magnet1);

		std::cout<<"---------------IDATA---------------"<<std::endl;
		std::cout<<"MAGNET :"<<myDataX_Magnet1.magnet<<std::endl;
		std::cout<<"POLE :"<<myDataX_Magnet1.pole<<std::endl;
		std::cout<<"STATE :"<<myDataX_Magnet1.state<<std::endl;
		std::cout<<"FAIL :"<<myDataX_Magnet1.fail<<std::endl;
		std::cout<<"MEASVOLT :"<<myDataX_Magnet1.measvolt<<std::endl;
		std::cout<<"MEASCURR :"<<myDataX_Magnet1.meascurr<<std::endl;
		std::cout<<"MEASCURR :"<<Magnet->get_measure_current()<<std::endl;
		std::cout<<"-----------------------------------"<<std::endl;

		Magnet->set_current_latch(-2.65432);
		std::cout<<"CURRENT TRIGGER SETTING:"<<-2.65432<<std::endl;
		Magnet->set_trigger_delay(3);

		//-------------------------------------------------------------------
			while(Magnet->get_measure_current()>0.);
		do{
			std::cout<<"\r"<<"MEASCURR1 :"<<Magnet->get_measure_current();
		}while(!Magnet->get_current_slope_completion());
		std::cout<<std::endl;
		do{
			std::cout<<"\r"<<"MEASCURR2 :"<<Magnet->get_measure_current();
		}while(!Magnet->get_current_tracking_status());
		std::cout<<std::endl;
		//-------------------------------------------------------------------
		Magnet->get_idata(&myDataX_Magnet1);

		std::cout<<"---------------IDATA---------------"<<std::endl;
		std::cout<<"MAGNET :"<<myDataX_Magnet1.magnet<<std::endl;
		std::cout<<"POLE :"<<myDataX_Magnet1.pole<<std::endl;
		std::cout<<"STATE :"<<myDataX_Magnet1.state<<std::endl;
		std::cout<<"FAIL :"<<myDataX_Magnet1.fail<<std::endl;
		std::cout<<"MEASVOLT :"<<myDataX_Magnet1.measvolt<<std::endl;
		std::cout<<"MEASCURR :"<<myDataX_Magnet1.meascurr<<std::endl;
		std::cout<<"MEASCURR :"<<Magnet->get_measure_current()<<std::endl;
		std::cout<<"-----------------------------------"<<std::endl;

		Magnet->set_state(MAGNET_OFF);	
	

		delete Magnet;
	}
	catch(yat::Exception &e)
	{
		for(int i=0;i<e.errors.size();i++)		//gestion des exceptions empilées dans le vecteur d'erreur
       		{
			std::cout<<e.errors[i].reason<<" , "<<e.errors[i].desc<<" , "<<e.errors[i].origin<<" , "<<
					e.errors[i].code<<std::endl;
		}
		delete Magnet;
	}

	return result;
}

bool test4()
{
	bool result=false;

	try{
		std::cout<<"---------------TEST4---------------"<<std::endl;
		Magnet = new PSC_ETH("192.168.150.101");
		std::cout<<Magnet->addrIP()<<std::endl;

		//-------------------------------------------------------------------
		Magnet->get_idata(&myDataX_Magnet1);

		std::cout<<"---------------IDATA---------------"<<std::endl;
		std::cout<<"MAGNET :"<<myDataX_Magnet1.magnet<<std::endl;
		std::cout<<"POLE :"<<myDataX_Magnet1.pole<<std::endl;
		std::cout<<"STATE :"<<myDataX_Magnet1.state<<std::endl;
		std::cout<<"FAIL :"<<myDataX_Magnet1.fail<<std::endl;
		std::cout<<"MEASVOLT :"<<myDataX_Magnet1.measvolt<<std::endl;
		std::cout<<"MEASCURR :"<<myDataX_Magnet1.meascurr<<std::endl;
		std::cout<<"MEASCURR :"<<Magnet->get_measure_current()<<std::endl;
		std::cout<<"-----------------------------------"<<std::endl;

		sleep(5);
		Magnet->set_current(3);

		Magnet->set_state(MAGNET_ON);

		//-------------------------------------------------------------------
		Magnet->get_idata(&myDataX_Magnet1);

		std::cout<<"---------------IDATA---------------"<<std::endl;
		std::cout<<"MAGNET :"<<myDataX_Magnet1.magnet<<std::endl;
		std::cout<<"POLE :"<<myDataX_Magnet1.pole<<std::endl;
		std::cout<<"STATE :"<<myDataX_Magnet1.state<<std::endl;
		std::cout<<"FAIL :"<<myDataX_Magnet1.fail<<std::endl;
		std::cout<<"MEASVOLT :"<<myDataX_Magnet1.measvolt<<std::endl;
		std::cout<<"MEASCURR :"<<myDataX_Magnet1.meascurr<<std::endl;
		std::cout<<"MEASCURR :"<<Magnet->get_measure_current()<<std::endl;
		std::cout<<"-----------------------------------"<<std::endl;
		
		sleep(5);

		//-------------------------------------------------------------------
		Magnet->get_idata(&myDataX_Magnet1);

		std::cout<<"---------------IDATA---------------"<<std::endl;
		std::cout<<"MAGNET :"<<myDataX_Magnet1.magnet<<std::endl;
		std::cout<<"POLE :"<<myDataX_Magnet1.pole<<std::endl;
		std::cout<<"STATE :"<<myDataX_Magnet1.state<<std::endl;
		std::cout<<"FAIL :"<<myDataX_Magnet1.fail<<std::endl;
		std::cout<<"MEASVOLT :"<<myDataX_Magnet1.measvolt<<std::endl;
		std::cout<<"MEASCURR :"<<myDataX_Magnet1.meascurr<<std::endl;
		std::cout<<"MEASCURR :"<<Magnet->get_measure_current()<<std::endl;
		std::cout<<"-----------------------------------"<<std::endl;
		
		delete Magnet;
	}
	catch(yat::Exception &e)
	{
		for(int i=0;i<e.errors.size();i++)		//gestion des exceptions empilées dans le vecteur d'erreur
       		{
			std::cout<<e.errors[i].reason<<" , "<<e.errors[i].desc<<" , "<<e.errors[i].origin<<" , "<<
					e.errors[i].code<<std::endl;
		}
		delete Magnet;
	}

	return result;
}

void init_pole(PSC_ETH* pole)
{
	pole->clear_all_err();
	pole->set_trigger_state(TRIGGER_STATE_ON);
	pole->set_state(MAGNET_ON);
	pole->set_current(0);

	
	pole->set_current_tracking_level(50e-6);	
	//-------------------------------------------------------------------	
	do{
		std::cout<<"\r"<<"MEASCURR1 :"<<pole->get_measure_current();
	}while(!pole->get_current_tracking_status());
	std::cout<<std::endl;
}

void release_pole(PSC_ETH* pole)
{
	sleep(2);
	pole->set_state(MAGNET_OFF);
}

void display_counter(PSC_ETH* pole)
{
	int ctn1,ctn2;
	pole->get_ctn(&ctn1,&ctn2);
	std::cout<<"---------------CTN---------------"<<std::endl;
	std::cout<<"CTN3 :"<<ctn1<<std::endl;
	std::cout<<"CTN4 :"<<ctn2<<std::endl;
}

void display_pole(PSC_ETH* pole)
{

	pole->get_idata(&myDataX_Magnet1);

	std::cout<<"---------------IDATA---------------"<<std::endl;
	std::cout<<"MAGNET :"<<myDataX_Magnet1.magnet<<std::endl;
	std::cout<<"POLE :"<<myDataX_Magnet1.pole<<std::endl;
	std::cout<<"STATE :"<<myDataX_Magnet1.state<<std::endl;
	std::cout<<"FAIL :"<<myDataX_Magnet1.fail<<std::endl;
	std::cout<<"MEASVOLT :"<<myDataX_Magnet1.measvolt<<std::endl;
	std::cout<<"MEASCURR :"<<myDataX_Magnet1.meascurr<<std::endl;
	std::cout<<"MEASCURR :"<<pole->get_measure_current()<<std::endl;
	std::cout<<"-----------------------------------"<<std::endl;
}

void wait_slope_completion(PSC_ETH* pole)
{
	std::cout<<"WAIT CURRENT SLOPE COMPLETION "<<std::endl;
	do{
		std::cout << "\r" << "Current :" << pole->get_measure_current();
	}while( !pole->get_current_slope_completion() );
	std::cout<<std::endl;

}
void wait_tracking(PSC_ETH* pole)
{
	std::cout<<"WAIT CURRENT TRACKING COMPLETION "<<std::endl;
	do{
		std::cout << "\r" << "Current :" << pole->get_measure_current();
	}while( !pole->get_current_tracking_status() );
	std::cout<<std::endl;
}


void display_current(PSC_ETH* pole)
{
	//std::cout<<std::endl;
	try
	{
		std::cout << "\r" << "Current :" << pole->get_measure_current();
		std::cout.flush(); 
	}
	catch(yat::Exception &e)
	{
		for(int i=0;i<e.errors.size();i++)		//gestion des exceptions empilées dans le vecteur d'erreur
       		{
			std::cout<<e.errors[i].reason<<" , "<<e.errors[i].desc<<" , "<<e.errors[i].origin<<" , "<<
					e.errors[i].code<<std::endl;
		}
	}
}

void display_request_rate(PSC_ETH* pole)
{
	timeval start, end, ellapsed;
	int count=0;
	int error=0;
	int nb_request=0;
	long int time=0;

	while(true)
	{
		
		try
		{
			nb_request++;
			gettimeofday(&start, NULL);
			pole->get_measure_current();
			gettimeofday(&end, NULL);
		}
		catch(yat::Exception &e)
		{
			gettimeofday(&end, NULL);
			error++;
			for(int i=0;i<e.errors.size();i++)		//gestion des exceptions empilées dans le vecteur d'erreur
	       		{
				std::cout<<e.errors[i].reason<<" , "<<e.errors[i].desc<<" , "<<e.errors[i].origin<<" , "<<
						e.errors[i].code<<std::endl;
			}
		}
		time = timeval_subtract(&ellapsed, &end, &start);
		std::cout << "\r" << "Time to serve ("<<error<<"/"<< nb_request<<"): "<< time<< "             ";
		std::cout.flush(); 
	}
}


main (int argc, char **argv)
{ 
	int val=0;
	if(argc>1)
	{
		// Display the current current of one pole
		if(strcmp(argv[1],"display-current")==0 && argc>3)
		{
			std::cout << "Current display (pole="<<argv[3]<<";ip="<<argv[2]<<")"<<std::endl;

			std::string ip = argv[2]; 
			int pole_number = atoi(argv[3]);

			PSC_ETH* pole = new PSC_ETH(ip.c_str());
			// Display the value indefinitively
			int i=0;
			while(true)
			{
				//std::cout<<"Frame "<<i++<<std::endl;
				display_current(pole);
				sleep(1);
			}
			// In case of ...
			delete pole;
		}
		// Display the request rate
		else if(strcmp(argv[1],"display-request-rate")==0 && argc>3)
		{
			std::cout << "Current display (pole="<<argv[3]<<";ip="<<argv[2]<<")"<<std::endl;

			std::string ip = argv[2]; 
			int pole_number = atoi(argv[3]);

			PSC_ETH* pole = new PSC_ETH(ip.c_str());
			// Display the value indefinitively
			//std::cout<<"Frame "<<i++<<std::endl;
			display_request_rate(pole);
			// In case of ...
			delete pole;
		}
		else if(strcmp(argv[1],"test-4")==0 && argc>3)	
		{
		}
	}
	else
	{
		std::cout << "Usage : TestMagnet Action Parameters" << std::endl;
		std::cout << std::endl;
		std::cout << "Actions are > " << std::endl;
		std::cout << "* display-request-rate : " << std::endl;
		std::cout << "  Read the current as fast as possible with 1 socket" << std::endl;
		std::cout << "  and display the response time of each request in us" << std::endl;
		std::cout << "* display-current : " << std::endl;
		std::cout << "* test-4 : " << std::endl;
		std::cout << std::endl;
		std::cout << "   - Parameters are : IP" << std::endl;
		std::cout << std::endl;
		std::cout << "Example $> TestMagnet display-request-rate 192.168.150.107" << std::endl;
		std::cout << "        > Current display (ip=192.168.150.107)" << std::endl;
		std::cout << "        > Time to serve (0/25363): 1955             " << std::endl;
		std::cout << "                         |   |      + response time in us " << std::endl;
		std::cout << "                         |   + number of try               " << std::endl;
		std::cout << "                         + number of error                 " << std::endl;
	}
		
			
	return 0;
}

