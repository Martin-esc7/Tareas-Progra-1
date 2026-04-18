#include <iostream> 
#include <winsock2.h> 
int main(){ 
WSADATA w; WSAStartup(MAKEWORD(2,2),&w); 
SOCKET s=socket(AF_INET,SOCK_STREAM,0); 
struct sockaddr_in sv; sv.sin_family=AF_INET; sv.sin_port=htons(3306); sv.sin_addr.s_addr=inet_addr("127.0.0.1"); 
if(connect(s,(struct sockaddr*)&sv,sizeof(sv))==0) std::cout<<"Conexion exitosa con MySQL!"<<std::endl; 
return 0;} 
