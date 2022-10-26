#ifndef LITTLE_XML_H
#define LITTLE_XML_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef TRUE
  const int TRUE = 1;
#endif
#ifndef FALSE
  const int FALSE = 0;
#endif

//
// Definitions
//

struct _XMLNode
{
  char* tag;
  char* inner_text;
  struct _XMLNode* parent;
};
typedef struct _XMLNode XMLNode;


struct _XMLDocument
{
  XMLNode* root;
};
typedef struct _XMLDocument XMLDocument;

int XMLDocument_load(XMLDocument* doc, const char* path);
void XMLDocument_free(XMLDocument* doc);

XMLNode* XMLNode_new(XMLNode* parent);
void XMLNode_free(XMLNode* node);

//
// Implementation
//

XMLNode* XMLNode_new(XMLNode* parent) {
  XMLNode* node = (XMLNode*) malloc (sizeof(XMLNode));
  node->parent = parent;
  node->tag = NULL;
  node->inner_text = NULL;
  return node;
}

void XMLNode_free(XMLNode* node) {
  if (node->tag) {
    free(node->tag);
  }
  if (node->inner_text) {
    free(node->inner_text);
  }
}


/**
 * Loading XML Document with pointer to object and pointer to path
*/
int XMLDocument_load(XMLDocument* doc, const char* path)
{
  FILE* xmlFile = fopen(path, "r");

  if (!xmlFile) {
    fprintf(stderr, "Failed to open file from '%s'\n", path);
    return FALSE;
  }

  // sets file position of stream
  // xmlfile, 0 offset, end of the file
  fseek(xmlFile, 0, SEEK_END);
  int size = ftell(xmlFile);
  fseek(xmlFile, 0, SEEK_SET);

  // will return the pointer to the start of xmlFile (source)
  char* source = (char*) malloc(sizeof(char) * size + 1); // extra byte for null terminating character
  
  // source = pointer to block of  memory, 1 = size in bytes of each element
  // size = number of elements, each with a size of 1 byte
  // xmlFile = stream or pointer to file
  fread(source, 1, size, xmlFile);
  fclose(xmlFile);
  source[size] = '\0';

  doc->root = XMLNode_new(NULL);

  char lex[256];
  int lexi = 0;
  int i = 0;


  // curr_node initialized to be the node to be parsed below
  XMLNode* curr_node = NULL;

  while (source[i] != '\0') {
    if (source[i] == '<') {
      if (!curr_node)
        curr_node = doc->root;
      else
        curr_node = XMLNode_new(curr_node);
      i++; // this may cause us to skip first letter
      while (source[i] != '>')
        lex[lexi++] = source[i++];
      lex[lexi] = '\0';
      curr_node->tag = strdup(lex);

      lexi = 0;
      i++;
      continue;
    }
  }
  
  return TRUE;
}

void XMLDocument_free(XMLDocument* doc) {
  XMLNode_free(doc->root);
}



#endif // LITTLE_XML_H