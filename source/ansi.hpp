#pragma once
#include<cstdio>//std::snprintf
#include<utility>//std::forward
#include<string>//std::string
#include<set>//std::set
#include<initializer_list>//std::initializer_list
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
StyleText& StyleText::text(std::string const& text){
    this->text_=text;
    return *this;
}
template<typename...Args_>
StyleText& StyleText::format(std::string const& format,Args_&&...args){
    std::size_t len=snprintf(nullptr,0,format.c_str(),std::forward<Args_>(args)...);//预计算长度
    std::string s;
    s.resize(len);//正确设置长度为有效内容长度（不含终止符）
    std::snprintf(&(s[0]),len+1,format.c_str(),std::forward<Args_>(args)...);
    this->text_=s;
    return *this;
}
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
