#ifndef __PROCESSINFO_H
#define __PROCESSINFO_H

GtkWidget *clist;

#define PROC_LINE_NAME         1
#define PROC_LINE_PID               5
#define PROC_LINE_STATE           2
#define PROC_LINE_VMSIZE        13
//#define PROC_LINE_PRIORITY
typedef struct _proc_info_st{
	char name[256];
	char pid[20];
	char state[20];
	char vmsize[20];
	char priority[20];
	char ppid[20];
}proc_info;

void CreatePageThree(GtkWidget *notebook);
gint ShowProcInfo(gpointer data);
void ReadProc(proc_info *ft, const char* path);
void ReadLine(FILE *fp, char *buff, int bufflen, int line);
void SearchProcByPID(gpointer data);
void ProcessRun(gpointer data);
void SetEntryInfo(GtkWidget *clist, gint row, gint column, GdkEventButton *event, gpointer data);
static GtkWidget *entry;

void CreatePageThree(GtkWidget *notebook)
{
	GtkWidget *vbox, *scroll, *table, *searchbutton1, *searchbutton2, *commondbutton, *button;

	table = gtk_table_new(10, 10, TRUE);
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), table, gtk_label_new("Process information"));
	gtk_widget_show(table);


	gchar *colname[6] = {"Processname", "PID", "PPID", "state", "Mem(KB)", "Priority"};

	scroll = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll), GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
	//gtk_notebook_append_page(GTK_NOTEBOOK(notebook), scroll, gtk_label_new("Page3-process"));
	gtk_table_attach_defaults(GTK_TABLE(table), scroll, 0, 10, 0, 9);


	clist = gtk_clist_new_with_titles(6, colname);
	g_signal_connect(GTK_OBJECT(clist), "select_row", GTK_SIGNAL_FUNC(SetEntryInfo), NULL);
	//gtk_signal_connext(GTK_OBJECT(clist), "select_row", GTK_SIGNAL_FUNC(), NULL);
	gtk_clist_set_column_width(GTK_CLIST(clist), 0, 200);
	gtk_clist_set_column_width(GTK_CLIST(clist), 1, 100);
	gtk_clist_set_column_width(GTK_CLIST(clist), 2, 100);
	gtk_clist_set_column_width(GTK_CLIST(clist), 3, 100);
	gtk_clist_set_column_width(GTK_CLIST(clist), 4, 100);
	gtk_clist_set_column_width(GTK_CLIST(clist), 4, 110);

	gtk_container_add(GTK_CONTAINER(scroll), clist);
	gtk_widget_show(clist);

	entry = gtk_entry_new();
	searchbutton1 = gtk_button_new_with_label("search by PID");
	gtk_container_set_border_width(GTK_CONTAINER(searchbutton1), 9);
	searchbutton2 = gtk_button_new_with_label("run");
	gtk_container_set_border_width(GTK_CONTAINER(searchbutton2), 9);
	button = gtk_button_new_with_label("refresh");
	gtk_container_set_border_width(GTK_CONTAINER(button), 5);
	gtk_table_attach_defaults(GTK_TABLE(table), entry, 0, 4, 9, 10);
	gtk_table_attach_defaults(GTK_TABLE(table), searchbutton1, 4, 6, 9, 10);
	gtk_table_attach_defaults(GTK_TABLE(table), searchbutton2, 6, 8, 9, 10);
	gtk_table_attach_defaults(GTK_TABLE(table), button, 8, 10, 9, 10);
	gtk_widget_show(entry);
	gtk_widget_show(searchbutton1);
	gtk_widget_show(searchbutton2);
	gtk_widget_show(button);

	g_signal_connect(G_OBJECT(searchbutton1), "clicked", G_CALLBACK(SearchProcByPID), NULL);
	g_signal_connect(G_OBJECT(searchbutton2), "clicked", G_CALLBACK(ProcessRun), NULL);
	g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(ShowProcInfo), NULL);
	gtk_timeout_add(30000, ShowProcInfo, NULL);

	//commondbutton
	ShowProcInfo(NULL);
}

