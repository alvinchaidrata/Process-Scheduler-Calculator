#include <stdio.h>
#include <stdlib.h>

//process stacks
struct process{
  int id;
  int arrivalTime;
  int burstTime;
  struct process *next;
};
struct queue{
  int id;
  int arrivalTime;
  int timeLeft;
  int burstTime;
  struct queue *next;
};
struct done{
  int id;
  int waitTime;
  int burstTime;
  int turnTime;
  struct done *next;
};

//funtions
void fcfsAlg(int processNum);
void sjfAlg(int processNum);
void rrAlg(int processNum,int timeQuantum,int Overhead);
void processToQue();
void queueToDone();
void refresh();
float averageWT();
float averageTT();
void print();

//global variable declaration
struct process *head,*tail,*ptr;
struct queue *head2=NULL,*tail2=NULL,*ptr2,*pos2;
struct done *head3=NULL,*tail3=NULL,*ptr3;
int clock=0;

//main function
int main(void) {
  //variable declaration
  int processNum, timeQuantum, overhead, minPosition;
  float average[4],min;
  //main menu
  printf("Please input the number of process(es) : ");
  scanf("%d", &processNum);
  printf("-------------------------------------------------- \n");
  //filling stack
  for(int i=1; i<=processNum ; i++){
    ptr=(struct process*)malloc(sizeof(struct process));
    printf("Please input the ARRIVAL time of process number %d : ", i);
    scanf("%d", &ptr->arrivalTime);
    printf("Please input the BURST   time of process number %d : ", i);
    scanf("%d", &ptr->burstTime);
    printf("\n");
    ptr->id=i;
    ptr->next=NULL;
    if(head==NULL){
      head=ptr;
      tail=ptr;
    }
    else{
      tail->next=ptr;
      tail=ptr;
    }
  }
  //scanning TimeQuantum and Overhead
  printf("-------------------------------------------------- \n");
  printf("Please enter TimeQuantum : ");
  scanf("%d", &timeQuantum);
  printf("Please enter Overhead    : ");
  scanf("%d", &overhead);
  printf("-------------------------------------------------- \n");
  //main program
  fcfsAlg(processNum);
  printf("First Come First Serve Algorithm :\n");
  print();
  printf("averageWT(%.2f),averageTT(%.2f)\n\n",averageWT(),averageTT());
  average[0]=(averageWT()+averageTT())/2;
  refresh();
  sjfAlg(processNum);
  printf("Shortest Job First Algorithm :\n");
  print();
  printf("averageWT(%.2f),averageTT(%.2f)\n\n",averageWT(),averageTT());
  average[1]=(averageWT()+averageTT())/2;
  refresh();
  rrAlg(processNum,timeQuantum,0);
  printf("Round Robin Algorithm :\n");
  print();
  printf("averageWT(%.2f),averageTT(%.2f)\n\n",averageWT(),averageTT());
  average[2]=(averageWT()+averageTT())/2;
  refresh();
  rrAlg(processNum,timeQuantum,overhead);
  printf("Round Robin with Overhead Algorithm :\n"); 
  print();
  printf("averageWT(%.2f),averageTT(%.2f)\n\n",averageWT(),averageTT());
  average[3]=(averageWT()+averageTT())/3;
  printf("-------------------------------------------------- \n");
  //comparing which algorithm is the best suited
  min=average[0];
  minPosition=0;
  for(int i=1;i<=3;i++){
    if(average[i]<min){
      min=average[i];
      minPosition=i;
    }
  }
  switch(minPosition){
    case 0:
      printf("The most efficient algorithm for the set of processes is First Come First Serve");
      break;
    case 1:
      printf("The most efficient algorithm for the set of processes is Shortest Job First");
      break;
    case 2:
      printf("The most efficient algorithm for the set of processes is Round Robin");
      break;
    case 3:
      printf("The most efficient algorithm for the set of processes is Round Robin with Overhead");
      break;
    default:
      break;
  }
}

//First Come First Serve Algorithm
void fcfsAlg(int processNum){
  int n=1;

  //runs while there are still process(es)
  while(n<=processNum){
    //moving arriving process(es) to queue
    processToQue();
    //processing queue
    if(head2!=NULL){
      //moving finished process to done
      if(head2->timeLeft==0){
        queueToDone();
        n++;
      }
      //processing process
      if(head2!=NULL){
        head2->timeLeft--;
      }
    }
    clock++;
  }
}

