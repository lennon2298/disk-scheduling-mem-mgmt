#include <stdio.h>
#include <stdlib.h>

#define LOW 0

#define HIGH 199

int main(){
  int queue[20];
  int head, max, q_size, temp, sum;
  int dloc; //location of disk head arr
    printf("%s\t", "Input no of queue elements");
    scanf("%d", &q_size);
    printf("%s\t", "Enter head position");
    scanf("%d", &head);
    printf("%s\n", "Input elements into disk queue");

  for(int i=0; i<q_size; i++)
    {
    scanf("%d", &queue[i]);
    }

    queue[q_size] = head; //add head into queue
    q_size++;
  //sort the array

  for(int i=0; i<q_size;i++)
    {
    for(int j=i; j<q_size; j++)
        {
        if(queue[i]>queue[j])
        {
        temp = queue[i];
        queue[i] = queue[j];
        queue[j] = temp;

      }

    }

  }

  max = queue[q_size-1];
  //locate head in the queue
  for(int i=0; i<q_size; i++){
    if(head == queue[i]){
     dloc = i;
     break;
    }

  }


 if(abs(head-LOW) <= abs(head-HIGH)){  /*Dloc tracks head position and is updated accordingly*/
      for(int j=dloc; j>=0; j--){
        printf("%d --> ",queue[j]);
      }
      for(int j=dloc+1; j<q_size; j++){
        printf("%d --> ",queue[j]);
      }
      } else
        {
        for(int j=dloc+1; j<q_size; j++)
            {
            printf("%d --> ",queue[j]);
            }
        for(int j=dloc; j>=0; j--)
        {
          printf("%d --> ",queue[j]);
        }

 }
  sum  = head + max;
  printf("\n Total movements  %d", sum);
  return 0;

}
