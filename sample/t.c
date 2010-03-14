#include <gtk/gtk.h> 
#include <string.h> 

void on_window_destroy(GtkObject * object, gpointer user_data) { 
    gtk_main_quit(); 
} 

char *old_clipboard = NULL; 
void selection_received(GtkWidget *window, GdkEvent *event) { 
    char *buf; 
    GdkAtom atom; 
    gint format, length; 

    length = gdk_selection_property_get(window->window, (guchar **)&buf, &atom, &format); 
    if(old_clipboard && !strcmp(old_clipboard, buf)) { 
        g_free(buf); 
    } else { 
        char *b = buf; 
        char *c; 
        while(b && (c = strstr(b, "http://"))) { 
            char *p = strpbrk(c, " \t\r\n"); 
            char *url = p ? g_strndup(c, p - c) : g_strdup(c); 
            puts(url); // handle url 
            g_free(url); 
            b = c + 1; 
        } 
        old_clipboard = buf; 
    } 
} 

int clipboard_timeout(void *window) { 
    gtk_selection_convert(GTK_WIDGET(window), GDK_SELECTION_CLIPBOARD, GDK_TARGET_STRING, GDK_CURRENT_TIME); 
} 

int main(int argc, char *argv[]) { 
    GtkBuilder *builder; 

    GtkWidget *window; 

    gtk_init(&argc, &argv); 

    builder = gtk_builder_new(); 
    gtk_builder_add_from_file(builder, "cm.ui", NULL); 

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window1")); 
    gtk_builder_connect_signals(builder, NULL); 
    g_object_unref(G_OBJECT(builder)); 

    gtk_widget_show(window); 

    // Monitor clipboard 
    g_signal_connect(G_OBJECT(window), "selection-received", G_CALLBACK(selection_received), NULL); 
    g_timeout_add(250, clipboard_timeout, window); 

    gtk_main(); 

    return 0; 
}

