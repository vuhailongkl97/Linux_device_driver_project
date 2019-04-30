#include "mpu6050.h"

float dpsPerDigit, rangePerDigit;

int mpu_init(struct i2c_client *client, mpu6050_dps_t scale, mpu6050_range_t range) {
	int ret = 0;

	/* step 1 check address device */
	if (mpu_read_value(client, MPU6050_REG_WHO_AM_I) != 0x68 ){
		return -1;
	}
	/* step 2 set clock source */
	ret = set_clock_source(client, MPU6050_CLOCK_PLL_XGYRO);
	if ( ret < 0) {
		pr_info("set clock source fail\n");
		return -1;
	}
	else {
		pr_info("set clock source ok\n");
	}
	/* set Scale */
	ret = setScale(client, scale);
	if ( ret < 0) {
		pr_info("set scale fail\n");
		return -1;
	}
	else {
		pr_info("set scale  ok\n");
	}
	
	ret = setRange(client, range);
	if ( ret < 0) {
		pr_info("set range fail\n");
		return -1;
	}
	else {
		pr_info("set range  ok\n");
	}

	ret = setSleepEnabled(client, 0);
	if ( ret < 0) {
		pr_info("set sleep enable fail\n");
		return -1;
	}
	else {
		pr_info("set sleep enable ok\n");
	}

	return 0;

}
int setScale(struct i2c_client *client, mpu6050_dps_t scale)
{
    uint8_t value;

    switch (scale)
    {
	case MPU6050_SCALE_250DPS:
	    dpsPerDigit = .007633f;
	    break;
	case MPU6050_SCALE_500DPS:
	    dpsPerDigit = .015267f;
	    break;
	case MPU6050_SCALE_1000DPS:
	    dpsPerDigit = .030487f;
	    break;
	case MPU6050_SCALE_2000DPS:
	    dpsPerDigit = .060975f;
	    break;
	default:
	    break;
    }

    value = mpu_read_value(client, MPU6050_REG_GYRO_CONFIG);
    value &= 0b11100111;
    value |= (scale << 3);
    return mpu_write_value(client, MPU6050_REG_GYRO_CONFIG, value);
}

int setRange(struct i2c_client *client, mpu6050_range_t range)
{
    uint8_t value;

    switch (range)
    {
	case MPU6050_RANGE_2G:
	    rangePerDigit = .000061f;
	    break;
	case MPU6050_RANGE_4G:
	    rangePerDigit = .000122f;
	    break;
	case MPU6050_RANGE_8G:
	    rangePerDigit = .000244f;
	    break;
	case MPU6050_RANGE_16G:
	    rangePerDigit = .0004882f;
	    break;
	default:
	    break;
    }

    value = mpu_read_value(client, MPU6050_REG_ACCEL_CONFIG);
    value &= 0b11100111;
    value |= (range << 3);
    return mpu_write_value(client, MPU6050_REG_ACCEL_CONFIG, value);
}
int set_clock_source(struct i2c_client *client, mpu6050_clockSource_t source){
	unsigned char value;

	value = mpu_read_value(client, MPU6050_REG_PWR_MGMT_1);
	value &= 0b11111000;
	value |= source;
	return mpu_write_value(client, MPU6050_REG_PWR_MGMT_1, value);
}
int mpu_read_value(struct i2c_client *client , int reg) {
        if (reg < 0x10)
                return i2c_smbus_read_byte_data(client, reg);
        else
                return i2c_smbus_read_word_data(client, reg);
}
int mpu_write_value(struct i2c_client *client, u8 reg, u16 value)
{
	if (reg == 0x10)	
		return -EINVAL;
	else if (reg < 0x10)	
		return i2c_smbus_write_byte_data(client, reg, value);
	else			
		return i2c_smbus_write_word_data(client, reg, value);
}
int mpu_write_register_bit(struct i2c_client *client, uint8_t reg, uint8_t pos, bool state)
{
	unsigned char value;

	value = mpu_read_value(client, reg);
	if (state)
	    value |= (1 << pos);
	else 
	    value &= ~(1 << pos);
	return mpu_write_value(client, reg, value);
}
int setSleepEnabled(struct i2c_client *client, char state)
{
	return mpu_write_register_bit(client, MPU6050_REG_PWR_MGMT_1, 6, state);
}

int read_raw_accel(struct i2c_client *client ,struct mpu_xyz *mpu) {
	unsigned char value = MPU6050_REG_ACCEL_XOUT_H;
	unsigned char buf[6] ;
	int ret;
	int Xasis ,Yasis, Zasis;

	
	ret = i2c_master_send(client, &value, 1);
	if (ret < 0 ) {
		pr_err("i2c_master_send fail\n");
		return -1;
	}
	
	ret = i2c_master_recv(client, buf, 6);
	if (ret < 0 ) {
		pr_err("i2c_master_recv fail\n");
		return -1;
	}
	Xasis =((unsigned char)buf[0] << 8) | (unsigned char)buf[1];
	Yasis =((unsigned char)buf[2] << 8) | (unsigned char)buf[3];
	Zasis =((unsigned char)buf[4] << 8) | (unsigned char)buf[5];
	
	//pr_info("%5d  %5d  %5d\n", Xasis, Yasis, Zasis);

	mpu->x_asis = Xasis;
	mpu->y_asis = Yasis;
	mpu->z_asis = Zasis;
	
	return 0;
}



int read_x_asis(void) {

	return 0;
}

int read_y_asis(void) {
	
	return 0;
}
int read_z_asis(void) {
	
	return 0;
}

