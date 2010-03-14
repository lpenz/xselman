
#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>

#define XSEL_NUM ((sizeof xsel_info)/(sizeof(*xsel_info)))
#define XSELMAN_INTERVAL 1500


const struct {
	const char *name;
	GdkAtom cba;
}
xsel_info[] = {
	{ "primary"   , GDK_SELECTION_PRIMARY   },
	{ "secondary" , GDK_SELECTION_SECONDARY },
	{ "clipboard" , GDK_SELECTION_CLIPBOARD },
};

typedef struct _xselman_t xselman_t;

typedef struct
{
	int i;
	char name[20];
	GtkClipboard *cb;
	char *txt;
	xselman_t *xselman;
}
xsel_t;

struct _xselman_t
{
	GtkWidget *w;
	xsel_t xsel[XSEL_NUM];
};

/****************************************************************************/

void xsel_check_txt(GtkClipboard *clipboard, const gchar *text, gpointer data)
{
	xsel_t *xsel = data;
	int i;

	if (! xsel->txt) {
		if (text)
			xsel->txt = strdup(text);
		return;
	}

	if (strcmp(text, xsel->txt) == 0)
		return;

	printf("xsel %-10s: [%s] -> [%s]\n", xsel->name, xsel->txt, text);
	for (i = 0; i < XSEL_NUM; i++) {
		xsel_t *xseli = &xsel->xselman->xsel[i];

		free(xseli->txt);
		xseli->txt = strdup(text);

		if (i == xsel->i)
			continue;

		gtk_clipboard_set_text(xseli->cb, text, strlen(text));
	}
}


int xsel_check(gpointer data)
{
	xsel_t *xsel = data;

	gtk_clipboard_request_text(xsel->cb, xsel_check_txt, xsel);
	g_timeout_add(XSELMAN_INTERVAL, xsel_check, xsel);

	return 0;
}


int main(int argc, char *argv[])
{
	xselman_t xselman;
	int i;

	gtk_init(&argc, &argv);

	xselman.w = gtk_invisible_new();

	for (i = 0; i < XSEL_NUM; i++) {
		xselman.xsel[i].i = i;
		snprintf(xselman.xsel[i].name, sizeof(xselman.xsel[i].name), "%s", xsel_info[i].name);
		xselman.xsel[i].cb = gtk_clipboard_get(xsel_info[i].cba);
		xselman.xsel[i].txt = NULL;
		xselman.xsel[i].xselman = &xselman;
		g_timeout_add(XSELMAN_INTERVAL, xsel_check, &xselman.xsel[i]);
	}

	gtk_main();

	return 0;
}

