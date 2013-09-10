#include "irc_signal_handler.h"

IrcSignalHandler::IrcSignalHandler(std::string nick_in)
: initialized(false),
  nick(nick_in)
{
    //create pointer to self for singleton mode.
    singleton = this;

    //Initialize callbacks
    memset(&callbacks, 0, sizeof(callbacks));

    //TODO
    //Set ALL callbacks

    callbacks.event_connect = &irc_event_connect;

    session = irc_create_session(&callbacks);

    if(!session)
        return; //something happened. Caught when initialized is cheched and returns false

    /*if(irc_connect(session, "irc.mindfang.org", 6667, 0, nick.c_str(), "pstrchm109", "OnVar's C++ Pesterchum client.")){
        initialized = 3;
        return; //something else happened. see other return
    }*///We don't want this here! If it's here, then event_connect will be called before we can set signals

    //This sets the context variable to the `this' pointer.
    //Now we can have class-based callbacks. \o/
    //(With a bit of work.)
    irc_set_ctx(session, this);

    //Everything went well. `Return' zero.
    initialized = true;
}

IrcSignalHandler::~IrcSignalHandler()
{
    //Nothing yet...
}

bool IrcSignalHandler::is_initialized()
{
    return initialized;
}

int IrcSignalHandler::connect(std::string server = "irc.mindfang.org", int port = 6667,
                               std::string username = "pstrchm109", std::string realname = "OnVar's C++ Pesterchum client.")
{
    if(irc_connect(session, server.c_str(), port, 0, nick.c_str(), username.c_str(), realname.c_str())){
        return 1; //something happened. Will make it more discriptive later.
    }
    return 0
}

//-----------------------CALLBACKS--------------------------
//This one doesn't require any data. Just the object pointer will do.
static void IrcSignalHandler::irc_event_connect(irc_session_t * s, const char * event, const char * origin, const char ** params, unsigned int count)
{
    IrcSignalHandler * ish = (IrcSignalHandler*) irc_get_ctx(s);
    ish.sig_connect(ish);
}

static void IrcSignalHandler::irc_event_nick(irc_session_t * s, const char * event, const char * origin, const char ** params, unsigned int count)
{
    IrcSignalHandler * ish = (IrcSignalHandler*) irc_get_ctx(s);
    std::string old_nick = origin;
    std::string new_nick = params[0];
    ish->sig_nick(ish, old_nick, new_nick);
}

static void IrcSignalHandler::irc_event_quit(irc_session_t * s, const char * event, const char * origin, const char ** params, unsigned int count)
{
    IrcSignalHandler * ish = (IrcSignalHandler*) irc_get_ctx(s);
    std::string user   = origin;
    std::string reason;

    if(count == 1)
        reason = params[0];
    else
        reason = "";

    ish->sig_quit(ish, user, reason);
    //ish->sig_leave(ish, user, reason); //Also called on part. So we can have a signal devoted to people leaving the channel.
    //NOTE: Erm... This will have to be checked first.
}

static void IrcSignalHandler::irc_event_join(irc_session_t * s, const char * event, const char * origin, const char ** params, unsigned int count)
{
    IrcSignalHandler * ish = (IrcSignalHandler*) irc_get_ctx(s);
    std::string user    = origin;
    std::string channel = params[0];
    ish->sig_join(ish, user, channel);
}

static void IrcSignalHandler::irc_event_part(irc_session_t * s, const char * event, const char * origin, const char ** params, unsigned int count)
{
    IrcSignalHandler * ish = (IrcSignalHandler*) irc_get_ctx(s);
    std::string user    = origin;
    std::string channel = params[0];
    std::string reason;

    if(count == 2)
        reason = params[1];
    else
        reason = "";
    //Hmm... Could be changed to 'count == 2 ? reason = params[1] : reason = "";'

    ish->sig_part(ish, user, channel, reason);
    //ish->sig_leave(ish, user, channel); //So we have a signal devoted to people leaving the channel.
    //NOTE: To Be Checked
}

static void IrcSignalHandler::irc_event_mode(irc_session_t * s, const char * event, const char * origin, const char ** params, unsigned int count)
{
    IrcSignalHandler * ish = (IrcSignalHandler*) irc_get_ctx(s);
    std::string user    = origin;
    std::string channel = params[0];
    //TODO: emit the signal with a map<std::string nick, std::string mode>
}

static void IrcSignalHandler::irc_event_umode(irc_session_t * s, const char * event, const char * origin, const char ** params, unsigned int count)
{
    IrcSignalHandler * ish = (IrcSignalHandler*) irc_get_ctx(s);
    //TODO: emit the signa; with probably an array of modes changed.
}

