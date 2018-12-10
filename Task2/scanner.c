#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pcap.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include <string.h>
#include <getopt.h> // For long options like -interface and -help.
#include <errno.h> // For better error messages

#include "./filter_params.h"
#include "json_builder.h"

#define PARAMS "hpi:" //input params

#define USAGE_MSG "Error, invalid options. Usage: -h, -i [interface] or -p.\n"
#define HELP_MSG "\nDisplays probe requests of nearby wifi devices.\n"\
	"-i [interface] Which interface to monitor probe requests on.\n"\
	"-h		Display this help message.\n"\
	"-p		Enable output, prints all packages found."


// 1 = True, everything else = False.
#define True 1
#define False !True

static unsigned int packet_counter = 0;
int enableOutput = False;

void packet_handler(u_char *args, const struct pcap_pkthdr *header, const u_char *packet);

int main(int argc, char *argv[]){
	int option;
	char *device;
	pcap_t *handle;
	char error_buffer[PCAP_ERRBUF_SIZE];
	int timeout_limit = 10000; /* In milliseconds */
	while((option = getopt(argc, argv, PARAMS)) != -1) {
		switch(option) {
			case 'h':
				fprintf(stdout, HELP_MSG);
				exit(EXIT_SUCCESS);
			case 'i':		
				device = optarg;
				fprintf(stdout, "device selected....\n");
				break;
			case 'p':
				enableOutput = True;
			    fprintf(stdout,"Output enabled\n");
                break;
            default:
				fprintf(stderr, USAGE_MSG);
				exit(EXIT_FAILURE);
		}
	}

	if(enableOutput == False) fprintf(stdout, "Output disabled, enable with argument -p\n");
	

	if(device == NULL) {
		fprintf(stderr, "Could not open device %s:%s\n",device, error_buffer);
		return 1;
	}

	/* Open device for live capture */
	handle = pcap_open_live(
			device,
			BUFSIZ,
			0,
			timeout_limit,
			error_buffer
			);

	if(handle == NULL){
		fprintf(stderr, "Could not open device %s: %s\n", device, error_buffer);
		return 2;
	}
	// Set monitormode
	if(pcap_can_set_rfmon(handle) == 1){
		fprintf(stdout,"Enable monitor mode on %s.\n",device);
		if(pcap_set_rfmon(handle, 1) == -1){
			fprintf(stderr,"Unable to set monitor mode.\n");
		}	
	}
	
	/* Create filters to only handle WLAN probes
	 *
	 * More about filters from "man pcap-filter"
	 *
	 * */
	char *filter = FILTER_PARAMS; //Filter parameters from filter_params.h
	struct bpf_program fp;
	bpf_u_int32 subnet_mask, ip;

	if(pcap_lookupnet(device, &ip, &subnet_mask, error_buffer) == -1){
		fprintf(stderr,"Could not retrieve device information from: %s, using default values.\nError message: %s\n",device,error_buffer);
		ip = 0;
		subnet_mask = 0;
	}

	if(pcap_compile(handle, &fp, filter, 0, ip)==-1){
		fprintf(stderr,"Error compiling libpcap filter: \"%s\"\n", filter);
	}

	if(pcap_setfilter(handle, &fp)==-1){
		fprintf(stderr,"Error setting libpcap filter \"%s\"\nError message: %s\n",filter, pcap_geterr(handle));

	}
	
	/* Set immediate mode */
	if(pcap_set_immediate_mode(handle,1) == -1) fprintf(stderr,"Could not set immediate mode.\nError message:%s\n", error_buffer);
	

	
	pcap_loop(handle, 0,packet_handler, NULL);


	return 0;
}
void packet_handler(u_char *args, const struct pcap_pkthdr *packet_header, const u_char *packet){
	
	struct radiotap_header{
		uint8_t it_rev;
		uint8_t it_pad;
		uint16_t it_len;
	};

	const u_char *assid; // AP MAC address
	const u_char *bssid; // transmitter MAC address
	const u_char *rssi; // received signal strength
	
	int offset = 0;
	struct radiotap_header *rtaphdr;
	rtaphdr = (struct radiotap_header *) packet;

	// Size of radiotap header. On raspberry it's 24 bytes.
	offset = rtaphdr->it_len;

	if(packet[offset] == 0x80){ 
	
	/* Get information */
	bssid = packet + offset + 10; 	// Transmitter address can be found on offset 34
	//assid = packet + offset +  16	// AP MAC can be found on offset 40.
	rssi = packet + offset - 2; 	// Signal strength in hex value.


	/* Get local time */
	char t_buffer[26];
	struct tm* tm_info = localtime(&(packet_header->ts.tv_sec));

	strftime(t_buffer, 26, "%F %T", tm_info);

	signed int rssiDbm = rssi[0] - 256; // Convert rssi to human readable value.
	char address_str[17];
    sprintf(address_str, "%02x:%02x:%02x:%02x:%02x:%02x", bssid[0], bssid[1], bssid[2], bssid[3], bssid[4], bssid[5]);
    char rssi_str[4]; //rssi range from -26 to -100.
    sprintf(rssi_str, "%d", rssiDbm);
    struct json_object object;
    json_object_init(&object);
    json_object_add_object(&object, "timestamp", t_buffer);
    json_object_add_object(&object, "rssi", rssi_str);
    json_object_add_object(&object, "address", address_str);
    json_object_send(&object); // For demonstration purposes prints to stdout.    
    // json_object_save();
    }
 
	return;
}
