#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string>
#include <vector>
#include <iostream>


int main(int argc, char* argv[]){
    if(argc != 2){
	std::cout << "Port is not defihed\n";
	return 1;
    }
    //some settings;
    int backlog = 10;
    sockaddr_storage their_addr;
    socklen_t addr_size;
    struct addrinfo hints;
    struct addrinfo *res;
    int sockfd, new_fd;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    
    if(getaddrinfo(NULL, argv[1], &hints, &res) != 0){
	std::cout << "ERROR: getaddrinfo\n";
	return 2;
    }
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1){
	std::cout << "ERROR: socket\n";
    return 3;
    }
    if(bind(sockfd, res->ai_addr, res->ai_addrlen) == -1){
	std::cout << "ERROR: bind\n";
	return 4;
    }
    if (listen(sockfd, backlog) == -1){
	std::cout << "ERROR: listen\n";
	return 5;
    }

    while(true){

	std::string c;
	addr_size = sizeof their_addr;

	new_fd = accept(sockfd, (struct sockaddr*)&their_addr, &addr_size);

	if (new_fd == -1)
	    continue;
	int read_len;
	int len_b = 1;
	char* buf = new char[len_b];
	std::vector<char> a;

	while (read_len = recv(new_fd, buf, len_b, 0) > 0){

	    a.insert(a.end(), buf, buf + read_len);
	}

	delete [] buf;
	
	//write
	int write_len;
	char* date = a.data();
	int len_to_write = a.size();

	while(write_len = send(new_fd, date, len_to_write, 0) > 0){
	    date += write_len;
	    len_to_write -= write_len;
	}

	if(len_to_write > 0)
	    std::cout << "ERROR: bytes lost\n";
    }
    freeaddrinfo(res);
    return 0;
}