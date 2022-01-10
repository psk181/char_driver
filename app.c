#include <sys/ioctl.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

#include "ioctl.h"

int main() {

	struct data_read kcs_dt;
	int len = 5, i;
	int fd = open("/dev/customdevice", O_WRONLY);
	if (fd < 0) {
		printf("ERROR: %s\n", strerror(errno));
		return -1;
	}
	
	kcs_dt.len = len;
	kcs_dt.buffer = (char*) malloc(len * sizeof(char));
	printf("Addr: %p\n", kcs_dt.buffer);
	if(!kcs_dt.buffer){
         	printf("FAILED to allocate memory for kcs buffer \n");
        	return -1;
	     }

	printf("IOCTL Sending.......... \n");
	int ret = ioctl(fd, DATA_READ, &kcs_dt);
	if (ret < 0) {
			printf("ERROR: %s", strerror(errno));
			printf("ERROR in IOCTL REturn \n");
			return -2;
		}
	
		for(i = 0; i < len; i++)
			printf("Data : %c\n", kcs_dt.buffer);
				
	
	printf("Addr: %p\n", kcs_dt.buffer);
	free(kcs_dt.buffer);
	return 0;
}
