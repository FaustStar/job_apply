#include "task.h"

int main(void) {
    matrix data;
    status info;
    if (input(&data) == VALID_INPUT) {
        for (int i = 0; i < data.rows; i++) {
            if (data.matrix[i][data.columns - 1] == VALID_INPUT) {
                info.mode = (data.matrix[i][1] == 1 || data.matrix[i][1] == 3) ? READ_MODE : WRITE_MODE;
                info.crc8 = check_crc8(data.matrix[i], info.mode);
                for (int j = 0; j < data.columns; j++) {
                    print(data.matrix[i][j], j, info);
                }
            } else {
                print_err(data.matrix[i][data.columns - 1]);
            }
            printf("\n");
        }
        free_memory(&data);
    } else {
        print_err(NOT_VALID_FILE);
    }
    return 0;
}

int input(matrix *data) {
    int error = VALID_INPUT;
    FILE *file = fopen("input00.txt", "r");
    if (file != NULL) {
        allocate_memory(data);
        char str[2];
        int ch = 0;
        for (int i = 0; i < data->rows; i++) {
            int count = 0;
            for (int j = 0; j < data->columns - 1 && ch != EOF; j++) {
                process_input(data, i, &j, file, str, &ch, &count);
            }
            check_error(data, i, count);
        }
        fclose(file);
    } else {
        error = NOT_VALID_FILE;
    }
    return error;
}

void process_input(matrix *data, int i, int *j, FILE *file, char *str, int *ch, int *count) {
    if (check_input(file, str, ch, count) == 0) {
        int n = strtol(str, NULL, 16);
        data->matrix[i][*j] = n;
    } else {
        data->matrix[i][data->columns - 1] = NOT_VALID_INPUT;
    }
    if (*count == 5 && *ch != '\n' && *ch != EOF) {
        data->matrix[i][data->columns - 1] = COUNT_ERROR_MAX;
        while((*ch = fgetc(file)) != '\n' && *ch != EOF);
    }
    if (*ch == '\n') {
        *j = data->columns;
        reallocate_memory(data);
    }
}

int check_input(FILE *file, char *str, int *ch, int *count) {
    (*count)++;
    if (fscanf(file, "%2s", str) != 1) {
        return 1;
    }
    *ch = fgetc(file);
    if (*ch != ' ' && *ch != '\n' && *ch != EOF) {
        return 1;
    }
    return 0;
}

void check_error(matrix *data, int i, int count) {
    if (data->matrix[i][data->columns - 1] != NOT_VALID_INPUT) {
        if (count <= 1) {
            data->matrix[i][data->columns - 1] = COUNT_ERROR_MIN;
        } else if (data->matrix[i][1] != 1 && data->matrix[i][1] != 3 && data->matrix[i][1] != 5 && data->matrix[i][1] != 6) {
            data->matrix[i][data->columns - 1] = NOT_VALID_OPERATION;
        } else if ((data->matrix[i][1] == 1 || data->matrix[i][1] == 3) && count != 4) {
            data->matrix[i][data->columns - 1] = COUNT_ERROR_R;
        } else if ((data->matrix[i][1] == 5 || data->matrix[i][1] == 6) && count != 5) {
            data->matrix[i][data->columns - 1] = COUNT_ERROR_W;
        }
    }
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

void print_err(my_type error) {
    switch (error) {
        case NOT_VALID_INPUT:
            printf("Ошибка: ввод не верный!\n");
            break;
        case NOT_VALID_OPERATION:
            printf("Ошибка: такого кода функции не существует!\n");
            break;
        case COUNT_ERROR_R:
            printf("Ошибка: для чтения в вводе должно быть 4 байта!\n");
            break;
        case COUNT_ERROR_W:
            printf("Ошибка: для записи в вводе должно быть 5 байтов!\n");
            break;
        case COUNT_ERROR_MIN:
            printf("Ошибка: слишком мало данных в вводе!\n");
            break;
        case COUNT_ERROR_MAX:
            printf("Ошибка: слишком много данных в вводе!\n");
            break;
        case NOT_VALID_FILE:
            printf("Ошибка: такого файла не существует!\n");
            break;
    }
}

void allocate_memory(matrix *data) {
    data->rows = ROWS;
    data->columns = COLUMNS;
    data->matrix = calloc(data->rows, sizeof(my_type *));
    for (int i = 0; i < data->rows; i++) {
        data->matrix[i] = calloc(data->columns, sizeof(my_type));
    }
}

void reallocate_memory(matrix *data) {
    (data->rows)++;
    data->matrix = realloc(data->matrix, data->rows * sizeof(my_type *));
    for (int i = data->rows - 1; i < data->rows; i++) {
        data->matrix[i] = calloc(data->columns, sizeof(my_type));
    }
}

void free_memory(matrix *data) {
    for (int i = 0; i < data->rows; i++) {
        free(data->matrix[i]);
    }
    free(data->matrix);
}