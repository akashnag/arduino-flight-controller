// ---------------------------------------------------------------------------------------------
//  Copyright (c) Akash Nag. All rights reserved.
//  Licensed under the MIT License. See LICENSE.md in the project root for license information.
// ---------------------------------------------------------------------------------------------

#include "Tracker.h"

Tracker::Tracker()
{
	gyroX = 0;
	gyroY = 0;
	gyroZ = 0;
}

void Tracker::init()
{
	pinMode(TRACKER_HCSR04_TRIG_PIN, OUTPUT);
	pinMode(TRACKER_HCSR04_ECHO_PIN, INPUT);
	gpsSerial = new SoftwareSerial(TRACKER_GPS_RX_PIN,TRACKER_GPS_TX_PIN);
	gpsSerial->begin(9600);
	initGyro();
	pressureSensor.begin();
}

bool Tracker::initGyro() const
{
	int count = 0;
	while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
	{
		count++;
		delay(500);

		if(count > MAX_GYRO_ATTEMPTS) return false;
	}

	mpu.calibrateGyro();
	mpu.setThreshold(3);
	return true;
}

float Tracker::getAltitude()
{
	// ADD CODE TO FUSE GPS-altitude, barometric-altitude and ultra-sonic altitude
	
	int duration;
	float distance;
	float sum = 0;
	const int N = 2;

	for(int i=1; i<=N; i++)
	{
		do {
			digitalWrite(TRACKER_HCSR04_TRIG_PIN, LOW);
		 	delayMicroseconds(2);
			
			digitalWrite(TRACKER_HCSR04_TRIG_PIN, HIGH);
			delayMicroseconds(10);
			digitalWrite(TRACKER_HCSR04_TRIG_PIN, LOW);
			
			duration = pulseIn(TRACKER_HCSR04_ECHO_PIN, HIGH);
			distance = (duration * 0.034)/2;
	
			if(distance < 0) delay(1);
		} while(distance < 0);
		sum+=distance;
	}
	return(sum/(N*100));		// average over N instances, and then convert from cm to metres
}

void Tracker::measureGyro()
{
	float temp = (millis() - lastGyroTime) / 1000.0;

	Vector normGyro = mpu.readNormalizeGyro();
	
	lastGyroTime = millis();
	
	/*
	gyroX = normGyro.XAxis;
	gyroY = normGyro.YAxis;
	gyroZ = normGyro.ZAxis;
	*/
	
	gyroX += normGyro.XAxis * temp;
	gyroY += normGyro.YAxis * temp;
	gyroZ += normGyro.ZAxis * temp;	
}

double Tracker::getPitch()
{
	measureGyro();
	return gyroY;
}

double Tracker::getRoll()
{
	measureGyro();
	return gyroX;
}

double Tracker::getYaw()
{
	measureGyro();
	return gyroZ;
}

void Tracker::measureAcceleration()
{
	Vector normAccel = mpu.readNormalizeAccel();

	accelX = normAccel.XAxis;
	accelY = normAccel.YAxis;
	accelZ = normAccel.ZAxis;
}

bool Tracker::measureGPSLocation()
{
	while(!gpsSerial->available());
	while(!gps.encode(gpsSerial->read()));

	gps.f_get_position(&gpsLatitude, &gpsLongitude, &fixAge);
	return(fixAge == TinyGPS::GPS_INVALID_AGE ? false : true);
}

Location Tracker::getGPSCoordinates()
{
	measureGPSLocation();
	return(Location(gpsLatitude, gpsLongitude));
}

float Tracker::getGPSVelocity()
{
	measureGPSLocation();
	return gps.f_speed_mps();
}

float Tracker::getGPSAltitude()
{
	measureGPSLocation();
	return gps.f_altitude();
}

float Tracker::getGPSBearing()
{
	measureGPSLocation();
	return gps.f_course();
}
