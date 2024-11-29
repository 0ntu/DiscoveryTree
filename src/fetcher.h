#pragma once
#include <string>
#include <vector>
namespace BooksFetcher {
struct Book {
  int bookId;
  std::string title;
  std::string authors;
  double rating;
  std::string isbn;
  std::string isbn13;
  std::string language;
  long ratings_count;
  long text_reviews_count;
  std::string publication_date;
  std::string publisher;
};
std::vector<Book> fetch(std::string fileName);
} // namespace BooksFetcher
