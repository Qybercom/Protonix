#include <Arduino.h>
#include <math.h>
#include <Wire.h>

#include "../Protonix.h"

#include "HSensorOrientationMPU6050.h"

using namespace Qybercom::Protonix;

int Hardware::HSensorOrientationMPU6050::_read (char reg) {
	if (!this->_init) return 0;

	short address = this->_config["address"];

	Wire.beginTransmission(address);
	Wire.write(reg);
	Wire.endTransmission(false);
	Wire.requestFrom(address, (char)2);

	int val = 0;

	if (Wire.available() >= 2) {
		val = Wire.read() << 8;
		val |= Wire.read();

		if (val & 0x8000) val -= 0x10000;
	}

	return val;
}

bool Hardware::HSensorOrientationMPU6050::_readRaw () {
	if (!this->_init) return false;

	float temperatureOffset = this->_config["temperatureOffset"];
	float temperatureTransform = this->_config["temperatureTransform"];
	float acceleromterSensibility = this->_config["accelerometerSensibility"];
	float gyroscopeScale = this->_config["gyroscopeScale"];
	float G = this->_config["G"];

	this->_axRaw = this->_read(0x3B);
	this->_ayRaw = this->_read(0x3D);
	this->_azRaw = this->_read(0x3F);
	this->_gxRaw = this->_read(0x43);
	this->_gyRaw = this->_read(0x45);
	this->_gzRaw = this->_read(0x47);

	this->_temperature = (this->_read(0x41) / temperatureTransform) + temperatureOffset;
	this->_ax = this->_axRaw * G / acceleromterSensibility;
	this->_ay = this->_ayRaw * G / acceleromterSensibility;
	this->_az = this->_azRaw * G / acceleromterSensibility;
	this->_gx = this->_gxRaw;
	this->_gy = this->_gyRaw;
	this->_gz = this->_gzRaw;

	const float alpha = 0.98f;

	float gx_rate = (this->_gxRaw - this->_offsetGX) / gyroscopeScale;
	float gy_rate = (this->_gyRaw - this->_offsetGY) / gyroscopeScale;
	float gz_rate = (this->_gzRaw - this->_offsetGZ) / gyroscopeScale;

	unsigned long now = millis();
	float dt = (now - this->_lastUpdate) * 0.001f;
	this->_lastUpdate = now;

	this->_gx += gx_rate * dt;
	this->_gy += gy_rate * dt;
	this->_gz += gz_rate * dt;

	float ax = _ax;
	float ay = _ay;
	float az = _az;
	float norm = sqrt(ax * ax + ay * ay + az * az);
	if (norm < 0.0001f) norm = 1.0f;
	ax /= norm; ay /= norm; az /= norm;

	float accX = atan2(ay, az) * 180.0f / 3.14159265f;
	float accY = atan2(-ax, sqrt(ay * ay + az * az)) * 180.0f / 3.14159265f;

	this->_gx = this->_gx * alpha + accX * (1.0f - alpha);
	this->_gy = this->_gy * alpha + accY * (1.0f - alpha);

	if (this->_gx < 0) this->_gx += 360; if (this->_gx >= 360) this->_gx -= 360;
	if (this->_gy < 0) this->_gy += 360; if (this->_gy >= 360) this->_gy -= 360;
	if (this->_gz < 0) this->_gz += 360; if (this->_gz >= 360) this->_gz -= 360;

	return true;
}

Hardware::HSensorOrientationMPU6050::HSensorOrientationMPU6050 (char address) {
	this->_init = false;

	this->_axRaw = 0;
	this->_ayRaw = 0;
	this->_azRaw = 0;
	this->_gxRaw = 0;
	this->_gyRaw = 0;
	this->_gzRaw = 0;

	this->_temperature = 0.0f;
	this->_ax = 0.0f;
	this->_ay = 0.0f;
	this->_az = 0.0f;
	this->_gx = 0.0f;
	this->_gy = 0.0f;
	this->_gz = 0.0f;

	this->_offsetGX = 0;
	this->_offsetGY = 0;
	this->_offsetGZ = 0;

	this->_lastUpdate = 0;

	this->_config["address"] = address;
	this->_config["calibrateAuto"] = true;
	this->_config["calibrateSamples"] = 500;
	this->_config["calibrateTimeout"] = 2;
	this->_config["temperatureOffset"] = 36.53f;
	this->_config["temperatureTransform"] = 340.0f;
	this->_config["accelerometerSensibility"] = 16384.0f;
	this->_config["gyroscopeScale"] = 131.0f;
	this->_config["G"] = 9.80665f;
}

