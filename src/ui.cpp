#include "ui.h"

using BooksFetcher::Book;
using std::string;
using std::vector;

void Ui::loop() { screen.Loop(render()); }

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

Ui &Ui::getInstance(const vector<Book> &books) {
  static Ui instance(books);
  return instance;
}

Ui::Ui(const vector<Book> &books)
    : books(books), screen(ScreenInteractive::Fullscreen()) {
  buildTabline();
  buildBrowseMenu();
  buildSuggestedMenu();
  buildBookmarksMenu();
  buildSettingsMenu();
  buildAboutMenu();
}

Component Ui::render() {
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
  return Renderer(container, [&] { return buildInterface(); });
}

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

Element Ui::renderPrimary() {
  if (tab_index == 0) {
    return browse_split->Render() | border;
  } else if (tab_index == 1) {
    return suggested_split->Render() | border;
  } else if (tab_index == 2) {
    return bookmarks_split->Render() | border;
  } else if (tab_index == 3) {
    return vbox(paragraph("Data structure to use for Suggested") | center,
                settings_ds_menu->Render() | center) |
           borderStyled(ROUNDED) | center;
  } else if (tab_index == 4) {
    return aboutElement;
  } else {
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
        return vbox(text("Book List") | bold | center, separator(),
                    title_container->Render() | vscroll_indicator | frame);
      }),
      Renderer([&] {
        return vbox(text("Details") | bold | center, separatorEmpty(),
                    renderBookDetails(books[browse_selected]));
      }),
      &split_size_browse);
}

void Ui::buildSuggestedMenu() {
  suggested_buttons = Container::Vertical({
    // Button("Bookmark", [&] {}, ButtonOption::Animated(Color::Blue)),
    // Button("Don't Bookmark", [&] {}, ButtonOption::Animated(Color::GrayDark)),
    Button("Bookmark", [&] {}, ButtonOption::Simple()),
    Button("Don't Bookmark", [&] {}, ButtonOption::Simple()),
  });

  split_size_suggested = 180;
  book_display =
      vbox(filler(), text(books[10].title) | center, separatorLight(),
           hbox(text("By: "), text(books[10].authors)) | center,
           separatorEmpty(),
           hbox(text(std::to_string(books[10].rating)), text("/5 rating")) |
               center,
           hbox(text("Published: "), text(books[10].publisher)) | center,
           hbox(text("ISBN: "), text(books[10].isbn) | underlined) | center) |
      center | borderStyled(DASHED) | size(HEIGHT, GREATER_THAN, 40);

  suggested_split = ResizableSplitLeft(
      Renderer([&] {
        return vbox(book_display | center | flex,
                    suggested_buttons->Render() | center | flex);
      }),
      Renderer([&] { return vbox(); }), &split_size_suggested);
}

void Ui::buildBookmarksMenu() {

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

void Ui::buildSettingsMenu() {
  settings_data_structures = {"Heap Tree", "B+ Tree"};
  settings_selected = 0;
  settings_ds_menu = Radiobox({
      .entries = &settings_data_structures,
      .selected = &settings_selected,
  });
}

void Ui::buildAboutMenu() {
  aboutElement = vbox({
    text("Made with ♥ by:") | bold | center,
    text("Dia Fallon (B+ Tree Implementation)") | center,
    text("Cainan Medeiros (Heap Tree Implementation)") | center,
    text("Nathan Padriga (User Interface)") | center,
    separatorEmpty(),
    text("Data Structures and Algorithms (COP3530) @ UF") | dim | center,
    text("Fall 2024 w/ Prof. Amanpreet Kapoor") | dim | center,
    separatorEmpty(),
    text("Project 3 - Team 166") | bold | blink | center,
  }) | center | borderHeavy;
}
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
