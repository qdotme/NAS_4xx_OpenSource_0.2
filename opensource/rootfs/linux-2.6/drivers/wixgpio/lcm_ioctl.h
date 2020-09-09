
#ifndef LCM_IOCTLS_H
#define LCM_IOCTLS_H

typedef struct wix_lcm_ioctl_s
{
	unsigned char rs;
	unsigned char data;
} wix_lcm_ioctl_t;

// GPIO ioctls

#define WIX_IOCTL_BASE 0xDD
#define IOCTL_WIX_CMD_WRITE _IOWR(WIX_IOCTL_BASE, 0, wix_lcm_ioctl_t)   
#define IOCTL_WIX_CMD_READ  _IOWR(WIX_IOCTL_BASE, 1, wix_lcm_ioctl_t)



#endif