float Hardware::HSensorOrientationMPU6050::Temperature () {
	return this->_temperature;
}

float Hardware::HSensorOrientationMPU6050::AccelerometerX () {
	return this->_ax;
}

float Hardware::HSensorOrientationMPU6050::AccelerometerY () {
	return this->_ay;
}

float Hardware::HSensorOrientationMPU6050::AccelerometerZ () {
	return this->_az;
}

float Hardware::HSensorOrientationMPU6050::GyroscopeX () {
	return this->_gx;
}

float Hardware::HSensorOrientationMPU6050::GyroscopeY () {
	return this->_gy;
}

float Hardware::HSensorOrientationMPU6050::GyroscopeZ () {
	return this->_gz;
}

bool Hardware::HSensorOrientationMPU6050::Calibrate (unsigned int samples, unsigned int timeout) {
	if (!this->_init) return false;

	long sum_gx = 0;
	long sum_gy = 0;
	long sum_gz = 0;
	int i = 0;

	while (i < samples) {
		this->_readRaw();

		sum_gx += this->_gxRaw;
		sum_gy += this->_gyRaw;
		sum_gz += this->_gzRaw;

		delay(timeout);

		i++;
	}

	this->_offsetGX = sum_gx / samples;
	this->_offsetGY = sum_gy / samples;
	this->_offsetGZ = sum_gz / samples;

	// TODO: signal

	return true;
}

String Hardware::HSensorOrientationMPU6050::HardwareSummary () {
	return "Orientation sensor MPU6050";
}

void Hardware::HSensorOrientationMPU6050::HardwareInitPre (Protonix* device) {
	(void)device;

	this->_capability("value", "temperature:float", "Temperature");
	this->_capability("value", "ax:float", "Accelerometer X axis");
	this->_capability("value", "ay:float", "Accelerometer Y axis");
	this->_capability("value", "az:float", "Accelerometer Z axis");
	this->_capability("value", "gx:float", "Gyroscope X axis");
	this->_capability("value", "gy:float", "Gyroscope Y axis");
	this->_capability("value", "gz:float", "Gyroscope Z axis");
}

bool Hardware::HSensorOrientationMPU6050::HardwareI2C () {
	return true;
}

void Hardware::HSensorOrientationMPU6050::HardwareI2CPre (Protonix* device) {
	(void)device;
}

void Hardware::HSensorOrientationMPU6050::HardwareI2CPost (Protonix* device) {
	(void)device;

	short address = this->_config["address"];

	Wire.beginTransmission(address);

	if (Wire.endTransmission() == 0) {
		Wire.beginTransmission(address);
		Wire.write(0x6B); // PWR_MGMT_1
		Wire.write(0x00);
		Wire.endTransmission();

		Wire.beginTransmission(address);
		Wire.write(0x1A); // CONFIG
		Wire.write(0x03); // DLPF = 44Hz
		Wire.endTransmission();

		this->_init = true;
	}

	if (this->_config["calibrateAuto"])
		this->Calibrate(
			(unsigned int)this->_config["calibrateSamples"],
			(unsigned int)this->_config["calibrateTimeout"]
		);
}

void Hardware::HSensorOrientationMPU6050::HardwarePipe (Protonix* device, short core) {
	(void)device;
	(void)core;

	if (!this->_init) return;

	this->_readRaw();

	this->_capability("temperature:float", String(this->_temperature));
	this->_capability("ax:float", String(this->_ax));
	this->_capability("ay:float", String(this->_ay));
	this->_capability("az:float", String(this->_az));
	this->_capability("gx:float", String(this->_gx));
	this->_capability("gy:float", String(this->_gy));
	this->_capability("gz:float", String(this->_gz));
}

void Hardware::HSensorOrientationMPU6050::HardwareOnCommand (Protonix* device, ProtonixCommand &command) {
	(void)device;
	String cmd = command.Argument(1);

	if (cmd == "calibrate") {
		Qybercom::Value samples = command.Argument(2);
		Qybercom::Value timeout = command.Argument(3);

		this->Calibrate(
			samples.IsUndefined() ? 500 : (unsigned int)samples,
			timeout.IsUndefined() ? 2 : (unsigned int)timeout
		);
	}
}