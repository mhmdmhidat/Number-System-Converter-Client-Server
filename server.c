/*
    Written by: mhmd mhidat
*/
#include "header.h"   //include all headers which program needs
void do_convert(int); 
void split(char *);  //split message format
void d_to_h (char []); //Decimal to Hex
void d_to_b (char []); //Deciaml to Binary
void h_to_d (char []); //hex to decimal
void b_to_d (char []); //binary to decimal
void b_to_h (char []); //binary to hex
void h_to_b (char []); //hex to binary
char *strrev(char *); //reverse string, I used it in conversion below
int pwo (int,int); //calculate the power
void sig_chld(int);
struct number
{
   char from_base;
   char from_number[40];
   char to_base;
   char to_number[40];
};
struct number tst;  
int main(int argc , char ** argv)
{
    if(argc!=2)
    {
      printf("usage: server <port_address>\n");
      exit(0);
    }
    char por[7] = "44008";
    if (strcmp(por, argv[1]))
    {
        printf("You must use 44008 as a Port number\n");
        exit(0);
    }
    signal(SIGCHLD, sig_chld);
    int listenfd, connfd;
    pid_t childpid;;
    socklen_t clilin;
    struct sockaddr_in cliaddr, servaddr;
    listenfd = Socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port=htons(atoi(argv[1]));
    Bind(listenfd,  (struct sockaddr *) &servaddr, sizeof(servaddr));
    Listen(listenfd, LISTENQ);
    for ( ; ; ) {
        clilin = sizeof(cliaddr);
        if((connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilin))<0);
        if(errno==EINTR) //my program interrupted while waiting in accept
            continue;
        char port[8];
        sprintf(port,"%d",ntohs(cliaddr.sin_port));
        printf("Welcome to you in our server\n");
        printf("your IP is %s\n",inet_ntoa(cliaddr.sin_addr));
        printf("your port is %s\n",port);
        if ( (childpid = Fork()) == 0) /* child process */
        { 
            Close(listenfd); /* close listening socket */
            do_convert(connfd); /* process the request */
            exit(0);
        }
        Close(connfd); //close connected socket in parent proccess
    }
}

