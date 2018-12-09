A simple C program that monitors network traffic over wireless interfaces.


The Raspberry pi3 that was used to this project has been patched with nexmon to enable monitor mode on the wireless network interfaces.
Read more about Nextmon [here](https://github.com/seemoo-lab/nexmon).

The Raspberry was setup with a wireless interface *mon0* that works with the proof-of-concpet code.


## Requirements

Following the tcpdump approach and uses Libraries containing pcap.

This code relies on the lpcap library.

Before compiling make sure you have lpcap library installed.  
`sudo apt-get install libpcap-dev`

- The code is very simple and portable to any deivce that can compile/run C code.


## Usage

Accepts arguments -h and -i.

```
Displays requests of nerby wireless devices.
-i [interface]  Select which interface to monitor.
-h              Display help message.
```

#### Task 1  
Task1 is completed with a single scanner.c file.
The file can be compiled with **gcc -o Scanner scanner.c -lpcap**.
If user want to specify different filters to be used to capture wireless traffic it should be done changing the filter definiting in filter_params.h. More information under [Filters](Filters).

#### Task 2

Task2 have a very simple implementation of a json-builder that builds and handles the json objects from the recieved data.

Requires you to compile with **gcc -o Scanner scanner.c json-builder.c -lpcap**

The API documentation can be found from the readme in the Task2 directory: [link]().
For demonstration purposes the json-builder prints the object instead of sending it over HTTP(s).

#### Filters  
Viable filters is pcap-filter, for a more detailed description read **man pcap-filter** or **man 7 pcap-filter**.

## Updates/Further work

* The code should be changed to work with changing filter settings on the fly or at start.
  Now the filters are in a header file and therefore when you want to change filter settings you have to recompile the project.
* Simple API server to communicate from the client following the API documentation.
