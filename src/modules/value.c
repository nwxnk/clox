#include "value.h"
#include "memory.h"
#include "common.h"
#include "scanner.h"

#include <stdio.h>
#include <stdlib.h>

void init_value_array(ValueArray* array) {
    array->count = 0;
    array->capacity = 0;
    array->values = NULL;
}

void write_value_array(ValueArray* array, Value value) {
    if (array->count + 1 > array->capacity) {
        int capacity = GROW_CAPACITY(array->capacity);
        array->values = GROW_ARRAY(Value, array->values, array->capacity, capacity);
        array->capacity = capacity;
    }

    array->values[array->count++] = value;
}

void free_value_array(ValueArray* array) {
    FREE_ARRAY(Value, array->values, array->capacity);
    init_value_array(array);
}

void print_value(Value value) {
    switch (value.type) {
        case VAL_NIL:    printf("nil"); break;
        case VAL_BOOL:   printf("%s", AS_BOOL(value) == true ? "true" : "false"); break;
        case VAL_NUMBER: printf("%g", AS_NUMBER(value)); break;
    }
}

bool values_equal(Value a, Value b) {
    if (a.type != b.type) return false;

    switch (a.type) {
        case VAL_NIL:    return true;
        case VAL_BOOL:   return AS_BOOL(a) == AS_BOOL(b);
        case VAL_NUMBER: return AS_NUMBER(a) == AS_NUMBER(b);
        default:
            return false;
    }
}