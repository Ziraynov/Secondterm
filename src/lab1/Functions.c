#include "Header.h"

void get_key(int *key) {
    while (!scanf_s("%d", key) || *key > 5 || *key <= 0 || getchar() != '\n') {
        rewind(stdin);
        printf("Error,try againg");

    }
}

void find_number(int *number, const char *html_string) {
    const char *buf = strstr(html_string,
                             "class='result__attr_var  cr-result__attr_odd'>Фактическая диагональ</td><td class='result__attr_val");
    while (buf != NULL) {
        buf = strstr(strstr(buf, "class='result__attr_var '>Разрешение</td><td class='result__attr_val"),
                     "class='result__attr_var  cr-result__attr_odd'>Фактическая диагональ</td><td class='result__attr_val");
        (*number)++;
    }
}

monitor *parse_html(const char *path, int *number) {
    char *html_string = file(path);
    find_number(number, html_string);
    monitor *arr = calloc(*number, sizeof(monitor));
    for (int i = 0; i < *number; i++) {
        arr[i].name = get_the_word(&html_string, "class=\"\" alt=\"", " - \"/>");

        if (second_word("LG", arr[i].name)) {
            arr[i].producer = LG;
        } else if (second_word("Samsung", arr[i].name)) {
            arr[i].producer = Samsung;
        } else if (second_word("AOC", arr[i].name)) {
            arr[i].producer = AOC;
        } else if (second_word("GigaByte", arr[i].name)) {
            arr[i].producer = GigaByte;
        } else if (second_word("Philips", arr[i].name)) {
            arr[i].producer = Philips;
        } else if (second_word("BenQ", arr[i].name)) {
            arr[i].producer = BenQ;
        } else if (second_word("HP", arr[i].name)) {
            arr[i].producer = HP;
        } else if (second_word("Asus", arr[i].name)) {
            arr[i].producer = Asus;
        } else {
            arr[i].producer = Huawei;
        }

        arr[i].matrix = get_the_word(&html_string,
                                     "</td></tr><tr><td class='result__attr_var  cr-result__attr_odd'>Тип матрицы</td><td class='result__attr_val  cr-result__attr_odd'>",
                                     "</td></tr><tr><td class='result__attr_var '>");
        arr[i].price = atof(get_the_word(&html_string, "price=\"", "\" data"));

    }
    return arr;
}

const char *const monitor_producers[] =
        {
                [LG] = "LG",
                [Samsung] = "Samsung",
                [GigaByte] = "GigaBite",
                [AOC] = "AOC",
                [Huawei] = "Huawei",
                [Philips] = "Philips",
                [BenQ] = "BenQ",
                [HP] = "HP",
                [Asus] = "Asus"
        };

void show(monitor *array, int len) {
    for (int i = 0; i < len; i++) {
        printf("%2d. %s |||| Price: %5.2fBYN. |||| Matrix: %s. |||| Producer: %s.\n", i + 1, array[i].name,
               array[i].price,
               array[i].matrix,
               monitor_producers[array[i].producer]);
    }
}

void sort(monitor *arr, int len) {
    printf("1. Name\n2. Price\n3. Producers\n4. Name and Price\n5. Producer and Matrix\n0. Exit\n");
    int key;
    get_key(&key);
    switch (key) {
        default:
            printf("Fatal ERROR");
            break;
        case 1:
            qsort(arr, len, sizeof(monitor), (int (*)(const void *, const void *)) comp_name);
            break;
        case 2:
            qsort(arr, len, sizeof(monitor), (int (*)(const void *, const void *)) comp_price);
            break;
        case 3:
            qsort(arr, len, sizeof(monitor), (int (*)(const void *, const void *)) comp_producer);
            break;
        case 4:
            qsort(arr, len, sizeof(monitor), (int (*)(const void *, const void *)) comp_name_price);
            break;
        case 5:
            qsort(arr, len, sizeof(monitor), (int (*)(const void *, const void *)) comp_producer_matrix);
            break;
    }

}

