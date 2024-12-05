#include "ui.h"

using BooksFetcher::Book;
using std::string;
using std::vector;

void Ui::loop() { screen.Loop(render()); }

// FTXUI does not have a scrollable table
// Create Table with Vertical Container of Book Components
// https://github.com/ArthurSonzogni/FTXUI/discussions/757
//
// BookLine is an entry in the scrollable table
Component BookLine(Book book) {
  class Impl : public ComponentBase {
    Element Render() final {
      auto element = hbox(text(book.title));

      if (Focused()) {
        element = focus(element);
        element |= inverted;
      } else if (Active()) {
        element = select(element);
        element |= inverted;
      }

      return element;
    }

    bool Focusable() const final { return true; }

  public:
    Book book;
  };
  auto instance = Make<Impl>();
  instance->book = book;
  return instance;
}

// Create and get UI singleton
Ui &Ui::getInstance(const vector<Book> &books, max_heap &heap, b_plus &bptree) {
  static Ui instance(books, heap, bptree);
  return instance;
}

//Initialize member functions
//Generate menu layout but dont display anything
Ui::Ui(const vector<Book> &books, max_heap &heap, b_plus &bptree)
    : books(books), screen(ScreenInteractive::Fullscreen()), heap(heap),
      bptree(bptree) {

  //Get sorted nodes from B+ tree
  bp_sorted = bptree.collect();
  buildTabline();
  buildBrowseMenu();
  buildSuggestedMenu();
  buildBookmarksMenu();
  buildSettingsMenu();
  buildAboutMenu();
}

//Renders the entire interface to a component, to be displayed by the main screen
Component Ui::render() {
  //handles input functionality
  auto container = Container::Vertical({
      Container::Horizontal(
          {Maybe(title_container, [&] { return tab_index == 0; }),
           Maybe(browse_split, [&] { return tab_index == 0; })}),
      Container::Horizontal(
          {Maybe(suggested_buttons, [&] { return tab_index == 1; }),
           Maybe(suggested_split, [&] { return tab_index == 1; })}),
      Container::Horizontal(
          {Maybe(saved_container, [&] { return tab_index == 2; }),
           Maybe(bookmarks_split, [&] { return tab_index == 2; })}),
      Maybe(settings_ds_menu, [&] { return tab_index == 3; }),
      Container::Horizontal({tab_selection, exit_button}),
  });
  
  //render to component
  return Renderer(container, [&] { return buildInterface(); });
}


//Interface is made of a primary screen and a tabline
//Render both and put them in a vertical box
Element Ui::buildInterface() {
  return vbox({
      text("󱁉  DiscoveryTree 󱁉") | bold | hcenter,
      renderPrimary() | flex,
      renderTabline(),
  });
}

Element Ui::renderTabline() {
  return hbox({
      tab_selection->Render() | flex,
      exit_button->Render(),
  });
}

//primary display, render to element
Element Ui::renderPrimary() {
  if (tab_index == 0) { // browse
    return browse_split->Render() | border;
  } else if (tab_index == 1) { // suggested
    return suggested_split->Render() | border;
  } else if (tab_index == 2) { // bookmarks
    return bookmarks_split->Render() | border;
  } else if (tab_index == 3) {
    //update suggested menu on match w/ settings
    buildCoreSuggestedMenu();
    return vbox(paragraph("Data structure to use for Suggested") | center,
                settings_ds_menu->Render() | center) |
           borderStyled(ROUNDED) | center;
  } else if (tab_index == 4) { // about
    return aboutElement;
  } else { //not possible but just in case,
    return emptyElement();
  }
}

void Ui::buildTabline() {
  tab_options = {
      "Browse", "Suggested", "Bookmarks", "Settings", "About",
  };
  exit_button = Button(
      "Exit", [&] { screen.Exit(); }, ButtonOption::Animated(Color::Default));
  tab_selection =
      Menu(&tab_options, &tab_index, MenuOption::HorizontalAnimated());
}

void Ui::buildBrowseMenu() {
  browse_selected = 0;
  title_container = Container::Vertical({}, &browse_selected);
  split_size_browse = 180;
  for (Book book : books) {
    Component title = BookLine(book);
    title_container->Add(title);
  }
  browse_split = ResizableSplitLeft(
      Renderer([&] {
        return vbox(
            hbox(text(std::to_string(browse_selected)) | dim, text("/") | dim,
                 text(std::to_string(books.size())) | dim,
                 text("Book List") | bold | center | flex),
            separator(), title_container->Render() | vscroll_indicator | frame);
      }),
      Renderer([&] {
        return vbox(text("Details") | bold | center, separatorEmpty(),
                    renderBookDetails(books[browse_selected]));
      }),
      &split_size_browse);
}

