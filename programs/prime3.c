#include <stdio.h> /* printf () */
#include <sys/times.h> /* times () */
#include <unistd.h> /* sysconf () */
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <signal.h>
#include "func.h"

#define YES 1
#define NO 0

int prime (int n){
    int i=0;

    if(n==2 || n==3)  return(YES);
    if(n%2==0 || n%3==0 || n==1) return(NO);
    for(i=5 ; i*i <= n ; i+=6)
        if(n%i == 0 || n%(i+2) == 0) return(NO);

    return(YES);
}

int main (int argc, char const *argv[]) {
    double t1 , t2 , t3, t4;
    struct tms tb1 , tb2 , tb3, tb4;
    double ticspersec ;
    ticspersec = ( double ) sysconf ( _SC_CLK_TCK );
    t1 = ( double ) times (& tb1) ;

    int lb=0 , ub=0 , i=0, fileDes, rootId;
    if ( (argc != 5) ) {
        printf ("usage: %s <lowerBound> <upperBound> <file Descriptor> <root Id>\n", argv[0]);
        exit(1); 
    }
    lb= atoi(argv[1]);
    ub= atoi(argv[2]);
    fileDes = atoi(argv[3]);
    rootId = atoi(argv[4]);
    if ( ( lb <1 ) || ( lb > ub ) ) {
        printf("conditions: lowerBound>=1, lowerBound < upperBound\n");
        exit(1);
    }

    Details dets;
    for (i=lb ; i <= ub ; i ++) {
        t3 = ( double ) times (& tb3);
        int result = prime(i);
        t4 = ( double ) times (& tb4);
        if ( result == YES ) {
            dets.prime = i;
            dets.time = (t4 - t3) / ticspersec;
            write(fileDes, &dets, sizeof(dets));
        }
    }
        
    t2 = ( double ) times (& tb2) ;
    dets.prime = 0;
    dets.time = (t2 - t1) / ticspersec;
    write(fileDes, &dets, sizeof(dets));

    close(fileDes);


    kill(rootId, SIGUSR1);
    return 0;
}