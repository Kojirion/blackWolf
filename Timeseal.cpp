#include "Timeseal.hpp"
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

//int crypt(std::string &s)
//{
//    static const char *key="Timestamp (FICS) v1.0 - programmed by Henrik Gram.";

//    int n;
//    struct timeval tv;
//    s[l++]='\x18';
//    gettimeofday(&tv,NULL);
//    l+=sprintf(&s[l],"%ld",(tv.tv_sec%10000)*1000+tv.tv_usec/1000);
//    s[l++]='\x19';
//    for(;l%12;l++)
//        s[l]='1';
//#define SC(A,B) s[B]^=s[A]^=s[B],s[A]^=s[B]
//    for(n=0;n<l;n+=12)
//        SC(n,n+11), SC(n+2,n+9), SC(n+4,n+7);
//    for(n=0;n<l;n++)
//        s[n]=((s[n]|0x80)^key[n%50])-32;
//    s[l++]='\x80';
//    s[l++]='\x0a';
//    return l;
//}

#define SC(A,B) s[B]^=s[A]^=s[B],s[A]^=s[B]

int crypt(std::string& s){

    static const char *key="Timestamp (FICS) v1.0 - programmed by Henrik Gram.";

    std::size_t n;
    s += "\x18";
    struct timeval tv;
    gettimeofday(&tv,NULL);
    auto toAdd = (tv.tv_sec%10000)*1000+tv.tv_usec/1000;
    s += std::to_string(toAdd);
    s += "x19";
    while(s.size()%12)
        s += '1';
    for(n=0;n<s.size();n+=12)
        SC(n,n+11), SC(n+2,n+9), SC(n+4,n+7);
    for(n=0;n<s.size();n++)
        s[n]=((s[n]|0x80)^key[n%50])-32;
    s += "\x80";
    s += "\x0a";
    return s.size();
}
