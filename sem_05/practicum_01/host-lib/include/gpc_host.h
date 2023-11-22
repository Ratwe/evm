#ifndef GPC_H_
#define GPC_H_
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>     //strlen
#include <sys/ioctl.h>  //ioctl
#include <unistd.h>     //write
#include <fcntl.h>      //O_RDWR
#include <errno.h>      //errno
#include <pthread.h>    //threads
#include <condition_variable>
#include <unistd.h>
#include <cstdlib>
#include <poll.h>
#include <thread>
#include <mutex>
#include "gpc_defs.h"
#include "io_ctl.h" 

//=============================================
// Класс для управления Graph Processing Core
//=============================================

class gpc
{
protected:

  int gpc_dev;
  int lnh_nfo_dev;
  struct pollfd pfd;
  std::thread *mq_send_th;
  std::mutex mq_send_th_init;
  std::mutex mq_send_th_write;
  std::mutex mq_send_finished_mu;
  std::condition_variable mq_send_finished;
  std::thread *mq_receive_th;
  std::mutex mq_receive_th_init;
  std::mutex mq_receive_th_read;
  std::mutex mq_received_finished_mu;
  std::condition_variable mq_received_finished;
  int open_file(char *filename);
  void close_file(int fd);
  int read_nfo();
  void mq_send_thread(unsigned long long sent_expected, unsigned int bufsize, char *buf);
  void mq_receive_thread(unsigned long long receive_expected, unsigned int bufsize,char *buf);
  unsigned long long received;
  unsigned long long sent;
  unsigned long long todo_received;
  unsigned long long todo_sent;
 
public:

  char gpc_dev_path[12];
  lnh_nfo nfo[CORE_COUNT];
  char gpc_enable[CORE_COUNT];
  gpc();
  gpc(char* chardev);
  gpc(char* chardev, char* rawbinary);
  ~gpc();
  int start(unsigned long event_handler);
  void finish();
  int load_swk(const char* rawbinary);
  void sync();
  void mq_send(unsigned long long data);
  std::thread* mq_send(unsigned int bufsize,char *buf);
  std::thread* mq_receive(unsigned int bufsize,char *buf);
  unsigned long long mq_receive();
  
};

#endif
