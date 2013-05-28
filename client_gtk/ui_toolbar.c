#include <gtk/gtk.h>
#include "ui_toolbar.h"

#include "ui_window_connect.h"

typedef struct {
	char* label;
	char* tooltip;
	char* tooltip_private;
	char* icon_widget;
	GtkWidget* iconw;
	void (*callback)(GtkWidget* widget, gpointer data);
	void* data;
} toolbar_button_info;

GtkWidget* get_toolbar(GtkWidget *window) {
	GtkWidget* toolbar;

	GtkToolItem* connect;
	GtkToolItem* disconnect;
	GtkToolItem* preferences;

	toolbar = gtk_toolbar_new();
	gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_ICONS);
	gtk_container_set_border_width(GTK_CONTAINER(toolbar), 0);

	connect = gtk_tool_button_new_from_stock(GTK_STOCK_CONNECT);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), connect, -1);

	disconnect = gtk_tool_button_new_from_stock(GTK_STOCK_DISCONNECT);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), disconnect, -1);

	preferences = gtk_tool_button_new_from_stock(GTK_STOCK_PREFERENCES);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), preferences, -1);

	return toolbar;
}
