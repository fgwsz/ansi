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
    StyleText& at(size_t x,size_t y);
    std::string ansi_string(void)const;
    template<typename OStream_>
    friend OStream_& operator<<(OStream_& os,TileMap const& tile_map);
private:
    size_t width_,height_;
    std::vector<std::vector<StyleText>> map_;
};
TileMap::TileMap(size_t width,size_t height,ansi::StyleText const& tile)
    :width_(width)
    ,height_(height)
    ,map_({}){
    this->map_.resize(this->height_);
    for(auto& line:this->map_){
        line.resize(this->width_,tile);
    }
}
StyleText& TileMap::at(size_t x,size_t y){
    return this->map_.at(x).at(y);
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
}//namespace ansi
