#include <iostream>
#include "toggleGpio.h"

ToggleGpio::ToggleGpio(const int *in, const int *out, const int *log):input_gpio(*in), output_gpio(*out), log(*log)  
{

}


int ToggleGpio::init(void)  
{

    return 0;
}


void ToggleGpio::start(void) 
{

}
