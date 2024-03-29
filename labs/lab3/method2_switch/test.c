#include <stdio.h>
#include <stdlib.h>

int temp = 0, overflow = 0, mutex = 1, underflow = 10;
 
int wait_protocol(int counter)
{
      return (--counter);
}
 
int signal_protocol(int counter)
{
      return(++counter);
}
 
void producer()
{
      mutex = wait_protocol(mutex);
      overflow = signal_protocol(overflow);
      underflow = wait_protocol(underflow);
      temp++;
      printf("\nProducer:\t Element %d\n", temp);
      mutex = signal_protocol(mutex);
}
 
void consumer()
{
      mutex = wait_protocol(mutex);
      overflow = wait_protocol(overflow);
      underflow = signal_protocol(underflow);
      printf("\nConsumer:\t Element %d\n", temp);
      temp--;
      mutex = signal_protocol(mutex);
}
 
int main()
{
      int choice;
      printf("\n1.Producer\t");
      printf("2. Consumer\t");
      printf("3. Quit\t");
      while(1)
      {
            printf("\nEnter your choice:\t");
            scanf("%d", &choice);
            switch(choice)
            {
                  case 1: if((mutex == 1) && (underflow != 0))
                          {
                                producer();
                          }
                          else
                          {
                                printf("Buffer Overflow\n");
                          }
                          break;
                  case 2: if((mutex == 1) && (overflow != 0))
                          {
                                consumer();
                          }
                          else
                          {
                                printf("Buffer Underflow\n");
                          }
                          break;
                  case 3: exit(0);
                          break;
            }
      }
      return 0;
}
