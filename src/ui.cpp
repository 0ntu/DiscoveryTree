#include "ui.h"

using BooksFetcher::Book;
using std::string;
using std::vector;

Ui &Ui::getInstance(const vector<Book> &books) {
  static Ui instance(books);
  return instance;
}

Ui::Ui(const vector<Book> &books) : books(books) {
  // set component
  tab_options = {
      "Browse",
      "Suggested",
      "Settings",
  };
  exit_button = Button("Exit", [&] { exit(0); }, ButtonOption::Animated(Color::Default));
  tab_selection =
      Menu(&tab_options, &tab_index, MenuOption::HorizontalAnimated());
}

Component Ui::render() {
  auto container = Container::Vertical({
      tab_selection,
      exit_button,
  });
  return Renderer(container, [&] { return buildInterface(); });
}

Element Ui::buildInterface() {
  return vbox({
      text("󱁉  DiscoveryTree 󱁉") | bold | hcenter,
      text("Lorem Ipsum") | flex,
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

}
