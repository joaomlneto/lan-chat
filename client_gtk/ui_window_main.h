#ifndef _UI_WINDOW_MAIN_H
#define _UI_WINDOW_MAIN_H

gboolean delete_event(GtkWidget *widget, GdkEvent *event, gpointer data);
void destroy(GtkWidget* widget, gpointer data);
void window_main_create();

#endif
