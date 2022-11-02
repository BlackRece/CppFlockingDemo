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

	void								setSeperationMultiplier(const float value);
	void								setAlignmentMultiplier(const float value);
	void								setCohesionMultiplier(const float value);

	void								setVelocityMultiplier(const float value);
	void								setRange(const float value);

protected:
	void								setDirection(XMFLOAT3 direction);


	vecBoid								nearbyBoids(vecBoid* boidList);
	XMFLOAT3							calculateSeparationVector_Group(vecBoid* drawList);
	XMFLOAT3							calculateSeparationVector_Nearest(vecBoid* drawList);
	XMFLOAT3							calculateAlignmentVector(vecBoid* drawList);
	XMFLOAT3							calculateCohesionVector(vecBoid* drawList);
	void								createRandomDirection();

	XMFLOAT3							addFloat3(XMFLOAT3& f1, XMFLOAT3& f2);
	XMFLOAT3							subtractFloat3(XMFLOAT3& f1, XMFLOAT3& f2);
	XMFLOAT3							normaliseFloat3(XMFLOAT3& f1);
	float								magnitudeFloat3(XMFLOAT3& f1);
	float								dotProduct(XMFLOAT3& f1, XMFLOAT3& f2);
	float								distanceFloat3(XMFLOAT3& f1, XMFLOAT3& f2);
	XMFLOAT3							multiplyFloat3(XMFLOAT3& f1, const float scalar);
	XMFLOAT3							divideFloat3(XMFLOAT3& f1, const float scalar);

	XMFLOAT3							addWeightedFloat3(XMFLOAT3& dest, XMFLOAT3& source, const float multiplier);

	XMFLOAT3							m_direction;
	XMFLOAT3							m_velocity;
	float								m_speed;

private:
	float								m_velocityMultiplier;
	float								m_range;

	float								m_seperationMultiplier;
	float								m_alignmentMultiplier;
	float								m_cohesionMultiplier;
};

