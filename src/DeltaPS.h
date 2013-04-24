#ifndef _DELTAPS_H_
#define _DELTAPS_H_

#include <yat/network/ClientSocket.h>
#include <yat/Exception.h>

#define SOCK_PORT 8462

#define PSGROUP_1               1
#define PSGROUP_2               2
#define PSGROUP_3               3

#define MAGNET_OFF		0
#define MAGNET_ON		1

#define CURRENT_TOLERANCE       0.0001 //in A
#define VOLTAGE_TOLERANCE       0.0001 //in V

#define TRIGGER_STATE_OFF	0
#define TRIGGER_STATE_ON	1
#define TRIGGER_EDGE_RISING	0
#define TRIGGER_EDGE_FALLING	1


class PSC_ETH
{
public:
    PSC_ETH(const std::string ip_address);

    virtual ~PSC_ETH();

    std::string idn();   
    std::string addrIP();
    int get_ps_group();
    
    std::string send_query(std::string query);

    void send_software_trigger();

    std::string read_error(void) throw (yat::Exception);
    void clear_all_err(void) throw (yat::Exception);

    int get_output_state(void) throw (yat::Exception);
    void set_output_state(bool val) throw (yat::Exception);

    //current
    double get_max_current();
    void set_max_current(double v);

    double get_measure_current(void) throw (yat::Exception);
    double get_source_current(void) throw (yat::Exception);
    void set_current(double ValF) throw (yat::Exception);
    bool get_current_state(void) throw (yat::Exception);

    //voltage
    double get_max_voltage();
    void set_max_voltage(double v);
    
    double get_measure_voltage(void) throw (yat::Exception);
    double get_source_voltage(void) throw (yat::Exception);
    void set_voltage(double ValF) throw (yat::Exception);
    bool get_voltage_state(void) throw (yat::Exception);

private:
    yat::ClientSocket sock;
    std::string ip_address;
    std::string id;
    int ps_group;

    int find_psgroup();
        
        
};


#endif // _DELTAPS_H_
