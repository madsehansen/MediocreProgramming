@echo off

set FILES=
set FILES=%FILES% .\header.md
set FILES=%FILES% .\miscellaneous\object_orientation.md
set FILES=%FILES% .\miscellaneous\functional_programming.md
set FILES=%FILES% .\unittests\unit_test.md
set FILES=%FILES% .\ClassDesign\DataHolder\data_holder.md
set FILES=%FILES% .\ClassDesign\IOClasses\io_classes.md
set FILES=%FILES% .\ClassDesign\BusinessLogic\business_logic.md

pandoc -s --slide-level=3 --number-sections -t slidy %FILES% -o .\slides.html
