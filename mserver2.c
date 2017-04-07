//620088204

#include <stdio.h>
#include <sys/types.h>	/* system type defintions */
#include <sys/socket.h>	/* network system functions */
#include <netinet/in.h>	/* protocol & struct definitions */
#include <stdlib.h>	/* exit() warnings */
#include <string.h>	/* memset warnings */
#include <arpa/inet.h>
#include <unistd.h>

#define BUF_SIZE	1024
#define LISTEN_PORT	60000
#define SEND_PORT       70000

void client_send(char ip[],char msg[]){

char *token;

char fmsg[400];

    int			sock_send;
    struct sockaddr_in	addr_send;
    char			text[80],buf[BUF_SIZE];
    int			send_len,bytes_sent;
        /* create socket for sending data */
    sock_send=socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock_send < 0){
        printf("socket() failed\n");
        exit(0);
    }

            /* fill the address structure for sending data */
    memset(&addr_send, 0, sizeof(addr_send));  /* zero out structure */
    addr_send.sin_family = AF_INET;  /* address family */
    addr_send.sin_addr.s_addr = inet_addr(ip);
    addr_send.sin_port = htons((unsigned short)SEND_PORT);


token=strtok(msg,":");
while(token!=NULL){
	strcpy(fmsg,token);
	token=strtok(NULL,":");
}
        strcpy(buf,fmsg);
        send_len=strlen(fmsg);
        bytes_sent=sendto(sock_send, buf, send_len, 0,(struct sockaddr *) &addr_send, sizeof(addr_send));

    close(sock_send);
}

void server(){


    int			sock_recv;
    struct sockaddr_in	my_addr;
    int			i;
    fd_set	readfds,active_fd_set,read_fd_set;
    struct timeval		timeout={0,0};
    int			incoming_len;
    struct sockaddr_in	remote_addr;
    int			recv_msg_size;
    char			buf[BUF_SIZE];
    int			select_ret;


            /* create socket for receiving */
    sock_recv=socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock_recv < 0){
        printf("socket() failed\n");
        exit(0);
    }
        /* make local address structure */
    memset(&my_addr, 0, sizeof (my_addr));	/* zero out structure */
    my_addr.sin_family = AF_INET;	/* address family */
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);  /* current machine IP */
    my_addr.sin_port = htons((unsigned short)LISTEN_PORT);
        /* bind socket to the local address */
    i=bind(sock_recv, (struct sockaddr *) &my_addr, sizeof (my_addr));
    if (i < 0){
        printf("bind() failed\n");
        exit(0);
    }
       FD_ZERO(&readfds);		/* zero out socket set */
       FD_SET(sock_recv,&readfds);	/* add socket to listen to */
        /* listen ... */

        read_fd_set = active_fd_set;
        select_ret=select(sock_recv+1,&readfds,NULL,NULL,NULL);
        /*select_ret=select(sock_recv+1,&readfds,NULL,NULL,&timeout);*/
        if (select_ret > 0){		/* anything arrive on any socket? */
            /*puts("I received something");*/
            incoming_len=sizeof(remote_addr);	/* who sent to us? */
            recv_msg_size=recvfrom(sock_recv,buf,BUF_SIZE,0,(struct sockaddr *)&remote_addr,&incoming_len);
           /*puts("After receiving...");*/
            if (recv_msg_size > 0){	/* what was sent? */
                buf[recv_msg_size]='\0';
              /*puts("After receiving2...");*/
                printf("From %s \n",inet_ntoa(remote_addr.sin_addr));
		printf("Received: %s \n",buf);
                /*printf("From  received:%s \n",buf);*/
                 /*puts("After receiving3...");*/
             }
            }
close(sock_recv);		
		if(buf[0]=='@'){
			FILE *fip;
			char temp[80];
			fip=fopen("registeredusers.txt","a");
			for(int i=0;i<strlen(buf);i++){
				temp[i]=buf[i+1];			
			}
			fprintf(fip,temp);
			fprintf(fip,"|");
			fprintf(fip,inet_ntoa(remote_addr.sin_addr));
			fprintf(fip,"\n");
			fclose(fip);	
		}
		else if(buf[0]=='#'){
			FILE *fip;
			char temp[80];
			char temp1[80];
			char c[80];
			fip=fopen("registeredusers.txt","r");
			for(int i=0;i<strlen(buf);i++){
				temp[i]=buf[i+1];			
			}
			while(fgets(temp1, 80, fip) != NULL){
				fscanf(fip,"%[^\n]", c);
				if(strcmp(c,temp)==0){
					printf("Valid User");
				}
			}		
		}
		else if(buf[0]=='$'){
			FILE *fip;
			char temp[80];
			fip=fopen("workgroup.txt","a");
			for(int i=0;i<strlen(buf);i++){
				temp[i]=buf[i+1];			
			}
			fprintf(fip,temp);
			fprintf(fip,"|");
			fprintf(fip,inet_ntoa(remote_addr.sin_addr));
			fprintf(fip,"\n");
			fclose(fip);		
		}
		else if(buf[0]=='%'){
			FILE *fip;
			char temp[80];
			fip=fopen("fungroup.txt","a");
			for(int i=0;i<strlen(buf);i++){
				temp[i]=buf[i+1];			
			}
			fprintf(fip,temp);
			fprintf(fip,"|");
			fprintf(fip,inet_ntoa(remote_addr.sin_addr));
			fprintf(fip,"\n");
			fclose(fip);		
		}
		else if(buf[0]=='^'){
			FILE *file;
			file=fopen("chatlog.txt","a");
			FILE *fip;
			char *token;
			char receiver[80];
			char temp[80];
			char ip_address[80];
			char msg[400];
			fip=fopen("registeredusers.txt","r");
			token=strtok(buf,"|");
			strcat(msg,token);
			while(token!=NULL){
				strcpy(receiver,token);
				token=strtok(NULL,"|");
			}
			while(fgets(temp, 80, fip) != NULL){
				token=strtok(temp,"|");
				if((strcmp(token,receiver))==0){ // Checks if the name sent to the server matches the name in the file 
					while(token!=NULL){  // Saving the IP address of corresponding name in a variable (not working)
						strcpy(ip_address,token);
						token=strtok(NULL,"|");
					}
					fprintf(file,msg);
					client_send(ip_address,msg);
					
				}
			}
		}

}

int main(){
	server();
}
