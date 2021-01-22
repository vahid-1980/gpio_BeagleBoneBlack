#include <iostream>
#include <string>
#include <signal.h>

#include "toggleGpio.h"


using namespace std;

atomic<bool> ctrl_c;

int parseCmd(char *cmd[], int num, int *in, int *out, int *log)
{
    int ret = 0;
    int fi = 0,fo = 0;
    string tmp;

    if(num > 4 || num < 3) {
        ret = -1;
    } else {
        for(int i = 1; i < num; i++) {
            tmp = cmd[i];
            if(tmp.find("-i") != std::string::npos && fi == 0){
                fi = 1;
                string::const_iterator it = tmp.begin();
                tmp = tmp.erase(0, 2);
                while(it != tmp.end() && isdigit(*it)) 
                    it++;
                if(!tmp.empty() && it == tmp.end()) {
                    *in = stoi(tmp);
                } else {
                    ret = -1;
                }
            } else if(tmp.find("-o") != std::string::npos && fo == 0) {
                fo = 1;
                string::const_iterator it = tmp.begin();
                tmp = tmp.erase(0, 2);
                while(it != tmp.end() && isdigit(*it)) 
                    it++;
                if(!tmp.empty() && it == tmp.end()) {
                    *out = stoi(tmp);
                } else {
                    ret = -1;
                }
            } else if(tmp == "--log" && *log == 0){
                *log = 1;
            } else {
                ret = -1;
            }
        }
    }
    if(fo == 0 || fi == 0) ret = -1;
    return ret;
}

void usage(void)
{
    cout << "Usage:" << endl << endl;
    cout << "mainApp -i<input gpio number> -o<output gpio number> [--log]" << endl;
    cout << "   input gpio number: a positive integer representing " << endl;
    cout << "   the gpio number which will determine wheather the output gpio blink or not" << endl << endl;
    
    cout << "   output gpio number: a positive integer representing " << endl;
    cout << "   the gpio number which will blink or not" << endl << endl;
 
    cout << "   --log: optional, if used, will print a time stamp in each output gpio toggle on a text file in /tmp/<output gpio number>.txt" << endl;
}

void ctrlCHandler(int sig)
{ 
    ctrl_c = true;
    cout << " got interrupted!" << endl; 
}

int main(int argc, char *argv[]) 
{
    int x = 0, y = 0, z = 0;
    int ret = 0;

    ctrl_c = false;

    signal(SIGINT, ctrlCHandler);

    if(0 != parseCmd(argv, argc, &x, &y, &z)){
        usage();
        return -1;
    } else {
        cout << "log= " << z << endl;
        cout << "in= " << x << endl;
        cout << "out= " << y << endl;
    }

    ToggleGpio toggleGpio(&x, &y, &z);
    toggleGpio.init();

    return ret;
}
