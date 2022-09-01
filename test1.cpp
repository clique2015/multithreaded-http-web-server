#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <bits/stdc++.h>
using namespace std;
int main(int argc, char *argv[])
{
  int fd;
  int rc;
  char name[]="./test.dat";
  char data[]="abcdefghijk";
  int oflag1 = O_CREAT | O_WRONLY ;
  int oflag2 = O_RDWR;
  mode_t mode = S_IRUSR | S_IWUSR | S_IXUSR;
  unsigned int file_ccsid = 819;
  unsigned int open_ccsid = 37;

  /***************************************************/
  /*  First create the file with the CCSID 819.      */
  /***************************************************/
string mess = "client_message";
    int found = mess.find("m");
    if(found != string::npos){
       // mess.erase(0, found+15);
        string newf = "./text.txt";
        int fdim, rc, counter=0;


        
          if ((fd = open(name,oflag1, S_IRWXU)) < 0)
          {
          perror("Cannot Open file path ");
          } 
        while(counter < 10){
          if ((rc=write(fd, data, strlen(data)) <0))
          {
            perror("write failed");
            return(0);
          }        
          counter++;
        }
          if((rc=close(fd)) < 0)
          {
          perror("close failed");
          return(0);
          }

    }
  }
 /** if ((fd=open(name,oflag1, S_IRWXU)) < 0)
  {
    perror("open() for create failed");
    return(0);
  } 

  /***************************************************/
  /* Write some EBCDIC data.                         */
  /***************************************************/
/**
  if (rc=write(fd, data, strlen(data)) <0)
  {
    perror("write failed");
    return(0);
  }

  if (0 != (rc=close(fd)))
  {
    perror("close failed");
    return(0);
  }
}

/**
<?php
  $servername = "localhost";
  $username   = "chatserver";
  $password ="password";

mysqli_connect($servername, $username, $password, "myDB");
 $sql = "CREATE DATABASE myDB user  $password";
 $sql = "DELETE DATABASE myDB $username  password"; 
$sql = "DELETE TABLE MyGuests"; 

 $sql = "CREATE TABLE MyGuests (
cc,
firstname,
lastname,
email,
emaiwwwl
)"; 
      setcookie("user", $POST[username], 24);
      header("/home.html");
 $sql = "INSERT INTO MyGuests (cc, lastname, email, lastname, emaiwwwl)
VALUES (John, 'Doe', 'john@example.com', firstname, $username )";
$ss = "John";
$sql = "UPDATE MyGuests SET lastname='$password' WHERE cc = '$ss' LIMIT 5 OFFSET -2";
$John = "Doess";
$sql = "DELETE FROM MyGuests WHERE lastname='$John' OR email='john' LIMIT 4";

 $sql = "SELECT id, firstname, lastname FROM MyGuests LIMIT 4";

  while($row = mysqli_query($sql)) {
    echo "id: ". $row[id]. "<br>" . $row[firstname]. " ---" . $row[email]. "name";
  }

?>
**/