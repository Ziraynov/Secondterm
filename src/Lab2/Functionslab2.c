#include "Mainlab2.h"

void print_file_size(const char *filename) {
    FILE *file;
    fopen_s(&file, filename, "rb");

    if (file == NULL) {
        printf("Ошибка при открытии файла!");
        return;
    }

    fseek(file, 0L, SEEK_END);
    long int size = ftell(file);

    printf("Размер файла '%s' равен %ld байт\n", filename, size);

    fclose(file);
}

int if_letter(const char *word, int i) {
    if (i > 1024 - 1)
        exit(1);
    if ((word[i] >= 'A' && word[i] <= 'Z') || (word[i] >= 'a' && word[i] <= 'z') ||
        (word[i] >= '0' && word[i] <= '9') || word[i] == -103)
        return 0;
    return 1;

}

void check(char *buffer, int i, int *flag, int priority, int word_len) {
    if (if_letter(buffer, i) != 0) {
        *flag = 1;
        priority = 1;
    }
    if (if_letter(buffer, word_len - 1) != 0)
        *flag = 2;

    if (*flag == 1 || priority == 1)
        while (if_letter(buffer, i) != 0) {
            for (int k = 0; k < word_len - 1; k++)
                buffer[k] = buffer[k + 1];
            buffer[word_len--] = '\0';

        }
}

char *only_symbols(char *buffer) {
    int word_len = (int) strlen(buffer);
    int i = 0;
    int flag = 0;
    int priority = 0;

    while (if_letter(buffer, i) != 0 || if_letter(buffer, word_len - 1) != 0) {

        check(buffer, i, &flag, priority, word_len);
        if (flag == 2)
            while (if_letter(buffer, word_len - 1) != 0) {
                buffer[--word_len] = '\0';
            }
    }
    buffer[word_len] = '\0';
    return buffer;
}


LIFO *create(LIFO *head, int count, char *word) {
    LIFO *n;
    n = (LIFO *) malloc(2 * sizeof(LIFO));
    n->next = NULL;
    n->words = word;
    n->amount = count;
    if (head == NULL)
        head = n;
    else {
        n->next = head;
        head = n;
    }
    return head;
}

short_words *struct_short_words(LIFO *head, int *size, int len, int pop) {

    short_words *buffer;
    buffer = (short_words *) calloc(1, sizeof(short_words));
    int i = 0;
    LIFO *p = head;
    while (p != NULL) {
        if (strlen(p->words) < len && p->amount < pop) {
            buffer = (short_words *) realloc(buffer, (i + 1) * sizeof(short_words));
            buffer[i].short_word = (char *) calloc(((int) strlen(p->words) + 1), sizeof(char));
            buffer[i].short_word = p->words;
            i++;
        }
        p = p->next;
    }
    *size = i;
    return buffer;
}

long_words *struct_long_words(LIFO *head, int *size, int len, int pop) {
    long_words *buffer;
    buffer = (long_words *) calloc(1, sizeof(long_words));
    int i = 0;
    LIFO *p = head;
    while (p != NULL) {
        if (strlen(p->words) >= len && p->amount >= pop) {
            buffer = (long_words *) realloc(buffer, (i + 1) * sizeof(long_words));
            buffer[i].long_word = (char *) calloc(((int) strlen(p->words) + 1), sizeof(char));
            buffer[i].long_word = p->words;
            i++;
        }
        p = p->next;
    }
    *size = i;

    return buffer;
}

int if_real_word(const char *buffer, const char *word) {
    int k = 0;
    char *word1 = (char *) calloc((int) strlen(buffer), sizeof(char));
    char *chek_symbol = (char *) calloc((int) strlen(buffer), sizeof(char));
    while (strcmp(chek_symbol, " ") != 0) {
        if (strcmp(chek_symbol, "\n") == 0) { break; }

        strncat_s(word1, 1, &buffer[k], 2);
        k++;
        strncpy_s(chek_symbol, 1, &buffer[k], 2);
    }
    free(chek_symbol);
    if (word1[(int) strlen(word1)] == '\n')
        word1[(int) strlen(word1)] = '\0';
    word1 = only_symbols(word1);
    if ((int) strlen(word1) != (int) strlen(word)) {
        free(word1);
        return 0;
    }
    k = 0;
    for (int i = 0; i < (int) strlen(word); i++)
        if (word1[i] == word[i])
            k++;
    if (k == (int) strlen(word))
        return 1;
    return 0;

}

