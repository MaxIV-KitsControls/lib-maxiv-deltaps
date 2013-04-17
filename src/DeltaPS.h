#ifndef _DELTAPS_H_
#define _DELTAPS_H_

#include <yat/network/ClientSocket.h>
#include <yat/Exception.h>

#define SOCK_PORT 8462

#define MAGNET_OFF		0
#define MAGNET_ON		1

#define TRIGGER_STATE_OFF	0
#define TRIGGER_STATE_ON	1
#define TRIGGER_EDGE_RISING	0
#define TRIGGER_EDGE_FALLING	1

#define CURR_LATCH_SOURCE_VALUE	0
#define CURR_LATCH_SOURCE_BUFFER 1

extern void initiate_udp_trigger(const char *_mask);

class PSC_ETH
{
public:
	PSC_ETH(const std::string ip_address);

	virtual ~PSC_ETH();

	std::string idn();
	double max_voltage();
	std::string addrIP(void){return ip_address;}

		//current
		double get_measure_current(void) throw (yat::Exception);
		double get_current(void) throw (yat::Exception);
		void set_current(double ValF) throw (yat::Exception);
		double get_current_latch(void) throw (yat::Exception);
		void set_current_latch(double ValF) throw (yat::Exception);

		bool get_current_latch_source(void) throw (yat::Exception);
		void set_current_latch_source(bool Val) throw (yat::Exception);

		double get_current_tracking_level(void) throw (yat::Exception);
		void set_current_tracking_level(double ValF) throw (yat::Exception);

		bool get_current_tracking_status(void) throw (yat::Exception);
		bool get_current_slope_completion(void) throw (yat::Exception);

		double get_measure_voltage(void) throw (yat::Exception);

		void get_idata(struct idata *) throw (yat::Exception);

		bool get_trigger_edge(void) throw (yat::Exception);

		//trigger
		bool get_trigger_state(void) throw (yat::Exception);
		void set_trigger_state(bool Val) throw (yat::Exception);
		double get_trigger_delay(void) throw (yat::Exception);
		void set_trigger_delay(double ValF) throw (yat::Exception);

        	void initiate_trigger(void) throw (yat::Exception);

		void clear_all_err(void) throw (yat::Exception);

		int get_state(void) throw (yat::Exception);
		int get_num_alarm(void) throw (yat::Exception);

		void clear_alarm(void) throw (yat::Exception);

		void set_state(bool val) throw (yat::Exception);

		void get_ctn(int *ctn1,int *ctn2) throw (yat::Exception);

		//table
		void set_current_buffer(float *valF,int index,int count);
		void get_current_buffer(float *valF,int index,int count);
		void set_current_buffer_loop(bool state);
		bool get_current_buffer_loop(void);

		void set_current_buffer_sample(float valF);
		float get_current_buffer_sample(void);
		void set_current_buffer_range(int index,int count);
		void get_current_buffer_range(int *index,int *count);

private:
	yat::ClientSocket sock;
	std::string ip_address;
};

struct idata{
	int magnet;
	int pole;
	int state;
	std::string fail;
	double measvolt;
	double meascurr;
};
#endif // _DELTAPS_H_
