#include "config.h"

#include "headers.h"
#include "error.h"
#include "fd.h"
#include "io.h"
#include "display.h"
#include "args.h"

#ifndef HAVE_SPRINTF
#	error *** ERROR: This system does not have sprintf()
#endif

struct _options_list {
	char *short_option1;
	char *short_option2;
	char *long_option1;
	char *long_option2;
	char *rc_option;
	char *arg_description;
	char *description;
	int (*cl_func)(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num);
	int (*rc_func)(FILE *ferr, char *filename, int line, char *tag, char *value);
};

typedef struct _options_list options_list;

struct _colors_list {
	char *name;
	char *code;
};

typedef struct _colors_list colors_list;

/* <Gasp!>  Another global variable!  And a shameless hack at that! */
char _parsing_blocks = 0;

int parse_infile_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num);
int parse_outfile_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num);
int parse_size_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num);
int parse_throttle_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num);
int parse_buffer_size_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num);
int parse_block_size_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num);
int parse_interval_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num);
int parse_timeout_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num);
int parse_kilo_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num);
int parse_width_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num);
int parse_minus_one_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num);
int parse_twiddle_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num);
int parse_count_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num);
int parse_throughput_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num);
int parse_time_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num);
int parse_elapsed_only_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num);
int parse_percent_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num);
int parse_bar_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num);
int parse_summary_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num);
int parse_ansi_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num);
int parse_displays_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num);
int parse_help_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num);
int parse_version_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num);
int parse_throughput_bits_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num);
int parse_count_bits_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num);
int parse_title_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num);
int parse_display_title_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num);
int parse_total_percent_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num);

int parse_space_bg_color_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num);
int parse_twiddle_fg_color_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num);
int parse_twiddle_bg_color_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num);
int parse_twiddle_fg_bold_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num);
int parse_title_fg_color_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num);
int parse_title_bg_color_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num);
int parse_title_fg_bold_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num);
int parse_count_fg_color_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num);
int parse_count_bg_color_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num);
int parse_count_fg_bold_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num);
int parse_throughput_label_fg_color_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num);
int parse_throughput_label_bg_color_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num);
int parse_throughput_label_fg_bold_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num);
int parse_throughput_fg_color_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num);
int parse_throughput_bg_color_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num);
int parse_throughput_fg_bold_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num);
int parse_time_label_fg_color_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num);
int parse_time_label_bg_color_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num);
int parse_time_label_fg_bold_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num);
int parse_time_fg_color_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num);
int parse_time_bg_color_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num);
int parse_time_fg_bold_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num);
int parse_percent_fg_color_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num);
int parse_percent_bg_color_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num);
int parse_percent_fg_bold_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num);
int parse_barbrace_fg_color_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num);
int parse_barbrace_bg_color_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num);
int parse_barbrace_fg_bold_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num);
int parse_bar_fg_color_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num);
int parse_bar_bg_color_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num);
int parse_bar_fg_bold_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num);

int parse_throttle_rc(FILE *ferr, char *filename, int line, char *tag, char *value);
int parse_buffer_size_rc(FILE *ferr, char *filename, int line, char *tag, char *value);
int parse_block_size_rc(FILE *ferr, char *filename, int line, char *tag, char *value);
int parse_interval_rc(FILE *ferr, char* filename, int line, char *tag, char *value);
int parse_timeout_rc(FILE *ferr, char* filename, int line, char *tag, char *value);
int parse_kilo_rc(FILE *ferr, char* filename, int line, char *tag, char *value);
int parse_width_rc(FILE *ferr, char* filename, int line, char *tag, char *value);
int parse_minus_one_rc(FILE *ferr, char* filename, int line, char *tag, char *value);
int parse_twiddle_rc(FILE *ferr, char* filename, int line, char *tag, char *value);
int parse_count_rc(FILE *ferr, char* filename, int line, char *tag, char *value);
int parse_throughput_rc(FILE *ferr, char* filename, int line, char *tag, char *value);
int parse_time_rc(FILE *ferr, char* filename, int line, char *tag, char *value);
int parse_elapsed_only_rc(FILE *ferr, char* filename, int line, char *tag, char *value);
int parse_percent_rc(FILE *ferr, char* filename, int line, char *tag, char *value);
int parse_bar_rc(FILE *ferr, char* filename, int line, char *tag, char *value);
int parse_summary_rc(FILE *ferr, char* filename, int line, char *tag, char *value);
int parse_ansi_rc(FILE *ferr, char* filename, int line, char *tag, char *value);
int parse_throughput_bits_rc(FILE *ferr, char* filename, int line, char *tag, char *value);
int parse_count_bits_rc(FILE *ferr, char* filename, int line, char *tag, char *value);
int parse_title_rc(FILE *ferr, char* filename, int line, char *tag, char *value);
int parse_display_title_rc(FILE *ferr, char* filename, int line, char *tag, char *value);
int parse_total_percent_rc(FILE *ferr, char* filename, int line, char *tag, char *value);

int parse_space_bg_color_rc(FILE *ferr, char *filename, int line, char *tag, char *value);
int parse_twiddle_fg_color_rc(FILE *ferr, char *filename, int line, char *tag, char *value);
int parse_twiddle_bg_color_rc(FILE *ferr, char *filename, int line, char *tag, char *value);
int parse_twiddle_fg_bold_rc(FILE *ferr, char *filename, int line, char *tag, char *value);
int parse_title_fg_color_rc(FILE *ferr, char *filename, int line, char *tag, char *value);
int parse_title_bg_color_rc(FILE *ferr, char *filename, int line, char *tag, char *value);
int parse_title_fg_bold_rc(FILE *ferr, char *filename, int line, char *tag, char *value);
int parse_count_fg_color_rc(FILE *ferr, char *filename, int line, char *tag, char *value);
int parse_count_bg_color_rc(FILE *ferr, char *filename, int line, char *tag, char *value);
int parse_count_fg_bold_rc(FILE *ferr, char *filename, int line, char *tag, char *value);
int parse_throughput_label_fg_color_rc(FILE *ferr, char *filename, int line, char *tag, char *value);
int parse_throughput_label_bg_color_rc(FILE *ferr, char *filename, int line, char *tag, char *value);
int parse_throughput_label_fg_bold_rc(FILE *ferr, char *filename, int line, char *tag, char *value);
int parse_throughput_fg_color_rc(FILE *ferr, char *filename, int line, char *tag, char *value);
int parse_throughput_bg_color_rc(FILE *ferr, char *filename, int line, char *tag, char *value);
int parse_throughput_fg_bold_rc(FILE *ferr, char *filename, int line, char *tag, char *value);
int parse_time_label_fg_color_rc(FILE *ferr, char *filename, int line, char *tag, char *value);
int parse_time_label_bg_color_rc(FILE *ferr, char *filename, int line, char *tag, char *value);
int parse_time_label_fg_bold_rc(FILE *ferr, char *filename, int line, char *tag, char *value);
int parse_time_fg_color_rc(FILE *ferr, char *filename, int line, char *tag, char *value);
int parse_time_bg_color_rc(FILE *ferr, char *filename, int line, char *tag, char *value);
int parse_time_fg_bold_rc(FILE *ferr, char *filename, int line, char *tag, char *value);
int parse_percent_fg_color_rc(FILE *ferr, char *filename, int line, char *tag, char *value);
int parse_percent_bg_color_rc(FILE *ferr, char *filename, int line, char *tag, char *value);
int parse_percent_fg_bold_rc(FILE *ferr, char *filename, int line, char *tag, char *value);
int parse_barbrace_fg_color_rc(FILE *ferr, char *filename, int line, char *tag, char *value);
int parse_barbrace_bg_color_rc(FILE *ferr, char *filename, int line, char *tag, char *value);
int parse_barbrace_fg_bold_rc(FILE *ferr, char *filename, int line, char *tag, char *value);
int parse_bar_fg_color_rc(FILE *ferr, char *filename, int line, char *tag, char *value);
int parse_bar_bg_color_rc(FILE *ferr, char *filename, int line, char *tag, char *value);
int parse_bar_fg_bold_rc(FILE *ferr, char *filename, int line, char *tag, char *value);

