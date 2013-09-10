#ifndef MAIN_WINDOW_H_INCLUDED
#define MAIN_WINDOW_H_INCLUDED

#include <string>

//#include "gtkmm.h"
#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/entry.h>
#include <gtkmm/label.h>

#include <boost/thread.hpp>
#include "irc_signal_handler.h"

class MainWindow : public Gtk::Window
{
    protected:
        //STUFF
        //Gtkmm stuff
        Gtk::Box    p_vbox;
        Gtk::Box    p_hbox;
        Gtk::Label  p_label;
        Gtk::Button p_pester_button;
        Gtk::Button p_close_button;
        Gtk::Entry  p_entry;

        //IrcSignalHandler
        Glib::RefPtr<IrcSignalHandler> ish;

        //session information
        std::string nick;

        //Functions
        void connect();
        void ish_init();
        void signal_init();
        void new_memo();

    public:
        //Constructors Destructors.
        MainWindow(std::string nick_in);
        virtual ~MainWindow();


}

#endif // MAIN_WINDOW_H_INCLUDED
