#include "main.h"
#include "window.h"
#include "config.h"
#include "path.h"

int main(int argc, char** argv) {
    ConfigEntry config[] = {
            {"game_path", ""},
            {"wine_path", "wine"}
    };

    if (config_init(get_config_file("config.conf"), config, sizeof(config) / sizeof(ConfigEntry)) == FALSE) {
        printf("Something went wrong with config. Goodbye\n");
        // TODO: Display an error message using gtk_alert..
        return EXIT_FAILURE;
    } else {
        GtkApplication* window = gtk_application_new("org.gtkmm.dzone.samp", G_APPLICATION_DEFAULT_FLAGS);
        g_signal_connect(window, "activate", G_CALLBACK(on_main_application_activate), NULL);
        int status = g_application_run(G_APPLICATION(window), argc, argv);
        g_object_unref(window);

        return status;
    }
}