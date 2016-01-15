#8005-a1-pt-benchmarker

This project is a benchmarking tool that measures the timelapse differences between processes and threads. The project
is configured to work best in the CLion IDE from JetBrains. As the project is based out of the included CMake file
included though it can be compiled from there aswell.

#Setup
To setup:

1. Clone the repo with CLion
2. Select Run > Run. You will be prompted with 2 build options
    1. 8005-a1-pt-benchmarker will build the benchmarker to test threads on your system
    2. p8005-a1-pt-benchmarker will build the benchmarker to test processess on your system
    
3. After compilation has completed the program will run

#Potential Bugs
The project uses the GMP math library and the POSIX thread library. If these libraries have not been included on your
system you may run into issues. The CMake file has been configured so as to include these libraries at compile time, so
you should hopefully receive errors about these missing libraries at compile time.