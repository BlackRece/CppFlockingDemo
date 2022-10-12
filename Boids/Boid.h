#pragma once
#include "DrawableGameObject.h"
class Boid :
	public DrawableGameObject
{
public:
	Boid(XMFLOAT3 position);
	~Boid();

	XMFLOAT3*							getDirection() { return &m_direction; }
	void								checkIsOnScreenAndFix(const XMMATRIX&  view, const XMMATRIX&  proj);
	void								update(float t, vecBoid* drawList);

	XMFLOAT3 addWeightedFloat3(XMFLOAT3& source, XMFLOAT3& desired, const float multiplier);


protected:
	void								setDirection(XMFLOAT3 direction);

	vecBoid								nearbyBoids(vecBoid* boidList);
	XMFLOAT3							calculateSeparationVector(vecBoid* drawList);
	XMFLOAT3							calculateAlignmentVector(vecBoid* drawList);
	XMFLOAT3							calculateCohesionVector(vecBoid* drawList);
	void								createRandomDirection();

	XMFLOAT3							addFloat3(XMFLOAT3& f1, XMFLOAT3& f2);
	XMFLOAT3							subtractFloat3(XMFLOAT3& f1, XMFLOAT3& f2);
	XMFLOAT3							normaliseFloat3(XMFLOAT3& f1);
	float								magnitudeFloat3(XMFLOAT3& f1);
	float								distanceFloat3(XMFLOAT3& f1, XMFLOAT3& f2);
	XMFLOAT3							multiplyFloat3(XMFLOAT3& f1, const float scalar);
	XMFLOAT3							divideFloat3(XMFLOAT3& f1, const float scalar);

	XMFLOAT3							addWeightedFloat3(XMFLOAT3& source, const float multiplier);
	XMFLOAT3							nearbyBoidsAveragePosition(vecBoid* boidList);

	XMFLOAT3							m_direction;
	//unsigned int*						m_nearbyDrawables;
	float								m_speed;
};