void add_word(monitor *arr, int *len) {
    char *add = malloc(1);
    printf("Input monitor [|name/matrix|price/]: ");
    rewind(stdin);
    fgets(add, 1000, stdin);
    const char *str = "/";
    const char *str2 = "|";
    ++(*len);

    arr[*len - 1].name = get_the_word(&add, "|", "/");
    if (second_word("LG", arr[0].name)) {
        arr[*len - 1].producer = LG;
    } else if (second_word("Samsung", arr[*len - 1].name)) {
        arr[*len - 1].producer = Samsung;
    } else if (second_word("AOC", arr[*len - 1].name)) {
        arr[*len - 1].producer = AOC;
    } else if (second_word("GigaByte", arr[*len - 1].name)) {
        arr[*len - 1].producer = GigaByte;
    } else if (second_word("Philips", arr[*len - 1].name)) {
        arr[*len - 1].producer = Philips;
    } else if (second_word("BenQ", arr[*len - 1].name)) {
        arr[*len - 1].producer = BenQ;
    } else if (second_word("HP", arr[*len - 1].name)) {
        arr[*len - 1].producer = HP;
    } else if (second_word("Asus", arr[*len - 1].name)) {
        arr[*len].producer = Asus;
    } else {
        arr[*len - 1].producer = Huawei;
    }
    arr[*len - 1].matrix = get_the_word(&add, str, str2);
    arr[*len - 1].price = atof(get_the_word(&add, str2, str));

}

void rem(monitor *arr, int *len) {
    int p;
    printf("Choose number[1-%d]: ", *len);
    while (!scanf_s("%d", &p) || p < 0 || p > *len || getchar() != '\n') {
        printf("Error,try againg");
        rewind(stdin);
    }
    for (int i = 0; i < *len; i++) {
        if (strstr(arr[i].name, arr[p - 1].name) != 0) {

            for (int j = i; j < *len - 1; j++) {
                arr[j] = arr[j + 1];
            }
            (*len)--;
            break;
        }
    }
}

char *file(const char *path) {
    FILE *fp;
    long len;
    fopen_s(&fp, path, "r");
    if (fp != NULL) {
        fseek(fp, 0, SEEK_END);
        len = ftell(fp);
        fseek(fp, 0, SEEK_SET);
    } else
        exit(1);
    char *buffer = calloc(len + 2, sizeof(char));
    fread(buffer, 1, len, fp);
    buffer[len + 1] = '\0';
    fclose(fp);
    return buffer;
}

int second_word(const char *pre, const char *array) {
    int i = 15;
    int count = 0;
    while (pre[count] != '\0')
        if (pre[count++] != array[i++])
            return 0;
    return 1;
}

char *get_the_word(char **doc, const char *firststr, const char *secondstr) {
    unsigned int lenfirst = (unsigned int) strlen(firststr);
    const char *buf = strstr((*doc), firststr);

    if (buf == NULL)
        return NULL;

    const char *buf2 = strstr(buf, secondstr);
    if (buf2 == NULL)
        return NULL;

    unsigned int len = (unsigned int) (buf2 - buf);
    char *newstr1 = (char *) buf;
    char *copy_of_str = (char *) calloc(len - lenfirst + 1, sizeof(char));
    strncpy_s(copy_of_str, len - lenfirst + 1, newstr1 + lenfirst, len - lenfirst);
    copy_of_str[len - lenfirst] = '\0';

    *doc = newstr1 + len;

    return copy_of_str;
}

int comp_name(const monitor *a, const monitor *b) {
    return strcmp(a->name, b->name);
}

int comp_price(const monitor *a, const monitor *b) {

    return (a->price < b->price) - (a->price > b->price);
}

unsigned int comp_producer(const monitor *a, const monitor *b) {
    return (a->producer) - b->producer;
}

int comp_producer_matrix(const monitor *a, const monitor *b) {

    if (b->producer < a->producer) {
        return 1;
    } else if (a->producer < b->producer) {
        return -1;
    } else {
        return strcmp(a->matrix, b->matrix);
    }
}

int comp_name_price(const monitor *a, const monitor *b) {
    if ((a->price > b->price) - (a->price < b->price) != 0) {
        return (a->price > b->price) - (a->price < b->price);
    } else {
        return strcmp(a->name, b->name);
    }
}