void do_convert(int connfd)
{
    while(true)
    {   char data[MAXLINE];
        int n;
        memset(data,0,strlen(data));
        if ( (n = Read(connfd, data, MAXLINE)) == 0)///* connection closed by other end */
        return; 
        memset(tst.from_number,0,strlen(tst.from_number)); //set tst.from_number to zero
        memset(tst.to_number,0,strlen(tst.to_number));     //set tst.to_number to zero
        split(data); //split the request and extract the fields to do converstion
        if(tst.from_base=='B' && tst.to_base=='D')
        {
            b_to_d(tst.from_number);
        }
        else if(tst.from_base=='D' && tst.to_base=='B')
        {
            d_to_b(tst.from_number);
        }
        else if(tst.from_base=='H' && tst.to_base=='D')
        {
            h_to_d(tst.from_number);
        }
        else if(tst.from_base=='D' && tst.to_base=='H')
        {
            d_to_h(tst.from_number);
        }
        else if(tst.from_base=='B' && tst.to_base=='H')
        {
            b_to_h(tst.from_number);
        }
        else if(tst.from_base=='H' && tst.to_base=='B')
        {
            h_to_b(tst.from_number);
        }
        else 
        {
          char un[30]="Unsupported conversion!";
          Writen(connfd,un, strlen(un));
          continue;
        }

        Writen(connfd,tst.to_number, strlen(tst.to_number));
    }
}
void split(char data[])
{
 int c=0;
 for(int i=0 ; i<strlen(data); i++)
 {  
     if(data[i]==':')
     {
         c++;
         continue;
     }
     if(c==0)
     tst.from_number[i]=data[i];
     else if(c==1)
     tst.from_base=data[i];
     else
     tst.to_base=data[i];
 }
}
void d_to_h (char x[]){
    int decnum, rem, i=0;
    decnum=atoi(x);
    while(decnum!=0)
    {
        rem = decnum%16;
        if(rem<10)
            rem = rem+48;
        else
            rem = rem+55;
        tst.to_number[i] = rem;
        i++;
        decnum = decnum/16;
    }
    strrev(tst.to_number); //reverse the string strrev(123)=321
}
void d_to_b (char x[]){
   int n,i;    
   n=atoi(x);
   for(i=0;n>0;i++){    
      tst.to_number[i]=(n%2)+48;    
      n=n/2;    
   }
   strrev(tst.to_number);   
}
void h_to_d (char x[])
{
     int p = 0; 
    int decimal = 0;
    int r, i;
    for(i = strlen(x) - 1 ; i >= 0 ; --i){
        
        if(x[i]>='0'&&x[i]<='9'){
            r = x[i] - '0';
        }
        else{
            r = x[i] - 'A' + 10;
        }
        
        decimal = decimal + r * pwo(16 , p);
        ++p;
    }
    sprintf(tst.to_number,"%d",decimal);
}
void b_to_d (char x[])
{ 
  int num, decimal_num = 0, base = 1, rem;  
  num=atoi(x); //convert string to integer
   while ( num > 0)  
    {  
        rem = num % 10; 
        decimal_num = decimal_num + rem * base;  
        num = num / 10; 
        base = base * 2;  
    }  
   sprintf(tst.to_number, "%d", decimal_num); //convert integer to string
}
char *strrev(char *str)
{
      char *p1, *p2;

      if (! str || ! *str)
            return str;
      for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
      {
            *p1 ^= *p2;
            *p2 ^= *p1;
            *p1 ^= *p2;
      }
      return str;
}
int pwo (int base,int exponent){
int result = 1;
for (exponent; exponent>0; exponent--)
result = result * base;
return result;
}
void h_to_b(char hex[])  
{  
   int i=0;   
   while(hex[i])  
   {  
       switch(hex[i])  
       {  
           case '0':  
           strcat(tst.to_number,"0000");  
           break;  
           case '1':  
           strcat(tst.to_number,"0001");  
           break;  
           case '2':  
           strcat(tst.to_number,"0010");  
           break;  
           case '3':  
           strcat(tst.to_number,"0011");  
           break;  
           case '4':  
           strcat(tst.to_number,"0100");  
           break;  
           case '5':  
           strcat(tst.to_number,"0101");  
           break;  
           case '6':  
           strcat(tst.to_number,"0110");  
           break;  
           case '7':  
           strcat(tst.to_number,"0111");  
           break;  
           case '8':  
           strcat(tst.to_number,"1000");  
           break;  
           case '9':  
           strcat(tst.to_number,"1000");  
           break;  
           case 'A':  
           strcat(tst.to_number,"1010");  
           break;  
           case 'B':  
           strcat(tst.to_number,"1011");  
           break;    
           case 'C':  
           strcat(tst.to_number,"1100");  
           break;  
           case 'D':  
           strcat(tst.to_number,"1101");  
           break;  
           case 'E':  
           strcat(tst.to_number,"1110");  
           break;  
           case 'F':  
           strcat(tst.to_number,"1111");  
           break;  
       }  
       i++;  
   }
}
void b_to_h (char x[])
{
    long int binaryval, hexadecimalval = 0, i = 1, remainder;
    binaryval=atoi(x);
    while (binaryval != 0)
    {
        remainder = binaryval % 10;
        hexadecimalval = hexadecimalval + remainder * i;
        i = i * 2;
        binaryval = binaryval / 10;
    }
    sprintf(tst.to_number, "%lX", hexadecimalval);
    strrev(tst.to_number); 
}  
void sig_chld(int signo)
{ pid_t pid;
 int stat;
 while ( (pid = waitpid(-1, &stat, WNOHANG)) > 0)
 printf("child %d terminated\n", pid);
 return;
}
