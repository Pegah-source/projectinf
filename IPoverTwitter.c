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
#define LENBUF 1024

int tun_alloc() {

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

    ifr.ifr_flags = IFF_TUN | IFF_NO_PI;   /* IFF_TUN or IFF_TAP, plus maybe IFF_NO_PI */
    /* Flags: IFF_TUN   - TUN device (no Ethernet headers) 
       *      IFF_TAP   - TAP device  
       *
       *      IFF_NO_PI - Do not provide packet information  
       */ 

//   if (*dev) {
     /* if a device name was specified, put it in the structure; otherwise,
      * the kernel will try to allocate the "next" device of the
      * specified type */
//as you have commented here i don't think that we would need an if and also i can not see any dev defined before this line.
    strncpy(ifr.ifr_name, "tun0", IFNAMSIZ);
//   }

   /* try to create the device */
   if( (err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0 ) {
     perror("ioctl[TUNSETIFF]");
     close(fd);
     return err;
   }

  /* if the operation was successful, write back the name of the
   * interface to the variable "dev", so the caller can know
   * it. Note that the caller MUST reserve space in *dev (see calling
   * code below) */
//  strcpy(dev, ifr.ifr_name);

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
  strncpy(ifr.ifr_name, "tun0", IFNAMSIZ);

  if ((e = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0) {
    perror("ioctl[TUNSETIFF]");
    close(fd);
    return e;
  }

  return fd;
}


int main(){
  struct timeval tv;
  memset(&tv,0,sizeof(struct timeval));
  tv.tv_sec = 1;
  tv.tv_usec = 10;
  char tun_buf[LENBUF];
  bzero(tun_buf,LENBUF);
  int tun_fd;
  if ((tun_fd = tun_allocAlt())<0){
    perror("Error at tun allocation");
    exit(EXIT_FAILURE);
  }
  while(1){
//    printf("Hello\n");
//    printf("half way!\n");
    fd_set readset;
    FD_ZERO(&readset);
    FD_SET(tun_fd, &readset);

    printf("I'm pissed\n");

    if(select(tun_fd+10,&readset, NULL, NULL, NULL) == -1){//i guess here you wanted to use the timeval tv that you have defined earlier
      //refrencing to the line : retval = select(1, &rfds, NULL, NULL, &tv); it will be like this : select(tun_fd+10,&readset, NULL, NULL, &tv) == -1
      // i don't get the first argument. but if we reference to this :nfds is the highest-numbered file descriptor in any of the three sets, plus 1.
      // it sould probably be one
      perror("select error");
      exit(EXIT_FAILURE);
    }

    int r;
    if (FD_ISSET(tun_fd, &readset)) {
      printf("In FD_ISSET");
      r = read(tun_fd, tun_buf, LENBUF);
      if (r < 0) {
        // TODO: ignore some errno
        perror("read from tun_fd error");
        break;
      }
    }
    else{
      printf("If fails");
    }
    printf("%s\n", tun_buf);
    printf("End\n");
  }
}

/*
Reference:  https://backreference.org/2010/03/26/tuntap-interface-tutorial/
            https://lxd.me/a-simple-vpn-tunnel-with-tun-device-demo-and-some-basic-concepts
*/
          
