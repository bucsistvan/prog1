#include <iostream>

using namespace std;

int main(){
    int a=8;
    int b=0;
    while(a){
        a=a<<1;
        b++;
    }
    cout<<b;
    return 0;
}
