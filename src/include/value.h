#ifndef clox_value_h
#define clox_value_h

#include "common.h"

typedef enum {
    VAL_NIL,
    VAL_BOOL,
    VAL_NUMBER
} ValueType;

typedef struct {
    ValueType type;
    union {
        bool boolean;
        double number;
    } as;
} Value;

typedef struct {
    int count;
    int capacity;
    Value* values;
} ValueArray;

#define AS_BOOL(value)      ((value).as.boolean)
#define AS_NUMBER(value)    ((value).as.number)

#define IS_NIL(value)       ((value).type == VAL_NIL)
#define IS_BOOL(value)      ((value).type == VAL_BOOL)
#define IS_NUMBER(value)    ((value).type == VAL_NUMBER)

#define NIL_VAL             ((Value) {VAL_NIL,    { .number = 0    }})
#define BOOL_VAL(value)     ((Value) {VAL_BOOL,   {.boolean = value}})
#define NUMBER_VAL(value)   ((Value) {VAL_NUMBER, { .number = value}})

void init_value_array (ValueArray* array);
void write_value_array(ValueArray* array, Value value);
void free_value_array (ValueArray* array);

void print_value(Value value);
bool values_equal(Value a, Value b);

#endif