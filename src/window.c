#include "main.h"
#include "window.h"
#include "config.h"
#include "actions.h"
#include "path.h"

#define SIGNAL_ON_CLICKED "clicked"
#define SIGNAL_ON_CHANGED "changed"

GtkBuilder* main_window_builder;
GtkEntryBuffer* buffer_of_input_path_to_game;
GtkEntryBuffer* wine_executable_buffer;
GtkWindow* main_window;

void on_main_application_activate(GtkApplication* app, gpointer data) {
    GtkSettings* settings;

    GObject* window;
    GObject* button_select_path_to_game;
    GObject* button_check_path_to_wine;
    GObject* input_path_to_game;
    GObject* input_path_to_wine;
    GObject* clear_game_path_button;
    GObject* clear_wine_path_button;

    const char* config_path = get_config_file("config.conf");

    buffer_of_input_path_to_game = gtk_entry_buffer_new(
            get_value_from_config(config_path, "game_path"),
            -1
            );

    wine_executable_buffer = gtk_entry_buffer_new(
            get_value_from_config(config_path, "wine_path"),
            -1
            );

    main_window_builder = gtk_builder_new();
    gtk_builder_add_from_file(main_window_builder, get_shared_file("interface/main.ui"), NULL);

    settings = gtk_settings_get_default();
    g_object_set(settings, "gtk-application-prefer-dark-theme", TRUE, NULL);

    //
    // The contents of the settings tab.
    //

    button_select_path_to_game = gtk_builder_get_object(main_window_builder, "button_select_path_to_game");
    g_signal_connect(button_select_path_to_game, SIGNAL_ON_CLICKED, G_CALLBACK(open_file_view_for_select_game), NULL);

    button_check_path_to_wine = gtk_builder_get_object(main_window_builder, "button_check_wine");
    g_signal_connect(button_check_path_to_wine, SIGNAL_ON_CLICKED, G_CALLBACK(check_wine_path), NULL);

    input_path_to_game = gtk_builder_get_object(main_window_builder, "input_path_to_game");
    gtk_entry_set_buffer(GTK_ENTRY(input_path_to_game), buffer_of_input_path_to_game);
    g_signal_connect(input_path_to_game, SIGNAL_ON_CHANGED, G_CALLBACK(on_game_path_input_changed), NULL);

    input_path_to_wine = gtk_builder_get_object(main_window_builder, "wine_input");
    gtk_entry_set_buffer(GTK_ENTRY(input_path_to_wine), wine_executable_buffer);
    g_signal_connect(input_path_to_wine, SIGNAL_ON_CHANGED, G_CALLBACK(on_wine_input_changed), NULL);

    clear_game_path_button = gtk_builder_get_object(main_window_builder, "clear_game_path_input");
    g_signal_connect(clear_game_path_button, SIGNAL_ON_CLICKED, G_CALLBACK(clear_game_path_input), NULL);

    clear_wine_path_button = gtk_builder_get_object(main_window_builder, "clear_wine_path_input");
    g_signal_connect(clear_wine_path_button, SIGNAL_ON_CLICKED, G_CALLBACK(clear_wine_path_input), NULL);

    //
    // Window.
    //

    window = gtk_builder_get_object(main_window_builder, "window");
    main_window = GTK_WINDOW(window);
    gtk_window_set_application(main_window, app);
    gtk_widget_set_visible(GTK_WIDGET(window), TRUE);

    g_object_unref(main_window_builder);
}