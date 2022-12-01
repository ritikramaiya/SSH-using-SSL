#include <errno.h>
#include<stdio.h>
#include<dirent.h>
#include<stdlib.h>
#include<string.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <resolv.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#define MAX_PENDING 5
#define FAIL -1
#ifndef MAX_BUF
#define MAX_BUF 200
#endif
char* _ls(const char *dir,int op_a,int op_l)
{
	//Here we will list the directory
	struct dirent *d;
	DIR *dh = opendir(dir);
	if (!dh)
	{
		if (errno = ENOENT)
		{
			//If the directory is not found
			perror("Directory doesn't exist");
		}
		else
		{
			//If the directory is not readable then throw error and exit
			perror("Unable to read directory");
		}
		exit(EXIT_FAILURE);
	}
	char *str2=malloc(256);
	//While the next entry is not readable we will print directory files
	while ((d = readdir(dh)) != NULL)
	{
		//If hidden files are found we continue
		if (!op_a && d->d_name[0] == '.')
			continue;
		char *str1=malloc(20);
		//printf("%s  ", d->d_name);
		strcpy(str1,d->d_name);
		strcat(str2,str1);
		strcat(str2," ");
	
		//if(op_l) printf("\n");
	}
		//printf("\nstr 2 %s  ", str2);
	//if(!op_l)
	//printf("\n");
	return str2;
}

int main(int argc, char * argv[])
{
   struct sockaddr_in sin;
   char buf[1024]; char buf2[1024];
   int s, new_s,sd;
   int  bytes;
   SSL_METHOD *method;
   SSL_CTX *ctx;
   SSL_library_init();
   OpenSSL_add_all_algorithms();
   SSL_load_error_strings();
   method=TLSv1_2_server_method();
   ctx=SSL_CTX_new(method);
   SSL_CTX_use_certificate_file(ctx, "cert.pem", SSL_FILETYPE_PEM);   
     SSL_CTX_use_PrivateKey_file(ctx, "key.pem", SSL_FILETYPE_PEM); 
    
    if ( !SSL_CTX_check_private_key(ctx) )
    {
        fprintf(stderr, "key phrase does not match\n");
        abort();
    }
   bzero((char *)&sin, sizeof(sin));
   sin.sin_family = AF_INET;
   sin.sin_addr.s_addr = INADDR_ANY;
   sin.sin_port = htons(atoi(argv[1]));
   if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
      perror("simplex-talk: socket");
      exit(1);
   }
   if ((bind(s, (struct sockaddr *)&sin, sizeof(sin))) < 0) {
      perror("simplex-talk: bind:");
      exit(1);
   } 
   listen(s, MAX_PENDING);
   while(1) {
        
        socklen_t len = sizeof(sin);
        SSL *ssl;
        new_s = accept(s, (struct sockaddr*)&sin, &len);
         
      
         
        printf("Connection established with : %s:%d\n",inet_ntoa(sin.sin_addr), ntohs(sin.sin_port));
        ssl = SSL_new(ctx);              
        SSL_set_fd(ssl, new_s);
         
        if ( SSL_accept(ssl) == FAIL ){     
        ERR_print_errors_fp(stderr);
        }
        else{
     	char path[MAX_BUF];	

     		while (bytes = SSL_read(ssl, buf, sizeof(buf))){
     			buf[bytes] = '\0';
     			if(strcmp(buf,"ls\n")==0){
     			const char* test= _ls(".",0,0); 
     			   SSL_write(ssl, test, strlen(test));      
         		printf("list of files : %s\n",test);
     			}
     			else if(strcmp(buf,"pwd\n")==0){
     			
   			getcwd(path, MAX_BUF);
   			SSL_write(ssl, path, strlen(path));
     			printf("message from sender : %s\n",path);
         		}
         		else if(strcmp(buf,"exit\n")==0){
         		//printf("exit command in loop");
         		SSL_write(ssl, buf, strlen(buf));
         		exit(0);
         		break;
         		}
         		else{
         		printf("invalid command\n");
         	SSL_write(ssl, "invalid command", strlen("invalid command"));
         		}
         		//printf("message from sender : %s",buf);
         		 //SSL_write(ssl, buf, strlen(buf));
         		}
         		
      		close(new_s);
        }	
           

   sd = SSL_get_fd(ssl);      
    SSL_free(ssl);        
    close(new_s);
   }
    
   close(s);
   SSL_CTX_free(ctx);
}
