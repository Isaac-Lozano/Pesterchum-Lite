#include "main_window.h"

MainWindow::MainWindow(std::string nick_in)
: p_pester_button("PESTER!"),
  p_close_button("Close"),
  p_label("not connected")
  p_hbox(Gtk::ORIENTATION_HORIZONTAL),
  p_vbox(Gtk::ORIENTATION_VERTICAL),
  nick(nick_in)
{
    set_title("Pesterchum");
    set_border(11); //Derived from pesterchum image.
    set_default_size(232, 346); //Size of the in-comic version according to an image on the Wiki.
    set_resizable(false); //We don't want it to change size. I may change this to vertical movement only in the future.

    add(p_vbox);

    p_hbox.pack_start(p_pester_button, Gtk::PACK_EXPAND_PADDING);
    p_hbox.pack_start(p_close_button, Gtk::PACK_EXPAND_PADDING);

    p_vbox.pack_start(p_label);
    p_vbox.pack_start(p_entry, Gtk::PACK_EXPAND_PADDING);
    p_vbox.pack_start(p_hbox);

    p_pester_button.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::new_memo));
    p_close_button.signal_clicked().connect(sigc::ptr_fun(&Gtk::Main::quit()));

    show_all_children();

    connect(nick);

    signal_init();`
}

void MainWindow::connect()
{
    p_thread.set_text("connecting...");
    boost::thread connect_thread(&MainWindow::ish_init, this);
}

void MainWindow::ish_init()
{
    ish = new IrcSignalHandler(nick);
    if(!ish->is_initialized())
        p_label.set_text("Could not init");
    if(ish->connect())
        p_label.set_text("Could not connect");
}

void MainWindow::signal_init()
{
    ish->sig_connected.connect(
}
