#ifndef TOGGLEGPIO_H
#define TOGGLEGPIO_H

#include <iostream>


class ToggleGpio {
    private:
        int input_gpio;
        int output_gpio;
        int log;

    public:
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


        ~ToggleGpio();
};

#endif