options_list options[] = {
	{
		"-if", 0, "--in-file", 0, 0, "<file>",
		"Read input from <file>.  Default: stdin",
		parse_infile_commandline,
		0
	},
	{
		"-of", 0, "--out-file", 0, 0, "<file>",
		"Write output to <file>.  Default: stdout",
		parse_outfile_commandline,
		0
	},
	{
		"-s", 0, "--size", 0, 0, "<size>",
		"Expect an input stream of <size> bytes.",
		parse_size_commandline,
		0
	},
	{
		"-th", 0, "--throttle", 0, "throttle", "<rate>",
		"Throttle I/O rate to <rate> bytes per second.",
		parse_throttle_commandline,
		parse_throttle_rc,
	},
	{
		"-bs", 0, "--buffer-size", 0, "buffer-size", "<size>",
		"Allocate an I/O buffer of <size> bytes.  Default: 1024",
		parse_buffer_size_commandline,
		parse_buffer_size_rc
	},
	{
		"-bl", 0, "--block-size", 0, "block-size", "<size>",
		"Assume blocks of <size> bytes when parsing sizes in blocks.  Default: 1024",
		parse_block_size_commandline,
		parse_block_size_rc
	},
	{
		"-i", 0, "--interval", 0, "interval", "<secs>",
		"Update the display every <secs> seconds.  Default: 1",
		parse_interval_commandline,
		parse_interval_rc
	},
	{
		"-t", 0, "--timeout", 0, "timeout", "<usecs>",
		"Wait <usecs> microseconds for a change in I/O.  Default: 250000",
		parse_timeout_commandline,
		parse_timeout_rc
	},
	{
		"-k", 0, "--kilo", 0, "kilobyte", "1000|1024",
		"Use 1000 or 1024 as the size of a kilobyte.  Default: 1024",
		parse_kilo_commandline,
		parse_kilo_rc
	},
	{
		"-sw", 0, "--screen-width", 0, "screen-width", "<width>",
		"Assume a screen width of <width> characters.  Default: 80",
		parse_width_commandline,
		parse_width_rc
	},
	{
		"-sw-1", "-sw-0", "--screen-width-minus-one", "--screen-width-minus-zero", "screen-width-minus-one", 0,
		"Use one character less than the maximum screen width.  Default: off",
		parse_minus_one_commandline,
		parse_minus_one_rc
	},
	{
		"-ti", 0, "--title", 0, "title", "<string>",
		"Title string to be displayed in the status line.",
		parse_title_commandline,
		parse_title_rc
	},
	{
		"-dti", "-nti", "--display-title", "--no-title", "display-title", 0,
		"Display title string.  Default: on",
		parse_display_title_commandline,
		parse_display_title_rc
	},
	{
		"-dtw", "-ntw", "--display-twiddle", "--no-twiddle", "display-twiddle", 0,
		"Turn on/off the twiddle in the display.  Default: on",
		parse_twiddle_commandline,
		parse_twiddle_rc
	},
	{
		"-dc", "-nc", "--display-count", "--no-count", "display-count", 0,
		"Turn on/off the data count in the display.  Default: on",
		parse_count_commandline,
		parse_count_rc
	},
	{
		"-dcb", "-ncb", "--display-count-bits", "--no-count-bits", "display-count-bits", 0,
		"Display the data count as bits instead of bytes.  Default: off",
		parse_count_bits_commandline,
		parse_count_bits_rc,
	},
	{
		"-dth", "-nth", "--display-throughput", "--no-throughput", "display-throughput", 0,
		"Turn on/off the data throughput in the display.  Default: on",
		parse_throughput_commandline,
		parse_throughput_rc
	},
	{
		"-dthb", "-nthb", "--display-throughput-bits", "--no-throughput-bits", "display-throughput-bits", 0,
		"Display throughput as bits/sec instead of bytes/sec.  Default: off",
		parse_throughput_bits_commandline,
		parse_throughput_bits_rc,
	},
	{
		"-dt", "-nt", "--display-time", "--no-time", "display-time", 0,
		"Turn on/off the time in the display.  Default: on",
		parse_time_commandline,
		parse_time_rc
	},
	{
		"-de", "-ne", "--display-elapsed-only", "--no-elapsed-only", "display-elapsed-only", 0,
		"Turn on/off displaying time as elapsed only (no eta).  Default: off",
		parse_elapsed_only_commandline,
		parse_elapsed_only_rc
	},
	{
		"-dp", "-np", "--display-percent", "--no-percent", "display-percent", 0,
		"Turn on/off the percent complete in the display.  Default: on",
		parse_percent_commandline,
		parse_percent_rc
	},
	{
		"-db", "-nb", "--display-bar", "--no-bar", "display-bar", 0,
		"Turn on/off the progress bar in the display.  Default: on",
		parse_bar_commandline,
		parse_bar_rc
	},
	{
		"-dtp", "-ntp", "--display-total-percent", "--no-total-percent", "display-total-percent", 0,
		"Turn on/off the percent of expected data in the summary.  Default: on",
		parse_total_percent_commandline,
		parse_total_percent_rc
	},
	{
		"-ds", "-ns", "--display-summary", "--no-summary", "display-summary", 0,
		"Turn on/off the summary information when complete.  Default: on",
		parse_summary_commandline,
		parse_summary_rc
	},
	{
		"-da", "-dn", "--display-all", "--display-none", 0, 0,
		"Turn on/off all displays.  Default: all on",
		parse_displays_commandline,
		0
	},
	{
		"-dan", "-nan", "--display-ansi", "--no-ansi", "display-ansi", 0,
		"Turn on/off the use of ansi color codes in the display.  Default is: off",
		parse_ansi_commandline,
		parse_ansi_rc
	},

	{
		"-spbg", 0, "--space-background", 0, "space-background", "<color>",
		"Use <color> as the background for spacing between display objects.",
		parse_space_bg_color_commandline,
		parse_space_bg_color_rc
	},

	{
		"-twfg", 0, "--twiddle-foreground", 0, "twiddle-foreground", "<color>",
		"Use <color> as the twiddle foreground.",
		parse_twiddle_fg_color_commandline,
		parse_twiddle_fg_color_rc
	},
	{
		"-twbg", 0, "--twiddle-background", 0, "twiddle-background", "<color>",
		"Use <color> as the twiddle background.",
		parse_twiddle_bg_color_commandline,
		parse_twiddle_bg_color_rc
	},
	{
		"-twb", "-twn", "--twiddle-bold", "--twiddle-normal", "twiddle-bold", 0,
		"Display the twiddle ansi foreground color in bold or normal.",
		parse_twiddle_fg_bold_commandline,
		parse_twiddle_fg_bold_rc
	},

	{
		"-tifg", 0, "--title-foreground", 0, "title-foreground", "<color>",
		"Use <color> as the title foreground.",
		parse_title_fg_color_commandline,
		parse_title_fg_color_rc
	},
	{
		"-tibg", 0, "--title-background", 0, "title-background", "<color>",
		"Use <color> as the title background.",
		parse_title_bg_color_commandline,
		parse_title_bg_color_rc
	},
	{
		"-tib", "-tin", "--title-bold", "--title-normal", "title-bold", 0,
		"Display the title ansi foreground color in bold or normal.",
		parse_title_fg_bold_commandline,
		parse_title_fg_bold_rc
	},

	{
		"-cfg", 0, "--count-foreground", 0, "count-foreground", "<color>",
		"Use <color> as the count foreground.",
		parse_count_fg_color_commandline,
		parse_count_fg_color_rc
	},
	{
		"-cbg", 0, "--count-background", 0, "count-background", "<color>",
		"Use <color> as the count background.",
		parse_count_bg_color_commandline,
		parse_count_bg_color_rc
	},
	{
		"-cb", "-cn", "--count-bold", "--count-normal", "count-bold", 0,
		"Display the count ansi foreground color in bold or normal.",
		parse_count_fg_bold_commandline,
		parse_count_fg_bold_rc
	},

	{
		"-thfg", 0, "--throughput-foreground", 0, "throughput-foreground", "<color>",
		"Use <color> as the throughput foreground.",
		parse_throughput_fg_color_commandline,
		parse_throughput_fg_color_rc
	},
	{
		"-thbg", 0, "--throughput-background", 0, "throughput-background", "<color>",
		"Use <color> as the throughput background.",
		parse_throughput_bg_color_commandline,
		parse_throughput_bg_color_rc
	},
	{
		"-thb", "-thn", "--throughput-bold", "--throughput-normal", "throughput-bold", 0,
		"Display the throughput ansi foreground color in bold or normal.",
		parse_throughput_fg_bold_commandline,
		parse_throughput_fg_bold_rc
	},

	{
		"-thlfg", 0, "--throughput-label-foreground", 0, "throughput-label-foreground", "<color>",
		"Use <color> as the throughput label foreground.",
		parse_throughput_label_fg_color_commandline,
		parse_throughput_label_fg_color_rc
	},
	{
		"-thlbg", 0, "--throughput-label-background", 0, "throughput-label-background", "<color>",
		"Use <color> as the throughput label background.",
		parse_throughput_label_bg_color_commandline,
		parse_throughput_label_bg_color_rc
	},
	{
		"-thlb", "-thln", "--throughput-label-bold", "--throughput-label-normal", "throughput-label-bold", 0,
		"Display the throughput label ansi foreground color in bold or normal.",
		parse_throughput_label_fg_bold_commandline,
		parse_throughput_label_fg_bold_rc
	},

	{
		"-tfg", 0, "--time-foreground", 0, "time-foreground", "<color>",
		"Use <color> as the time foreground.",
		parse_time_fg_color_commandline,
		parse_time_fg_color_rc
	},
	{
		"-tbg", 0, "--time-background", 0, "time-background", "<color>",
		"Use <color> as the time background.",
		parse_time_bg_color_commandline,
		parse_time_bg_color_rc
	},
	{
		"-tb", "-tn", "--time-bold", "--time-normal", "time-bold", 0,
		"Display the time ansi foreground color in bold or normal.",
		parse_time_fg_bold_commandline,
		parse_time_fg_bold_rc
	},

	{
		"-tlfg", 0, "--time-label-foreground", 0, "time-label-foreground", "<color>",
		"Use <color> as the time label foreground.",
		parse_time_label_fg_color_commandline,
		parse_time_label_fg_color_rc
	},
	{
		"-tlbg", 0, "--time-label-background", 0, "time-label-background", "<color>",
		"Use <color> as the time label background.",
		parse_time_label_bg_color_commandline,
		parse_time_label_bg_color_rc
	},
	{
		"-tlb", "-tln", "--time-label-bold", "--time-label-normal", "time-label-bold", 0,
		"Display the time label ansi foreground color in bold or normal.",
		parse_time_label_fg_bold_commandline,
		parse_time_label_fg_bold_rc
	},

	{
		"-pfg", 0, "--percent-foreground", 0, "percent-foreground", "<color>",
		"Use <color> as the percent foreground.",
		parse_percent_fg_color_commandline,
		parse_percent_fg_color_rc
	},
	{
		"-pbg", 0, "--percent-background", 0, "percent-background", "<color>",
		"Use <color> as the percent background.",
		parse_percent_bg_color_commandline,
		parse_percent_bg_color_rc
	},
	{
		"-pb", "-pn", "--percent-bold", "--percent-normal", "percent-bold", 0,
		"Display the percent ansi foreground color in bold or normal.",
		parse_percent_fg_bold_commandline,
		parse_percent_fg_bold_rc
	},

	{
		"-bbfg", 0, "--bar-brace-foreground", 0, "bar-brace-foreground", "<color>",
		"Use <color> as the foreground color for the braces on the progress bar.",
		parse_barbrace_fg_color_commandline,
		parse_barbrace_fg_color_rc
	},
	{
		"-bbbg", 0, "--bar-brace-background", 0, "bar-brace-background", "<color>",
		"Use <color> as the background color for the braces on the progress bar.",
		parse_barbrace_bg_color_commandline,
		parse_barbrace_bg_color_rc
	},
	{
		"-bbb", "-bbn", "--bar-brace-bold", "--bar-brace-normal", "bar-brace-bold", 0,
		"Display the progress bar braces' ansi foreground color in bold or normal.",
		parse_barbrace_fg_bold_commandline,
		parse_barbrace_fg_bold_rc
	},

	{
		"-bfg", 0, "--bar-foreground", 0, "bar-foreground", "<color>",
		"Use <color> as the foreground color for the progress bar.",
		parse_bar_fg_color_commandline,
		parse_bar_fg_color_rc
	},
	{
		"-bbg", 0, "--bar-background", 0, "bar-background", "<color>",
		"Use <color> as the background color for the progress bar.",
		parse_bar_bg_color_commandline,
		parse_bar_bg_color_rc
	},
	{
		"-bb", "-bn", "--bar-bold", "--bar-normal", "bar-bold", 0,
		"Display the progress bar ansi foreground color in bold or normal.",
		parse_bar_fg_bold_commandline,
		parse_bar_fg_bold_rc
	},

	{
		"-h", 0, "--help", 0, 0, 0,
		"Display this help text and exit.",
		parse_help_commandline,
		0
	},
	{
		"-v", 0, "--version", 0, 0, 0,
		"Display version and exit.",
		parse_version_commandline,
		0
	},
	{ 0, 0, 0, 0, 0, 0, 0 }
};

