#include "stdio.h"
#define ROWS 4
#define COLUMNS 5
#define READ_MODE 0
#define WRITE_MODE 1

void print(int value, int position, int columns);

int main(void) {
    int arr[ROWS][COLUMNS] = {{0x01, 0x03, 0x01, 0x53},
                            {0x3A, 0x01, 0x10, 0x03},
                            {0xAB, 0x06, 0x4C, 0x13, 0xA8},
                            {0x0C, 0x05, 0x04, 0x00, 0x7C}
    };
    for (int i = 0; i < ROWS; i++) {
        int mode = (arr[i][1] == 1 || arr[i][1] == 3) ? READ_MODE : WRITE_MODE;
        for (int j = 0; j < COLUMNS; j++) {
            print(arr[i][j], j, mode);
        }
        printf("\n");
    }
    return 0;
}

void print(int value, int position, int mode) {
    switch (position) {
        case 0:
            printf("Адрес устройства: %d\n", value);
            break;
        case 1:
            if (value == 1)
                printf("Код функции: %d (Чтение бит)\n", value);
            if (value == 3)
                printf("Код функции: %d (Чтение байт)\n", value);
            if (value == 5)
                printf("Код функции: %d (Запись в бит)\n", value);
            if (value == 6)
                printf("Код функции: %d (Запись в байт)\n", value);
            break;
        case 2:
            printf("Адрес ячейки памяти: %d\n", value);
            break;
        case 3:
            if (mode == WRITE_MODE)
                printf("Записываемое значение: %d\n", value);
            else
                printf("Контрольная сумма для проверки целостности данных: %d\n", value);
            break;
        case 4:
            if (mode == WRITE_MODE)
                printf("Контрольная сумма для проверки целостности данных: %d\n", value);
            break;
    }
}
