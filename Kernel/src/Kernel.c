
#include <socketConfig.h>
#include <commons/config.h>

#define TRUE   1
#define FALSE  0
#define PORT 8888

typedef struct kernell_configuracion {
	int PUERTO_PROG;
	int PUERTO_CPU;
	char* IP_MEMORIA;
	int PUERTO_MEMORIA;
	char* IP_FS;
	int PUERTO_FS;
	int QUANTUM;
	int QUANTUM_SLEEP;
	char* ALGORITMO;
	int GRADO_MULTIPROG;
	int STACK_SIZE;
	char** SEM_IDS;
	char** SEM_INIT;
	char** SHARED_VARS;
} kernell_configuracion;

kernell_configuracion get_configuracion() {
	printf("Inicializando proceso Kernel\n");
	kernell_configuracion configuracion;
	// Obtiene el archivo de configuracion
	char* path = "/home/utnso/workspace/tp-2017-1c-AsadoClash/Kernel/config-kernell.cfg";
	t_config* archivo_configuracion = config_create(path);

	configuracion.PUERTO_PROG = get_campo_config_int(archivo_configuracion, "PUERTO_PROG");
	configuracion.PUERTO_CPU = get_campo_config_int(archivo_configuracion, "PUERTO_CPU");
	configuracion.IP_MEMORIA = get_campo_config_char(archivo_configuracion, "IP_MEMORIA");
	configuracion.PUERTO_MEMORIA = get_campo_config_int(archivo_configuracion, "PUERTO_MEMORIA");
	configuracion.IP_FS = get_campo_config_char(archivo_configuracion, "IP_FS");
	configuracion.PUERTO_FS = get_campo_config_int(archivo_configuracion, "PUERTO_FS");
	configuracion.QUANTUM = get_campo_config_int(archivo_configuracion, "QUANTUM");
	configuracion.QUANTUM_SLEEP = get_campo_config_int(archivo_configuracion, "QUANTUM_SLEEP");
	configuracion.ALGORITMO = get_campo_config_char(archivo_configuracion, "ALGORITMO");
	configuracion.GRADO_MULTIPROG = get_campo_config_int(archivo_configuracion, "GRADO_MULTIPROG");
	configuracion.SEM_IDS = get_campo_config_array(archivo_configuracion, "SEM_IDS");
	configuracion.SEM_INIT = get_campo_config_array(archivo_configuracion, "SEM_INIT");
	configuracion.SHARED_VARS = get_campo_config_array(archivo_configuracion, "SHARED_VARS");
	configuracion.STACK_SIZE = get_campo_config_int(archivo_configuracion, "STACK_SIZE");
	return configuracion;
}


