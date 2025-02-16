#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Song structure as a Binary Search Tree (BST)
struct songlistss {
    char song[100];            
    char link[200];            
    struct songlistss* left;   
    struct songlistss* right;  
};

// Language structure as a DLL
struct languages {
    char lang[20];              
    struct songlistss* artist;        //pointer referring to songlistsss strcuture......we add song by inserting language
    struct languages* lelink;  
    struct languages* rilink;   
};

// Genre structure as a DLL
struct genress {
    char genre[20];             
    struct languages* language;      //pointer referring to languages strcuture......we add language by inserting genre
    struct genress* llink;     
    struct genress* rlink;      
};

// Type definitions
typedef struct languages* Lang;
typedef struct genress* Genre;
typedef struct songlistss* SL;

// Function declarations
Genre addgenre(Genre head, char* g);
Genre addlang(Genre head, char* l,char* g);
SL addsong(SL root, char* s, char* link);
Genre addsongtog(Genre head, char* g, char* l, char* s, char* link);
void recommend(Genre head, char* g);
void display(Genre head);
void open_url(const char* s);

// Add a new genre
Genre addgenre(Genre head, char* g) {
    Genre temp = (Genre)malloc(sizeof(struct genress));
    strcpy(temp->genre, g);
    temp->language = NULL;
    temp->rlink = NULL;
    temp->llink = NULL;

    if (head == NULL) {
        return temp; // First genre
    }

    Genre cur = head;
    while (cur->rlink != NULL) {
        if (strcmp(cur->genre, g) == 0) { // Check if genre exists
            printf("Genre '%s' already exists.\n", g);
            free(temp);
            return head;
        }
        cur = cur->rlink;
    }
     // after traversing all nodes,checks for the genre in the last node
    if (strcmp(cur->genre, g) == 0) {
        printf("Genre '%s' already exists.\n", g);
        free(temp);
        return head;
    }

    cur->rlink = temp;
    temp->llink = cur;
    return head;
}

// Add a new language
Genre addlang(Genre head, char* g, char* l) {
    Genre cur = head;// Find the genre 
    while (cur != NULL && strcmp(cur->genre, g) != 0) {
        cur = cur->rlink;
    }
    if (cur == NULL) {
        printf("Genre '%s' not found.\n", g);
        return head;
    }
    Lang langHead = cur->language;// Acts like Head node of Lang DLL
    Lang temp = (Lang)malloc(sizeof(struct languages));
    strcpy(temp->lang, l);
    temp->artist = NULL;
    temp->lelink = NULL;
    temp->rilink = NULL;
    if (langHead == NULL) {
        cur->language = temp; // First language
        return head;
    }
    Lang langCur = langHead;
    while (langCur->rilink != NULL) {
        if (strcmp(langCur->lang, l) == 0) { // Language already exists
            printf("Language '%s' already exists in genre '%s'.\n", l, g);
            free(temp); // Avoid memory leak
            return head;}
        langCur = langCur->rilink;
    }
    if (strcmp(langCur->lang, l) == 0) {
        printf("Language '%s' already exists in genre '%s'.\n", l, g);
        free(temp);
        return head;
    }
    langCur->rilink = temp;
    temp->lelink = langCur;
    return head;
}
// Add a song to the BST
SL addsong(SL root, char* s, char* link) {
    if (root == NULL) {
        SL temp = (SL)malloc(sizeof(struct songlistss));
        strcpy(temp->song, s);
        strcpy(temp->link, link);
        temp->left = NULL;
        temp->right = NULL;
        return temp; }  
    if (strcmp(s, root->song) < 0) {
        root->left = addsong(root->left, s, link);
    } else if (strcmp(s, root->song) > 0) {
        root->right = addsong(root->right, s, link);
    } else {
        printf("Song '%s' already exists.\n", s);}
    return root;
}
// Add a song to a genre and language
Genre addsongtog(Genre head, char* g, char* l, char* s, char* link) {
    Genre cur = head;
    while (cur != NULL && strcmp(cur->genre, g) != 0) {
        cur = cur->rlink; }
    if (cur == NULL) {
        printf("Genre '%s' not found.\n", g);
        return head; }
    Lang langCur = cur->language;
    while (langCur != NULL && strcmp(langCur->lang, l) != 0) {
        langCur = langCur->rilink; }
    if (langCur == NULL) {
        printf("Language '%s' not found in genre '%s'.\n", l, g);
        return head;
    }
    langCur->artist = addsong(langCur->artist, s, link);
    return head;
}

// Recommend songs by genre
void recommend(Genre head, char* g) {
    Genre cur = head;
    while (cur != NULL && strcmp(cur->genre, g) != 0) {
        cur = cur->rlink;
    }
    if (cur != NULL) {
        printf("\nSongs recommended in genre '%s':\n", g);
        Lang langc = cur->language;
        while (langc != NULL) {
            printf("Language: %s\n", langc->lang);
            SL songc = langc->artist;
            while (songc != NULL) {
                printf("\tSong: %s - Link: %s\n", songc->song, songc->link);
                songc = songc->right;
            }
            langc = langc->rilink;
        }
    } else {
        printf("Genre '%s' not found.\n", g);
    }
}

// Display the collection
void display(Genre head) {
    if (head == NULL) {
        printf("No genres available.\n");
        return;
    }

    Genre cur = head;
    while (cur != NULL) {
        printf("\nGenre: %s\n", cur->genre);
        Lang langc = cur->language;
        while (langc != NULL) {
            printf("\tLanguage: %s\n", langc->lang);
            SL songc = langc->artist;
            while (songc != NULL) {
                printf("\t\tSong: %s - Link: %s\n", songc->song, songc->link);
                songc = songc->right;
            }
            langc = langc->rilink;
        }
        cur = cur->rlink;
    }
}

// Open a song URL
void open_url(const char* url) {
    char command[256];
    snprintf(command, sizeof(command), "start %s", url); // Format command for system call
    system(command); // Execute command
}

// Main function
int main() {
    Genre gHead = NULL;
    int choice;
    char genre[20], language[20], song[100], link[200];

    while (1) {
        printf("\nMenu:\n");
        printf("1. Add Genre\n");
        printf("2. Add Language to Genre\n");
        printf("3. Add Song to Language\n");
        printf("4. Recommend Songs by Genre\n");
        printf("5. Open Song URL\n");
        printf("6. Display Collection\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); 

        switch (choice) {
            case 1:
                printf("Enter Genre: ");
                scanf("%[^\n]", genre);
                gHead = addgenre(gHead, genre);
                break;
            case 2:
               printf("Enter Genre to add Language to: ");
               scanf(" %[^\n]", genre);
               printf("Enter Language: ");
               scanf(" %[^\n]", language);
               gHead = addlang(gHead, genre, language);
               break;
            case 3:
                printf("Enter Genre: ");
                scanf("%[^\n]", genre);
                getchar();
                printf("Enter Language: ");
                scanf("%[^\n]", language);
                getchar();
                printf("Enter Song: ");
                scanf("%[^\n]", song);
                getchar();
                printf("Enter Song URL: ");
                scanf("%[^\n]", link);
                gHead = addsongtog(gHead, genre, language, song, link);
                break;
            case 4:
                printf("Enter Genre to recommend songs: ");
                scanf("%[^\n]", genre);
                recommend(gHead, genre);
                break;
            case 5:
                printf("Enter Song URL to open: ");
                scanf("%[^\n]", link);
                open_url(link);
                break;
            case 6:
                display(gHead);
                break;
            case 7:
                printf("Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice! Try again.\n");
        }
    }
    return 0;
}
