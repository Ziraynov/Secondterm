
#include "Mainlab5.h"

int main() {
    FILE *DNS;
    DNS = checkFile("r+");
    cacheEntryT *Head=NULL;
    cacheEntryT  *Tail=NULL;
    cacheT *cache = createCache();
    int x = menu(DNS, &cache,&Head,&Tail);
    while (x != 0) {
        x = menu(DNS, &cache,&Head,&Tail);
        DNS = checkFile("r+");
    }
    fclose(DNS);
    freCache(&cache,&Head,&Tail);
    return 0;
}