char *long_to_short(const char *buffer, const char *longword, const char *shortword) {
    char *newbuffer = (char *) calloc((int) strlen(buffer) + 100, sizeof(char));
    while (strstr(buffer, longword)) {
        const char *start_of_longw;
        start_of_longw = strstr(buffer, longword);
        int len = ((int) strlen(buffer) - (int) strlen(start_of_longw));
        if (if_real_word(start_of_longw, longword) == 1 && if_letter(buffer, len - 1) == 1) {
            strncat_s(newbuffer, len, buffer, len - 1);
            strcat_s(newbuffer, 10000, shortword);
            buffer += len + (int) strlen(longword);
        } else {
            strncat_s(newbuffer, len, buffer, len - 1);
            strcat_s(newbuffer, 10000, longword);
            buffer += len + (int) strlen(longword);
        }
    }
    strncat_s(newbuffer, strlen(buffer), buffer, strlen(buffer) - 1);
    return newbuffer;
}

char *short_to_long(const char *buffer, const char *longword, const char *shortword) {
    char *newbuffer = (char *) calloc((int) strlen(buffer) + 100, sizeof(char));
    while (strstr(buffer, shortword) != 0) {
        const char *start_of_shortw;
        start_of_shortw = strstr(buffer, shortword);
        int len = ((int) strlen(buffer) - (int) strlen(start_of_shortw));
        if (if_real_word(start_of_shortw, shortword) == 1 && if_letter(buffer, len - 1) == 1) {
            strncat_s(newbuffer, len, buffer, len - 1);
            strncat_s(newbuffer, (int) strlen(longword), longword, (int) strlen(longword) - 1);
            buffer += len + (int) strlen(shortword);
        } else {
            strncat_s(newbuffer, len, buffer, len - 1);
            strcat_s(newbuffer, 10000, shortword);
            buffer += len + (int) strlen(shortword);
        }

    }
    strncat_s(newbuffer, (int) strlen(buffer), buffer, (int) strlen(buffer) - 1);
    return newbuffer;
}

char *replace_words(const char *buffer, const char *lw, const char *sw) {
    char *newbuffer = (char *) calloc((int) strlen(buffer) + 100, sizeof(char));
    while (strstr(buffer, lw) != NULL || strstr(buffer, sw) != NULL) {

        if (((int *) strstr(buffer, lw) < (int *) strstr(buffer, sw)) && strstr(buffer, lw) != 0 ||
            (strstr(buffer, sw) == NULL && strstr(buffer, lw) != NULL)) {
            const char *start_of_longw;
            start_of_longw = strstr(buffer, lw);
            int len = ((int) strlen(buffer) - (int) strlen(start_of_longw));
            if (if_real_word(start_of_longw, lw) == 1 && if_letter(buffer, len - 1) == 1) {
                strncat_s(newbuffer, len, buffer, len - 1);
                strcat_s(newbuffer, 10000, sw);
                buffer += len + (int) strlen(lw);
            } else {
                strncat_s(newbuffer, len, buffer, len - 1);
                strcat_s(newbuffer, 100000, lw);
                buffer += len + (int) strlen(lw);
            }
        }
        if (((int *) strstr(buffer, sw) < (int *) strstr(buffer, lw)) && strstr(buffer, sw) != 0 ||
            (strstr(buffer, lw) == NULL && strstr(buffer, sw) != NULL)) {
            const char *start_of_shortw;
            start_of_shortw = strstr(buffer, sw);
            int len = ((int) strlen(buffer) - (int) strlen(start_of_shortw));
            if (if_real_word(start_of_shortw, sw) == 1 && if_letter(buffer, len - 1) == 1) {
                strncat_s(newbuffer, len, buffer, len);
                strncat_s(newbuffer, (int) strlen(lw), lw, (int) strlen(lw));
                buffer += len + (int) strlen(sw);
            } else {
                strncat_s(newbuffer, len, buffer, len);
                strcat_s(newbuffer, 10000, sw);
                buffer += len + (int) strlen(sw);
            }

        }
    }
    strncat_s(newbuffer, (int) strlen(buffer), buffer, (int) strlen(buffer));
    return newbuffer;
}

const char *new_str(const char *buffer, const char *longword, const char *shortword) {
    const char *newbuffer;
    if (strstr(buffer, longword) != NULL && strstr(buffer, shortword) != NULL) {
        newbuffer = replace_words(buffer, longword, shortword);
        return newbuffer;
    }
    if (strstr(buffer, longword) != NULL && strstr(buffer, shortword) == NULL) {

        newbuffer = long_to_short(buffer, longword, shortword);
        return newbuffer;

    }
    if (strstr(buffer, shortword) != NULL && strstr(buffer, longword) == NULL) {

        newbuffer = short_to_long(buffer, longword, shortword);
        return newbuffer;
    }

    return buffer;
}

void free_structl(long_words *data) {
    free(data->long_word);
    data->long_word = NULL;
}

void free_structs(short_words *data) {
    free(data->short_word);
    data->short_word = NULL;
}

