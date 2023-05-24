#include "Mainlab4.h"

void functions(int *x) {
    printf("This is akkinator. Lets play!\n");
    char *UserName = (char *) calloc(KB, sizeof(char));
    getUserName(UserName);
    TREE *Tree = NULL;
    FILE *Data = fopen(path, "r");
    char *Word = (char *) calloc(KB, sizeof(char));
    char *GetUnswer = (char *) calloc(KB, sizeof(char));
    getWord(Word, UserName);
    Tree = createFromData(Tree, Data);
    diagram(Tree);
    treePrint(Tree, &GetUnswer, UserName);
    fclose(Data);
    checkUnswer(Word, Tree, Data, GetUnswer, UserName);
    openSite(UserName);
    freeTree(Tree);
    free(Word);
    getMenuIndex(x, UserName);
}

void openSite(char *UserName) {
    printf("You wanna open top 30 HLTV?\n");
    if (getUn(UserName) == 1) {
        const char *url = "https://www.hltv.org/ranking/teams/2023/april/17";
        char cmd[256];
        snprintf(cmd, sizeof(cmd), "start %s", url);
        system(cmd);
    }
}

void getMenuIndex(int *x, char *UserName) {
    printf("Enter 1, if you wanna play. Enter 2,if you wanna end.\n");
    printf("Entre your choice:");
    while (scanf_s("%d", x) != 1 || getchar() != '\n' || ((*x) != 1 && (*x) != 2)) {
        printf("Error,try againg\n");
        rewind(stdin);
    }
    char *y = (char *) calloc(KB, sizeof(char));
    itoa(*x, y, 10);
    strcat(y, "\n");
    addLogs(y, UserName);
}

void getWord(char *Word, char *UserName) {
    printf("Input your csgo team:");
    fgets(Word, KB - 1, stdin);
    rewind(stdin);
    addLogs(Word, UserName);
}

void getQuestion(char *Quest, char *UserName) {
    fgets(Quest, KB - 1, stdin);
    rewind(stdin);
    addLogs(Quest, UserName);
}

void makeNewDataBase(char *Quest, char *Word, TREE *Tree, FILE *Data, char *GetUnswer) {

    if (Tree != NULL) {
        int Flag = 0;
        if (strcmp(Tree->data, GetUnswer) == 0 || strcmp(Tree->data, GetUnswer) == 0) {
            fputs(Quest, Data);
            Flag = 1;
        }
        fputs(Tree->data, Data);
        if (Flag == 1)
            fputs("#\n", Data);
        makeNewDataBase(Quest, Word, Tree->left, Data, GetUnswer);
        fputs("#\n", Data);
        if (Flag == 1) {
            fputs(Word, Data);
            fputs("#\n", Data);
        }

        makeNewDataBase(Quest, Word, Tree->right, Data, GetUnswer);
    }


}

void addNewElementInDataBase(char *Word, TREE *Ttree, FILE *Data, char *getUnswer, char *UserName) {
    printf("So, what differences between my answer and your gues?Enter your question:\n ");
    char *quest = (char *) calloc(KB, sizeof(char));
    getQuestion(quest, UserName);
    Data = fopen(path, "w");
    makeNewDataBase(quest, Word, Ttree, Data, getUnswer);
    fputs("#", Data);
    fclose(Data);

}

void checkUnswer(char *Word, TREE *Tree, FILE *Data, char *getUnswer, char *UserName) {
    printf("↑↑↑↑↑↑This this the answer↑↑↑↑↑↑\n");
    printf("You guessed: %sThis is a right unswer?\n", Word);
    if (getUn(UserName) == 0)
        addNewElementInDataBase(Word, Tree, Data, getUnswer, UserName);

}

int getUn(char *UserName) {
    printf("Input your unswer(Y/N)");
    char *Str = (char *) calloc(KB, sizeof(char));
    fgets(Str, KB - 1, stdin);
    rewind(stdin);
    while ((int) strlen(Str) != 2) {
        printf("Error, try againg!");
        fgets(Str, KB - 1, stdin);
        addLogs(Str, UserName);
    }
    addLogs(Str, UserName);
    if (strstr(Str, "Y") != 0 || strstr(Str, "y") != 0) {
        return 1;
    } else
        return 0;

}

void treePrint(TREE *Tree, char **getUnswer, char *UserName) {
    if (Tree != NULL) {
        printf("%s", Tree->data);
        if (Tree->left == NULL && Tree->right == NULL)
            (*getUnswer) = Tree->data;
        if (Tree->left != NULL || Tree->right != NULL) {
            int flag = getUn(UserName);
            if (flag == 0)
                treePrint(Tree->left, getUnswer, UserName);
            if (flag == 1)
                treePrint(Tree->right, getUnswer, UserName);

        }
    }

}

TREE *createFromData(TREE *Tree, FILE *Data) {
    if (feof(Data) == 1) {
        return NULL;
    }
    char *Str = (char *) calloc(KB, sizeof(char));
    fgets(Str, KB - 1, Data);
    if (strstr(Str, "#") != 0) {
        return NULL;
    }
    Tree = (TREE *) calloc(1, sizeof(TREE));
    Tree->data = Str;
    Tree->left = NULL;
    Tree->right = NULL;
    Tree->left = createFromData(Tree->left, Data);
    Tree->right = createFromData(Tree->right, Data);
    return Tree;
}

void freeTree(TREE *Tree) {
    if (Tree->right == NULL || Tree->left == NULL) {
        free(Tree->data);
        free(Tree);
        return;
    }

    freeTree(Tree->left);
    freeTree(Tree->right);

    free(Tree->data);
    free(Tree);
}

void dot(TREE *Tree, FILE *file) {

    if (Tree) {
        if (Tree->right != NULL) fprintf(file, "\"%s\"-> \"%s\"\n", Tree->data, Tree->right->data);
        dot(Tree->right, file);
        if (Tree->left != NULL) fprintf(file, "\"%s\"-> \"%s\"\n", Tree->data, Tree->left->data);
        dot(Tree->left, file);
    }

}

void diagram(TREE *Tree) {
    FILE *fp = fopen("../src/lab4/tree.dot", "w");
    fputs("digraph G {\n", fp);
    dot(Tree, fp);
    fputs("}\n", fp);
    fclose(fp);
}

void addLogs(char *UserUnswer, char *UserName) {
    FILE *logs;
    errno_t ErrFile = fopen_s(&logs, "C:\\Users\\ziray\\CLionProjects\\Secondterm\\src\\Lab4\\Loggs.txt", "a+");
    if (ErrFile != 0) {
        printf("Cant open file");
        exit(ErrFile);
    }
    char *str_log = (char *) calloc(KB, sizeof(char));
    if (str_log != NULL) {
        time_t mytime = time(NULL);
        struct tm *now = localtime(&mytime);
        char str[15];
        strftime(str, sizeof(str), "%x", now);
        fputs("Data:", logs);
        fputs(str, logs);
        fputs(SPACE, logs);
        strftime(str, sizeof(str), "%X", now);
        fputs("Time:", logs);
        fputs(str, logs);
        fputs(SPACE, logs);
        fputs(UserName, logs);
        fputs(" says:", logs);
        fputs(UserUnswer, logs);
        free(str_log);
        fclose(logs);
    }
}

void getUserName(char *Name) {
    printf("Enter a user Name:\n");
    fgets(Name, KB - 1, stdin);
    rewind(stdin);
}