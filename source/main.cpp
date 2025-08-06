#include<iostream>
#include"ansi.hpp"
#include"ansi_tilemap.hpp"
int main(int argc,char* argv[]){
    std::cout<<
        ansi::StyleText("{}\n","Hello world!")
        .set_style(ansi::Style::StrikeThrough)
    ;
    std::cout<<
        ansi::StyleText("Hello world!\n")
        .set_styles({ansi::Style::Italic,ansi::Style::Bold,ansi::Style::UnderLine})
        .set_fg(ansi::Color::White)
        .set_bg(ansi::Color::Black)
    ;
    std::cout<<
        ansi::StyleText("Hello world!\n")
        .set_styles({ansi::Style::Italic,ansi::Style::Bold,ansi::Style::UnderLine})
        .set_color(ansi::Color::Cyan,ansi::Color::Red)
    ;
    std::cout<<
        ansi::StyleText("Hello world!\n")
        .set_style(ansi::Style::Italic)
        .set_style(ansi::Style::Bold)
        .set_style(ansi::Style::UnderLine)
        .set_fg(ansi::Color::Magenta)
        .set_bg(ansi::Color::Green)
    ;
    std::cout<<
        ansi::StyleText("Hello world!\n")
        .set_style(ansi::Style::Italic)
        .set_style(ansi::Style::Bold)
        .set_style(ansi::Style::UnderLine)
        .set_fg(ansi::Color::Blue)
        .set_bg(ansi::Color::Yellow)
    ;
    std::cout
        <<ansi::StyleText("  ").set_bg(ansi::Color::Black  )
        <<ansi::StyleText("  ").set_bg(ansi::Color::Red    )
        <<ansi::StyleText("  ").set_bg(ansi::Color::Green  )
        <<ansi::StyleText("  ").set_bg(ansi::Color::Yellow )
        <<ansi::StyleText("  ").set_bg(ansi::Color::Blue   )
        <<ansi::StyleText("  ").set_bg(ansi::Color::Magenta)
        <<ansi::StyleText("  ").set_bg(ansi::Color::Cyan   )
        <<ansi::StyleText("  ").set_bg(ansi::Color::White  )
        <<ansi::StyleText("  ").set_bg(ansi::Color::Default)
        <<ansi::StyleText("\n")
    ;
    std::cout
        <<ansi::StyleText("赢").set_bg(ansi::Color::Black  )
        <<ansi::StyleText("赢").set_bg(ansi::Color::Red    )
        <<ansi::StyleText("赢").set_bg(ansi::Color::Green  )
        <<ansi::StyleText("赢").set_bg(ansi::Color::Yellow )
        <<ansi::StyleText("赢").set_bg(ansi::Color::Blue   )
        <<ansi::StyleText("赢").set_bg(ansi::Color::Magenta)
        <<ansi::StyleText("赢").set_bg(ansi::Color::Cyan   )
        <<ansi::StyleText("赢").set_bg(ansi::Color::White  )
        <<ansi::StyleText("赢").set_bg(ansi::Color::Default)
        <<ansi::StyleText("\n")
    ;
    std::cout<<ansi::StyleText("Hello world!\n")
        .set_styles({ansi::Style::Italic,ansi::Style::Bold,ansi::Style::UnderLine})
        .set_color(ansi::Color::Cyan,ansi::Color::Red)
        .print_string()
        <<'\n';
    auto tile=ansi::StyleText("01");
    tile.set_color(ansi::Color::Yellow,ansi::Color::Blue);
    ansi::TileMap tile_map(10,10,tile);
    tile_map.set_tile(4,4,
            ansi::StyleText()
            .set_text("赢")
            .set_style(ansi::Style::Bold)
            .set_color(ansi::Color::Black,ansi::Color::White)
    )
    .set_tile(0,0,
            ansi::StyleText()
            .set_text("赢麻了")
            .set_style(ansi::Style::Bold)
            .set_color(ansi::Color::Black,ansi::Color::Red)
    );
    std::cout<<tile_map.ansi_string();
    return 0;
}