void rewrite_file(const char *old_file, const char *new_file, long_words *words, short_words *words1, int size) {
    FILE* file;
    if (fopen_s(&file, old_file, "r") != 0) {
        printf("Ошибка при открытии файла!\n");
        return;
    }

    FILE* newfile;
    if (fopen_s(&newfile, new_file, "w") != 0) {
        printf("Ошибка при открытии файла!\n");
        fclose(file);
        return;
    }

    int amount = 0;
    for (int i = 0; i < size; i++)
        amount += 2 + (int)strlen(words[i].long_word) + (int)strlen(words1[i].short_word);

    char* am = (char*)calloc(100, sizeof(char));  // Increased the size to accommodate the number
    _itoa_s(amount, am, 100, 10);
    fputs(am, newfile);
    fputs(" ", newfile);
    free(am);

    for (int i = 0; i < size; i++) {
        fputs(" ", newfile);
        fputs(words[i].long_word, newfile);
        fputs(" ", newfile);
        fputs(words1[i].short_word, newfile);
    }
    fputs("\n", newfile);

    char line[5001];
    char* buffer = (char*)calloc(5001, sizeof(char));

    while (fgets(line, sizeof(line), file)) {
        strncpy(buffer, line, sizeof((char*)buffer));

        for (int i = 0; i < size - 1; i++) {
            char* modified = (char*)new_str(buffer, words[i].long_word, words1[i].short_word);
            if (modified != NULL) {
                free(buffer);
                buffer = modified;
            }
        }

        fputs(buffer, newfile);
    }

    free(buffer);
    fclose(file);
    fclose(newfile);
    free_structl(words);
    free_structs(words1);
}



void free_stack(LIFO **head) {
    while (*head != NULL) {
        LIFO *tmp = *head;
        *head = (*head)->next;
        free(tmp);
    }
}

void new_file(const char *path, LIFO *head, int len_of_words, int popularity) {
    long_words *words;
    short_words *words1;
    int sizelong;
    int sizeshort;
    words = struct_long_words(head, &sizelong, len_of_words, popularity);
    words1 = struct_short_words(head, &sizeshort, len_of_words, popularity);
    if (sizelong > sizeshort)
        sizelong = sizeshort;
    else
        sizeshort = sizelong;
    for (int i = 0; i < sizeshort; i++) {

        printf(" LONG %s SHORT %s \n", words[i].long_word, words1[i].short_word);
    }
    const char *pathnew = "C:/Users/ziray/CLionProjects/Secondterm/src/Lab2/newfile.txt";
    free_stack(&head);
    rewrite_file(path, pathnew, words, words1, sizeshort);
free(words1);
free(words);
}

void printf_stack(LIFO *head, int *all_words, int *several_len) {
    LIFO *p = head;

    while (p != NULL) {
        printf("%s ", p->words);
        (*several_len) += (int) strlen(p->words);
        printf("%d\n", p->amount);
        (*all_words) += p->amount;
        p = p->next;
    }

}

int get_word(char **word, int *pos, const char *path) {
    FILE *file;
    if ((fopen_s(&file, path, "r")) != 1) {
        fprintf(stderr, "Error opening file %s\n", path);
        exit(EXIT_FAILURE);
    }
    fseek(file, *pos, SEEK_SET);
    char *word1 = (char *) malloc(100 * sizeof(char));
    fscanf_s(file, "%99s", word1, 100);
    (*word) = _strdup(word1);
    *pos = ftell(file);
    free(word1);
    if (feof(file) != 0)
        return 0;
    fclose(file);
    return 1;
}

void compression(const char *path, LIFO **head) {
    int number_of_uniq_words = 0;
    int all_words = 0;
    int several_len = 0;
    char *word = (char *) calloc(1024, sizeof(char));
    int pos = 0;
    int flag = 0;
    int flag1 = 0;
    FILE *file;
    if ((fopen_s(&file, path, "r")) != 1) {
        fprintf(stderr, "Error opening file %s\n", path);
        exit(EXIT_FAILURE);
    }
    while (get_word(&word, &pos, path) != 0) {
        for (int i = 0; i < (int) strlen(word)-1; i++) {
            if (if_letter(word, i) == 0)
                break;
            if (i == (int) strlen(word) - 1)
                flag = 2;
        }
        if (flag == 2) {
            flag = 0;
            continue;
        }
        LIFO *p = *head;
        word = only_symbols(word);
        if (strstr(word, "-") != NULL)
            flag = 1;
        while (p != NULL && flag != 1) {
            if (!strcmp(word, p->words)) {
                p->amount++;
                flag = 4;
                flag1 = 1;
                break;
            } else
                p = p->next;
        }
        if (flag == 0 || flag1 == 0) {
            *head = create(*head, 1, word);
            number_of_uniq_words++;
        }
        flag = 0;
    }
    free(word);
    printf_stack(*head, &all_words, &several_len);
    printf("\nALL WORDS:%d UNIQ: %d SEV_LEN%d sevpop %d \n", all_words, number_of_uniq_words,
           several_len / (number_of_uniq_words+1),
           all_words / (number_of_uniq_words+1));
    fclose(file);
    new_file(path, *head, several_len / (number_of_uniq_words+1), all_words / (number_of_uniq_words+1));
}