//Shortest Job First Algorithm
void sjfAlg(int processNum){
  int n=1,flag=0,dec=0,min,minPosition,que;

  while(n<=processNum){
    //moving arriving process(es) to queue
    processToQue();
    //processing queue
    if(head2!=NULL){
      //checking cpu status
      if(!flag){
        //moving the shortest job into the frontmost queue
        ptr2=head2;
        que=1;
        min=ptr2->timeLeft;
        minPosition=que;
        while(ptr2!=NULL){
          if(ptr2->timeLeft<min){
            min=ptr2->timeLeft;
            minPosition=que;
          }
          ptr2=ptr2->next;
          que++;
        }
        if(minPosition!=1){
          ptr2=head2;
          for(int i=1;i<minPosition;i++){
            ptr2=ptr2->next;
          }
          pos2=head2;
          for(int i=1;i<minPosition-1;i++){
            pos2=pos2->next;
          }
          pos2->next=ptr2->next;
          ptr2->next=head2;
          head2=ptr2;
          if(pos2->next==NULL){
            tail2=pos2;
          }
        }
        flag=1;
        if(dec==1){
          head2->timeLeft--;
          dec=0;
        }
      }
      //moving finished process to done
      if(head2->timeLeft==0){
        queueToDone();
        n++;
        flag=0;
        dec=1;
      }
      //processing process
      else{
        head2->timeLeft--;
      }
    }
    clock++;
  }
}

//Round Robin Algorithm
void rrAlg(int processNum,int timeQuantum,int overhead){
  int n=1,tCounter=timeQuantum,oCounter=0,que=0,flag=0;

  while(n<=processNum){
    //moving arriving process(es) to queue
    processToQue();
    //checking overhead
    if(oCounter==0){
      //processing queue
      if(head2!=NULL){
        //moving finished process to done
        if(flag){
          queueToDone();
          n++;
          flag=0;
          tCounter=timeQuantum;
          oCounter=overhead;
        }
        //processing process
        if(head2!=NULL){
          if(tCounter!=0){
            head2->timeLeft--;
            if(head2->timeLeft==0){
              flag=1;
            }
            tCounter--;
          }
          //when the TimeQuantum of a process is up, move it to the backmost queue
          else{
            ptr2=head2;
            que=0;
            while(ptr2!=NULL){
              ptr2=ptr2->next;
              que++;
            }
            if(que!=1){
              ptr2=head2;
              head2=head2->next;
              tail2->next=ptr2;
              tail2=ptr2;
              tail2->next=NULL;
            }
            head2->timeLeft--;
            if(head2->timeLeft==0){
              flag=1;
            }
            oCounter=overhead;
            tCounter=timeQuantum-1;
          }
        }
      }
    }
    else{
      oCounter--;
    }
    clock++;
  }
}

//moving arriving process(es) to queue
void processToQue(){
  ptr=head;
  while(ptr!=NULL){
    if(ptr->arrivalTime==clock){
      ptr2=(struct queue*)malloc(sizeof(struct queue));
      ptr2->id=ptr->id;
      ptr2->arrivalTime=ptr->arrivalTime;
      ptr2->timeLeft=ptr->burstTime;
      ptr2->burstTime=ptr->burstTime;
      ptr2->next=NULL;
      if(head2==NULL){
        head2=ptr2;
        tail2=ptr2;
      }
      else{
        tail2->next=ptr2;
        tail2=ptr2;
      }
    }
    ptr=ptr->next;
  }
}

//moving finished process to done
void queueToDone(){
  ptr3=(struct done*)malloc(sizeof(struct done));
  ptr3->id=head2->id;
  ptr3->waitTime= clock - (head2->arrivalTime + head2->burstTime);
  ptr3->burstTime=head2->burstTime;
  ptr3->turnTime= clock - head2->arrivalTime;
  ptr3->next=NULL;
  if(head3==NULL){
    head3=ptr3;
    tail3=ptr3;
  }
  else{
    tail3->next=ptr3;
    tail3=ptr3;
  }
  ptr2=head2;
  head2=head2->next;
  free(ptr2);
}

//resetting clock and emptying done
void refresh(){
  //resetting parameters
  clock=0;
  while(head3!=NULL){
    ptr3=head3;
    head3=ptr3->next;
    free(ptr3);
  }
}

//Calculating Average Wait Time
float averageWT(){
  float total=0,n=0;

  ptr3=head3;
  while(ptr3!=NULL){
    total+=ptr3->waitTime;
    n++;
    ptr3=ptr3->next;
  }
  return total/n;
}

//Calculating Average Turnaround Time
float averageTT(){
  float total=0,n=0;

  ptr3=head3;
  while(ptr3!=NULL){
    total+=ptr3->turnTime;
    n++;
    ptr3=ptr3->next;
  }
  return total/n;
}

//printing done
void print(){
  ptr3=head3;
  while(ptr3!=NULL){
    printf("id(%d) WaitTime(%d) TurnTime(%d)\n",ptr3->id,ptr3->waitTime,ptr3->turnTime);
    ptr3=ptr3->next;
  }
}