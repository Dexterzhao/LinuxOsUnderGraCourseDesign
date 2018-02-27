#ifndef __MEMINFO_H
#define __MEMINFO_H

#define MEM_LINE_MEMALL    1
#define MEM_LINE_MEMFREE   2
#define MEM_LINE_BUFFER    3
#define MEM_LINE_CACHED    4
#define MEM_LINE_SWAPCACHED    5
#define MEM_LINE_SWAPALL    14
#define MEM_LINE_SWAPFREE   15

void CreatePageTwo(GtkWidget *notebook);    //MEM
void DrawingMM();
void GetMmInfo();
int GetValueFromString(char *string);
void DrawingSWAP();

typedef struct _mem{
	char MemTotal[20];
	char MemFree[20];
	char Buffers[20];
	char Cached[20];
	char SwapCached[20];
	char SwapTotal[20];
	char SwapFree[20];
}mem_info;

GtkWidget *draw_mm;
GtkWidget *window;
GtkWidget *labelofmm;
GtkWidget *mminfolabel;
GdkPixmap *mm_graph;
mem_info memft;
static gint mmPoints[100];
static gint swapPoints[100];
static gfloat mm_rate = 0.0;
static gfloat swap_rate = 0.0;
gdouble memUseRatio, swapUseRatio;
char mmBuffer[2000];

GtkWidget *window_swap;
GtkWidget *draw_swap;
GdkPixmap *swap_graph;
char swapBuffer[200];

static gboolean mm_expose_event(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
	gdk_draw_drawable(widget->window, widget->style->fg_gc[GTK_WIDGET_STATE(widget)], mm_graph, event->area.x, event->area.y, event->area.x, event->area.y, event->area.width, event->area.height);
	return FALSE;
}
static gboolean mm_configure_event(GtkWidget *widget, GdkEventConfigure *event, gpointer data)
{
	if (mm_graph)
	{
		g_object_unref(mm_graph);
	}
	mm_graph = gdk_pixmap_new(widget->window, widget->allocation.width, widget->allocation.height, -1);
	gdk_draw_rectangle(mm_graph, widget->style->white_gc, TRUE, 0, 0, widget->allocation.width, widget->allocation.height);
	return TRUE;
}

static gboolean swap_expose_event(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
	gdk_draw_drawable(widget->window, widget->style->fg_gc[GTK_WIDGET_STATE(widget)], swap_graph, event->area.x, event->area.y, event->area.x, event->area.y, event->area.width, event->area.height);
	return FALSE;
}
static gboolean swap_configure_event(GtkWidget *widget, GdkEventConfigure *event, gpointer data)
{
	if (swap_graph)
	{
		g_object_unref(swap_graph);
	}
	swap_graph = gdk_pixmap_new(widget->window, widget->allocation.width, widget->allocation.height, -1);
	gdk_draw_rectangle(swap_graph, widget->style->white_gc, TRUE, 0, 0, widget->allocation.width, widget->allocation.height);
	return TRUE;
}

void CreatePageTwo(GtkWidget *notebook)
{
	GtkWidget *vbox, *frame, *MMframe, *SWAPframe;

	frame = gtk_frame_new("");
	gtk_container_set_border_width(GTK_CONTAINER(frame), 10);
	gtk_widget_show(frame);

	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), frame, gtk_label_new("Memory information"));

	GtkWidget *table = gtk_table_new(10, 10, TRUE);
	gtk_container_add(GTK_CONTAINER(frame), table);
	MMframe = gtk_frame_new("Memory usage history");
	gtk_table_attach_defaults(GTK_TABLE(table), MMframe, 1, 9, 0, 5);
	gtk_widget_show(MMframe);
	//labelofmm = gtk_label_new("NULL");
	//gtk_table_attach_defaults(GTK_TABLE(table), labelofmm, 8, 10, 0, 10);
	//gtk_widget_show(labelofmm);

	SWAPframe = gtk_frame_new("Swap usage history");
	gtk_table_attach_defaults(GTK_TABLE(table), SWAPframe, 1, 9, 5, 10);
	gtk_widget_show(SWAPframe);

	
	

	GetCpuInfo();
	GetMmInfo();
	gtk_widget_show(table);



	draw_mm = gtk_drawing_area_new();
	gtk_widget_set_app_paintable(draw_mm, TRUE);
	gtk_drawing_area_size(GTK_DRAWING_AREA(draw_mm), 40, 80);
	gtk_container_add(GTK_CONTAINER(MMframe), draw_mm);
	gtk_widget_show(draw_mm);

	DrawingMM();

	draw_swap = gtk_drawing_area_new();
	gtk_widget_set_app_paintable(draw_swap, TRUE);
	gtk_drawing_area_size(GTK_DRAWING_AREA(draw_swap), 40, 80);
	gtk_container_add(GTK_CONTAINER(SWAPframe), draw_swap);
	gtk_widget_show(draw_swap);

	DrawingSWAP();
}


void DrawingMM()
{
	int i;
	for (i = 0; i < 100; i++)
	{
		/* 要取[a,b)之间的随机整数（包括a，但不包括b)，使用：
		 * (rand() % (b - a)) + a */
		mmPoints[i] = 999;
	}

	g_signal_connect(draw_mm, "expose_event",G_CALLBACK(mm_expose_event), NULL);
	g_signal_connect(draw_mm, "configure_event", G_CALLBACK(mm_configure_event), NULL);
	//gtk_widget_show(draw_mm);
}

