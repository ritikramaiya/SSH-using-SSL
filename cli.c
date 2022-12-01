#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <sys/socket.h>
#include <resolv.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <strings.h>

#define FAIL    -1
int main(int argc, char * argv[])
{   SSL_CTX *ctx;
    SSL *ssl;
    int sd;
    char buf1[1024]; char buf2[1024]; int bytes;
    if ( argc != 3 )
    {
        printf("please enter the command as follows :%s <hostname> <portnum>\n", argv[0]);
        exit(0);
    }
    SSL_library_init();
    SSL_METHOD *method;    
    OpenSSL_add_all_algorithms();  
    SSL_load_error_strings();   
    method = TLSv1_2_client_method(); 
    ctx = SSL_CTX_new(method);   
    struct hostent *host;
    struct sockaddr_in sin;

    if ( (host = gethostbyname(argv[1])) == NULL )
    {
        perror(argv[1]);
        abort();
    }
    
   bzero((char *)&sin, sizeof(sin));
   sin.sin_family = AF_INET;
   bcopy(host->h_addr, (char *)&sin.sin_addr, host->h_length);
   sin.sin_port = htons(atoi(argv[2]));
   if ((sd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
      perror("simplex-talk: socket");
      exit(1);
   }
   if (connect(sd, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
      perror("simplex-talk: connect");
      close(sd);
      exit(1);
   }
    ssl = SSL_new(ctx);      
    SSL_set_fd(ssl, sd);    
    if ( SSL_connect(ssl) == FAIL )  
        ERR_print_errors_fp(stderr);
    else
    {   

        printf("Connected to the server,please type command\n");
        
	 while (fgets(buf1, sizeof(buf1), stdin)) {
	 
      		 
      		 char s[100]; int rc; int server_sock; char cwd[256];
      		//fgets(s,100,stdin);
      		 //s[strlen(s)-1]='\0'; 
                //echolen = strlen(s)
    
                SSL_write(ssl, buf1, strlen(buf1)); 
      		//buf1[1024]='\0';
      		
      	//	if(strcmp(buf,"exit\n")==0)    		  
   // {
    //rc=close(server_sock); 
    //exit(0);
   // printf("debugging");
   // break;
   // }
    
    bytes = SSL_read(ssl, buf2, sizeof(buf2));
    buf2[bytes] = '\0';
    if(strcmp(buf2,"exit\n")==0)    		  
    {
    //rc=close(server_sock); 
    //exit(0);
   //printf("debugging");
   exit(0);
   break;
    }
    
    printf("%s\n ",buf2);
           }
        buf1[1024]='\0';
        SSL_free(ssl);       
    }
           
    
    close(sd);        
    SSL_CTX_free(ctx);      
    return 0;
}
