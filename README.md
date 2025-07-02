# PP11

## Goal

In this exercise you will:

* Design and implement a **doubly linked list** as a reusable library.
* Create and interact with a **singly linked list** via an interactive menu, using dynamic memory (`malloc`/`free`).
* Parse a JSON array from a file using **jansson**, store objects in a linked list, and manipulate them in C.

**Important:** Start a stopwatch when you begin and work uninterruptedly for **90 minutes**. When time is up, stop immediately and record where you paused.

---

## Workflow

1. **Fork** this repository on GitHub.
2. **Clone** your fork locally.
3. Create a `solutions/` directory at the project root:

   ```bash
   mkdir solutions
   ```
4. For each task, add the specified source files under `solutions/`.
5. **Commit** and **push** your changes to GitHub.
6. **Submit** your GitHub repo link for review.

---

## Prerequisites

* GNU C compiler (`gcc`).
* Understanding of pointers and dynamic memory (`malloc`, `free`).
* Jansson library installed (`libjansson-dev`).
* Linker flags (`-ljansson`).

---

## Tasks

### Task 1: Compile-Time Doubly Linked List

**Objective:** Implement and traverse a doubly linked list entirely with compile-time allocation.

1. Create `solutions/dlist_static.c` and include `dlist.h` (which defines `DNode` and `DList`).
2. Define a static array of five `DNode` elements:

   ```c
   #include <stdio.h>
   #include "dlist.h"  // defines DNode and DList

   // Static nodes for values 1–5
   DNode nodes[5];
   DList list;

   int main(void) {
       // Initialize static nodes
       for (int i = 0; i < 5; i++) {
           // Example: store node index as data
           nodes[i].data = (void*)(long)(i + 1);
           nodes[i].prev = (i > 0) ? &nodes[i - 1] : NULL;
           nodes[i].next = (i < 4) ? &nodes[i + 1] : NULL;
       }
       list.head = &nodes[0];
       list.tail = &nodes[4];

       // Traverse forward
       printf("Forward: ");
       for (DNode *p = list.head; p != NULL; p = p->next) {
           printf("%ld ", (long)p->data);
       }
       printf("\n");

       // Traverse backward
       printf("Backward: ");
       for (DNode *p = list.tail; p != NULL; p = p->prev) {
           printf("%ld ", (long)p->data);
       }
       printf("\n");

       return 0;
   }
   ```
3. Compile and run:

   ```bash
   gcc -o solutions/dlist_static solutions/dlist_static.c
   ./solutions/dlist_static
   ```

#### Reflection Questions

1. **How do you link `prev` and `next` pointers correctly using a static array?**
<summary>Each element in the static array nodes [i] is linked to its neighbors using:</summary>

```bash
 nodes[i].prev = (i > 0) ? &nodes[i - 1] : NULL;
nodes[i].next = (i < 4) ? &nodes[i + 1] : NULL;
```
2. **What are advantages and limitations of compile-time vs. dynamic allocation?**
   
**Advantages**
<summary>compile-time No need to manage memory manually its faster</summary>
<summary>dynamic allocation  flexible size at runtime , can grow/shrink based on need</summary>

**drawback**
<summary>compile-time  hat fixed size and not suitable for unknow data size</summary>
<summary>dynamic allocation hat risk of memory leaks if mismanaged and slightly slower due to heap allocation</summary>

3. **How would you extend this static list to include additional data fields?**
<summary>By updating DNode structure in dlist.h</summary>
---

### Task 2: Interactive Singly Linked List

**Objective:** Build a menu-driven program to manipulate a singly linked list at runtime.

1. Define `SNode` and function prototypes in `solutions/slist.h`:

   ```c
   typedef struct SNode {
       int value;
       struct SNode *next;
   } SNode;

   SNode* add_node(SNode *head, int value);
   int    count_nodes(SNode *head);
   void   free_list(SNode *head);
   ```