void DrawingSWAP()
{
	int i;
	for (i = 0; i < 100; i++)
	{
		/* 要取[a,b)之间的随机整数（包括a，但不包括b)，使用：
		 * (rand() % (b - a)) + a */
		swapPoints[i] = 999;
	}

	g_signal_connect(draw_swap, "expose_event",G_CALLBACK(swap_expose_event), NULL);
	g_signal_connect(draw_swap, "configure_event", G_CALLBACK(swap_configure_event), NULL);
}

gint refresh_mm()
{

	GdkGC *gc_chart_mm = gdk_gc_new(draw_mm->window);
	GdkColor color;
	color.red = 0xFFFF;
	color.green = 0xFFFF;
	color.blue = 0xFFFF;
	gdk_gc_set_rgb_fg_color(gc_chart_mm, &color);
	int width1, height1, curPoint1, step1;
	mm_rate = memUseRatio;
	gdk_draw_rectangle(mm_graph, gc_chart_mm, TRUE, 0, 0,draw_mm->allocation.width, draw_mm->allocation.height);
	width1 = draw_mm->allocation.width;
	height1 = draw_mm->allocation.height;
	curPoint1 = (int) (mm_rate * (double) height1);
	mmPoints[99] = height1 - curPoint1;
	int j;
	for (j = 0; j < 99; j++)
		mmPoints[j] = mmPoints[j + 1];
	step1 = width1 / 99;
	GdkGC *gc = gdk_gc_new(GDK_DRAWABLE(mm_graph));
    	gdk_color_parse("red",&color);
	gdk_gc_set_foreground(gc, &color);
	gdk_gc_set_line_attributes(gc, 1, GDK_LINE_SOLID, GDK_CAP_ROUND,GDK_JOIN_MITER);
	for (j = 99; j >= 1; j--)
	{
		gdk_draw_line(GDK_DRAWABLE(mm_graph), gc, j * step1, mmPoints[j], /* 第一个点坐标 */
		(j - 1) * step1, mmPoints[j - 1]); /* 第二个点坐标 */
	}
	gtk_widget_queue_draw(draw_mm);

	GetMmInfo();
	return TRUE;
}

gint refresh_swap()
{
	GdkGC *gc_chart_swap = gdk_gc_new(draw_swap->window);
	GdkColor color;
	color.red = 0xFFFF;
	color.green = 0xFFFF;
	color.blue = 0xFFFF;
	gdk_gc_set_rgb_fg_color(gc_chart_swap, &color);
	int width1, height1, curPoint1, step1;
	swap_rate = swapUseRatio;
	gdk_draw_rectangle(swap_graph, gc_chart_swap, TRUE, 0, 0,draw_swap->allocation.width, draw_swap->allocation.height);
	width1 = draw_swap->allocation.width;
	height1 = draw_swap->allocation.height;
	curPoint1 = (int) (swap_rate * (double) height1);
	swapPoints[99] = height1 - curPoint1;
	int j;
	for (j = 0; j < 99; j++)
		swapPoints[j] = swapPoints[j + 1];
	step1 = width1 / 99;
	GdkGC *gc = gdk_gc_new(GDK_DRAWABLE(swap_graph));
    	gdk_color_parse("red",&color);
	gdk_gc_set_foreground(gc, &color);
	gdk_gc_set_line_attributes(gc, 1, GDK_LINE_SOLID, GDK_CAP_ROUND,GDK_JOIN_MITER);
	for (j = 99; j >= 1; j--)
	{
		gdk_draw_line(GDK_DRAWABLE(swap_graph), gc, j * step1, swapPoints[j], /* 第一个点坐标 */
		(j - 1) * step1, swapPoints[j - 1]); /* 第二个点坐标 */
	}
	gtk_widget_queue_draw(draw_swap);
	return TRUE;
}


void GetMmInfo()
{
	//printf("hhe\n");
	GetInfo("/proc/meminfo", "MemTotal", memft.MemTotal);
	GetInfo("/proc/meminfo", "MemFree", memft.MemFree);
	GetInfo("/proc/meminfo", "Buffers", memft.Buffers);
	GetInfo("/proc/meminfo", "Cached", memft.Cached);
	GetInfo("/proc/meminfo", "SwapCached", memft.SwapCached);
	GetInfo("/proc/meminfo", "SwapTotal", memft.SwapTotal);
	GetInfo("/proc/meminfo", "SwapFree", memft.SwapFree);

	int mem_total, mem_free;
	mem_total = GetValueFromString(memft.MemTotal);
	mem_free = GetValueFromString(memft.MemFree);
	memUseRatio = (double)(mem_total - mem_free) / mem_total;
	//printf("memUseRatio = %lf\n", memUseRatio);

	int swap_total, swap_free;
	swap_total = GetValueFromString(memft.SwapTotal);
	swap_free = GetValueFromString(memft.SwapFree);
	swapUseRatio = (double)(swap_total - swap_free) / swap_total;
	//printf("swapUseRatio = %lf\n", swapUseRatio);
	return;
}

int GetValueFromString(char *string)
{
	char *p = string;
	int val = 0;
	while(*p >= '0' && *p <= '9')
	{
		val = 10 * val + *p - '0';
		p++;
	}
	return val;
}

#endif