colors_list fg_colors[] = {
	{ "black", "[30m" },
	{ "red", "[31m" },
	{ "green", "[32m" },
	{ "yellow", "[33m" },
	{ "blue", "[34m" },
	{ "magenta", "[35m" },
	{ "cyan", "[36m" },
	{ "white", "[37m" },
	{ 0, 0 }
};

colors_list bg_colors[] = {
	{ "black", "[40m" },
	{ "red", "[41m" },
	{ "green", "[42m" },
	{ "yellow", "[43m" },
	{ "blue", "[44m" },
	{ "magenta", "[45m" },
	{ "cyan", "[46m" },
	{ "white", "[47m" },
	{ 0, 0 }
};

void version(FILE *out)
{
	fprintf(out, "%s version %s\n", PACKAGE, VERSION);
}

void help(FILE *out)
{
	int o = 0;
	int screen_used = 0;
	char option_buffer[256];

	fprintf(out, "Usage:\n");
	fprintf(out, "\n");

	/*
	 * Print out short option usage:
	 */
	fprintf(out, "   bar ");
	screen_used = 7;
	for (o = 0; options[o].cl_func != 0; o++) {
		char *short_option1 = options[o].short_option1;
		char *short_option2 = options[o].short_option2;
		char *arg_description = options[o].arg_description;

		sprintf(option_buffer, "[ %s%s%s%s%s ]",
			short_option1,
			(short_option2 != 0) ? "|" : "",
			(short_option2 != 0) ? short_option2 : "",
			(arg_description != 0) ? " " : "",
			(arg_description != 0) ? arg_description : ""
			);
		if (d.screen_width - screen_used < strlen(option_buffer)) {
			fprintf(out, "\n       ");
			screen_used = 7;
		}
		fprintf(out, option_buffer);
		screen_used += strlen(option_buffer);
	}

	fprintf(out, "\n");
	fprintf(out, "\n");

	/*
	 * Print out long option usage:
	 */
	fprintf(out, "   bar ");
	screen_used = 7;
	for (o = 0; options[o].cl_func != 0; o++) {
		char *long_option1 = options[o].long_option1;
		char *long_option2 = options[o].long_option2;
		char *arg_description = options[o].arg_description;

		sprintf(option_buffer, "[ %s%s%s%s%s ]",
			long_option1,
			(long_option2 != 0) ? "|" : "",
			(long_option2 != 0) ? long_option2 : "",
			(arg_description != 0) ? " " : "",
			(arg_description != 0) ? arg_description : ""
			);
		if (d.screen_width - screen_used < strlen(option_buffer)) {
			fprintf(out, "\n       ");
			screen_used = 7;
		}
		fprintf(out, option_buffer);
		screen_used += strlen(option_buffer);
	}

	fprintf(out, "\n");
	fprintf(out, "\n");

	/*
	 * Print out help text
	 */
	for (o = 0; options[o].cl_func != 0; o++) {
		char *short_option1 = options[o].short_option1;
		char *short_option2 = options[o].short_option2;
		char *long_option1 = options[o].long_option1;
		char *long_option2 = options[o].long_option2;
		char *arg_description = options[o].arg_description;
		char *description = options[o].description;

		fprintf(out, "   %s %s\n",
			short_option1,
			(arg_description != 0) ? arg_description : ""
			);
		if (short_option2 != 0) {
			fprintf(out, "   %s %s\n",
				short_option2,
				(arg_description != 0) ? arg_description : ""
				);
		}
		fprintf(out, "   %s %s\n",
			long_option1,
			(arg_description != 0) ? arg_description : ""
			);
		if (long_option2 != 0) {
			fprintf(out, "   %s %s\n",
				long_option2,
				(arg_description != 0) ? arg_description : ""
				);
		}
		fprintf(out, "\n");
		fprintf(out, "     %s\n", description);
		fprintf(out, "\n");
	}
}

int isOpt(char *s)
{
	int o = 0;

	while (options[o].cl_func != 0) {
		if (strcmp(s, options[o].short_option1) == 0)
			return(o);
		if ((options[o].short_option2 != 0) 
			&& (strcmp(s, options[o].short_option2) == 0))
			return(o);
		if (strcmp(s, options[o].long_option1) == 0)
			return(o);
		if ((options[o].long_option2 != 0) 
			&& (strcmp(s, options[o].long_option2) == 0))
			return(o);
		if ((options[o].rc_option != 0) 
			&& (strcasecmp(s, options[o].rc_option) == 0))
			return(o);
		o++;
	}
	return(-1);
}

int safe_add(uint64 *n, uint64 a)
{
	uint64 t = *n;

	if (MAX_UINT64 - t < a)
		return(1);
	t += a;
	*n = t;
	return(0);
}

int safe_mul(uint64 *n, uint64 x)
{
	uint64 a1, a2;
	uint64 t = 0;
	uint64 multiplier;
	
	if (x < *n) {
		a1 = *n;
		a2 = x;
	}
	else {
		a2 = *n;
		a1 = x;
	}

	multiplier = 1;
	multiplier *= 1000;
	multiplier *= 1000;
	multiplier *= 1000;
	multiplier *= 1000;
	multiplier *= 1000;
	multiplier *= 1000;
	while (a2 > 0) {
		while (multiplier > a2)
			multiplier /= 10;
		if (safe_add(&t, a1*multiplier) != 0)
			return(1);
		a2 -= multiplier;
	}
	*n = t;
	return(0);
}

