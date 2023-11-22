#include <gtk/gtk.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "client.h"

GtkWidget *entry;

void send_data(const char *data) {
    int client_socket;
    struct sockaddr_in server_address;

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // Send data to the server
    write(client_socket, data, strlen(data));

    close(client_socket);
}

static void send_button_clicked(GtkWidget *widget, gpointer user_data) {
    const char *text = gtk_entry_get_text(GTK_ENTRY(entry));
    send_data(text);
}

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *send_button;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Client");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Enter text to send");
    gtk_grid_attach(GTK_GRID(grid), entry, 0, 0, 1, 1);

    send_button = gtk_button_new_with_label("Send");
    g_signal_connect(send_button, "clicked", G_CALLBACK(send_button_clicked), NULL);
    gtk_grid_attach(GTK_GRID(grid), send_button, 0, 1, 1, 1);

    gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("com.example.ClientApp", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}