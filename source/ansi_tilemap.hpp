#include <cctype>//std::isprint std::iscntrl
#include <cstdint>//uit32_t
#include<vector>//std::vector
#include"ansi.hpp"
namespace ansi{
/*
(0,0)-->(x,0) x增长的方向
  |
  V
(y,0) y增长的方向
*/
class TileMap{
public:
    TileMap(size_t width,size_t height,ansi::StyleText const& tile={});
    ansi::StyleText const& get_tile(size_t x,size_t y)const;
    TileMap& set_tile(size_t x,size_t y,ansi::StyleText const& tile);
    std::string ansi_string(void)const;
    template<typename OStream_>
    friend OStream_& operator<<(OStream_& os,TileMap const& tile_map);
private:
    static bool is_chinese_character(uint32_t code_point);
    static bool is_display_width_two(const std::string& str);
    size_t width_,height_;
    std::vector<std::vector<StyleText>> map_;
};
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
bool check_tile_text(std::string const& text){
    return true;
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