int parse_num(FILE *ferr, char *s, uint64 *n, uint64 min, uint64 max)
{
	char *ptr_decimal = 0;
	char *ptr_unit = 0;
	char *ptr_start = 0;
	char *ptr_end = 0;
	char *ptr = 0;
	uint64 w = 0;
	uint64 f = 0;
	uint64 unit_multiplier = 1;
	uint64 multiplier = 0;
	uint64 tmp = 0;
	uint64 new_n = 0;
	size_t num_whole_part = 0;
	size_t num_fractional_part = 0;

	*n = 0;

	if (strlen(s) == 0)
		return(0);

	ptr_decimal = s;
	while ((*ptr_decimal != '\0') && (*ptr_decimal != '.'))
		ptr_decimal++;
	
	ptr_unit = s;
	while ((*ptr_unit != '\0') && (isdigit((int)*ptr_unit) || (*ptr_unit == '.')))
		ptr_unit++;
	if ((*ptr_unit != '\0') && (*(ptr_unit+1) != '\0')) {
		print_error(ferr, "Could not parse number: %s", s);
		print_esup(ferr, "Unit multiplier parse error at: \"%s\"", ptr_unit);
		print_esup(ferr, "Unit multiplier should be the last character");
		return(1);
	}

	if (*ptr_decimal != '\0') {
		num_whole_part = (size_t)(ptr_decimal - s);
		num_fractional_part = (size_t)(ptr_unit - ptr_decimal - 1);
	}
	else if (*ptr_unit != '\0') {
		num_whole_part = (size_t)(ptr_unit - s);
	}
	else {
		num_whole_part = strlen(s);
	}

	if ((num_whole_part == 0) && (num_fractional_part == 0)) {
		print_error(ferr, "Could not parse number: %s", s);
		print_esup(ferr, "No digits found");
		return(1);
	}

	switch (toupper(*ptr_unit)) {
		case '\0':
			break;
		case 'K':
			unit_multiplier *= (uint64)d.k;
			break;
		case 'M':
			unit_multiplier *= (uint64)d.k;
			unit_multiplier *= (uint64)d.k;
			break;
		case 'G':
			unit_multiplier *= (uint64)d.k;
			unit_multiplier *= (uint64)d.k;
			unit_multiplier *= (uint64)d.k;
			break;
		case 'T':
			unit_multiplier *= (uint64)d.k;
			unit_multiplier *= (uint64)d.k;
			unit_multiplier *= (uint64)d.k;
			unit_multiplier *= (uint64)d.k;
			break;
		case 'P':
			unit_multiplier *= (uint64)d.k;
			unit_multiplier *= (uint64)d.k;
			unit_multiplier *= (uint64)d.k;
			unit_multiplier *= (uint64)d.k;
			unit_multiplier *= (uint64)d.k;
			break;
		case 'E':
			unit_multiplier *= (uint64)d.k;
			unit_multiplier *= (uint64)d.k;
			unit_multiplier *= (uint64)d.k;
			unit_multiplier *= (uint64)d.k;
			unit_multiplier *= (uint64)d.k;
			unit_multiplier *= (uint64)d.k;
			break;
		case 'B':
			if (_parsing_blocks == 0) {
				unit_multiplier *= (uint64)io.block_size;
			}
			else {
				print_error(ferr, "Cannot specify block size in terms of blocks");
				print_esup(ferr, "Expected: 'k', 'm', 'g', 't', 'p', or 'e'");
				return(1);
			}
			break;
		default:
			print_error(ferr, "Cannot parse number: %s", s);
			print_esup(ferr, "Invalid unit multiplier: '%c'", *ptr_unit);
			print_esup(ferr, "Expected: 'b', 'k', 'm', 'g', 't', 'p', or 'e'");
			return(1);
			break;
	}

	ptr_start = s;
	if (*ptr_decimal == '.')
		ptr_end = ptr_decimal;
	else
		ptr_end = ptr_unit;
	
	multiplier = 1;
	if (ptr_start != ptr_end) {
		for (ptr = ptr_start+1; ptr != ptr_end; ptr++) {
			if (safe_mul(&multiplier, 10) != 0) {
				print_error(ferr, "Whole number precision error at: %s", ptr);
				print_esup(ferr, "Whole number portion too large");
				return(1);
			}
		}
	}
	for (ptr = ptr_start; ptr != ptr_end; ptr++) {
		tmp = (uint64)((*ptr) - '0');
		if (tmp != 0) {
			if (safe_mul(&tmp, multiplier) != 0) {
				print_error(ferr, "Multiplication overflow error");
				print_esup(ferr, "Could not parse number at: %s", ptr);
				return(1);
			}
			if (safe_add(&w, tmp) != 0) {
				print_error(ferr, "Addition overflow error");
				print_esup(ferr, "Could not parse number at: %s", ptr);
				return(1);
			}
		}
		multiplier /= 10;
	}
	if (safe_mul(&w, unit_multiplier) != 0) {
		print_error(ferr, "Multiplication overflow error");
		print_esup(ferr, "Number too large: %s", s);
		return(1);
	}

	if (*ptr_decimal == '.') {
		ptr_start = ptr_decimal+1;
		ptr_end = ptr_unit;
		multiplier = 1;
		for (ptr = ptr_start; ptr != ptr_end; ptr++) {
			tmp = (*ptr) - '0';
			if (tmp != 0) {
				if (safe_mul(&tmp, unit_multiplier) != 0) {
					print_error(ferr, "Multiplication overflow error");
					print_esup(ferr, "Could not parse fraction at: %s", ptr);
					return(1);
				}
				tmp /= multiplier;
				tmp += 5;
				tmp /= 10;
				if (safe_add(&f, tmp) != 0) {
					print_error(ferr, "Addition overflow error");
					print_esup(ferr, "Could not parse fraction at: %s", ptr);
				}
			}
			if (safe_mul(&multiplier, 10) != 0) {
				print_error(ferr, "Multiplication overflow error");
				print_esup(ferr, "Could not parse fraction at: %s", ptr);
				return(1);
			}
			if (multiplier > unit_multiplier)
				break;
		}
	}

	new_n = w;
	if (safe_add(&new_n, f) != 0) {
		print_error(ferr, "Addition overflow error");
		print_esup(ferr, "Number too large: %s", s);
		return(1);
	}

	if (new_n < min) {
		print_error(ferr, "Number too small: %s", s);
		print_esup(ferr, "Value must be %llu or greater", UINT64_CTYPE(min));
		return(1);
	}
	if (new_n > max) {
		print_error(ferr, "Number too large: %s", s);
		print_esup(ferr, "Value must be %llu or less", UINT64_CTYPE(max));
		return(1);
	}

	*n = new_n;

	return(0);
}

int parse_infile_value(FILE *ferr, char *value)
{
	if (strcmp(value, "-") != 0) {
		io.in = open(value, O_RDONLY
#ifdef O_LARGEFILE
		|O_LARGEFILE
#endif
		);
		if (io.in < 0) {
			print_error(ferr, "Cannot open file for reading: %s", value);
			return(1);
		}
		if (fdIsFile(io.in) && fdFileSize(io.in, &io.total_size) == 0) {
			io.total_size_known = 1;
		}
	}
	return(0);
}

int parse_infile_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num)
{
	if ((*arg_num)+1 == argc) {
		print_error(ferr, "Missing filename after %s", argv[(*arg_num)]);
		return(1);
	}
	if (parse_infile_value(ferr, argv[(*arg_num)+1]) != 0)
		return(1);
	(*arg_num)++;
	return(0);
}

int parse_outfile_value(FILE *ferr, char *value)
{
	if (strcmp(value, "-") != 0) {
		io.out = open(value, O_WRONLY|O_CREAT
#ifdef O_LARGEFILE
		|O_LARGEFILE
#endif
		, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
		if (io.out < 0) {
			print_error(ferr, "Cannot open file for writing: %s", value);
			return(1);
		}
	}
	return(0);
}

int parse_outfile_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num)
{
	if ((*arg_num)+1 == argc) {
		print_error(ferr, "Missing filename after %s", argv[(*arg_num)]);
		return(1);
	}
	if (parse_outfile_value(ferr, argv[(*arg_num)+1]) != 0)
		return(1);
	(*arg_num)++;
	return(0);
}

int parse_size_value(FILE *ferr, char *value)
{
	uint64 n = 0;

	if (parse_num(ferr, value, &n, 0, MAX_UINT64) != 0) {
		return(1);
	}
	io.total_size = n;
	io.total_size_known = 1;
	return(0);
}

int parse_throttle_value(FILE *ferr, char *value)
{
	uint64 n = 0;

	if (parse_num(ferr, value, &n, 0, MAX_UINT64) != 0) {
		return(1);
	}
	io.throttle = n+1;
	return(0);
}

int parse_size_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num)
{
	if ((*arg_num)+1 == argc) {
		print_error(ferr, "Missing size after %s", argv[(*arg_num)]);
		return(1);
	}
	parse_size_value(ferr, argv[(*arg_num)+1]);
	(*arg_num)++;
	return(0);
}

int parse_throttle_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num)
{
	if ((*arg_num)+1 == argc) {
		print_error(ferr, "Missing rate after %s", argv[(*arg_num)]);
		return(1);
	}
	parse_throttle_value(ferr, argv[(*arg_num)+1]);
	(*arg_num)++;
	return(0);
}

int parse_throttle_rc(FILE *ferr, char* filename, int line, char *tag, char *value)
{
	if (parse_throttle_value(ferr, value) != 0) {
		print_esup(ferr, "In file: %s[%d]", filename, line);
		return(1);
	}
	return(0);
}

int parse_buffer_size_value(FILE *ferr, char *value)
{
	uint64 n = 0;

	if (parse_num(ferr, value, &n, 1, MAX_SIZE_T) != 0) {
		return(1);
	}
	io.buffer_size = (size_t)n;
	return(0);
}

int parse_buffer_size_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num)
{
	if ((*arg_num)+1 == argc) {
		print_error(ferr, "Missing size after %s", argv[(*arg_num)]);
		return(1);
	}
	parse_buffer_size_value(ferr, argv[(*arg_num)+1]);
	(*arg_num)++;
	return(0);
}

int parse_buffer_size_rc(FILE *ferr, char* filename, int line, char *tag, char *value)
{
	if (parse_buffer_size_value(ferr, value) != 0) {
		print_esup(ferr, "In file: %s[%d]", filename, line);
		return(1);
	}
	return(0);
}

int parse_block_size_value(FILE *ferr, char *value)
{
	uint64 n = 0;
	int r;

	_parsing_blocks = 1; /* Shameless hack */
	r = parse_num(ferr, value, &n, 1, MAX_SIZE_T);
	_parsing_blocks = 0; /* Shameless hack */
	if (r != 0) {
		return(1);
	}
	io.block_size = (size_t)n;
	return(0);
}

int parse_block_size_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num)
{
	if ((*arg_num)+1 == argc) {
		print_error(ferr, "Missing size after %s", argv[(*arg_num)]);
		return(1);
	}
	parse_block_size_value(ferr, argv[(*arg_num)+1]);
	(*arg_num)++;
	return(0);
}

int parse_block_size_rc(FILE *ferr, char* filename, int line, char *tag, char *value)
{
	if (parse_block_size_value(ferr, value) != 0) {
		print_esup(ferr, "In file: %s[%d]", filename, line);
		return(1);
	}
	return(0);
}

int parse_interval_value(FILE *ferr, char *value)
{
	int n = 0;
	char check[4096] = { 0 };

	sscanf(value, "%d", &n);
	sprintf(check, "%d", n);
	if (strcmp(value, check) != 0) {
		print_error(ferr, "Type mismatch or number too large: %s", value);
		return(1);
	}
	if ((n < 1) || (n > 60*60*24)) {
		print_error(ferr, "Invalid display interval: %s", value);
		return(1);
	}
	d.display_interval = n;
	return(0);
}

int parse_interval_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num)
{
	if ((*arg_num)+1 == argc) {
		print_error(ferr, "Missing seconds after %s", argv[(*arg_num)]);
		return(1);
	}
	if (parse_interval_value(ferr, argv[(*arg_num)+1]) != 0)
		print_esup(ferr, "Ignoring given interval");
	(*arg_num)++;
	return(0);
}

int parse_interval_rc(FILE *ferr, char *filename, int line, char *tag, char *value)
{
	if (parse_interval_value(ferr, value) != 0) {
		print_esup(ferr, "In file: %s[%d]", filename, line);
		return(1);
	}
	return(0);
}

int parse_timeout_value(FILE *ferr, char *value)
{
	unsigned long n = 0;
	char check[4096] = { 0 };

	sscanf(value, "%lu", &n);
	sprintf(check, "%lu", n);
	if (strcmp(value, check) != 0) {
		print_error(ferr, "Type mismatch or number too large: %s", value);
		return(1);
	}
	if (n > 999999) {
		print_error(ferr, "Type mismatch or number too large: %s", value);
		return(1);
	}
	io.timeout = (uint32)n;
	return(0);
}

int parse_timeout_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num)
{
	if ((*arg_num)+1 == argc) {
		print_error(ferr, "Missing microseconds after %s", argv[(*arg_num)]);
		return(1);
	}
	if (parse_timeout_value(ferr, argv[(*arg_num)+1]) != 0)
		print_esup(ferr, "Ignoring given timeout");
	(*arg_num)++;
	return(0);
}

