#include "fetcher.h"
#include <iostream>
#include <limits>
#include <vector>
#include <string>
#include <fstream>

using std::vector;
using std::string;

namespace BooksFetcher {
vector<Book> fetch(string fileName) {
    vector<Book> result;
    std::ifstream ifs(fileName);
    string line;

    //1st line is not valid data
    ifs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    while (std::getline(ifs, line) && ifs) {
        Book book;
        string temp;
        std::getline(ifs, temp, ',');
        std::cerr << temp << " ";
        book.bookId = std::stoi(temp);
        std::getline(ifs, temp, ',');
        std::cerr << temp << " ";
        book.title = temp;
        std::getline(ifs, temp, ',');
        std::cerr << temp << " ";
        book.authors = temp;
        std::getline(ifs, temp, ',');
        std::cerr << temp << " ";
        book.rating = std::stod(temp);
        std::getline(ifs, temp, ',');
        std::cerr << temp << " ";
        book.isbn = temp;
        std::getline(ifs, temp, ',');
        std::cerr << temp << " ";
        book.isbn13 = temp;
        std::getline(ifs, temp, ',');
        std::cerr << temp << " ";
        book.language = temp;
        std::getline(ifs, temp, ',');
        std::cerr << temp << " ";
        book.ratings_count = std::stol(temp);
        std::getline(ifs, temp, ',');
        std::cerr << temp << " ";
        book.text_reviews_count = std::stol(temp);
        std::getline(ifs, temp, ',');
        std::cerr << temp << " ";
        book.publication_date = temp;
        std::getline(ifs, temp, ',');
        std::cerr << temp;
        std::cerr << '\n';
        book.publisher = temp;
        result.push_back(book);
    }
    Book test = result[256];
    return result;
}
} // namespace BooksFetcher
