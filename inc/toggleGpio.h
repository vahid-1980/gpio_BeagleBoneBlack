#ifndef TOGGLEGPIO_H
#define TOGGLEGPIO_H

#include <iostream>
#include <fstream>
#include <thread> 
#include <condition_variable>
#include <mutex>
#include <atomic>


using namespace std;

extern atomic<bool> ctrl_c;

class ToggleGpio {
    private:
        int input_gpio;
        int output_gpio;
        int log;
        
        atomic<bool> run;
        
        string filePath_output;
        string filePath_input;
//        string filePath_output = GPIO_SYSFS + to_string(output_gpio) + "/value";

        thread *tglThreadPtr = nullptr;
        thread *polThreadPtr = nullptr;
        ofstream gpio_out_value;
        ifstream gpio_in_value;

    public:
        int time;
        /*
        I want constructor to initialize the private values
        */
        ToggleGpio(const int *in, const int *out,const int *log);

        /*
        init method will do:
            1- put its start and stop log on stdout
            2- export the gpios and return success/error
            3- if log is on, create a text file in /tmp
            4- create a tread to poll the `in` gpio
            returns zero if successful and -1 otherwise.
        */
        int init(void);
        
        /*
        if init has ended success fully start will run the thread for toggling gpio.
        also will poll the input to run/stop the thread.
        */
        void start(void);
        void stop(void);

        void toggling();
        void polling();
        friend void ctrlCHandler(int sig);

        ~ToggleGpio();
};


#endif