int parse_timeout_rc(FILE *ferr, char *filename, int line, char *tag, char *value)
{
	if (parse_timeout_value(ferr, value) != 0) {
		print_esup(ferr, "In file: %s[%d]", filename, line);
		return(1);
	}
	return(0);
}

int parse_kilo_value(FILE *ferr, char *value)
{
	if (strcmp(value, "1000") == 0) {
		d.k = 1000;
		return(0);
	}
	if (strcmp(value, "1024") == 0) {
		d.k = 1024;
		return(0);
	}
	return(1);
}

int parse_kilo_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num)
{
	if ((*arg_num)+1 == argc) {
		print_error(ferr, "Missing option after %s", argv[(*arg_num)]);
		return(1);
	}
	if (parse_kilo_value(ferr, argv[(*arg_num)+1]) != 0) {
		print_error(ferr, "Invalid option for %s: %s",
			argv[(*arg_num)], argv[(*arg_num)+1]);
		return(1);
	}
	(*arg_num)++;
	return(0);
}

int parse_kilo_rc(FILE *ferr, char *filename, int line, char *tag, char *value)
{
	if (parse_kilo_value(ferr, value) != 0) {
		print_esup(ferr, "In file: %s[%d]", filename, line);
		return(1);
	}
	return(0);
}

int parse_width_value(FILE *ferr, char *value)
{
	int n = 0;

	sscanf(value, "%d", &n);
	if (n < 1) {
		print_error(ferr, "Invalid screen width: %s", value);
		return(1);
	}
	d.screen_width = n;
	d.manual_width = 1;
	return(0);
}

int parse_width_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num)
{
	if ((*arg_num)+1 == argc) {
		print_error(ferr, "Missing width after %s", argv[(*arg_num)]);
		return(1);
	}
	if (parse_width_value(ferr, argv[(*arg_num)+1]) != 0)
		return(1);
	(*arg_num)++;
	return(0);
}

int parse_width_rc(FILE *ferr, char *filename, int line, char *tag, char *value)
{
	if (parse_width_value(ferr, value) != 0) {
		print_esup(ferr, "In file: %s[%d]", filename, line);
		return(1);
	}
	return(0);
}

int parse_switch_commandline(char *arg, options_list *option, int *sw)
{
	if ((option->short_option1 != 0) && (strcmp(arg, option->short_option1) == 0))
		(*sw) = 1;
	if ((option->long_option1 != 0) && (strcmp(arg, option->long_option1) == 0))
		(*sw) = 1;
	if ((option->short_option2 != 0) && (strcmp(arg, option->short_option2) == 0))
		(*sw) = 0;
	if ((option->long_option2 != 0) && (strcmp(arg, option->long_option2) == 0))
		(*sw) = 0;
	return(0);
}

int parse_switch_rc(FILE *ferr, char *filename, int line, char *value, int *sw)
{
	if ((strcasecmp(value, "on") == 0)
		|| (strcasecmp(value, "yes") == 0)
		|| (strcasecmp(value, "y") == 0)
		|| (strcasecmp(value, "true") == 0)
		|| (strcasecmp(value, "t") == 0)
		|| (strcasecmp(value, "1") == 0)
		)
	{
		(*sw) = 1;
	}
	else if ((strcasecmp(value, "off") == 0)
		|| (strcasecmp(value, "no") == 0)
		|| (strcasecmp(value, "n") == 0)
		|| (strcasecmp(value, "false") == 0)
		|| (strcasecmp(value, "f") == 0)
		|| (strcasecmp(value, "0") == 0)
		)
	{
		(*sw) = 0;
	}
	else {
		print_error(ferr, "Parse error in rc file: %s[%d]", filename, line);
		print_esup(ferr, "Unknown switch value: %s", value);
		return(1);
	}
	return(0);
}

int parse_switch_all_commandline(char *arg, options_list *option)
{
	if ((option->short_option1 != 0) && (strcmp(arg, option->short_option1) == 0)) {
		d.display_twiddle = 1;
		d.display_title = 1;
		d.display_datacount = 1;
		d.display_throughput = 1;
		d.display_time = 1;
		d.display_percent = 1;
		d.display_bar = 1;
		d.display_summary = 1;
	}
	if ((option->long_option1 != 0) && (strcmp(arg, option->long_option1) == 0)) {
		d.display_twiddle = 1;
		d.display_title = 1;
		d.display_datacount = 1;
		d.display_throughput = 1;
		d.display_time = 1;
		d.display_percent = 1;
		d.display_bar = 1;
		d.display_summary = 1;
	}
	if ((option->short_option2 != 0) && (strcmp(arg, option->short_option2) == 0)) {
		d.display_twiddle = 0;
		d.display_title = 0;
		d.display_datacount = 0;
		d.display_throughput = 0;
		d.display_time = 0;
		d.display_percent = 0;
		d.display_bar = 0;
		d.display_summary = 0;
	}
	if ((option->long_option2 != 0) && (strcmp(arg, option->long_option2) == 0)) {
		d.display_twiddle = 0;
		d.display_title = 0;
		d.display_datacount = 0;
		d.display_throughput = 0;
		d.display_time = 0;
		d.display_percent = 0;
		d.display_bar = 0;
		d.display_summary = 0;
	}
	return(0);
}

int parse_minus_one_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num)
{
	int r;
	
	r = parse_switch_commandline(
		argv[(*arg_num)],
		&options[option_num],
		&d.screen_width_minus_one
		);
	return(r);
}

int parse_minus_one_rc(FILE *ferr, char *filename, int line, char *tag, char *value)
{
	int r;

	r = parse_switch_rc(ferr, filename, line, value,
		&d.screen_width_minus_one
		);
	return(r);
}

int parse_twiddle_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num)
{
	int r;
	
	r = parse_switch_commandline(
		argv[(*arg_num)],
		&options[option_num],
		&d.display_twiddle
		);
	return(r);
}

int parse_twiddle_rc(FILE *ferr, char *filename, int line, char *tag, char *value)
{
	int r;

	r = parse_switch_rc(ferr, filename, line, value,
		&d.display_twiddle
		);
	return(r);
}

int parse_count_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num)
{
	int r;
	
	r = parse_switch_commandline(
		argv[(*arg_num)],
		&options[option_num],
		&d.display_datacount
		);
	return(r);
}

int parse_count_rc(FILE *ferr, char *filename, int line, char *tag, char *value)
{
	int r;

	r = parse_switch_rc(ferr, filename, line, value,
		&d.display_datacount
		);
	return(r);
}

int parse_throughput_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num)
{
	int r;
	
	r = parse_switch_commandline(
		argv[(*arg_num)],
		&options[option_num],
		&d.display_throughput
		);
	return(r);
}

int parse_throughput_rc(FILE *ferr, char *filename, int line, char *tag, char *value)
{
	int r;

	r = parse_switch_rc(ferr, filename, line, value,
		&d.display_throughput
		);
	return(r);
}

int parse_time_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num)
{
	int r;
	
	r = parse_switch_commandline(
		argv[(*arg_num)],
		&options[option_num],
		&d.display_time
		);
	return(r);
}

int parse_elapsed_only_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num)
{
	int r;

	r = parse_switch_commandline(
		argv[(*arg_num)],
		&options[option_num],
		&d.display_elapsed_only
		);
	return(r);
}

int parse_time_rc(FILE *ferr, char *filename, int line, char *tag, char *value)
{
	int r;

	r = parse_switch_rc(ferr, filename, line, value,
		&d.display_time
		);
	return(r);
}

int parse_elapsed_only_rc(FILE *ferr, char *filename, int line, char *tag, char *value)
{
	int r;

	r = parse_switch_rc(ferr, filename, line, value,
		&d.display_elapsed_only
		);
	return(r);
}

int parse_percent_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num)
{
	int r;
	
	r = parse_switch_commandline(
		argv[(*arg_num)],
		&options[option_num],
		&d.display_percent
		);
	return(r);
}

int parse_percent_rc(FILE *ferr, char *filename, int line, char *tag, char *value)
{
	int r;

	r = parse_switch_rc(ferr, filename, line, value,
		&d.display_percent
		);
	return(r);
}

int parse_bar_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num)
{
	int r;
	
	r = parse_switch_commandline(
		argv[(*arg_num)],
		&options[option_num],
		&d.display_bar
		);
	return(r);
}

int parse_bar_rc(FILE *ferr, char *filename, int line, char *tag, char *value)
{
	int r;

	r = parse_switch_rc(ferr, filename, line, value,
		&d.display_bar
		);
	return(r);
}

int parse_title_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num)
{
	if ((*arg_num)+1 == argc) {
		print_error(ferr, "Missing title string after %s", argv[(*arg_num)]);
		return(1);
	}
	sprintf(d.title, "%.*s", 80, argv[(*arg_num)+1]);
	(*arg_num)++;
	return(0);
}

int parse_title_rc(FILE *ferr, char *filename, int line, char *tag, char *value)
{
	sprintf(d.title, "%.*s", 80, value);
	return(0);
}

int parse_total_percent_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num)
{
	int r;
	
	r = parse_switch_commandline(
		argv[(*arg_num)],
		&options[option_num],
		&d.total_display_percent
		);
	return(r);
}

int parse_total_percent_rc(FILE *ferr, char *filename, int line, char *tag, char *value)
{
	int r;

	r = parse_switch_rc(ferr, filename, line, value,
		&d.total_display_percent
		);
	return(r);
}

int parse_summary_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num)
{
	int r;
	
	r = parse_switch_commandline(
		argv[(*arg_num)],
		&options[option_num],
		&d.display_summary
		);
	return(r);
}

int parse_summary_rc(FILE *ferr, char *filename, int line, char *tag, char *value)
{
	int r;

	r = parse_switch_rc(ferr, filename, line, value,
		&d.display_summary
		);
	return(r);
}

int parse_display_title_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num)
{
	int r;
	
	r = parse_switch_commandline(
		argv[(*arg_num)],
		&options[option_num],
		&d.display_title
		);
	return(r);
}

