#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <errno.h>
#include <memory.h>
#include <unistd.h>
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

	fd_set master_set; // Master_set
	FD_ZERO(&master_set);
	int MAX_S = S; // Maximum socket descriptor id
	FD_SET(S, &master_set);

	struct timeval timeout; // Set timeout for select
	timeout.tv_sec = 30;
	timeout.tv_usec = 0;

	fd_set working_set; // Working set
	
	int rc;
	int amount;
	int i;
	int INPUT_S;
	char buffer[256];

	while(true)
	{
		working_set = master_set;
		rc = select(MAX_S + 1, &working_set, NULL, NULL, &timeout);
		if (rc == 0) break; // Timeout => EXIT
		amount = rc;
		for (i = 0; i <= MAX_S && amount > 0; ++i)
		{
			if (FD_ISSET(i, &working_set)) // If descriptor "i" is ready
			{
				amount -= 1;
				if (i == S) // Socket "S" is readable
				{
					do
					{
						INPUT_S = accept(S, NULL, NULL);
						if (INPUT_S == -1) break;
						FD_SET(INPUT_S, &master_set);
						if (INPUT_S > MAX_S) MAX_S = INPUT_S;
					} while (INPUT_S != -1);
				}
				else
				{
					rc = recv(i, buffer, sizeof(buffer) - 1, 0);
					if (rc < 0) break;
					if (rc == 0) // Close connection
					{
						close(i);
						FD_CLR(i, &master_set);
						if (i == MAX_S) while (FD_ISSET(MAX_S, &master_set) == false)  MAX_S -= 1; // New max
					}
					buffer[rc] = (char)NULL;
					cout<<buffer;
					cout.flush();
					send(i, buffer, rc, 0);
				}
			}
		}
	}
}






