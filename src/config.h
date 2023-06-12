#ifndef GTK_SAMP_CONFIG_H
#define GTK_SAMP_CONFIG_H

#define ADD_IN_ARRAY_START 0
#define ADD_IN_ARRAY_END 1

typedef char* ConfigArray;
typedef struct {
    char* key;
    char* value;
} ConfigEntry;

char* config_get_value(const char* filepath, const char* key);
int config_init(const char* filepath, const ConfigEntry config_structure[], size_t num_entries);
void config_change_value(const char* filepath, const char* key, const char* value, int delete_only);

char** config_array_parse(ConfigArray _config_array);
size_t config_array_get_length(ConfigArray _config_array);
ConfigArray config_array_add_item(ConfigArray config_array, char* _item, int where);
ConfigArray config_array_make(char* elements[], size_t num_entries);
ConfigArray config_array_remove_item(ConfigArray _config_array, int index);

#endif // GTK_SAMP_CONFIG_H