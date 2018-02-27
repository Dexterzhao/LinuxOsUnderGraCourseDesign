#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/utsname.h>

#ifndef __SYSINFO_H
#define __SYSINFO_H

void CreatePageFive(GtkWidget *notebook);    //Info
void GetSysTime(char *runtime);
gint UpdateSystime(gpointer data);



//get the information of system
void CreatePageFive(GtkWidget *notebook)
{
	int s;
	struct utsname u_name;
	s = uname(&u_name);
	if (s < 0)
	{
		printf("uname()failed\n");
		exit(1);
	}

	char starttime[100];
	GetSysTime(starttime);



	char sysinfo[1000];
	strcpy(sysinfo, "sysname:                         ");
	strcat(sysinfo, u_name.sysname);
	strcat(sysinfo, "\nnodename:                     ");
	strcat(sysinfo, u_name.nodename);
	strcat(sysinfo, "\nrelease:                            ");
	strcat(sysinfo, u_name.release);
	strcat(sysinfo, "\nversion:                            ");
	strcat(sysinfo, u_name.version);
	strcat(sysinfo, "\nmachine:                          ");
	strcat(sysinfo, u_name.machine);
	strcat(sysinfo, "\n\n\n\n\nTotalMemory:                   ");
	
	strcat(sysinfo, memft.MemTotal);
	strcat(sysinfo, "FreeMemory:                    ");
	strcat(sysinfo, memft.MemFree);
	strcat(sysinfo, "Buffers:                                ");
	strcat(sysinfo, memft.Buffers);
	strcat(sysinfo, "SwapTotal:                         ");
	strcat(sysinfo, memft.SwapTotal);
	strcat(sysinfo, "SwapFree:                          ");
	strcat(sysinfo, memft.SwapFree);

	GtkWidget *label, *frame, *vbox, *timelabel;
	frame = gtk_frame_new("");
	gtk_container_set_border_width(GTK_CONTAINER(frame), 10);
	gtk_widget_show(frame);

	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), frame, gtk_label_new("System information"));

	vbox = gtk_vbox_new(FALSE, 10);
	gtk_container_set_border_width(GTK_CONTAINER(vbox), 5);

	label = gtk_label_new(sysinfo);
	gtk_container_add(GTK_CONTAINER(frame), vbox);
	gtk_widget_show(label);
	gtk_box_pack_start(GTK_BOX(vbox), label, TRUE, TRUE, 0);

	timelabel = gtk_label_new(starttime);
	gtk_box_pack_start(GTK_BOX(vbox), timelabel, TRUE, TRUE, 0);
	gtk_widget_show(timelabel);

	gtk_timeout_add(1000, UpdateSystime, timelabel);

}

void GetSysTime(char *starttime)
{
	char buf[50], *ch;
	time_t nowtime;
	struct tm *p;
	int i_time, i_runtime,i_stime;
	
	time(&nowtime);
	p = localtime(&nowtime);
	i_time = p->tm_hour*3600 + p->tm_min*60+p->tm_sec;

	int fd = open("/proc/uptime", O_RDONLY);
	read(fd, buf, sizeof(buf));
	close(fd);
	ch = strtok(buf, " ");
	i_runtime = atoi(ch);

	i_stime = i_time - i_runtime; 
	int hour, min, sec;
	char temp[2];
	hour = i_stime / 3600;
	min = (i_stime - 3600 * hour) / 60;
	sec = i_stime - 3600*hour - 60*min;

	strcpy(starttime, "System start from : ");
	sprintf(temp, "%02d", hour);
	strcat(starttime, temp);
	strcat(starttime, ":");
	sprintf(temp, "%02d", min);
	strcat(starttime, temp);
	strcat(starttime, ":");
	sprintf(temp, "%02d", sec);
	strcat(starttime, temp);
	strcat(starttime, "\n");

	hour = i_runtime / 3600;
	min = (i_runtime - 3600 * hour) / 60;
	sec = i_runtime - 3600*hour - 60*min;
	strcat(starttime, "System runing for : ");
	sprintf(temp, "%02d", hour);
	strcat(starttime, temp);
	strcat(starttime, ":");
	sprintf(temp, "%02d", min);
	strcat(starttime, temp);
	strcat(starttime, ":");
	sprintf(temp, "%02d", sec);
	strcat(starttime, temp);
	strcat(starttime, "\n\0");
}

gint UpdateSystime(gpointer data)
{
	char systime[100];
	GetSysTime(systime);
	gtk_label_set_text(GTK_LABEL(data), systime);
	return TRUE;
}

#endif
