/************************************/
/* Prog serveur udp                 */
/* Auteur ....                      */
/* 9/9/16                           */
/* serveur UDP                      */
/* gcc -Wall -o        ...........  */
/************************************/




#include <stdio.h>
#include <memory.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <netdb.h>


#include <sys/wait.h>
#include <syslog.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>

	  
#include <unistd.h>
#include <string.h>	 
#include <signal.h>

int sock_SERVEUR;
void arret(int sig)
{
	printf(" \nARRET PROG serveur SOCKET   ---> echo \n");
	 close(sock_SERVEUR);
	kill(getpid(),SIGKILL);
       	  
}

int main(int argc, char **argv)
{
	struct sockaddr_in	sin_dest , sin_src; //adresses sockets destination et sources
	socklen_t taille = sizeof(sin_src);
	unsigned long hostAddr = 0;
	//int	taille=0;
	char	cm[200],txt[200];
	char	c,*cmd;
	int		pos,i;

	/* Attache routine sortie correcte sur CTRL-C */
	signal(SIGINT, arret);
												   
	/* init socket destination CLIENTE */
	bzero((char *) &sin_dest,sizeof(sin_dest));
	
    //COMPLETEZ ci - dessous le port
    sin_dest.sin_port =htons(4000); /*  Numero de port du client  */
	
    sin_dest.sin_family = PF_INET;
	
	 
	sin_dest.sin_addr.s_addr = INADDR_ANY;/* n'importe quelle adresse "acceptable" (par le serveur) en tant qu'adresse source  */
    
	/* init socket source SERVEUR */
	//init à zero de tous les champs
	bzero((char *) &sin_src,sizeof(sin_src));
	
   	
	sin_src.sin_port =htons(5000)     ;  /* port serveur  A renseigner*/
    
    /* RENSEIGNEMENT adresse  Tout est fait ici !!! */
    hostAddr = (inet_addr("127.0.0.1"));
	if((long) hostAddr != (long) -1)
	{
		bcopy(&hostAddr,&sin_src.sin_addr.s_addr,sizeof(hostAddr));
        sin_src.sin_family = PF_INET;
	}
	   
	
     /* CREER la socket serveur pour protocole UDP */

	if((sock_SERVEUR=socket(PF_INET,SOCK_DGRAM,IPPROTO_UDP))<0){
		perror("client:pb creation socket");
		exit(-1);
	} 
		printf("N socket serveur %d\n",sock_SERVEUR);
	
	
    /* ATTACHEZ descripteur de la socket SERVEURà la structure socket "source" sin_src  */
    
    if (bind(sock_SERVEUR,(struct sockaddr*)&sin_src, sizeof(sin_src)) < 0)
    {
        perror("erreur de bind");
        exit(-1);
    }
    
    /* boucle reception / emission   socket   */
    for(;;)
    {
        taille = sizeof(sin_src);//   REINIT du champ taille a chaque reception
        

		       	/*lecture reponse client */
			for(i=0;i < sizeof(txt); *(txt+i++) = '\0')  ;   /* init du buffer de reception */    
			pos=recvfrom(sock_SERVEUR,txt,sizeof(txt),0,(struct sockaddr*)&sin_src,&taille); /*RECEPTION sur socket */

			if(pos > 0)
			{
				printf("client de pid %d\n %d caractere recu du client >> %s\n",getpid(),pos,txt);
				sendto(sock_SERVEUR,txt,pos,0,(struct sockaddr*)&sin_dest,sizeof(sin_dest));/* renvoie vers le client prendre   sendto()*/
			}
				
		}
		   	
	close(sock_SERVEUR);    //fermeture socket serveur
	exit(0);
}
