/*
 * Copyright (C) 2015 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * This is an example to display text on the Grove
 * LCD RGB Display panel.
 */

#include <jhd1313m1.h>
#include <mraa.hpp>

#include <getopt.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>

#define DEFAULT_COLOR_RED    255
#define DEFAULT_COLOR_GREEN  255
#define DEFAULT_COLOR_BLUE   255
#define DEFAULT_DISPLAY_TEXT "hello world"

// Structure to hold the decoded command line options
struct pgm_options {
  uint8_t      red, blue, green;
  std::string  display_text;
};

// Be sure to keep the options for longopts and shortopts in the same order
// so that Usage() is correct.
static struct option longopts[] = {
  {"help",  no_argument,        NULL,   '?'},
  {"text",  required_argument,  NULL,   't'},
  {"red",   no_argument,        NULL,   'r'},
  {"green", no_argument,        NULL,   'g'},
  {"blue",  no_argument,        NULL,   'b'},
  {NULL,    0,                  NULL,    0 }
};
static char shortopts[] = "?t:rgb";

// Describes the options for this program.
void Usage(char *pgm_name) {
  printf("Usage: %s [options...]\n", pgm_name);
  printf("Prints a message on the Grove LCD RGB Display\n");
  printf("Options:\n");
  for (int i = 0, j = 0; longopts[i].name; i++, j++) {
    if (shortopts[j] == ':')
      j++;
    printf(" --%-6s or -%c\n", longopts[i].name, shortopts[j]);
  }
  return;
}

// Processes all command line options.
//   sets the options members for commnd line options
//   returns (0) on success, -1 otherwise.
int ReadOpts(int argc, char **argv, struct pgm_options *options) {
  int  ch = 0;
  bool color_specified = false;

  while ((ch = getopt_long(argc, argv, shortopts, longopts, NULL)) != -1) {
    switch (ch) {
      case 'r':
        options->red = 255;
        color_specified = true;
        break;
      case 'g':
        options->green = 255;
        color_specified = true;
        break;
      case 'b':
        options->blue = 255;
        color_specified = true;
        break;
      case 't':
        options->display_text = optarg;
        break;
      default:
        Usage(argv[0]);
        return -1;
    }
  }
  argc -= optind;
  argv += optind;

  if (!color_specified) {
    options->red   = DEFAULT_COLOR_RED;
    options->green = DEFAULT_COLOR_GREEN;
    options->blue  = DEFAULT_COLOR_BLUE;
  }
  if (options->display_text.length() == 0)
    options->display_text = DEFAULT_DISPLAY_TEXT;

  return 0;
}

int main(int argc, char* argv[]) {
  pgm_options options = {0, 0, 0, ""};

  if (ReadOpts(argc, argv, &options) < 0)
    return 1;

  upm::Jhd1313m1 display(mraa_get_default_i2c_bus(MRAA_MAIN_PLATFORM_OFFSET));
  display.write(options.display_text.c_str());
  display.setColor(options.red, options.green, options.blue);
  sleep(5);

  display.displayOff();
  return 0;
}
