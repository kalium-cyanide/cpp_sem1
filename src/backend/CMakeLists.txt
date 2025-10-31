cmake_minimum_required(VERSION 3.15)

add_library(backend
        Boards/BoardState.hpp
        Boards/ClassicBoard.hpp
        Boards/iBoard.hpp

        Figures/Bishop.hpp
        Figures/IFigure.hpp
        Figures/King.hpp
        Figures/Knight.hpp
        Figures/Pawn.hpp
        Figures/Queen.hpp
        Figures/Rook.hpp

        GameCores/ClassicChessCore.cpp
        GameCores/ClassicChessCore.hpp
        GameCores/iGameCore.hpp

        Players/GUIPlayer.hpp
        Players/HumanPlayer.hpp
        Players/IPlayer.hpp
        Players.hpp

        Validators/ClassicChessValidations.hpp
        Validators/Validator.hpp
        Validators/ValidatorIO.hpp

        Figure.hpp
        Board.hpp
        DataFabric.hpp
        Move.hpp
        PossibleMovesGenerator.hpp
        utility.hpp
        Vector2.hpp
        ../frontend/main_window.cpp
        ../frontend/main_window.hpp
)

target_include_directories(backend PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})