#define _DEFAULT_SOURCE

#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/select.h>

#include "os-update.h"
#include "minui/minui.h"

#define IMAGES_MAX	30

static struct option options[] = {
	{"animate",     required_argument, 0, 'a'},
	{"progressbar", required_argument, 0, 'p'},
	{"stopafter",   required_argument, 0, 's'},
	{"text",        required_argument, 0, 't'},
	{"font",        required_argument, 0, 'f'},
	{"help",        no_argument,       0, 'h'},
	{0, 0, 0, 0},
};

/* ------------------------------------------------------------------------ */

static void
short_help(void)
{
	printf("  os-update-minui [OPTIONS] [IMAGE(s)]\n");
}

/* ------------------------------------------------------------------------ */

static void
print_help(void)
{
	printf("  yamui - tool to display progress bar, logo, or small animation on UI\n");
	printf("  Usage:\n");
	short_help();
	printf("    IMAGE(s)   - png picture file names in /res/images without .png extension\n");
	printf("                 NOTE: currently maximum of %d pictures supported\n",
	       IMAGES_MAX);
	printf("\n  OPTIONS:\n");
	printf("  --animate=PERIOD, -a PERIOD\n");
	printf("         Show IMAGEs (at least 2) in rotation over PERIOD ms\n");
	printf("  --progressbar=TIME, -p TIME\n");
	printf("         Show a progess bar over TIME milliseconds\n");
	printf("  --stopafter=TIME, -s TIME\n");
	printf("         Stop showing the IMAGE(s) after TIME milliseconds\n");
	printf("  --text=STRING, -t STRING\n");
	printf("         Show STRING on the screen\n");
	printf("  --font=N, -t N\n");
	printf("         Multiply the text font size by N (default 1)\n");
	printf("  --help, -h\n");
	printf("         Print this help\n");
}

/* ------------------------------------------------------------------------ */

/* Add text to both sides of the "flip" */
static void
add_text(char *text, unsigned int scale)
{
	int i = 0;
	int x = 20;
	int y = 20;
	if (!text)
		return;

	int text_width = gr_measure(text, scale);
	if (text_width < gr_fb_width()) {
	    x = (gr_fb_width() - text_width)/2;
	}

	y = gr_fb_height()*0.75;

	for (i = 0; i < 2; i++) {
		gr_color(255, 255, 255, 255);
		gr_text(x, y, text, 1, scale);
		gr_flip();
	}
}

/* ------------------------------------------------------------------------ */

int
main(int argc, char *argv[])
{
	int c, option_index;
	unsigned long int animate_ms = 0;
	unsigned long long int stop_ms = 0;
	unsigned long long int progress_ms = 0;
	unsigned long int scale_font_by = 1;
	char * text = NULL;
	char * images[IMAGES_MAX];
	int image_count = 0;
	int ret = 0;
	int i = 0;

	while (1) {
		c = getopt_long(argc, argv, "a:p:s:t:f:h", options,
				&option_index);
		if (c == -1)
			break;

		switch (c) {
		case 'a':
			printf("got animate %s ms\n", optarg);
			animate_ms = strtoul(optarg, (char **)NULL, 10);
			break;
		case 'p':
			printf("got progressbar %s ms\n", optarg);
			progress_ms = strtoull(optarg, (char **)NULL, 10);
			break;
		case 's':
			printf("got stop at %s ms\n", optarg);
			stop_ms = strtoull(optarg, (char **)NULL, 10);
			break;
		case 't':
			printf("got text \"%s\" to display\n", optarg);
			text = optarg;
			break;
		case 'f':
			printf("got font mulitplier \"%s\"\n", optarg);
			scale_font_by = strtoul(optarg, (char **)NULL, 10);
			break;
		case 'h':
			print_help();
			goto out;
			break;
		default:
			printf("getopt returned character code 0%o\n", c);
			short_help();
			goto out;
			break;
		}
	}

	while (optind < argc && image_count < IMAGES_MAX)
		images[image_count++] = argv[optind++];

	if (osUpdateScreenInit())
		return -1;

	/* In case there is text to add, add it to both sides of the "flip" */
	add_text(text, scale_font_by);

	if (image_count == 1 && !progress_ms) {
		ret = loadLogo(images[0]);
		if (ret) {
			printf("Image \"%s\" not found in /res/images/\n",
			       images[0]);
			goto cleanup;
		}

		showLogo();
		if (stop_ms)
			usleep(stop_ms * 1000);
		else
			select(0, NULL, NULL, NULL, NULL);

		goto cleanup;
	}

	if (image_count <= 1 && progress_ms) {
		if (image_count == 1)
			loadLogo(images[0]);
		i = 0;
		while (i <= 100){
			osUpdateScreenShowProgress(i);
			usleep(1000 * progress_ms / 100);
			i++;
		}

		goto cleanup;
	}

	if (image_count > 1 && progress_ms) {
		printf("Can only show one image with progressbar\n");
		goto cleanup;
	}

	if (animate_ms) {
		bool never_stop;
		long int time_left = stop_ms;
		int period = animate_ms / image_count;

		if (image_count < 2) {
			printf("Animating requires at least 2 images\n");
			goto cleanup;
		}

		if (stop_ms)
			never_stop = false;
		else
			never_stop = true;

		i = 0;
		while (never_stop || time_left > 0) {
			ret = loadLogo(images[i]);
			if (ret) {
				printf("\"%s\" not found in /res/images/\n",
				       images[i]);
				goto cleanup;
			}

			showLogo();
			usleep(1000 * period);
			time_left -= period;
			i++;
			i = i % image_count;
		}

		goto cleanup;
	}

	if (text) {
		if (stop_ms)
			usleep(1000 * stop_ms);
		else
			select(0, NULL, NULL, NULL, NULL);
		goto cleanup;
	}

cleanup:
	osUpdateScreenExit();
out:
	return ret;
}