2. Implement `add_node`, `count_nodes`, and `free_list` in `solutions/slist.c` using `malloc` and `free`.
3. Create `solutions/slist_main.c`:

   ```c
   #include <stdio.h>
   #include "slist.h"

   int main(void) {
       SNode *head = NULL;
       int choice, val;

       do {
           printf("1) Count nodes\n2) Add node\n3) Exit\n> ");
           if (scanf("%d", &choice) != 1) break;
           switch (choice) {
               case 1:
                   printf("Count: %d\n", count_nodes(head));
                   for (SNode *p = head; p; p = p->next)
                       printf("Node at %p: %d\n", (void*)p, p->value);
                   break;
               case 2:
                   printf("Value: ");
                   scanf("%d", &val);
                   head = add_node(head, val);
                   break;
               case 3:
                   free_list(head);
                   break;
               default:
                   printf("Invalid option\n");
           }
       } while (choice != 3);

       return 0;
   }
   ```
4. Compile and run:

   ```bash
   gcc -c solutions/slist.c -o solutions/slist.o
   gcc -o solutions/slist_main solutions/slist_main.c solutions/slist.o
   ./solutions/slist_main
   ```

#### Reflection Questions

1. **Why is `malloc` necessary when adding nodes dynamically?**
<summary>malloc allows your list to grow dynamically without a fixed size.</summary>

2. **How can you traverse the list to print each node’s address and value?**
<summary>By  using a loop</summary>

3. **What are the consequences of not freeing the list before exit?**
<summary>If you don’t call free_list , the memory you allocated with malloc is never returned,resulting in a memory leak.</summary>

---

### Task 3: JSON Array to Linked List

**Objective:** Parse a JSON array from a file into a linked list of C structs, using `getopt` to accept the input filename.

1. Create `solutions/json_list.h`:

   ```c
   #include <jansson.h>
   typedef struct JObject {
       char *name;
       int age;
       struct JObject *next;
   } JObject;

   JObject* parse_json(const char *filename);
   void     print_list(JObject *head);
   void     free_list(JObject *head);
   ```

2. Implement `solutions/json_list.c`:

   * Use `json_load_file` to load an array of objects with fields `"name"` and `"age"`.
   * For each element, allocate a `JObject`, duplicate the `name`, store `age`, and link into a list.

3. Create `solutions/json_main.c` to use `getopt`:

   ```c
   #include "json_list.h"
   #include <stdio.h>
   #include <stdlib.h>
   #include <unistd.h>

   void usage(const char *prog) {
       fprintf(stderr, "Usage: %s -i <file.json>
   ```

", prog);
exit(EXIT\_FAILURE);
}

int main(int argc, char \*argv\[]) {
int opt;
char \*filename = NULL;

```
   while ((opt = getopt(argc, argv, "i:")) != -1) {
       if (opt == 'i') {
           filename = optarg;
       } else {
           usage(argv[0]);
       }
   }
   if (!filename) {
       usage(argv[0]);
   }

   JObject *head = parse_json(filename);
   print_list(head);
   free_list(head);
   return 0;
```

}

````
4. Compile and run:
```bash
gcc -c solutions/json_list.c -o solutions/json_list.o -ljansson
gcc -o solutions/json_main solutions/json_main.c solutions/json_list.o -ljansson
./solutions/json_main -i data.json
````

#### Suggested Student Modifications

1. **Add** `void remove_by_age(JObject **head, int age)` to remove matching nodes.
2. **Extend** `JObject` with a new field (e.g., `char *email`) and update parsing accordingly.

#### Reflection Questions

1. **How does using `getopt` make the program more flexible than `argv[1]`?**
<summary>getopt allows: named options , order independent arguments,extensibility for future flags</summary>

2. **What happens if the user omits the `-i` option?**
<summary>The program prints usage and exists</summary>

3. **How can you validate that the JSON file loaded is indeed an array?**
<summary>By using this syntax</summary>

````bash
if (!json_is_array(root)) {
    fprintf(stderr, "Error: root is not a JSON array\n");
    json_decref(root);
    return NULL;
}
````

---

**Remember:** Stop after **90 minutes** and record where you stopped.
