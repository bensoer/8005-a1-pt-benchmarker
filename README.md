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
    
3. After compilation has completed the program will run. The .idea folder contains meta information that has configured
the program to run passing the number 143. The program will solve the 2 prime numbers that make up this number (11 and 13).
4. The project will create a bin folder within the project root which will contain the compiled version of the program.
Upon completion of the benchmark execution a `syslog.log` file will be generated containing formal output of the benchmark
results. It will list in milliseconds the time it took for each calculation along with the amount of time it took from
start to find each prime number making up the passed in number

#Run
To run the program simply requires the following parameter passed at execution
```
./8005_a1_pt_benchmarker -n 143
```
The `-n` parameter followed by a number is the number the benchmarker will try to decompose and find the prime numbers
that make up this passed in number. Please only pass in numbers that are the product of two prime numbers.

#Potential Bugs
The project uses the GMP math library and the POSIX thread library. If these libraries have not been included on your
system you may run into issues. The CMake file has been configured so as to include these libraries at compile time, so
you should hopefully receive errors about these missing libraries at compile time.