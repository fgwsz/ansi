#include"game.h"

#include<cstdlib>//std::system

#include<vector>//std::vector
#include<random>//std::random_device std::mt19937 std::uniform_int_distribution
#include<stdexcept>//std::out_of_range
#include<limits>//std::numeric_limits
#include<string>//std::string std::getline std::stoll std::to_string
#include<iostream>//std::cout std::cin
#include<functional>//std::function
#include<chrono>//std::chrono
#include<thread>//std::this_thread

#include"ansi.hpp"//ansi::TileMap

namespace game{
struct Cell{
    enum class LifeState:unsigned char{
        DEAD=0,
        ALIVE=1
    };
    LifeState life_state_;
    unsigned char alive_neighbors_count_;
    long long age_;// ALIVE Cell.age_>=0,DEAD Cell.age_=-1
};
struct Grid{
    // row in range(0,+Inf)
    // and col in range(0,+Inf)
    // else throw std::out_of_range
    static void init(long long row,long long col);
    static long long row(void)noexcept;
    static long long col(void)noexcept;
    // row in range[0,Grid::row())
    // and col in range[0,Grid::col())
    // else throw std::out_of_range
    static Cell const& at(long long row,long long col);
    static void update(void)noexcept;
    static long long turn_count(void)noexcept;
    static long long alive_count(void)noexcept;
};
struct __CellPos{
    long long row_,col_;
};
static std::vector<std::vector<Cell>> __grid;
static long long __grid_turn_count=0;
static long long __grid_alive_count=0;
static constexpr __CellPos const __cell_dpos_array[8]={
    __CellPos{-1,-1},
    __CellPos{-1, 0},
    __CellPos{-1, 1},
    __CellPos{ 0,-1},
    __CellPos{ 0, 1},
    __CellPos{ 1,-1},
    __CellPos{ 1, 0},
    __CellPos{ 1, 1}
};
static bool const __iostream_init_flag=[](void)noexcept->bool{
    std::cout.tie(nullptr);
    return true;
}();
static inline void __increment(long long& number){
    if(number<std::numeric_limits<long long>::max()){
        ++number;
    }
}
static inline Cell::LifeState __make_random_life_state(void)noexcept{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<unsigned short> distrib(0,1);
    return static_cast<Cell::LifeState>(distrib(gen));
}
long long Grid::row(void)noexcept{
    return static_cast<long long>(__grid.size());
}
long long Grid::col(void)noexcept{
    return static_cast<long long>(__grid[0].size());
}
Cell const& Grid::at(long long row,long long col){
    if(row<0||row>=Grid::row()||col<0||col>=Grid::col()){
        throw std::out_of_range(
            "row must be in range[0,Grid::row())"
            "and col must be in range[0,Grid::col())"
        );
    }
    return __grid[row][col];
}
static inline void __grid_update_alive_neighbors_count(void)noexcept{
    for(long long row=0;row<Grid::row();++row){
        for(long long col=0;col<Grid::col();++col){
            __grid[row][col].alive_neighbors_count_=0;
            for(__CellPos const& dpos:__cell_dpos_array){
                if(
                    __grid
                        [(row+dpos.row_+Grid::row())%Grid::row()]
                        [(col+dpos.col_+Grid::col())%Grid::col()]
                        .life_state_==Cell::LifeState::ALIVE
                ){
                    ++(__grid[row][col].alive_neighbors_count_);
                }
            }
        }
    }
}
static inline void __grid_update_alive_neighbors_count_fast(void)noexcept{
    std::vector<__CellPos> stack;
    for(long long row=0;row<Grid::row();++row){
        for(long long col=0;col<Grid::col();++col){
            __grid[row][col].alive_neighbors_count_=0;
            if(__grid[row][col].life_state_==Cell::LifeState::ALIVE){
                stack.push_back(__CellPos{row,col});
            }
        }
    }
    while(!stack.empty()){
        for(__CellPos const& dpos:__cell_dpos_array){
            ++(
                __grid
                    [(stack.back().row_+dpos.row_+Grid::row())%Grid::row()]
                    [(stack.back().col_+dpos.col_+Grid::col())%Grid::col()]
                    .alive_neighbors_count_
            );
        }
        stack.pop_back();
    }
}
void Grid::init(long long row,long long col){
    if(row<=0||col<=0){
        throw std::out_of_range(
            "row must be in range(0,+Inf)"
            "and col must be in range(0,+Inf)"
        );
    }
    __grid.reserve(row);
    __grid.resize(row);
    __grid_turn_count=0;
    __grid_alive_count=0;
    for(auto& line:__grid){
        line.reserve(col);
        line.resize(col);
        for(auto& cell:line){
            cell.life_state_=__make_random_life_state();
            if(cell.life_state_==Cell::LifeState::ALIVE){
                cell.age_=0;
                __increment(__grid_alive_count);
            }else{
                cell.age_=-1;
            }
        }
    }
    __grid_update_alive_neighbors_count_fast();
}
void Grid::update(void)noexcept{
    __grid_alive_count=0;
    for(auto& line:__grid){
        for(auto& cell:line){
            if(cell.life_state_==Cell::LifeState::ALIVE){
                if(
                    cell.alive_neighbors_count_<2
                    ||cell.alive_neighbors_count_>3
                ){
                    cell.life_state_=Cell::LifeState::DEAD;
                    cell.age_=-1;
                }else{
                    __increment(cell.age_);
                    __increment(__grid_alive_count);
                }
            }else{
                if(cell.alive_neighbors_count_==3){
                    cell.life_state_=Cell::LifeState::ALIVE;
                    cell.age_=0;
                    __increment(__grid_alive_count);
                }
            }
        }
    }
    __grid_update_alive_neighbors_count_fast();
    __increment(__grid_turn_count);
}
long long Grid::turn_count(void)noexcept{
    return __grid_turn_count;
}
long long Grid::alive_count(void)noexcept{
    return __grid_alive_count;
}
static inline long long __read_ll(
    std::string const& prompt,
    std::function<bool(long long)> const& pred
)noexcept{
    std::string input;
    long long ret=0;
    for(;;){
        std::cout<<prompt;
        std::getline(std::cin,input);
        try{
            ret=std::stoll(input);
        }catch(std::exception const& exception){
            std::cout<<exception.what()<<"\n";
            continue;
        }
        if(pred(ret)){
            break;
        }
    }
    return ret;
}
static inline void __console_clear(void)noexcept{
    #ifdef _WIN32
        std::system("cls");
    #else
        std::system("clear");
    #endif
}
static inline std::string __grid_to_string(void)noexcept{
    static ansi::TileMap tile_map={static_cast<size_t>(Grid::col()),static_cast<size_t>(Grid::row())};
    Cell cell={};
    for(long long row=0;row<Grid::row();++row){
        for(long long col=0;col<Grid::col();++col){
            cell=Grid::at(row,col);
            if(cell.age_<0){
                tile_map.set_tile(row,col,
                    ansi::StyleText("-1")
                    .set_color(ansi::Color::Red,ansi::Color::White)
                    .set_style(ansi::Style::Italic)
                );
            }else if(cell.age_<10){
                tile_map.set_tile(row,col,
                    ansi::StyleText(std::string{"0"}+char('0'-0+cell.age_))
                    .set_color(ansi::Color::White,ansi::Color::Green)
                    .set_style(ansi::Style::Bold)
                );
            }else{
                tile_map.set_tile(row,col,
                    ansi::StyleText(std::to_string(cell.age_>99?99:cell.age_))
                    .set_color(ansi::Color::White,ansi::Color::Blue)
                    .set_style(ansi::Style::Bold)
                );
            }
        }
    }
    return tile_map.ansi_string();
}
int exec(void){
    std::cout<<"Game Of Life (Console Mode)\n";
    long long row=__read_ll(
        "Input Row(>0): ",
        [](long long num)->bool{return num>0;}
    );
    long long col=__read_ll(
        "Input Col(>0): ",
        [](long long num)->bool{return num>0;}
    );
    Grid::init(row,col);
    std::chrono::high_resolution_clock::time_point start_time;
    std::chrono::milliseconds duration;
    std::chrono::milliseconds const once_total_time(500);
    for(;;){
        start_time=std::chrono::high_resolution_clock::now();
        __console_clear();
        std::cout<<(
            __grid_to_string()
            +"Turn:"
            +std::to_string(Grid::turn_count())
            +"\tAlive:"
            +std::to_string(Grid::alive_count())
            +"\n"
        );
        if(Grid::alive_count()==0){
            std::cout<<"Game Over!\n";
            break;
        }
        Grid::update();
        duration=std::chrono::duration_cast<decltype(duration)>(
            std::chrono::high_resolution_clock::now()-start_time
        );
        if(duration<once_total_time){
            std::this_thread::sleep_for(once_total_time-duration);
        }
    }
    return 0;
}
}//namespace game
