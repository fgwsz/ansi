#ifndef ANSI_HPP
#define ANSI_HPP

#include<cctype>//std::isprint std::iscntrl
#include<cstdint>//uit32_t
#include<cstdio>//std::snprintf

#include<utility>//std::forward
#include<string>//std::string
#include<set>//std::set
#include<initializer_list>//std::initializer_list
#include<format>//std::format_string std::format
#include<unordered_map>//std::unordered_map
#include<vector>//std::vector

namespace ansi{
//============================================================================
//                                  API                                       
//============================================================================
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
    inline StyleText(std::string const& text={});
    template<typename...Args_>requires requires{sizeof...(Args_)>0;}
    inline StyleText(std::format_string<Args_...> const& fmt,Args_&&...args);
    inline StyleText& set_text(std::string const& text);
    template<typename...Args_>
    inline StyleText& set_text_format(std::string const& fmt,Args_&&...args);
    inline StyleText& set_style(ansi::Style s);
    inline StyleText& set_styles(std::initializer_list<ansi::Style>const& style_list);
    inline StyleText& set_fg(ansi::Color c);
    inline StyleText& set_bg(ansi::Color c);
    inline StyleText& set_color(ansi::Color fg,ansi::Color bg);
    inline std::string ansi_string(void)const;
    template<typename OStream_>
    inline friend OStream_& operator<<(OStream_& os,StyleText const& style_text);
    inline std::string const& get_text(void)const;
    inline std::set<ansi::Style>const& get_styles(void)const;
    inline ansi::Color get_fg(void)const;
    inline ansi::Color get_bg(void)const;
    inline std::string print_string(void)const;
private:
    std::string text_;
    std::set<ansi::Style> styles_;
    ansi::Color fg_,bg_;
};
// (0,0)-->(x,0) x增长的方向
//   |
//   V
// (y,0) y增长的方向
class TileMap{
public:
    inline TileMap(size_t width,size_t height,ansi::StyleText const& tile={});
    inline ansi::StyleText const& get_tile(size_t x,size_t y)const;
    inline TileMap& set_tile(size_t x,size_t y,ansi::StyleText const& tile);
    inline std::string ansi_string(void)const;
    template<typename OStream_>
    inline friend OStream_& operator<<(OStream_& os,TileMap const& tile_map);
private:
    inline static bool is_chinese_character(uint32_t code_point);
    inline static bool is_display_width_two(const std::string& str);
    size_t width_,height_;
    std::vector<std::vector<StyleText>> map_;
};
//============================================================================
//                                  IMPL                                      
//============================================================================
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
static std::unordered_map<ansi::Style,std::string>const style_to_string={
    {ansi::Style::Bold          ,"ansi::Style::Bold"         },
    {ansi::Style::Italic        ,"ansi::Style::Italic"       },
    {ansi::Style::UnderLine     ,"ansi::Style::UnderLine"    },
    {ansi::Style::StrikeThrough ,"ansi::Style::StrikeThrough"}
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
TileMap::TileMap(size_t width,size_t height,ansi::StyleText const& tile)
    :width_(width)
    ,height_(height)
    ,map_({}){
    this->map_.resize(this->height_);
    for(auto& line:this->map_){
        if(this->is_display_width_two(tile.get_text())){
            line.resize(this->width_,tile);
        }else{
            line.resize(this->width_,ansi::StyleText("  "));
        }
    }
}
ansi::StyleText const& TileMap::get_tile(size_t x,size_t y)const{
    return this->map_.at(x).at(y);
}
TileMap& TileMap::set_tile(size_t x,size_t y,ansi::StyleText const& tile){
    this->map_.at(x).at(y)=tile;
    if(!this->is_display_width_two(tile.get_text())){
        this->map_.at(x).at(y).set_text("  ");
    }
    return *this;
}
std::string TileMap::ansi_string(void)const{
    std::string ret;
    //(18+3)*width*height+1('\0')
    //18:style_text
    //3:style_text.text_ max strlen
    //每个Tile只能显示一个中文汉字(3字节)/2个英文字符(2字节)
    ret.reserve(21*this->width_*this->height_+1);
    for(auto const& line:this->map_){
        for(auto const& tile:line){
            ret+=tile.ansi_string();
        }
        ret+='\n';
    }
    return ret;
}
template<typename OStream_>
OStream_& operator<<(OStream_& os,TileMap const& tile_map){
    os<<tile_map.ansi_string();
    return os;
}
// 判断Unicode码点是否是中文字符
bool TileMap::is_chinese_character(uint32_t code_point){
    // 基本汉字区 (U+4E00 - U+9FFF)
    if(code_point >= 0x4E00 && code_point <= 0x9FFF){
        return true;
    }
    // 扩展A区 (U+3400 - U+4DBF)
    if(code_point >= 0x3400 && code_point <= 0x4DBF){
        return true;
    }
    // 扩展B-F区 (U+20000 - U+2FFFF)
    if(code_point >= 0x20000 && code_point <= 0x2FFFF){
        return true;
    }
    // 兼容汉字区 (U+F900 - U+FAFF)
    if(code_point >= 0xF900 && code_point <= 0xFAFF){
        return true;
    }
    return false;
}
// 判断UTF-8字符串显示宽度是否为2
bool TileMap::is_display_width_two(const std::string& str){
    int total_width = 0;
    size_t i = 0;
    while (i < str.size()){
        unsigned char c = static_cast<unsigned char>(str[i]);
        size_t char_len = 0;
        uint32_t code_point = 0;
        // 确定字符长度和计算码点
        if(c <= 0x7F){
            // 1字节字符 (ASCII)
            char_len = 1;
            code_point = c;
        } else if((c & 0xE0) == 0xC0){
            // 2字节字符
            char_len = 2;
            if(i + 1 >= str.size()) return false;
            code_point = (c & 0x1F) << 6;
            code_point |= (str[i + 1] & 0x3F);
        } else if((c & 0xF0) == 0xE0){
            // 3字节字符
            char_len = 3;
            if(i + 2 >= str.size()) return false;
            code_point = (c & 0x0F) << 12;
            code_point |= (str[i + 1] & 0x3F) << 6;
            code_point |= (str[i + 2] & 0x3F);
        } else if((c & 0xF8) == 0xF0){
            // 4字节字符
            char_len = 4;
            if(i + 3 >= str.size()) return false;
            code_point = (c & 0x07) << 18;
            code_point |= (str[i + 1] & 0x3F) << 12;
            code_point |= (str[i + 2] & 0x3F) << 6;
            code_point |= (str[i + 3] & 0x3F);
        }else{
            // 无效的UTF-8起始字节
            return false;
        }
        // 验证字符长度是否在字符串范围内
        if(i + char_len > str.size()){
            return false;
        }
        // 计算字符宽度
        if(char_len == 1){
            // ASCII字符
            if(std::isprint(c) && !std::iscntrl(c)){
                total_width += 1;
            }else{
                // 控制字符不计宽度,直接返回
                return false;
            }
        }else{
            // 多字节字符
            if(is_chinese_character(code_point)){
                total_width += 2; // 中文字符宽度为2
            }else{
                total_width += 1; // 其他多字节字符宽度为1
            }
        }
        // 如果总宽度已超过2，提前返回false
        if(total_width > 2){
            return false;
        }
        i += char_len; // 移动到下一个字符
    }
    // 最终宽度必须恰好为2
    return total_width == 2;
}
}//namespace ansi

#endif //!ANSI_HPP
