#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <errno.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <vector>
#include <semaphore.h>
#include <pthread.h> 
#include "server.h"
#include "parser.h"

#define client_message_SIZE 1024
#define PORT 8080
int thread_count = 0;
int randomV = PORT;
sem_t mutex;
std::vector<std::string> serverData;

void send_message(int fd, string FilePath, string headerFile){

    string header = Messages[HTTP_HEADER] + headerFile;
    FilePath = "./public"+ FilePath;
    struct stat stat_buf;  /* hold information about input file */

    write(fd, header.c_str(), header.length());

    int fdimg = open(FilePath.c_str(), O_RDONLY);
    
    if(fdimg < 0){      
        printf("Cannot Open file path : %s with error %d\n", FilePath.c_str(), fdimg); 
        return; 
    }
     
    fstat(fdimg, &stat_buf);
    int img_total_size = stat_buf.st_size;
    int block_size = stat_buf.st_blksize;
    //printf("image block size: %d\n", stat_buf.st_blksize);  
    //printf("image total byte st_size: %d\n", stat_buf.st_size);
    if(fdimg >= 0){
        ssize_t sent_size;

        while(img_total_size > 0){
              int send_bytes = ((img_total_size < block_size) ? img_total_size : block_size);
              int done_bytes = sendfile(fd, fdimg, NULL, send_bytes);
              img_total_size = img_total_size - done_bytes;
            //}
        }
        if(sent_size >= 0){
            printf("send file: %s \n" , FilePath.c_str());
        }
        close(fdimg);
    }
}

string findFileExt(string fileEXt){
    for(int i = 0; i <= sizeof(fileExtension); i++){
        if(fileExtension[i] == fileEXt.substr(0, fileExtension[i].length())){
            return ContentType[i];
        }
    }
    printf("serving: .%s as html\n", fileEXt.c_str()); 
    return ("Content-Type: text/html\r\n\r\n");    
}


void getData(string requestType, string client_message){
    string extract;
    string data = client_message;
    printf("client_messagess : %s\n",client_message.c_str());
  //  printf("requestType : %s\n",requestType.c_str());
    if(requestType == "GET"){
        data.erase(0, getStr(data, ' ').length()+1);
        data = getStr(data, ' ');
        data.erase(0, getStr(data, '?').length()+1);

    }else
    if(requestType == "POST"){
        int counter = data.length()-1;
        while(counter > 0){
            if(data[counter] == ' ' || data[counter] == '\n'){
            break;}
            counter--;
        }data.erase(0, counter+1);
        int found = data.find("=");
        if(found == string::npos){
            data = "";
        }        
    }

    int found = client_message.find("Cookie");
    if(found != string::npos){
        client_message.erase(0, found+8);
        client_message = getStr(client_message, ' ');
        data = data+"&"+getStr(client_message, '\n');
    }

    size_t pos = data.find("`");
    while( pos != std::string::npos)
    {
        data.replace(pos, 1, "");
        pos =data.find("`", pos + 1);
    }
    pos = data.find("\n");
    while( pos != std::string::npos)
    {
        data.replace(pos, 1, "");
        pos =data.find("\n", pos + 1);
    }
    pos = data.find("\r");
    while( pos != std::string::npos)
    {
        data.replace(pos, 1, "");
        pos =data.find("\r", pos + 1);
    }
    pos = data.find("\t");
    while( pos != std::string::npos)
    {
        data.replace(pos, 1, "");
        pos =data.find("\t", pos + 1);
    }
    pos = data.find("%60");
    while( pos != std::string::npos)
    {
        data.replace(pos, 1, "");
        pos =data.find("%60", pos + 1);
    }



    while(data.length() > 0){
        extract = getStr(data, '&');
        serverData.push_back(extract);
        data.erase(0, getStr(data, '&').length()+1);
    }   
}

