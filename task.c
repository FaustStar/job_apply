#include <stdio.h>
#include <stdlib.h>
#define ROWS 1
#define COLUMNS 6
#define READ_MODE 0
#define WRITE_MODE 1
#define VALID_INPUT 0
#define NO_VALID_INPUT 1

typedef unsigned char my_type;

typedef struct matrix {
    int rows;
    int columns;
    my_type **matrix;
} matrix;

typedef struct status {
    int mode;
    my_type crc8;
} status;

void process_input(matrix *data);
int check_input(FILE *file, char *str, int *ch);
my_type check_crc8(my_type *matrix, int len);
void print(my_type value, int position, status info);
void print_crc8_status(my_type value, my_type crc8);
void free_memory(matrix *data);

int main(void) {
    matrix data;
    status info;
    process_input(&data);
    for (int i = 0; i < data.rows; i++) {
        if (data.matrix[i][data.columns - 1] == VALID_INPUT) {
            info.mode = (data.matrix[i][1] == 1 || data.matrix[i][1] == 3) ? READ_MODE : WRITE_MODE;
            info.crc8 = check_crc8(data.matrix[i], info.mode);
            for (int j = 0; j < data.columns; j++) {
                print(data.matrix[i][j], j, info);
            }
        } else {
            printf("Error: not correct input!\n");
        }
        printf("\n");
    }
    free_memory(&data);
    return 0;
}

void process_input(matrix *data) {
    FILE *file = fopen("input00.txt", "r");
    if (file != NULL) {
        data->rows = ROWS;
        data->columns = COLUMNS;
        data->matrix = calloc(data->rows, sizeof(my_type *));
        for (int i = 0; i < data->rows; i++) {
            data->matrix[i] = calloc(data->columns, sizeof(my_type));
        }
        char str[2];
        int ch = 0;
        for (int i = 0; i < data->rows; i++) {
            for (int j = 0; j < data->columns - 1 && ch != EOF; j++) {
                if (check_input(file, str, &ch) == 0) {
                    int n = strtol(str, NULL, 16);
                    data->matrix[i][j] = n;
                } else {
                    data->matrix[i][data->columns - 1] = NO_VALID_INPUT;
                }
                if (ch == '\n') {
                    j = data->columns;
                    (data->rows)++;
                    data->matrix = realloc(data->matrix, data->rows * sizeof(my_type *));
                    for (int k = data->rows - 1; k < data->rows; k++) {
                        data->matrix[k] = calloc(data->columns, sizeof(my_type));
                    }
                }
            }
        }
        fclose(file);
    } else {
        printf("Error: the file doesn't exist!\n");
    }
}

int check_input(FILE *file, char *str, int *ch) {
    if (fscanf(file, "%2s", str) != 1) {
        return 1;
    }
    *ch = fgetc(file);
    if (*ch != ' ' && *ch != '\n' && *ch != EOF) {
        //while((*ch = fgetc(file)) != '\n' && *ch != EOF);
        return 1;
    }
    return 0;
}

my_type check_crc8(my_type *matrix, int mode) {
    my_type crc8 = 0x00;
    int len = (mode == READ_MODE) ? 3 : 4;
    while (len--) {
        crc8 ^= *matrix++; //crc8 ^= matrix[i++]
        for (int i = 0; i < 8; i++)
            crc8 = crc8 & 0x80 ? (crc8 << 1) ^ 0x07 : crc8 << 1;
    }
    return crc8;
}

void print(my_type value, int position, status info) {
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
            if (info.mode == WRITE_MODE)
                printf("Записываемое значение: %d\n", value);
            else
                print_crc8_status(value, info.crc8);
            break;
        case 4:
            if (info.mode == WRITE_MODE)
                print_crc8_status(value, info.crc8);
            break;
    }
}

void print_crc8_status(my_type value, my_type crc8) {
    printf("Контрольная сумма для проверки целостности данных: %d ", value);
    if (value == crc8)
        printf("(расчет произведен верно)\n");
    else
        printf("(расчет произведен не верно, сумма должна быть %d)\n", crc8);
}

void free_memory(matrix *data) {
    for (int i = 0; i < data->rows; i++) {
        free(data->matrix[i]);
    }
    free(data->matrix);
}