//build buttons & menu
void Ui::buildSuggestedMenu() {
  bp_index = 30;
  suggested_buttons = Container::Vertical({
      Button(
          "Bookmark",
          [&] {
            Book popped;
            if (settings_selected == 0) {
              popped = heap.pop();
            } else {
              popped = bp_sorted[bp_index];
              bp_index += 1;
            }
            saved_books.push_back(popped);
            buildCoreSuggestedMenu();
            saved_container->Add(BookLine(popped));
          },
          ButtonOption::Simple()),
      Button(
          "Don't Bookmark",
          [&] {
            if (settings_selected == 0) {
              heap.pop();
            } else {
              bp_index += 1;
            }
            buildCoreSuggestedMenu();
          },
          ButtonOption::Simple()),
  });

  split_size_suggested = 180;

  buildCoreSuggestedMenu();
}

//reason this is separated is because this has to be updated w/o changing the split_size/bp_index
//whenever the next book is suggested
void Ui::buildCoreSuggestedMenu() {
  // use heap to suggest
  if (settings_selected == 0) {
    current_suggested_book = heap.top();
  } else {
    // use bptree to suggest
    current_suggested_book = bp_sorted[bp_index];
  }
  //suggested menu layout
  book_display =
      vbox(
          filler(), text(current_suggested_book.title) | center,
          separatorLight(),
          hbox(text("By: "), text(current_suggested_book.authors)) | center,
          separatorEmpty(),
          hbox(text(std::to_string(current_suggested_book.rating)),
               text("/5 rating")) |
              center,
          hbox(text("Published: "), text(current_suggested_book.publisher)) |
              center,
          hbox(text("ISBN: "), text(current_suggested_book.isbn) | underlined) |
              center,
          hbox(text("Language: "), text(current_suggested_book.language)) |
              center) |
      center | borderStyled(DASHED) | size(HEIGHT, GREATER_THAN, 40) |
      size(WIDTH, GREATER_THAN, 60);

  //render menu & buttons together
  suggested_split = Renderer([&] {
    return vbox(book_display | center | flex,
                suggested_buttons->Render() | center | flex);
  });
}

void Ui::buildBookmarksMenu() {
  //bookmarks is essnetially the same as browse menu
  bookmarks_selected = 0;
  saved_container = Container::Vertical({}, &bookmarks_selected);
  split_size_bookmarks = 180;
  bookmarks_split = ResizableSplitLeft(
      Renderer([&] {
        return vbox(text("Bookmarks") | bold | center, separator(),
                    saved_container->Render() | vscroll_indicator | frame);
      }),
      Renderer([&] {
        if (saved_books.empty())
          return vbox(text("Details") | bold | center, separatorEmpty(),
                      emptyElement());
        return vbox(text("Details") | bold | center, separatorEmpty(),
                    renderBookDetails(saved_books[bookmarks_selected]));
      }),
      &split_size_browse);
}

//Select between heap tree & b+ tree w/ radiobox selector
void Ui::buildSettingsMenu() {
  settings_data_structures = {"Heap Tree (Rating)", "B+ Tree (ISBN)"};
  settings_selected = 0;
  settings_ds_menu = Radiobox({
      .entries = &settings_data_structures,
      .selected = &settings_selected,
  });
}

//wall of text essentially
//info about the project
void Ui::buildAboutMenu() {
  aboutElement =
      vbox({
          text("|| Discovery Tree ||") | bold | center,
          text("A fast and free book finder for the 90s!") | center,
          text("Search through a catalog of over 10,000 books") | center,
          separatorEmpty(),
          separatorEmpty(),
          separatorEmpty(),
          text("Made with ♥ by:") | bold | center,
          text("Dia Fallon (B+ Tree Implementation)") | center,
          text("Cainan Medeiros (Heap Tree Implementation)") | center,
          text("Nathan Padriga (User Interface)") | center,
          separatorEmpty(),
          text("Data Structures and Algorithms (COP3530) @ UF") | dim | center,
          text("Fall 2024 w/ Prof. Amanpreet Kapoor") | dim | center,
          separatorEmpty(),
          text("Project 3 - Team 166") | bold | blink | center,
      }) |
      center | borderHeavy;
}

//Layout for bookdetails to be displayed
const Element Ui::renderBookDetails(const Book book) {
  return vbox({
      text("Title") | bold,
      text(book.title),
      separatorEmpty(),
      text("Authors") | bold,
      text(book.authors),
      separatorEmpty(),
      text("Publication Date") | bold,
      text(book.publication_date),
      separatorEmpty(),
      text("Rating") | bold,
      text(std::to_string(book.rating)),
      separatorEmpty(),
      text("ISBN | ISBN13") | bold,
      hbox(text(book.isbn), text(" | "), text(book.isbn13)),
      separatorEmpty(),
      text("Language") | bold,
      text(book.language),
      separatorEmpty(),
  });
}
