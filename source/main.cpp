#include<iostream>
#include"ansi.hpp"
int main(int argc,char* argv[]){
    std::cout<<
        ansi::StyleText("{}\n","Hello world!")
        .style(ansi::Style::StrikeThrough)
    ;
    std::cout<<
        ansi::StyleText("Hello world!\n")
        .styles({ansi::Style::Italic,ansi::Style::Bold,ansi::Style::UnderLine})
        .fg(ansi::Color::White)
        .bg(ansi::Color::Black)
    ;
    std::cout<<
        ansi::StyleText("Hello world!\n")
        .styles({ansi::Style::Italic,ansi::Style::Bold,ansi::Style::UnderLine})
        .color(ansi::Color::Cyan,ansi::Color::Red)
    ;
    std::cout<<
        ansi::StyleText("Hello world!\n")
        .style(ansi::Style::Italic)
        .style(ansi::Style::Bold)
        .style(ansi::Style::UnderLine)
        .fg(ansi::Color::Magenta)
        .bg(ansi::Color::Green)
    ;
    std::cout<<
        ansi::StyleText("Hello world!\n")
        .style(ansi::Style::Italic)
        .style(ansi::Style::Bold)
        .style(ansi::Style::UnderLine)
        .fg(ansi::Color::Blue)
        .bg(ansi::Color::Yellow)
    ;
    std::cout
        <<ansi::StyleText("  ").bg(ansi::Color::Black  )
        <<ansi::StyleText("  ").bg(ansi::Color::Red    )
        <<ansi::StyleText("  ").bg(ansi::Color::Green  )
        <<ansi::StyleText("  ").bg(ansi::Color::Yellow )
        <<ansi::StyleText("  ").bg(ansi::Color::Blue   )
        <<ansi::StyleText("  ").bg(ansi::Color::Magenta)
        <<ansi::StyleText("  ").bg(ansi::Color::Cyan   )
        <<ansi::StyleText("  ").bg(ansi::Color::White  )
        <<ansi::StyleText("  ").bg(ansi::Color::Default)
        <<ansi::StyleText("\n")
    ;
    return 0;
}
