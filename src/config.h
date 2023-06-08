#ifndef GTK_SAMP_CONFIG_H
#define GTK_SAMP_CONFIG_H

typedef struct {
    char key[64];
    char value[256];
} ConfigEntry;

char* get_value_from_config(const char* filepath, const char* key);
int config_init(const char* filepath, const ConfigEntry* config_structure, size_t num_entries);
void change_value_in_config(const char* filepath, const char* key, const char* value, int delete_only);

#endif // GTK_SAMP_CONFIG_H