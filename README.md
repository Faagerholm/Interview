The Raspberry that this was coded on was setup with mon0 interface that works with the proof-of-concpet code.

## Requirements

Task2 depends on json-c, can be found in this directory or here: https://github.com/json-c.


## Usage

Task1 is completed with the scanner.c file.
The file can be compiled with **gcc scanner.c -lpcap**.
If user want to specify different filters to be used to capture wireless traffic it should be done changing the filter definiting in filter_params.h

Viable filters is pcap-filter, for a more detailed description read **man pcap-filter** or **man 7 pcap-filter**.
