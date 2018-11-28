//Justin Pimentel
//jfpiment
//1559599

#include "crack.h"

static void threadCrack(Message &msg, unsigned int idx){
	char plainText[256];
	//Zero out buffer
        bzero(plainText, 256);
        //Crack the password
        crack(msg.passwds[idx], plainText);
        //Replace the password with plain text
        strcpy(msg.passwds[idx], plainText);	
}

//Function to crack passwords and replace them into the message
static void crackMsg(Message &msg){
	std::vector<std::thread> vecThreads;
	
	for(unsigned int i = 0; i < ntohl(msg.num_passwds); i++){
		vecThreads.push_back(std::thread(threadCrack, std::ref(msg), i));
		
		if(vecThreads.size() == 24){
			for(std::thread &t: vecThreads){
				t.join();
			}
			vecThreads.clear();
		}
        }

	if(vecThreads.size() > 0){
		for(std::thread &t: vecThreads){
			t.join();
		}
		vecThreads.clear();
	}
}

int main(){
	int port = get_multicast_port();

	//Listening over UDP 
	int sockID = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockID < 0){
		exit(-1);
	}


	struct sockaddr_in server_addr;
	bzero((char *)&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);

	struct sockaddr_in remote_addr;
        socklen_t len2 = sizeof(remote_addr);

	if(bind(sockID, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
		exit(-1);
	}
	
	struct ip_mreq multicastRequest;
	multicastRequest.imr_multiaddr.s_addr = get_multicast_address();
	multicastRequest.imr_interface.s_addr = htonl(INADDR_ANY);

	if(setsockopt(sockID, IPPROTO_IP, IP_ADD_MEMBERSHIP
			, (void *)&multicastRequest, sizeof(multicastRequest)) < 0){
		exit(-1);
	}
	//End UDP

	Message msg = {};

	while(true){
		/*bzero(plainText, 256);
		int n = recvfrom(sockID, plainText, 255, 0 , NULL, 0);
		if(n < 0){
			exit(-1);
		}
		crack("00Pp9Oy0VWmn2", plainText);
		printf("Plain: %s\n", plainText);
		*/

		int n = recvfrom(sockID, &msg, sizeof(msg), 0, (struct sockaddr *)&remote_addr, &len2);
		if(n < 0){
			exit(-1);
		}
		
		if(strcmp(msg.cruzid, "jfpiment") == 0){		
			break;
		}
	}

	//Begin cracking
	/*for(unsigned int i = 0; i < ntohl(msg.num_passwds); i++){
		//Zero out buffer
		bzero(plainText, 256);
		//Crack the password
		crack(msg.passwds[i], plainText);
		//Replace the password with plain text
		strcpy(msg.passwds[i], plainText);
	}*/

	crackMsg(msg);
	
	//Starting TCP sender
	int sockTCP = socket(AF_INET, SOCK_STREAM, 0);
        if(sockTCP < 0){
                exit(-1);
        }

        struct hostent *server = gethostbyname(msg.hostname);
        if(server == NULL){
                exit(-1);
        }

        //printf("Hostname: %s\n", msg.hostname);

        struct sockaddr_in serv_addr;
        bzero((char *)&serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);

        serv_addr.sin_port = msg.port;

        if(connect(sockTCP, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
                printf("Uh oh\n");
                exit(-1);
        }

	int n = write(sockTCP, &msg, sizeof(msg));
        if(n < 0){
                exit(-1);
        }
	//End TCP

	return 0;
}
