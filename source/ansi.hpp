#pragma once
#include<cstdio>//std::snprintf
#include<utility>//std::forward
#include<string>//std::string
#include<set>//std::set
#include<initializer_list>//std::initializer_list
#include<format>//std::format_string std::format
#include<unordered_map>//std::unordered_map
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
static std::unordered_map<ansi::Color,std::string>const color_to_string={
    {ansi::Color::Black  ,"ansi::Color::Black"  },
    {ansi::Color::Red    ,"ansi::Color::Red"    },
    {ansi::Color::Green  ,"ansi::Color::Green"  },
    {ansi::Color::Yellow ,"ansi::Color::Yellow" },
    {ansi::Color::Blue   ,"ansi::Color::Blue"   },
    {ansi::Color::Magenta,"ansi::Color::Magenta"},
    {ansi::Color::Cyan   ,"ansi::Color::Cyan"   },
    {ansi::Color::White  ,"ansi::Color::White"  },
    {ansi::Color::Default,"ansi::Color::Default"}
};
enum class Style:unsigned char{
    Bold            ='1',
    Italic          ='3',
    UnderLine       ='4',
    StrikeThrough   ='9'
};
static std::unordered_map<ansi::Style,std::string>const style_to_string={
    {ansi::Style::Bold          ,"ansi::Style::Bold"         },
    {ansi::Style::Italic        ,"ansi::Style::Italic"       },
    {ansi::Style::UnderLine     ,"ansi::Style::UnderLine"    },
    {ansi::Style::StrikeThrough ,"ansi::Style::StrikeThrough"}
};
class StyleText{
public:
    StyleText(std::string const& text={});
    template<typename...Args_>requires requires{sizeof...(Args_)>0;}
    StyleText(std::format_string<Args_...> const& fmt,Args_&&...args);
    StyleText& set_text(std::string const& text);
    template<typename...Args_>
    StyleText& set_text_format(std::string const& fmt,Args_&&...args);
    StyleText& set_style(ansi::Style s);
    StyleText& set_styles(std::initializer_list<ansi::Style>const& style_list);
    StyleText& set_fg(ansi::Color c);
    StyleText& set_bg(ansi::Color c);
    StyleText& set_color(ansi::Color fg,ansi::Color bg);
    std::string ansi_string(void)const;
    template<typename OStream_>
    friend OStream_& operator<<(OStream_& os,StyleText const& style_text);
    std::string const& get_text(void)const;
    std::set<ansi::Style>const& get_styles(void)const;
    ansi::Color get_fg(void)const;
    ansi::Color get_bg(void)const;
    std::string print_string(void)const;
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
StyleText& StyleText::set_text(std::string const& text){
    this->text_=text;
    return *this;
}
template<typename...Args_>
StyleText& StyleText::set_text_format(std::string const& fmt,Args_&&...args){
    this->text_=std::format(fmt,std::forward<Args_>(args)...);
    return *this;
}
StyleText& StyleText::set_style(ansi::Style s){
    this->styles_.insert(s);
    return *this;
}
StyleText& StyleText::set_styles(std::initializer_list<ansi::Style>const& style_list){
    for(auto const& s:style_list){
        this->styles_.insert(s);
    }
    return *this;
}
StyleText& StyleText::set_fg(ansi::Color c){
    this->fg_=c;
    return *this;
}
StyleText& StyleText::set_bg(ansi::Color c){
    this->bg_=c;
    return *this;
}
StyleText& StyleText::set_color(ansi::Color fg,ansi::Color bg){
    this->fg_=fg;
    this->bg_=bg;
    return *this;
}
std::string StyleText::ansi_string(void)const{
    std::string ret;
    //\033 [ x ; y ; z ; 3  ?  ;  4  ?  m{text_}\033 [  0  m
    //1    2 3 4 5 6 7 8 9 10 11 12 13 14       15  16 17 18
    ret.reserve(18+this->text_.size());
    ret+="\033[";
    for(auto const& style:this->styles_){
        ret+=static_cast<unsigned char>(style);
        ret+=';';
    }
    ret+='3';
    ret+=static_cast<unsigned char>(this->fg_);
    ret+=';';
    ret+='4';
    ret+=static_cast<unsigned char>(this->bg_);
    ret+='m';
    ret+=this->text_;
    ret+="\033[0m";
    return ret;
}
template<typename OStream_>
OStream_& operator<<(OStream_& os,StyleText const& style_text){
    os<<style_text.ansi_string();
    return os;
}
std::string const& StyleText::get_text(void)const{
    return this->text_;
}
std::set<ansi::Style>const& StyleText::get_styles(void)const{
    return this->styles_;
}
ansi::Color StyleText::get_fg(void)const{
    return this->fg_;
}
ansi::Color StyleText::get_bg(void)const{
    return this->bg_;
}
std::string StyleText::print_string(void)const{
    std::string ansi_string=this->ansi_string();
    std::string text;
    //'\033'->"\\033":size+3
    text.reserve(this->text_.size()+6);
    for(auto const& ch:ansi_string){
        if(ch=='\033'){
            text+="\\033";
        }else if(ch=='\n'){
            text+="\\n";
        }else if(ch=='\t'){
            text+="\\t";
        }else{
            text+=ch;
        }
    }
    std::string styles{"{"};
    for(auto const& style:this->styles_){
        styles+=ansi::style_to_string.at(style);
        styles+=',';
    }
    if(!this->styles_.empty()){
        styles.pop_back();//remove last ','
    }
    styles+='}';
    return std::format(
        "class ansi::StyleText[text_:\"{}\",styles_:{},fg_:{},bg_:{}]"
        ,text
        ,styles
        ,ansi::color_to_string.at(this->fg_)
        ,ansi::color_to_string.at(this->bg_)
    );
}
}//namespace ansi
