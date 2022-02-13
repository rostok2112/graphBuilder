#include <types.h>
#include <glib.h>
#include <gtk/gtk.h>
#include <time.h>


G_MODULE_EXPORT void onBtnBuildGraphClicked(GtkButton * btnDrawGraph, GtkDialog * dlgDrawGraph)
{
    GtkWidget * dlgParent = GTK_WIDGET(btnDrawGraph);

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
            GtkDrawingArea *drawArea = GTK_DRAWING_AREA(listChilds->data);

            drawingAreaDrawGraph(drawArea, funcStr);
            gtk_widget_set_visible(GTK_WIDGET(dlgDrawGraph), true);
        }
}

G_MODULE_EXPORT void onDrawOnDrawArea(GtkDrawingArea *drawArea, cairo_t *cr)
{
    guint width, height;
    GdkRGBA color;
    GtkStyleContext *context;
    guchar *funcStr = g_object_get_data(G_OBJECT(drawArea), "funcStr");

    context=gtk_widget_get_style_context(GTK_WIDGET(drawArea));
    width=gtk_widget_get_allocated_width(GTK_WIDGET(drawArea));
    height=gtk_widget_get_allocated_height(GTK_WIDGET(drawArea));
    gtk_render_background(context,cr,0,0,width,height);

    gtk_style_context_get_color(context,gtk_style_context_get_state
                                (context),&color);

    /* cord lines */
    gdk_rgba_parse(&color, "rgb(255, 255, 255)");
    cairo_set_line_width (cr, 1.0);
    cairo_move_to(cr, width/2.0, 0);
    cairo_line_to(cr, width/2.0, height);
    cairo_move_to(cr, 0, height/2.0);
    cairo_line_to(cr, width, height/2.0);
    cairo_stroke (cr);

    double x = width/2.0 - 10.0;
    double y = 0.0;

    #define FONT_SIZE 14.0
    #define ONE_CENTIMETER 40.0
    #define CORD_DASH_LENGTH 20.0
    cairo_set_font_size (cr, FONT_SIZE);
    cairo_select_font_face (cr, "Georgia", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    guchar * buffer = NULL;

    /* y plus cord dashes and marks */
    for(int i = 0, y = height/2.0 ; y < height; y+= ONE_CENTIMETER, i++)
        if(i) {
            buffer = g_strdup_printf("%d", i);
            cairo_move_to(cr, x - ((strlen(buffer))*FONT_SIZE)/2.0, height - y);
            cairo_show_text(cr, buffer);
            g_free(buffer);
            cairo_rel_line_to(cr, CORD_DASH_LENGTH, 0.0);
        }

    cairo_move_to(cr, x + CORD_DASH_LENGTH, FONT_SIZE); // y plus cords pointer
    buffer = g_strdup_printf("%c", 'y');
    cairo_show_text(cr, buffer);
    g_free(buffer);

    cairo_stroke (cr);
    /* y minus cord dashes and marks */
    for(int i = 0, y = height/2.0; y > 0; y-= ONE_CENTIMETER, i--)
        if(i) {
            buffer = g_strdup_printf("%d", i);
            cairo_move_to(cr, x - (strlen(buffer)*FONT_SIZE)/2.0, height - y);
            cairo_show_text(cr, buffer);
            g_free(buffer);
            cairo_rel_line_to(cr, CORD_DASH_LENGTH, 0.0);
        }

    cairo_stroke (cr);

    x = 0.0;
    y = height/2.0 - 10.0;

    /* x minus cord dashes and marks */
    for(int i = 0, x = width/2.0; x < width; x+= ONE_CENTIMETER, i--)
        if(i) {
            cairo_move_to(cr, width - x, y);
            cairo_rel_line_to(cr, 0.0, CORD_DASH_LENGTH);

            buffer = g_strdup_printf("%d", i);
            cairo_rel_move_to(cr, 0.0, FONT_SIZE);
            cairo_show_text(cr, buffer);
            g_free(buffer);
        }
    /* x plus cord dashes and marks */
    for(int i = 0, x = width/2.0; x > 0; x-= ONE_CENTIMETER, i++)
        if(i) {
            cairo_move_to(cr, width - x, y);
            cairo_rel_line_to(cr, 0.0, CORD_DASH_LENGTH);

            buffer = g_strdup_printf("%d", i);
            cairo_rel_move_to(cr, 0.0, FONT_SIZE);
            cairo_show_text(cr, buffer);
            g_free(buffer);
        }

    cairo_move_to(cr, width - FONT_SIZE, y);  // x plus cords pointer
    buffer = g_strdup_printf("%c", 'x');
    cairo_show_text(cr, buffer);
    g_free(buffer);

    buffer = g_strdup_printf("%d", 0);
    cairo_move_to(cr, width/2 - FONT_SIZE,  height/2 + FONT_SIZE); // zero middle of cords
    cairo_show_text(cr, buffer);
    g_free(buffer);

    cairo_stroke (cr);
    #undef FONT_SIZE
    #undef ONE_CENTIMETER
    #undef CORD_DASH_LENGTH

    x = 0.0;
    y = calculateFromStr(x, funcStr);

    gdk_rgba_parse(&color, "rgba(255, 0, 0, 0.5)");
    gdk_cairo_set_source_rgba(cr,&color);

    /* x plus graph part */
    for(x = 0; x > -(width/2.0); x -= 0.05){
        y = calculateFromStr(x, CONTINUE_WITH_CACHE);
        if(height/2.0 - y < height && height/2.0 - y > 0 && x + width/2.0 < width && x + width/2.0 > 0)
            cairo_arc(cr, 40*x + width/2.0, height/2.0 - 40*y, 1.0, 0.0, 2.0*G_PI);
    }
    cairo_stroke (cr);
    /* x minus graph part */
    for(x = 0; x < width/2.0; x += 0.05){
        y = calculateFromStr(x , CONTINUE_WITH_CACHE);
        if(height/2.0 - y < height && height/2.0 - y > 0 && x + width/2.0 < width && x + width/2.0 > 0)
            cairo_arc(cr, 40*x + width/2.0, height/2.0 - 40*y, 1.0, 0.0, 2.0*G_PI);
    }

    cairo_stroke (cr);
    calculateFromStr(0.0, FREE_CACHE);

    if(g_object_steal_data(G_OBJECT(drawArea), "isNeedScreenshot")) { // if pressed screenshot button
        if(system("mkdir screenshots"));
        buffer = g_strdup_printf("screenshots/screenshot_%lld.png", (unsigned long long)time(NULL));
        cairo_surface_write_to_png(cairo_get_target(cr), buffer);
        free(buffer);

    }
}

G_MODULE_EXPORT void onBtnScreenshotClicked(GtkButton *btn, GtkDrawingArea *drawArea)
{
    g_object_set_data(G_OBJECT(drawArea), "isNeedScreenshot", GINT_TO_POINTER(1));
}

G_MODULE_EXPORT void closeWindow(GtkWidget *widget, GtkWidget *window)
{
    gtk_widget_set_visible(window, false);
}

G_MODULE_EXPORT void onHideDrawingDlg(GtkDialog *drawingDlg, GtkDrawingArea *drawArea)
{
    if(g_object_get_data(G_OBJECT(drawArea), "funcStr"))
        g_free(g_object_steal_data(G_OBJECT(drawArea), "funcStr"));
}


