#include "gpc_io_swk.h"

using namespace std;

//====================================================
// Чтение 64 бит из очереди
//====================================================

unsigned long long mq_receive() {
    unsigned long long h2c_cidx=H2C_CIDX;
    unsigned long long data;
    while (H2C_PIDX == h2c_cidx);
    data = *((volatile unsigned long long*)(MSG_MEMORY_BASE + (h2c_cidx<<3)));
    H2C_CIDX = (h2c_cidx+1) % (H2C_MSG_DATA>>3);
    return data;
}

//====================================================
// Чтение всего содержимого из очереди
//====================================================

unsigned int mq_receive(unsigned int bufsize,char *buf) {
    unsigned int offs=0;
    unsigned long long h2c_cidx=H2C_CIDX;
    while (H2C_PIDX != h2c_cidx && offs<bufsize) {
        *((volatile unsigned long long*)(buf+offs)) = *((volatile unsigned long long*)(MSG_MEMORY_BASE + (h2c_cidx<<3)));
        h2c_cidx = (h2c_cidx+1) % (H2C_MSG_DATA>>3);
        H2C_CIDX=h2c_cidx;
        offs+=sizeof(unsigned long long);
    }
    return offs;
}


//====================================================
// Запись 64 бит в очередь
//====================================================

void mq_send(unsigned long long data) {
    unsigned long long c2h_pidx=C2H_PIDX;
    while  (C2H_CIDX == (c2h_pidx+1)%((C2H_MSG_DATA-C2H_MSG_REGION)>>3));
    *((volatile unsigned long long*)(MSG_MEMORY_BASE + C2H_MSG_REGION + (c2h_pidx<<3))) = data;
    C2H_PIDX = (c2h_pidx+1)%((C2H_MSG_DATA-C2H_MSG_REGION)>>3);
}




//====================================================
// Запись в DMA FIFO с немедленной отправкой
//====================================================

void mq_send_dma_flush(unsigned long long data) {
    C2H = data;
    C2H_FLUSH = 1;
}

//====================================================
// Запись в DMA FIFO с пакетной отправкой
//====================================================

void mq_send_dma(unsigned long long data) {
    C2H = data;
}

//====================================================
// Запись блока в DMA FIFO с пакетной отправкой
//====================================================

void mq_send_dma(unsigned int bufsize,char *buf) {
    unsigned int offs;
    for (offs=0;offs<bufsize;offs+=8) {
        C2H = *((volatile unsigned long long*)(buf+offs));
    }
    C2H_FLUSH = 1;
}


//====================================================
// Чтение из MSG памяти в RAM
//====================================================


void msg_read(unsigned int size, char *local_buf)
{
    for (unsigned long long int i=0; i<size; i+=8) 
        *((volatile unsigned long long *)(local_buf + i)) = *((volatile unsigned long long*)(MSG_MEMORY_BASE + i));
}


//====================================================
// Запись в MSG из RAM 
//====================================================

void msg_write(unsigned int size, char *local_buf)
{
    for (unsigned long long int i=0; i<size; i+=8) 
        *((volatile unsigned long long*)(MSG_MEMORY_BASE + i)) = *((volatile unsigned long long *)(local_buf + i));
}



//====================================================
// Чтение буфера из global memory в RAM
//====================================================

void bin_read(unsigned int size, char *local_buf)
{
    for (unsigned long long int i=0; i<size; i+=8) 
        *((volatile unsigned long long *)(local_buf + i)) = *((volatile unsigned long long*)(BIN_MEMORY_BASE + i));
}


//====================================================
// Чтение буфера из global memory в RAM
//====================================================

void bin_write(unsigned int size, char *local_buf)
{
    for (unsigned long long int i=0; i<size; i+=8) 
        *((volatile unsigned long long*)(BIN_MEMORY_BASE + i)) = *((volatile unsigned long long *)(local_buf + i));
}

//====================================================
// Синхронизация с хост (рукопожатие)
//====================================================
void sync() 
{
    while (mq_receive()!=0xdeadbeafdeadbeaf);
    mq_send(0xbeafdeadbeafdead);
}

