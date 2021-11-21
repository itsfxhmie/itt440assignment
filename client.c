#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>

int main(int argc,char *argv[])
{
    int sockfd,len,ret;
    struct sockaddr_in saddr;
    char *hname,buff[256];
    struct hostent *hostinfo;
    struct servent *servinfo;
    if(argc == 1)
        hname = "localhost";
    else
        hname = argv[1];
   //getting host  information by name
    hostinfo = gethostbyname(hname);
    //if the host not found
    if(!hostinfo) {
        fprintf(stderr,"No host: %s\n ",hname);
        exit(1);
    }
   //getting server information by name
    servinfo = getservbyname("daytime","tcp");
    //if the server not found
    if(!servinfo) {
        fprintf(stderr,"[+]invalid day time server \n");
        exit(1);
    }
    //if the server found 
    printf("[+]Day time server Port: %d\n", ntohs(servinfo->s_port));

    // creating tcp socket
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    saddr.sin_family = AF_INET;
    saddr.sin_port = servinfo->s_port;
    saddr.sin_addr = *(struct in_addr *) *(hostinfo->h_addr_list);
    len = sizeof(saddr);
   
    ret = connect(sockfd,(struct sockaddr *)&saddr,len);
    if(ret == -1) {
        perror("[+]connect ");
        exit(1);
    }
   
    ret = read(sockfd,buff,sizeof(buff));
    buff[ret] = '\0';
    printf("[+]Result: %s and %d bytes read\n",buff,ret);
   
    //closing the socket
    close(sockfd);

return 0;
}
