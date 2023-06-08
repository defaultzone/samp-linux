#ifndef GTK_SAMP_ACTIONS_H
#define GTK_SAMP_ACTIONS_H

void open_file_view_for_select_game(GtkButton* button, gpointer data);
void check_wine_path(GtkButton* button, gpointer data);
void on_wine_input_changed(GtkEditable* self, gpointer user_data);
void on_game_path_input_changed(GtkEditable* self, gpointer user_data);
void clear_game_path_input(GtkButton* button, gpointer data);
void clear_wine_path_input(GtkButton* button, gpointer data);

#endif // GTK_SAMP_ACTIONS_H
