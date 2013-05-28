#include <gtk/gtk.h>

void window_about_create(GtkWidget* widget, gpointer window) {
	GdkPixbuf* pixbuf = gdk_pixbuf_new_from_file("icon.png", NULL);
	GtkWidget* dialog = gtk_about_dialog_new();

	gtk_about_dialog_set_name(GTK_ABOUT_DIALOG(dialog), "LAN Party Intracomms");
	gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog), "0.0.1");
	gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog), "(c) João 'anatoly' Neto");
	gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog), "XLPARTY Client");
	gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(dialog), "http://www.sourceforge.net/p/xl-com");
	gtk_about_dialog_set_website_label(GTK_ABOUT_DIALOG(dialog), "Visit the project website on SourceForge");
	gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(dialog), pixbuf);

	g_object_unref(pixbuf);

	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}
