#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "lcd_lib.h"

int main()
{
	lcd_clear_screen();
	lcd_draw_circle(12, 5, 4, Pixel_Set);
	lcd_draw_rect(24, 5, 48,10 , Pixel_Set);

	lcd_gotoxy(20, 24);
	draw_string("Huy Nam", Pixel_Set, FontSize_5x7);
	lcd_gotoxy(20, 34);
	draw_string("Thanh Tung", Pixel_Set, FontSize_5x7);
	lcd_send_buff();
	return 0;
}
