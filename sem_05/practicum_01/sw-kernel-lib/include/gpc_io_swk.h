/*
 * gpc_io_swk.h
 *
 * sw_kernel library
 *
 *  Created on: April 23, 2021
 *      Author: A.Popov
 */

#ifndef GPC_IO_H_
#define GPC_IO_H_

#include "gpc_swk.h"

//====================================
// Адресация ресурсов на шине AXI
//====================================
#define C2H_MSG_BLOCK_START 0x0000000000000000                              // area for IO C2H
#define H2C_MSG_BLOCK_START 0x0000000000000800                              // area for IO H2C
#define C2H_MSG             (*(volatile unsigned long long  *)            (MSG_MEMORY_BASE+C2H_MSG_BLOCK_START)
#define H2C_MSG             (*(volatile unsigned long long  *)            (MSG_MEMORY_BASE+H2C_MSG_BLOCK_START)
#define BIN_MEM             (*(volatile unsigned long long  *)            (BIN_MEMORY_BASE)


//====================================================
// Запись 64 бит в очередь MSG RAM
//====================================================
void mq_send(unsigned long long data);
//====================================================
// Чтение из очереди MSG RAM
//====================================================
unsigned long long mq_receive();
//====================================================
// Чтение всего содержимого из очереди
//====================================================
unsigned int mq_receive(unsigned int bufsize,char *buf);
//====================================================
// Запись в DMA FIFO с немедленной отправкой
//====================================================
void mq_send_dma_flush(unsigned long long data);
//====================================================
// Запись в DMA FIFO с пакетной отправкой
//====================================================
void mq_send_dma(unsigned long long data);
//====================================================
// Запись блока в DMA FIFO с пакетной отправкой
//====================================================
void mq_send_dma(unsigned int bufsize,char *buf);
//====================================================
// Чтение буфера из msg memory в RAM
//====================================================
void msg_read(unsigned int size, char *local_buf);
//====================================================
// Запись буфера из RAM message memory
//====================================================
void msg_write(unsigned int size, char *local_buf);
//====================================================
// Чтение буфера из global memory в RAM
//====================================================
void bin_read(unsigned int size, char *local_buf);
//====================================================
// Чтение буфера из global memory в RAM
//====================================================
void bin_write(unsigned int size, char *local_buf);
//====================================================
// Синхронизация с хост (рукопожатие)
//====================================================
void sync();

#endif
