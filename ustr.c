#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ustr.h"
#include "pset1.h"

/*
Initializes a new UStr with contents
*/
UStr new_ustr(char* contents) {
	int32_t bytes = strlen(contents);
	int32_t codepoints = utf8_strlen(contents);
	uint8_t all_ascii = bytes == codepoints;
	char* contents_copy = malloc(bytes + 1);
	strcpy(contents_copy, contents);
	UStr s = {
		codepoints,
		bytes,
		all_ascii,
		contents_copy
	};
	return s;
}

/*
Returns length of string
*/
int32_t len(UStr s) {
	return s.codepoints;
}

/*
Returns a substring of str, starting at index start (inclusive) 
and ending at index end (exclusive).

Returns an empty string on invalid range.
*/
UStr substring(UStr s, int32_t start, int32_t end) {
	// TODO: implement this
    if (start < 0 || end > s.codepoints || start >= end) {
        return new_ustr("");
    }

    char *str = s.contents;
    int32_t byte_pos = 0, cp = 0;

    while (cp < start) {
        unsigned char c = (unsigned char)str[byte_pos];
        int32_t clen;
        if      ((c & 0x80) == 0x00) clen = 1;
        else if ((c & 0xE0) == 0xC0) clen = 2;
        else if ((c & 0xF0) == 0xE0) clen = 3;
        else if ((c & 0xF8) == 0xF0) clen = 4;
        else                          clen = 1;
        byte_pos += clen;
        cp++;
    }
    int32_t byte_start = byte_pos;

    while (cp < end) {
        unsigned char c = (unsigned char)str[byte_pos];
        int32_t clen;
        if      ((c & 0x80) == 0x00) clen = 1;
        else if ((c & 0xE0) == 0xC0) clen = 2;
        else if ((c & 0xF0) == 0xE0) clen = 3;
        else if ((c & 0xF8) == 0xF0) clen = 4;
        else                          clen = 1;
        byte_pos += clen;
        cp++;
    }
    int32_t byte_end = byte_pos;

    int32_t slice_bytes = byte_end - byte_start;
    char *slice = malloc(slice_bytes + 1);
    memcpy(slice, str + byte_start, slice_bytes);
    slice[slice_bytes] = '\0';

    UStr out = new_ustr(slice);
    free(slice);
    return out;
}

/*
Given 2 strings s1 and s2, returns a string that is the result of 
concatenating s1 and s2. 
*/
UStr concat(UStr s1, UStr s2) {
	// TODO: implement this
    int32_t B1 = s1.bytes, B2 = s2.bytes;
    char *buf = malloc(B1 + B2 + 1);
    memcpy(buf,            s1.contents, B1);
    memcpy(buf + B1,       s2.contents, B2);
    buf[B1 + B2] = '\0';

    UStr out = {
        .codepoints = s1.codepoints + s2.codepoints,
        .bytes      = B1 + B2,
        .is_ascii   = s1.is_ascii && s2.is_ascii,
        .contents   = buf
    };
    return out;
}

/*
Given a string s and an index, return a string with the character at index 
removed from the original string. 

Returns the original string if index is out of bounds.
*/
UStr removeAt(UStr s, int32_t index) {
	// TODO: implement this
    if (index < 0 || index >= s.codepoints) {
        return s;
    }

    char *str = s.contents;
    int32_t byte_pos = 0, cp = 0;

    while (cp < index) {
        unsigned char c = (unsigned char)str[byte_pos];
        int32_t clen;
        if      ((c & 0x80) == 0x00) clen = 1;
        else if ((c & 0xE0) == 0xC0) clen = 2;
        else if ((c & 0xF0) == 0xE0) clen = 3;
        else if ((c & 0xF8) == 0xF0) clen = 4;
        else                          clen = 1;
        byte_pos += clen;
        cp++;
    }
    int32_t start = byte_pos;
    unsigned char c = (unsigned char)str[start];
    int32_t clen;
    if      ((c & 0x80) == 0x00) clen = 1;
    else if ((c & 0xE0) == 0xC0) clen = 2;
    else if ((c & 0xF0) == 0xE0) clen = 3;
    else if ((c & 0xF8) == 0xF0) clen = 4;
    else                          clen = 1;
    int32_t end = start + clen;

    int32_t new_bytes = s.bytes - clen;
    char *buf = malloc(new_bytes + 1);
    memcpy(buf,            str,       start);
    memcpy(buf + start,    str + end, s.bytes - end);
    buf[new_bytes] = '\0';

    UStr out = {
        .codepoints = s.codepoints - 1,
        .bytes      = new_bytes,
        .is_ascii   = s.is_ascii,
        .contents   = buf
    };
    return out;
}

/*
Given a string s, return s reversed. 

Example: reverse("apples🍎 and bananas🍌") = "🍌sananab dna 🍎selppa")
*/
UStr reverse(UStr s) {
	// TODO: implement this
    char *str = s.contents;
    int32_t B = s.bytes, C = s.codepoints;

    int32_t *pos = malloc((C + 1) * sizeof *pos);
    int32_t byte_pos = 0;
    for (int32_t i = 0; i < C; i++) {
        pos[i] = byte_pos;
        unsigned char c = (unsigned char)str[byte_pos];
        int32_t clen;
        if      ((c & 0x80) == 0x00) clen = 1;
        else if ((c & 0xE0) == 0xC0) clen = 2;
        else if ((c & 0xF0) == 0xE0) clen = 3;
        else if ((c & 0xF8) == 0xF0) clen = 4;
        else                          clen = 1;
        byte_pos += clen;
    }
    pos[C] = byte_pos;

    char *buf = malloc(B + 1);
    int32_t out_i = 0;
    for (int32_t j = C - 1; j >= 0; j--) {
        int32_t a = pos[j], b = pos[j+1], L = b - a;
        memcpy(buf + out_i, str + a, L);
        out_i += L;
    }
    buf[B] = '\0';
    free(pos);

    UStr out = {
        .codepoints = s.codepoints,
        .bytes      = s.bytes,
        .is_ascii   = s.is_ascii,
        .contents   = buf
    };
    return out;
}


void print_ustr(UStr s) {
	printf("%s [codepoints: %d | bytes: %d]", s.contents, s.codepoints, s.bytes);
}

void free_ustr(UStr s) {
	if (s.contents != NULL) {
		free(s.contents);
		s.contents = NULL;
	}
}

