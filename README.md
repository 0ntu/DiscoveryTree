```
  _____  _                                _______            
 |  __ \(_)                              |__   __|           
 | |  | |_ ___  ___ _____   _____ _ __ _   _| |_ __ ___  ___ 
 | |  | | / __|/ __/ _ \ \ / / _ \ '__| | | | | '__/ _ \/ _ \
 | |__| | \__ \ (_| (_) \ V /  __/ |  | |_| | | | |  __/  __/
 |_____/|_|___/\___\___/ \_/ \___|_|   \__, |_|_|  \___|\___|
                                        __/ |                
                                       |___/                 
                                              By: Stack Squad
```
<img src="https://raw.githubusercontent.com/catppuccin/catppuccin/main/assets/palette/macchiato.png" width="600px" /> <br>

A fast and free book finder for the 90s!

About
------
DiscoveryTree is a C++ application designed to efficiently organize and sort through large datasets 
of book information. It helps users quickly locate books by using advanced data structures for 
sorting by ISBN13 numbers and ratings.

Prerequisites (To run DiscoveryTree, ensure the following tools and dependencies are installed):
C++ Compiler (e.g., GCC or Clang)
Nix Package Manager
Libraries -> FTXUI
Standard C++ libraries -> algorithm, queue, iostream, vector, string, fstream, etc.

Screenshots
----------
![Screenshot_2024-12-04_13-53-04](https://github.com/user-attachments/assets/ddabb5d6-b831-45cc-9e87-144c6da15ba1)
![Screenshot_2024-12-04_13-53-26](https://github.com/user-attachments/assets/865c8198-bf5f-4447-af3b-fa18496155b2)
![Screenshot_2024-12-04_13-53-48](https://github.com/user-attachments/assets/a6ac56fc-2c8b-4478-aeb5-f271e236cc92)


Installation with Nix (Preferred)
----------------------
Zero-click Run
```
nix run github:0ntu/DiscoveryTree
```

OR

Build the derivation
```
git clone https://github.com/0ntu/DiscoveryTree.git
cd DiscoveryTree
nix build .
```

Installation (Manual)
----------------------

Clone the Repository:
```
git clone https://github.com/0ntu/DiscoveryTree.git
```

Set Up Environment:
Ensure you have `cmake`, `make`, and the `FTXUI` library

Building:
Ideally, you should be able to just type:
```
cmake .
make
```

Usage
-----

Provide Input Data:
The program accepts a CSV file containing book data. You can either use the provided file, or 
create/find your own. If not using provided CSV file, ensure the file is formatted as 
follows...
ID,Title,Authors,Average Rating,ISBN,ISBN13,Language Code,Number of Pages,Number of Ratings,Number of Reviews,Publication Date,Publisher

Run the Program:
You can now interact with the UI. Use the provided menu options to sort and search through the 
dataset.


Project Roles
-------------

Dia Fallon:
Designed and implemented the B+ Tree. Authored the project report.

Cainan Medeiros:
Designed and implemented the Max Heap. Authored this README file.

Nathan Padriga:
Developed the user interface and CSV file parser. Recorded the demo video.
