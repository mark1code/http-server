#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Base64 character set
static const char base64_chars[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

// Function to get the Base64 index of a character
static int base64_index(char c) {
    const char *ptr = strchr(base64_chars, c);
    if (ptr) {
        return ptr - base64_chars;
    }
    return -1; // Character not found in Base64 character set
}

void decode_base64(char *decoded, const char *encoded) {
    size_t encoded_len = strlen(encoded);
    size_t decoded_len = encoded_len / 4 * 3; // Every 4 Base64 chars = 3 bytes decoded data 

    // Handle padding
    if (encoded[encoded_len - 1] == '=') {
        decoded_len--;
        if (encoded[encoded_len - 1] == '=') {
            decoded_len--;
        }
    }

    size_t i, j;
    unsigned char buf[4];
    unsigned char temp[3];
    memset(decoded, 0, decoded_len);
    
    for (i = 0, j = 0; i < encoded_len;) {
        // Read four characters from the encoded string
        for (size_t k = 0; k < 4; k++) {
            buf[k] = base64_index(encoded[i++]);
        }

        // Decode Base64 block. Converts four 6-bit values to three bytes
        temp[0] = (buf[0] << 2) | (buf[1] >> 4);
        temp[1] = ((buf[1] & 0x0F) << 4) | (buf[2] >> 2);
        temp[2] = ((buf[2] & 0x03) << 6) | buf[3];

        // Handle padding
        if (encoded[i - 2] == '=') {
            decoded[j++] = temp[0];
        } else if (encoded[i - 1] == '=') {
            decoded[j++] = temp[0];
            decoded[j++] = temp[1];
        } else {
            decoded[j++] = temp[0];
            decoded[j++] = temp[1];
            decoded[j++] = temp[2];
        }
    }
}
