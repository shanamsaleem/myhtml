/*
 Copyright 2016 Alexander Borisov
 
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 
 http://www.apache.org/licenses/LICENSE-2.0
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 
 Authors: insoreiges@gmail.com (Evgeny Yakovlev)
 
 html2sexpr
 Convert html tag tree into s-expression string in stdout
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#include <myhtml/api.h>

#define DIE(msg, ...) do { fprintf(stderr, msg, ##__VA_ARGS__); exit(EXIT_FAILURE); } while(0);

static const char* tag_str(myhtml_tag_id_t id)
{
    switch (id)
    {
    case MyHTML_TAG__UNDEF:        return "_UNDEF";
    case MyHTML_TAG__TEXT:         return "_TEXT";
    case MyHTML_TAG__COMMENT:      return "_COMMENT";   
    case MyHTML_TAG__DOCTYPE:      return "_DOCTYPE";
    case MyHTML_TAG_A:             return "A";
    case MyHTML_TAG_ABBR:          return "ABBR";
    case MyHTML_TAG_ACRONYM:       return "ACRONYM";
    case MyHTML_TAG_ADDRESS:       return "ADDRESS";
    case MyHTML_TAG_ANNOTATION_XML:return "ANNOTATION_XML";
    case MyHTML_TAG_APPLET:        return "APPLET";
    case MyHTML_TAG_AREA:          return "AREA";
    case MyHTML_TAG_ARTICLE:       return "ARTICLE";
    case MyHTML_TAG_ASIDE:         return "ASIDE";
    case MyHTML_TAG_AUDIO:         return "AUDIO";
    case MyHTML_TAG_B:             return "B";
    case MyHTML_TAG_BASE:          return "BASE";
    case MyHTML_TAG_BASEFONT:      return "BASEFONT";
    case MyHTML_TAG_BDI:           return "BDI";
    case MyHTML_TAG_BDO:           return "BDO";
    case MyHTML_TAG_BGSOUND:       return "BGSOUND";
    case MyHTML_TAG_BIG:           return "BIG";
    case MyHTML_TAG_BLINK:         return "BLINK";
    case MyHTML_TAG_BLOCKQUOTE:    return "BLOCKQUOTE";
    case MyHTML_TAG_BODY:          return "BODY";
    case MyHTML_TAG_BR:            return "BR";
    case MyHTML_TAG_BUTTON:        return "BUTTON";
    case MyHTML_TAG_CANVAS:        return "CANVAS";
    case MyHTML_TAG_CAPTION:       return "CAPTION";
    case MyHTML_TAG_CENTER:        return "CENTER";
    case MyHTML_TAG_CITE:          return "CITE";
    case MyHTML_TAG_CODE:          return "CODE";
    case MyHTML_TAG_COL:           return "COL";
    case MyHTML_TAG_COLGROUP:      return "COLGROUP";
    case MyHTML_TAG_COMMAND:       return "COMMAND";
    case MyHTML_TAG_COMMENT:       return "COMMENT";
    case MyHTML_TAG_DATALIST:      return "DATALIST";
    case MyHTML_TAG_DD:            return "DD";
    case MyHTML_TAG_DEL:           return "DEL";
    case MyHTML_TAG_DESC:          return "DESC";
    case MyHTML_TAG_DETAILS:       return "DETAILS";
    case MyHTML_TAG_DFN:           return "DFN";
    case MyHTML_TAG_DIALOG:        return "DIALOG";
    case MyHTML_TAG_DIR:           return "DIR";
    case MyHTML_TAG_DIV:           return "DIV";
    case MyHTML_TAG_DL:            return "DL";
    case MyHTML_TAG_DT:            return "DT";
    case MyHTML_TAG_EM:            return "EM";
    case MyHTML_TAG_EMBED:         return "EMBED";
    case MyHTML_TAG_FIELDSET:      return "FIELDSET";
    case MyHTML_TAG_FIGCAPTION:    return "FIGCAPTION";
    case MyHTML_TAG_FIGURE:        return "FIGURE";
    case MyHTML_TAG_FONT:          return "FONT";
    case MyHTML_TAG_FOOTER:        return "FOOTER";
    case MyHTML_TAG_FOREIGNOBJECT: return "FOREIGNOBJECT";
    case MyHTML_TAG_FORM:          return "FORM";
    case MyHTML_TAG_FRAME:         return "FRAME";
    case MyHTML_TAG_FRAMESET:      return "FRAMESET";
    case MyHTML_TAG_H1:            return "H1";
    case MyHTML_TAG_H2:            return "H2";
    case MyHTML_TAG_H3:            return "H3";
    case MyHTML_TAG_H4:            return "H4";
    case MyHTML_TAG_H5:            return "H5";
    case MyHTML_TAG_H6:            return "H6";
    case MyHTML_TAG_HEAD:          return "HEAD";
    case MyHTML_TAG_HEADER:        return "HEADER";
    case MyHTML_TAG_HGROUP:        return "HGROUP";
    case MyHTML_TAG_HR:            return "HR";
    case MyHTML_TAG_HTML:          return "HTML";
    case MyHTML_TAG_I:             return "I";
    case MyHTML_TAG_IFRAME:        return "IFRAME";
    case MyHTML_TAG_IMAGE:         return "IMAGE";
    case MyHTML_TAG_IMG:           return "IMG";
    case MyHTML_TAG_INPUT:         return "INPUT";
    case MyHTML_TAG_INS:           return "INS";
    case MyHTML_TAG_ISINDEX:       return "ISINDEX";
    case MyHTML_TAG_KBD:           return "KBD";
    case MyHTML_TAG_KEYGEN:        return "KEYGEN";
    case MyHTML_TAG_LABEL:         return "LABEL";
    case MyHTML_TAG_LEGEND:        return "LEGEND";
    case MyHTML_TAG_LI:            return "LI";
    case MyHTML_TAG_LINK:          return "LINK";
    case MyHTML_TAG_LISTING:       return "LISTING";
    case MyHTML_TAG_MAIN:          return "MAIN";
    case MyHTML_TAG_MALIGNMARK:    return "MALIGNMARK";
    case MyHTML_TAG_MAP:           return "MAP";
    case MyHTML_TAG_MARK:          return "MARK";
    case MyHTML_TAG_MARQUEE:       return "MARQUEE";
    case MyHTML_TAG_MATH:          return "MATH";
    case MyHTML_TAG_MENU:          return "MENU";
    case MyHTML_TAG_MENUITEM:      return "MENUITEM";
    case MyHTML_TAG_MERROR:        return "MERROR";
    case MyHTML_TAG_META:          return "META";
    case MyHTML_TAG_METER:         return "METER";
    case MyHTML_TAG_MGLYPH:        return "MGLYPH";
    case MyHTML_TAG_MI:            return "MI";
    case MyHTML_TAG_MO:            return "MO";
    case MyHTML_TAG_MN:            return "MN";
    case MyHTML_TAG_MS:            return "MS";
    case MyHTML_TAG_MTEXT:         return "MTEXT";
    case MyHTML_TAG_NAV:           return "NAV";
    case MyHTML_TAG_NOBR:          return "NOBR";
    case MyHTML_TAG_NOEMBED:       return "NOEMBED";
    case MyHTML_TAG_NOFRAMES:      return "NOFRAMES";
    case MyHTML_TAG_NOSCRIPT:      return "NOSCRIPT";
    case MyHTML_TAG_OBJECT:        return "OBJECT";
    case MyHTML_TAG_OL:            return "OL";
    case MyHTML_TAG_OPTGROUP:      return "OPTGROUP";
    case MyHTML_TAG_OPTION:        return "OPTION";
    case MyHTML_TAG_OUTPUT:        return "OUTPUT";
    case MyHTML_TAG_P:             return "P";
    case MyHTML_TAG_PARAM:         return "PARAM";
    case MyHTML_TAG_PLAINTEXT:     return "PLAINTEXT";
    case MyHTML_TAG_PRE:           return "PRE";
    case MyHTML_TAG_PROGRESS:      return "PROGRESS";
    case MyHTML_TAG_Q:             return "Q";
    case MyHTML_TAG_RB:            return "RB";
    case MyHTML_TAG_RP:            return "RP";
    case MyHTML_TAG_RT:            return "RT";
    case MyHTML_TAG_RTC:           return "RTC";
    case MyHTML_TAG_RUBY:          return "RUBY";
    case MyHTML_TAG_S:             return "S";
    case MyHTML_TAG_SAMP:          return "SAMP";
    case MyHTML_TAG_SCRIPT:        return "SCRIPT";
    case MyHTML_TAG_SECTION:       return "SECTION";
    case MyHTML_TAG_SELECT:        return "SELECT";
    case MyHTML_TAG_SMALL:         return "SMALL";
    case MyHTML_TAG_SOURCE:        return "SOURCE";
    case MyHTML_TAG_SPAN:          return "SPAN";
    case MyHTML_TAG_STRIKE:        return "STRIKE";
    case MyHTML_TAG_STRONG:        return "STRONG";
    case MyHTML_TAG_STYLE:         return "STYLE";
    case MyHTML_TAG_SUB:           return "SUB";
    case MyHTML_TAG_SUMMARY:       return "SUMMARY";
    case MyHTML_TAG_SUP:           return "SUP";
    case MyHTML_TAG_SVG:           return "SVG";
    case MyHTML_TAG_TABLE:         return "TABLE";
    case MyHTML_TAG_TBODY:         return "TBODY";
    case MyHTML_TAG_TD:            return "TD";
    case MyHTML_TAG_TEMPLATE:      return "TEMPLATE";
    case MyHTML_TAG_TEXTAREA:      return "TEXTAREA";
    case MyHTML_TAG_TFOOT:         return "TFOOT";
    case MyHTML_TAG_TH:            return "TH";
    case MyHTML_TAG_THEAD:         return "THEAD";
    case MyHTML_TAG_TIME:          return "TIME";
    case MyHTML_TAG_TITLE:         return "TITLE";
    case MyHTML_TAG_TR:            return "TR";
    case MyHTML_TAG_TRACK:         return "TRACK";
    case MyHTML_TAG_TT:            return "TT";
    case MyHTML_TAG_U:             return "U";
    case MyHTML_TAG_UL:            return "UL";
    case MyHTML_TAG_VAR:           return "VAR";
    case MyHTML_TAG_VIDEO:         return "VIDEO";
    case MyHTML_TAG_WBR:           return "WBR";
    case MyHTML_TAG_XMP:           return "XMP";
    case MyHTML_TAG__END_OF_FILE:  return "_END_OF_FILE";
    default:                       assert(0); return "(unknown)";
    };
}

static bool filter_node(myhtml_tree_node_t* node) 
{
    assert(node);
    myhtml_tag_id_t tag = myhtml_node_tag_id(node);
    return (tag != MyHTML_TAG__TEXT) && (tag != MyHTML_TAG__END_OF_FILE) && (tag != MyHTML_TAG__COMMENT) && (tag != MyHTML_TAG__UNDEF);
}

/* depth-first lefthand tree walk */
static void walk_subtree(myhtml_tree_node_t* root, int level)
{
    if (!root) {
        return;
    }

    /* Check if we handle this node type */
    if (!filter_node(root)) {
        return;
    }

    /* start sexpr */
    putchar('(');

    // XXX: why not myhtml_tag_name_by_id in api?
    /* print this node */
    printf("%s", tag_str(myhtml_node_tag_id(root)));
    myhtml_tree_attr_t* attr = myhtml_node_attribute_first(root);
    while (attr != NULL) {
        /* attribute sexpr (name value)*/
        printf("(%s \'%s\')", myhtml_attribute_name(attr, NULL), myhtml_attribute_value(attr, NULL));
        attr = myhtml_attribute_next(attr);
    }

    /* left hand depth-first recoursion */
    myhtml_tree_node_t* child = myhtml_node_child(root);
    while (child != NULL) {
        walk_subtree(child, level + 1);
        child = myhtml_node_next(child);
    }

    /* close sexpr */
    putchar(')');
}

