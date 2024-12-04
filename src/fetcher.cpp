#include "fetcher.h"
#include <iostream>
#include <limits>
#include <vector>
#include <string>
#include <fstream>

using std::vector;
using std::string;

namespace BooksFetcher {
//fetch books dataset as a vector given a file
vector<Book> fetch(string fileName) {
    vector<Book> result;
    std::ifstream ifs(fileName);
    string line;
    
    if (!ifs) {
        throw std::runtime_error("Dataset could not be opened:\n./DiscoveryTree <dataset_path>");
    }

    //1st line is not valid data
    ifs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    while (std::getline(ifs, line) && ifs) {
        if (ifs.peek() == EOF) break;
        Book book;
        string temp;
        std::getline(ifs, temp, ',');
        book.bookId = std::stoi(temp);
        std::getline(ifs, temp, ',');
        book.title = temp;
        std::getline(ifs, temp, ',');
        book.authors = temp;
        std::getline(ifs, temp, ',');
        book.rating = std::stod(temp);
        std::getline(ifs, temp, ',');
        book.isbn = temp;
        std::getline(ifs, temp, ',');
        book.isbn13 = temp;
        std::getline(ifs, temp, ',');
        book.language = temp;
        std::getline(ifs, temp, ',');
        book.ratings_count = std::stol(temp);
        std::getline(ifs, temp, ',');
        book.text_reviews_count = std::stol(temp);
        std::getline(ifs, temp, ',');
        book.publication_date = temp;
        std::getline(ifs, temp, ',');
        book.publisher = temp;
        result.push_back(book);
    }
    return result;
}
} // namespace BooksFetcher
