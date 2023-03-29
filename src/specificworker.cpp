/*
 *    Copyright (C) 2023 by YOUR NAME HERE
 *
 *    This file is part of RoboComp
 *
 *    RoboComp is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    RoboComp is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with RoboComp.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "specificworker.h"

/**
* \brief Default constructor
*/
SpecificWorker::SpecificWorker(TuplePrx tprx, bool startup_check) : GenericWorker(tprx)
{
	this->startup_check_flag = startup_check;
	// Uncomment if there's too many debug messages
	// but it removes the possibility to see the messages
	// shown in the console with qDebug()
//	QLoggingCategory::setFilterRules("*.debug=false\n");
}

/**
* \brief Default destructor
*/
SpecificWorker::~SpecificWorker()
{
	std::cout << "Destroying SpecificWorker" << std::endl;
}

bool SpecificWorker::setParams(RoboCompCommonBehavior::ParameterList params)
{
//	THE FOLLOWING IS JUST AN EXAMPLE
//	To use innerModelPath parameter you should uncomment specificmonitor.cpp readConfig method content
//	try
//	{
//		RoboCompCommonBehavior::Parameter par = params.at("InnerModelPath");
//		std::string innermodel_path = par.value;
//		innerModel = std::make_shared(innermodel_path);
//	}
//	catch(const std::exception &e) { qFatal("Error reading config params"); }






	return true;
}

void SpecificWorker::initialize(int period)
{
	std::cout << "Initialize worker" << std::endl;
	this->Period = period;
	if(this->startup_check_flag)
	{
		this->startup_check();
	}
	else
	{
		timer.start(Period);
	}

}

void SpecificWorker::compute()
{
	//computeCODE
	//QMutexLocker locker(mutex);
	//try
	//{
	//  camera_proxy->getYImage(0,img, cState, bState);
	//  memcpy(image_gray.data, &img[0], m_width*m_height*sizeof(uchar));
	//  searchTags(image_gray);
	//}
	//catch(const Ice::Exception &e)
	//{
	//  std::cout << "Error reading from Camera" << e << std::endl;
	//}

    std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
    deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - previousTime).count() / 1000.0f;

    RoboCompIMU::DataImu imu_data = imu_proxy->getDataImu();
    float pitch = imu_data.rot.Pitch;

    // Sistema de control
    error = pitch - pitchObj;
    integral = integral + (error * deltaTime);
    derivate = (error - previousError) / deltaTime;

    kd = 0.125 * kp * Td; // Siendo Td el tiempò de derivacion 
    ki = ;

    float output = kp * error + ki * integral + kd * derivate;
    if(output < 0.3 && output > -0.3)
    {
        output = (output > 0) ? 0.3 : -0.3;
    }

    cout << "## DEBUG ##" << endl;

    cout << "pitch: " << pitch << endl;
    cout << "error: " << error << endl;
    cout << "integral: " << integral << endl;
    cout << "derivate: " << derivate << endl;

    cout << "###########" << endl;

    goal_velocity.velocity = output;

    cout << "Output: " << output << endl;

    if(!isnan(output))
    {

        jointmotorsimple_proxy->setVelocity("", goal_velocity);
    }

    previousTime = currentTime;
    previousError = error;
}

int SpecificWorker::startup_check()
{
	std::cout << "Startup check" << std::endl;
	QTimer::singleShot(200, qApp, SLOT(quit()));
	return 0;
}

/**************************************/
// From the RoboCompIMU you can call this methods:
// this->imu_proxy->getAcceleration(...)
// this->imu_proxy->getAngularVel(...)
// this->imu_proxy->getDataImu(...)
// this->imu_proxy->getMagneticFields(...)
// this->imu_proxy->getOrientation(...)
// this->imu_proxy->resetImu(...)

/**************************************/
// From the RoboCompIMU you can use this types:
// RoboCompIMU::Acceleration
// RoboCompIMU::Gyroscope
// RoboCompIMU::Magnetic
// RoboCompIMU::Orientation
// RoboCompIMU::DataImu

/**************************************/
// From the RoboCompJointMotorSimple you can publish calling this methods:
// this->jointmotorsimple_pubproxy->getMotorParams(...)
// this->jointmotorsimple_pubproxy->getMotorState(...)
// this->jointmotorsimple_pubproxy->setPosition(...)
// this->jointmotorsimple_pubproxy->setVelocity(...)
// this->jointmotorsimple_pubproxy->setZeroPos(...)

/**************************************/
// From the RoboCompJointMotorSimple you can use this types:
// RoboCompJointMotorSimple::MotorState
// RoboCompJointMotorSimple::MotorParams
// RoboCompJointMotorSimple::MotorGoalPosition
// RoboCompJointMotorSimple::MotorGoalVelocity

