//Server executable
//Justin Pimentel
//jfpiment
//1559599

#include <iostream>
#include "crack.h"

int main(int argc, char* argv[]){
	int port = get_multicast_port();

	int sockID = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockID < 0){
		exit(-1);
	}

	int ttl = 1;
	if(setsockopt(sockID, IPPROTO_IP, IP_MULTICAST_TTL, (void *)&ttl, sizeof(ttl)) < 0){
		exit(-1);
	}

	struct sockaddr_in multicastAddr;
	memset(&multicastAddr, 0, sizeof(multicastAddr));
	multicastAddr.sin_family = AF_INET;
	multicastAddr.sin_addr.s_addr = get_multicast_address();
	multicastAddr.sin_port = htons(port);

	Message msg = {};

	strcpy(msg.cruzid, "jfpiment");
	for(int i = 0; i < 1; i++){
		strcpy(msg.passwds[i], "00Pp9Oy0VWmn2");
	}
	msg.num_passwds = 1;
	strcpy(msg.hostname, "graculus");
	msg.port = 5995;

	while(true){
		//printf("Sending: %s\n", argv[1]);

		msg.num_passwds = htonl(msg.num_passwds);
		msg.port = htons(msg.port);

		int n = sendto(sockID, &msg, sizeof(msg), 0,
				(struct sockaddr *)&multicastAddr, sizeof(multicastAddr));

		if(n < 0){
			printf("n: %d\n", n);
			exit(-1);
		}		
		
		sleep(1);
		break;
	}

	close(sockID);

	/*int sockTCP = socket(AF_INET, SOCK_STREAM, 0);
	if(sockTCP < 0){
		exit(-1);
	}

	struct hostent *server = gethostbyname(msg.hostname);
	if(server == NULL){
		exit(-1);
	}

	printf("Hostname: %s\n", msg.hostname);

	printf("Got here\n");

	struct sockaddr_in serv_addr;
	bzero((char *)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);

	serv_addr.sin_port = htonl(get_unicast_port());

	if(connect(sockTCP, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
		printf("Uh oh\n");
		exit(-1);
	}

	printf("Got here too\n");

	int n = read(sockTCP, &msg, sizeof(msg));
	if(n < 0){
		exit(-1);
	}*/

	int sockTCP = socket(AF_INET, SOCK_STREAM, 0);
        if(sockTCP < 0){
                exit(-1);
        }

        struct sockaddr_in server_addrTCP;
        bzero((char *)&server_addrTCP, sizeof(server_addrTCP));

        server_addrTCP.sin_family = AF_INET;
        server_addrTCP.sin_addr.s_addr = INADDR_ANY;
        server_addrTCP.sin_port = msg.port;

        if(bind(sockTCP, (struct sockaddr *)&server_addrTCP, sizeof(server_addrTCP)) < 0){
                exit(-1);
        }

        listen(sockTCP, 5);

        struct sockaddr_in client_addr;
        socklen_t len = sizeof(client_addr);

        int newsockfd = accept(sockTCP, (struct sockaddr *)&client_addr, &len);
        if(newsockfd < 0){
                exit(-1);
        }

        int n = read(newsockfd, &msg, sizeof(msg));
        if(n < 0){
        	exit(-1);
        }

	for(unsigned int i = 0; i < ntohl(msg.num_passwds); i++){
		printf("Password %d: %s\n", i, msg.passwds[i]);
	}

	
	return 0;
}
