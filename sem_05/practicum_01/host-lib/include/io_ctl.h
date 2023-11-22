#ifndef IO_CTL_H
#define IO_CTL_H
#define IOC_MAGIC 'q'
#include <linux/ioctl.h>

//Chardev ioctl for gpc instances
#define CORE_COUNT 24

//gpc states
#define gpc_init                     0
#define gpc_boot                     1
#define gpc_boot_ok                  2
#define gpc_boot_err                 3
#define gpc_ready                    4
#define gpc_busy                     5



struct rawbin_t
{
        unsigned int size;
        char *data;
};

#define GPC_REBOOT 	_IOW(IOC_MAGIC, 1, struct rawbin_t) 	//universal write operation to device IO port
#define GPC_IS_IDLE	_IOR(IOC_MAGIC, 2, bool*)			//universal read operation from IO port
#define GPC_START 	_IOW(IOC_MAGIC, 3, unsigned long*) 	//universal write operation to device IO port

//Chardev ioctl for info

struct lnh_nfo
{
        char is_present;
        char state;
        int dev_open;
        unsigned int structure_cardinality[8];
        unsigned char subtree_empty[8];
        unsigned char subtree_overflow[8];
        unsigned char subtree_structure[8];
};


#define INFO_READ       _IOW(IOC_MAGIC, 4, struct lnh_nfo[CORE_COUNT])     //universal write operation to device IO port

#endif

