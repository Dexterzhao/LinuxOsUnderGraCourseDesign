#include <time.h>
#include <gtk/gtk.h>

#define MAXLEN  100
#define WINWIDTH 200
#define WINHEIGHT 100
#define REFRESH 3000

static GtkWidget *clocker;

static gboolean add_handler()
{
	static adder = 0;
	static sum = 0;
	sum += adder;
	++adder;
	gchar buf[MAXLEN];
	sprintf(buf, "<span foreground='black' underline='double' underline_color='black' font_desc='30'>%d</span>", sum);

	// set the color and font, and the content
	gtk_label_set_markup(GTK_LABEL(clocker), buf);

	// TRUE to continue to be called back
	// FALSE to stop to be called
	return TRUE;
}

int main(int argc, char *argv[])
{
	GtkWidget *window;
	gtk_init(&argc, &argv);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_usize(window, WINWIDTH, WINHEIGHT);
	gtk_window_set_title(GTK_WINDOW(window), "Clock");
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_NONE);
	// connect the delete signal to gtk_main_quit to quit the program
	// delete signal comes from the close button or close option
	g_signal_connect(G_OBJECT(window), "delete_event", G_CALLBACK(gtk_main_quit), NULL);

	clocker = gtk_label_new(NULL);
	gtk_widget_set_usize(clocker, WINWIDTH, WINHEIGHT);
	gtk_label_set_justify(GTK_LABEL(clocker), GTK_JUSTIFY_CENTER);
	gtk_container_add(GTK_CONTAINER(window), clocker);

	g_timeout_add(REFRESH, (GtkFunction)add_handler, (gpointer)window);
	gtk_widget_show_all(window);
	// waiting for events
	gtk_main();

	return 0;
}