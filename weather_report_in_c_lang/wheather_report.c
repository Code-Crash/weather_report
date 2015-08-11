/**************   Wheather Report  ***************
 * This snippet call the Wheather api and get the response from server by using socket
 * 
 * 
 * **************************************************/


#include<stdio.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<netdb.h> //hostent
#include<arpa/inet.h> //inet_addr

#define MAX_BUF  200
#define MAX_AD 4096

 
int main(int argc , char *argv[])
{
    int socket_desc;
    struct sockaddr_in server;
    char request[MAX_AD + 1] , response[MAX_AD + 1] , ip[100];
    struct hostent *he;
    struct in_addr **addr_list;
    int i;
    char *hostname = "api.openweathermap.org";              // Host name
    char *data = "/data/2.5/weather?q=London,uk";           // data 
    int len = strlen(data);

         
    if ( (he = gethostbyname( hostname ) ) == NULL) 
    {
        //gethostbyname failed
        herror("gethostbyname");
        return 1;
    }
     
    //Cast the h_addr_list to in_addr , since h_addr_list also has the ip address in long format only
    addr_list = (struct in_addr **) he->h_addr_list;
     
    for(i = 0; addr_list[i] != NULL; i++) 
    {
        //Return the first one;
        strcpy(ip , inet_ntoa(*addr_list[i]) );
    }
     
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
         
    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_family = AF_INET;
    server.sin_port = htons( 80 );
 
    //Connect to remote server
    if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        puts("connect error");
        return 1;
    }
     
    puts("\nConnected\n");
    
    // make request with size MAXSUB
    snprintf(request, MAX_BUF,
         "GET %s HTTP/1.0\r\n"
         "Host: %s\r\n"
         "Content-type: application/x-www-form-urlencoded\r\n"
         "Content-length: %u\r\n\r\n"
         "%s", data, hostname, len, data);
    
    //int snprintf(char *str, size_t size, const char *format, ...);

    // print our formated data
    printf("\nFormated request:\n%s \n",request);

    
    //Send some data
    if( send(socket_desc , request , strlen(request) , 0) < 0)
    {
        puts("Send failed");
        return 1;
    }
    puts("\nData Send\n");
     
    //Receive a reply from the server
    if( recv(socket_desc, response , 2000 , 0) < 0)
    {
        puts("recv failed");
    }
    puts("\nReply received\n");
    puts(response);
     
    return 0;
}