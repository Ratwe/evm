/*
 * lnh64.h
 *
 * sw_kernel library
 *
 *  Created on: May 3, 2021
 *      Author: A.Popov
 */

#ifndef HDR_OPERATIONS_H_
#define HDR_OPERATIONS_H_

#include "gpc_swk.h"

//========================================
// Основные настроечные параметры Leonhard
//========================================
//Компилируется для riscv64
#define __riscv64__
//LNH revision
#define LNH64
//#define LNH128
//#define LNH256

#ifdef LNH64
    #define LNH_WEIGHT              2
    #define DATA_WIDTH_FACTOR       2
#endif
#define lnh_n                       8  //Razryadnost' koda vyborki i kolichestvo PE1 v Kataloge
#define lnh_log_n                   3  //log(Razryadnost' koda vyborki i kolichestvo PE1 v Kataloge)
#define lnh_key_w                   64 //Razryadnost' klyucha i registrov dlya ikh khraneniya
#define lnh_val_w                   64 //Razryadnost' polya dannykh, assotsiirovannykh s klyuchom
#define lnh_s                       3  //Log(Kolichestvo struktur v protsessore s uchetom nulevoj (pustoj struktury))
#define lnh_s_count                 8  //Kolichestvo struktur v protsessore s uchetom nulevoj (pustoj struktury)
#define lnh_l                       7  //Razmer linejki v pamyati struktur
#define lnh_tree_num                8  //Kolichestvo urovnej dereva (kol-vo elementov v pamjati = l*(n^tree_num))
#define lnh_o_k                     2
#define pe1_w                       (lnh_key_w + lnh_key_w + lnh_s + lnh_tree_num*lnh_log_n + 3);
#define max_keys_count              7*8*8*8*8*8*8*8*8
#define mailbox_weight              3
#define                             lnh_cmd 3*lnh_s
#define lnh_mbox_key                ((0*mailbox_weight)+7+(3*lnh_s))
#define lnh_mbox_val                ((1*mailbox_weight)+7+(3*lnh_s))
#define lnh_mbox_st                 ((2*mailbox_weight)+7+(3*lnh_s))


//==================================
// Коды синхронных операций Leonhard
//==================================

#define NOP                         0x0
#define DEL                         0x1
#define INS                         0x2
#define FST                         0x3
#define LST                         0x4
#define SEARCH                      0x5
#define SQUIZ                       0x6
#define NUM                         0x7
#define POR                         0x8
#define PAND                        0x9
#define PNOT                        0xA
#define LSEQ                        0xB
#define LS                          0xC
#define GREQ                        0xD
#define GR                          0xE
#define DELSTR                      0xF
#define NEXT                        0x10
#define PREV                        0x11
#define NSM                         0x12
#define NGR                         0x13
#define GRLS                        0x14

//===================================================================
// Коды асинхронных операций Leonhard с записью в очередь результатов
//===================================================================

#define NOPQ                        0x20
#define DELQ                        0x21
#define INSQ                        0x22
#define FSTQ                        0x23
#define LSTQ                        0x24
#define SEARCHQ                     0x25
#define SQUIZQ                      0x26
#define NUMQ                        0x27
#define PORQ                        0x28
#define PANDQ                       0x29
#define PNOTQ                       0x2A
#define LSEQQ                       0x2B
#define LSQ                         0x2C
#define GREQQ                       0x2D
#define GRQ                         0x2E
#define DELSTRQ                     0x2F
#define NEXTQ                       0x30
#define PREVQ                       0x31
#define NSMQ                        0x32
#define NGRQ                        0x33
#define GRLSQ                       0x34

//==================================================================
// Коды асинхронных операций Leonhard со сбросом очереди результатов
//==================================================================

#define NOPR                        0x40
#define DELR                        0x41
#define INSR                        0x42
#define FSTR                        0x43
#define LSTR                        0x44
#define SEARCHR                     0x45
#define SQUIZR                      0x46
#define NUMR                        0x47
#define PORR                        0x48
#define PANDR                       0x49
#define PNOTR                       0x4A
#define LSEQR                       0x4B
#define LSR                         0x4C
#define GREQR                       0x4D
#define GRR                         0x4E
#define DELSTRR                     0x4F
#define NEXTR                       0x50
#define PREVR                       0x51
#define NSMR                        0x52
#define NGRR                        0x53
#define GRLSR                       0x54

