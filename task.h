#ifndef TASK_H
#define TASK_H

#include <stdio.h>
#include <stdlib.h>

#define ROWS 1
#define COLUMNS 6
#define READ_MODE 0
#define WRITE_MODE 1

#define VALID_INPUT 0
#define NOT_VALID_INPUT 1
#define NOT_VALID_OPERATION 2
#define NOT_VALID_FILE 3
#define COUNT_ERROR_R 4
#define COUNT_ERROR_W 5
#define COUNT_ERROR_MIN 6
#define COUNT_ERROR_MAX 7

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

int input(matrix *data);
void process_input(matrix *data, int i, int *j, FILE *file, char *str, int *ch, int *count);
int check_input(FILE *file, char *str, int *ch, int *count);
void check_error(matrix *data, int i, int count);
my_type check_crc8(my_type *matrix, int len);
void print(my_type value, int position, status info);
void print_crc8_status(my_type value, my_type crc8);
void print_err(my_type error);
void allocate_memory(matrix *data);
void reallocate_memory(matrix *data);
void free_memory(matrix *data);

#endif