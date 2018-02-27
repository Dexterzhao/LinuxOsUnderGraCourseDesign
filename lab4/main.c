#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <dirent.h>
#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <cairo.h>
#include <glib.h>
#include <time.h>

#include "MenuBar.h"
#include "CPUinfo.h"
#include "Meminfo.h"
#include "Processinfo.h"
#include "Sysinfo.h"


//Create the main version window
void CreateVersion(GtkWidget *vbox);

void CreatePageFour(GtkWidget *notebook);
gint refresh_clock(gpointer data);


GtkWidget *statusbar;

int main(int argc, char* argv[])
{
	GtkWidget *window, *main_vbox;
	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "SYSTEM MONITOR");
	gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_widget_show(window);
	g_signal_connect(G_OBJECT(window),"destroy",G_CALLBACK(gtk_main_quit),NULL);

	main_vbox = gtk_vbox_new(FALSE, 10);
	gtk_container_set_border_width(GTK_CONTAINER(main_vbox), 5);
	gtk_container_add(GTK_CONTAINER(window), main_vbox);
	gtk_widget_show(main_vbox);	

	//Create menubar
	CreateMenuBar(main_vbox);
	CreateVersion(main_vbox);

	gtk_timeout_add(500, refresh_cpu, NULL);
	gtk_timeout_add(500, refresh_mm, NULL);
	gtk_timeout_add(500, refresh_swap, NULL);
	gtk_widget_show_all(window);
	gtk_main();
}

//Create the main version window
void CreateVersion(GtkWidget *vbox)
{
	GtkWidget *notebook;
	gint context_id;
	notebook = gtk_notebook_new();
	gtk_notebook_set_tab_pos((GtkNotebook *)notebook, GTK_POS_TOP);
	gtk_notebook_set_show_border((GtkNotebook *) notebook, FALSE);
	gtk_box_pack_start(GTK_BOX(vbox), notebook, TRUE, TRUE, 0);
	gtk_widget_show(notebook);
	statusbar = gtk_statusbar_new();
	gtk_box_pack_start(GTK_BOX(vbox), statusbar, TRUE, TRUE, 0);
	gtk_widget_show(statusbar);
	context_id = gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar), "Timestatus");
	gtk_timeout_add(1000, refresh_clock, GINT_TO_POINTER(context_id));
	refresh_clock(GINT_TO_POINTER(context_id));
	
	CreatePageOne(notebook);
	CreatePageTwo(notebook);
	CreatePageThree(notebook);
	CreatePageFive(notebook);

	gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 0);

}

gint refresh_clock(gpointer data)
{
	time_t t;
	t=time(NULL);
	char msg[1000], temp[100];
	char *buf, *p;
	buf = ctime(&t);
	p = strtok(buf, "\n");

	strcpy(msg, buf);
	sprintf(temp, "                     CPU: %2.2lf%%", cpu_rate * 100);
	strcat(msg, temp);
	sprintf(temp, "                     Memory: %2.2lf%%", mm_rate * 100);
	strcat(msg, temp);
	sprintf(temp, "           Swap: %2.2lf%%", swap_rate * 100);
	strcat(msg, temp);

	gtk_statusbar_push(GTK_STATUSBAR(statusbar), GPOINTER_TO_INT(data), msg);
}
