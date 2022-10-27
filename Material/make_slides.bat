@echo off

set FILES=
set FILES=%FILES% .\header.md
set FILES=%FILES% .\miscellaneous\object_orientation.md
set FILES=%FILES% .\miscellaneous\functional_programming.md
set FILES=%FILES% .\tasks\TicTacToe_1.md
set FILES=%FILES% .\unittests\unit_test.md
set FILES=%FILES% .\ClassDesign\DataHolder\data_holder.md
set FILES=%FILES% .\tasks\TicTacToe_2.md
set FILES=%FILES% .\ClassDesign\IOClasses\io_classes.md
set FILES=%FILES% .\tasks\TicTacToe_3.md
set FILES=%FILES% .\ClassDesign\BusinessLogic\business_logic.md
set FILES=%FILES% .\tasks\TicTacToe_4.md
set FILES=%FILES% .\ClassDesign\ResourceManager\resource_manager.md
set FILES=%FILES% .\FunctionDesign\function_design.md

REM Course material
echo Course part 1
pandoc -s --slide-level=3 --number-sections --self-contained -t slidy %FILES% -o .\slidesPart1.html

REM Lecture materials
echo Lecture unit tests
pandoc -s --slide-level=3 --number-sections --self-contained -t slidy .\header.md .\unittests\unit_test.md                      -o .\unit_test.html
echo Lecture data holder
pandoc -s --slide-level=3 --number-sections --self-contained -t slidy .\header.md .\ClassDesign\DataHolder\data_holder.md       -o .\data_holder.html
echo Lecture IO classes
pandoc -s --slide-level=3 --number-sections --self-contained -t slidy .\header.md .\ClassDesign\IOClasses\io_classes.md         -o .\io_classes.html
echo Lecture business logic
pandoc -s --slide-level=3 --number-sections --self-contained -t slidy .\header.md .\ClassDesign\BusinessLogic\business_logic.md -o .\business_logic.html
echo Lecture function design
pandoc -s --slide-level=3 --number-sections --self-contained -t slidy .\header.md .\FunctionDesign\function_design.md           -o .\function_design.html


echo Done
