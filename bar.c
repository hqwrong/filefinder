#include <gtk/gtk.h>
#include "bar.h"

#define ITEM_SIZE 6

static UPDATE_CB update_cb;
static COMMIT_CB commit_cb;

static void *
row_activate_cb(GtkTreeView * tree, GtkTreePath *path) {
    GtkTreeIter iter;
    GtkTreeModel *model = gtk_tree_view_get_model(tree);
    char * content;
    
    gtk_tree_model_get_iter(model, &iter, path);
    gtk_tree_model_get(model, &iter, 0, &content, -1);
    commit_cb(content);         /* todo: free */
}

static void
activate_cb(GtkEntry * entry, GtkTreeView * tree) {
    GtkTreeModel *model = gtk_tree_view_get_model(tree);
    GtkTreeIter iter;

    if (gtk_tree_model_get_iter_first(model, &iter)) {
        char *content;
        gtk_tree_model_get(model, &iter, 0, &content, -1);
        commit_cb(content);         /* todo free */
    } else {
        commit_cb(gtk_entry_get_text(entry));
    }
}

static void
changed_cb(GtkWidget * entry, GtkTreeView * tree) {
    if (update_cb != NULL) {
        GtkListStore * store = GTK_LIST_STORE(gtk_tree_view_get_model(tree));
        char * item[ITEM_SIZE];
        GtkTreeIter iter;
            
        gtk_list_store_clear(store);
            
        GtkEntryBuffer * buf = gtk_entry_get_buffer(GTK_ENTRY(entry));
        int sz = update_cb(gtk_entry_buffer_get_text(buf), item);
        for (int i=0;i<sz;i++) {
            gtk_list_store_append (store, &iter);
            gtk_list_store_set(store, &iter, 0, item[i], -1);
            gtk_list_store_set(store, &iter, 1, "file", -1);
        }

        GtkWidget *window = gtk_widget_get_toplevel(GTK_WIDGET(tree));
        gtk_window_resize(GTK_WINDOW(window), 1, 1);
    }
}

GtkWidget *
new_bar() {
    GtkWidget * window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
    gtk_window_set_resizable(GTK_WINDOW(window), TRUE);
    gtk_window_resize(GTK_WINDOW(window), 500, 1);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    
    GtkWidget * vbox = gtk_vbox_new(FALSE, 1);
    gtk_container_add(GTK_CONTAINER (window), vbox);

    GtkListStore * store = gtk_list_store_new (2, G_TYPE_STRING, G_TYPE_STRING);

    GtkWidget * tree = gtk_tree_view_new_with_model(GTK_TREE_MODEL (store));
    g_signal_connect(tree, "row-activated", G_CALLBACK(row_activate_cb), NULL);
    gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(tree), FALSE);
    g_object_unref(G_OBJECT (store));

    GtkCellRenderer *renderer = gtk_cell_renderer_text_new ();
    g_object_set(G_OBJECT (renderer), "foreground", "green", NULL);
    
    GtkTreeViewColumn * column = gtk_tree_view_column_new_with_attributes(NULL, renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW (tree), column);

    renderer = gtk_cell_renderer_text_new ();
    g_object_set(G_OBJECT (renderer), "foreground", "blue", NULL);
    column = gtk_tree_view_column_new_with_attributes(NULL, renderer, "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW (tree), column);


    GtkWidget * entry = gtk_entry_new();
    g_signal_connect(entry, "changed", G_CALLBACK(changed_cb), tree);
    g_signal_connect(entry, "activate",G_CALLBACK(activate_cb), tree);

    gtk_box_pack_start(GTK_BOX (vbox), entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), tree, TRUE, TRUE, 0);
    gtk_widget_show_all(window);
    return window;
}

void
bar_set_update_cb(UPDATE_CB cb) {
    update_cb = cb;
}

void
bar_set_commit_cb(COMMIT_CB cb) {
    commit_cb = cb;
}

void
bar_main() {
    new_bar();
    gtk_main();
}

void
bar_main_quit() {
    gtk_main_quit();
}
