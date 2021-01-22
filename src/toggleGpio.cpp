#include "toggleGpio.h"

ToggleGpio::ToggleGpio(const int *in, const int *out, const int *log): input_gpio(*in), output_gpio(*out), log(*log)  
{
    run = false;
    ctrl_c = false;

    if(tglThreadPtr == nullptr) tglThreadPtr = new thread(&ToggleGpio::toggling, this);
    if(polThreadPtr == nullptr) polThreadPtr = new thread(&ToggleGpio::polling, this);

    filePath_output = GPIO_SYSFS + to_string(output_gpio) + "/value";
    filePath_input = GPIO_SYSFS + to_string(input_gpio) + "/value";
    filePath_log = GPIO_LOGPATH + to_string(output_gpio) + ".txt";
}


int ToggleGpio::init(void)  
{
    gpio_out_value.open(filePath_output);
    gpio_out_value << "0"; // just to make sure the output is off at the start.
    gpio_out_value.close();

    return 0;
}

void ToggleGpio::toggling(void)
{
    char buf[30];
    struct tm * timeInfo;
    auto time = chrono::system_clock::now();
    time_t curentTime = chrono::system_clock::to_time_t(time);
    gpio_out_value.open(filePath_output);
    log_stream.open(filePath_log, log_stream.out | log_stream.app);

    while(!ctrl_c) {
        if(run) {
            gpio_out_value.seekp(0);
            gpio_out_value << "1"; 
            time = chrono::system_clock::now();
            curentTime = chrono::system_clock::to_time_t(time);
            timeInfo = localtime(&curentTime);
            strftime(buf, sizeof(buf), "%Y-%m-%d T %X", timeInfo);
           // cout << buf << " gpio " << output_gpio << " " << "ON" << endl;
            if(log == 1) log_stream << buf << " gpio " << output_gpio << " " << "HIGH" << endl;
            this_thread::sleep_for(std::chrono::milliseconds(1000));//
            gpio_out_value.seekp(0);
            gpio_out_value << "0"; 
            time = chrono::system_clock::now();
            curentTime = chrono::system_clock::to_time_t(time);
            timeInfo = localtime(&curentTime);
            strftime(buf, sizeof(buf), "%Y-%m-%d T %X", timeInfo);
           // cout << buf << " gpio " << output_gpio << " " << "OFF" << endl;
            if(log == 1) log_stream << buf << " gpio " << output_gpio << " " << "LOW" << endl;
            this_thread::sleep_for(std::chrono::milliseconds(1000));
            } else {
            gpio_out_value.seekp(0);
            gpio_out_value << "0";
            this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }

    gpio_out_value.close();
    log_stream.close();
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
    if(log_stream.is_open()) log_stream.close();
}

