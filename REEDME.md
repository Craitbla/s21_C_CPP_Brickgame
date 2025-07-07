# Brickgame 

## Общая информация 

Сначала был реализован [тетрис](TETRIS_REEDME.md)
потом к нему была подключена [змейка](SNAKE_REEDME.md), 
есть общая [спецификация](materials/specification.md), а также файл с описанием созданных [конечных автоматов](materials/FSM.md)

## О запуске

```git clone``` \
```cd src```

Есть возможность запустить консольную версию игр через терминал с использованием Docker контейнера (предполагается установленный Docker)

```docker pull alpine:3.20``` \
```sh docker_run.sh``` \
```make install_cli``` \
```make run_cli``` \
```make test```

## Управление

Tetris \
Arrow keys(right, down, left) to move figures, space for rotation

Snake \
Arrow keys(right, down, left, up) to move snake

Common controls \
Exit - Esc key \
Pause - English 'p' or 'P'