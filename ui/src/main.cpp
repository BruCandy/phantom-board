#include <iostream>

//gtk
#include <gtk/gtk.h>

//libayatana-appindicator
#include <libayatana-appindicator/app-indicator.h>

//sdbus-c++
#include <sdbus-c++/sdbus-c++.h>

//original
#include <dbus_configure.hpp>


using namespace phantomboard::daemon;

static AppIndicator *indicator;

static void set_icon_for_mode(const gchar *mode) {
    if (g_strcmp0(mode, "Phantom") == 0) {
        app_indicator_set_icon_full(indicator, "icon_white", "Phantom Mode");
    } else {
        app_indicator_set_icon_full(indicator, "icon_gray", "Normal Mode");
    }
}

static gboolean update_icon_idle(gpointer data) {
    gchar *mode = static_cast<gchar*>(data);
    set_icon_for_mode(mode);
    g_free(mode);
    return G_SOURCE_REMOVE;
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    indicator = app_indicator_new(
        "phantom-board",
        "icon_gray",
        APP_INDICATOR_CATEGORY_APPLICATION_STATUS
    );

    app_indicator_set_icon_theme_path(indicator, ".local/share/icons");
    app_indicator_set_status(indicator, APP_INDICATOR_STATUS_ACTIVE);

    GtkWidget *menu = gtk_menu_new();
    GtkWidget *item_quit = gtk_menu_item_new_with_label("Quit Phantom Board");
    g_signal_connect(item_quit, "activate", G_CALLBACK(gtk_main_quit), NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), item_quit);
    gtk_widget_show_all(menu);

    app_indicator_set_menu(indicator, GTK_MENU(menu));

    auto connection = sdbus::createSessionBusConnection();
    auto proxy = sdbus::createProxy(*connection, kServiceName, kObjectPath);

    proxy->uponSignal("ModeChanged")
         .onInterface(kInterfaceName)
         .call([](const std::string& mode) {
             g_idle_add(update_icon_idle, g_strdup(mode.c_str()));
         });
    proxy->finishRegistration();

    try {
        std::string initial_mode;
        proxy->callMethod("GetMode")
             .onInterface(kInterfaceName)
             .storeResultsTo(initial_mode);
        set_icon_for_mode(initial_mode.c_str());
    } catch (const sdbus::Error& e) {
        std::cerr << "GetMode failed" << std::endl;
    }

    connection->enterEventLoopAsync();

    gtk_main();

    connection->leaveEventLoop();
    
    return 0;
}