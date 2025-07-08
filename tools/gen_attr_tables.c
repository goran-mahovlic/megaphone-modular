#include <stdio.h>

int x=0;
int count=0;

void start_table(char *name)
{
  printf("unsigned char %s[64]={\n",name);
  x=0;
  count=0;
}

void emit_val(unsigned char v)
{
  if (!x) { printf("  "); x=2; }
  printf("0x%02x",v); x+=4;
  if (count!=63) { printf(","); x++; }
  if ((count&7)==7) { printf("\n"); x=0; }
  count++;
}

void end_table(void)
{
  if (x) printf("\n");
  printf("};\n\n");
}

int main(int argc,char **argv)
{

  start_table("screen_ram_1_left");
  for(int i=0;i<64;i++)
    {
      int width = i&31;
      int is_colour=i&32;
      int width_subtract = 0;
      if (width<8) width_subtract = 8 - width;
      // zero-width glyphs don't exist
      if (width_subtract >= 8) width_subtract = 7;
      
      unsigned char val = 0x00;
      val |= (width_subtract&7)<<5;

      emit_val(val);
    }
  end_table();

  start_table("screen_ram_1_right");
  for(int i=0;i<64;i++)
    {
      int width = i&31;
      int is_colour=i&32;
      int width_subtract = 0;
      if (width<16) width_subtract = 16 - width;
      else if (width<32) width_subtract = 32 - width;
      if (width_subtract >= 16) width_subtract = 15;

      if (width<8) width_subtract = 0;
      
      unsigned char val = 0x00;
      val |= (width_subtract&7)<<5;

      emit_val(val);
    }
  end_table();

  start_table("colour_ram_0_left");
  for(int i=0;i<64;i++)
    {
      int width = i&31;
      int is_colour=i&32;
      int width_subtract = 0;
      if (width<8) width_subtract = 8 - width;
      // zero-width glyphs don't exist
      if (width_subtract == 8) width_subtract = 7;
      
      unsigned char val = 0x00;
      val |= is_colour? 0x00 : 0x20;
      val |= (width>15)? 0x08 : 0x00;
      val |= (width_subtract&8) ? 0x04 : 0x00;
      
      emit_val(val);
    }
  end_table();
  
  start_table("colour_ram_0_right");
  for(int i=0;i<64;i++)
    {
      int width = i&31;
      int is_colour=i&32;
      int width_subtract = 0;
      if (width<16) width_subtract = 16 - width;
      else if (width<32) width_subtract = 32 - width;
      // zero-width glyphs don't exist
      if (width_subtract >= 16) width_subtract = 15;
      
      unsigned char val = 0x00;
      val |= is_colour? 0x00 : 0x20;
      val |= (width>15)? 0x08 : 0x00;
      val |= (width_subtract&8) ? 0x04 : 0x00;
      
      emit_val(val);
    }
  end_table();
  
  start_table("colour_ram_1");
  for(int i=0;i<64;i++)
    {
      int is_colour=i&32;
      
      unsigned char val = 0x00;
      val |= is_colour? 0x60 : 0x00;  // Select alternate palette for colour glyphs
      
      emit_val(val);
    }
  end_table();

 
}
