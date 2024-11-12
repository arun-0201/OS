#include<stdio.h> 
#include<sys/types.h> 
#include<unistd.h> 
int main() 
{ 
int p[2]; 
int pid; 
char inbuf[10],outbuf[10]; 
pipe(p); //To send message between parent and child // pid=fork(); // Fork call to create child process // 
if(pid) //// Code of Parent process 
{ 
printf("In parent process\n"); 
printf("type the data to be sent to child"); 
scanf("%s",outbuf); // Writing a message into the pipe write (p[1],outbuf, sizeof(outbuf)); //p[1] indicates write sleep(2); // To allow the child to run 
printf("after sleep in parent process\n"); 
} 
else // Coding of child process // 
{ 
sleep(2); 
printf("In child process\n"); 
read(p[0],inbuf,10); // Read the message written by parent printf("the data received by the child is %s\n",inbuf); } 
return 0; 
} 