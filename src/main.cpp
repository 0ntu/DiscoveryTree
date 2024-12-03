#include <string>
#include <vector>

#include "fetcher.h"
#include "ui.h"

using namespace ftxui;
using BooksFetcher::Book;

int main() {
  std::vector<Book> books = BooksFetcher::fetch("books.csv");

  Ui& ui = Ui::getInstance(books);

  auto screen = ScreenInteractive::Fullscreen();
  screen.Loop(ui.render());
}

//Old UI for reference
  /* std::vector<std::string> tab_values{
      "Browse",
      "Suggested",
      "Settings",
      "Visualization",
  };
  int tab_selected = 0;
  auto tab_toggle = Toggle(&tab_values, &tab_selected);

  std::vector<std::string> tab_1_entries{
      "Forest",
      "Water",
      "I don't know",
  };
  int tab_1_selected = 0;

  std::vector<std::string> tab_2_entries{
      "Hello",
      "Hi",
      "Hay",
  };
  int tab_2_selected = 0;

  std::vector<std::string> tab_3_entries{
      "Table",
      "Nothing",
      "Is",
      "Empty",
  };
  int tab_3_selected = 0;
  auto tab_container = Container::Tab(
      {
          Radiobox(&tab_1_entries, &tab_1_selected),
          Radiobox(&tab_2_entries, &tab_2_selected),
          Radiobox(&tab_3_entries, &tab_3_selected),
      },
      &tab_selected);

  auto container = Container::Vertical({
      tab_toggle,
      tab_container,
  });

  auto text = ftxui::text("DiscoveryTree");

  auto renderer = Renderer(container, [&] {
    return vbox({
               hflow(text),
               separator(),
               tab_toggle->Render(),
               separator(),
               tab_container->Render(),
           }) |
           border;
  });

  auto screen = ScreenInteractive::Fullscreen();
  screen.Loop(renderer); */
