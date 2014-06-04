#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
//#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
int main(int argc, char* argv[]){
char *login = "test";
char *haslo = "alamakota";
int login_ok;
int pass_ok;
login_ok = 0;
pass_ok = 0;
/* Tworzenie gniazda */
/* socket() zwraca deskrypotor gniazda */
/* To jest master socket - naslucuje na nim */
/* gdy polaczenie zostaje nawiazane przechodzi */
/* poprzez connect() na 'another socket' */
int socket_desc = socket(AF_INET, SOCK_STREAM,0);
/* Print ID procesu i deskryptora gnigazda */
//printf ("The process ID: %d\n", (int) getpid());
printf("Socket descriptor: %d\n",socket_desc);

struct sockaddr_in adres;
/* Typ gniazda socket_desc */
adres.sin_family = AF_INET;
adres.sin_addr.s_addr = INADDR_ANY;
/* Port na jakim serwer nasluchuje */
int portno;
/*Konwersja char na int z argumentu wywolania programu */
portno=atoi(argv[1]);
adres.sin_port = htons(portno);
/* htons() konwertuje z host byte order */
/* do network byte order */
/* bindowanie gniazda do przypisanego portu */
int* bind_check = bind(socket_desc,(struct sockaddr *)&adres,sizeof(adres));
/* Print zwroconej wartosci przez bind(), ok > 0, not > -1 */
if (&bind_check > 0){printf("Socket binded properly\n");}
/* Nasluchiwanie na master porcie */
/*  max. 3 polaczenia */
/* if (conn > 3) timeout err; */
int* listen_check = listen(socket_desc, 3);
if (&listen_check > 0){printf("Listening on port: %d\n",portno);}
/* Accept connection */
int adr_len;
int new_socket;
adr_len = sizeof(struct sockaddr_in);
new_socket = accept(socket_desc, (struct sockaddr *)&adres, &adr_len);
if (new_socket < 0){ perror("Accept connection");}
printf("New socket descriptor: %d\n", new_socket);
/* Sending data */
char *msg = "Connection has been established.\r\nNow you can rule the World\r\nEnter your login: ";
char *confirm = ".\n";
int send_check = send(new_socket,msg,strlen(msg),0);
printf("Bytes sent: %d\n",send_check);
/*Revice data */
int buffsize=1024;
char *buffer=malloc(buffsize);
int recv_check = recv(new_socket,buffer,buffsize,0);
printf("%s",buffer);
if (buffer==&login){printf("OK!\n");}else{printf("FAIL!\n");}
//printf("Bytes recived: %d\n",recv_check);
//printf("Msg: %s",buffer);
//send_check = send(new_socket,confirm,strlen(confirm),0);
/*if (buffer==login){
	buffer = "0";
	login_ok = 1;
	printf("Enter your passowrd: ");
	recv(new_socket,buffer,buffsize,0);

if (buffer==haslo){
	printf("Password ok! Acces granted!\n");
	pass_ok = 1;
 
if (pass_ok == 1) {printf("GRANTED!");}}}*/

sleep(1000);
int close_check = close(socket_desc);
printf("Close: %d\n",close_check);
}
