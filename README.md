------README------
Stack Squad's DiscoveryTree

About DiscoveryTree:
DiscoveryTree is a C++ application designed to efficiently organize and sort through large datasets 
of book information. It helps users quickly locate books by using advanced data structures for 
sorting by ISBN13 numbers and ratings.

Prerequisites (To run DiscoveryTree, ensure the following tools and dependencies are installed):
C++ Compiler (e.g., GCC or Clang)
Nix Package Manager
Libraries -> FTXUI
Standard C++ libraries -> algorithm, queue, iostream, vector, string, fstream, etc.


------How to Run the Program------
Clone the Repository:
https://github.com/0ntu/DiscoveryTree.git

Set Up Environment:
You can install and use Nix if you haven’t already, and use it to set up the required 
dependencies.

Build the Program:
Compile the application using the provided Makefile.

Provide Input Data:
The program accepts a CSV file containing book data. You can either use the provided file, or 
create/find your own. If not using provided CSV file, ensure the file is formatted as 
follows...
ID,Title,Authors,Average Rating,ISBN,ISBN13,Language Code,Number of Pages,Number of Ratings,Number of Reviews,Publication Date,Publisher

Run the Program:
You can now interact with the UI. Use the provided menu options to sort and search through the 
dataset.


------Project Roles------
Dia Fallon:
Designed and implemented the B+ Tree. Authored the project report.

Cainan Medeiros:
Designed and implemented the Max Heap. Authored this README file.

Nathan Padriga:
Developed the user interface and CSV file parser. Recorded the demo video.