//==============================================================================
// Коды асинхронных операций Leonhard со сбросом и записью в очередь результатов
//==============================================================================
#define NOPQR                       0x60
#define DELQR                       0x61
#define INSQR                       0x62
#define FSTQR                       0x63
#define LSTQR                       0x64
#define SEARCHQR                    0x65
#define SQUIZQR                     0x66
#define NUMQR                       0x67
#define PORQR                       0x68
#define PANDQR                      0x69
#define PNOTQR                      0x6A
#define LSEQQR                      0x6B
#define LSQR                        0x6C
#define GREQQR                      0x6D
#define GRQR                        0x6E
#define DELSTRQR                    0x6F
#define NEXTQR                      0x70
#define PREVQR                      0x71
#define NSMQR                       0x72
#define NGRQR                       0x73
#define GRLSQR                      0x74

//=======================================
// Регистры Leonhard, доступные на запись
//=======================================


#define 	KEY2LNH      		    (*(volatile unsigned long long int*)(AXL4LNH64_BASE + (0<<3)))
#define 	VAL2LNH      	    	(*(volatile unsigned long long int*)(AXL4LNH64_BASE + (1<<3)))
#define 	CMD2LNH                 (*(volatile unsigned long long int*)(AXL4LNH64_BASE + (2<<3)))
#define 	LNH_CNTL                (*(volatile unsigned long long int*)(AXL4LNH64_BASE + (4<<3)))


//=======================================
// Регистры Leonhard, доступные на чтение
//=======================================

#define 	LNH2KEY 	    	    (*(volatile unsigned long long int*)(AXL4LNH64_BASE + (0<<3)))
#define 	LNH2VAL 	    	    (*(volatile unsigned long long int*)(AXL4LNH64_BASE + (1<<3)))
#define 	LNH_STATE           	(*(volatile unsigned long long int*)(AXL4LNH64_BASE + (2<<3)))
#define     CARDINALITY             (*(volatile unsigned long long int*)(AXL4LNH64_BASE + (3<<3)))
#define     LNH2KEYQ                (*(volatile unsigned long long int*)(AXL4LNH64_BASE + (5<<3)))
#define     LNH2VALQ                (*(volatile unsigned long long int*)(AXL4LNH64_BASE + (6<<3)))
#define     LNH_STATEQ              (*(volatile unsigned long long int*)(AXL4LNH64_BASE + (7<<3)))
#define 	TSC                     (*(volatile unsigned long long int*)(AXL4LNH64_BASE + (8<<3)))
#define 	CSC                     (*(volatile unsigned long long int*)(AXL4LNH64_BASE + (9<<3)))
#define 	DBG_A                   (*(volatile unsigned long long int*)(AXL4LNH64_BASE + (10<<3)))
#define 	DBG_B                   (*(volatile unsigned long long int*)(AXL4LNH64_BASE + (11<<3)))
#define 	DBG_C                   (*(volatile unsigned long long int*)(AXL4LNH64_BASE + (12<<3)))
#define 	DBG_D                   (*(volatile unsigned long long int*)(AXL4LNH64_BASE + (13<<3)))


//============================================
// Регистры асинхроного взаимодействия Mailbox
//============================================
#define     MBOX_FAKE_REG           (*(volatile unsigned long long int*)(AXL4LNH64_BASE + (23<<3)))
//Mailbox IO data structures
#define     mr0                     0
#define     mr1                     1
#define     mr2                     2
#define     mr3                     3
#define     mr4                     4
#define     mr5                     5
#define     mr6                     6
#define     mr7                     7
// mr7 is used as FAKE register
#define     mrf                     7
const unsigned long long int   MBOX[8] =  {
                                    (*(volatile unsigned long long int*)(AXL4LNH64_BASE + (16<<3))),
                                    (*(volatile unsigned long long int*)(AXL4LNH64_BASE + (17<<3))),
                                    (*(volatile unsigned long long int*)(AXL4LNH64_BASE + (18<<3))),
                                    (*(volatile unsigned long long int*)(AXL4LNH64_BASE + (19<<3))),
                                    (*(volatile unsigned long long int*)(AXL4LNH64_BASE + (20<<3))),
                                    (*(volatile unsigned long long int*)(AXL4LNH64_BASE + (21<<3))),
                                    (*(volatile unsigned long long int*)(AXL4LNH64_BASE + (22<<3))),
                                    (*(volatile unsigned long long int*)(AXL4LNH64_BASE + (23<<3)))
};

