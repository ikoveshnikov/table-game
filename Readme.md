Table Game
==========

Compilation
-----------

Required:
* CMake >= 3.1 
* gcc >= 4.7
* Doxygen (optional)
* Boost (optional)

To compile simple run:
    cmake .
    make

Optional
--------

If you have Doxygen available, API documentation will be compilled in your build directory.
No additional actions needed.

If you would like to compile unit tests as well use this CMake options:
    cmake . -DTESTS=yes
    make
    make test

Run
---

Run application calling:
    ./table_game -f %file_name%

option '-d' can be used to run in debug mode
