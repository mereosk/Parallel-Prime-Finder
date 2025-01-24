#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include "ADTBList.h"
#include "func.h"

#define  READ 0
#define  WRITE 1

struct Range;

int numUSR1rec = 0;

void handle_sigusr1(int signum) {
	signal(SIGUSR1, handle_sigusr1);
	numUSR1rec++;
}

// For debugging purposes
void printArray(double *array, int length) {
    printf("\n");
    for (int i = 0; i < length; i++) {
        printf("Time for W%d: %.2lf msecs\n", i,1000*array[i]);
    }
}

double findMax(double *array, int length) {
    double max = array[0];
    for(int i = 1; i < length; i++) 
        if(array[i] > max)
            max = array[i];
    
    return max;
     
}

double findMin(double *array, int length) {
    double min = array[0];
    for(int i = 1; i < length; i++) 
        if(array[i] < min)
            min = array[i];
    
    return min;
}

int main(int argc, char const *argv[]) {
    int lowerBound, upperBound, numOfChildren;
    char *lChar = "-l";
    char *uChar = "-u";
    char *chChar = "-w";

    if(argc == 7) {
        if(strcmp(argv[1], lChar) == 0 && strcmp(argv[3], uChar) == 0 && strcmp(argv[5], chChar) == 0) {
            lowerBound = atoi(argv[2]);
            upperBound = atoi(argv[4]);
            numOfChildren = atoi(argv[6]);
        }
        else if(strcmp(argv[1], uChar) == 0 && strcmp(argv[3], lChar) == 0 && strcmp(argv[5], chChar) == 0) {
            lowerBound = atoi(argv[4]);
            upperBound = atoi(argv[2]);
            numOfChildren = atoi(argv[6]);
        }
        else if(strcmp(argv[1], uChar) == 0 && strcmp(argv[3], chChar) == 0 && strcmp(argv[5], lChar) == 0) {
            lowerBound = atoi(argv[6]);
            upperBound = atoi(argv[2]);
            numOfChildren = atoi(argv[4]);
        }
        else if(strcmp(argv[1], lChar) == 0 && strcmp(argv[3], chChar) == 0 && strcmp(argv[5], uChar) == 0) {
            lowerBound = atoi(argv[2]);
            upperBound = atoi(argv[6]);
            numOfChildren = atoi(argv[4]);
        }
        else if(strcmp(argv[1], chChar) == 0 && strcmp(argv[3], uChar) == 0 && strcmp(argv[5], lChar) == 0) {
            lowerBound = atoi(argv[6]);
            upperBound = atoi(argv[4]);
            numOfChildren = atoi(argv[2]);
        }
        else if(strcmp(argv[1], chChar) == 0 && strcmp(argv[3], lChar) == 0 && strcmp(argv[5], uChar) == 0) {
            lowerBound = atoi(argv[4]);
            upperBound = atoi(argv[6]);
            numOfChildren = atoi(argv[2]);
        }
        else {
            printf("Wrong input\n");
            exit(0);
        }
    }
    else {
        printf("usage: %s <-l> <lowerBound> <-u> <upperBound> <-w> <numOfChildren>\n", argv[0]);
        exit(0);
    }

    if (( lowerBound < 1 ) || ( lowerBound > upperBound || (numOfChildren < 1))) {
        printf("conditions: lowerBound>=1, lowerBound < upperBound, number of Children >= 1\n");
        exit (1); 
    }

    // Handle the signal SIGUSR1
	signal(SIGUSR1, handle_sigusr1);

    int numOfWorkers = numOfChildren*numOfChildren;
    double workers[numOfWorkers];

    // Initiallization of list
    List list = list_create(free);

    Range ranges[numOfChildren];

    generate_ranges(lowerBound, upperBound, numOfChildren, ranges);

    // Initialization of pipes
    int fd[numOfChildren][2];
    for(int i=0 ; i<numOfChildren ; i++)
        if(pipe(fd[i]) < 0) {
            perror("Error in pipe\n");
            exit(2);
        }

    pid_t childpid;
    int counter = 0;
    for(int i=0 ; i<numOfChildren ; i++) // loop will run numOfChildren times 
    {   

        childpid = fork();
        if (childpid== -1) {
            perror("Failed to fork\n");
            exit(1);
        }

        if(childpid == 0) { 

            // Close all fd's I will not use
            int saveFD;
            for(int i=0 ; i<numOfChildren ; i++) {
                if(i==counter) {
                    close(fd[i][READ]);
                    saveFD = fd[i][WRITE];
                    continue;
                }
                close(fd[i][READ]);close(fd[i][WRITE]);
            }

            // Converting integers to strings
            char str1[10], str2[10], str3[10], str4[10], str5[10];
            sprintf(str1, "%d", ranges[counter].lo);
            sprintf(str2, "%d", ranges[counter].hi);
            sprintf(str3, "%d", numOfChildren);

            // Pass in prime file the correct file descriptor
            sprintf(str4, "%d", saveFD);

            // Pass the number of the child, so that I can dinstinguish
            // them, this will help with the Wi times at the end
            sprintf(str5, "%d", counter+1);

            char* args[] = { "inter", str1 , str2, str3, str4, str5, NULL };
            int i = execv("./inter", args);
            printf(" i = %d\n", i);
            exit(0); 
        }

        counter++;
    }
    for(int i=0;i<numOfChildren;i++) // loop will run numOfChildren times
            wait(NULL);

    if(childpid>0) {
        // Close all write fd's cause I don't need them
        for(int i=0 ; i<numOfChildren ; i++)
            close(fd[i][WRITE]);
        // Read from the pipes of each child
        for(int i=0 ; i<numOfChildren; i++) {
            Details temp;
            while(read(fd[i][READ], &temp, sizeof(temp)) != 0) {
                list_ordered_insert(list, create_details(temp), compare_details);
            }
            close(fd[i][READ]);
        }
        wait(NULL);
    }  
    printf("\n");

    printf("Primes in [%d,%d] are: ",lowerBound, upperBound);
    for(ListNode lnode = list_first(list);
    lnode != LIST_EOF;
    lnode = list_next(list, lnode)){
        Details *det = list_node_value(list, lnode);
        if(det->prime > 0)
            printf("{%d,%.2lf} ",det->prime, 1000*det->time);
        // If temp is not 0 the value is not a prime but the time
        // for the worker to finish
        else {
            workers[(-(det->prime))-1] = det->time;
        }
    }
    printf("\n\n");

    printf("Num of USR1 Received : %d/%d", numUSR1rec, numOfWorkers);
    printArray(workers, numOfWorkers);

    double min, max;
    max = findMax(workers, numOfWorkers);
    min = findMin(workers, numOfWorkers);
    printf("Min Time for Workers : %.2lf msecs\n", 1000*min);
    printf("Max Time for Workers : %.2lf msecs\n", 1000*max);

    list_destroy(list);
    return 0;
}
