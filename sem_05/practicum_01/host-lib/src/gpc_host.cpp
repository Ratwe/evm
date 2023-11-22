#include "gpc_host.h"

// Constructor
gpc::gpc() //open any free gpc
{
	int core=0;
	received=sent=todo_received=todo_sent=0ull;
	mq_receive_th=mq_send_th=NULL;
	mq_send_finished.notify_all();
	mq_received_finished.notify_all();
	{
		//Get list of available gpc's
		read_nfo(); //nfo device
		for (core=0;core<CORE_COUNT;core++) {
			if (nfo[core].is_present && !nfo[core].dev_open) {
				sprintf(gpc_dev_path,"/dev/gpc%d",core);
        		//open dev file
        		gpc_dev = open_file(gpc_dev_path);
				pfd.fd = gpc_dev;
  				pfd.events = ( GPC_STATE );
				break;
			}
		}
	} while (gpc_dev==-1);
}

gpc::gpc(char* chardev) 
{
	received=sent=todo_received=todo_sent=0ull;
	mq_receive_th=mq_send_th=NULL;
	gpc_dev = open_file(chardev);
	pfd.fd = gpc_dev;
	pfd.events = ( GPC_STATE );
	sprintf(gpc_dev_path,"%s",chardev);
}

gpc::gpc(char* chardev, char* rawbinary)
{
	received=sent=todo_received=todo_sent=0ull;
	mq_receive_th=mq_send_th=NULL;
	gpc_dev = open_file(chardev);
	pfd.fd = gpc_dev;
	pfd.events = ( GPC_STATE );
	load_swk(rawbinary);
} 

// Destructor
gpc::~gpc()
{
	close_file(gpc_dev);
}

//Open file
int gpc::open_file(char *filename)
{
  	int fd;
  	fd = open(filename, O_RDWR);
  	if (fd == -1) {
  		perror("open");
  	}
  	return (fd);
}

//Close file
void gpc::close_file(int fd)
{
  	if (close(fd) != 0) {
  		perror("close");
  	}
}

//Read nfo
int gpc::read_nfo()
{
	char device [] = "/dev/lnh_nfo0";
	lnh_nfo_dev = open_file(&device[0]);
	if (ioctl(lnh_nfo_dev, INFO_READ, (void *)&nfo) == -1) {
		printf("errno %d\n", errno); 
		close_file(lnh_nfo_dev); 
		return -1;
	}
	close_file(lnh_nfo_dev); 
	return 0;
}

// Basic methods
int gpc::start(unsigned long event_handler)
{
	if( poll(&pfd, (unsigned long)1, WAIT_FOR_IDLE)<=0 || ( pfd.revents & GPC_STATE ) != GPC_STATE  || ioctl(gpc_dev, GPC_START, (unsigned long)&event_handler) == -1) {
        printf("Wait gpc idle state too long, check you code or set WAIT_FOR_IDLE correctly, errno %d\n", errno);  
        return -1;
    }
    return 0;
}

// Wait for IDLE state
void gpc::finish()
{
	if( poll(&pfd, (unsigned long)1, -1)<=0 || ( pfd.revents & GPC_STATE ) != GPC_STATE  ) {
        printf("Wait gpc idle state too long, check you code or set WAIT_FOR_IDLE correctly, errno %d\n", errno);  
    }
}

//Reset GPC and load sw_kernel
int gpc::load_swk(const char* rawbinary)
{
	struct rawbin_t q;

    FILE *f = fopen(rawbinary, "rb");
    if (f == NULL) 
        return -1; 
    fseek(f, 0, SEEK_END);
    q.size = ftell(f);
    fseek(f, 0, SEEK_SET);
    //Allocate buffer
    char rawbinary_buf[q.size];
    q.data = rawbinary_buf;
    //Read file to buffer
    if (q.size != fread(rawbinary_buf, sizeof(char), q.size, f)) 
        return -1;
    fclose(f);
    if (ioctl(gpc_dev, GPC_REBOOT, (unsigned long)&q) == -1) {
        printf("errno %d\n", errno);  
        return -1;
    }
    return 0;
}

//Sync host with sw_kernel
void gpc::sync()
{
    mq_send(0xdeadbeafdeadbeaf);
    while (mq_receive()!=0xbeafdeadbeafdead);
}

//Send buffer to mq
void gpc::mq_send_thread(unsigned long long sent_expected, unsigned int bufsize, char *buf)
{
	unsigned int offs=0;

	std::unique_lock <std::mutex> locker(mq_send_finished_mu);
	while (sent != sent_expected) { 
        mq_send_finished.wait(locker);
    }
	mq_send_th_write.lock();
	while(offs!=bufsize) {
		offs+=write(gpc_dev, (char*)buf+offs, bufsize-offs);
	}
	sent+=bufsize;
	mq_send_finished.notify_all();
	mq_send_th_write.unlock();
}

//Send ull to mq
void gpc::mq_send(unsigned long long data)
{
	mq_send_th_init.lock();
	mq_send_thread(todo_sent, sizeof(unsigned long long), (char*)&data);
	todo_sent+=sizeof(unsigned long long);
	mq_send_th_init.unlock();
}

//Send buffer to mq asyncronously
std::thread* gpc::mq_send(unsigned int bufsize,char *buf)
{
	mq_send_th_init.lock();
    std::thread *mq_send_thread_ = new std::thread(&gpc::mq_send_thread, this,  todo_sent, bufsize, buf);
	todo_sent+=bufsize;
	mq_send_th_init.unlock();
	return mq_send_thread_;
}

//Receive buf from mq
void gpc::mq_receive_thread(unsigned long long received_expected, unsigned int bufsize,char *buf)
{
	unsigned int offs=0;
	//wait for prev thread joining
	std::unique_lock <std::mutex> locker(mq_received_finished_mu);
	while (received != received_expected) { 
        mq_received_finished.wait(locker);
    }
	mq_receive_th_read.lock();
	while(offs!=bufsize) {
		offs+=read(gpc_dev, (char*)buf+offs, bufsize-offs);
	}
	received += bufsize;
	mq_received_finished.notify_all();
	mq_receive_th_read.unlock();
}

//Receive buf from mq asyncronously
std::thread* gpc::mq_receive(unsigned int bufsize,char *buf)
{
	mq_receive_th_init.lock();
    std::thread *mq_receive_thread_ = new std::thread(&gpc::mq_receive_thread, this,  todo_received, bufsize, buf);
	todo_received+=bufsize;
	mq_receive_th_init.unlock();
	return mq_receive_thread_;
}

//Receive ull from mq
unsigned long long gpc::mq_receive()
{
	unsigned long long data;
	mq_receive_th_init.lock();
    mq_receive_thread(todo_received, sizeof(unsigned long long), (char*)&data);
	todo_received+=sizeof(unsigned long long);
	mq_receive_th_init.unlock();
	return data;
}

