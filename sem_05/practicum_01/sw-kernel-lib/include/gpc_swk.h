/*
 * gpc_swk.h
 *
 * sw_kernel library
 *
 */

#ifndef GPC_H_
#define GPC_H_

#include <stdint.h>


//===========================================
// Адресация ресурсов на шине AXI
//===========================================

#define AXI4_BASE               0x0000000000200000      //Global Memory Base Address
#define GPC_START               (*(volatile unsigned char *)     (AXI4_BASE))
#define GPC_STATE               (*(volatile unsigned char *)     (AXI4_BASE+0x8))
#define GPC_CONFIG              (*(volatile unsigned long long int  *)     (AXI4_BASE+0x10))
#define HANDLER_IRQ             (*(volatile unsigned long long int  *)     (AXI4_BASE+0x18))
#define MSG_MEMORY_BASE         0x0000000000201000      //Message Memory Base Address
#define MSG_MEMORY_SIZE         0x0000000000001000      //Message Memory Size
#define BIN_MEMORY_BASE         0x0000000000210000      //Rawbinary Memory Base Address
#define BIN_MEMORY_SIZE         0x0000000000010000      //Rawbinary Memory Size
            

//===========================================
// Адресация ресурсов на шине AXL
//===========================================

#define AXL4LNH64_BASE          0x0000000000300000              //lnh64 microprocessor Base Address

//===========================================
// Адресация ресурсов на шине AXI-Stream в сторону DMA C2H
//===========================================

#define AXIS_C2H_BASE           0x0000000000400000              
#define C2H		                (*(volatile unsigned long long int  *) (AXIS_C2H_BASE))
#define C2H_FLUSH	            (*(volatile unsigned long long int  *) (AXIS_C2H_BASE+0x8))

//===========================================
// Адресация ресурсов в памяти MSG в сторону H2C
//===========================================

#define H2C_MSG_REGION          0x00000 //Address space for MSG memory 
#define H2C_MSG_DATA            0x007e0 //Address space for MSG data 
#define H2C_PIDX                (*((volatile unsigned long long*)(MSG_MEMORY_BASE + H2C_MSG_DATA)))  //Address space for MSG pidx 
#define H2C_CIDX                (*((volatile unsigned long long*)(MSG_MEMORY_BASE + H2C_MSG_DATA+0x8))) //Address space for MSG cidx 

//===========================================
// Адресация ресурсов в памяти MSG в сторону H2C
//===========================================

#define C2H_MSG_REGION          0x00800 //Address space for MSG memory 
#define C2H_MSG_DATA            0x00fe0 //Address space for MSG data 
#define C2H_PIDX                (*((volatile unsigned long long*)(MSG_MEMORY_BASE + C2H_MSG_DATA)))  //Address space for MSG pidx 
#define C2H_CIDX                (*((volatile unsigned long long*)(MSG_MEMORY_BASE + C2H_MSG_DATA+0x8))) //Address space for MSG cidx 

//===========================================
// Адресация ресурсов DEBUG в памяти MSG
//===========================================

#define H2C_DEBUG               (*((volatile unsigned long long*)(MSG_MEMORY_BASE + C2H_MSG_DATA + 0x10)))  //Debug data H2C 
#define C2H_DEBUG               (*((volatile unsigned long long*)(MSG_MEMORY_BASE + C2H_MSG_DATA + 0x18)))  //Debug data C2H 


//===========================================
// Биты конфигурационного регистра GPC
//===========================================

#define     START_REQUEST           (1<<0)
#define     CONTINUE_REQUEST        (1<<1) 

//===========================================
// Биты статуса GPC
//===========================================

#define     STATUS_IDLE             0
#define     STATUS_READY            1
#define     PIDX_CHANGED            7
#define     INIT                    0x0
#define     BOOT                    0x1
#define     BOOT_OK                 0x2
#define     BOOT_ERR                0x3
#define     READY                   0x4
#define     BUSY                    0x5


//===========================================
// Служебные структуры
//===========================================

union uint64 {
    uint64_t  u64;
    uint32_t  u32[2];
    uint16_t  u16[4];   
    uint8_t   u8[8];
    double    d64;
    float     f32[2];
};


//===========================================
// Генератор случайных чисел
//===========================================


static unsigned long long seed {0x872d30b1eab1cd6e}; //   /* Seed, choose whatever you like! */
static unsigned long long rand_single()
{
    seed = 828090353856353729 * seed + 2309503295186496403;
    return seed;
}


//===========================================
// Функции для обмена данными с HOST
//===========================================

void                            axi_wr_int8  (unsigned long long offs, unsigned char val);
unsigned char                   axi_rd_int8  (unsigned long long offs);
void                            axi_wr_int16 (unsigned long long offs, unsigned short int val);
unsigned short int              axi_rd_int16 (unsigned long long offs);
void                            axi_wr_int32 (unsigned long long offs, unsigned int val);
unsigned int                    axi_rd_int32 (unsigned long long offs);
void                            axi_rd_int64 (unsigned long long offs, void *buf);
void                            axi_wr_int64 (unsigned long long offs, void *buf);
unsigned long long              axi_rd_int64 (unsigned int offs);
void                            axi_wr_int64 (unsigned long long offs, unsigned long long buf);
void                            set_gpc_state(unsigned char val);
unsigned long long              gpc_config   ();
bool                            gpc_start    ();
void                            handler_irq  (unsigned long long payload);
unsigned long long              wait_event   ();

#define wait 
/*
({ \
    for (int i;i<1000; i++); \
})
*/


#endif //GPC_H
