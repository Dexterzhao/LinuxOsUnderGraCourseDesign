#ifndef __MENUBAR_H
#define __MENUBAR_H

void CreateMenuBar(GtkWidget *vbox);
void ShowAbout(void);
void ShutDown(void);

void CreateMenuBar(GtkWidget *vbox)
{
	GtkWidget *menu1, *menu2;
	GtkWidget *menu_bar;
	GtkWidget *HELP_MENU, *FUNC_MENU;
	GtkWidget *helpmenu;
	GtkWidget *menuitems;

	menu_bar = gtk_menu_bar_new();
	gtk_box_pack_start(GTK_BOX(vbox), menu_bar, FALSE, TRUE, 0);
	gtk_widget_show(menu_bar);

	helpmenu = gtk_menu_new();
	menu1 = gtk_menu_new();
	menu2 = gtk_menu_new();

	FUNC_MENU = gtk_menu_item_new_with_label("functions");
	gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), FUNC_MENU);
	gtk_widget_show(HELP_MENU);
	HELP_MENU = gtk_menu_item_new_with_label("help");
	gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), HELP_MENU);
	gtk_widget_show(HELP_MENU);

	gtk_menu_item_set_submenu(GTK_MENU_ITEM(FUNC_MENU), menu1);
	menuitems = gtk_menu_item_new_with_label("shutdown");
	g_signal_connect_swapped(G_OBJECT(menuitems), "activate", G_CALLBACK(ShutDown), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu1), menuitems);
	gtk_widget_show(menuitems);
	menuitems = gtk_menu_item_new_with_label("logout");
	gtk_menu_shell_append(GTK_MENU_SHELL(menu1), menuitems);
	gtk_widget_show(menuitems);
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(HELP_MENU), menu2);
	menuitems = gtk_menu_item_new_with_label("About..");
	g_signal_connect_swapped(G_OBJECT(menuitems), "activate", G_CALLBACK(ShowAbout), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu2), menuitems);
	gtk_widget_show(menuitems);
}

void ShowAbout(void)
{
	GtkWidget *dialog = gtk_about_dialog_new();
	gtk_about_dialog_set_name(GTK_ABOUT_DIALOG(dialog), "SysMonitor for Linux");
	gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog), "HUST ZZA 1.0");
	gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog),"Copyright © 2016 Dexter");
	gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(dialog),"dexter.zhza@gmail.com");
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}

//shutdown the computer
void ShutDown(void)
{
	if(geteuid() != 0)//root : geteuid == 0
	{
		GtkWidget *dialog ;
		gchar *msg = "please start this programe in root";
		dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, msg, "");
		gtk_dialog_run(GTK_DIALOG(dialog));
		gtk_widget_destroy(dialog);

	}
	else
		system("/sbin/init 0"); //shutdown
}
#endif
