#include <math.h>
#include <gtk/gtk.h>
#include <epoxy/gl.h>

static GtkCssProvider *_css_provider = NULL;
static GdkDisplay *_display = NULL;
static GdkScreen *_screen = NULL;

/* The object we are drawing */
static const GLfloat vertex_data[] = 
{
  0.f,   0.5f,   0.f, 1.f,
  0.5f, -0.366f, 0.f, 1.f,
 -0.5f, -0.366f, 0.f, 1.f,
};

static void init_buffers(GLuint *vao_out, GLuint *buff_out)
{
    //////////////////////////////////////////////////////////////////////////////
    // https://gitlab.gnome.org/GNOME/gtk/-/blob/master/demos/gtk-demo/glarea.c //
    //                                                                          //
    //////////////////////////////////////////////////////////////////////////////

    GLuint vao, buff;

    glGenVertexArrays (1, &vao);
    glBindVertexArray (vao);

    glGenBuffers (1, &buff);
    glBindBuffer (GL_ARRAY_BUFFER, buff);
    glBufferData (GL_ARRAY_BUFFER, sizeof (vertex_data), vertex_data, GL_STATIC_DRAW);
    glBindBuffer (GL_ARRAY_BUFFER, 0);

    if (vao_out != NULL)
    {
        *vao_out = vao;
    }

    if (buff_out != NULL)
    {
        *buff_out = buff;
    }
}

static void unrealize(GtkWidget *gl_area)
{
    
}

static GLuint position_buffer;

static void realize(GtkWidget *gl_area)
{
    GdkGLContext *context;
    gtk_gl_area_make_current (GTK_GL_AREA (gl_area));
    if (gtk_gl_area_get_error (GTK_GL_AREA (gl_area)) != NULL)
        return;
    context = gtk_gl_area_get_context (GTK_GL_AREA (gl_area));

    init_buffers(&position_buffer, NULL);
}

static void render(GtkGLArea *gl_area)
{    
    /////////////////////////////////////////////////
    // https://www.youtube.com/watch?v=Q_kFcRlLTk0 //
    //                                             //
    /////////////////////////////////////////////////
}

static void play(GtkButton *main_button, gpointer main_window)
{
    GList *children, *iter;

    children = gtk_container_get_children(GTK_CONTAINER(main_window));
    for(iter = children; iter != NULL; iter = g_list_next(iter))
    {
        gtk_container_remove(main_window, GTK_WIDGET(iter->data));
    }

    g_list_free(children);

    gtk_style_context_remove_provider_for_screen(_screen, GTK_STYLE_PROVIDER(_css_provider));

    GtkWidget *gl_area = gtk_gl_area_new();
    g_signal_connect(gl_area, "realize", G_CALLBACK(realize), NULL);
    g_signal_connect(gl_area, "render", G_CALLBACK(render), NULL);
    gtk_container_add(GTK_CONTAINER(main_window), gl_area);

    gtk_widget_show_all (GTK_WIDGET(main_window));
}

static void on_activate (GtkApplication *app) 
{
    GtkWidget *window;
    GtkWidget *button_play, *button_quit;
    GtkWidget *buttons_box;

    GtkCssProvider *css_provider = gtk_css_provider_new();
    GdkDisplay *display = gdk_display_get_default();
    GdkScreen *screen = gdk_display_get_default_screen (display);

    _css_provider = css_provider;
    _display = display;
    _screen = screen;

    gtk_css_provider_load_from_path(css_provider, "style.css", NULL);

    gtk_style_context_add_provider_for_screen(
        screen,
        GTK_STYLE_PROVIDER(css_provider),
        GTK_STYLE_PROVIDER_PRIORITY_USER
    );

    window = gtk_application_window_new(app);
    gtk_widget_set_name(GTK_WIDGET(window), "main_window");
    gtk_window_set_title(GTK_WINDOW(window), "CPong");
    gtk_window_set_default_size (GTK_WINDOW (window), 1280, 720);

    buttons_box = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);
    gtk_widget_set_name(buttons_box, "buttons_box");
    gtk_container_add (GTK_CONTAINER (window), buttons_box);
    gtk_widget_set_valign(buttons_box, GTK_ALIGN_CENTER);
    gtk_widget_set_halign(buttons_box, GTK_ALIGN_CENTER);

    button_play = gtk_button_new_with_label ("PLAY");
    gtk_widget_set_size_request(button_play, 350, 135);
    g_signal_connect(button_play, "clicked", G_CALLBACK(play), (gpointer) window);
    gtk_container_add (GTK_CONTAINER (buttons_box), button_play);
    gtk_button_set_relief(GTK_BUTTON(button_play), GTK_RELIEF_NONE);

    button_quit = gtk_button_new_with_label("QUIT");
    gtk_widget_set_size_request(button_quit, 350, 135);
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