//===============================   
// Биты регистра статуса Leonhard
//===============================

#define 	SPU_READY_FLAG          0
#define 	SPU_ERROR_FLAG          1
#define 	SPU_ERROR_Q_FLAG        2
#define 	DDR_Q_OVF_FLAG          3
#define 	DDR_TEST_SUCC_FLAG      4
#define     SPU_ALL_DONE            9
#define 	AXI2LNH_Q_EMP_FLAG      16
#define 	AXI2LNH_Q_FULL_FLAG     17
#define 	AXI2LNH_Q_AEMP_FLAG     18
#define 	AXI2LNH_Q_AFULL_FLAG    19
#define     LNH2AXI_Q_EMP_FLAG      20
#define     LNH2AXI_Q_FULL_FLAG     21
#define     LNH2AXI_Q_AEMP_FLAG     22
#define     LNH2AXI_Q_AFULL_FLAG    23
#define     LNH_DATA_PARTITION      48
#define     LNH_INDEX_PARTITION     51
#define     LNH_INDEX_REGION        54
const int   MBOX_VFLAG[8]           = {32,33,34,35,36,37,38,39};


//==================================
// Биты регистра управления Leonhard
//==================================

#define 	ALLOW_LNH_FLAG          0
#define 	SUSPEND_Q_FLAG          1
#define 	LSM_DMA_FLAG            2
#define 	LCM_DMA_FLAG            3
#define 	ENABLE_TSC_FLAG         4
#define 	ENABLE_READY_INT        5

#define 	RESET_MAILBOX_0         32
#define 	RESET_MAILBOX_1         33
#define 	RESET_MAILBOX_2         34
#define 	RESET_MAILBOX_3         35
#define 	RESET_MAILBOX_4         36
#define 	RESET_MAILBOX_5         37
#define 	RESET_MAILBOX_6         38
#define 	RESET_MAILBOX_7         39
#define 	RESET_SPU               48
#define 	RESET_ALL_QUEUES        49
#define 	RESET_LNH2AXI_QUEUE     50
#define 	RESET_AXI2LNH_QUEUE     51
#define 	RESET_TSC               52
#define 	RESET_RISCV             53 

#define     RESET_GPN				(	(1ull	<<	(RESET_SPU		     ))|\
										(1ull	<<	(RESET_LNH2AXI_QUEUE ))|\
										(1ull	<<	(RESET_TSC			 )))

//AXI2LNH QUEUE maximum credits
#define AXI2LNH_QUEUE_CREDITS_MAX   255
//Enable Leonhard jardware reset by RiscV
//#define LNH_HW_RESET_ENABLE

//===================================
// Типа данных и структуры для работы
//===================================

typedef struct //Data structure for the lnh operation result
lnh_result {
        uint64_t                         status;	//operation status
        uint64_t                         key;		//result key
        uint64_t                         value;  	//result value
} lnh_result;

typedef struct //Data structure for the lnh memory access
lnh_partition {
        uint64_t	                     data_partition;
        uint64_t                         index_partition;
        uint64_t                         index_region;
} lnh_partition;

// Define Leonhard x64 descriptor
typedef struct 
lnh {
        // AXI3LNH queue credits
        uint32_t                         axi2lnh_queue_credits;
        uint32_t                         axi2lnh_queue_state;
        // AXI I/O 32 bit buffer
        uint32_t                         buf32;
        uint32_t                         *buf32_ptr;
        // AXI I/O 64 bit buffer
        uint64_t                         buf64;
        uint64_t                         *buf64_ptr;
        //Buffers for IO
        lnh_result                  result;
        //LNH configuration
        lnh_partition               partition;
} lnh;


