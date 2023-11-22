#!/bin/bash

# Проверка аргументов
if [ "$#" -ne 2 ]; then
   echo "Usage: $0 data1.in data1.out"
   exit 1
fi

# Задание команды запуска
run="host/host_main sw-kernel/sw_kernel.rawbinary"

# Запуск программы и получение ответа
$run < "$1" > test/app.out

# Получение кода завершения программы
exit_code=$?

# Сравнение файлов
if cmp -s "test/app.out" "$2"; then
   echo "Тестирование пройдено"
else
   echo "Тестирование провалено"
fi

# Вывод кода завершения программы
echo "Код завершения программы: $exit_code"

