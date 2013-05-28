#include <gtk/gtk.h>
#include "ui_menu.h"

#include "ui_window_about.h"
#include "ui_window_status.h"
#include "ui_window_connect.h"
#include "ui_window_status.h"


static GtkItemFactoryEntry menu_items[] = {
	{"/_Connection",            NULL,      NULL,                  0, "<Branch>" },
	{"/Connection/_Connect",    "<CTRL>C", window_connect_create, 0, "<Item>"   },
	{"/Connection/_Disconnect", NULL,      NULL,                  0, "<Item>"   },
	{"/Connection/_Status",     NULL,      window_status_create,  0, "<Item>"   },
	{"/_Help",                  NULL,      NULL,                  0, "<Branch>" },
	{"/Help/_About",            NULL,      window_about_create,   0, "<Item>"   },
};

GtkWidget* get_menubar_menu(GtkWidget *window) {
	GtkItemFactory *item_factory;
	GtkAccelGroup *accel_group;
	gint num_menu_items = sizeof(menu_items)/sizeof(menu_items[0]);

	/* Make an accelerator group (shortcut keys) */
	accel_group = gtk_accel_group_new();

	/* Make an ItemFactory and create items for it */
	item_factory = gtk_item_factory_new(GTK_TYPE_MENU_BAR, "<main>", accel_group);
	gtk_item_factory_create_items(item_factory, num_menu_items, menu_items, NULL);

	/* Attach the new accelerator group to the window */
	gtk_window_add_accel_group(GTK_WINDOW(window), accel_group);

	/* Finally return the actual menu bar created by the item factory */
	return gtk_item_factory_get_widget(item_factory, "<main>");

}
