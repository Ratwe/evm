
#ifndef GITERS_H_
#define GITERS_H_

#include <cassert>
#include "common_struct.h"

//Итератор для обхода всех ролей пользователя

struct role_sentinel {};
struct role_iterator {
        users USERS;
        users::vertex_t user; //Исходная запись о роли пользователя
        Handle<users::key, users::val> crole; //курсор (роль)
        [[gnu::always_inline]] role_iterator(users USERS, users::vertex_t user, Handle<users::key, users::val> val) : USERS(USERS), user(user), crole(val) {}


        [[gnu::always_inline]] users::val operator*() const {
                return crole.value();
        }


        [[gnu::always_inline]] role_iterator& operator++() { //Обход всегда в обратном порядке
                crole = USERS.nsm(crole.key());
                return *this;
        }


        [[gnu::always_inline]] bool operator==(const role_iterator rhs) {
                assert(user == rhs.crole.key().user); //Сравнение итераторов ролей разных пользователей не имеет смысла
                return ((!crole) || (user != crole.key().user));
        }

        //Достигнут ли конец
        [[gnu::always_inline]] bool operator==(const role_sentinel rhs) {
                return ((!crole) || (user != crole.key().user));
        }

};
struct role_range {
        users USERS;
        users::vertex_t user;
        [[gnu::always_inline]] role_range(users USERS, users::vertex_t user) : USERS(USERS), user(user) {}
        [[gnu::always_inline]] auto begin() {return role_iterator(USERS, user, USERS.nsm(users::key{.idx = users::idx_min, .user = user}));}
        [[gnu::always_inline]] auto end() {return role_sentinel{};}
};

#endif
