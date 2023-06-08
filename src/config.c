#include "main.h"

typedef struct {
    char key[64];
    char value[256];
} ConfigEntry;

int config_init(const char* filepath, const ConfigEntry* config_structure, size_t num_entries) {
    if (access(filepath, F_OK) != 0) {
        FILE* config = fopen(filepath, "w");

        if (config == NULL) return FALSE;
        for (int i = 0; i < num_entries; i++) {
            fprintf(config, "%s=%s\n", config_structure[i].key, config_structure[i].value);
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

char* get_value_from_config(const char* filepath, const char* key) {
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

void change_value_in_config(const char* filepath, const char* key, const char* value, int delete_only) {
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
                fprintf(temp, "%s=%s\n", key, value);
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