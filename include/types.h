#ifndef TYPES_H_INCLUDED
#define TYPES_H_INCLUDED

#include <stdlib.h>
#include <stdbool.h>
#include <glib.h>
#include <gtk/gtk.h>
#include <calc.h>

extern GtkBuilder *builder;
extern GtkWidget *window;
extern GtkDialog *dialog;

void drawingAreaDrawGraph(GtkDrawingArea *, guchar []);
void createWin(GtkWidget **, guchar [], guchar []);

G_MODULE_EXPORT void onBtnBuildGraphClicked(GtkButton * btnDrawGraph, GtkDialog * dlgDrawGraph);
G_MODULE_EXPORT void onDrawOnDrawArea(GtkDrawingArea *drawArea, cairo_t *cr);
G_MODULE_EXPORT void onBtnScreenshotClicked(GtkButton *btn, GtkDrawingArea *drawArea);
G_MODULE_EXPORT void onHideDrawingDlg(GtkDialog *drawingDlg, GtkDrawingArea *drawArea);

#endif // TYPES_H_INCLUDED
