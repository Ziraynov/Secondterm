
#include "Main.h"
int main(){
const char path[]={"C:/Users/ziray/CLionProjects/Secondterm/src/Lab2/book.txt"};
LIFO* head=NULL;
stack(&path,&head);
    print_file_size(path);
    print_file_size("C:/Users/ziray/CLionProjects/Secondterm/src/Lab2/newfile.txt");
return 0;
}