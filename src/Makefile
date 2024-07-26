# ПОДГОТОВКА ПЕРЕМЕННЫХ

NAME = AlgoTrading

# Определение операционной системы, имена программ и параметров
CC=g++
CFLAGS=-Wall -Wextra -Werror -std=c++17 -Wno-nullability-completeness
OPEN = open

# Пути до используемых папок
SRC_DIR = .
RUN_DIR = $(SRC_DIR)/bin
OBJ_DIR = $(SRC_DIR)/build
QT_DIR = $(SRC_DIR)/qt

# Указываем список каталогов, в которых make выполняет поиск
vpath %.c $(SRC_DIR)
vpath %.o $(OBJ_DIR)

# ОБЯЗАТЕЛЬНЫЕ ЦЕЛИ СБОРКИ

all: clean install open

# Цель сборки и установки приложения
install: $(RUN_DIR) $(OBJ_DIR)
	cd $(RUN_DIR) && qmake ../$(QT_DIR)/$(NAME).pro && make

# Цель удаления приложения
uninstall:
	rm -rf $(RUN_DIR)

# Удаление временных файлов и папок
clean:
	rm -rf $(RUN_DIR)
	rm -rf $(OBJ_DIR)
	rm -f *.a
	rm -f *.o
	rm -f *.gz

# Сборка документации
dvi:
	doxygen
	open html/index.html

# Упаковка всех исходников в архив dist.tar.gz
dist:
	mkdir $(RUN_DIR)
	cp -r ./* main.cc Makefile bin
	tar -cvzf dist.tar.gz $(RUN_DIR)
	rm -rf $(RUN_DIR)

# Автоматическое форматирование по Google-style
format:
	cp ../materials/linters/.clang-format ./
	clang-format -i $(shell find . -name "*.cc" -o -name "*.h")
	rm .clang-format

# ПРОЧИЕ ЦЕЛИ СБОРКИ

# Открытие установленного приложения
open:
	$(OPEN) bin/$(NAME).app

# Пересборка проекта (при этом не изменившиеся модули не пересобираются)
rebuild:
	make -B

# Создание папки для запуска приложения
$(RUN_DIR):
	@mkdir -p $(RUN_DIR)

# Создание временной папки для .o-файлов
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/$(TEST_DIR)
	@mkdir -p $(OBJ_DIR)/$(SRC_DIR)

.PHONY: all install uninstall clean dvi dist test gcov_report style format cppcheck valgrind init_docker docker open rebuild