static void IrcSignalHandler::irc_event_topic(irc_session_t * s, const char * event, const char * origin, const char ** params, unsigned int count)
{
    IrcSignalHandler * ish = (IrcSignalHandler*) irc_get_ctx(s);
    std::string user    = origin;
    std::string channel = params[0];
    std::string new_topic;

    if(params == 2)
        new_topic = params[1];
    else
        new_topic = "";

    sig_topic(ish, user, channel, new_topic);
}

static void IrcSignalHandler::irc_event_kick(irc_session_t * s, const char * event, const char * origin, const char ** params, unsigned int count)
{
    IrcSignalHandler * ish = (IrcSignalHandler*) irc_get_ctx(s);
    std::string user    = origin;
    std::string channel = params[0];
    std::string to_user = "";
    std::string reason  = "";

    //Libircclient says the nick and reason are optional.
    //I don't know of any time that the nick that was kicked
    //wouldn't be there.
    if(count >= 2){
        to_user = params[1];
        if(count == 3)
            reason = params[2];
    }

    sig_kick(ish, user, channel, to_user, reason);
}

static void IrcSignalHandler::irc_event_channel(irc_session_t * s, const char * event, const char * origin, const char ** params, unsigned int count)
{
    IrcSignalHandler * ish = (IrcSignalHandler*) irc_get_ctx(s);
    std::string user    = origin;
    std::string channel = params[0];
    std::string text;

    if(count == 2)
        text = params[1];
    else
        text = "";

    sig_channel(ish, user, channel, text);
    sig_talk(ish, user, channel, text); //For broader signal handling. basically anytime a privmsg happens.
                                        //(This kind and that kind *points down*.)
}

static void IrcSignalHandler::irc_event_privmsg(irc_session_t * s, const char * event, const char * origin, const char ** params, unsigned int count)
{
    IrcSignalHandler * ish = (IrcSignalHandler*) irc_get_ctx(s);
    std::string user    = origin;
    std::string channel = origin;
    std::string text;

    if(count == 2)
        text = params[1];
    else
        text = "";

    sig_privmsg(ish, user, text);
    sig_talk(ish, user, user, text); //Yes, the second and third arguments are the same.
}

static void IrcSignalHandler::irc_event_notice(irc_session_t * s, const char * event, const char * origin, const char ** params, unsigned int count)
{
    IrcSignalHandler * ish = (IrcSignalHandler*) irc_get_ctx(s);
    std::string user    = origin;
    std::string to_nick = params[0]; //probably our nick. libircclient says "the target nick name". :shrug:
    std::string text;

    if(count == 2)
        text = params[1];
    else
        text = "";

    sig_priv_notice(ish, user, to_nick, text); //Again, assuming that this is a private notice.
                                               //Or a wall notice... Hm...
    sig_notice(ish, user, to_nick, text); // Again. Broader signals.
}

static void IrcSignalHandler::irc_event_channel_notice(irc_session_t * s, const char * event, const char * origin, const char ** params, unsigned int count)
{
    IrcSignalHandler * ish = (IrcSignalHandler*) irc_get_ctx(s);
    std::string user    = origin;
    std::string channel = params[0];
    std::string text;

    if(count == 2)
        text = params[1];
    else
        text = "";

    sig_channel_notice(ish, user, channel, text);
    sig_notice(ish, user, channel, text);
}

static void IrcSignalHandler::irc_event_invite(irc_session_t * s, const char * event, const char * origin, const char ** params, unsigned int count)
{
    IrcSignalHandler * ish = (IrcSignalHandler*) irc_get_ctx(s);
    std::string user = origin;
    std::string channel;

    if(count == 2)
        channel = params[1];
    else
        channel = ""; //Maybe I should return at this point...

    sig_invite(ish, user, channel);
}

/*static void IrcSignalHandler::irc_event_ctcp_req(irc_session_t * s, const char * event, const char * origin, const char ** params, unsigned int count)
{

}

static void IrcSignalHandler::irc_event_ctcp_rep(irc_session_t * s, const char * event, const char * origin, const char ** params, unsigned int count)
{

}*/

static void IrcSignalHandler::irc_event_ctcp_action(irc_session_t * s, const char * event, const char * origin, const char ** params, unsigned int count)
{
    IrcSignalHandler * ish = (IrcSignalHandler*) irc_get_ctx(s);
    std::string user = origin;
    std::string text = params[0];

    sig_emote(ish, user, text);
}//Woo! Implemented all the events! *sillydance*
//...Wait,,, How do I change windows without a mouse...
//Darnit...

//Eh... I'm not sure DCC should be included in Pesterchum...
//So no handlers for that.
