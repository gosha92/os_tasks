#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <errno.h>
#include <memory.h>
#include <unistd.h>
#include <vector>
#include <iostream>

using namespace std;

int main()
{
	int S = socket(AF_INET, SOCK_STREAM, 0); // Socket

	int on = 1; // Make reusable
	setsockopt(S, SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on));
	
	ioctl(S, FIONBIO, (char *)&on); // Make non-blocking

	struct sockaddr_in addr; // Bind to address
	//memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(12345); // port
	bind(S, (struct sockaddr *)&addr, sizeof(addr));

	listen(S, 64); // 64 clients

    pollfd LISTEN_S; // Vector for sockets
    LISTEN_S.fd = S;
	LISTEN_S.events = POLLIN;
	vector<pollfd> sockets = vector<pollfd>();
	sockets.push_back(LISTEN_S);
	
	int length;
	int i;
	char buffer[256];
	pollfd NOW_S;
	int NEW_CLIENT_S;

	while(true)
	{
		poll(sockets.data(), sockets.size(), 30 * 1000);cout.flush();

		for (i = 1; i < sockets.size(); ++i)
		{
			NOW_S = sockets[i];

			if (NOW_S.revents == 0) {continue;}

			length = recv(NOW_S.fd, buffer, sizeof(buffer) - 1, 0);
			if (length <= 0)
			{
				close(NOW_S.fd);
				NOW_S.fd = -1;
			}
			else
			{
				buffer[length] = (char)NULL;
				cout<<buffer;
				cout.flush();
				send(NOW_S.fd, buffer, length, 0);
			}
		}
		
		if (sockets[0].revents == POLLIN)
		{
			while(true)
			{
				NEW_CLIENT_S = accept(S, NULL, NULL);
				if (NEW_CLIENT_S < 0) break;

				pollfd new_client;
				new_client.fd = NEW_CLIENT_S;
				new_client.events = POLLIN;
				sockets.push_back(new_client);
			}
		}
        
		for (i = 0; i < sockets.size(); ++i)
		{
			if (sockets[i].fd == -1)
			{
				sockets[i] = sockets.back();
				sockets.pop_back();
			}
		}
	}
}
