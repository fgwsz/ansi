#include<iostream>
#include"ansi.hpp"
int main(int argc,char* argv[]){
    std::cout<<
        ansi::StyleText()
        .format("%s\n","Hello world!")
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
        ansi::StyleText()
        .text("Hello world!\n")
        .style(ansi::Style::Italic)
        .style(ansi::Style::Bold)
        .style(ansi::Style::UnderLine)
        .fg(ansi::Color::Blue)
        .bg(ansi::Color::Yellow)
    ;
    return 0;
}
