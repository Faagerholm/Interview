#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pcap.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include <string.h>
#include <unistd.h> // Standard symbolics constants and types
#include <getopt.h> // For long options like -interface and -help.
#include <errno.h> // For better error messages
#include <linux/kernel.h> // Needed for KERN_INFO (printk) 

#include "./filter_params.h"

#define PARAMS "hi:" //input params
#define USAGE_MSG "Error, invalid options. Usage: -h or -i [interface]\n"
#define HELP_MSG  "\nDisplays probe requests of nearby wifi devices.\n"\
	"-i [interface] Which interface to monitor probe requests on.\n"\
	"-h		Display this help message.\n"


static unsigned int packet_counter = 0;

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
				fprintf(stdout, HELP_MSG, argv[0]);
				exit(EXIT_SUCCESS);
			case 'i':		
				device = optarg;
				fprintf(stdout, "device selected....\n", argv[0]);
				break;
			default:
				fprintf(stderr, USAGE_MSG, argv[0]);
				exit(EXIT_FAILURE);
		}
	}
	//TODO: implement device lookup.

	if(device == NULL) {
		fprintf(stderr, "Could not open device %s\n",device, error_buffer);
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
	if(pcap_can_set_rfmon(handle) == 1){
		fprintf(stdout,"Setting %s to monitor mode.\n",device);
		if(pcap_set_rfmon(handle, 1) == -1){
			fprintf(stderr,"Unable to set monitor mode.\n");
		}	
	}
	
	/* Create filters to only handle WLAN beacon frames
	 *
	 * More about filters from "man pcap-filter"
	 *
	 * */
	//TODO: work on implementing right filter. beacon works but doesnt accept subtype probe-req
	char *filter = FILTER_PARAMS; //Filter parameters from Project_parameters.h
	struct bpf_program fp;
	bpf_u_int32 subnet_mask, ip;

	if(pcap_lookupnet(device, &ip, &subnet_mask, error_buffer) == -1){
		fprintf(stderr,"Error retrieving device information from: %s, using default values.\n",device);
		ip = 0;
		subnet_mask = 0;
	}

	if(pcap_compile(handle, &fp, filter, 0, ip)==-1){
		fprintf(stderr,"Error compiling libpcap filter, %s\n", filter);
	}
	if(pcap_setfilter(handle, &fp)==-1){
		fprintf(stderr,"Error setting libpcap filter %s\nError message: %s",filter, pcap_geterr(handle));

	}
	
	/* Set immediate mode */
	if(pcap_set_immediate_mode(handle,1) == -1) fprintf(stderr,"Could not set immediate mode.\n", error_buffer);
	

	
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
	const u_char *bssid; //transmitter MAC address
	const u_char *rssi; //received signal strength
	
	int offset = 0;
	struct radiotap_header *rtaphdr;
	rtaphdr = (struct radiotap_header *) packet;

	// Size of radiotap header. On raspberry it's 24 bytes.
	offset = rtaphdr->it_len;

	if(packet[offset] == 0x80){ 
	
	/* Get information */
	bssid = packet + 40; 	// Transmitter address can be found on offset 36
				// AP MAC can be found on offset 40.
	rssi = packet + 22; 	// Signal strength in hex value.


	/* Get local time */
	char t_buffer[26];
	struct tm* tm_info = localtime(&(packet_header->ts.tv_sec));

	strftime(t_buffer, 26, "%T", tm_info);

	signed int rssiDbm = rssi[0] - 256; // Convert rssi to human readable value.
	fprintf(stdout, "ID:%i\t",packet_counter++);
	fprintf(stdout, "%s\t",t_buffer);
	fprintf(stdout, "%ddBm", rssiDbm);
	fprintf(stdout, " %02x:%02x:%02x:%02x:%02x:%02x\n",
			bssid[0],
			bssid[1],
			bssid[2],
			bssid[3],
			bssid[4],
			bssid[5]);
	}

	return;
}