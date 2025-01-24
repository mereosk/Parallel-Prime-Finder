#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include "func.h"
#include "ADTBList.h"

#define  READ 0
#define  WRITE 1


int main(int argc, char const *argv[]) {
    int lb, ub, numOfChildren, fileDes, numOfInter;

    if(argc != 6) {
        printf ("usage: %s <lowerBound> <upperBound> <numOfChildren> <file Descriptor> <numOfInter>\n", argv[0]);
        exit(1); 
    }
    lb= atoi(argv[1]);
    ub= atoi(argv[2]);
    numOfChildren = atoi(argv[3]);
    fileDes = atoi(argv[4]);
    numOfInter = atoi(argv[5]);
    if (( lb < 1 ) || ( lb > ub ) || (numOfChildren < 1)) {
        printf("conditions: lowerBound>=1, lowerBound < upperBound, number of Children >= 1\n");
        exit(1);
    }

    //Initialization of list
    List list = list_create(free);

    Range ranges[numOfChildren];

    generate_ranges(lb, ub, numOfChildren, ranges);

    // Initialization of pipes
    int fd[numOfChildren][2];
    for(int i=0 ; i<numOfChildren ; i++)
        if(pipe(fd[i]) < 0) {
            perror("Error in pipe\n");
            exit(2);
        }

    pid_t root_id = getppid();
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
            char str1[10], str2[10], str3[10], str4[10];
            sprintf(str1, "%d", ranges[counter].lo);
            sprintf(str2, "%d", ranges[counter].hi);

            char fileName[10];
            char filePath[256];
            char ch[3];
            
            // Creating the correct file name
            strcpy(fileName, "prime");
            sprintf(ch, "%d", counter%3+1);      // We have 3 algortithms so number should always be [1,2,3]
            strcat(fileName, ch);

            // Creating the correct file path
            strcpy(filePath, "./");
            strcat(filePath, fileName);

            sprintf(str1, "%d", ranges[counter].lo);
            sprintf(str2, "%d", ranges[counter].hi);

            // Pass in prime file the correct file descriptor
            sprintf(str3, "%d", saveFD);

            // Root Id so that workers can send it signals
            sprintf(str4, "%d", root_id);

            char* args[] = { fileName, str1 , str2, str3, str4, NULL };
            execv(filePath, args);
            exit(0); 
        }

        counter++;
    }
    for(int i=0;i<numOfChildren;i++) // loop will run nnumOfChildren times
            wait(NULL);

    if(childpid>0) {
        // Close all write fd's cause I don't need them
        for(int i=0 ; i<numOfChildren ; i++)
            close(fd[i][WRITE]);
        // Read from the pipes of each child
        for(int i=0 ; i<numOfChildren; i++) {
            Details temp;
            while(read(fd[i][READ], &temp, sizeof(temp)) != 0) {
                if(temp.prime == 0) {
                    int numberOfW = (numOfInter-1)*numOfChildren + (i+1);
                    // Mark the struct's integer (prime) as negative so that
                    // the root will undrestand its the time of the whole
                    // child's process to finish
                    temp.prime = -numberOfW;
                }
                list_ordered_insert(list, create_details(temp), compare_details);
            }
            close(fd[i][READ]);
        }
        wait(NULL);
    }

    // Now write to myprime
    for(ListNode lnode = list_first(list);
    lnode != LIST_EOF;
    lnode = list_next(list, lnode)){
        Details det = *(Details*)list_node_value(list, lnode);
        write(fileDes, &det, sizeof(det));
    }

    close(fileDes);
    
    list_destroy(list);
    return 0;
}
