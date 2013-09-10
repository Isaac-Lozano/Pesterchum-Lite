#ifndef IRC_SIGNAL_HANDLER_H_INCLUDED
#define IRC_SIGNAL_HANDLER_H_INCLUDED

#include <string>

#include <sigc++/sigc++.h>
#include "libircclient.h"

//Eventually, this will be replaced by a modified libircclient that supports sigc.

class IrcSignalHandler
{
    protected:

        //For libircclient
        irc_callbacks_t callbacks;
        irc_session_t * session;

        //IRC
        std::string nick;

        //So that we don't try to use the class without it being
        //properly initialized.
        bool initialized;

        //Callback Functions
        //These are static since we cannot access member-functions
        //through the `this' pointer with c-style callbacks.
        //TODO: Put ALL callbacks here.
        static void irc_join_event();
        static void irc_event_connect();

    public:
        //Constructors Destructors blah blah
        IrcSignalHandler(/*std::string server_in, int port_in, */std::string nick_in);
        ~IrcSignalHandler();

        //MUST be called after constructor to check if things went well.
        bool is_initialized();

        //Connect to server
        int connect(std::string server = "irc.mindfang.org", int port = 6667,
                               std::string username = "pstrchm109", std::string realname = "OnVar's C++ Pesterchum client.");

        //Signals. May be moved to protected with "get" functions that return the signal.
        //sigc::signal<void, IrcSignalHandler*> signal_connected();
        sigc::signal1<void, IrcSignalHandler*>                                        sig_connected;
        sigc::signal3<void, IrcSignalHandler*, std::string, std::string>              sig_nick;
        sigc::signal3<void, IrcSignalHandler*, std::string, std::string>              sig_quit;
        sigc::signal3<void, IrcSignalHandler*, std::string, std::string>              sig_join;
        sigc::signal4<void, IrcSignalHandler*, std::string, std::string, std::string> sig_part;
        //sigc::signal3<void, IrcSignalHandler*,
        //sigc::signal3<void, IrcSignalHandler*,
        sigc::signal4<void, IrcSignalHandler*, std::string, std::string. std::string> sig_topic;
        sigc::signal4<void, IrcSignalHandler*, std::string, std::string, std::string> sig_kick;
        sigc::signal4<void, IrcSignalHandler*, std::string, std::string, std::string> sig_channel;
        sigc::signal4<void, IrcSignalHandler*, std::string, std::string, std::string> sig_talk;
        sigc::signal3<void, IrcSignalHandler*, std::string, std::string>              sig_privmsg;
        sigc::signal4<void, IrcSignalHandler*, std::string, std::string, std::string> sig_priv_notice;
        sigc::signal4<void, IrcSignalHandler*, std::string, std::string, std::string> sig_notice;
        sigc::signal4<void, IrcSignalHandler*, std::string, std::string, std::string> sig_channel_notice;
        sigc::signal3<void, IrcSignalHandler*, std::string, std::string>              sig_invite;
        sigc::signal3<void, IrcSignalHandler*, std::string, std::string>              sig_emote;
}

#endif // IRC_SIGNAL_HANDLER_H_INCLUDED
