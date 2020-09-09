/****************************************/
/*  Global LCM                          */
/*  Wistron Corporation.                */
/****************************************/

typedef struct
{
	unsigned char data;
	unsigned char rw;
  	unsigned char rs;
} LCM_CMD_PACKET;


/* LCM Specification */
/* This example are use 16x2 LCM modules */
#define LCM_DISP_ROW_LEN        16      //LCM ROW display length
#define LCM_DISP_COL_LEN        2       //LCM COLUMN display length
#define LCM_ACTUALLY_ROW_LEN    40      //LCM ROW actually display length


/* LCM REGISTER SELECT definition */
#define	REG_SEL_CMD			0x00
#define	REG_SEL_DDRAM			0x01

/* LCM READ/WRITE definition */
#define	RW_READ				0x01
#define	RW_WRITE			0x00

/* LCM COMMAND definition */
#define CMD_CLR				0x01

#define CMD_CURSOR_HOME			0x02

#define CMD_ENTRY_MODE_SET		0x04
#define	CMD_ENTRY_MODE_SET_INC		0x02	/* Cursor Moving Inc bit */
#define CMD_ENTRY_MODE_SET_DISP		0x01	/* Display ON bit*/

#define CMD_DISP_CNTL			0x08
#define CMD_DISP_CTRL_D_ON		0x04	/* Display ON bit */
#define CMD_DISP_CTRL_C_ON		0x02	/* Cursor ON bit */
#define CMD_DISP_CTRL_B_ON		0x01	/* Blinking ON bit */

#define	CMD_CURSOR_DISP_SHIFT		0x10
#define CMD_CURSOR_DISP_SHIFT_DISP	0x08 	/* Display shift bit */
#define CMD_CURSOR_DISP_SHIFT_CURSOR	0x00	/* Cursor move bit */
#define	CMD_CURSOR_DISP_SHIFT_RIGHT	0x04	/* shift to right bit */
#define CMD_CURSOR_DISP_SHIFT_LEFT	0x00	/* shift to left bit */

#define CMD_FUNC_SET			0x20
#define CMD_FUNC_SET_DL_8		0x10	/* Data Width=8 bit */
#define CMD_FUNC_SET_DL_4		0x00	/* Data Width=4 bit */
#define	CMD_FUNC_SET_N_2		0x08	/* 2 lines bit */
#define	CMD_FUNC_SET_N_1		0x00	/* 1 line bit */
#define	CMD_FUNC_SET_F_11		0x04	/* 5x11 font bit */
#define	CMD_FUNC_SET_F_8		0x00	/* 5x8 font bit */ 

#define	CMD_SET_CGRAM_ADDR		0x40	
#define	CMD_SET_DDRAM_ADDR		0x80	


/* KEY status definition */
/* bit 7  6    5    4 */
/*     UP DOWN LEFT RIGHT */
#define UP_SHIFT			7
#define DOWN_SHIFT			6
#define	LEFT_SHIFT			5
#define RIGHT_SHIFT			4
#define PRESS_STATE			0
#define RELEASE_STATE			1
