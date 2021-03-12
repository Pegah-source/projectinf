#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <netdb.h>
#include <fcntl.h>
#include <signal.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include "decode.c"
#include "decode.h"
#include "toBinary.c"
#define LENBUF 1024
int tun_allocAlt() {
  struct ifreq ifr;
  int fd, e;

  if ((fd = open("/dev/net/tun", O_RDWR)) < 0) {
    perror("Cannot open /dev/net/tun");
    return fd;
  }
  memset(&ifr, 0, sizeof(ifr));

  ifr.ifr_flags = IFF_TUN | IFF_NO_PI;
  strncpy(ifr.ifr_name, "tun2", IFNAMSIZ);

  if ((e = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0) {
    perror("ioctl[TUNSETIFF]");
    close(fd);
    return e;
  }

  return fd;
}

int main(){
    FILE *rptr;
    uint8_t buff[LENBUF];
    bzero(buff,LENBUF);
    fd_set writefd;
    int tun_fd;
    if ((tun_fd = tun_allocAlt())<0){
      perror("Error at tun allocation");
      exit(EXIT_FAILURE);
    }
// New code
    //run("python3 downloadTweet.py");

	size_t size = 2000;
	char words[size][40];
	next pointersToNext[size];
	size = readfromjson(size, words, pointersToNext);
	char* decoded = decodetxt(size,words,pointersToNext);
	toBinary(decoded,strlen(decoded));
//
    FILE *rptr2;
    rptr2 = fopen("test.bin", "rb");

//    fwrite(tun_buf,1,r,write_ptr);
    fseek(rptr2, 0L, SEEK_END);  
    size_t length = ftell(rptr2);
    fclose(rptr2);
//    unsigned char *buff = malloc((size_t)length);
//    printf("Length: %ld\n", length);

// End of new code


    rptr = fopen("test.bin", "rb");
    
    if(rptr == NULL){
        perror("Opening file error");
    }
    int a = fread(buff, 1, length, rptr);
    printf("%d\n", a);

    FD_ZERO(&writefd);
    FD_SET(tun_fd,&writefd);
    if(select(tun_fd+1, NULL, &writefd, NULL, NULL) == -1){
      perror("select write");
    }

    if(FD_ISSET(tun_fd, &writefd)){
      int r = write(tun_fd, buff, length);
      if (r < 0){
        perror("write to file descriptor error");
      }
    }
//    fwrite(tun_buf,1,r,write_ptr);
//    fseek(rptr, 0L, SEEK_END);  
//    size_t length = ftell(rptr);
//    unsigned char *buff = malloc((size_t)length);
//    printf("Length: %ld\n", length);


    for(int i = 0; i<length; i++){
        printf("%u ", *(buff+i));  
    }
    
//    free(buff);
    fclose(rptr);
}
