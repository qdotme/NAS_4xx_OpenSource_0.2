#ifndef CCISS_H_
#define CCISS_H_

#define CCISS_H_CVSID "$Id: cciss.h,v 1.1.1.1.4.2 2008/11/26 07:31:03 wiley Exp $\n"

int cciss_io_interface(int device, int target,
			      struct scsi_cmnd_io * iop, int report);

#endif /* CCISS_H_ */
