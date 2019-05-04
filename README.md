# linux_final_course_project

link demo youtube: https://www.youtube.com/watch?v=CphT0TvB78E
1. Build
In 'last_course_linux' directory run make. (this will build out button_ctl.ko, lcd_driver.ko,
in driver folder and snake binary file in user_main folder).

2. run
	+ "make"
	+ sudo ./setup

- run ./user_main/snake	<speed> <mode>
	* speed: 0->100 , 30 is recommend when using mpu6050

	note :
	+ to using button: ./user_main/snake 30 button
	+ to using mpu6050: ./user_main/snake 30

note : 
	this default setup  for control snake with mpu6050 sensor
