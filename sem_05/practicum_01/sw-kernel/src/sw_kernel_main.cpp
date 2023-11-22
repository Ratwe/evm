#include <stdlib.h>
#include <ctime>
#include <cmath>
#include "lnh64.hxx"
#include "gpc_io_swk.h"
#include "gpc_handlers.h"
#include "iterators.h"
#include "common_struct.h"
#include "compose_keys.hxx"

#define __fast_recall__

extern lnh lnh_core;
volatile unsigned int event_source;

int main(void) {
        /////////////////////////////////////////////////////////
        //                  Main Event Loop
        /////////////////////////////////////////////////////////
        //Leonhard driver structure should be initialised
        lnh_init();
        for (;;) {
                //Wait for event
                event_source = wait_event();
                switch(event_source) {
                /////////////////////////////////////////////
                //  Measure GPN operation frequency
                /////////////////////////////////////////////
                case __event__(update) : update(); break;
                case __event__(select) : select(); break;
                case __event__(update_obj_x) : update_obj_x(); break;
                case __event__(select_obj_x) : select_obj_x(); break;
                case __event__(update_obj_y) : update_obj_y(); break;
                case __event__(select_obj_xy) : select_obj_xy(); break;
                }
                set_gpc_state(READY);
        }
}
    
//-------------------------------------------------------------
//      Вставка ключа и значения в структуру
//-------------------------------------------------------------
 
void update() {
        while(1){
                users::key key=users::key::from_int(mq_receive());
                if (key==-1ull) break;
                users::val val=users::val::from_int(mq_receive());
                // Поля структуры могут записываться явно следующим образом 
                //      auto new_key = users::key{.rec_idx=1,.user=2};
                //      auto new_val = users::val{.role=3,.lst_time=0}
                // Копирование полей в переменные можно выполнить следующим образом:
                //      auto user = key.user;
                //      auto [lst_time,role] = val;
                USERS.ins_async(key,val); //Вставка в таблицу с типизацией uint64_t
        } 
}
 
void update_obj_x() {
        while(1){
                objects_x::key key=objects_x::key::from_int(mq_receive());
                if (key==-1ull) break;
                objects_x::val val=objects_x::val::from_int(mq_receive());
                OBJECTS_X.ins_async(key,val); //Вставка в таблицу с типизацией uint64_t
        } 
}

void update_obj_y() {
        while(1){
                objects_y::key key=objects_y::key::from_int(mq_receive());
                if (key==-1ull) break;
                objects_y::val val=objects_y::val::from_int(mq_receive());
                OBJECTS_Y.ins_async(key,val); //Вставка в таблицу с типизацией uint64_t
        } 
}


//-------------------------------------------------------------
//      Передать все роли пользователя и время доступа 
//-------------------------------------------------------------

void select() {
        while(1){
                uint32_t quser = mq_receive(); // gpc64_inst->mq_send(stoi(match_query1[4])); //пользователь
                if (quser==-1) break;
                uint32_t qtime = mq_receive(); // gpc64_inst->mq_send(stoi(match_query1[6])); //время доступа
                //Найдем все роли пользователя и последнее время доступа:
                // Результаты поиска могут быть доступны следующим образом:
                //      auto user = USERS.search(users::key{.idx=1,.user=2}).key().user;
                //      auto role = USERS.search(users::key{.idx=3,.user=4}).value().role;

                //Вариант 1 - обход записей пользователя явным образом
                auto crole = USERS.nsm(users::key{.idx=users::idx_min,.user=quser});
                while (crole && crole.key().user==quser) {
                       if (crole.value().time>qtime) mq_send(crole.value());
                       crole = USERS.nsm(crole.key());
                }
                
                //Вариант 2 - использование итератора
                // for (auto val : role_range(USERS,quser)) {
                //        if (val.time>qtime) mq_send(val);
                // }
                mq_send(-1ull);
        }
}

void select_obj_x() {
        uint64_t x1, y1, x2, y2;

        while(1){
                uint64_t msg = mq_receive();

                if (msg == -2ull) {
                        mq_send(-22ull);
                        x1 = mq_receive();
                        y1 = mq_receive();
                        x2 = mq_receive();
                        y2 = mq_receive();
                        mq_send(-3ull);
                } else if (msg == -4ull) {
                        //Вариант 1 - обход записей явным образом
                        auto qidx = OBJECTS_X.nsm(objects_x::key{.x_coord=objects_x::x_coord_max});
                        while (qidx) {
                                mq_send(qidx.key());
                                mq_send(qidx.value());
                                qidx = OBJECTS_X.nsm(qidx.key());
                        }
                        mq_send(-1ull);
                }
        }
}

void select_obj_xy() {
        uint64_t x1, y1, x2, y2;

        while(1){
                uint64_t msg = mq_receive();

                if (msg == -2ull) {
                        mq_send(-22ull);
                        x1 = mq_receive();
                        y1 = mq_receive();
                        x2 = mq_receive();
                        y2 = mq_receive();
                        mq_send(-3ull);
                } else if (msg == -4ull) {
                        //Вариант 1 - обход записей явным образом
                        auto q_obj_x = OBJECTS_X.nsm(objects_x::key{.x_coord=objects_x::x_coord_max});
                        uint64_t qidx = q_obj_x.value(); 
                        auto q_obj_y = OBJECTS_Y.search(objects_y::key{.idx=qidx});

                        while (1)
                        {
                                if (q_obj_x.key() >= std::min(x1, x2) && q_obj_x.key() <= std::max(x1, x2))
                                        if (q_obj_y.value() >= std::min(x1, x2) && q_obj_y.value() <= std::max(y1, y2))
                                        {
                                                mq_send(qidx);
                                                mq_send(q_obj_x.key());
                                                mq_send(q_obj_y.value());
                                        }
                                
                                q_obj_x = OBJECTS_X.nsm(q_obj_x.key());

                                if (!q_obj_x) break;
                                qidx = q_obj_x.value(); 
                                q_obj_y = OBJECTS_Y.search(objects_y::key{.idx=qidx});
                        }

                        mq_send(-1ull);
                }
        }
}
