#include <gtk/gtk.h>
#include "ui_window_main.h"

#include "ui_menu.h"
#include "ui_toolbar.h"

/*
 * Callback functions
 */
gboolean delete_event(GtkWidget *widget, GdkEvent *event, gpointer data) {
	g_print("Delete event occurred.\n");
	return FALSE; // FALSE will destroy the window.
}

void destroy(GtkWidget *widget, gpointer data) {
	gtk_main_quit();
}

/*
 * Function to create a pixel buffer from png file
 */
GdkPixbuf* create_pixbuf(const gchar* filename) {
	GdkPixbuf* pixbuf;
	GError *error = NULL;
	pixbuf = gdk_pixbuf_new_from_file(filename, &error);
	if(!pixbuf) {
		fprintf(stderr, "%s\n", error->message);
		g_error_free(error);
	}
	return pixbuf;
}

/*
 * Main window function
 */
void window_main_create() {

	GtkWidget *window;
	GtkWidget *vbox;
	GtkWidget *toolbar;
	GtkWidget *menubar;
	GtkWidget *log_scroll;
	GtkWidget *log_text;
	GtkTextBuffer *log_buffer;

	// Initialize window
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
	gtk_window_set_title(GTK_WINDOW(window), "XLPARTY Intracomms Client");
	gtk_window_set_icon(GTK_WINDOW(window), create_pixbuf("icon.png"));
	g_signal_connect(window, "delete-event", G_CALLBACK(delete_event), NULL);
	g_signal_connect(window, "destroy", G_CALLBACK(destroy), NULL);
	gtk_container_set_border_width(GTK_CONTAINER(window), 0);

	// Make a vbox to put everything
	vbox = gtk_vbox_new(FALSE, 1);
	gtk_container_set_border_width(GTK_CONTAINER(vbox), 0);
	gtk_container_add(GTK_CONTAINER(window), vbox);

	// Get window components
	toolbar = get_toolbar(window);
	menubar = get_menubar_menu(window);

	// Create vertical-scrolling window for log textarea
	log_scroll = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(log_scroll), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_container_set_resize_mode(GTK_CONTAINER(log_scroll), GTK_RESIZE_PARENT);

	// Create logging textarea
	log_text = gtk_text_view_new();
	log_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(log_text));
	gtk_text_view_set_editable(GTK_TEXT_VIEW(log_text), FALSE);
	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(log_text), GTK_WRAP_WORD);
	gtk_container_add(GTK_CONTAINER(log_scroll), log_text);

	/* Pack it all together */
	gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), toolbar, FALSE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), log_scroll, TRUE, TRUE, 0);

	// Show everything
	gtk_widget_show_all(window);

}
