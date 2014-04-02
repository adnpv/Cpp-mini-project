#include<iostream>
#include<stdio.h>
#include<string.h>
#include<string>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netdb.h>

using namespace std;


class tcp_client
{
private:
	int sock;
	std::string address;
	int port;
	struct sockaddr_in server;
	
public:
	tcp_client();
	bool conn(string, int);
	bool send_data(string data);
	string receive(int);
};

tcp_client::tcp_client()
{
	sock = -1;
	port = 0;
	address = "";
}

bool tcp_client::conn(string address , int port)
{
	if(sock == -1)
	{
		sock = socket(AF_INET , SOCK_STREAM , 0);
		if (sock == -1)
		{
			perror("Could not create socket");
		}
		
		cout<<"Socket created\n";
	}
	else	{		}
	

	if(inet_addr(address.c_str()) == -1)
	{
		struct hostent *he;
		struct in_addr **addr_list;
		
		if ( (he = gethostbyname( address.c_str() ) ) == NULL)
		{
			herror("gethostbyname");
			cout<<"Failed to resolve hostname\n";
			
			return false;
		}
		
		
		addr_list = (struct in_addr **) he->h_addr_list;

		for(int i = 0; addr_list[i] != NULL; i++)
		{
			server.sin_addr = *addr_list[i];
			
			cout<<address<<" resolved to "<<inet_ntoa(*addr_list[i])<<endl;
			
			break;
		}
	}
	

	else
	{
		server.sin_addr.s_addr = inet_addr( address.c_str() );
	}
	
	server.sin_family = AF_INET;
	server.sin_port = htons( port );
	

	if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		perror("connect failed. Error");
		return 1;
	}
	
	cout<<"Connected\n";
	return true;
}


bool tcp_client::send_data(string data)
{

	if( send(sock , data.c_str() , strlen( data.c_str() ) , 0) < 0)
	{
		perror("Send failed : ");
		return false;
	}
	cout<<"Data send\n";
	
	return true;
}


string tcp_client::receive(int size=512)
{
	char buffer[size];
	string reply;
	
	if( recv(sock , buffer , sizeof(buffer) , 0) < 0)
	{
		puts("recv failed");
	}
	
	reply = buffer;
	return reply;
}

int main(int argc , char *argv[])
{
	tcp_client c;
	string host;
	
	cout<<"Enter hostname : ";
	cin>>host;
	
	c.conn(host , 80);
	
	c.send_data("GET / HTTP/1.1\r\n\r\n");

	cout<<"----------------------------\n\n";
	cout<<c.receive(1024);
	cout<<"\n\n----------------------------\n\n";
	
	return 0;
}
 