int parse_display_title_rc(FILE *ferr, char *filename, int line, char *tag, char *value)
{
	int r;

	r = parse_switch_rc(ferr, filename, line, value,
		&d.display_title
		);
	return(r);
}

int parse_ansi_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num)
{
	int r;
	
	r = parse_switch_commandline(
		argv[(*arg_num)],
		&options[option_num],
		&d.display_ansi
		);
	return(r);
}

int parse_ansi_rc(FILE *ferr, char *filename, int line, char *tag, char *value)
{
	int r;

	r = parse_switch_rc(ferr, filename, line, value,
		&d.display_ansi
		);
	return(r);
}

int parse_throughput_bits_rc(FILE *ferr, char *filename, int line, char *tag, char *value)
{
	int r;

	r = parse_switch_rc(ferr, filename, line, value,
		&d.display_throughput_bits
		);
	return(r);
}

int parse_count_bits_rc(FILE *ferr, char *filename, int line, char *tag, char *value)
{
	int r;

	r = parse_switch_rc(ferr, filename, line, value,
		&d.display_count_bits
		);
	return(r);
}

int parse_displays_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num)
{
	int r;
	
	r = parse_switch_all_commandline(
		argv[(*arg_num)],
		&options[option_num]
		);
	return(r);
}

int parse_help_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num)
{
	help(ferr);
	return(1);
}

int parse_version_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num)
{
	version(ferr);
	return(1);
}

int parse_throughput_bits_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num)
{
	int r;
	
	r = parse_switch_commandline(
		argv[(*arg_num)],
		&options[option_num],
		&d.display_throughput_bits
		);
	return(r);
}

int parse_count_bits_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num)
{
	int r;
	
	r = parse_switch_commandline(
		argv[(*arg_num)],
		&options[option_num],
		&d.display_count_bits
		);
	return(r);
}

int isColor(char *s, colors_list *list)
{
	int c = 0;

	while (list[c].name != 0) {
		if (strcasecmp(s, list[c].name) == 0)
			return(c);
		c++;
	}
	return(-1);
}

int parse_color(FILE *ferr, char *value, colors_list *list, char **code_ptr)
{
	int c;

	*code_ptr = 0;
	if (strcasecmp(value, "normal") == 0)
		return(0);
	c = isColor(value, list);
	if (c == -1) {
		print_error(ferr, "Invalid color: %s", value);
		return(1);
	}
	*code_ptr = list[c].code;
	return(0);
}

int parse_space_bg_color_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num)
{
	char *code = 0;

	if ((*arg_num)+1 == argc) {
		print_error(ferr, "Missing color name after %s", argv[(*arg_num)]);
		print_esup(ferr, "Ignoring color code");
		return(0);
	}
	if (parse_color(ferr, argv[(*arg_num)+1], bg_colors, &code) != 0)
		print_esup(ferr, "Ignoring color code");
	else {
		d.space_bg_color = code;
	}
	(*arg_num)++;
	return(0);
}

int parse_space_bg_color_rc(FILE *ferr, char *filename, int line, char *tag, char *value)
{
	char *code = 0;

	if (parse_color(ferr, value, bg_colors, &code) != 0) {
		print_esup(ferr, "in file: %s[%d]", filename, line);
		return(1);
	}
	d.space_bg_color = code;
	return(0);
}

int parse_twiddle_fg_color_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num)
{
	char *code = 0;

	if ((*arg_num)+1 == argc) {
		print_error(ferr, "Missing color name after %s", argv[(*arg_num)]);
		print_esup(ferr, "Ignoring color code");
		return(0);
	}
	if (parse_color(ferr, argv[(*arg_num)+1], fg_colors, &code) != 0)
		print_esup(ferr, "Ignoring color code");
	else {
		d.twiddle_fg_color = code;
	}
	(*arg_num)++;
	return(0);
}

int parse_twiddle_fg_color_rc(FILE *ferr, char *filename, int line, char *tag, char *value)
{
	char *code = 0;

	if (parse_color(ferr, value, fg_colors, &code) != 0) {
		print_esup(ferr, "In file: %s[%d]", filename, line);
		return(1);
	}
	d.twiddle_fg_color = code;
	return(0);
}

int parse_twiddle_bg_color_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num)
{
	char *code = 0;

	if ((*arg_num)+1 == argc) {
		print_error(ferr, "Missing color name after %s", argv[(*arg_num)]);
		print_esup(ferr, "Ignoring color code");
		return(0);
	}
	if (parse_color(ferr, argv[(*arg_num)+1], bg_colors, &code) != 0)
		print_esup(ferr, "Ignoring color code");
	else {
		d.twiddle_bg_color = code;
	}
	(*arg_num)++;
	return(0);
}

int parse_twiddle_bg_color_rc(FILE *ferr, char *filename, int line, char *tag, char *value)
{
	char *code = 0;

	if (parse_color(ferr, value, bg_colors, &code) != 0) {
		print_esup(ferr, "in file: %s[%d]", filename, line);
		return(1);
	}
	d.twiddle_bg_color = code;
	return(0);
}

int parse_twiddle_fg_bold_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num)
{
	int r;

	r = parse_switch_commandline(
		argv[(*arg_num)],
		&options[option_num],
		&d.twiddle_fg_bold
		);
	return(r);
}

int parse_twiddle_fg_bold_rc(FILE *ferr, char *filename, int line, char *tag, char *value)
{
	int r;

	r = parse_switch_rc(ferr, filename, line, value,
		&d.twiddle_fg_bold
		);
	return(r);
}

int parse_title_fg_color_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num)
{
	char *code = 0;

	if ((*arg_num)+1 == argc) {
		print_error(ferr, "Missing color name after %s", argv[(*arg_num)]);
		print_esup(ferr, "Ignoring color code");
		return(0);
	}
	if (parse_color(ferr, argv[(*arg_num)+1], fg_colors, &code) != 0)
		print_esup(ferr, "Ignoring color code");
	else {
		d.title_fg_color = code;
	}
	(*arg_num)++;
	return(0);
}

int parse_title_fg_color_rc(FILE *ferr, char *filename, int line, char *tag, char *value)
{
	char *code = 0;

	if (parse_color(ferr, value, fg_colors, &code) != 0) {
		print_esup(ferr, "In file: %s[%d]", filename, line);
		return(1);
	}
	d.title_fg_color = code;
	return(0);
}

int parse_title_bg_color_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num)
{
	char *code = 0;

	if ((*arg_num)+1 == argc) {
		print_error(ferr, "Missing color name after %s", argv[(*arg_num)]);
		print_esup(ferr, "Ignoring color code");
		return(0);
	}
	if (parse_color(ferr, argv[(*arg_num)+1], bg_colors, &code) != 0)
		print_esup(ferr, "Ignoring color code");
	else {
		d.title_bg_color = code;
	}
	(*arg_num)++;
	return(0);
}

int parse_title_bg_color_rc(FILE *ferr, char *filename, int line, char *tag, char *value)
{
	char *code = 0;

	if (parse_color(ferr, value, bg_colors, &code) != 0) {
		print_esup(ferr, "in file: %s[%d]", filename, line);
		return(1);
	}
	d.title_bg_color = code;
	return(0);
}

int parse_title_fg_bold_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num)
{
	int r;

	r = parse_switch_commandline(
		argv[(*arg_num)],
		&options[option_num],
		&d.title_fg_bold
		);
	return(r);
}

int parse_title_fg_bold_rc(FILE *ferr, char *filename, int line, char *tag, char *value)
{
	int r;

	r = parse_switch_rc(ferr, filename, line, value,
		&d.title_fg_bold
		);
	return(r);
}

int parse_count_fg_color_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num)
{
	char *code = 0;

	if ((*arg_num)+1 == argc) {
		print_error(ferr, "Missing color name after %s", argv[(*arg_num)]);
		print_esup(ferr, "Ignoring color code");
		return(0);
	}
	if (parse_color(ferr, argv[(*arg_num)+1], fg_colors, &code) != 0)
		print_esup(ferr, "Ignoring color code");
	else {
		d.datacount_fg_color = code;
	}
	(*arg_num)++;
	return(0);
}

int parse_count_fg_color_rc(FILE *ferr, char *filename, int line, char *tag, char *value)
{
	char *code = 0;

	if (parse_color(ferr, value, fg_colors, &code) != 0) {
		print_esup(ferr, "In file: %s[%d]", filename, line);
		return(1);
	}
	d.datacount_fg_color = code;
	return(0);
}

int parse_count_bg_color_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num)
{
	char *code = 0;

	if ((*arg_num)+1 == argc) {
		print_error(ferr, "Missing color name after %s", argv[(*arg_num)]);
		print_esup(ferr, "Ignoring color code");
		return(0);
	}
	if (parse_color(ferr, argv[(*arg_num)+1], bg_colors, &code) != 0)
		print_esup(ferr, "Ignoring color code");
	else {
		d.datacount_bg_color = code;
	}
	(*arg_num)++;
	return(0);
}

int parse_count_bg_color_rc(FILE *ferr, char *filename, int line, char *tag, char *value)
{
	char *code = 0;

	if (parse_color(ferr, value, bg_colors, &code) != 0) {
		print_esup(ferr, "in file: %s[%d]", filename, line);
		return(1);
	}
	d.datacount_bg_color = code;
	return(0);
}

int parse_count_fg_bold_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num)
{
	int r;

	r = parse_switch_commandline(
		argv[(*arg_num)],
		&options[option_num],
		&d.datacount_fg_bold
		);
	return(r);
}

int parse_count_fg_bold_rc(FILE *ferr, char *filename, int line, char *tag, char *value)
{
	int r;

	r = parse_switch_rc(ferr, filename, line, value,
		&d.datacount_fg_bold
		);
	return(r);
}

