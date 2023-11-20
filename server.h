#ifndef SERVER_H
#define SERVER_H

#include <gtk/gtk.h>

extern GtkWidget *text_view;

void append_text(const char *text);
void start_server();

#endif // SERVER_H
