cmake_minimum_required(VERSION 3.30)

add_module(parser xml_parser.ixx)

add_library(gui_engine
        xml_parser.cppm
        css_parser.cppm
        main.cpp
)

add_dependencies(gui_engine parser.pcm)