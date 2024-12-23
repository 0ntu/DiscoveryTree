#pragma once
#include "b_plus.h"
#include "fetcher.h"
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include "max_heap.h"
using namespace ftxui;

class Ui {
public:
  //UI is a singleton, creates one instance and returns the copy on subsequent calls
  static Ui &getInstance(const std::vector<BooksFetcher::Book> &books,
                         max_heap &heap, b_plus &bptree);
  
  //This function does not return,
  //Start the UI and display to the console
  void loop();

private:
  Ui(const std::vector<BooksFetcher::Book> &books, max_heap &heap,
     b_plus &bptree);

  // UI is a singleton, creation should be called from Ui::getInstance(books)
  Component render();

  // UI is a singleton, remove all copy & assignment operations
  Ui(Ui const &) = delete;
  void operator=(Ui const &) = delete;

  //Build the interface as an element, to be rendered by render()
  Element buildInterface();

  // Render component to element
  Element renderPrimary();
  Element renderTabline();

  // Handles Layout for each part of the UI
  void buildTabline();
  void buildBrowseMenu();
  void buildSuggestedMenu();
  void buildBookmarksMenu();
  void buildCoreSuggestedMenu();
  void buildSettingsMenu();
  void buildAboutMenu();

  //Used to display the book in the scrollable table
  static const Element renderBookDetails(BooksFetcher::Book book);

  ScreenInteractive screen;
  int tab_index = 0;
  std::vector<std::string> tab_options;
  std::vector<std::string> settings_data_structures;
  Component tab_selection;
  Component exit_button;
  Component title_container;
  Component saved_container;
  Component browse_split;
  Component suggested_split;
  Component bookmarks_split;
  Component settings_ds_menu;
  Component suggested_buttons;
  Element book_display;
  Element aboutElement;
  int split_size_browse;
  int split_size_suggested;
  int split_size_bookmarks;
  int browse_selected;
  int settings_selected;
  int bookmarks_selected;
  Book current_suggested_book;
  std::vector<BooksFetcher::Book> saved_books;
  size_t bp_index = 0;
  std::vector<BooksFetcher::Book> bp_sorted;
  const std::vector<BooksFetcher::Book> &books;

  max_heap heap;
  b_plus bptree;

};
