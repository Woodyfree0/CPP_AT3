// client_gui.c
#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

GtkWidget *entry;
GtkWidget *text_view;

int client_socket;

void connect_to_server(GtkWidget *widget, gpointer data) {
    // Connect to the server
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(1234); // Change port accordingly
    inet_pton(AF_INET, "127.0.0.1", &(server_address.sin_addr));

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address));

    gtk_text_buffer_insert_at_cursor(gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view)), "Connected to server\n", -1);
}

void send_message(GtkWidget *widget, gpointer data) {
    // Send a message to the server
    const char *message = gtk_entry_get_text(GTK_ENTRY(entry));
    send(client_socket, message, strlen(message), 0);
    gtk_entry_set_text(GTK_ENTRY(entry), "");
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window, *grid, *button_connect, *button_send;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    text_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_grid_attach(GTK_GRID(grid), text_view, 0, 0, 2, 1);

    entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry, 0, 1, 1, 1);

    button_connect = gtk_button_new_with_label("Connect");
    g_signal_connect(button_connect, "clicked", G_CALLBACK(connect_to_server), NULL);
    gtk_grid_attach(GTK_GRID(grid), button_connect, 1, 1, 1, 1);

    button_send = gtk_button_new_with_label("Send");
    g_signal_connect(button_send, "clicked", G_CALLBACK(send_message), NULL);
    gtk_grid_attach(GTK_GRID(grid), button_send, 0, 2, 2, 1);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
