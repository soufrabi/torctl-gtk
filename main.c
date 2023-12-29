#include <gtk/gtk.h>
#include <unistd.h>
#include <vte/vte.h>

static void child_ready(VteTerminal *terminal, GPid pid, GError *error,
                        gpointer user_data) {
    if (!terminal)
        return;
    if (pid == -1) {
        g_print("Quit App : child-ready did not start properly\n");
    }
}

static void clicked_torctl_start(GtkButton *btn, VteTerminal *terminal) {
    g_print("Clicked Torctl Start\n");

    /* clear terminal screen */
    vte_terminal_reset(terminal, TRUE, TRUE);

    gchar **command = (gchar *[]){"pkexec", "torctl", "start", NULL};
    vte_terminal_spawn_async(VTE_TERMINAL(terminal), VTE_PTY_DEFAULT,
                             NULL,        /* working directory  */
                             command,     /* command */
                             NULL,        /* environment */
                             0,           /* spawn flags */
                             NULL, NULL,  /* child setup */
                             NULL,        /* child pid */
                             -1,          /* timeout */
                             NULL,        /* cancellable */
                             child_ready, /* callback */
                             NULL);       /* user_data */
}

static void clicked_torctl_stop(GtkButton *btn, VteTerminal *terminal) {
    g_print("Clicked Torctl Stop\n");

    /* clear terminal screen */
    vte_terminal_reset(terminal, TRUE, TRUE);

    /* Start a new shell */
    gchar **command = (gchar *[]){"pkexec", "torctl", "stop", NULL};
    vte_terminal_spawn_async(VTE_TERMINAL(terminal), VTE_PTY_DEFAULT,
                             NULL,        /* working directory  */
                             command,     /* command */
                             NULL,        /* environment */
                             0,           /* spawn flags */
                             NULL, NULL,  /* child setup */
                             NULL,        /* child pid */
                             -1,          /* timeout */
                             NULL,        /* cancellable */
                             child_ready, /* callback */
                             NULL);       /* user_data */
}

static void clicked_torctl_status(GtkButton *btn, VteTerminal *terminal) {
    g_print("Clicked Torctl Status\n");

    /* clear terminal screen */
    vte_terminal_reset(terminal, TRUE, TRUE);

    gchar **command = (gchar *[]){"pkexec", "torctl", "status", NULL};
    vte_terminal_spawn_async(VTE_TERMINAL(terminal), VTE_PTY_DEFAULT,
                             NULL,        /* working directory  */
                             command,     /* command */
                             NULL,        /* environment */
                             0,           /* spawn flags */
                             NULL, NULL,  /* child setup */
                             NULL,        /* child pid */
                             -1,          /* timeout */
                             NULL,        /* cancellable */
                             child_ready, /* callback */
                             NULL);       /* user_data */
}


static void quit_app(VteTerminal *terminal) { g_print("Quit App"); }

static void update_state(GtkLabel *lab) {
    if (access("/var/lib/torctl/started", F_OK) == 0) {
        g_print("File Exists\n");
    } else {
        g_print("File does not exist\n");
    }
}

static void app_activate(GApplication *app, gpointer *user_data) {
    g_print("GtkApplication is activated.\n");

    GtkWidget *win;
    GtkWidget *box;
    GtkWidget *btn_box;
    GtkWidget *terminal;
    GtkWidget *btn1, *btn2, *btn3;
    GtkWidget *state_label;

    win = gtk_application_window_new(GTK_APPLICATION(app));
    gtk_window_set_title(GTK_WINDOW(win), "Torctl GUI");
    gtk_window_set_default_size(GTK_WINDOW(win), 400, 500);

    btn_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_set_homogeneous(GTK_BOX(btn_box), TRUE);

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_set_homogeneous(GTK_BOX(box), FALSE);
    gtk_window_set_child(GTK_WINDOW(win), box);

    btn1 = gtk_button_new_with_label("Start");
    btn2 = gtk_button_new_with_label("Stop");
    btn3 = gtk_button_new_with_label("Status");

    state_label = gtk_label_new("State : Unknown");

    terminal = vte_terminal_new();
    g_signal_connect(btn1, "clicked", G_CALLBACK(clicked_torctl_start),
                     terminal);
    g_signal_connect(btn2, "clicked", G_CALLBACK(clicked_torctl_stop),
                     terminal);
    g_signal_connect(btn3, "clicked", G_CALLBACK(clicked_torctl_status),
                     terminal);

    gtk_box_append(GTK_BOX(btn_box), btn1);
    gtk_box_append(GTK_BOX(btn_box), btn2);
    gtk_box_append(GTK_BOX(btn_box), btn3);
    gtk_box_append(GTK_BOX(btn_box), state_label);
    gtk_box_append(GTK_BOX(box), btn_box);
    gtk_box_append(GTK_BOX(box), terminal);
    /* Configure terminal */
    /* vte_terminal_set_word_chars(VTE_TERMINAL(terminal),"-A-Za-z0-9:./?%&#_=+@~");
     */
    vte_terminal_set_scrollback_lines(VTE_TERMINAL(terminal), 1000);
    vte_terminal_set_scroll_on_output(VTE_TERMINAL(terminal), FALSE);
    vte_terminal_set_scroll_on_keystroke(VTE_TERMINAL(terminal), TRUE);
    /* vte_terminal_set_rewrap_on_resize(VTE_TERMINAL(terminal), TRUE); */

    /* Connect some signals */
    g_signal_connect(win, "delete-event", G_CALLBACK(quit_app), NULL);
    g_signal_connect(terminal, "child-exited", G_CALLBACK(quit_app), NULL);

    gtk_window_present(GTK_WINDOW(win));
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;
    app = gtk_application_new("org.soufrabi.torctl-gtk", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(app_activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
