/*
Copyright (c) 2010-2014, Mathieu Labbe - IntRoLab - Universite de Sherbrooke
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Universite de Sherbrooke nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef ODOMETRY_H_
#define ODOMETRY_H_

#include <rtabmap/core/RtabmapExp.h>

#include <rtabmap/core/Transform.h>
#include <rtabmap/core/SensorData.h>
#include <rtabmap/core/Parameters.h>

namespace rtabmap {

class OdometryInfo;
class ParticleFilter;

class RTABMAP_EXP Odometry
{
public:
	enum Type {
		kTypeUndef = -1,
		kTypeLocalMap = 0,
		kTypeF2F = 1
	};

public:
	static Odometry * create(const ParametersMap & parameters);
	static Odometry * create(Type & type, const ParametersMap & parameters = ParametersMap());

public:
	virtual ~Odometry();
	Transform process(const SensorData & data, OdometryInfo * info = 0);
	virtual void reset(const Transform & initialPose = Transform::getIdentity());

	//getters
	const Transform & getPose() const {return _pose;}
	const std::string & getRoiRatios() const {return _roiRatios;}
	int getMinInliers() const {return _minInliers;}
	float getInlierDistance() const {return _inlierDistance;}
	int getIterations() const {return _iterations;}
	int getRefineIterations() const {return _refineIterations;}
	float getMinDepth() const {return _minDepth;}
	float getMaxDepth() const {return _maxDepth;}
	bool isInfoDataFilled() const {return _fillInfoData;}
	int getEstimationType() const {return _estimationType;}
	double getPnPReprojError() const {return _pnpReprojError;}
	int getPnPFlags() const {return _pnpFlags;}
	int getPnPRefineIterations() const {return _pnpRefineIterations;}
	const Transform & previousTransform() const {return previousTransform_;}
	bool isVarianceFromInliersCount() const {return _varianceFromInliersCount;}

private:
	virtual Transform computeTransform(const SensorData & image, OdometryInfo * info = 0) = 0;

	void initKalmanFilter();
	void updateKalmanFilter(float dt, float & x, float & y, float & z, float & roll, float & pitch, float & yaw);

private:
	std::string _roiRatios;
	int _minInliers;
	float _inlierDistance;
	int _iterations;
	int _refineIterations;
	float _minDepth;
	float _maxDepth;
	int _resetCountdown;
	bool _force2D;
	bool _holonomic;
	int _filteringStrategy;
	int _particleSize;
	float _particleNoiseT;
	float _particleLambdaT;
	float _particleNoiseR;
	float _particleLambdaR;
	bool _fillInfoData;
	int _estimationType;
	double _pnpReprojError;
	int _pnpFlags;
	int _pnpRefineIterations;
	bool _varianceFromInliersCount;
	float _kalmanProcessNoise;
	float _kalmanMeasurementNoise;
	Transform _pose;
	int _resetCurrentCount;
	double previousStamp_;
	Transform previousTransform_;
	Transform previousGroundTruthPose_;
	float distanceTravelled_;

	std::vector<ParticleFilter *> filters_;
	cv::KalmanFilter kalmanFilter_;

protected:
	Odometry(const rtabmap::ParametersMap & parameters);
};

} /* namespace rtabmap */
#endif /* ODOMETRY_H_ */
