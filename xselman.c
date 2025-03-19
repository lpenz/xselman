// Copyright (C) 2010 Leandro Lisboa Penz <lpenz@lpenz.org>
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

/**
 * \file
 * \brief  xselman manages the X selections.
 *
 * For now, xselman has a single feature: X selection synchronization.
 *
 * X windows has 2 relevant selections: the "clipboard" and the "primary"
 * selections.
 *
 * The behaviour of these selections is not 100% consistent: the pasting
 * behaviour from most applications (SHIFT+INS for "primary",
 * middle-mouse-button for "clipboard") is different from the behaviour of the
 * terminal emulators (always "primary").
 */

#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>

#define PROGRAM_NAME "xselman"
#define PROGRAM_VERSION "0.2.3"

#define XSEL_NUM ((sizeof xsel_info) / (sizeof(*xsel_info)))
#define XSELMAN_INTERVAL 1500

//! Information about the selections to sync.
const struct {
    const char *name;
    GdkAtom cba;
} xsel_info[] = {
    {"primary", GDK_SELECTION_PRIMARY}, {"clipboard", GDK_SELECTION_CLIPBOARD},
    /* No reason to sync secondary: */
    /* { "secondary" , GDK_SELECTION_SECONDARY }, */
};

typedef struct _xselman_t xselman_t;

//! Selection runtime data.
typedef struct {
    size_t i;
    GtkClipboard *cb;
    char *txt;
    xselman_t *xselman;
} xsel_t;

//! Top runtime data.
struct _xselman_t {
    GtkWidget *w;
    xsel_t xsel[XSEL_NUM];
};

/****************************************************************************/

/**
 * \brief  Checks if the text of a selection changed and synchronizes the
 * others if it did.
 */
void xsel_check_txt(GtkClipboard *clipboard, const gchar *text, gpointer data) {
    (void)clipboard;
    xsel_t *xsel = data;
    size_t i;

    /* Initialization. */
    if (!xsel->txt) {
        if (text) xsel->txt = strdup(text);
        return;
    }

    /* No change, quit. */
    if (strcmp(text, xsel->txt) == 0) return;

    /* Syncing selections and storing last-seen-text. */
    for (i = 0; i < XSEL_NUM; i++) {
        xsel_t *xseli = &xsel->xselman->xsel[i];

        free(xseli->txt);
        xseli->txt = strdup(text);

        /* No need to set the clipboard that changed. */
        if (i == xsel->i) continue;

        gtk_clipboard_set_text(xseli->cb, text, strlen(text));
    }
}

/**
 * \brief  Timeout handler - requests selection text for xsel_check_txt.
 */
int xsel_check(gpointer data) {
    xsel_t *xsel = data;

    gtk_clipboard_request_text(xsel->cb, xsel_check_txt, xsel);
    g_timeout_add(XSELMAN_INTERVAL, xsel_check, xsel);

    return 0;
}

/**
 * \brief  Initialization.
 */
int main(int argc, char *argv[]) {
    xselman_t xselman;
    size_t i;

    gtk_init(&argc, &argv);

    xselman.w = gtk_invisible_new();

    for (i = 0; i < XSEL_NUM; i++) {
        xselman.xsel[i].i = i;
        xselman.xsel[i].cb = gtk_clipboard_get(xsel_info[i].cba);
        xselman.xsel[i].txt = NULL;
        xselman.xsel[i].xselman = &xselman;
        g_timeout_add(XSELMAN_INTERVAL, xsel_check, &xselman.xsel[i]);
    }

    gtk_main();

    return 0;
}