#ifdef      DEFINE_LNH_DRIVER
#define     EXTERN                  /* nothing */
#else
#define     EXTERN                  extern
#endif                              /* DEFINE_LNH_DRIVER */
EXTERN      lnh                     lnh_core;


//========================================================
// Проверка кредитов FIFO буфера и ожидание его готовности
//========================================================

#define lnh_axi2lnh_queue_credits_check ({ \
    if (lnh_core.axi2lnh_queue_credits==0) { \
        while ( (LNH_STATE & ( 1ull<<AXI2LNH_Q_AFULL_FLAG  ) )!=0); \
        lnh_core.axi2lnh_queue_credits = AXI2LNH_QUEUE_CREDITS_MAX; \
    } \
    lnh_core.axi2lnh_queue_credits--; \
})


//==================================
// Функции для работы с Leonhard API
//==================================

void                            lnh_hw_reset();
void                            lnh_sw_reset();
void                            lnh_init();

//================================
// Сервисные функции  Leonhard API
//================================

uint32_t                        float2uint(float value);
float                           uint2float(uint32_t value);
uint64_t                        double2ull(double value);
double                          ull2double(uint64_t value);
void                            lnh_sync();
void                            lnh_syncm(int mbr);

//================================
// Синхронные функции Leonhard API
//================================

bool                            lnh_ins_sync(uint64_t str, uint64_t key, uint64_t value);
bool                            lnh_del_sync(uint64_t str, uint64_t key);
uint32_t                        lnh_get_num(uint64_t str);
bool                            lnh_del_str_sync(uint64_t str);
bool                            lnh_sq_sync(uint64_t str);
bool                            lnh_or_sync(uint64_t A, uint64_t B, uint64_t R);
bool                            lnh_and_sync(uint64_t A, uint64_t B, uint64_t R);
bool                            lnh_not_sync(uint64_t A, uint64_t B, uint64_t R);
bool                            lnh_lseq_sync(uint64_t key, uint64_t A, uint64_t R);
bool                            lnh_ls_sync(uint64_t key, uint64_t A, uint64_t R);
bool                            lnh_greq_sync(uint64_t key, uint64_t A, uint64_t R);
bool                            lnh_gr_sync(uint64_t key, uint64_t A, uint64_t R);
bool                            lnh_grls_sync(uint64_t key_ls, uint64_t key_gr, uint64_t A, uint64_t R);
bool                            lnh_search(uint64_t str, uint64_t key);
bool                            lnh_next(uint64_t str, uint64_t key);
bool                            lnh_prev(uint64_t str, uint64_t key);
bool                            lnh_nsm(uint64_t str, uint64_t key);
bool                            lnh_ngr(uint64_t str, uint64_t key);
bool                            lnh_nsm_signed(uint64_t str, long long int key);
bool                            lnh_ngr_signed(uint64_t str, long long int key);
bool                            lnh_get_first(uint64_t str);
bool                            lnh_get_last(uint64_t str);
bool                            lnh_get_first_signed(uint64_t str);
bool                            lnh_get_last_signed(uint64_t str);


//================================================================
// Синхронные функции Leonhard API с записью в очередь результатов
//================================================================

bool                            lnh_ins_syncq(uint64_t str, uint64_t key, uint64_t value);
bool                            lnh_del_syncq(uint64_t str, uint64_t key);
uint32_t                        lnh_get_numq(uint64_t str);
bool                            lnh_del_str_syncq(uint64_t str);
bool                            lnh_sq_syncq(uint64_t str);
bool                            lnh_or_syncq(uint64_t A, uint64_t B, uint64_t R);
bool                            lnh_and_syncq(uint64_t A, uint64_t B, uint64_t R);
bool                            lnh_not_syncq(uint64_t A, uint64_t B, uint64_t R);
bool                            lnh_lseq_syncq(uint64_t key, uint64_t A, uint64_t R);
bool                            lnh_ls_syncq(uint64_t key, uint64_t A, uint64_t R);
bool                            lnh_greq_syncq(uint64_t key, uint64_t A, uint64_t R);
bool                            lnh_gr_syncq(uint64_t key, uint64_t A, uint64_t R);
bool                            lnh_grls_syncq(uint64_t key_ls, uint64_t key_gr, uint64_t A, uint64_t R);
bool                            lnh_searchq(uint64_t str, uint64_t key);
bool                            lnh_nextq(uint64_t str, uint64_t key);
bool                            lnh_prevq(uint64_t str, uint64_t key);
bool                            lnh_nsmq(uint64_t str, uint64_t key);
bool                            lnh_ngrq(uint64_t str, uint64_t key);
bool                            lnh_get_firstq(uint64_t str);
bool                            lnh_get_lastq(uint64_t str);
bool                            lnh_get_q();


