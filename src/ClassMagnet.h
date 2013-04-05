// ============================================================================
//
// = CONTEXT
//    ITest Power Supplies - C++ Interface
//
// = FILENAME
//    ClassMagnet.h
//
// = AUTHOR
//    Sebastien Laurent - ITEST
//
// = VERSION
//	 2.00
//
// ============================================================================
#ifndef _ITEST_CLASSMAGNET_H_
#define _ITEST_CLASSMAGNET_H_
//---------------------------------------------------------------------------
#include <string>
#include "Exception.h"

namespace itest{
//---------------------------------------------------------------------------
#define NBRPOLEMAX  3

#define BUFFSIZE	130			//taille buffer de caractère de réception
#define SOCK_PORT	5025		//Port de communication socket

#define MAXTIME_CONNECT_READ	2	//temps max d'attente pour une connexion et une lecture sur BILT modif V201 

#define X	0
#define Y	1
#define Z	2

#define MAGNET_OFF		0
#define MAGNET_ON		1

#define TRIGGER_STATE_OFF	0
#define TRIGGER_STATE_ON	1
#define TRIGGER_EDGE_RISING	0
#define TRIGGER_EDGE_FALLING	1

#define CURR_LATCH_SOURCE_VALUE	0
#define CURR_LATCH_SOURCE_BUFFER 1

#define Q_SYSTERR	":syst:err?"

#define Q_IDATA ";:idata?"
#define Q_MDATA ";:mdata?"
#define Q_FAIL ";:lim:fail?"

#define Q_MVOLT	";:meas:volt?"
#define Q_VOLT_SAT	";:volt:sat?"

#define Q_CURR	";:curr?"
#define Q_CURR_LATCH ";:curr:latch?"
#define Q_CURR_LATCH_SOURCE ";:curr:latch:sour?"
#define Q_MCURR	";:meas:curr?"
#define Q_CURR_SLOPE ";:curr:slop?"
#define Q_CURR_TRACKING_LEVEL ";:curr:track:lev?"

#define Q_STAT ":stat?"

#define Q_COMPLETION	"curr:comp?"
#define Q_TRACK		"curr:track?"

#define CURR	";:curr "
#define CURR_LATCH ";:curr:latch "
#define CURR_LATCH_SOURCE ";:curr:latch:sour "
#define CURR_SLOPE ";:curr:slop "
#define CURR_TRACKING_LEVEL ";:curr:track:lev "

#define STAT	":stat "
#define STATCLEAR ":stat:clear"

#define SYSTVERS	":syst:vers?"

//trigger
#define Q_TRIG_STATE ";:trig:stat?"
#define Q_TRIG_EDGE ";:trig:edge?"
//#define Q_TRIG_SOURCE ";:trig:sour?"
#define Q_TRIG_DEL	";:trig:del?"

#define TRIG_STATE ";:trig:stat "
#define TRIG_EDGE ";:trig:edge "
#define TRIG_SOURCE ";:trig:sour "
#define TRIG_DEL	";:trig:del "
#define TRIG_INITIATE ";:trig:init"

#define Q_LATCH_SOURCE ";:curr:latch:sour?"
#define LATCH_SOURCE ";:curr:latch:sour "

//BUFFER
#define Q_BUFF_LOOP ";:curr:buff:loop?"
#define Q_BUFF_RANG ";:curr:buff:rang?"
#define Q_BUFF_SAMP ";:curr:buff:samp?"

#define BUFF_LOOP ";:curr:buff:loop "
#define BUFF_RANG ";:curr:buff:rang "
#define BUFF_VAL ";:curr:buff "
#define BUFF_SAMP ";:curr:buff:samp "

//test
#define Q_CTN	";:test:temp1?;:test:temp2?"	//modif V140
//---------------------------------------------------------------------------
#define ITEST_EXCEPT_TRY_STR "ItestException exception caught while trying "
#define ITEST_EXCEPT_MAGN_STR "ItestException exception caught on magnet"

#define UNKNOW_EXCEPT_MAGN_STR "Unknown exception caugth on magnet"

#define SOCK_ERROR_STR "Socket error"
#define CONNECT_FAILED_STR "Connection failed"
//---------------------------------------------------------------------------
class Socket{
	private:	
		int Sockfd;
		char R_Buff[BUFFSIZE];	
		bool init_device_done;		//modif V202 07/06/07 pas d'exception dans constructeur mais dans fonctions membres

