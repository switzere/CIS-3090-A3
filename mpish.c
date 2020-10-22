#include <stdio.h>
#include <mpi.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#include <wordexp.h>

const int MAX_STRING = 100;


int handleCommand(int flag, char* fileName, char* message, int pNum);



int main(void) {
  char command[100];
  int my_rank, comm_sz, flag;
  int thread = 1;
  wordexp_t p;
  char **w;

  MPI_Status status;

  MPI_Init(NULL, NULL);

  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);




  if (my_rank == 0) {
    int exitFlag = 1;

    while( exitFlag == 1 ) {

      printf("Thread: %d\n",thread);
      fgets(command, 100, stdin);
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
        printf("exit cmp: %d - %s\n",strcmp(w[0], "exit"), w[0]);
        if(strcmp(w[0], "put") == 0) {
          printf("put\n");
          if(p.we_wordc == 2) {

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
          printf("cmd: %s\n",w[0]);
          printf("Exiting...\n");
          if(p.we_wordc == 1) {
            for(int i = 1; i < comm_sz; i++) {
              MPI_Send(w[0], strlen(w[0]) + 1, MPI_CHAR, i, 5, MPI_COMM_WORLD);
            }
          exitFlag = 0;
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

      wordfree(&p);

    }
  } else {
    int resp = 1;
    while(resp == 1) {
      char path[100];
      sprintf(path, "p%d", my_rank);
      mkdir(path, 0777);
      printf("Made directory: %s\n",path);

      printf("Look here: \n");
      MPI_Recv(command, MAX_STRING, MPI_CHAR, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      printf("here: %s from %d with tag: %d\n",command, my_rank, status.MPI_TAG);

      resp = handleCommand(status.MPI_TAG, command, "This is stuff in a file", my_rank);
      if(resp == 0){
        printf("Exiting %d\n", my_rank);
      }
      else {
        printf("Not exiting resp = %d\n",resp);
      }
    }

  }




  MPI_Finalize();

  return 0;
}

int handleCommand(int flag, char* fileName, char* message, int pNum) {
  //put <filename>
  if(flag == 1) {
    printf("put <filename>\n");
    char tempFN[100];
    char tempNum[100];
    FILE *fp;

    sprintf(tempNum, "%d", pNum);

    strcpy(tempFN, "p");
    strcat(tempFN, tempNum);
    strcat(tempFN, "/");
    strcat(tempFN, fileName);





    printf("FN: %s\n",tempFN);

    fp = fopen(tempFN, "w");
    fputs(message, fp);
    fclose(fp);
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
    return 0;
  }
  //lls
  else if(flag == 6) {
    system("ls");
  }
  //lrm <filename>
  else if(flag == 7) {
    char tempCommand[100];

    strcpy(tempCommand, "rm ");
    strcat(tempCommand, fileName);

    system(tempCommand);
  }
  else {

  }
}
