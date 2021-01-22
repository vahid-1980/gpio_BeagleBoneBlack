#include <chrono>

#include "toggleGpio.h"

using namespace std;

#define GPIO_SYSFS "/sys/class/gpio/gpio"

ToggleGpio::ToggleGpio(const int *in, const int *out, const int *log): input_gpio(*in), output_gpio(*out), log(*log)  
{
    run = false;
    ctrl_c = false;

    if(tglThreadPtr == nullptr) tglThreadPtr = new thread(&ToggleGpio::toggling, this);
    if(polThreadPtr == nullptr) polThreadPtr = new thread(&ToggleGpio::polling, this);
    filePath_output = GPIO_SYSFS + to_string(output_gpio) + "/value";
    filePath_input = GPIO_SYSFS + to_string(input_gpio) + "/value";
}


int ToggleGpio::init(void)  
{
    gpio_out_value.open(filePath_output);
    gpio_out_value << "0"; // just to make sure the output is off at the start.
    gpio_out_value.close();

    return 0;
}


void ToggleGpio::start() 
{
    /* not used it */    
}

void ToggleGpio::stop() 
{
   /* not used it */ 
}

void ToggleGpio::toggling(void)
{
    gpio_out_value.open(filePath_output);
    
    while(!ctrl_c) {
        if(run) {
            gpio_out_value.seekp(0);
            gpio_out_value << "1"; 
            this_thread::sleep_for(std::chrono::milliseconds(1000));
            gpio_out_value.seekp(0);
            gpio_out_value << "0"; 
            this_thread::sleep_for(std::chrono::milliseconds(1000));
        } else {
            gpio_out_value.seekp(0);
            gpio_out_value << "0";
            this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }

    gpio_out_value.close();
}

void ToggleGpio::polling(void)
{
    char c;

    gpio_in_value.open(filePath_input);

    while(!ctrl_c){
        gpio_in_value.clear();
        gpio_in_value.seekg(0);

        gpio_in_value.get(c);
        
        if(c == '1'){
            run = true;
        } else {
            run = false;
        }
        this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    gpio_in_value.close();
}

ToggleGpio::~ToggleGpio()
{
    if(tglThreadPtr != nullptr) {
        if(tglThreadPtr->joinable()) tglThreadPtr->join();
        delete tglThreadPtr;
    }

    if(polThreadPtr != nullptr) {
        if(polThreadPtr->joinable()) polThreadPtr->join();
        delete polThreadPtr;
    }
    
    if(gpio_out_value.is_open()) gpio_out_value.close();
    if(gpio_in_value.is_open()) gpio_in_value.close();
}

