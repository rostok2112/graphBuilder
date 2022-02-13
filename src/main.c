#include <stdio.h>
#include <locale.h>
#include <types.h>

GtkBuilder *builder;
GtkWidget *window;

int main(int argc, char * argv[])
{
    setlocale(LC_ALL, "ukr");
    #if defined(_WIN32) || defined(WIN32)
        if(system("chcp 1251"));
    #endif
    gtk_init(&argc, &argv);
    createWin (&window, "winMain", "resource/gtk_widgets/winMain.glade");
    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
