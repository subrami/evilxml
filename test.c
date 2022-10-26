#include "exml.h"

int main()
{
  XMLDocument doc;
  if (XMLDocument_load(&doc, "test.xml")) {
    printf("[tag]: %s [inner text]: %s\n", doc.root->tag, doc.root->inner_text);
    XMLDocument_free(&doc);

  }

  return 0;
}