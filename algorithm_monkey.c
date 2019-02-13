#include <stdio.h>
/* Having realized the my algorithm was poor
this was my wire frame of the improved way
before feeding into thread monkey
*/

int main () {
    int nuts;
    int a;
    int b;
    int c;
    int count;
    nuts = 1001;
    int loop2;
count = 0;
for( a = 1; a < nuts; a = a + 2 ){
    loop2 = (nuts - a) + 1;
    for( b = 1; b < loop2; b = b + 2 ){
        c = nuts - a - b;
        if (c > 0 && a!=b && a!=c && b!=c){
        count++;
        }
    }
}
printf("%d\n", count);

        
 
return 0;
}