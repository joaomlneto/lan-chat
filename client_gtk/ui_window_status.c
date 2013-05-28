#include <gtk/gtk.h>
#include "ui_window_status.h"

#include "client.h"
#include "ui_window_main.h"

void window_status_create(GtkWidget* widget, gpointer window) {
	GtkWidget* window_connect;
	GtkWidget* button_ok;

	GtkWidget* label_title = gtk_label_new("Connection Status");
	GtkWidget* label_host = gtk_label_new("Server Address");
	GtkWidget* label_ping = gtk_label_new("Ping");

	GtkWidget* value_host = gtk_label_new("Not connected");
	GtkWidget* value_ping = gtk_label_new("Not connected");

	GtkWidget* vbox0;
	GtkWidget* hbox0; // host
	GtkWidget* hbox1; // ping

	// create the window
	window_connect = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(window_connect), GTK_WIN_POS_CENTER);
	gtk_window_set_title(GTK_WINDOW(window_connect), "Connect");
	gtk_window_set_resizable(GTK_WINDOW(window_connect), FALSE);
	gtk_container_set_border_width(GTK_CONTAINER(window_connect), 0);

	// configure buttons
	button_ok = gtk_button_new_with_label("Okay");

	// configure layout
	vbox0 = gtk_vbox_new(FALSE, 1);
	gtk_container_set_border_width(GTK_CONTAINER(vbox0), 0);
	gtk_container_add(GTK_CONTAINER(window_connect), vbox0);

	hbox0 = gtk_hbox_new(FALSE, 1);
	gtk_container_set_border_width(GTK_CONTAINER(hbox0), 0);
	gtk_container_add(GTK_CONTAINER(hbox0), label_host);
	gtk_container_add(GTK_CONTAINER(hbox0), value_host);

	hbox1 = gtk_hbox_new(FALSE, 1);
	gtk_container_set_border_width(GTK_CONTAINER(hbox1), 0);
	gtk_container_add(GTK_CONTAINER(hbox1), label_ping);
	gtk_container_add(GTK_CONTAINER(hbox1), value_ping);

	// Put everything together
	gtk_container_add(GTK_CONTAINER(vbox0), label_title);
	gtk_container_add(GTK_CONTAINER(vbox0), hbox0);
	gtk_container_add(GTK_CONTAINER(vbox0), hbox1);

	// Show everything
	gtk_widget_show_all(window_connect);

}
