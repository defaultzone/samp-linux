#ifndef GTK_SAMP_WINDOW_H
#define GTK_SAMP_WINDOW_H

extern GtkBuilder* main_window_builder;
extern GtkEntryBuffer* buffer_of_input_path_to_game;
extern GtkEntryBuffer* wine_executable_buffer;
extern GtkWindow* main_window;

void on_main_application_activate(GtkApplication* app, gpointer data);

#endif //GTK_SAMP_WINDOW_H
