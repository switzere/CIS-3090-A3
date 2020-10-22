#include <stdio.h>
#include <math.h>
#include <mpi.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <wordexp.h>

const int MAX_STRING = 1000;


int handleCommand(int flag, char* fileName, int pNum);



int main(void) {
  char command[1000];
  int my_rank, comm_sz, flag;
  int thread = 1;
  wordexp_t p;
  char **w;

  MPI_Status status;

  MPI_Init(NULL, NULL);

  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);




  if (my_rank == 0) {
    while( 0 == 0 ) {

      printf("Thread: %d\n",thread);
      fgets(command, 1000, stdin);
      command[strlen(command)-1] = '\0';
      printf("command: %s\n",&command);

      printf("hi: %s\n",&command);

      wordexp(command, &p, 0);
      w = p.we_wordv;
      for(int i = 0; i < p.we_wordc; i++) {
        printf("%d - %s\n", i, w[i]);
      }

      printf("p.we: %d\n", p.we_wordc);

      printf("before if\n");
      if(p.we_wordc > 0){
        printf("after if\n");

        printf("put cmp: %d\n",strcmp(w[0], "put"));
        if(strcmp(w[0], "put") == 0) {
          printf("put\n");
          if(p.we_wordc == 2) {
            char passFC[1001];
            char* fileContents;
            int fileSize = 0;
            int readSize = 0;
            FILE *fp;

            fp = fopen(w[1], "r");

            if(fp != NULL) {
              fseek(fp, 0, SEEK_END);
              fileSize = ftell(fp);
              rewind(fp);
              fileContents = (char*) malloc(sizeof(char)*(fileSize+1));
              readSize = fread(fileContents, sizeof(char), fileSize, fp);

              fileContents[fileSize] = '\0';

            }

            fclose(fp);

            printf("fileContents: %s\n",fileContents);

            MPI_Send(w[1], strlen(w[1])+1, MPI_CHAR, thread, 1, MPI_COMM_WORLD);

            int done = 1;

            //int lenOverThous = ceil((double)strlen(fileContents)/1000.0);

          /*  for(int i = 0; i < lenOverThous; i++) {
              int c = i * 1000;
              int l = (i + 1) * 1000;
              if(l < strlen(fileContents)) {
                l = strlen(fileContents);
                done = 0;
              }
              while(c < l) {
                passFC[c - i*1000] = fileContents[c];
                c++;
              }
              passFC[c - i*1000] = '\0';

              printf("passFC: %s\n",&passFC);

              MPI_Send(passFC, strlen(passFC)+1, MPI_CHAR, thread, done, MPI_COMM_WORLD);

            }*/
            printf("send 1 1\n");
            //MPI_Send(fileContents, strlen(fileContents)+1, MPI_CHAR, thread, 1, MPI_COMM_WORLD);
            printf("send 1 2\n");




            if(thread + 1 >= comm_sz) {
              thread = 1;
            }
            else {
              thread++;
            }

          }
          else if(p.we_wordc == 3) {

          }
          else {
            printf("Invalid number of parameters\n");
          }
        }
        else if(strcmp(w[0], "get") == 0) {
          if(p.we_wordc == 2) {

          }
          else {
            printf("Invalid number of parameters\n");
          }
        }
        else if(strcmp(w[0], "ls") == 0) {
          if(p.we_wordc == 1) {

          }
          else {
            printf("Invalid number of parameters\n");
          }
        }
        else if(strcmp(w[0], "exit") == 0) {
          if(p.we_wordc == 1) {

          }
          else {
            printf("Invalid number of parameters\n");
          }
        }
        else if(strcmp(w[0], "lls") == 0) {
          if(p.we_wordc == 1) {

          }
          else {
            printf("Invalid number of parameters\n");
          }
        }
        else if(strcmp(w[0], "lrm") == 0) {
          if(p.we_wordc == 2) {

          }
          else {
            printf("Invalid number of parameters\n");
          }
        }



      }



    }
  } else {
    char path[1000];
    sprintf(path, "p%d", my_rank);
    mkdir(path, 0777);
    printf("Made directory: %s\n",path);

    printf("Look here: \n");
    MPI_Recv(command, MAX_STRING, MPI_CHAR, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    printf("here: %s from %d with tag: %d\n",command, my_rank, status.MPI_TAG);

    handleCommand(status.MPI_TAG, command, my_rank);

  }



  wordfree(&p);

  MPI_Finalize();

  return 0;
}

int handleCommand(int flag, char* fileName, int pNum) {
  MPI_Status status;

  //put <filename>
  if(flag == 1) {
    printf("put <filename>\n");
    char tempFN[1000];
    char tempNum[1000];
    char tempFileContents[1000];
    FILE *fp;

    sprintf(tempNum, "%d", pNum);

    strcpy(tempFN, "p");
    strcat(tempFN, tempNum);
    strcat(tempFN, "/");
    strcat(tempFN, fileName);

    printf("FN: %s\n",tempFN);

    //fp = fopen(tempFN, "w");

    //MPI_Recv(tempFileContents, MAX_STRING, MPI_CHAR, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    //printf("tFC: %s\n",tempFileContents);
    //fputs(tempFileContents, fp);


    /*while( 0 == 0 ) {
      printf("before recv\n");
      MPI_Recv(tempFileContents, MAX_STRING, MPI_CHAR, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      printf("tFC: %s\n",tempFileContents);
      if(status.MPI_TAG == 1) {
        fputs(tempFileContents, fp);
      }
      else if(status.MPI_TAG == 0){
        break;
      }
    }*/

    //fclose(fp);
  }
  //put <filename> #
  else if(flag == 2) {

  }
  //get <filename>
  else if(flag == 3) {

  }
  //ls
  else if(flag == 4) {

  }
  //exit
  else if(flag == 5) {
    return 1;
  }
  //lls
  else if(flag == 6) {
    system("ls");
  }
  //lrm <filename>
  else if(flag == 7) {
    char tempCommand[1000];

    strcpy(tempCommand, "rm ");
    strcat(tempCommand, fileName);

    system(tempCommand);
  }
  else {

  }
}
