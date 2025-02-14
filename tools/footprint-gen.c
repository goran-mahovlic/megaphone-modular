#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>

void usage(void)
{
  fprintf(stderr,"footprint-gen <cut-out width> <cut-out height> <number of pins> <variant>\n");
  exit(-1);
}

int main(int argc, char **argv)
{
  if (argc!=5) {
    usage();
  }
  
  float co_width=atof(argv[1]);
  float co_height=atof(argv[2]);
  float pins_used=atof(argv[3]);
  float variant=atof(argv[4]);

  double module_height = co_height + 1.6 + 1.6;
  double module_width = co_width + 1.8 + 1.8;

  // Make module size integer 10ths of an inch
  double frac =  module_height - ((int)(module_height / 2.54))*2.54;
  if (frac > 0.001) module_height = module_height + (2.54 - frac);

  frac =  module_width - ((int)(module_width / 2.54))*2.54;
  if (frac > 0.001) module_width = module_width + (2.54 - frac);

  int half_pin_count = module_height / 2.54;

  while ( (half_pin_count*2-1) < pins_used ) half_pin_count++;

  module_height = half_pin_count * 2.54;
  
  fprintf(stderr,"INFO: Module size = %fx%f mm, %d pins each side.\n",
	  module_width, module_height, half_pin_count);
  
  return 0;
}
