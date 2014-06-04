#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
//#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
int main(int argc, char* argv[]){
int buffsize=1024;
char *login = "test\n";
char *haslo = "alamakota";
int login_ok;
int pass_ok;
int ibuff;
int ilogin;
int ihaslo;
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
int send_check = send(new_socket,msg,strlen(msg),0);
printf("Bytes sent: %d\n",send_check);
/*Revice data */
char* buffer=malloc(buffsize);
int recv_check = recv(new_socket,buffer,buffsize,0);
printf("%s",buffer);
printf("%s",login);
	char* lconfirm = "Login OK!\n";
	char* ldeny = "Login incorrect!\n";
	char* pconfirm = "Passowrd OK!\n";
	char* pdeny = "Password incorrect!\n";
int login_check(char* ltocheck){
	//int iltocheck;
	//iltocheck=atoi(ltocheck);
	//int ilogin;
	//ilogin = atoi(login);
	//printf("%d\n",iltocheck);
	//printf("%d\n",ilogin);
	htons(login);
	printf("%s",login);
	printf("%s",ltocheck);
		if (ltocheck==login){
			login_ok=1;
			send(new_socket,lconfirm,strlen(lconfirm),0);
		} else { 
			send(new_socket,ldeny,strlen(ldeny),0);
			login_ok=0;
		}
	return login_ok;
}

int pass_check(char* ptocheck){
	int iptocheck;
	iptocheck=atoi(ptocheck);
	int ipass;
	ipass = atoi(haslo);
		if (iptocheck==ipass){
			pass_ok=1;
			send(new_socket,pconfirm,strlen(pconfirm),0);
		} else { 
			send(new_socket,pdeny,strlen(pdeny),0);
			pass_ok=0;
		}
	return pass_ok;
}
const int test = login_check(buffer);
printf("Login: %d\n",test);
char* enter_pass = "Podaj haslo: ";
send(new_socket,enter_pass,strlen(enter_pass),0);
recv(new_socket,buffer,buffsize,0);
const int test2 = pass_check(buffer);
sleep(1000);
int close_check = close(socket_desc);
printf("Pass: %d\n",test2);
printf("Close: %d\n",close_check);
return 0;
}
