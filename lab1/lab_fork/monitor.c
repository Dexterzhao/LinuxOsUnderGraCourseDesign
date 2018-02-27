#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN  100
#define WINWIDTH 200
#define WINHEIGHT 100
#define REFRESH 2000

typedef struct PACKED
{
	char name[20];	
	unsigned int user;
	unsigned int nice;
	unsigned int system;
	unsigned int idle;
	unsigned int lowait;
	unsigned int irq;
	unsigned int softirq;
}CPU_OCCUPY;

double cal_cpuoccupy (CPU_OCCUPY *o, CPU_OCCUPY *n)
{   
	unsigned long od, nd;
	unsigned long id, sd;
	double cpu_use = 0;

	od = (unsigned long) (o->user + o->nice + o->system +o->idle + o->lowait + o->irq + o->softirq);
	nd = (unsigned long) (n->user + n->nice + n->system +n->idle + n->lowait + n->irq + n->softirq);

	double sum = nd - od;
	double idle = n->user + n->system + n->nice -o->user - o->system- o->nice;
	cpu_use = idle/sum;

	return cpu_use;
}

void get_cpuoccupy (CPU_OCCUPY *cpust)
{   
	FILE *fd;
	int n;
	char buff[256];
	CPU_OCCUPY *cpu_occupy;
	cpu_occupy=cpust;

	fd = fopen ("/proc/stat", "r");
	fgets (buff, sizeof(buff), fd);
	sscanf (buff, "%s %u %u %u %u %u %u %u", cpu_occupy->name, &cpu_occupy->user, &cpu_occupy->nice,&cpu_occupy->system, &cpu_occupy->idle,&cpu_occupy->lowait,&cpu_occupy->irq,&cpu_occupy->softirq);
	fclose(fd);
}

static GtkWidget *clocker;

static gboolean cpu_monitor()
{
	CPU_OCCUPY cpu_stat1;
	CPU_OCCUPY cpu_stat2;
	double cpu_perc;
	get_cpuoccupy((CPU_OCCUPY *)&cpu_stat1);
	usleep(100000);
	get_cpuoccupy((CPU_OCCUPY *)&cpu_stat2);
	cpu_perc = 100*cal_cpuoccupy ((CPU_OCCUPY *)&cpu_stat1, (CPU_OCCUPY *)&cpu_stat2);
	
	gchar buf[MAXLEN];
	sprintf(buf, "<span foreground='orange' font_desc='30'>%.2f%%</span>", cpu_perc);

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

	g_timeout_add(REFRESH, (GtkFunction)cpu_monitor, (gpointer)window);
	gtk_widget_show_all(window);
	// waiting for events
	gtk_main();

	return 0;
}