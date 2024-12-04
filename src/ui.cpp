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
  // ---- Build Tabline ----
  tab_options = {
      "Browse",
      "Suggested",
      "Settings",
      "About",
  };
  exit_button = Button(
      "Exit", [&] { screen.Exit(); }, ButtonOption::Animated(Color::Default));
  tab_selection =
      Menu(&tab_options, &tab_index, MenuOption::HorizontalAnimated());
  // --------------------------

  // ---- Build Browse Menu ---
  selected = 0;
  title_container = Container::Vertical({}, &selected);
  split_size_browse = 180;
  for (int i = 0; i < books.size(); ++i) {
    auto book = books[i];
    // for (Book book : books) {
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
                    renderBookDetails(books[selected]));
      }),
      &split_size_browse);
  // --------------------------

  // --- Build Suggested Menu ---
  split_size_suggested = 180;
  book_display =
      vbox(filler(), text(books[10].title) | center, separatorLight(),
           hbox(text("By: "), text(books[10].authors)) | center,
           separatorEmpty(),
           hbox(text(std::to_string(books[10].rating)), text("/5 rating")) |
               center,
           hbox(text("Published: "), text(books[10].publisher)) | center,
           hbox(text("ISBN: "), text(books[10].isbn) | underlined) | center,
           separatorEmpty(), separatorEmpty(), separatorEmpty()) |
      center | borderStyled(DASHED);
  suggested_split = ResizableSplitLeft(
      Renderer([&] { return vbox(book_display | center | flex); }),
      Renderer([&] { return vbox(); }), &split_size_suggested);
  // ----------------------------

  // --- Build Settings Menu ---
  settings_data_structures = {"Heap Tree", "B+ Tree"};
  settings_selected = 0;
  settings_ds_menu = Radiobox({
      .entries = &settings_data_structures,
      .selected = &settings_selected,
  });
  // ---------------------------

  // --- Build About Page ---

  // ------------------------
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

Component Ui::render() {
  auto container = Container::Vertical({
      Maybe(settings_ds_menu, [&] { return tab_index == 2; }),
      Container::Horizontal(
          {Maybe(suggested_split, [&] { return tab_index == 1; })}),
      Container::Horizontal(
          {Maybe(title_container, [&] { return tab_index == 0; }),
           Maybe(browse_split, [&] { return tab_index == 0; })}),
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
    return vbox(paragraph("Data structure to use for Suggested") | center,
                settings_ds_menu->Render() | center) |
           borderStyled(ROUNDED) | center ;
  } else {
    return emptyElement();
  }
}
