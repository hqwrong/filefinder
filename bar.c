#include <gtk/gtk.h>

GtkListStore *Store;

void *
show_menu(GtkWidget * window) {
    GtkListStore *store;
    GtkTreeIter iter;
    GtkWidget * tree;
    GtkTreeViewColumn *column;
    GtkCellRenderer *renderer;
    GtkWidget * vbox;

    vbox = g_list_nth_data(gtk_container_get_children(GTK_CONTAINER(window)), 0);
    
    store = gtk_list_store_new (1, G_TYPE_STRING);
    Store = store;

    /* Append one word */
    gtk_list_store_append (store, &iter);
    gtk_list_store_set (store, &iter, 0, "GNOME", -1);

    /* Append another word */
    gtk_list_store_append (store, &iter);
    gtk_list_store_set (store, &iter, 0, "total", -1);

    /* And another word */
    gtk_list_store_append (store, &iter);
    gtk_list_store_set (store, &iter, 0, "totally", -1);

    tree = gtk_tree_view_new_with_model (GTK_TREE_MODEL (store));
    gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(tree), FALSE);
    g_object_unref (G_OBJECT (store));

    renderer = gtk_cell_renderer_text_new ();
    g_object_set(G_OBJECT (renderer), "foreground", "red", "background", "blue", NULL);
    
    column = gtk_tree_view_column_new_with_attributes (NULL, renderer,
                                                       "text", 0, NULL);
    gtk_tree_view_append_column (GTK_TREE_VIEW (tree), column);
    
    gtk_box_pack_start(GTK_BOX (vbox), tree, FALSE, FALSE, 0);
    gtk_widget_show(tree);
}

static void
changed_cb(GtkWidget * entry, GtkWidget * window) {
    GtkTreeIter iter;
    GtkEntryBuffer * buf;
    GtkListStore * store;

    store = Store;
    gtk_list_store_clear(store);
    
    gtk_list_store_append (store, &iter);
    buf = gtk_entry_get_buffer(GTK_ENTRY(entry));
    gtk_list_store_set(store, &iter, 0, gtk_entry_buffer_get_text(buf), -1);

    gtk_widget_show(window);
}

GtkWidget *
new_bar() {
    GtkWidget * window;
    GtkWidget * vbox;
    GtkWidget * entry;
    
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    
    vbox = gtk_vbox_new(FALSE, 1);
    gtk_container_add(GTK_CONTAINER (window), vbox);
    gtk_widget_show(vbox);
    
    entry = gtk_entry_new();
    g_signal_connect(entry, "changed", G_CALLBACK(changed_cb), window);
    gtk_box_pack_start(GTK_BOX (vbox), entry, FALSE, FALSE, 0);
    gtk_widget_show(entry);

    show_menu(window);
    return window;
}

void *
show_bar(GtkWidget * window) {
    gtk_widget_show(window);    
}

int
main(int argc, char * argv[]) {
    GtkWidget * window;
    
    gtk_init(&argc, &argv);
    
    window = new_bar();
    show_bar(window);
    
    gtk_main();
}