void *connection_handler(void *socket_desc)
{
    int request, send_byte ;
    char client_message[client_message_SIZE];

    int newSock = *((int *)socket_desc);
    request = read(newSock, client_message, client_message_SIZE);
    string message = client_message;
    sem_wait(&mutex);
    thread_count++; 
    printf("thread counter . %d\n",thread_count);
    if(thread_count > 20)
    {
        write(newSock, Messages[BAD_REQUEST].c_str(), Messages[BAD_REQUEST].length());
        thread_count--; 
        sem_post(&mutex);
        close(newSock);
        pthread_exit(NULL);
    }
    sem_post(&mutex);

    if (request < 0) // Receive failed.
    {
        puts("Recv failed");
    }
    else if (request == 0) // receive socket closed. Client disconnected unexpectedly.
    {
        puts("Client disconnected unexpectedly.");
    }
    else 
    {    string mess = client_message;
        int found = mess.find("multipart/form-data");
    if(found != string::npos){
        found = mess.find("Content-Length:");
        mess.erase(0, found+16);
        int length = stoi(getStr(mess,' '));
        found = mess.find("filename=");
        mess.erase(0, found+10);
        string newf = getStr(mess,'"');
        newf = "./downloads/"+ newf;
        found = mess.find("Content-Type:");
        mess.erase(0, found+15);
        mess.erase(0, getStr(mess,'\n').length()+3);

        int fdim, rc;

        char client_mess[client_message_SIZE];
        int req, fd, rcc, counter = 0;
            
          if ((fd = open(newf.c_str(), O_CREAT | O_WRONLY, S_IRWXU)) < 0)
          {
          perror("Cannot Open file path ");
          }
          write(fd, mess.c_str(), mess.length());

        printf("file size------------- %d \n" , length);                   
        while(length > 0){
            
            req = read(newSock, client_mess, client_message_SIZE);
            
            send_byte = ((length < client_message_SIZE) ? length : client_message_SIZE);
            
            if ((rcc=write(fd, client_mess, send_byte) <0))
            {
            perror("write failed");
            return(0);
            } 
            length -= send_byte;
            printf("file size------------- %d \n" , length);  
            if(req < 1000)
            {
                break;
            }            
        }
          if((rcc=close(fd)) < 0)
          {
          perror("close failed");
          return(0);
          }

    }


//        printf("head------------- %d-------%s \n" , length, client_mess); 

//        req = read(newSock, client_mess, client_message_SIZE);








        printf("\n client_message message: %s \n ", client_message);
        string requestType = getStr(message, ' ');  //Try to get the path which the client ask for
        message.erase(0, requestType.length()+1);
      //  printf("Client method: %s\n", requestType.c_str());
        string requestFile = getStr(message, ' ');
       // printf("Client ask for path: %s\n", requestFile.c_str());

        string requestF = requestFile; 
        string fileExt  = requestF.erase(0, getStr(requestF, '.').length()+1); 
        string fileEx   = getStr(fileExt, '/');
        requestFile = getStr(requestFile, '.')+"."+getStr(fileEx, '?');
        if(requestType== "GET" || requestType == "POST"){
            if(requestFile.length() <= 1){
                requestFile =  "/index.html";
            }
            if(getStr(fileEx, '?') == "php"){
                requestFile = "./public/"+requestFile;
                int n = requestFile.length();
                char char_array[n + 1];
                strcpy(char_array, requestFile.c_str());
                printf("requestng . %s\n",char_array);
                getData(requestType, client_message);
                requestFile = "/new.php";
                sem_wait(&mutex);
                if(openFile(char_array)){
                requestFile = "/new.php";}               
                else{
                    write(newSock, Messages[NOT_FOUND].c_str(), Messages[NOT_FOUND].length());
                    close(newSock);
                    sem_wait(&mutex);
                    thread_count--;
                    sem_post(&mutex);
                    pthread_exit(NULL);
                }sem_post(&mutex);
            }                
            send_message(newSock, requestFile, findFileExt(fileExt));     
        
        }
    }printf("\n------------------exiting server----------------------------------------------------\n");
    close(newSock);
    sem_wait(&mutex);
    thread_count--;
    sem_post(&mutex);
    pthread_exit(NULL);
}




int main(int argc, char const *argv[])
{
    sem_init(&mutex, 0, 1); 
    struct sockaddr_in server_address, client_address;
    int server_socket, client_socket, recvData, *thread_sock;
    int yes = 1;
    char ip4[INET_ADDRSTRLEN];

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket <= 0)
    {
        perror("In sockets");
        exit(EXIT_FAILURE);
    }
	memset(&server_address, 0, sizeof server_address); 
	server_address.sin_family=AF_INET;
	server_address.sin_addr.s_addr=htonl(INADDR_ANY);
	server_address.sin_port = htons(PORT);


	while (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address))<0)
	{   randomV = 8080 + (rand() % 10);
        memset(&server_address, 0, sizeof server_address); 
        server_address.sin_family=AF_INET;
        server_address.sin_addr.s_addr=htonl(INADDR_ANY);
        server_address.sin_port = htons(randomV);
        
	}

    if (listen(server_socket, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }

    while(1)
    { 
		socklen_t len= sizeof(client_address);
		printf("Listening Port: %d \n", randomV);
		
		client_socket = accept(server_socket,(struct sockaddr *)&client_address,&len);
		if(client_socket<0)
		{
			perror("Unable to accept connection");
			return 0;
		}
		else
		{
            inet_ntop(AF_INET, &(client_address.sin_addr), ip4, INET_ADDRSTRLEN);
			printf("Connected to: %s\n", ip4);
		}
        pthread_t multi_thread;
        thread_sock = new int(); 
        *thread_sock = client_socket;  

        if (pthread_create(&multi_thread, NULL, connection_handler, (void *)thread_sock) > 0) 
        {
            perror("Could not create thread");
            return 0;
        }           
	}

}