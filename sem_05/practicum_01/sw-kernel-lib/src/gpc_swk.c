/*
 * gpc_swk.h
 *
 * sw_kernel library
 *
 *  Created on: April 23, 2021
 *      Author: A.Popov
 */

#include "gpc_swk.h"

using namespace std;

//====================================================
// Установка флага состояния ядра 
//====================================================																														

void set_gpc_state(unsigned char val) {

   GPC_STATE = val;
}

//====================================================
// Чтение конфигурации ядра 
//====================================================

unsigned long long gpc_config() {
    
   return GPC_CONFIG; 
}

//====================================================
// Чтение бита запуска ядра 
//====================================================

bool gpc_start() {

   return (GPC_START & START_REQUEST); 
}

//====================================================
// Ожидание сигнала START и чтение номера обработчика
//====================================================

unsigned long long wait_event() {

   unsigned long long data;
   while (!gpc_start());
   do data = gpc_config(); while (gpc_start());
   return data;
}
//====================================================
// Запись данных в регистр  
//====================================================                                                                                          

void handler_irq(unsigned long long payload) {
   while (HANDLER_IRQ);
   HANDLER_IRQ = payload;
}

//====================================================
// Функция записи 8 бит по адресу  
//====================================================

void axi_wr_int8(unsigned long long offs, unsigned char val) {
    
   *((volatile unsigned char *)(offs)) = val;
}

//====================================================
// Функция чтения 32 бит по адресу  
//====================================================

unsigned char axi_rd_int8(unsigned long long offs) {
    
   return *((volatile unsigned char *)(offs)); 
}

//====================================================
// Функция записи 16 бит по адресу  
//====================================================

void axi_wr_int16(unsigned long long offs, unsigned short int val) {
    
   *((volatile unsigned short int *)(offs)) = val;
}

//====================================================
// Функция чтения 16 бит по адресу  
//====================================================

unsigned short int axi_rd_int16(unsigned long long offs) {
    
   return *((volatile unsigned short int *)(offs)); 
}


//====================================================
// Функция записи 32 бит по адресу  
//====================================================

void axi_wr_int32(unsigned long long offs, unsigned int val) {
    
   *((volatile unsigned int *)(offs)) = val;
}

//====================================================
// Функция чтения 32 бит по адресу  
//====================================================

unsigned int axi_rd_int32(unsigned long long offs) {
    
   return *((volatile unsigned int *)(offs)); 
}


//====================================================
// Функция чтения 64 бит по адресу  
//====================================================

unsigned long long axi_rd_int64(unsigned long long offs) {
    
   return *((volatile unsigned long long *)(offs)); 
}


//====================================================
// Функция записи 64 бит по адресу и по значению 
//====================================================

void axi_wr_int64(unsigned long long offs, unsigned long long val) {
    
   *((volatile unsigned long long *)(offs)) = val;
}
   
//====================================================
// Функция записи 64 бит по адресу и по ссылке  
//====================================================

void axi_rd_int64(unsigned long long offs, void *buf) {

    *((unsigned long long*)buf) = *((volatile unsigned long long *)(offs));
}

//====================================================
// Функция записи 64 бит по адресу и по ссылке 
//====================================================

void axi_wr_int64(unsigned long long offs, void *buf) {
    
   *((volatile unsigned long long *)(offs)) = *((unsigned long long*)buf);

}

