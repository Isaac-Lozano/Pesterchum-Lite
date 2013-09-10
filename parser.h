/*void pesterchum_parse_markup(Gtk::TextBuffer * buffer, Gtk::TextBuffer::iterator it)
{
    Gtk::TextBuffer::iterator match_start, dummy_1, tag_end, start_;

    while(it.forward_search("<", Gtk::TEXT_SEARCH_TEXT_ONLY, match_start, dummy_1)){
    
        if(end_tag = pesterchum_is_opening_tag(match_start)){
            *buffer->erase(match_start, tag_end);
            buffer->pesterchum_parse_markup(buffer, match_start);

        }
        else if(end_tag = pesterchum_is_closing_tag(match_start)){
            *buffer->erase(match_start, tag_end);
            return match_start;
        }
    }
}

Gtk::TextBuffer pesterchum_parse_markup(Gtk::TextBuffer * buffer)
{
    Gtk::TextBuffer::iterator it = buffer->start();
    pesterchum_parse_markup(buffer, it);
}*/

struct TagData{


//<m>This is in <b>bold and <u>underlined</u> still <s>bored</s> bold</b>.</m>
void pesterchum_parse_markup(Gtk::TextBuffer * buffer, Gtk::TextBuffer::iterator it)
{
    Gtk::TextBuffer::iterator match_start, dummy;
    Gtk::TextBuffer::iterator start_search = it;
    Glib::RefPtr<Gtk::TextBuffer::mark> begin_mark = new Gtk::TextBuffer::mark();
    Glib::RefPtr<Gtk::TextBuffer::mark> end_mark = new Gtk::TextBuffer::mark();

    while(start_search.forward_search('<', Gtk::TEXT_SEARCH_TEXT_ONLY, match_start, dummy))
    {
        start_search = match_start;
        if(pestarchum_is_opening_tag(match_start)){
            begin_mark = buffer->add_mark(begin_mark, match_start);
        }
        else if(pesterchum_is_ending_tag(match_start)){
            end_mark = buffer->add_mark(end_mark, match_start);
            break;
        }
    }
    pesterchum_erase_tag(buffer, begin_mark);
    pesterchum_erase_tag(buffer, end_mark);
    pesterchum_parse_markup(buffer, it);
}
