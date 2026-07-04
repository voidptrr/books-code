#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

int main()
{
	printf("Configuring local address...\n");
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	struct addrinfo *bind_address;
	int code = getaddrinfo(NULL, "8080", &hints, &bind_address);
	if (code != 0) {
		fprintf(stderr, "%s", gai_strerror(code));
		return 1;
	}

	int socket_listen =
		socket(bind_address->ai_family, bind_address->ai_socktype,
		       bind_address->ai_protocol);

	if (socket_listen < 0) {
		fprintf(stderr, "socket() failed. (%d)\n", errno);
		return 1;
	}

	if (bind(socket_listen, bind_address->ai_addr,
		 bind_address->ai_addrlen)) {
		fprintf(stderr, "bind() failed. (%d) \n", errno);
		return 1;
	}

	freeaddrinfo(bind_address);

	if (listen(socket_listen, 10) < 0) {
		fprintf(stderr, "listen() failed. (%d)\n", errno);
		return 1;
	}

	fd_set master;
	FD_ZERO(&master);
	FD_SET(socket_listen, &master);

	int max_socket = socket_listen;
	printf("Waiting for connections...\n");

	for (;;) {
		fd_set reads = master;
		if (select(max_socket + 1, &reads, NULL, NULL, NULL) < 0) {
			fprintf(stderr, "select() failed. (%d)\n", errno);
			return 1;
		}

		int current;
		for (current = 1; current <= max_socket; current++) {
			if (FD_ISSET(current, &reads)) {
				if (current == socket_listen) {
					struct sockaddr_storage client_address;
					socklen_t client_len =
						sizeof(client_address);
					int socket_client = accept(
						socket_listen,
						(struct sockaddr
							 *)&client_address,
						&client_len);
					if (socket_client < 0) {
						fprintf(stderr,
							"accept() failed. (%d)\n",
							errno);
						return 1;
					}

					FD_SET(socket_client, &master);
					if (socket_client > max_socket)
						max_socket = socket_client;

					char address_buffer[100];
					getnameinfo((struct sockaddr
							     *)&client_address,
						    client_len, address_buffer,
						    sizeof(address_buffer),
						    NULL, 0, NI_NUMERICHOST);
					printf("New connection from %s\n",
					       address_buffer);
				} else {
					char read[1024];
					int bytes_received = recv(
						current, read, sizeof(read), 0);
					if (bytes_received < 1) {
						FD_CLR(current, &master);
						close(current);
						continue;
					}

					for (int j = 0; j < bytes_received; j++)
						read[j] = toupper(read[j]);

					send(current, read, bytes_received, 0);
				}
			}
		}
	}

	close(socket_listen);
	printf("Finished.\n");

	return 0;
}
