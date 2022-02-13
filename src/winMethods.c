#include <types.h>
#include <cairo.h>

void drawingAreaDrawGraph(GtkDrawingArea *drawArea, guchar funcStr[]){
    if(funcStr)
        if(*funcStr) {
            guchar *bufStr = strdup(funcStr);
            gtk_widget_set_size_request(GTK_WIDGET(drawArea), 200, 200);
            g_object_set_data(G_OBJECT(drawArea), "funcStr", bufStr);
        }
}


void createWin(GtkWidget **window, guchar id [], guchar pathToBuilder [])
{
    GError *error = NULL;

    builder = gtk_builder_new ();
    if (!gtk_builder_add_from_file (builder, pathToBuilder, &error)) {

        g_critical ("Can't open file: %s", error->message);
        g_error_free(error);
    }

    gtk_builder_connect_signals (builder, NULL);

    *window = GTK_WIDGET (gtk_builder_get_object (builder, id));
    if (!(*window) )
        g_critical ("Erron in fetching window");
}


