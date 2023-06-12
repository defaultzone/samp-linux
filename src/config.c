#include <assert.h>
#include "main.h"
#include "string.h"
#include "config.h"

int config_init(const char* filepath, const ConfigEntry config_structure[], size_t num_entries) {
    if (access(filepath, F_OK) != 0) {
        FILE* config = fopen(filepath, "w");

        if (config == NULL) return FALSE;
        for (int i = 0; i < num_entries; i++) {
            fprintf(config, "%s=%s\n", config_structure[i].key, string_make_config_string(config_structure[i].value));
        }

        fclose(config);
        return TRUE;
    } else {
        regex_t regex;
        int result;
        const char* pattern = "^\\w+=.*$";

        FILE* config = fopen(filepath, "r");
        if (config == NULL) return FALSE;

        char* line = NULL;
        size_t size = 0;

        result = regcomp(&regex, pattern, REG_EXTENDED);

        // Couldn't compile regex or no access to file.
        if (result != 0 || config == NULL) return FALSE;
        while (getline(&line, &size, config) != -1) {
            result = regexec(&regex, line, 0, NULL, 0);
            if (result == REG_NOMATCH) return FALSE; // Someone (or something) changed the settings file incorrectly.
        }

        free(line);
        fclose(config);
        regfree(&regex);

        return TRUE;
    }
}

char* config_get_value(const char* filepath, const char* key) {
    char* value = NULL;

    FILE* config = fopen(filepath, "r");
    char* line = NULL;
    size_t size = 0;

    while (getline(&line, &size, config) != -1) {
        if (strstr(line, key) == line) {
            size_t len = strlen(key);
            if (line[len] == '=') {
                // Skip the equal sign and get the value.
                value = strdup(line + len + 1);
                // Remove the newline character at the end of the value, if present.
                if (value[strlen(value) - 1] == '\n') {
                    value[strlen(value) - 1] = '\0';
                }
                break;
            }
        }
    }

    fclose(config);
    free(line);

    return value;
}

void config_change_value(const char* filepath, const char* key, const char* value, int delete_only) {
    FILE* config = fopen(filepath, "r");
    FILE* temp = fopen("temp", "w");
    char* line = NULL;
    size_t size = 0;

    while (getline(&line, &size, config) != -1) {
        char* delim = strchr(line, '=');
        if (delim == NULL) {
            fprintf(temp, "%s", line);
            continue;
        }
        char* curr_key = calloc(delim - line + 1, sizeof(char));
        strncpy(curr_key, line, delim - line);

        if (strcmp(curr_key, key) == 0) {
            if (delete_only) {
                continue;
            } else {
                fprintf(temp, "%s=%s\n", key, string_make_config_string(value));
            }
        } else {
            fprintf(temp, "%s", line);
        }

        free(curr_key);
    }

    free(line);
    fclose(config);
    fclose(temp);

    remove(filepath);
    rename("temp", filepath);
}

char** config_array_parse(ConfigArray _config_array) {
    // Deleting first and last symbol in 'string_array', they always be '[' and ']'.
    char* config_array = string_remove_first_last_char(_config_array);

    // Counting total entries of ';'. We need this for memory allocating.
    int count = 0;
    for (int i = 0; i < strlen(config_array); i++)
        if (config_array[i] == ';') count++;

    // Count of ';' can't be zero. If it's zero then don't use string_array: use string as value.
    assert(count != 0);

    // Allocate memory for char** and char*.
    char** values = malloc(count * sizeof(char*));  // to return
    char* value;                                    // to parse values

    // Create a temporary copy of the string
    char* string_copy = strdup(config_array); // allocate memory for the copy and copy the string

    // Parsing values; replace each '%#0` to `;`.
    value = strtok(string_copy, ";");
    int i = 0;
    while (value != NULL) {
        size_t value_len = strlen(value);
        values[i] = malloc(value_len + 1); // +1 for terminating character

        size_t j = 0;
        for (size_t k = 0; k < value_len; k++) {
            if (value[k] == '%' && value[k + 1] == '#' && value[k + 2] == '0') {
                // we found '%#0' => replace with ';'
                values[i][j++] = ';';
                k += 2;
            } else {
                values[i][j++] = value[k];
            }
        }

        values[i][j] = '\0';
        i++;
        value = strtok(NULL, ";");
    }

    free(string_copy); // free the temporary copy of the string

    return values;
}

size_t config_array_get_length(ConfigArray _config_array) {
    char** config_array = config_array_parse(_config_array);
    size_t length = 0;

    while (config_array[length] != NULL) length++;
    free(config_array);

    return length;
}

ConfigArray config_array_add_item(ConfigArray config_array, char* _item, int where) {
    char* item = string_replace_semicolons(_item);
    size_t len = strlen(config_array);
    size_t elen = strlen(item);
    char* new_array = (char*) malloc(len + elen + 3); // +3 for '[' , ']' and '\0'.

    if (new_array == NULL) return NULL;

    if (where == ADD_IN_ARRAY_END) {
        strcpy(new_array, config_array);
        new_array[len - 1] = ';';
        new_array[len] = '\0';
        strcat(new_array, item);
        strcat(new_array, "]");
    } else if (where == ADD_IN_ARRAY_START) {
        strcpy(new_array, "[");
        strcat(new_array, item);
        strcat(new_array, ";");
        strcat(new_array, config_array + 1);
    } else {
        free(new_array);
        free(item);
        return NULL;
    }

    free(item);
    return new_array;
}

ConfigArray config_array_make(char* elements[], size_t num_entries) {
    size_t all_elements_length = 0;
    for (size_t i = 0; i < num_entries; i++)
        all_elements_length += strlen(string_replace_semicolons(elements[i]));

    char* result = malloc(all_elements_length + (num_entries - 1) + 3);
    result[0] = '[';

    for (size_t i = 0; i < num_entries; i++) {
        strcat(result, string_replace_semicolons(elements[i]));
        if (i != (num_entries - 1)) strcat(result, ";");
    }

    result[strlen(result)]      = ']';
    result[strlen(result) + 1]  = '\0';

    return result;
}

ConfigArray config_array_remove_item(ConfigArray _config_array, int index) {
    size_t config_array_length = config_array_get_length(_config_array);

    if (config_array_length - 1 >= index) {
        char** config_array = config_array_parse(config_array);
        char** temp_config_array = (char**) malloc(sizeof(char*) * (config_array_length - 1));
        size_t i, j;

        for (i = 0, j = 0; i < config_array_length; i++)
            if (i != index) temp_config_array[j++] = config_array[i];

        ConfigArray new_config_array = config_array_make(temp_config_array, config_array_length - 1);
        free(config_array);
        free(temp_config_array);

        return new_config_array;
    } else {
        return _config_array;
    }
}