#pragma once

#include "Bridges.h"
#include "DataSource.h"

using bridges::dataset::GutenbergBook;
using std::string;


class BridgesFetcher {
public:
  BridgesFetcher() {
    //bad practice...
    bridge = bridges::Bridges(1, "npadriga", "1519968279798");
    ds = bridges::DataSource(&bridge);
  }

  vector<GutenbergBook> fetch_gutenberg_books() {
    return ds.getGutenbergBookMetaData("", "");
  };

  string fetch_text(GutenbergBook book) {
    return ds.getGutenbergBookText(book.getId());
  };

private:
  bridges::Bridges bridge;
  bridges::DataSource ds;
};
