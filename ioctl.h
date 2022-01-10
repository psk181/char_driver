#define MY_MACIG 'G'
#define DATA_READ _IOWR(MY_MACIG, 0, struct data_read)

struct data_read {
	int len;
	char *buffer;
};
