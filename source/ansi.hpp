#pragma once
#include<cstdio>//std::snprintf
#include<utility>//std::forward
#include<string>//std::string
#include<set>//std::set
#include<initializer_list>//std::initializer_list
#include<format>//std::format_string std::format
namespace ansi{
enum class Color:unsigned char{
    Black   ='0',
    Red     ='1',
    Green   ='2',
    Yellow  ='3',
    Blue    ='4',
    Magenta ='5',
    Cyan    ='6',
    White   ='7',
    Default ='9'
};
enum class Style:unsigned char{
    Bold            ='1',
    Italic          ='3',
    UnderLine       ='4',
    StrikeThrough   ='9'
};
class StyleText{
public:
    StyleText(std::string const& text={});
    template<typename...Args_>requires requires{sizeof...(Args_)>0;}
    StyleText(std::format_string<Args_...> const& fmt,Args_&&...args);
    StyleText& text(std::string const& text);
    template<typename...Args_>
    StyleText& format(std::string const& format,Args_&&...args);
    StyleText& style(ansi::Style s);
    StyleText& styles(std::initializer_list<ansi::Style>const& style_list);
    StyleText& fg(ansi::Color c);
    StyleText& bg(ansi::Color c);
    StyleText& color(ansi::Color fg,ansi::Color bg);
    template<typename OStream_>
    friend OStream_& operator<<(OStream_& os,StyleText const& style_text);
private:
    std::string text_;
    std::set<ansi::Style> styles_;
    ansi::Color fg_,bg_;
};
StyleText::StyleText(std::string const& text)
    :text_(text)
    ,styles_({})
    ,fg_(ansi::Color::Default)
    ,bg_(ansi::Color::Default)
{}
template<typename...Args_>requires requires{sizeof...(Args_)>0;}
StyleText::StyleText(std::format_string<Args_...> const& fmt,Args_&&...args)
    :text_(std::format(fmt,std::forward<Args_>(args)...))
    ,styles_({})
    ,fg_(ansi::Color::Default)
    ,bg_(ansi::Color::Default)
{}
StyleText& StyleText::style(ansi::Style s){
    this->styles_.insert(s);
    return *this;
}
StyleText& StyleText::styles(std::initializer_list<ansi::Style>const& style_list){
    for(auto const& s:style_list){
        this->styles_.insert(s);
    }
    return *this;
}
StyleText& StyleText::fg(ansi::Color c){
    this->fg_=c;
    return *this;
}
StyleText& StyleText::bg(ansi::Color c){
    this->bg_=c;
    return *this;
}
StyleText& StyleText::color(ansi::Color fg,ansi::Color bg){
    this->fg_=fg;
    this->bg_=bg;
    return *this;
}
template<typename OStream_>
OStream_& operator<<(OStream_& os,StyleText const& style_text){
    os<<"\033[";
    for(auto const& style:style_text.styles_){
        os<<static_cast<unsigned char>(style)<<';';
    }
    return os
        <<'3'<<static_cast<unsigned char>(style_text.fg_)<<';'
        <<'4'<<static_cast<unsigned char>(style_text.bg_)<<'m'
        <<style_text.text_<<"\033[0m";
}
}//namespace ansi
