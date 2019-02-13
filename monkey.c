

#include <stdio.h>
 
int main () {
    int nuts;
    int a;
    int b;
    int c;
    int count;
    nuts = 5001;
count = 0;
for( a = 1; a < nuts; a = a + 2 ){
    for( b = 1; b < nuts; b = b + 2 ){
        for( c = 1; c < nuts; c = c + 2 ){
            if (a + b + c == nuts && a!=b && a!=c && b!=c){
                count++;
            }
        }
    }
}
printf("%d\n", count);
return 0;
}