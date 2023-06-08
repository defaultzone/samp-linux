#include "main.h"
#include "window.h"
#include "config.h"
#include "path.h"
#include "gtk-assets.h"

int main(int argc, char** argv) {
    ConfigEntry config[] = {
            {"game_path", ""},
            {"wine_path", "wine"}
    };

    if (config_init(get_config_file("config.conf"), config, 2) == FALSE) {
        printf("Something went wrong with config. Goodbye\n");
        // TODO: Display an error message using gtk_alert..
        return EXIT_FAILURE;
    } else {
        // Create the main GTK-4 window.
        GtkApplication *main_window = gtk_application_new("org.gtkmm.dzone.samp", G_APPLICATION_DEFAULT_FLAGS);
        g_signal_connect(main_window, "activate", G_CALLBACK(on_main_application_activate), NULL);
        int status = g_application_run(G_APPLICATION(main_window), argc, argv);
        g_object_unref(main_window);

        return status;
    }
}