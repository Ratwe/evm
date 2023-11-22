#include <iostream>
#include <iterator>
#include <string>
#include <regex>
#include <sstream>
#include <fstream>
#include <ctime>
#include "host_main.h"

using namespace std;

#define TEST_USER_COUNT 1000
#define TEST_ROLE_COUNT 1000
#define TEST_IDX_COUNT 20

int main(int argc, char** argv)
{
	ofstream log("prac1.log"); //поток вывода сообщений
	unsigned long long offs=0ull;
	gpc *gpc64_inst; //указатель на класс gpc
	unsigned long long x1, x2, y1, y2, mode;

	//Инициализация gpc
	if (argc<2) {
		log<<"Использование: host_main <путь к файлу rawbinary>"<<endl;
		return -1;
	}

	//Захват ядра gpc и запись sw_kernel
	gpc64_inst = new gpc();
	log<<"Открывается доступ к "<<gpc64_inst->gpc_dev_path<<endl;
	if (gpc64_inst->load_swk(argv[1])==0) {
		log<<"Программное ядро загружено из файла "<<argv[1]<<endl;
	}
	else {
		log<<"Ошибка загрузки sw_kernel файла << argv[1]"<<endl;
		return -1;
	}

	cout << "Введите 0, чтобы программа работала со структурой пользователей, а не со структурой, заданной по варианту" << endl;
	cin >> mode;

	if (mode == 0) {
		//Инициализация таблицы для вложенного запроса
		gpc64_inst->start(__event__(update)); //обработчик вставки 

		regex select_regex_query("select +(.*?) +from +(.*?) +where +(.*?)=(.*?) +and +(.*?)>(.*);", //запрос
            std::regex_constants::ECMAScript | std::regex_constants::icase);

		//1-й вариант: пересылка коротких сообщений
	 	for (uint32_t user=0;user<TEST_USER_COUNT;user++) {
			for (uint32_t idx=0;idx<TEST_ROLE_COUNT;idx++,offs+=2) {
				gpc64_inst->mq_send(users::key{.idx=idx,.user=user}); //запись о роли #idx
				gpc64_inst->mq_send(users::val{.role=idx,.time=time_t(0)}); //роль и время доступа
			}
		}

		//2-й вариант: блочная передача
		unsigned long long *buf = (unsigned long long*)malloc(sizeof(unsigned long long)*TEST_USER_COUNT*TEST_ROLE_COUNT*2);
		for (uint32_t user=0,offs=0;user<TEST_USER_COUNT;user++) {
			for (uint32_t idx=0;idx<TEST_ROLE_COUNT;idx++,offs+=2) {
				buf[offs]=users::key{.idx=idx,.user=user};
				buf[offs+1]=users::val{.role=idx,.time=time_t(idx*3600)};
			}
		}
		auto send_buf_th = gpc64_inst->mq_send(sizeof(unsigned long long)*TEST_USER_COUNT*TEST_ROLE_COUNT*2,(char*)buf);
		send_buf_th->join();
		free(buf);
		//Терминальный символ
		gpc64_inst->mq_send(-1ull);

		gpc64_inst->start(__event__(select)); //обработчик запроса поиска 
		while(1) {
			string query1;
			//разбор полей запроса
			smatch match_query1;
			getline(cin, query1);
			log<<"Введен запрос: "<<query1<<endl;
			if (!query1.compare("exit")) {
				gpc64_inst->mq_send(-1ull);
				break;
			}
			if (regex_match (query1, match_query1, select_regex_query) && 
				match_query1[3]=="user" && 
				match_query1[5] == "time") {
				//match_query1[1] - возвращаемое поле запроса
				//match_query1[2] - номер структуры запроса 
				//match_query1[3] - поле поиска 1
				//match_query1[4] - значение поля поиска 1
				//match_query1[5] - поле поиска 2
				//match_query1[6] - значение поля поиска 2
				log << "Запрос принят в обработку." << endl;
				log << "Поиск ролей пользователя " << match_query1[4] << "и time > " << time_t(stoi(match_query1[6])) << endl;
				gpc64_inst->mq_send(stoi(match_query1[4])); //пользователь
				gpc64_inst->mq_send(stoi(match_query1[6])); //время доступа
				while (1) {
					uint64_t result = gpc64_inst->mq_receive();
					if (result!=-1ull) {
						cout << "Роль: " << users::val::from_int(result).role << " - ";
						cout << "Время доступа: " << users::val::from_int(result).time << endl;
					} else {
						break;
					}
				}
			} else {
				log << "Ошибка в запросе!" << endl;
			}
		}
	} else {
		log << "Инициализация начальных значений...";

		//Инициализация начальных значений 
		gpc64_inst->start(__event__(update_obj_x)); //обработчик вставки 


		//1-й вариант: пересылка коротких сообщений
	 	for (uint64_t idx=0; idx<TEST_IDX_COUNT; idx++) {
				uint64_t x_coord = idx; 
				log << "Запись: x = " << x_coord << " idx = " << idx << endl;
				gpc64_inst->mq_send(objects_x::key{.x_coord=x_coord}); //запись ключа x_coord
				gpc64_inst->mq_send(objects_x::val{.idx=idx}); //запись значения idx
			}

		//Терминальный символ
		gpc64_inst->mq_send(-1ull);

		gpc64_inst->start(__event__(update_obj_y)); //обработчик вставки 

		//1-й вариант: пересылка коротких сообщений
	 	for (uint64_t idx=0; idx<TEST_IDX_COUNT; idx++) {
				uint64_t y_coord = (idx * 17) % 10; 
				log << "Запись: y = " << y_coord << " idx = " << idx << endl;
				gpc64_inst->mq_send(objects_y::key{.idx=idx}); //запись ключа idx
				gpc64_inst->mq_send(objects_y::val{.y_coord=y_coord}); //запись значения y_coord
			}

		//Терминальный символ
		gpc64_inst->mq_send(-1ull);

		log << " завершена." << endl;

		gpc64_inst->start(__event__(select_obj_xy)); //обработчик запроса поиска 
		while(1) {
			cout << "Введите вершины прямоугольника (x1, y1, x2, y2): ";
			cin >> x1 >> y1 >> x2 >> y2;
			//разбор полей запроса
			log << "Введены вершины: " << x1 << y1 << x2 << y2 << endl;
			log << "Запрос принят в обработку." << endl;

			gpc64_inst->mq_send(-2ull);

			log << "mq_send(-2ull) completed" << endl;

			uint64_t result = gpc64_inst->mq_receive();
			log << "result = " << result << endl;

			gpc64_inst->mq_send(x1);
			gpc64_inst->mq_send(y1);
			gpc64_inst->mq_send(x2);
			gpc64_inst->mq_send(y2);

			result = gpc64_inst->mq_receive();
			if (result == -3ull)
				log << "Вершины переданы в обработку." << endl; 

			gpc64_inst->mq_send(-4ull);

			cout << "Вершины, находящиеся внутри заданного прямоугольника:" << endl;

			while (1) {
			 	uint64_t key = gpc64_inst->mq_receive();
			 	if (key != -1ull) {
					uint64_t val_x = gpc64_inst->mq_receive();
					uint64_t val_y = gpc64_inst->mq_receive();
			 		
			 		log << "индекс idx: " << key << endl;
					log << "(x, y): " << val_x << ", " << val_y << endl;
					cout << "(x, y): " << val_x << ", " << val_y << endl;
			 	} else {
			 		break;
			 	}
			}

			// log << "Поиск ролей пользователя " << match_query1[4] << "и time > " << time_t(stoi(match_query1[6])) << endl;
			// gpc64_inst->mq_send(stoi(match_query1[4])); //пользователь
			// gpc64_inst->mq_send(stoi(match_query1[6])); //время доступа
			// while (1) {
			// 	uint64_t result = gpc64_inst->mq_receive();
			// 	if (result!=-1ull) {
			// 		cout << "Роль: " << users::val::from_int(result).role << " - ";
			// 		cout << "Время доступа: " << users::val::from_int(result).time << endl;
			// 	} else {
			// 		break;
			// 	}
			// }
			break;
		}
	}

	log << "Выход!" << endl;
	return 0;
}

