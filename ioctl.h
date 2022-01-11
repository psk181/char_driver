
# define _IOC_WRITE     1U
#define MY_MACIG 'G'

#define DATA_READ _IOC(_IOC_WRITE,'K',0x0d,0x3FFF)
//#define DATA_READ _IOWR(MY_MACIG, 0, struct data_read)

struct data_read {
	int len;
	char *buffer;
};
