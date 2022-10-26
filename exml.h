// default material dark theme
// super default dark theme
// deep normal/dark

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
    XMLNode* node = (XMLNode*)malloc(sizeof(XMLNode));
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
    char* source = (char*)malloc(sizeof(char) * size + 1); // extra byte for null terminating character

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
            // reset the lexer
            lex[lexi] = '\0';

            // Inner text
            if (lexi > 0) {
                if (!curr_node) {
                    fprintf(stderr, "Inner text outside of document.\n");
                    return FALSE;
                }

                // set inner text of curr_node to current lex
                // reset lexi
                curr_node->inner_text = strdup(lex);
                lexi = 0;
            }

            // End of node
            if (source[i + 1] == '/') {
                i += 2;
                while (source[i] != '>')
                    lex[lexi++] = source[i++];
                lex[lexi] = '\0';

                if (!curr_node) {
                    fprintf(stderr, "Close tag presented without opening tag.\n");
                    return FALSE;
                }

                // if closing tag doesn't exist
                if (strcmp(curr_node->tag, lex) != 0) {
                    fprintf(stderr, "Mismatched tags [%s is NOT %s]", curr_node->tag, lex);
                    return FALSE;
                }

                curr_node = curr_node->parent;
                i++;
                continue;
            }

            // Set current node
            if (!curr_node)
                curr_node = doc->root;
            else
                curr_node = XMLNode_new(curr_node);

            // Get tag name
            i++; // this may cause us to skip first letter
            while (source[i] != '>')
                lex[lexi++] = source[i++];
            lex[lexi] = '\0';
            curr_node->tag = strdup(lex);

            // Reset lexical
            lexi = 0;
            i++;
            continue;
        }
        else {
            lex[lexi++] = source[i++];
        }
    }

    return TRUE;
}

void XMLDocument_free(XMLDocument* doc) {
    XMLNode_free(doc->root);
}



#endif // LITTLE_XML_H