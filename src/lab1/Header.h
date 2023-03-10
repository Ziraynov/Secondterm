#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <conio.h>

typedef enum {
	LG, Samsung,GigaByte,AOC, Huawei, Philips, BenQ, HP,Asus

}producer_of_monitor;
typedef struct {
	char* name;
	char* matrix;
	double price;
	producer_of_monitor producer;
}monitor;
void get_key(int* key);
void find_number(int* number, char* html_string);
monitor* parse_html(const char*  path, int* number);
void show(monitor* array, int len);
void sort(monitor* arr, int len);
void add_word(monitor* arr, int* len);
void rem(monitor* arr, int* len);







char* file(const char  *path);
int second_word(const char* pre,const char* string);
char* get_the_word(char** doc, char* firststr, char* secondstr);

int comp_name(monitor* a, monitor* b);

int comp_price(monitor* a, monitor* b);

int comp_producer(monitor* a, monitor* b);

int comp_producer_matrix(const monitor* a,const monitor* b);

int comp_name_price(const monitor* a, const monitor* b);