//=================================================================
// Асинхронные функции Leonhard API с записью в асинхронный Mailbox
//=================================================================

bool                            lnh_ins_syncm(int st_mreg, uint64_t str, uint64_t key, uint64_t value);
bool                            lnh_del_syncm(int st_mreg, uint64_t str, uint64_t key);
uint32_t                        lnh_get_numm(uint64_t str);
bool                            lnh_del_str_syncm(int st_mreg, uint64_t str);
bool                            lnh_sq_syncm(int st_mreg, uint64_t str);
bool                            lnh_or_syncm(int st_mreg, uint64_t A, uint64_t B, uint64_t R);
bool                            lnh_and_syncm(int st_mreg, uint64_t A, uint64_t B, uint64_t R);
bool                            lnh_not_syncm(int st_mreg, uint64_t A, uint64_t B, uint64_t R);
bool                            lnh_lseq_syncm(int st_mreg, uint64_t key, uint64_t A, uint64_t R);
bool                            lnh_ls_syncm(int st_mreg, uint64_t key, uint64_t A, uint64_t R);
bool                            lnh_greq_syncm(int st_mreg, uint64_t key, uint64_t A, uint64_t R);
bool                            lnh_gr_syncm(int st_mreg, uint64_t key, uint64_t A, uint64_t R);
bool                            lnh_grls_syncm(int st_mreg, uint64_t key_ls, uint64_t key_gr, uint64_t A, uint64_t R);
bool                            lnh_searchm(int key_mreg, int val_mreg, int st_mreg, uint64_t str, uint64_t key);
bool                            lnh_nextm(int key_mreg, int val_mreg, int st_mreg, uint64_t str, uint64_t key);
bool                            lnh_prevm(int key_mreg, int val_mreg, int st_mreg, uint64_t str, uint64_t key);
bool                            lnh_nsmm(int key_mreg, int val_mreg, int st_mreg, uint64_t str, uint64_t key);
bool                            lnh_ngrm(int key_mreg, int val_mreg, int st_mreg, uint64_t str, uint64_t key);
bool                            lnh_get_firstm(int key_mreg, int val_mreg, int st_mreg, uint64_t str);
bool                            lnh_get_lastm(int key_mreg, int val_mreg, int st_mreg, uint64_t str);
uint64_t                        lnh_get_m(int mreg);


//========================================================
// Асинхронные функции Leonhard API без записи результатов
//========================================================
bool                            lnh_ins_async(uint64_t str, uint64_t key, uint64_t value);
bool                            lnh_del_async(uint64_t str, uint64_t key);
bool                            lnh_del_str_async(uint64_t str);
bool                            lnh_sq_async(uint64_t str);
bool                            lnh_or_async(uint64_t A, uint64_t B, uint64_t R);
bool                            lnh_and_async(uint64_t A, uint64_t B, uint64_t R);
bool                            lnh_not_async(uint64_t A, uint64_t B, uint64_t R);
bool                            lnh_lseq_async(uint64_t key, uint64_t A, uint64_t R);
bool                            lnh_ls_async(uint64_t key, uint64_t A, uint64_t R);
bool                            lnh_greq_async(uint64_t key, uint64_t A, uint64_t R);
bool                            lnh_gr_async(uint64_t key, uint64_t A, uint64_t R);
bool                            lnh_grls_async(uint64_t key_ls, uint64_t key_gr, uint64_t A, uint64_t R);
    

#endif /* HDR_OPERATIONS_H_ */