	protected:
	public:	
		Socket(const char *StrIP);
		virtual ~Socket(void);
		char *query(const std::string& Str);	
		void write(const std::string& Str);
};
//----------------------------------
class itBilt : private Socket	//public Socket modif V202 05/06/07
{
	private:	
		char BuffLastErr[50];
		int NumLastErr;
		const char *StrLastErr;				//modif V202 07/06/07 passage en private
	protected:
	public:	
		itBilt(const char *StrIP);
		virtual ~itBilt(void);
		void write(const std::string& _Str) throw (itest::ItestException);
		char *query(const std::string& _Str) throw (itest::ItestException);	//ajout de const modif V202 05/06/07
};
//---------------------------------------------------------------------------
extern void initiate_udp_trigger(const char *_mask);
//---------------------------------------------------------------------------
//----------------------------------
class itPole2811{

	private:
		int Inst;
		int *PtrSock;
		char *Ptr_R_Buff;
		int NumMagnet;
		int NumPole;
		itBilt *Bilt;
        bool OwnBilt;
		std::string StrAddrIp;
	public:
		itPole2811(itBilt *_Bilt, int _NumMagnet, int _NumPole,int _NumInst);
		itPole2811(const char *_AddrIp, int _NumPole,int _NumInst);
		virtual ~itPole2811(void);

        	//current
		double get_measure_current(void) throw (itest::ItestException);
		double get_current(void) throw (itest::ItestException);
		void set_current(double ValF) throw (itest::ItestException);
		double get_current_latch(void) throw (itest::ItestException);
		void set_current_latch(double ValF) throw (itest::ItestException);

		bool get_current_latch_source(void) throw (itest::ItestException);
		void set_current_latch_source(bool Val) throw (itest::ItestException);

		double get_current_tracking_level(void) throw (itest::ItestException);
		void set_current_tracking_level(double ValF) throw (itest::ItestException);

		bool get_current_tracking_status(void) throw (itest::ItestException);
		bool get_current_slope_completion(void) throw (itest::ItestException);

		double get_measure_voltage(void) throw (itest::ItestException);

		void get_idata(struct idata *) throw (itest::ItestException);

		bool get_trigger_edge(void) throw (itest::ItestException);

		//trigger
		bool get_trigger_state(void) throw (itest::ItestException);
		void set_trigger_state(bool Val) throw (itest::ItestException);
		double get_trigger_delay(void) throw (itest::ItestException);
		void set_trigger_delay(double ValF) throw (itest::ItestException);

        	void initiate_trigger(void) throw (itest::ItestException);

		void clear_all_err(void) throw (itest::ItestException);

		int get_state(void) throw (itest::ItestException);
		int get_num_alarm(void) throw (itest::ItestException);

		void clear_alarm(void) throw (itest::ItestException);

		void set_state(bool val) throw (itest::ItestException);

		void get_ctn(int *ctn1,int *ctn2) throw (itest::ItestException);

		//table
		void set_current_buffer(float *valF,int index,int count);
		void get_current_buffer(float *valF,int index,int count);
		void set_current_buffer_loop(bool state);
		bool get_current_buffer_loop(void);

		void set_current_buffer_sample(float valF);
		float get_current_buffer_sample(void);
		void set_current_buffer_range(int index,int count);
		void get_current_buffer_range(int *index,int *count);

        int instrument(void){return Inst;}
        int num_pole(void){return NumPole;}
		const char *addrIP(void){return StrAddrIp.c_str();}


};
//---------------------------------------------------------------------------
//Structure de données de idata
struct idata{
	int magnet;
	int pole;
	int state;
	std::string fail;
	double measvolt;
	double meascurr;
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
}	//end namespace
//---------------------------------------------------------------------------
#endif //_ITEST_CLASSMAGNET_H_