int parse_throughput_label_fg_color_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num)
{
	char *code = 0;

	if ((*arg_num)+1 == argc) {
		print_error(ferr, "Missing color name after %s", argv[(*arg_num)]);
		print_esup(ferr, "Ignoring color code");
		return(0);
	}
	if (parse_color(ferr, argv[(*arg_num)+1], fg_colors, &code) != 0)
		print_esup(ferr, "Ignoring color code");
	else {
		d.throughput_label_fg_color = code;
	}
	(*arg_num)++;
	return(0);
}

int parse_throughput_label_fg_color_rc(FILE *ferr, char *filename, int line, char *tag, char *value)
{
	char *code = 0;

	if (parse_color(ferr, value, fg_colors, &code) != 0) {
		print_esup(ferr, "In file: %s[%d]", filename, line);
		return(1);
	}
	d.throughput_label_fg_color = code;
	return(0);
}

int parse_throughput_label_bg_color_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num)
{
	char *code = 0;

	if ((*arg_num)+1 == argc) {
		print_error(ferr, "Missing color name after %s", argv[(*arg_num)]);
		print_esup(ferr, "Ignoring color code");
		return(0);
	}
	if (parse_color(ferr, argv[(*arg_num)+1], bg_colors, &code) != 0)
		print_esup(ferr, "Ignoring color code");
	else {
		d.throughput_label_bg_color = code;
	}
	(*arg_num)++;
	return(0);
}

int parse_throughput_label_bg_color_rc(FILE *ferr, char *filename, int line, char *tag, char *value)
{
	char *code = 0;

	if (parse_color(ferr, value, bg_colors, &code) != 0) {
		print_esup(ferr, "in file: %s[%d]", filename, line);
		return(1);
	}
	d.throughput_label_bg_color = code;
	return(0);
}

int parse_throughput_label_fg_bold_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num)
{
	int r;

	r = parse_switch_commandline(
		argv[(*arg_num)],
		&options[option_num],
		&d.throughput_label_fg_bold
		);
	return(r);
}

int parse_throughput_label_fg_bold_rc(FILE *ferr, char *filename, int line, char *tag, char *value)
{
	int r;

	r = parse_switch_rc(ferr, filename, line, value,
		&d.throughput_label_fg_bold
		);
	return(r);
}

int parse_throughput_fg_color_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num)
{
	char *code = 0;

	if ((*arg_num)+1 == argc) {
		print_error(ferr, "Missing color name after %s", argv[(*arg_num)]);
		print_esup(ferr, "Ignoring color code");
		return(0);
	}
	if (parse_color(ferr, argv[(*arg_num)+1], fg_colors, &code) != 0)
		print_esup(ferr, "Ignoring color code");
	else {
		d.throughput_fg_color = code;
	}
	(*arg_num)++;
	return(0);
}

int parse_throughput_fg_color_rc(FILE *ferr, char *filename, int line, char *tag, char *value)
{
	char *code = 0;

	if (parse_color(ferr, value, fg_colors, &code) != 0) {
		print_esup(ferr, "In file: %s[%d]", filename, line);
		return(1);
	}
	d.throughput_fg_color = code;
	return(0);
}

int parse_throughput_bg_color_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num)
{
	char *code = 0;

	if ((*arg_num)+1 == argc) {
		print_error(ferr, "Missing color name after %s", argv[(*arg_num)]);
		print_esup(ferr, "Ignoring color code");
		return(0);
	}
	if (parse_color(ferr, argv[(*arg_num)+1], bg_colors, &code) != 0)
		print_esup(ferr, "Ignoring color code");
	else {
		d.throughput_bg_color = code;
	}
	(*arg_num)++;
	return(0);
}

int parse_throughput_bg_color_rc(FILE *ferr, char *filename, int line, char *tag, char *value)
{
	char *code = 0;

	if (parse_color(ferr, value, bg_colors, &code) != 0) {
		print_esup(ferr, "in file: %s[%d]", filename, line);
		return(1);
	}
	d.throughput_bg_color = code;
	return(0);
}

int parse_throughput_fg_bold_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num)
{
	int r;

	r = parse_switch_commandline(
		argv[(*arg_num)],
		&options[option_num],
		&d.throughput_fg_bold
		);
	return(r);
}

int parse_throughput_fg_bold_rc(FILE *ferr, char *filename, int line, char *tag, char *value)
{
	int r;

	r = parse_switch_rc(ferr, filename, line, value,
		&d.throughput_fg_bold
		);
	return(r);
}

int parse_time_label_fg_color_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num)
{
	char *code = 0;

	if ((*arg_num)+1 == argc) {
		print_error(ferr, "Missing color name after %s", argv[(*arg_num)]);
		print_esup(ferr, "Ignoring color code");
		return(0);
	}
	if (parse_color(ferr, argv[(*arg_num)+1], fg_colors, &code) != 0)
		print_esup(ferr, "Ignoring color code");
	else {
		d.time_label_fg_color = code;
	}
	(*arg_num)++;
	return(0);
}

int parse_time_label_fg_color_rc(FILE *ferr, char *filename, int line, char *tag, char *value)
{
	char *code = 0;

	if (parse_color(ferr, value, fg_colors, &code) != 0) {
		print_esup(ferr, "In file: %s[%d]", filename, line);
		return(1);
	}
	d.time_label_fg_color = code;
	return(0);
}

int parse_time_label_bg_color_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num)
{
	char *code = 0;

	if ((*arg_num)+1 == argc) {
		print_error(ferr, "Missing color name after %s", argv[(*arg_num)]);
		print_esup(ferr, "Ignoring color code");
		return(0);
	}
	if (parse_color(ferr, argv[(*arg_num)+1], bg_colors, &code) != 0)
		print_esup(ferr, "Ignoring color code");
	else {
		d.time_label_bg_color = code;
	}
	(*arg_num)++;
	return(0);
}

int parse_time_label_bg_color_rc(FILE *ferr, char *filename, int line, char *tag, char *value)
{
	char *code = 0;

	if (parse_color(ferr, value, bg_colors, &code) != 0) {
		print_esup(ferr, "in file: %s[%d]", filename, line);
		return(1);
	}
	d.time_label_bg_color = code;
	return(0);
}

int parse_time_label_fg_bold_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num)
{
	int r;

	r = parse_switch_commandline(
		argv[(*arg_num)],
		&options[option_num],
		&d.time_label_fg_bold
		);
	return(r);
}

int parse_time_label_fg_bold_rc(FILE *ferr, char *filename, int line, char *tag, char *value)
{
	int r;

	r = parse_switch_rc(ferr, filename, line, value,
		&d.time_label_fg_bold
		);
	return(r);
}

int parse_time_fg_color_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num)
{
	char *code = 0;

	if ((*arg_num)+1 == argc) {
		print_error(ferr, "Missing color name after %s", argv[(*arg_num)]);
		print_esup(ferr, "Ignoring color code");
		return(0);
	}
	if (parse_color(ferr, argv[(*arg_num)+1], fg_colors, &code) != 0)
		print_esup(ferr, "Ignoring color code");
	else {
		d.time_fg_color = code;
	}
	(*arg_num)++;
	return(0);
}

int parse_time_fg_color_rc(FILE *ferr, char *filename, int line, char *tag, char *value)
{
	char *code = 0;

	if (parse_color(ferr, value, fg_colors, &code) != 0) {
		print_esup(ferr, "In file: %s[%d]", filename, line);
		return(1);
	}
	d.time_fg_color = code;
	return(0);
}

int parse_time_bg_color_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num)
{
	char *code = 0;

	if ((*arg_num)+1 == argc) {
		print_error(ferr, "Missing color name after %s", argv[(*arg_num)]);
		print_esup(ferr, "Ignoring color code");
		return(0);
	}
	if (parse_color(ferr, argv[(*arg_num)+1], bg_colors, &code) != 0)
		print_esup(ferr, "Ignoring color code");
	else {
		d.time_bg_color = code;
	}
	(*arg_num)++;
	return(0);
}

int parse_time_bg_color_rc(FILE *ferr, char *filename, int line, char *tag, char *value)
{
	char *code = 0;

	if (parse_color(ferr, value, bg_colors, &code) != 0) {
		print_esup(ferr, "in file: %s[%d]", filename, line);
		return(1);
	}
	d.time_bg_color = code;
	return(0);
}

int parse_time_fg_bold_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num)
{
	int r;

	r = parse_switch_commandline(
		argv[(*arg_num)],
		&options[option_num],
		&d.time_fg_bold
		);
	return(r);
}

int parse_time_fg_bold_rc(FILE *ferr, char *filename, int line, char *tag, char *value)
{
	int r;

	r = parse_switch_rc(ferr, filename, line, value,
		&d.time_fg_bold
		);
	return(r);
}

int parse_percent_fg_color_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num)
{
	char *code = 0;

	if ((*arg_num)+1 == argc) {
		print_error(ferr, "Missing color name after %s", argv[(*arg_num)]);
		print_esup(ferr, "Ignoring color code");
		return(0);
	}
	if (parse_color(ferr, argv[(*arg_num)+1], fg_colors, &code) != 0)
		print_esup(ferr, "Ignoring color code");
	else {
		d.percent_fg_color = code;
	}
	(*arg_num)++;
	return(0);
}

int parse_percent_fg_color_rc(FILE *ferr, char *filename, int line, char *tag, char *value)
{
	char *code = 0;

	if (parse_color(ferr, value, fg_colors, &code) != 0) {
		print_esup(ferr, "In file: %s[%d]", filename, line);
		return(1);
	}
	d.percent_fg_color = code;
	return(0);
}

int parse_percent_bg_color_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num)
{
	char *code = 0;

	if ((*arg_num)+1 == argc) {
		print_error(ferr, "Missing color name after %s", argv[(*arg_num)]);
		print_esup(ferr, "Ignoring color code");
		return(0);
	}
	if (parse_color(ferr, argv[(*arg_num)+1], bg_colors, &code) != 0)
		print_esup(ferr, "Ignoring color code");
	else {
		d.percent_bg_color = code;
	}
	(*arg_num)++;
	return(0);
}

int parse_percent_bg_color_rc(FILE *ferr, char *filename, int line, char *tag, char *value)
{
	char *code = 0;

	if (parse_color(ferr, value, bg_colors, &code) != 0) {
		print_esup(ferr, "in file: %s[%d]", filename, line);
		return(1);
	}
	d.percent_bg_color = code;
	return(0);
}

