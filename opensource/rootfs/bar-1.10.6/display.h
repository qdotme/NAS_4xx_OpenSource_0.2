/*
 * Display
 */

#ifndef __display_h__
#define __display_h__

#include "headers.h"
#include "types.h"
#include "io.h"

struct _display {
	time_t start_time;
	time_t total_time;
	time_t current_time;
	time_t elapsed_time;
	float percent_complete;
	int display_interval;
	int overtime_flag;
	unsigned int k;
	char twiddle;
	int screen_width;
	int manual_width;
	int screen_width_minus_one;
	int display_twiddle;
	int display_title;
	int display_datacount;
	int display_throughput;
	int display_time;
	int display_elapsed_only;
	int display_percent;
	int display_bar;
	int display_summary;
	int display_ansi;
	int display_throughput_bits;
	int display_count_bits;
	char title[81];
	char *space_bg_color;
	char *twiddle_fg_color;
	char *twiddle_bg_color;
	int twiddle_fg_bold;
	char *title_bg_color;
	char *title_fg_color;
	int title_fg_bold;
	char *datacount_fg_color;
	char *datacount_bg_color;
	int datacount_fg_bold;
	char *throughput_label_fg_color;
	char *throughput_label_bg_color;
	int throughput_label_fg_bold;
	char *throughput_fg_color;
	char *throughput_bg_color;
	int throughput_fg_bold;
	char *time_label_fg_color;
	char *time_label_bg_color;
	int time_label_fg_bold;
	char *time_fg_color;
	char *time_bg_color;
	int time_fg_bold;
	char *percent_fg_color;
	char *percent_bg_color;
	int percent_fg_bold;
	char *bar_fg_color;
	char *bar_bg_color;
	int bar_fg_bold;
	char *barbrace_fg_color;
	char *barbrace_bg_color;
	int barbrace_fg_bold;
	int total_display_percent;
};

typedef struct _display display;

extern display d;

int displayInit(void);
int displayBegin(void);
int displayUpdate(void);
int displayEnd(void);

#endif
