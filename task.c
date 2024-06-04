#include <stdio.h>
#include <stdlib.h>
#define ROWS 4
#define COLUMNS 6
#define READ_MODE 0
#define WRITE_MODE 1

typedef unsigned char my_type;

typedef struct matrix {
    int rows;
    int columns;
    my_type **matrix;
} matrix;

void process_input(matrix *data);
int check_input(FILE *file, char *str, int *ch);
void print(my_type value, int position, int columns);
void free_memory(matrix *data);

int main(void) {
    /*int arr[ROWS][COLUMNS] = {{0x01, 0x03, 0x01, 0x53},
                            {0x3A, 0x01, 0x10, 0x03},
                            {0xAB, 0x06, 0x4C, 0x13, 0xA8},
                            {0x0C, 0x05, 0x04, 0x00, 0x7C}
    };*/
    matrix data;
    process_input(&data);
    for (int i = 0; i < data.rows; i++) {
        int mode = (data.matrix[i][1] == 1 || data.matrix[i][1] == 3) ? READ_MODE : WRITE_MODE;
        for (int j = 0; j < data.columns; j++) {
            print(data.matrix[i][j], j, mode);
        }
        printf("\n");
    }
    free_memory(&data);
    return 0;
}

void process_input(matrix *data) {
    FILE *file = fopen("input00.txt", "r");
    if (file != NULL) {
        data->rows = 1;
        data->columns = COLUMNS;
        data->matrix = calloc(data->rows, sizeof(my_type *));
        for (int i = 0; i < data->rows; i++) {
            data->matrix[i] = calloc(data->columns, sizeof(my_type));
        }
        char str[2];
        int ch = 0;
        for (int i = 0; i < data->rows; i++) {
            for (int j = 0; j < data->columns - 1; j++) {
                if (check_input(file, str, &ch) == 0) {
                    int n = strtol(str, NULL, 16);
                    data->matrix[i][j] = n;
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
        printf("The file doesn't exist!\n");
    }
}

int check_input(FILE *file, char *str, int *ch) {
    if (fscanf(file, "%2s", str) != 1) {
        return 1;
    }
    *ch = fgetc(file);
    if (*ch != ' ' && *ch != '\n' && *ch != EOF) {
        return 1;
    }
    return 0;
}

void print(my_type value, int position, int mode) {
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

void free_memory(matrix *data) {
    for (int i = 0; i < data->rows; i++) {
        free(data->matrix[i]);
    }
    free(data->matrix);
}