struct res_html {
    char  *html;
    size_t size;
};

struct res_html load_html_file(const char* filename)
{
    FILE *fh = fopen(filename, "rb");
    if(fh == NULL) {
        fprintf(stderr, "Can't open html file: %s\n", filename);
        exit(EXIT_FAILURE);
    }
    
    fseek(fh, 0L, SEEK_END);
    long size = ftell(fh);
    fseek(fh, 0L, SEEK_SET);
    
    char *html = (char*)malloc(size + 1);
    if(html == NULL) {
        DIE("Can't allocate mem for html file: %s\n", filename);
    }
    
    size_t nread = fread(html, 1, size, fh);
    if (nread != size) {
        DIE("could not read %ld bytes (%zu bytes done)\n", size, nread);
    }

    fclose(fh);
    
    if(size < 0) {
        size = 0;
    }
    
    struct res_html res = {html, (size_t)size};
    return res;
}

static void usage(void)
{
    fprintf(stderr, "html2sexpr <file>\n");
}

int main(int argc, char** argv)
{
    if (argc != 2) {
        usage();
        DIE("Invalid number of arguments\n");   
    }

    struct res_html data = load_html_file(argv[1]);
    myhtml_status_t res;

	// basic init
    myhtml_t* myhtml = myhtml_create();
    res = myhtml_init(myhtml, MyHTML_OPTIONS_DEFAULT, 1, 0);
    if (MYHTML_FAILED(res)) {
    	DIE("myhtml_init failed with %d\n", res);
    }
    
    // init tree
    // XXX: Why do we need to explicitly create a tree before calling parse_html? Can't parse_html just create and return a new tree for us?
    // YYY: There are many resources initialized. It can be used many times for acceleration.
    //      There 0.0006s :). Though, who needs to speed uses the low-level api. We will see...
    myhtml_tree_t* tree = myhtml_tree_create();
    res = myhtml_tree_init(tree, myhtml);
    if (MYHTML_FAILED(res)) {
    	DIE("myhtml_tree_init failed with %d\n", res);
    }
    
    // parse html
    myhtml_parse(tree, MyHTML_ENCODING_UTF_8, data.html, data.size);
    
    // XXX: see myhtml_tree_get_document in myhtml/api.h -- return root of tree,
    //      continue to use myhtml_node_child(root_node)
    
    // Find <html>, we'll dump from there to make things easier
    myhtml_collection_t* collection = myhtml_get_nodes_by_tag_id(tree, NULL, MyHTML_TAG_HTML, &res);
    if (!collection) {
        DIE("Can't find root node: %d\n", res);
    }

    walk_subtree(collection->list[0], 0);
    printf("\n");

    // release resources
    myhtml_collection_destroy(collection);
    myhtml_tree_destroy(tree);
    myhtml_destroy(myhtml);
    free(data.html);

    return EXIT_SUCCESS;
}