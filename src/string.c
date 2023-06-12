#include <string.h>
#include <stdlib.h>

char* string_remove_first_last_char(char* source) {
    size_t len = strlen(source);
    char* result = malloc(sizeof(char) * (len - 2));
    strncpy(result, source + 1, len - 2);
    result[len - 2] = '\0';
    return result;
}

char* string_make_config_string(const char* string) {
    const struct Characters {
        char source;
        const char* const new;
    } characters[] = {
            {'\n', "\\n"}
    };

    char* output = strdup(string);
    for (size_t i = 0; i < strlen(output); i++) {
        for (size_t j = 0; j < sizeof(characters) / sizeof(characters[0]); j++) {
            if (output[i] == characters[j].source) {
                memmove(output + i + strlen(characters[j].new) - 1, output + i, strlen(output + i));
                memcpy(output + i, characters[j].new, strlen(characters[j].new));
                i += strlen(characters[j].new) - 1;
                break;
            }
        }
    }

    return output;
}

char* string_replace_semicolons(char* source) {
    size_t semicolons_count = 0;
    for (size_t i = 0; i < strlen(source); i++)
        if (source[i] == ';') semicolons_count++;

    size_t j = 0;
    char* output = malloc(strlen(source) + (semicolons_count * 2) + 1);
    for (size_t i = 0; i < strlen(source); i++) {
        if (source[i] == ';') {
            output[j]     = '%';
            output[j + 1]   = '#';
            output[j + 2]   = '0';
            j += 3;
        } else {
            output[j++] = source[i];
        }
    }
    output[j] = '\0';

    return output;
}