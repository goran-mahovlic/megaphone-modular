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

  double module_height = co_height + 2.4 + 2.4;
  double module_width = co_width + 1.8 + 1.8;

  // Make module size integer 10ths of an inch
  double frac =  module_height - ((int)(module_height / 2.54))*2.54;
  if (frac > 0.001) module_height = module_height + (2.54 - frac);

  frac =  module_width - ((int)(module_width / 2.54))*2.54;
  if (frac > 0.001) module_width = module_width + (2.54 - frac);

  int half_pin_count = module_height / 2.54;

  while ( (half_pin_count*2-1) < pins_used ) half_pin_count++;

  module_height = half_pin_count * 2.54;

  // Increase cut-out height if we added more pins than the original size
  // required.
  co_height = module_height - (2.4 + 2.4);
  
  fprintf(stderr,"INFO: Module size = %fx%f mm, %d pins each side.\n",
	  module_width, module_height, half_pin_count);

  printf("(footprint \"MegaCastle2x%d-Module-Mfoo\"\n"
	   "        (version 20240108)\n"
	   "	    (generator \"pcbnew\")\n"
	   "        (generator_version \"8.0\")\n"
	   "        (layer \"F.Cu\")\n"
	   "        (property \"Reference\" \"REF**\"\n"
	   "                (at 0 11.43 0)\n"   // XXX - Set position of REF**
	   "                (unlocked yes)\n"
	   "	 (layer \"F.SilkS\")\n"
	   "                (uuid \"08d1cbd6-6dbd-4696-9477-43e1380128be\")\n"
	   "                (effects\n"
	   "                        (font\n"
	   "                                (size 1 1)\n"
	   "                                (thickness 0.1)\n"
	   "                        )\n"
	   "                )\n"
	   "        )\n",
	   half_pin_count
	   );

	 printf("        (property \"Value\" \"MegaCastle2x%d-Module-Mfoo\"\n"
		"                (at 0 12.47 0)\n"
		"                (unlocked yes)\n"
		"                (layer \"F.Fab\")\n"
		"                (uuid \"7848b12f-b698-40ff-9b18-5d3dc1ad63e0\")\n"
		"                (effects\n"
		"                        (font\n"
		"                                (size 1 1)\n"
		"                                (thickness 0.15)\n"
		"                        )\n"
		"                )\n"
		"        )\n",
		half_pin_count);

	 printf("        (property \"Footprint\" \"\"\n"
		"                (at 0 0 0)\n"
		"                (layer \"F.Fab\")\n"
		"                (hide yes)\n"
		"                (uuid \"24d79921-7c8a-4bf5-b40a-0bc77c603d9c\")\n"
		"                (effects\n"
		"                        (font\n"
		"                                (size 1.27 1.27)\n"
		"                                (thickness 0.15)\n"
		"                        )\n"
		"                )\n"
		"        )\n"
		);

	 printf("        (property \"Datasheet\" \"\"\n"
		"                (at 0 0 0)\n"
		"                (layer \"F.Fab\")\n"
		"                (hide yes)\n"
		"                (uuid \"1a070192-ad50-44fd-8acb-aa45a22d9533\")\n"
		"                (effects\n"
		"                        (font\n"
		"                                (size 1.27 1.27)\n"
		"                                (thickness 0.15)\n"
		"                        )\n"
		"                )\n"
		"       )\n"
		);

	 printf("	 (property \"Description\" \"\"\n"
		"                (at 0 0 0)\n"
		"                (layer \"F.Fab\")\n"
		"                (hide yes)\n"
		"                (uuid \"d6dd6c62-1d04-4d09-a07a-575c94e93110\")\n"
		"                (effects\n"
		"                        (font\n"
		"                                (size 1.27 1.27)\n"
		"                                (thickness 0.15)\n"
		"                        )\n"
		"                )\n"
		"        )\n"
		"        (attr smd)\n"
		);

	 printf(
		"       (fp_rect\n"
		"                (start %.2f %.2f)\n"
		"                (end %.2f %.2f)\n"
		"                (stroke\n"
		"                        (width 0.1)\n"
		"                        (type default)\n"
		"                )\n"
		"                (fill none)\n"
		"                (layer \"Edge.Cuts\")\n"
		"                (uuid \"e0e166a7-b48f-4580-b2a9-bbb7fa1421f5\")\n"
		"        )\n",
		-module_width/2,module_height/2,
		module_width/2,-module_height/2		
		);

	 // Labels for PRY zones
	 printf(
		"        (fp_text user \"PRY\"\n"
		"                (at 0 %.2f 0)\n"
		"                (unlocked yes)\n"
		"                (layer \"B.SilkS\")\n"
		"                (uuid \"1c67120b-cd38-4dc1-b421-22e46cf03846\")\n"
		"                (effects\n"
		"                        (font\n"
		"                                (size 0.8 0.8)\n"
		"                                (thickness 0.1)\n"
		"                        )\n"
		"                        (justify bottom mirror)\n"
		"                )\n"
		"        )\n",
		-module_height/2.0 + 0.8 + 0.5
		);
	 printf(
		"        (fp_text user \"PRY\"\n"
		"                (at 0 %.2f 0)\n"
		"                (unlocked yes)\n"
		"                (layer \"B.SilkS\")\n"
		"                (uuid \"1c67120b-cd38-4dc1-b421-22e46cf03846\")\n"
		"                (effects\n"
		"                        (font\n"
		"                                (size 0.8 0.8)\n"
		"                                (thickness 0.1)\n"
		"                        )\n"
		"                        (justify bottom mirror)\n"
		"                )\n"
		"        )\n",
		module_height/2.0 - 0.3
		);

	 // Rectangles for PRY zones
	 float pry_height = 6.268592 - 4.8;
	 printf(
		"	 (fp_rect\n"
		"                (start -1.984241 %.3f)\n"
		"                (end 2.019516 %.3f)\n"
		"                (stroke\n"
		"                        (width 0.1)\n"
		"                        (type default)\n"
		"                )\n"
		"                (fill none)\n"
		"                (layer \"B.SilkS\")\n"
		"                (uuid \"966a509b-00f8-4763-8fcb-dbbd2ac08dcd\")\n"
		"        )\n",
		module_height/2 - 0.08 - pry_height,
		module_height/2 - 0.08
		);
	 printf(
		"	 (fp_rect\n"
		"                (start -1.984241 %.3f)\n"
		"                (end 2.019516 %.3f)\n"
		"                (stroke\n"
		"                        (width 0.1)\n"
		"                        (type default)\n"
		"                )\n"
		"                (fill none)\n"
		"                (layer \"B.SilkS\")\n"
		"                (uuid \"966a509b-00f8-4763-8fcb-dbbd2ac08dcd\")\n"
		"        )\n",
		-module_height/2 + 0.08 + pry_height,
		-module_height/2 + 0.08
		);

	 // Component and track exclusion zones for pry zones
	 printf(
		"	 (zone\n"
		"                (net 0)\n"
		"                (net_name \"\")\n"
		"                (layer \"B.Cu\")\n"
		"                (uuid \"2d931374-edf7-43e7-92d4-bb3e10af8b39\")\n"
		"                (name \"Upper Pry Zone\")\n"
		"                (hatch edge 0.5)\n"
		"                (connect_pads\n"
		"                        (clearance 0)\n"
		"                )\n"
		"                (min_thickness 0.25)\n"
		"                (filled_areas_thickness no)\n"
		"                (keepout\n"
		"                        (tracks not_allowed)\n"
		"                        (vias not_allowed)\n"
		"                        (pads not_allowed)\n"
		"                        (copperpour allowed)\n"
		"                        (footprints not_allowed)\n"
		"                )\n"
		"                (fill\n"
		"                        (thermal_gap 0.5)\n"
		"                        (thermal_bridge_width 0.5)\n"
		"                )\n"
		"                (polygon\n"
		"                        (pts\n"
		"                                (xy -1.984241 %f) (xy -1.984241 %f) (xy 2.019516 %f) (xy 2.019516 %f)\n"
		"                        )\n"
		"                )\n"
		"        )\n",
		module_height/2 - 0.08 - pry_height,
		module_height/2 - 0.08,
		module_height/2 - 0.08,
		module_height/2 - 0.08 - pry_height		
		);
	 
	 printf(
		"	 (zone\n"
		"                (net 0)\n"
		"                (net_name \"\")\n"
		"                (layer \"B.Cu\")\n"
		"                (uuid \"2d931374-edf7-43e7-92d4-bb3e10af8b39\")\n"
		"                (name \"Upper Pry Zone\")\n"
		"                (hatch edge 0.5)\n"
		"                (connect_pads\n"
		"                        (clearance 0)\n"
		"                )\n"
		"                (min_thickness 0.25)\n"
		"                (filled_areas_thickness no)\n"
		"                (keepout\n"
		"                        (tracks not_allowed)\n"
		"                        (vias not_allowed)\n"
		"                        (pads not_allowed)\n"
		"                        (copperpour allowed)\n"
		"                        (footprints not_allowed)\n"
		"                )\n"
		"                (fill\n"
		"                        (thermal_gap 0.5)\n"
		"                        (thermal_bridge_width 0.5)\n"
		"                )\n"
		"                (polygon\n"
		"                        (pts\n"
		"                                (xy -1.984241 %f) (xy -1.984241 %f) (xy 2.019516 %f) (xy 2.019516 %f)\n"
		"                        )\n"
		"                )\n"
		"        )\n",
		-module_height/2 + 0.08 + pry_height,
		-module_height/2 + 0.08,
		-module_height/2 + 0.08,
		-module_height/2 + 0.08 + pry_height		
		);
	 
	 // Exclusion zone for footprints outside of cut-out on rear
	 printf(
		"       (zone\n"
		"                (net 0)\n"
		"                (net_name \"\")\n"
		"                (layer \"B.Cu\")\n"
		"                (uuid \"ae5d0a55-9783-4f23-9325-7c00d54e0c32\")\n"
		"                (hatch edge 0.5)\n"
		"                (connect_pads\n"
		"                        (clearance 0)\n"
		"                )\n"
		"                (min_thickness 0.25)\n"
		"                (filled_areas_thickness no)\n"
		"                (keepout\n"
		"                        (tracks allowed)\n"
		"                        (vias allowed)\n"
		"                        (pads allowed)\n"
		"                        (copperpour allowed)\n"
		"                        (footprints not_allowed)\n"
		"                )\n"
		"                (fill\n"
		"                        (thermal_gap 0.5)\n"
		"                        (thermal_bridge_width 0.5)\n"
		"                )\n"
		"                (polygon\n"
		"                        (pts\n"
		"                                (xy %f %f) (xy %f %f) (xy %f %f) (xy %f %f)\n"
		"                        )\n"
		"                )\n"
		"        )\n",
		-module_width/2,module_height/2,
		-module_width/2,-module_height/2,
		-co_width/2,-module_height/2,
		-co_width/2,module_height/2		
		);
	 printf(
		"       (zone\n"
		"                (net 0)\n"
		"                (net_name \"\")\n"
		"                (layer \"B.Cu\")\n"
		"                (uuid \"ae5d0a55-9783-4f23-9325-7c00d54e0c32\")\n"
		"                (hatch edge 0.5)\n"
		"                (connect_pads\n"
		"                        (clearance 0)\n"
		"                )\n"
		"                (min_thickness 0.25)\n"
		"                (filled_areas_thickness no)\n"
		"                (keepout\n"
		"                        (tracks allowed)\n"
		"                        (vias allowed)\n"
		"                        (pads allowed)\n"
		"                        (copperpour allowed)\n"
		"                        (footprints not_allowed)\n"
		"                )\n"
		"                (fill\n"
		"                        (thermal_gap 0.5)\n"
		"                        (thermal_bridge_width 0.5)\n"
		"                )\n"
		"                (polygon\n"
		"                        (pts\n"
		"                                (xy %f %f) (xy %f %f) (xy %f %f) (xy %f %f)\n"
		"                        )\n"
		"                )\n"
		"        )\n",
		module_width/2,module_height/2,
		module_width/2,-module_height/2,
		co_width/2,-module_height/2,
		co_width/2,module_height/2		
		);
	 
	 
	 // Draw rectangle for component area on rear for cut-out
	 printf("         (fp_rect\n"
		"                (start %.2f %.2f)\n"
		"                (end %.2f %.2f)\n"
		"                (stroke\n"
		"                        (width 0.05)\n"
		"                        (type default)\n"
		"                )\n"
		"                (fill none)\n"
		"                (layer \"B.SilkS\")\n"
		"                (uuid \"a267cc7c-3519-453e-849e-e978b46e7c98\")\n"
		"	   )\n",
		-co_width/2,co_height/2,
		co_width/2,-co_height/2
		);

	 for(int i=0;i<half_pin_count;i++)
	   {
	     printf("       (pad \"%d\" thru_hole rect\n"
		    "                (at %f %f)\n"
		    "                (size 2.54 2)\n"
		    "                (drill oval 2 1.5)\n"
		    "                (property pad_prop_castellated)\n"
		    "                (layers \"*.Cu\" \"*.Mask\")\n"
		    "                (remove_unused_layers no)\n"
		    "                (thermal_bridge_angle 45)\n"
		    "                (uuid \"f85e55df-ccf1-4a29-ab8b-2b081b1da284\")\n"
		    "        )\n",
		    i+1,
		    -module_width/2,
		    -module_height/2 + 2.54/2.0 + 2.54*i
		    );
	   }

	 for(int i=0;i<half_pin_count;i++)
	   {
	     printf("       (pad \"%d\" thru_hole rect\n"
		    "                (at %f %f)\n"
		    "                (size 2.54 2)\n"
		    "                (drill oval 2 1.5)\n"
		    "                (property pad_prop_castellated)\n"
		    "                (layers \"*.Cu\" \"*.Mask\")\n"
		    "                (remove_unused_layers no)\n"
		    "                (thermal_bridge_angle 45)\n"
		    "                (uuid \"f85e55df-ccf1-4a29-ab8b-2b081b1da284\")\n"
		    "        )\n",
		    i+1+half_pin_count,
		    module_width/2,
		    -module_height/2 + 2.54/2.0 + 2.54*i
		    );
	   }
	 
	 

	 
	 printf(")\n");
  
  return 0;
}
