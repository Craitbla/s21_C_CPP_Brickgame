UNAME_S := $(shell uname -s)
export PKG_CONFIG_PATH=/usr/local/Qt-6.6.2/lib/pkgconfig/

FLAGS = -Wall -Wextra -Werror -Wno-unknown-pragmas -Uscroll -lncurses
FLAGS += -fprofile-arcs -ftest-coverage#только здесь

TARGET_APP = brickgame
TARGET_CLI = cli_game
CC = g++
CXXFLAGS = -std=c++17 $(FLAGS)

CXXFLAGS += -fprofile-arcs -ftest-coverage
LDFLAGS += -fprofile-arcs -ftest-coverage

BACK_COMMON_FILE = $(wildcard brick_game/*.c)
BACK_C_FILES = $(wildcard brick_game/tetris/*.c)
BACK_CC_FILES = $(wildcard brick_game/snake/*.cc)

CLI_CC_FILES = $(wildcard gui/cli/*.cc)
QT_CC_FILES = $(wildcard gui/desktop/brickgame/*.cc)

BACK_COMMON_OBJ = $(BACK_COMMON_FILE:.c=.o)
BACK_C_OBJS = $(BACK_C_FILES:.c=.o)
BACK_CC_OBJS = $(BACK_CC_FILES:.cc=.o)

CLI_CC_OBJS = $(CLI_CC_FILES:.cc=.o)
QT_CC_OBJS = $(QT_CC_FILES:.cc=.o)

CLI_MAIN_FILES = $(wildcard main_cli.cc)
QT_MAIN_FILES = $(wildcard main_desktop.cc)

CLI_MAIN_OBJS = $(CLI_MAIN_FILES:.cc=.o)
QT_MAIN_OBJS = $(QT_MAIN_FILES:.cc=.o)

QTDIR ?= /usr/local/Qt-6.6.2

ifeq ($(UNAME_S),Darwin)  # macOS
    RUN_COMMAND = open ./$(TARGET_APP).app
else                      # Linux/Unix
    RUN_COMMAND = ./$(TARGET_APP)
endif

QTPKG_PATH := $(shell find /usr -name Qt6Widgets.pc 2>/dev/null)

ifeq ($(QTPKG_PATH),)
  $(info Qt6Widgets.pc не найден в стандартных путях)
else
  QTPKG_DIR := $(dir $(QTPKG_PATH))
  PKG_CONFIG_PATH := $(QTPKG_DIR)
endif

define get_pkg_config_flags
	PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) pkg-config --cflags $1
endef

define get_pkg_config_libs
	PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) pkg-config --libs $1
endef

QTPKG_CFLAGS := $(shell $(call get_pkg_config_flags,Qt6Widgets)) \
                $(shell $(call get_pkg_config_flags,Qt6Gui)) \
                $(shell $(call get_pkg_config_flags,Qt6Core))

QTPKG_LDFLAGS := $(shell $(call get_pkg_config_libs,Qt6Widgets)) \
                 $(shell $(call get_pkg_config_libs,Qt6Gui)) \
                 $(shell $(call get_pkg_config_libs,Qt6Core))


CLI_FLAGS := -lncurses
APP_FLAGS := -Wl,-rpath,$(QTDIR)/lib

.PHONY: all install_app install_cli uninstall clean run_app run_cli

all: clean install

install: install_app install_cli

uninstall: clean

clean:
	rm -f $(BACK_COMMON_OBJ) $(BACK_C_OBJS) $(BACK_CC_OBJS) $(QT_CC_OBJS) $(CLI_CC_OBJS) *.gcno *.gcda $(TARGET_CLI)
	rm -f main_cli.o main_desktop.o
	rm -rf build
	rm -rf report
	-rm -f tests_cc
	-rm -f res.info
	-rm -f *.gcov
	find . -name "*.gcda" -delete
	find . -name "*.gcno" -delete
	rm -rf coverage_report *.info
	rm -rf *.dSYM $(TETRIS_TEST_CC_TARGET) $(SNAKE_TEST_CC_TARGET)

GCOV_LIBS = -lgcov
LDFLAGS += $(GCOV_LIBS)

install_cli: $(BACK_COMMON_OBJ) $(BACK_C_OBJS) $(BACK_CC_OBJS) $(CLI_CC_OBJS) $(CLI_MAIN_FILES)
	$(CC) $^ -o $(TARGET_CLI) $(CLI_FLAGS) $(LDFLAGS)


ifeq ($(UNAME_S),Darwin)  # macOS
    QMAKE = /usr/local/Qt-6.6.2/bin/qmake6
else                      # Linux/Unix
    QMAKE = /usr/lib/qt6/bin/qmake6
endif

ifeq ($(UNAME_S),Darwin)  # macOS
    RUN_COMMAND = open ./$(TARGET_APP).app
else                      # Linux/Unix
    RUN_COMMAND = ./$(TARGET_APP)
endif

install_app:
	mkdir -p build && cd build && \
	$(QMAKE) ../gui/desktop/brickgame/brickgame.pro && \
	make

run_app:
	./build/brickgame.app/Contents/MacOS/brickgame

run_cli:
	./$(TARGET_CLI)

%.o: %.c
	gcc $(FLAGS) -c $< -o $@ $(QTPKG_CFLAGS) 

%.o: %.cc
	$(CC) $(CXXFLAGS) -c $< -o $@ $(QTPKG_CFLAGS) 

INSTALL_TEST_C = brick_game/tetris/*.c brick_game/*.c -lncurses
INSTALL_TEST_CC = brick_game/snake/*.cc -lncurses

TEST_FLAGS = --coverage -g -O0
TEST_LIBS = -lgtest -lgtest_main -lpthread#возм не нужен -lpthread
ifeq ($(UNAME_S),Linux)
    TEST_LIBS += -lrt -lpthread -lsubunit
endif

TETRIS_TEST_CC_TARGET = tetris_tests_cc
SNAKE_TEST_CC_TARGET = snake_tests_cc

test: tetris_test snake_test

tetris_test: $(BACK_COMMON_OBJ) $(BACK_C_OBJS)
	$(CC) $(FLAGS) $(TEST_FLAGS) $^  tests/tetris_tests.cc -o $(TETRIS_TEST_CC_TARGET) $(TEST_LIBS) $(LDFLAGS)
	./$(TETRIS_TEST_CC_TARGET)

snake_test: $(BACK_COMMON_OBJ) $(BACK_CC_OBJS)
	$(CC) $(FLAGS) $(TEST_FLAGS) $^  tests/snake_tests.cc -o $(SNAKE_TEST_CC_TARGET) $(TEST_LIBS) $(LDFLAGS)
	./$(SNAKE_TEST_CC_TARGET)

INFO_FILE = res.info
REPORT_DIR = report
gcov_report: test
	lcov --base-directory . --directory . --capture --output-file coverage.info --no-external --ignore-errors inconsistent,unused
	lcov --remove coverage.info "*/tests/*" "*/gtest/*" -o filtered.info --ignore-errors inconsistent,unused
	genhtml -o coverage_report filtered.info
	open coverage_report/index.html

dvi:
	latex description.tex

dist:
	tar -czvf archive.tar.gz ./*

uninstall: clean

load:
	@echo "Starting installation in goinfre..."
    
	@if [ ! -d "/opt/goinfre/$(USER)" ]; then \
        echo "Creating goinfre directory..."; \
        mkdir -p /opt/goinfre/$(USER); \
    fi
    
	@if [ ! -d "/opt/goinfre/$(USER)/homebrew" ]; then \
        echo "Cloning Homebrew..."; \
        git clone https://github.com/Homebrew/brew /opt/goinfre/$(USER)/homebrew; \
    else \
        echo "Homebrew already exists in goinfre, skipping clone..."; \
    fi
    
	@echo "Setting up Homebrew environment..."
	@eval "$(/opt/goinfre/$(USER)/homebrew/bin/brew shellenv)"
    
	@echo "Updating Homebrew..."
	@brew update --force --quiet
    
	@echo "Installing packages..."
	@brew install lcov
	@brew install doxygen
	@brew install gotests
	@brew install --cask mactex
    
	@echo "Installation completed successfully!"

style:
	@echo "Checking code style..."
	@clang-format -n -style=Google \
	brick_game/*.c \
	brick_game/snake/*.cc \
	brick_game/tetris/*.c \
	tests/*.cc \
	gui/cli/*.cc \
	gui/desktop/brickgame/*.cc 
	@echo "Applying code style..." 
	@clang-format -i -style=Google \
	brick_game/*.c \
	brick_game/snake/*.cc \
	brick_game/tetris/*.c \
	tests/*.cc \
	gui/cli/*.cc \
	gui/desktop/brickgame/*.cc \

valgrind: test
	CK_FORK=no valgrind --vgdb=no --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./snake_tests_cc ./tetris_tests_cc