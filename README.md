## ToggleGpio App

- Folder Structure
- Compilation
- Running as app
- Running as daemon with systemd
- Logging
- testing


1. Folder Structure
    * inc: contains include files
    * script: contains a helper script to run on target 
            with two numbers as argumants. which will be the gpio input and output.
            this script will export them on sysfs and set the in/out directions. (gpios should be free)
            *Note: this shoud run with root permissions* 
    * build: The final executable will be created in this folder. the name would be *mainApp*.
            *Note: this folder is in .gitignore*
    * src: source files is in there: main.cpp and ToggleGpio.cpp
    * srv: A very simple systemd service file placed in here. please note that: if you want to  use this as service you should add DAEMON definition in main.cpp then compile.
    * Maikfile: very simple make config.

2. Compilation:
    * In case compiling as an app:
        Be sure DAEMON is not defined in main.cpp then you should have proper gcc and modify the Makefile to set currect compiler and include path.
     Mine is:
     ```
        CXX=arm-linux-gnueabihf-g++
        SDKINCPATH=/usr/arm-linux-gnueabihf/include
     ```
     compile:
    ```bash
        make all
    ```
    * In case compile daemon:
        Just be sure you have the *#define DAEMON* line in main.cpp the rest is the same as above 

3. Running as App, copy executable and script to target :
    e.g.
    ```bash
        scp build/mainApp debian@192.168.10.2:/tmp
        scp script/gpio_init.sh debian@192.168.10.2:/tmp
    ```
    Then first run the script. be sure to give it your gpio numbers in then out.

    e.g.
    ```bash
        debian@beaglebone:~$source /tmp/gpio_init.sh 68 67
        debian@beaglebone:~$/tmp/mainApp.sh -i68 -o67 --log
    ``` 
    *Note: mainApp will understand if you change the order of switches. also you could skip --log*
    *incase of error it will give you usage help. it is not the same with the script!*

4. Running the app as daemon: make sure you compiled it as daemon.copy service in target systemd service path and also the executable mainApp in taget /usr/local/bin
    e.g.
    ```bash
        scp build/mainApp debian@192.168.10.2:/usr/local/bin
        scp srv/toggle-gpio.service debian@192.168.10.2:/lib/systemd/system
    ```
    Then use systemctl to bringit up/down:    
    e.g.    
    ```bash
        systemctl start toggle-gpio
        systemctl stop toggle-gpio
        systemctl enable toggle-gpio
        systemctl disable toggle-gpio
    ```
    *Note: if you wish to change to another gpio numbers or add --log to service, you should do it in /lib/systemd/system/toggle-gpio.service file*

5. Logging: if use switch --log the app will put a log of gpio output status in */tmp/log_gpio<num>.txt*    e.g.

    ```
        debian@beaglebone:~$ cat /tmp/log_gpio67.txt
        2018-10-07 T 21:00:41 gpio 67 LOW
        2018-10-07 T 21:00:42 gpio 67 HIGH
        2018-10-07 T 21:00:43 gpio 67 LOW
        2018-10-07 T 21:00:44 gpio 67 HIGH
        2018-10-07 T 21:00:45 gpio 67 LOW
        2018-10-07 T 21:00:46 gpio 67 HIGH
        2018-10-07 T 21:00:47 gpio 67 LOW
        2018-10-07 T 21:07:46 gpio 67 HIGH
        2018-10-07 T 21:07:47 gpio 67 LOW
        2018-10-07 T 21:07:48 gpio 67 HIGH
        2018-10-07 T 21:07:49 gpio 67 LOW
        2018-10-07 T 21:07:50 gpio 67 HIGH
        2018-10-07 T 21:07:51 gpio 67 LOW
        2018-10-07 T 21:07:56 gpio 67 HIGH
    ```

6. Testig: 
    I compiled this with :
    ```
        arm-linux-gnueabihf-g++ (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0
        Copyright (C) 2019 Free Software Foundation, Inc.
    ```
    Also tested on Beaglebone black with linux:
    ```
        debian@beaglebone:~$ uname -a
        Linux beaglebone 4.14.71-ti-r80 #1 SMP PREEMPT Fri Oct 5 23:50:11 UTC 2018 armv7l GNU/Linux
    ```