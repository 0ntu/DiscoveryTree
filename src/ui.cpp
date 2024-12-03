#include "ui.h"

using BooksFetcher::Book;
using std::string;
using std::vector;

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

Ui::Ui(const vector<Book> &books) : books(books) {
  // ---- Build Tabline ----
  tab_options = {
      "Browse",
      "Suggested",
      "Settings",
  };
  exit_button =
      Button("Exit", [&] { exit(0); }, ButtonOption::Animated(Color::Default));
  tab_selection =
      Menu(&tab_options, &tab_index, MenuOption::HorizontalAnimated());
  // --------------------------

  // ---- Build Browse Menu ---
  selected = 0;
  title_container = Container::Vertical({}, &selected);
  split_size = 180;
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
      &split_size);
  // --------------------------
  
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
      Container::Horizontal({title_container, browse_split}),
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
  } else {
    // return text(std::to_string(tab_index));
    return emptyElement();
  }
}
