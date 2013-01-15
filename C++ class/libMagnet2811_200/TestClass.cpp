#include <sstream>
#include <iostream>

#include <stdio.h>

#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "ClassMagnet.h"

//g++ -o TestClass TestClass.cpp -L./ -lMagnet
//--------
//g++ -D_REENTRANT -o TestClass TestClass.cpp -L./ -lMagnet -lpthread
//--------

using namespace std;
using namespace itest;
itPole2811 *Magnet=0;

main (int argc, char **argv)
{ 
	int val=0;
	struct idata myDataX_Magnet1;

/*	try{
		std::cout<<"---------------TEST0---------------"<<std::endl;
		Magnet = new itPole2811("192.168.150.101",1,4);
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
		initiate_udp_trigger("192.168.150.255");

		while(getchar() != 'q')
			std::cout<<"\r"<<"MEASCURR1 :"<<Magnet->get_measure_current();
		std::cout<<std::endl;
		Magnet->set_current_buffer_loop(1);
		Magnet->set_current_latch_source(CURR_LATCH_SOURCE_VALUE);
		Magnet->set_state(MAGNET_OFF);
	}
	catch(ItestException &e)
	{
		for(int i=0;i<e.errors.size();i++)		//gestion des exceptions empilées dans le vecteur d'erreur
       		{
			std::cout<<e.errors[i].reason<<" , "<<e.errors[i].desc<<" , "<<e.errors[i].origin<<" , "<<
					e.errors[i].code<<std::endl;
		}
		delete Magnet;
	}
	try{
		std::cout<<"---------------TEST1---------------"<<std::endl;
		Magnet = new itPole2811("192.168.150.101",1,4);
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
	catch(ItestException &e)
	{
		for(int i=0;i<e.errors.size();i++)		//gestion des exceptions empilées dans le vecteur d'erreur
       		{
			std::cout<<e.errors[i].reason<<" , "<<e.errors[i].desc<<" , "<<e.errors[i].origin<<" , "<<
					e.errors[i].code<<std::endl;
		}
		delete Magnet;
	}	
*/
/*
	try{
		std::cout<<"---------------TEST2---------------"<<std::endl;
		Magnet = new itPole2811("192.168.150.101",1,4);
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

		initiate_udp_trigger("192.168.150.255");
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
		initiate_udp_trigger("192.168.150.255");
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
		int ctn1,ctn2;
		Magnet->get_ctn(&ctn1,&ctn2);
		std::cout<<"---------------CTN---------------"<<std::endl;
		std::cout<<"CTN3 :"<<ctn1<<std::endl;
		std::cout<<"CTN4 :"<<ctn2<<std::endl;

		delete Magnet;
	}
	catch(ItestException &e)
	{
		for(int i=0;i<e.errors.size();i++)		//gestion des exceptions empilées dans le vecteur d'erreur
       		{
			std::cout<<e.errors[i].reason<<" , "<<e.errors[i].desc<<" , "<<e.errors[i].origin<<" , "<<
					e.errors[i].code<<std::endl;
		}
		delete Magnet;
	}	

	try{
		std::cout<<"---------------TEST2---------------"<<std::endl;
		Magnet = new itPole2811("192.168.150.101",1,4);
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
		int ctn1,ctn2;
		Magnet->get_ctn(&ctn1,&ctn2);
		std::cout<<"---------------CTN---------------"<<std::endl;
		std::cout<<"CTN3 :"<<ctn1<<std::endl;
		std::cout<<"CTN4 :"<<ctn2<<std::endl;

		delete Magnet;
	}
	catch(ItestException &e)
	{
		for(int i=0;i<e.errors.size();i++)		//gestion des exceptions empilées dans le vecteur d'erreur
       		{
			std::cout<<e.errors[i].reason<<" , "<<e.errors[i].desc<<" , "<<e.errors[i].origin<<" , "<<
					e.errors[i].code<<std::endl;
		}
		delete Magnet;
	}*/

	try{
		std::cout<<"---------------TEST0---------------"<<std::endl;
		Magnet = new itPole2811("192.168.150.101",3,4);
		std::cout<<Magnet->addrIP()<<std::endl;
		Magnet->set_current(1);

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
		
	}
	catch(ItestException &e)
	{
		for(int i=0;i<e.errors.size();i++)		//gestion des exceptions empilées dans le vecteur d'erreur
       		{
			std::cout<<e.errors[i].reason<<" , "<<e.errors[i].desc<<" , "<<e.errors[i].origin<<" , "<<
					e.errors[i].code<<std::endl;
		}
		delete Magnet;
	}
    return 0;
}

