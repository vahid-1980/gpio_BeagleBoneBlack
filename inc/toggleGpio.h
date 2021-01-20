#ifndef TOGGLEGPIO_H
#define TOGGLEGPIO_H

#include <iostream>


class ToggleGpio {
    private:
        int input_gpio;
        int output_gpio;
        int log;

    public:
        ToggleGpio(char *cmd[], int num, int *x, int *y, int *log);
};

#endif