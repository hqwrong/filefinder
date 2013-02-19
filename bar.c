#include <gtk/gtk.h>

#define ITEM_SIZE 6
typedef int (*UPDATE_CB)(const char * content, char ** item) ;
typedef void (*COMMIT_CB)(const char * content) ;

static UPDATE_CB update_cb;
static COMMIT_CB commit_cb;

void *
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
    gtk_tree_model_get_iter_first(model, &iter);

    char *content;
    gtk_tree_model_get(model, &iter, 0, &content, -1);
    commit_cb(content);         /* todo free */
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
        }
        /* gtk_widget_show(GTK_WIDGET(tree)); */
    }
}

GtkWidget *
new_bar() {
    GtkWidget * window = gtk_window_new(GTK_WINDOW_TOPLEVEL);;
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
    
    GtkWidget * vbox = gtk_vbox_new(FALSE, 1);
    gtk_container_add(GTK_CONTAINER (window), vbox);
    gtk_widget_show(vbox);

    GtkListStore * store = gtk_list_store_new (1, G_TYPE_STRING);

    GtkWidget * tree = gtk_tree_view_new_with_model(GTK_TREE_MODEL (store));
    g_signal_connect(tree, "row-activated", G_CALLBACK(row_activate_cb), NULL);
    gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(tree), FALSE);
    g_object_unref(G_OBJECT (store));

    GtkCellRenderer *renderer = gtk_cell_renderer_text_new ();
    g_object_set(G_OBJECT (renderer), "foreground", "green", NULL);
    
    GtkTreeViewColumn * column = gtk_tree_view_column_new_with_attributes(NULL, renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW (tree), column);

    GtkWidget * entry = gtk_entry_new();
    g_signal_connect(entry, "changed", G_CALLBACK(changed_cb), tree);
    g_signal_connect(entry, "activate",G_CALLBACK(activate_cb), tree);

    gtk_box_pack_start(GTK_BOX (vbox), entry, FALSE, FALSE, 0);
    gtk_widget_show(entry);
    gtk_box_pack_start(GTK_BOX(vbox), tree, FALSE, FALSE, 0);
    gtk_widget_show(tree);        
    return window;
}

void *
show_bar(GtkWidget * window) {
    gtk_widget_show(window);    
}

void *
bar_set_update_cb(UPDATE_CB cb) {
    update_cb = cb;
}

void *
bar_set_commit_cb(COMMIT_CB cb) {
    commit_cb = cb;
}

/* int */
/* main(int argc, char * argv[]) { */
/*     GtkWidget * window; */
    
/*     gtk_init(&argc, &argv); */
    
/*     window = new_bar(); */
/*     show_bar(window); */
    
/*     gtk_main(); */
/* } */

void
bar_main() {
    GtkWidget * window = new_bar();
    show_bar(window);
    
    gtk_main();
}

void
bar_main_quit() {
    gtk_main_quit();
}
