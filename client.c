#include "header.h"
void split(char []);
void capitlize (char s[]);
struct number
{
   char from_base;
   char from_number[40];
   char to_base;
};
char * sys_name;
struct number tst;
bool validate_base(char); //to check if base is one of three bases which I will deal with.
void capitize (); //make all letters capital, to accept capital and small letters
void to_sys_name(char); //B--> Binary , D-->Decimal , H-->Hexadecimal
bool validate_num(char[],char);//check if number compatible with base
void cli(int); //Client functionality
void handler(int ); //signal handler
int main(int argc , char **argv)
{  
   int sockfd;
   struct sockaddr_in servaddr;
   if(argc!=3)
   {
      printf("usage: client <IP_address> <port_address>\n");
      exit(1);
   }
   char por[7] = "44008";
   if (strcmp(por, argv[2]))
   {
       printf("You must use 44008 as a Port number\n");
       exit(0);
   }
   signal(SIGQUIT,handler);
   sockfd=Socket(AF_INET, SOCK_STREAM,0);
   bzero(&servaddr,sizeof(servaddr));
   servaddr.sin_family=AF_INET;
   Inet_aton(argv[1],&servaddr.sin_addr); //argv[1] is the server IP address
   servaddr.sin_port=htons(atoi(argv[2])); //argv[2] is the port address
   Connect(sockfd,(struct sockaddr *)&servaddr, sizeof(servaddr)); //Three way handshaking
   cli(sockfd);
   exit(0);
}
void cli(int sockfd)
{
   printf("Welcome to you in this simple systems converter :)\n");
   printf("Notes:B or b for Binary, D or d for Decimal, and H or h for Hexadecimal\n");
   printf("Don't Enter wrong system or number, I'll catch you ^_^\n");
   printf("Enter your input as following: number:from_base:to_base e.g: 123:d:h\n");
   printf("Press ctrl+\\ to exit!\n");
   while(true)
   {
      char sendline [MAXLINE];
      scanf(" %s",sendline); //input from user
      bool flage=true;
      memset(tst.from_number,0,strlen(tst.from_number)); //set tst.from_number to zero
      tst.from_base='\0';
      tst.to_base='\0';
      split(sendline); //split input: number,from base,to base
      tst.from_base=toupper(tst.from_base);
      to_sys_name(tst.from_base);
      capitize(); //capitlize every char in number: 0xa --> 0xA
      tst.to_base=toupper(tst.to_base);
      if(!validate_base(tst.from_base))
      {
         printf("Wrong input! %c Is an Unknown system\n",tst.from_base);
         flage=false;
      }
      if(!validate_num(tst.from_number, tst.from_base))
      {
         printf("Wrong input! %s Is not compatible with %s System!\n",tst.from_number,sys_name);
         flage=false;
      }
      if(!validate_base(tst.to_base))
      {
         printf("Wrong input! %c Is an Unknown system!\n",tst.to_base);
         flage=false;
      }
      if(!flage)
      {
         printf("Try again or Press Ctrl+\\ to exit \n"); //send Ctrl+\ signal to exit
         continue;
      }
      char recvline[MAXLINE];
      memset(recvline,0,strlen(recvline));
      capitlize(sendline); 
      Writen(sockfd,sendline,strlen(sendline)); //send the request
      memset(sendline,0,strlen(sendline));
      if(Read(sockfd,recvline,MAXLINE)==0) //server terminates
      {
         printf("cli: server terminated prematurely\n");
         exit(1);
      }
      if(strcmp(recvline,"Unsupported conversion!"))
      { 
         printf("%s in %s = %s ",tst.from_number,sys_name,recvline);
         to_sys_name(tst.to_base);
         printf("in %s\n",sys_name);
      }  
      else
      printf("%s\n",recvline);
      printf("you can use my program again, Press Ctrl+\\ to exit!\n");
      continue;
   }
}

bool validate_num(char x [], char y ){
int len = strlen(x);
if(y=='B')
{
   for(int i=0 ; i<len ; i++)
      if(x[i]!='1' && x[i]!='0')
         return false;
}
else if(y=='D')
{
   for(int i=0 ; i<len ; i++)
      if(x[i]<'0' || x[i]>'9')
         return false;
}
else if(y=='H')
{
   for(int i=0 ; i<len ; i++)
      if(!isxdigit(x[i]))
         return false; 
}
return true; 
}
bool validate_base(char x){
   return(!(x!='H' && x!='D' && x!='B'));
}
void capitize (){
   for(int i=0 ;i<strlen(tst.from_number);i++)
   tst.from_number[i] = toupper(tst.from_number[i]);
}
void to_sys_name(char x){
if (x=='B')
sys_name="Binary";
else if (x=='D')
sys_name="Decimal";
else if (x=='H')
sys_name="Hexadecimal";
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
void capitlize (char s[]){
   for(int i=0 ;i<strlen(s);i++)
   s[i] = toupper(s[i]);
}
void handler(int signo)
 {
   printf("Good bye\n");
   exit(0);
 }
