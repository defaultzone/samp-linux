#include "main.h"
#include "window.h"
#include "config.h"
#include "path.h"
#include "gtk-assets.h"

static void on_file_dialog_compete(GObject* source_object, GAsyncResult* result, gpointer data) {
    GtkFileDialog* dialog = GTK_FILE_DIALOG(source_object);
    GFile* file;

    file = gtk_file_dialog_open_finish(dialog, result, NULL);
    if (file) {
        const char* filepath = g_file_peek_path(file);
        gtk_entry_buffer_set_text(buffer_of_input_path_to_game, filepath, -1);
        change_value_in_config(get_config_file("config.conf"), "game_path", filepath, FALSE);
        g_object_unref(file);
    }
}

void open_file_view_for_select_game(GtkButton* button, gpointer data) {
    GtkFileDialog* file_dialog;
    GtkFileFilter* filter;

    filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, "gta_sa.exe");
    gtk_file_filter_add_pattern(filter, "gta_sa.exe");

    file_dialog = gtk_file_dialog_new();
    gtk_file_dialog_set_title(file_dialog, "Select gta_sa.exe");
    gtk_file_dialog_set_default_filter(file_dialog, filter);
    gtk_file_dialog_open(file_dialog, NULL, NULL, on_file_dialog_compete, NULL);
}

void check_wine_path(GtkButton* button, gpointer data) {
    const char* wine_command = gtk_entry_buffer_get_text(wine_executable_buffer);
    char wine_command_path[1024];
    snprintf(wine_command_path, sizeof(wine_command_path), "/usr/bin/which %s", wine_command);

    FILE* fp = popen(wine_command_path, "r");
    if (fp == NULL) {
        gtk_alert_dialog_show(gtk_alert_dialog_new("wine не установлен"), main_window);
    } else {
        char result[1024];
        if (fgets(result, sizeof(result), fp)) {
            char* p = strchr(result, '\n');
            if (p != NULL) *p = '\0';
            char command[1024];
            snprintf(command, sizeof(command), "%s --version", result);
            pclose(fp);
            fp = popen(command, "r");
            if (fp == NULL) {
                gtk_alert_dialog_show(gtk_alert_dialog_new("Ошибка запуска wine"), main_window);
            } else {
                while (fgets(result, sizeof(result), fp)) {
                    char* p = strchr(result, '\n');
                    if (p != NULL) *p = '\0';
                    gtk_alert_dialog_show(gtk_alert_dialog_new("Версия wine: %s", result), main_window);
                }
                pclose(fp);
            }
        } else {
            gtk_alert_dialog_show(gtk_alert_dialog_new("Команда не найдена"), main_window);
        }
    }
}

void on_game_path_input_changed(GtkEditable* self, gpointer user_data) {
    change_value_in_config(get_config_file("config.conf"), "game_path", gtk_editable_get_text(self), FALSE);
}

void on_wine_input_changed(GtkEditable* self, gpointer user_data) {
    change_value_in_config(get_config_file("config.conf"), "wine_path", gtk_editable_get_text(self), FALSE);
}

void clear_game_path_input(GtkButton* button, gpointer data) {
    gtk_entry_buffer_set_text(buffer_of_input_path_to_game, "", -1);
    change_value_in_config(get_config_file("config.conf"), "game_path", "", FALSE);
}

void clear_wine_path_input(GtkButton* button, gpointer data) {
    gtk_entry_buffer_set_text(wine_executable_buffer, "", -1);
    change_value_in_config(get_config_file("config.conf"), "game_path", "", FALSE);
}