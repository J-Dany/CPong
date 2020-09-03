#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <gtk/gtk.h>


static void on_activate (GtkApplication *app) 
{
    GtkWidget *window;
    GtkWidget *button_play, *button_quit;
    GtkWidget *buttons_box;

    GtkCssProvider *css_provider = gtk_css_provider_new();

    gtk_css_provider_load_from_path(css_provider, "./style.css", NULL);

    gtk_style_context_add_provider_for_screen(
        gdk_display_get_default_screen (gdk_display_get_default()),
        GTK_STYLE_PROVIDER(css_provider),
        GTK_STYLE_PROVIDER_PRIORITY_THEME
    );

    window = gtk_application_window_new(app);
    gtk_widget_set_name(GTK_WIDGET(window), "main_window");
    gtk_window_set_title(GTK_WINDOW(window), "CPong");
    gtk_window_set_default_size (GTK_WINDOW (window), 1280, 720);
    
    GdkRGBA color = 
    {
        .alpha =   0,
        .blue  = 1.0,
        .green = 1.0,
        .red   = 1.0
    };
    gtk_widget_override_background_color(GTK_WIDGET(window), GTK_WINDOW_TOPLEVEL, &color);

    buttons_box = gtk_button_box_new (GTK_ORIENTATION_VERTICAL);
    gtk_widget_set_name(buttons_box, "buttons_box");
    gtk_container_add (GTK_CONTAINER (window), buttons_box);
    gtk_widget_set_valign(buttons_box, GTK_ALIGN_CENTER);

    button_play = gtk_button_new_with_label ("Play");
    gtk_widget_set_size_request(button_play, 350, 150);
    gtk_container_add (GTK_CONTAINER (buttons_box), button_play);
    gtk_button_set_relief(GTK_BUTTON(button_play), GTK_RELIEF_NONE);
    gtk_widget_set_opacity(GTK_WIDGET(button_play), 1);

    button_quit = gtk_button_new_with_label("Quit");
    gtk_widget_set_size_request(button_quit, 350, 150);
    g_signal_connect(button_quit, "clicked", G_CALLBACK(gtk_window_close), NULL);
    gtk_container_add(GTK_CONTAINER(buttons_box), button_quit);
    gtk_button_set_relief(GTK_BUTTON(button_quit), GTK_RELIEF_NONE);

    g_object_unref(css_provider);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    gtk_widget_show_all (window);
}

int main(int argc, char *argv[])
{
    GtkApplication *app = gtk_application_new ("com.example.GtkApplication", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (on_activate), NULL);

    return g_application_run (G_APPLICATION (app), argc, argv);
}