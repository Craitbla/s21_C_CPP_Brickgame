FROM alpine:3.20 AS builder

RUN apk --no-cache --upgrade add \
    cmake \
    make \
    gcc \
    g++ \
    musl-dev \
    gtest-dev \
    gtest \
    valgrind \
    # Установка Qt6 и зависимостей
    qt6-qtbase-dev \
    qt6-qttools-dev \
    qt6-qtdeclarative-dev \
    qt6-qt5compat-dev \
    # Графические зависимости для Qt
    mesa-dev \
    libx11-dev \
    libxcb-dev \
    # Установка subunit
    subunit-dev \
    # Настройка переменных окружения Qt
    && echo "export Qt6_DIR=/usr/lib/cmake/Qt6" >> /etc/profile.d/qt6.sh

# Указываем CMake путь к Qt6
ENV Qt6_DIR=/usr/lib/cmake/Qt6
ENV CMAKE_PREFIX_PATH=/usr

WORKDIR /testing
COPY ./ ./

# RUN mkdir -p tests/build \
# && gcc tests/*.cc -lgtest -lgtest_main -lm -lstdc++ -g -Wall -Wextra -Werror -std=c++17 -o 