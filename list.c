#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ustr.h"
#include "list.h"

/*
Returns an empty list of initial_capacity
*/
List new_list(int32_t initial_capacity) {
    List list;
    list.size = 0;
    list.capacity = initial_capacity;
    list.data = malloc(initial_capacity * sizeof(struct UStr));
    return list;
}

/*
Initializes an list of length size with elements from array
*/
List new_list_from_array(struct UStr* array, int32_t size) {
    List list = new_list(size);
    for (int32_t i = 0; i < size; i++) {
        list.data[i] = array[i];
    }
    list.size = size;
    return list;
}


/*
Given a list of strings and a separator string, returns a single string 
containing all the strings in list joined by the separator.
*/
UStr join(List* list, UStr separator) {
    // TODO: implement this
    if (list->size == 0) {
        return new_ustr("");
    }

    int total_bytes = 0;
    for (int i = 0; i < list->size; i++) {
        total_bytes += list->data[i].bytes;
        if (i < list->size - 1) {
            total_bytes += separator.bytes;
        }
    }

    char* buffer = malloc(total_bytes + 1);  // +1 for '\0'
    char* ptr = buffer;

    for (int i = 0; i < list->size; i++) {
        memcpy(ptr, list->data[i].contents, list->data[i].bytes);
        ptr += list->data[i].bytes;

        if (i < list->size - 1) {
            memcpy(ptr, separator.contents, separator.bytes);
            ptr += separator.bytes;
        }
    }

    *ptr = '\0';
    UStr result = new_ustr(buffer);
    free(buffer);
    return result;
}

/*
Inserts string s into list at index s, shifting elements to the right.
Expands the list's capacity if necessary (double the capacity, or set to 1 if 0).

Returns 1 on success, 0 if the index is invalid (out of bounds).
*/
int8_t insert(List* list, UStr s, int32_t index) {
    // TODO: implement this
    if (index < 0 || index > list->size) return 0;

    if (list->size >= list->capacity) {
        int new_capacity = list->capacity == 0 ? 1 : list->capacity * 2;
        list->data = realloc(list->data, new_capacity * sizeof(UStr));
        list->capacity = new_capacity;
    }

    for (int i = list->size; i > index; i--) {
        list->data[i] = list->data[i - 1];
    }

    list->data[index] = s;
    list->size++;
    return 1;
}

/*
Removes the element at the given index and shifts all subsequent elements left.

Returns 1 on success, 0 if the index is invalid (out of bounds).
*/
int8_t listRemoveAt(List* list, int32_t index) {
    // TODO: implement this
    if (index < 0 || index >= list->size) return 0;

    free_ustr(list->data[index]);
    for (int i = index; i < list->size - 1; i++) {
        list->data[i] = list->data[i + 1];
    }
    list->size--;
    return 1;
}

/*
Splits the given string s into substrings separated by the given delimiter string.

Returns a List of String objects, each containing a segment between delimiters.

If the delimiter is the empty string, return a list containing the original 
string as a single element.

If the input string ends with the delimiter, include an empty string at the 
end of the result.

Note that the delimiter could be of a length of more than 1 character
*/
List split(UStr s, UStr separator) {
    // TODO: implement this
     List result = new_list(4);

    if (separator.bytes == 0) {
        insert(&result, s, result.size);
        return result;
    }

    char* start = s.contents;
    char* end = strstr(start, separator.contents);

    while (end != NULL) {
        int segment_length = end - start;

        char* segment = malloc(segment_length + 1);
        memcpy(segment, start, segment_length);
        segment[segment_length] = '\0';

        insert(&result, new_ustr(segment), result.size);
        free(segment);

        start = end + separator.bytes;
        end = strstr(start, separator.contents);
    }

    // Add the last segment (or empty string if ends with separator)
    insert(&result, new_ustr(start), result.size);

    return result;
}
