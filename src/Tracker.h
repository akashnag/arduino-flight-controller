// ---------------------------------------------------------------------------------------------
//  Copyright (c) Akash Nag. All rights reserved.
//  Licensed under the MIT License. See LICENSE.md in the project root for license information.
// ---------------------------------------------------------------------------------------------

#ifndef Tracker_h
	#define Tracker_h

	#include "Location.h"
	#include <MPU6050.h>
	#include <TinyGPS.h>
	#include <SoftwareSerial.h>
	#include "Global.h"
	#include <SFE_BMP180.h>
	#include <Wire.h>

	class Tracker
	{
		private:
			float gpsLatitude, gpsLongitude;
			double gyroX, gyroY, gyroZ;
			double accelX, accelY, accelZ;
			bool isReady;
			int lastGyroTime;
	
			SoftwareSerial *gpsSerial;
			TinyGPS gps;			
			MPU6050 mpu;
			SFE_BMP180 pressureSensor;
						
		public:
			unsigned long fixAge;
		
			Tracker();
			void init();

			bool initGyro() const;
			
			bool measureGPSLocation();
			void measureGyro();
			void measureAcceleration();
			
			double getPitch();
			double getRoll();
			double getYaw();
			double getAccelerationX();
			double getAccelerationY();
			double getAccelerationZ();

			Location getGPSCoordinates();
			float getGPSVelocity();
			float getGPSAltitude();
			float getGPSBearing();
			
			float getAltitude();
	};
#endif
