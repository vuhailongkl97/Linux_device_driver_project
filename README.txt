1. Build
In 'last_course_linux' directory run make. (this will build out button_ctl.ko, lcd_driver.ko,
in driver folder and snake binary file in user_main folder).

2. run
- insmod lcd_driver.ko
- insmod button_ctl.ko

- run ./user_main/snake	<speed>
	* speed: 0->100