int main(void){
	kernell_configuracion configuracion = get_configuracion();

	fd_set listaOriginal;
	fd_set listaAux;
	FD_ZERO(&listaOriginal);
	FD_ZERO(&listaAux);

	int fd_max = 1;

	int newfd;        // newly accept()ed socket descriptor
	struct sockaddr_storage remoteaddr; // client address
	socklen_t addrlen;

	char buf[256];    // buffer for client data
	int nbytes;

	char remoteIP[INET6_ADDRSTRLEN];

	int yes=1;        // for setsockopt() SO_REUSEADDR, below
	int i, j, rv;

	struct addrinfo hints, *ai, *p;
	/*
	// get us a socket and bind it
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	if ((rv = getaddrinfo(NULL, PORT, &hints, &ai)) != 0) {
		//fprintf(stderr, "selectserver: %s\n", gai_strerror(rv));
		exit(1);
	}

	for(p = ai; p != NULL; p = p->ai_next) {
		socketServer = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
	    if (socketServer < 0) {
	    	continue;
	    }

// lose the pesky "address already in use" error message
	setsockopt(socketServer, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

	if (bind(socketServer, p->ai_addr, p->ai_addrlen) < 0) {
		close(socketServer);
		continue;
	}
		break;
	}
*/
	struct sockaddr_in direccionServidor;

	direccionServidor.sin_family = AF_INET;
	direccionServidor.sin_addr.s_addr = inet_addr("127.0.0.1");
	direccionServidor.sin_port = htons(PORT);

	un_socket socketServer = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(socketServer, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
	bind(socketServer, (struct sockaddr *) &direccionServidor, sizeof(direccionServidor));
	listen(socketServer, 256);

	FD_SET(socketServer,&listaOriginal);
	listaAux = listaOriginal;

while(1){
	select(fd_max + 1, &listaAux, NULL, NULL, NULL);
	for(i = 0; i <= fd_max; i++) {
		if (FD_ISSET(i, &listaAux)) {
			if (i == socketServer) {
				//manejamos conexiones nuevas
				addrlen = sizeof remoteaddr;
	            newfd = accept(socketServer,(struct sockaddr *)&remoteaddr,&addrlen);
	            if (newfd == -1) {
	            	perror("accept");
	            } else {
	                FD_SET(newfd, &listaOriginal); // add to master set
	                if (newfd > fd_max) {    // keep track of the max
	                	fd_max = newfd;
	                }
	                printf("Kernel recibio una nueva conexion");
	                //printf("Kernel: nueva conexion de %s en ""socket %d\n", inet_ntop(remoteaddr.ss_family, get_in_addr((struct sockaddr*)&remoteaddr),remoteIP, INET6_ADDRSTRLEN),newfd);
	            }
			} else {
				// manejamos informacion recibida
				if ((nbytes = recv(i, buf, 5, 0)) <= 0) {
	            // got error or connection closed by client
					if (nbytes == 0) {
						// connection closed
	                    printf("selectserver: socket %d hung up\n", i);
	                } else {
	                    perror("recv");
	                }
	                close(i); // bye!
	                FD_CLR(i, &listaOriginal); // remove from master set
	             } else {
	            	 // we got some data from a client
	                 printf("%s", (char*) buf);
	            	 /*
	            	 for(j = 0; j <= fd_max; j++) {
	                	 // send to everyone!
	                     if (FD_ISSET(j, &listaOriginal)) {
	                    	 // except the listener and ourselves
	                         if (j != socketServer && j != i) {
	                        	 if (send(j, buf, nbytes, 0) == -1) {
	                        		 perror("send");
	                             }
	                         }
	                     }*/
	                 }
			}
		}
		listaAux = listaOriginal;
} // END got new incoming connection
}
	void *get_in_addr(struct sockaddr *sa)
	{
	    if (sa->sa_family == AF_INET) {
	        return &(((struct sockaddr_in*)sa)->sin_addr);
	    }

	    return &(((struct sockaddr_in6*)sa)->sin6_addr);
	}

	return 0;
}

	/*
	int opt = TRUE;
	    int master_socket , addrlen , new_socket , client_socket[30] ,
	          max_clients = 30 , activity, i , valread , sd;
	    int max_sd;
	    struct sockaddr_in address;

	    char buffer[1025];  //data buffer of 1K

	    //set of socket descriptors
	    fd_set readfds;

	    //initialise all client_socket[] to 0 so not checked
	    for (i = 0; i < max_clients; i++)
	    {
	        client_socket[i] = 0;
	    }

	    //create a master socket
	    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)
	    {
	        perror("socket failed");
	        exit(EXIT_FAILURE);
	    }

	    //set master socket to allow multiple connections ,
	    //this is just a good habit, it will work without this
	    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
	          sizeof(opt)) < 0 )
	    {
	        perror("setsockopt");
	        exit(EXIT_FAILURE);
	    }

	    //type of socket created
	    address.sin_family = AF_INET;
	    address.sin_addr.s_addr = INADDR_ANY;
	    address.sin_port = htons( PORT );

	    //bind the socket to localhost port 8888
	    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)
	    {
	        perror("bind failed");
	        exit(EXIT_FAILURE);
	    }
	    printf("Listener on port %d \n", PORT);

	    //try to specify maximum of 3 pending connections for the master socket
	    if (listen(master_socket, 3) < 0)
	    {
	        perror("listen");
	        exit(EXIT_FAILURE);
	    }

	    //accept the incoming connection
	    addrlen = sizeof(address);
	    puts("Waiting for connections ...");

	    while(TRUE)
	    {
	        //clear the socket set
	        FD_ZERO(&readfds);

	        //add master socket to set
	        FD_SET(master_socket, &readfds);
	        max_sd = master_socket;

	        //add child sockets to set
	        for ( i = 0 ; i < max_clients ; i++)
	        {
	            //socket descriptor
	            sd = client_socket[i];

	            //if valid socket descriptor then add to read list
	            if(sd > 0)
	                FD_SET( sd , &readfds);

	            //highest file descriptor number, need it for the select function
	            if(sd > max_sd)
	                max_sd = sd;
	        }

	        //wait for an activity on one of the sockets , timeout is NULL ,
	        //so wait indefinitely
	        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);

	        if ((activity < 0) && (errno!=EINTR))
	        {
	            printf("select error");
	        }

	        //If something happened on the master socket ,
	        //then its an incoming connection
	        if (FD_ISSET(master_socket, &readfds))
	        {
	            if ((new_socket = accept(master_socket,
	                    (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
	            {
	                perror("accept");
	                exit(EXIT_FAILURE);
	            }

	            //inform user of socket number - used in send and receive commands
	            printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , new_socket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));


	            //a message
	            char *message = "hola";

	            //memcpy(&message, address.sin_port, 5);

	            //send new connection greeting message
	            if( send(new_socket, message, strlen(message), 0) != strlen(message) )
	            {
	                perror("send");
	            }

	            puts("Welcome message sent successfully");

	            //
	            char* buffer1 = malloc(1000);

	            int bytesRecibidos = recv(new_socket, buffer1, 1000, 0);
	            buffer[bytesRecibidos] = '\0';
	            printf("Me llegaron %d bytes con %s\n", bytesRecibidos, buffer1);

	            free(buffer1);
	            //

	            //add new socket to array of sockets
	            for (i = 0; i < max_clients; i++)
	            {
	                //if position is empty
	                if( client_socket[i] == 0 )
	                {
	                    client_socket[i] = new_socket;
	                    printf("Adding to list of sockets as %d\n" , i);

	                    break;
	                }
	            }
	        }

	        //else its some IO operation on some other socket
	        for (i = 0; i < max_clients; i++)
	        {
	            sd = client_socket[i];

	            if (FD_ISSET( sd , &readfds))
	            {
	                //Check if it was for closing , and also read the
	                //incoming message
	                if ((valread = read( sd , buffer, 1024)) == 0)
	                {
	                    //Somebody disconnected , get his details and print
	                    getpeername(sd , (struct sockaddr*)&address , \
	                        (socklen_t*)&addrlen);
	                    printf("Host disconnected , ip %s , port %d \n" ,
	                          inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

	                    //Close the socket and mark as 0 in list for reuse
	                    close( sd );
	                    client_socket[i] = 0;
	                }

	                //Echo back the message that came in
	                else
	                {
	                    //set the string terminating NULL byte on the end
	                    //of the data read
	                    buffer[valread] = '\0';
	                    send(sd , buffer , strlen(buffer) , 0 );
	                }
	            }
	        }
	    }
		*/

