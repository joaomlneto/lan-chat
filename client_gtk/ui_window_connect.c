#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>
#include "ui_window_connect.h"

#include "client.h"
#include "ui_window_main.h"

#define DEFAULT_HOST "127.0.0.1"
#define DEFAULT_PORT "64787"

static char str_host[1024] = DEFAULT_HOST;
static char str_port[1024] = DEFAULT_PORT;
static char str_user[1024];
static char str_pass[1024];

void set_hostname_callback(GtkWidget* widget, GtkWidget* entry) {
	const gchar *entry_text = gtk_entry_get_text(GTK_ENTRY(entry));
	printf("Hostname: %s\n", entry_text);
	strncpy(str_host, entry_text, 1024);
}

void set_port_callback(GtkWidget* widget, GtkWidget* entry) {
	const gchar *entry_text = gtk_entry_get_text(GTK_ENTRY(entry));
	printf("Port: %s\n", entry_text);
	strncpy(str_port, entry_text, 1024);
}

void set_user_callback(GtkWidget* widget, GtkWidget* entry) {
	const gchar *entry_text = gtk_entry_get_text(GTK_ENTRY(entry));
	printf("User: %s\n", entry_text);
	strncpy(str_user, entry_text, 1024);
}

void set_pass_callback(GtkWidget* widget, GtkWidget* entry) {
	const gchar *entry_text = gtk_entry_get_text(GTK_ENTRY(entry));
	printf("Pass: %s\n", entry_text);
	strncpy(str_pass, entry_text, 1024);
}

void window_connect_create(GtkWidget* widget, gpointer window) {
	GtkWidget* window_connect;
	GtkWidget* text_host;
	GtkWidget* text_port;
	GtkWidget* text_user;
	GtkWidget* text_pass;
	GtkWidget* button_connect;
	GtkWidget* button_cancel;

	GtkWidget* label_title = gtk_label_new("Server details:");
	GtkWidget* label_host = gtk_label_new("Host");
	GtkWidget* label_port = gtk_label_new("Port");
	GtkWidget* label_user = gtk_label_new("User");
	GtkWidget* label_pass = gtk_label_new("Pass");

	GtkWidget* vbox0;
	GtkWidget* hbox0_0;
	GtkWidget* hbox0_1;
	GtkWidget* hbox0_2;
	GtkWidget* hbox0_3;
	GtkWidget* hbox0_4;

	// create the window
	window_connect = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(window_connect), GTK_WIN_POS_CENTER);
	gtk_window_set_title(GTK_WINDOW(window_connect), "Connect");
	gtk_window_set_resizable(GTK_WINDOW(window_connect), FALSE);
	gtk_container_set_border_width(GTK_CONTAINER(window_connect), 0);

	// configure text boxes
	text_host = gtk_entry_new();
	gtk_entry_set_text(GTK_ENTRY(text_host), DEFAULT_HOST);
	g_signal_connect(text_host, "changed", G_CALLBACK(set_hostname_callback), text_host);

	text_port = gtk_entry_new();
	gtk_entry_set_text(GTK_ENTRY(text_port), DEFAULT_PORT);
	g_signal_connect(text_port, "changed", G_CALLBACK(set_port_callback), text_port);

	text_user = gtk_entry_new();
	g_signal_connect(text_user, "changed", G_CALLBACK(set_user_callback), text_user);

	text_pass = gtk_entry_new();
	gtk_entry_set_visibility(GTK_ENTRY(text_pass), FALSE);
	gtk_entry_set_invisible_char(GTK_ENTRY(text_pass), '#');
	g_signal_connect(text_pass, "changed", G_CALLBACK(set_pass_callback), text_pass);

	// configure buttons
	button_connect = gtk_button_new_with_label("Connect");
	button_cancel = gtk_button_new_with_label("Cancel");

	// configure layout
	vbox0 = gtk_vbox_new(FALSE, 1);
	gtk_container_set_border_width(GTK_CONTAINER(vbox0), 0);
	gtk_container_add(GTK_CONTAINER(window_connect), vbox0);

	hbox0_0 = gtk_hbox_new(FALSE, 1);
	gtk_container_set_border_width(GTK_CONTAINER(hbox0_0), 0);
	gtk_container_add(GTK_CONTAINER(hbox0_0), label_host);
	gtk_container_add(GTK_CONTAINER(hbox0_0), text_host);

	hbox0_1 = gtk_hbox_new(FALSE, 1);
	gtk_container_set_border_width(GTK_CONTAINER(hbox0_1), 0);
	gtk_container_add(GTK_CONTAINER(hbox0_1), label_port);
	gtk_container_add(GTK_CONTAINER(hbox0_1), text_port);

	hbox0_2 = gtk_hbox_new(FALSE, 1);
	gtk_container_set_border_width(GTK_CONTAINER(hbox0_2), 0);
	gtk_container_add(GTK_CONTAINER(hbox0_2), label_user);
	gtk_container_add(GTK_CONTAINER(hbox0_2), text_user);

	hbox0_3 = gtk_hbox_new(FALSE, 1);
	gtk_container_set_border_width(GTK_CONTAINER(hbox0_3), 0);
	gtk_container_add(GTK_CONTAINER(hbox0_3), label_pass);
	gtk_container_add(GTK_CONTAINER(hbox0_3), text_pass);

	hbox0_4 = gtk_hbox_new(FALSE, 1);
	gtk_container_set_border_width(GTK_CONTAINER(hbox0_4), 0);
	gtk_container_add(GTK_CONTAINER(hbox0_4), button_connect);
	gtk_container_add(GTK_CONTAINER(hbox0_4), button_cancel);

	// Put everything together
	gtk_container_add(GTK_CONTAINER(vbox0), label_title);
	gtk_container_add(GTK_CONTAINER(vbox0), hbox0_0);
	gtk_container_add(GTK_CONTAINER(vbox0), hbox0_1);
	gtk_container_add(GTK_CONTAINER(vbox0), hbox0_2);
	gtk_container_add(GTK_CONTAINER(vbox0), hbox0_3);
	gtk_container_add(GTK_CONTAINER(vbox0), hbox0_4);

	// Show everything
	gtk_widget_show_all(window_connect);

}
