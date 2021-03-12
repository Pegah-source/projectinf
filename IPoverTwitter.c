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
#include <jansson.h>
#include "encode.h"
#include "encode.c"

#define LENBUF 1024

static void run(char *cmd) {
  printf("Execute `%s`\n", cmd);
  if (system(cmd)) {
    perror(cmd);
    exit(1);
  }
}


int tun_alloc(char *dev, int flags) {

  struct ifreq ifr;
  int fd, err;
  char *clonedev = "/dev/net/tun";

  /* Arguments taken by the function:
   *
   * char *dev: the name of an interface (or '\0'). MUST have enough
   *   space to hold the interface name if '\0' is passed
   * int flags: interface flags (eg, IFF_TUN etc.)
   */

   /* open the clone device */
   if( (fd = open(clonedev, O_RDWR)) < 0 ) {
     return fd;
   }

   /* preparation of the struct ifr, of type "struct ifreq" */
   memset(&ifr, 0, sizeof(ifr));

   ifr.ifr_flags = flags;   /* IFF_TUN or IFF_TAP, plus maybe IFF_NO_PI */

   if (*dev) {
     /* if a device name was specified, put it in the structure; otherwise,
      * the kernel will try to allocate the "next" device of the
      * specified type */
     strncpy(ifr.ifr_name, dev, IFNAMSIZ);
   }

   /* try to create the device */
   if( (err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0 ) {
     close(fd);
     return err;
   }

  /* if the operation was successful, write back the name of the
   * interface to the variable "dev", so the caller can know
   * it. Note that the caller MUST reserve space in *dev (see calling
   * code below) */
  strcpy(dev, ifr.ifr_name);

  /* this is the special file descriptor that the caller will use to talk
   * with the virtual interface */
  return fd;
}

int tun_allocAlt() {
  struct ifreq ifr;
  int fd, e;

  if ((fd = open("/dev/net/tun", O_RDWR)) < 0) {
    perror("Cannot open /dev/net/tun");
    return fd;
  }
  memset(&ifr, 0, sizeof(ifr));

  ifr.ifr_flags = IFF_TUN | IFF_NO_PI;
  strncpy(ifr.ifr_name, "tun1", IFNAMSIZ);

  if ((e = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0) {
    perror("ioctl[TUNSETIFF]");
    close(fd);
    return e;
  }

  return fd;
}


int main(){
/*
  struct timeval tv;
  memset(&tv,0,sizeof(struct timeval));
  tv.tv_sec = 1;
  tv.tv_usec = 10;
*/
//
  size_t size = 2000;
  char words[size][40];
  next pointersToNext[size];
  size = readfromjson(size, words, pointersToNext);
//
  FILE *write_ptr;
//  write_ptr = fopen("test.bin", "w");
  uint8_t tun_buf[LENBUF];
//  unsigned char buff[LENBUF];
  bzero(tun_buf,LENBUF);
  int tun_fd;
  if ((tun_fd = tun_allocAlt())<0){
    perror("Error at tun allocation");
    exit(EXIT_FAILURE);
  }

//  write_ptr = fopen("test.bin","wb");
/*  
  run("sudo openvpn --mktun --dev tun1");
  run("sudo ip link set tun1 up");
  run("sudo ip addr add 10.0.0.1/24 dev tun1");
*/
int counter = 0;
  while(1){
	printf("hi pegah  ");
    counter++;
    if(counter==100) break;
//    FILE *write_ptr;
    write_ptr = fopen("readToTwitter1.txt","w");
    fd_set readset;
    FD_ZERO(&readset);
    FD_SET(tun_fd, &readset);

    if(select(tun_fd+1,&readset, NULL, NULL, NULL) == -1){
      perror("select error");
      exit(EXIT_FAILURE);
    }

    int r;
    if (FD_ISSET(tun_fd, &readset)) {
      r = read(tun_fd, tun_buf, LENBUF);
      
      if (r < 0) {
        // TODO: ignore some errno
        perror("read from tun_fd error");
        break;
      }
      printf("There are %d bytes in the packet\n", r);  
      int *temp = malloc(2*r*sizeof(int));      
      for(int i = 0; i<r; i++){
        (*(temp+2*i)) = (int)(*(tun_buf+i))/16; 
        (*(temp+2*i+1)) = (int)(*(tun_buf+i))%16;
      }
      for(int i = 0; i<r; i++){
        printf("%d ", *(temp+2*i));
        printf("%d ", *(temp+2*i+1));
        if (i == r-1){
          printf("\n");
        } 
      }
/*      int count = fwrite(temp, 2*r*sizeof(int), 1, write_ptr);
      if(count == 0){
        perror("write to file error\n");
      }
*/   
/* 
      for(int i = 0; i<r; i++){
        if (*(temp+i) < 8){
          fputs("c",write_ptr);
        }
        else fputs("a",write_ptr);
      }
      
*/    
      int numIter = 2*r/20;
      int res = 2*r%20;
      for (int i = 0; i < numIter; i++){
          writencode(size,words,pointersToNext,i,counter+100,20, temp+i*20);
          run("python3 postTweet.py");  
      }
      writencode(size, words, pointersToNext, numIter,counter+100, res, temp+numIter*20);
      run("python3 postTweet.py");
      fclose(write_ptr);
      free(temp);
//      int a = fwrite(tun_buf,1,r,write_ptr);
//      printf("Have written %d bytes into the file\n", a);
//      run("python3 postTweet.py");  
    }
    else{
      printf("If fails");
    }
//    free(temp);

  }
}

/*
Reference:  https://backreference.org/2010/03/26/tuntap-interface-tutorial/
            https://lxd.me/a-simple-vpn-tunnel-with-tun-device-demo-and-some-basic-concepts
*/
          /*
      fseek(write_ptr, 0L, SEEK_END);  
      long int length = ftell(write_ptr); 
      printf("Length of file is: %ld", length); 
*/