gint ShowProcInfo(gpointer data)
{
	DIR *dir;
	struct dirent *ptr;
	dir = opendir("/proc");

	gtk_clist_clear(GTK_CLIST(clist));
	while ((ptr = readdir(dir)) != NULL)
	{
		if ((ptr->d_name[0] >= '0') && (ptr->d_name[0] <= '9'))
		{
			proc_info ft;
			ReadProc(&ft, ptr->d_name);
			//printf("%s\n", ft.pid);
			//printf("%s\n%s\n%s\n%s\n\n", ft.name, ft.pid, ft.state, ft.vmsize);
		}
	}
	closedir(dir);
	return TRUE;
}
void ReadProc(proc_info *ft, const char* path)
{
	FILE *fp = NULL;
	char file[512] = {0};
	char buff[1024] = {0};

	sprintf(file, "/proc/%s/status", path);
	if(!(fp = fopen(file, "r")))
	{
		printf("read file %s wrong! exit\n", path);
		return;
		//exit(0);                                 //MENTION ::  CANNOT USE EXIT HERE, OR THIS PROGRAM WILL KINN ITSELF
	}
	char name[256];
	ReadLine(fp, buff, 1024, PROC_LINE_NAME);
	sscanf(buff, "%s %s", name, ft->name);

	fseek(fp, 0, SEEK_SET);
	ReadLine(fp, buff, 1024, PROC_LINE_PID);
	sscanf(buff, "%s %s", name, ft->pid);

	fseek(fp, 0, SEEK_SET);
	ReadLine(fp, buff, 1024, PROC_LINE_STATE);
	sscanf(buff, "%s %s", name, ft->state);

	fseek(fp, 0, SEEK_SET);
	ReadLine(fp, buff, 1024, PROC_LINE_VMSIZE);
	sscanf(buff, "%s %s", name, ft->vmsize);
	if(strcmp(name, "VmSize:"))
	{
		ft->vmsize[0] = '0';
		ft->vmsize[1] = '\0';
	}
	fclose(fp);

	sprintf(file, "/proc/%s/stat", path);                   //Get priority
	if(!(fp = fopen(file, "r")))
	{
		printf("read file %s wrong! exit\n", path);
		return ;
		//exit(0);
	}
	ReadLine(fp, buff, 1024, 1);
	sscanf(buff, "%*s %*s %*s %s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %s", ft->ppid, ft->priority);
	fclose(fp);

	gchar *list[1][6] = {
		{ft->name, ft->pid, ft->ppid, ft->state, ft->vmsize, ft->priority}
	};

	gtk_clist_append((GtkCList*) clist, list[0]);
	gtk_clist_thaw((GtkCList *) clist);

}

void ReadLine(FILE *fp, char *buff, int bufflen, int line)
{
	char linebuff[bufflen];
	int i;

	for(i = 0; i < line - 1; i++)
	{
		fgets(linebuff, sizeof(linebuff), fp);
	}
	fgets(linebuff, sizeof(linebuff), fp);
	memcpy(buff, linebuff, bufflen);
}

void SearchProcByPID(gpointer data)
{
	const gchar *pid;
	pid = gtk_entry_get_text(GTK_ENTRY(entry));
	puts(pid);

	int i;
	FILE *fp = NULL;
	char file[512] = {0};
	char buf[9][256] = {0};
	char msg[256];

	sprintf(file, "/proc/%s/status", pid);
	if(!(fp = fopen(file, "r")))
	{
		strcpy(msg, "pid is wrong\nplease input the right id of process!\n");
		//puts(msg);
		GtkWidget *dialog ;
		dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, msg, "");
		gtk_dialog_run(GTK_DIALOG(dialog));
		gtk_widget_destroy(dialog);
		return;
	}
	for(i = 0; i < 9; i++)
	{
		fgets(buf[i], sizeof(buf[i]), fp);
		//puts(buf[i]);
	}
	strcpy(msg, buf[0]);
	for(i = 1; i < 9; i++)
		strcat(msg, buf[i]);


	GtkWidget *dialog;
	dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, msg, "");
	gtk_window_set_title(GTK_WINDOW(dialog), "杀死进程?");
	GtkResponseType result = gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
	if (result == GTK_RESPONSE_YES || result == GTK_RESPONSE_APPLY)
	{
		//Kill the process
		gchar cmd_kill[50];
		strcpy(cmd_kill, "kill ");
		strcat(cmd_kill, pid);
		system(cmd_kill);
		ShowProcInfo(NULL);
	}

	return;
}

void ProcessRun(gpointer data)
{
	const gchar *name;
	name = gtk_entry_get_text(GTK_ENTRY(entry));
	int id;
	printf("%s", name);
	if((id = fork()) == 0)
	{
		printf("hehe");
		system(name);
		return;
	}
	return;
}

void SetEntryInfo(GtkWidget *clist, gint row, gint column, GdkEventButton *event, gpointer data)
{
	gchar *pid;
	gtk_clist_get_text(GTK_CLIST(clist), row, 1, &pid);
	gtk_entry_set_text(GTK_ENTRY(entry), pid);
}


#endif
