//620088204


#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>	/* exit() warnings */
#include <string.h>	/* memset warnings */
#include <unistd.h>
#include <arpa/inet.h>


#define BUF_SIZE	1024
#define SERVER_IP	"192.168.42.66"
#define	SERVER_PORT	60000
#define LISTEN_PORT     70000

void servsend(char x,char b[]){

	static char temp[80];

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

            
    memset(&addr_send, 0, sizeof(addr_send));
    addr_send.sin_family = AF_INET;  
    addr_send.sin_addr.s_addr = inet_addr(SERVER_IP);
    addr_send.sin_port = htons((unsigned short)SERVER_PORT);

	
	temp[0]=x;
	for(int i=1;i<=strlen(b);i++){
		temp[i]=b[i-1];
	}
	temp[strlen(temp)]='\0';          

	strcpy(buf,temp);
        send_len=(strlen(temp)+1);
        bytes_sent=sendto(sock_send, buf, send_len,0,(struct sockaddr *) &addr_send, sizeof(addr_send));


    close(sock_send);
}

void serv_recv(){
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
                printf("From %s received: %s\n",inet_ntoa(remote_addr.sin_addr),buf);
                /*printf("From  received:%s \n",buf);*/
                 /*puts("After receiving3...");*/
             }
            }

    close(sock_recv);
}


void client_register(char b[]){  //Registers a client 
	servsend('@',b);
	printf("You have succesfully registered!");
}

int is_registered(char a[]){   //Checks to see if a user is registered or not
	servsend('#',a);
	return 0;
}

void addcontact(){
	char name[80];
	char cname[80];
	char pnum[8];
	char record[300];
	FILE *fil;
	fil=fopen("contacts.txt","a");
	printf("Please enter your username before proceeding.");
	scanf("%s",name);
	/*if(is_registered(name)==0){*/
		printf("Enter the name of the contact you wish to add: ");
		scanf("%s",cname);
		printf("Enter the number of the contact you wish to add: ");
		scanf("%s",pnum);
		strcat(record,cname);
		strcat(record,"|");
		strcat(record,pnum);
		fprintf(fil,record);
		fclose(fil);	
}

void list_friends(){		
	char temp1[80];
	FILE *fip;
	fip=fopen("contacts.txt","r");
	while(!feof(fip)){
		fscanf(fip,"%s",temp1);
		printf("%s \n", temp1);
	}
	fclose(fip);
}

void work_group(char text[]){    //Register for the work group 
	servsend('$',text);
	
}

void fun_group(char text[]){      //Register for the fun group 
	servsend('%',text);
}

void chat(){		//Allows clients to chat to one another 
	char name[400];
	char msg[400];
	char rname[400];
	printf("Please enter your username before proceeding: ");
	scanf("%s",name);
	printf("Please enter the message you would like to send.(A single message cannot exceed 200 characters.): ");
	scanf("%s",msg);
	printf("Please enter the intended receiver of this message: ");
	scanf("%s",rname);
	strcat(name,":");
	strcat(name,msg);
	strcat(name,"|");
	strcat(name,"To|");
	strcat(name,rname);
	servsend('^',name);
	serv_recv();
}

main(){
	int opt;
	char text[80];
	char temp[10];

while(1){
	printf("========================================= \n");
	printf("=============WHATSAPP CLONE============== \n");
	printf("========================================= \n");
	printf("What would you like to do? \n");
	printf("1. Register to use the system. \n");
  	printf("2. See a list of friends/contacts. \n");  
  	printf("3. Chat with a friend. \n");
  	printf("4. Register for Work Group. \n");
  	printf("5. Register for Fun Group. \n");
	printf("6. Send a message across the work group. \n");
	printf("7. Send a message across the fun group. \n");
	printf("8. Add to contacts. \n");
	printf("9. Quit. \n");
	printf("========================================= \n");
	scanf("%d",&opt);
	if(opt==1){
        	printf("What username would you like? (Note: A username cannot start with the following: @ # $ % ^ & *): ");
        	scanf("%s",text);
        	printf("Enter phone number: ");
        	scanf("%s",temp);
        	strcat(text,"|");
        	strcat(text,temp);
		client_register(text);	
	}
	else if(opt==2){
		list_friends();	
	}
	else if(opt==3){
		chat();
	}
	else if(opt==4){
		printf("Please enter your username before proceeding: ");
		scanf("%s",text);
		printf("Enter phone number: ");
        	scanf("%s",temp);
        	strcat(text,"|");
        	strcat(text,temp);
		work_group(text);	
	}
	else if(opt==5){	
		printf("Please enter your username before proceeding: ");
		scanf("%s",text);
		printf("Enter phone number: ");
        	scanf("%s",temp);
        	strcat(text,"|");
        	strcat(text,temp);
		fun_group(text);
	}
	else if(opt==6){
		printf("Not yet implemented.");
	}
	else if(opt==7){
		printf("Not yet implemented.");
	}
	else if(opt==8){
		addcontact();
	}
	else if(opt==9){
		break;
	}
}
}
