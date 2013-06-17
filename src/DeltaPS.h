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


class PSC_ETH
{
public:
    PSC_ETH(const std::string ip_address, int ps_group);

    virtual ~PSC_ETH();

    std::string idn();   
    std::string addrIP();
    int get_ps_group();
    
    std::string send_query(std::string query) throw (yat::Exception);

    void send_software_trigger();

    std::string read_error(void) throw (yat::Exception);
    void clear_all_err(void) throw (yat::Exception);

    int get_output_state(void) throw (yat::Exception);
    void set_output_state(bool val) throw (yat::Exception);
    
    int get_interlock_state(void) throw (yat::Exception);

    //current
    double get_max_current() throw (yat::Exception);
    void set_max_current(double v) throw (yat::Exception);

    double get_measure_current(void) throw (yat::Exception);
    double get_source_current(void) throw (yat::Exception);
    void set_current(double ValF) throw (yat::Exception);
    bool is_current_moving() throw (yat::Exception);

    //voltage
    double get_max_voltage() throw (yat::Exception);
    void set_max_voltage(double v) throw (yat::Exception);
    
    double get_measure_voltage(void) throw (yat::Exception);
    double get_source_voltage(void) throw (yat::Exception);
    void set_voltage(double ValF) throw (yat::Exception);
    yat::ClientSocket sock;
    
    //tolerance
    double get_tolerance(void);
    void set_tolerance(double tol);

private:
    std::string ip_address;
    std::string id;
    int ps_group;
    double current_tolerance;
    
    int read_group2_register(void) throw (yat::Exception);
    int read_group3_register(void) throw (yat::Exception);
    
        
        
};


#endif // _DELTAPS_H_
