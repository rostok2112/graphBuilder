#include <types.h>
#include <glib.h>
#include <gtk/gtk.h>

G_MODULE_EXPORT void onBtnBuildGraphClicked(GtkButton * btnDrawGraph, GtkDialog * dlgDrawGraph)
{
    GtkWidget * dlgParent = btnDrawGraph;

    while(dlgParent && !GTK_IS_WINDOW(dlgParent))
        dlgParent =  gtk_widget_get_parent(dlgParent);

    GList * listChilds = gtk_container_get_children(GTK_CONTAINER(GTK_BOX(gtk_bin_get_child(GTK_BIN(dlgParent)))));

    while(listChilds && !GTK_IS_GRID(listChilds->data))
        listChilds = listChilds->next;

    listChilds =  gtk_container_get_children(GTK_CONTAINER(listChilds->data));

    while(listChilds && !GTK_IS_TEXT_VIEW(listChilds->data))
        listChilds = listChilds->next;

    GtkTextView *formFunc = listChilds->data;
    GtkTextBuffer *formFuncBuf = gtk_text_view_get_buffer(formFunc);
    GtkTextIter start, end;
    gtk_text_buffer_get_start_iter(formFuncBuf, &start);
    gtk_text_buffer_get_end_iter(formFuncBuf, &end);
    guchar *funcStr = g_locale_from_utf8(gtk_text_buffer_get_text(formFuncBuf, &start, &end, FALSE), -1, NULL, NULL, NULL);

    if(funcStr)
        if(*funcStr){
            listChilds = gtk_container_get_children(GTK_CONTAINER(GTK_BOX(gtk_bin_get_child(GTK_BIN(dlgDrawGraph)))));
            while(listChilds && !GTK_IS_DRAWING_AREA(listChilds->data))
                listChilds = listChilds->next;
            GtkDrawingArea *drawArea = listChilds->data;

            drawingAreaDrawGraph(drawArea, funcStr);
            gtk_widget_set_visible(dlgDrawGraph, true);
        }
}

G_MODULE_EXPORT void onDrawOnDrawArea(GtkDrawingArea *drawArea,cairo_t *cr)
{
    guint width, height;
    GdkRGBA color;
    GtkStyleContext *context;
    guchar *funcStr = g_object_get_data(drawArea, "funcStr");

    context=gtk_widget_get_style_context(drawArea);
    width=gtk_widget_get_allocated_width(drawArea);
    height=gtk_widget_get_allocated_height(drawArea);
    gtk_render_background(context,cr,0,0,width,height);

    gtk_style_context_get_color(context,gtk_style_context_get_state
                                (context),&color);

    gdk_rgba_parse(&color, "rgb(255, 255, 255)");
    cairo_set_line_width (cr, 1.0);
    cairo_move_to(cr, width/2.0, 0);
    cairo_line_to(cr, width/2.0, height);
    cairo_move_to(cr, 0, height/2.0);
    cairo_line_to(cr, width, height/2.0);
    cairo_stroke (cr);

    double x = 0.0;
    double y = calculateFromStr(x, funcStr);


    gdk_rgba_parse(&color, "rgba(255, 0, 0, 0.5)");
    gdk_cairo_set_source_rgba(cr,&color);

    for(x = 0; x > -(width/2.0); x -= 0.1){
        y = calculateFromStr(x, CONTINUE_WITH_CACHE);
            cairo_line_to(cr,  x+ width/2.0, height/2.0 - y);
    }
    cairo_move_to(cr, width/2.0, height/2.0);
    cairo_line_to(cr, width/2.0, height/2.0);
    for(x = 0; x < width/2.0; x += 0.1){
        y = calculateFromStr(x , CONTINUE_WITH_CACHE);
            cairo_line_to(cr,  x + width/2.0, height/2.0 - y);
    }

    cairo_stroke (cr);
    calculateFromStr(0.0, FREE_CACHE);
}

G_MODULE_EXPORT void closeWindow(GtkWidget *widget, GtkWidget *window){
    gtk_widget_set_visible(window, false);
}


G_MODULE_EXPORT void onHideDrawingDlg(GtkDialog *drawingDlg, GtkDrawingArea *drawArea){
    if(g_object_get_data(drawArea, "funcStr"))
        g_free(g_object_steal_data(drawArea, "funcStr"));
}
