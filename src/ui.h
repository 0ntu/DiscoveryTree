#pragma once
#include "fetcher.h"
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
using namespace ftxui;

class Ui {
public:
  static Ui &getInstance(const std::vector<BooksFetcher::Book> &books);
  void loop();

private:
  // UI is a singleton, creation should be called from Ui::getInstance(books)
  Component render();
  Ui(const std::vector<BooksFetcher::Book> &books);

  // UI is a singleton, remove all copy & assignment operations
  Ui(Ui const &) = delete;
  void operator=(Ui const &) = delete;

  Element buildInterface();
  
  Element renderPrimary();
  Element renderTabline();

  ScreenInteractive screen;
  int tab_index = 0;
  std::vector<std::string> tab_options;
  std::vector<std::string> settings_data_structures;
  Component tab_selection;
  Component exit_button;
  Component title_container;
  Component browse_split;
  Component suggested_split;
  Component settings_ds_menu;
  Element book_display;
  int split_size_browse;
  int split_size_suggested;
  int selected;
  int settings_selected;
  const std::vector<BooksFetcher::Book> &books;

  static const Element renderBookDetails(BooksFetcher::Book book);
};
