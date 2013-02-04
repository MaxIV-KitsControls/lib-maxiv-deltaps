#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <iostream>

#include "ClassMagnet.h"
#include "gpio.h"

#define GPIO_PIN 8
#define MAX_WAVEFORM_SAMPLES 1024

using namespace std;
using namespace itest;

// globals
float buffer[MAX_WAVEFORM_SAMPLES];

itPole2811 *m1, *m2;

int read_waveform(const char *filename);

void test1()
{
	cout << "########################################" << endl;
	cout << "Test 1" << endl;
	cout << "########################################" << endl;
	m1->set_current(0);
	m2->set_current(0);
	m1->set_state(1);
	m2->set_state(1);
	m1->set_trigger_state(TRIGGER_STATE_OFF);
	m2->set_trigger_state(TRIGGER_STATE_OFF);
	sleep(2);

	for(int i = 0; i<1000; i++)
	{
		printf("%d\n", i);
		gpio_set(GPIO_PIN, 1);
		m1->set_current(1);
		m2->set_current(1);
		gpio_set(GPIO_PIN, 0);
		usleep(100000);
		m1->set_current(0);
		m2->set_current(0);
		usleep(200000);
	}

	m1->set_state(0);
	m2->set_state(0);
}

void test2()
{
	cout << "########################################" << endl;
	cout << "Test 2" << endl;
	cout << "########################################" << endl;

	m1->set_current(0);
	m2->set_current(0);
	m1->set_state(1);
	m2->set_state(1);
	m1->set_trigger_state(TRIGGER_STATE_ON);
	m2->set_trigger_state(TRIGGER_STATE_ON);
	m1->set_trigger_delay(0);
	m2->set_trigger_delay(0);
	sleep(2);

	for(int i = 0; i<1000; i++)
	{
		printf("%d\n", i);
		m1->set_current_latch(1);
		m2->set_current_latch(1);
		usleep(20000);

		gpio_set(GPIO_PIN, 1);
		initiate_udp_trigger("192.168.150.255");
		gpio_set(GPIO_PIN, 0);

		usleep(100000);
		m1->set_current(0);
		m2->set_current(0);
		usleep(100000);
	}

	m1->set_state(0);
	m2->set_state(0);
}

void test3()
{
	cout << "########################################" << endl;
	cout << "Test 3" << endl;
	cout << "########################################" << endl;

	m1->set_current(0);
	m1->set_state(1);
	m1->set_trigger_state(TRIGGER_STATE_OFF);
	sleep(2);

	int n_samples = read_waveform("test3.txt");
	double readback;
	while (1)
	{
		for (int i=0; i<n_samples; i++)
		{
			m1->set_current(buffer[i]);
			readback = m1->get_measure_current();
			printf ("%03d: %lf\n", i, readback);
			usleep(1000);
		}
	}
	m1->set_current(0);
	m1->set_state(0);
}

int read_waveform(const char *filename)
{
	int n;
	FILE *fd = fopen(filename, "r");

	if (!fd)
	{
		perror("read_waveform()");
		return 0;
	}

	for(n=0; n<MAX_WAVEFORM_SAMPLES; n++)
		if (fscanf(fd, "%f\n", &buffer[n]) <= 0)
			break;

	fclose(fd);
	cout << "read_waveform(): read " << n << " samples" << endl;
	return n;
}

int main()
{
	int rv = gpio_init(GPIO_PIN, GPIO_OUTPUT);
	if (rv < 0)
	{
		perror("gpio_init failed");
		return rv;
	}
	try {
		m1 = new itPole2811("192.168.150.101", 1, 1);
		m2 = new itPole2811("192.168.150.107", 1, 2);
		test2();
	}
	catch(ItestException &e)
	{
		for(int i=0; i<e.errors.size(); i++)
      		{
			cout << "Error: " << e.errors[i].desc << " " << e.errors[i].reason << endl;
		}
	}

	delete m1;
	delete m2;
	rv = gpio_free(GPIO_PIN);

	return rv;
}