int parse_percent_fg_bold_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num)
{
	int r;

	r = parse_switch_commandline(
		argv[(*arg_num)],
		&options[option_num],
		&d.percent_fg_bold
		);
	return(r);
}

int parse_percent_fg_bold_rc(FILE *ferr, char *filename, int line, char *tag, char *value)
{
	int r;

	r = parse_switch_rc(ferr, filename, line, value,
		&d.percent_fg_bold
		);
	return(r);
}

int parse_barbrace_fg_color_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num)
{
	char *code = 0;

	if ((*arg_num)+1 == argc) {
		print_error(ferr, "Missing color name after %s", argv[(*arg_num)]);
		print_esup(ferr, "Ignoring color code");
		return(0);
	}
	if (parse_color(ferr, argv[(*arg_num)+1], fg_colors, &code) != 0)
		print_esup(ferr, "Ignoring color code");
	else {
		d.barbrace_fg_color = code;
	}
	(*arg_num)++;
	return(0);
}

int parse_barbrace_fg_color_rc(FILE *ferr, char *filename, int line, char *tag, char *value)
{
	char *code = 0;

	if (parse_color(ferr, value, fg_colors, &code) != 0) {
		print_esup(ferr, "In file: %s[%d]", filename, line);
		return(1);
	}
	d.barbrace_fg_color = code;
	return(0);
}

int parse_barbrace_bg_color_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num)
{
	char *code = 0;

	if ((*arg_num)+1 == argc) {
		print_error(ferr, "Missing color name after %s", argv[(*arg_num)]);
		print_esup(ferr, "Ignoring color code");
		return(0);
	}
	if (parse_color(ferr, argv[(*arg_num)+1], bg_colors, &code) != 0)
		print_esup(ferr, "Ignoring color code");
	else {
		d.barbrace_bg_color = code;
	}
	(*arg_num)++;
	return(0);
}

int parse_barbrace_bg_color_rc(FILE *ferr, char *filename, int line, char *tag, char *value)
{
	char *code = 0;

	if (parse_color(ferr, value, bg_colors, &code) != 0) {
		print_esup(ferr, "in file: %s[%d]", filename, line);
		return(1);
	}
	d.barbrace_bg_color = code;
	return(0);
}

int parse_barbrace_fg_bold_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num)
{
	int r;

	r = parse_switch_commandline(
		argv[(*arg_num)],
		&options[option_num],
		&d.barbrace_fg_bold
		);
	return(r);
}

int parse_barbrace_fg_bold_rc(FILE *ferr, char *filename, int line, char *tag, char *value)
{
	int r;

	r = parse_switch_rc(ferr, filename, line, value,
		&d.barbrace_fg_bold
		);
	return(r);
}

int parse_bar_fg_color_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num)
{
	char *code = 0;

	if ((*arg_num)+1 == argc) {
		print_error(ferr, "Missing color name after %s", argv[(*arg_num)]);
		print_esup(ferr, "Ignoring color code");
		return(0);
	}
	if (parse_color(ferr, argv[(*arg_num)+1], fg_colors, &code) != 0)
		print_esup(ferr, "Ignoring color code");
	else {
		d.bar_fg_color = code;
	}
	(*arg_num)++;
	return(0);
}

int parse_bar_fg_color_rc(FILE *ferr, char *filename, int line, char *tag, char *value)
{
	char *code = 0;

	if (parse_color(ferr, value, fg_colors, &code) != 0) {
		print_esup(ferr, "In file: %s[%d]", filename, line);
		return(1);
	}
	d.bar_fg_color = code;
	return(0);
}

int parse_bar_bg_color_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num)
{
	char *code = 0;

	if ((*arg_num)+1 == argc) {
		print_error(ferr, "Missing color name after %s", argv[(*arg_num)]);
		print_esup(ferr, "Ignoring color code");
		return(0);
	}
	if (parse_color(ferr, argv[(*arg_num)+1], bg_colors, &code) != 0)
		print_esup(ferr, "Ignoring color code");
	else {
		d.bar_bg_color = code;
	}
	(*arg_num)++;
	return(0);
}

int parse_bar_bg_color_rc(FILE *ferr, char *filename, int line, char *tag, char *value)
{
	char *code = 0;

	if (parse_color(ferr, value, bg_colors, &code) != 0) {
		print_esup(ferr, "in file: %s[%d]", filename, line);
		return(1);
	}
	d.bar_bg_color = code;
	return(0);
}

int parse_bar_fg_bold_commandline(FILE *ferr, int argc, char *argv[], int *arg_num, int option_num)
{
	int r;

	r = parse_switch_commandline(
		argv[(*arg_num)],
		&options[option_num],
		&d.bar_fg_bold
		);
	return(r);
}

int parse_bar_fg_bold_rc(FILE *ferr, char *filename, int line, char *tag, char *value)
{
	int r;

	r = parse_switch_rc(ferr, filename, line, value,
		&d.bar_fg_bold
		);
	return(r);
}

int parse_args(FILE *ferr, int argc, char *argv[])
{
	int c = 0;

	for (c = 1; c < argc; c++) {
		int o = 0;

		o = isOpt(argv[c]);
		if (o == -1) {
			print_error(ferr, "Unknown command line option: %s", argv[c]);
			return(1);
		}
		if (options[o].cl_func == parse_kilo_commandline) {
			if (options[o].cl_func(ferr, argc, argv, &c, o) != 0)
				return(1);
		}
		else if (options[o].cl_func == parse_help_commandline) {
			if (options[o].cl_func(ferr, argc, argv, &c, o) != 0)
				return(1);
		}
		else if (options[o].cl_func == parse_version_commandline) {
			if (options[o].cl_func(ferr, argc, argv, &c, o) != 0)
				return(1);
		}
		else if (options[o].cl_func == parse_block_size_commandline) {
			if (options[o].cl_func(ferr, argc, argv, &c, 0) != 0)
				return(1);
		}
		else if (options[o].arg_description != 0) {
			c++;
		}
	}

	for (c = 1; c < argc; c++) {
		int o = 0;
		o = isOpt(argv[c]);
		if (options[o].cl_func(ferr, argc, argv, &c, o) != 0)
			return(1);
	}
	return(0);
}

int parse_rc_by_tag(FILE *ferr, char* filename, int line, char *tag, char *value)
{
	int o = 0;

	o = isOpt(tag);
	if (o == -1) {
		print_error(ferr, "Parse error in rc file: %s[%d]", filename, line);
		print_esup(ferr, "Unknown tag: %s", tag);
		return(1);
	}
	if (options[o].rc_func(ferr, filename, line, tag, value) != 0)
		return(1);
	return(0);
}

int parse_rcfile(FILE *ferr, char *filename)
{
	FILE *rcfile = 0;
	char tag[80];
	char value[80];
	int c;
	int line = 0;
	char ch;

	if (access(filename, F_OK) != 0)
		return(0);

	rcfile = fopen(filename, "r");
	if (rcfile == 0) {
		print_error(ferr, "Cannot open rc file for reading: %s", filename);
		return(1);
	}

	while (!feof(rcfile)) {
		line++;

		for (c = 0; c < 80; c++) {
			tag[c] = 0;
			value[c] = 0;
		}

		ch = fgetc(rcfile);
		if (feof(rcfile)) {
			return(0);
		}
		if (ch == '#') {
			while (!feof(rcfile) && (ch != '\n'))
				ch = fgetc(rcfile);
			continue;
		}
		c = 0;
		while (!feof(rcfile) && (isalnum((int)ch) || (ch == '-')) && (c < 80)) {
			tag[strlen(tag)] = ch;
			ch = fgetc(rcfile);
			c++;
		}

		if (c == 80) {
			print_error(ferr, "Parse error in rc file: %s[%d]", filename, line);
			print_esup(ferr, "Tag too long");
			return(1);
		}
		if (feof(rcfile)) {
			print_error(ferr, "Parse error in rc file: %s[%d]", filename, line);
			print_esup(ferr, "Unexpected end of file reached");
			return(1);
		}
		if (strlen(tag) == 0) {
			print_error(ferr, "Parse error in rc file: %s[%d]", filename, line);
			print_esup(ferr, "No tag");
			return(1);
		}
		if (ch != ':') {
			print_error(ferr, "Parse error in rc file: %s[%d]", filename, line);
			print_esup(ferr, "':' delimiter expected between tag and value");
			return(1);
		}

		ch = fgetc(rcfile);
		while (!feof(rcfile) && ((ch == ' ') || (ch == '\t')))
			ch = fgetc(rcfile);
		if (ch == '\n') {
			print_error(ferr, "Parse error in rc file: %s[%d]", filename, line);
			print_esup(ferr, "No value found after tag: %s", tag);
			return(1);
		}
		c = 0;
		while (!feof(rcfile) && (ch != '\n') && (c < 80)) {
			value[strlen(value)] = ch;
			ch = fgetc(rcfile);
			c++;
		}
		if (c == 80) {
			print_error(ferr, "Parse error in rc file: %s[%d]", filename, line);
			print_esup(ferr, "Value too long");
			return(1);
		}
		if (strlen(value) == 0) {
			print_error(ferr, "Parse error in rc file: %s[%d]", filename, line);
			print_esup(ferr, "No value for tag: %s", tag);
			return(1);
		}

		if (parse_rc_by_tag(ferr, filename, line, tag, value) != 0)
			return(1);
	}

	fclose(rcfile);
	return(0);
}

int parse_rcfiles(FILE *ferr)
{
	char filename[4096] = { 0 };
	char * home_dir = 0;

	home_dir = getenv("HOME");
	if (home_dir != 0) {
		sprintf(filename, "%.*s/.barrc", 4088, home_dir);
		if (parse_rcfile(ferr, filename) != 0)
			return(1);
	}

	sprintf(filename, "./.barrc");
	if (parse_rcfile(ferr, filename) != 0)
		return(1);
	
	return(0);
}

