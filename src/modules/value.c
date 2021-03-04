#include "value.h"
#include "memory.h"
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
    printf("%g", value);
}