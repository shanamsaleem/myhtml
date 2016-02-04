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
 
 Author: lex.borisov@gmail.com (Alexander Borisov)
*/

#include <stdio.h>
#include <stdlib.h>
#include <myhtml/api.h>

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
        fprintf(stderr, "Can't allocate mem for html file: %s\n", filename);
        exit(EXIT_FAILURE);
    }
    
    size_t nread = fread(html, 1, size, fh);
    if (nread != size) {
        fprintf(stderr, "could not read %ld bytes (%zu bytes done)\n", size, nread);
        exit(EXIT_FAILURE);
    }
    
    fclose(fh);
    
    if(size < 0)
        size = 0;
        
    struct res_html res = {html, (size_t)size};
    return res;
}

void print_node_attr(myhtml_tree_node_t *node)
{
    myhtml_tree_attr_t *attr = myhtml_node_attribute_first(node);
    
    while (attr) {
        const char *name = myhtml_attribute_name(attr, NULL);
        
        if(name) {
            printf(" %s", name);
            
            const char *value = myhtml_attribute_value(attr, NULL);
            
            if(value)
                printf("=\"%s\"", value);
        }
        
        attr = myhtml_attribute_next(attr);
    }
}

void print_tree(myhtml_tree_t* tree, myhtml_tree_node_t *node, size_t inc)
{
    while (node)
    {
        for(size_t i = 0; i < inc; i++)
            printf("\t");
        
        // print current element
        const char *tag_name = myhtml_tag_name_by_id(tree, myhtml_node_tag_id(node), NULL);
        
        if(tag_name)
            printf("<%s", tag_name);
        else
            // it can not be
            printf("<!something is wrong!");
        
        // print node attributes
        print_node_attr(node);
        
        if(myhtml_node_is_close_self(node))
            printf(" /");
        
        printf(">\n");
        
        // print childs
        print_tree(tree, myhtml_node_child(node), (inc + 1));
        node = myhtml_node_next(node);
    }
}

int main(int argc, const char * argv[])
{
    const char* path;
    
    if (argc == 2) {
        path = argv[1];
    }
    else {
        printf("Bad ARGV!\nUse: get_title_low_level <path_to_html_file>\n");
        exit(EXIT_FAILURE);
    }
    
    struct res_html res = load_html_file(path);
    
    // basic init
    myhtml_t* myhtml = myhtml_create();
    myhtml_init(myhtml, MyHTML_OPTIONS_DEFAULT, 1, 0);
    
    // init tree
    myhtml_tree_t* tree = myhtml_tree_create();
    myhtml_tree_init(tree, myhtml);
    
    // parse html
    myhtml_parse(tree, MyHTML_ENCODING_UTF_8, res.html, res.size);
    
    // print tree
    myhtml_tree_node_t *node = myhtml_tree_get_document(tree);
    print_tree(tree, myhtml_node_child(node), 0);
    
    // release resources
    myhtml_tree_destroy(tree);
    myhtml_destroy(myhtml);
    
    free(res.html);
    
    return 0;
}




