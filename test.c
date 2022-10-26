#include "exml.h"

int main()
{
  XMLDocument doc;
  if (XMLDocument_load(&doc, "test.xml")) {
    printf("%s\n", doc.root->tag);
    XMLDocument_free(&doc);

  }
  
  return 0;
}