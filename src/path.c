#include "main.h"

char* get_config_file(const char* which) {
    char* config_dir = g_build_filename(g_get_user_config_dir(), "samp-linux", NULL);
    g_mkdir_with_parents(config_dir, 07777);
    char* file_path = g_build_filename(config_dir, which, NULL);
    g_free(config_dir);
    return file_path;
}

char* get_shared_file(const char* which) {
    char* dir = g_build_filename("/", "usr", "share", "samp-linux", which, NULL);
    g_mkdir_with_parents(dir, 07777);
    return dir;
}