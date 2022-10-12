#include "Boid.h"


#define NEARBY_DISTANCE			100.0f	// how far boids can see

Boid::Boid(XMFLOAT3 position)
{
	m_position = position;
	createRandomDirection();

	m_speed = 100;
}

Boid::~Boid()
{
}

void Boid::createRandomDirection()
{
	float x = (float)(rand() % 10);
	x -= 5;
	float y = (float)(rand() % 10);
	y -= 5;
	float z = 0;
	setDirection(XMFLOAT3(x, y, z));
}

void Boid::setDirection(XMFLOAT3 direction)
{
	XMVECTOR v = XMLoadFloat3(&direction);
	v = XMVector3Normalize(v);
	XMStoreFloat3(&m_direction, v);
}
void Boid::setSeperationMultiplier(const float value)
{
	m_seperationMultiplier = value;
}

void Boid::setAlignmentMultiplier(const float value)
{
	m_alignmentMultiplier = value;
}

void Boid::setCohesionMultiplier(const float value)
{
	m_alignmentMultiplier = value;
}

void Boid::setVelocityMultiplier(const float value)
{
	m_velocityMultiplier = value;
}

void Boid::update(float t, vecBoid* boidList)
{
	// create a list of nearby boids
	vecBoid nearBoids = nearbyBoids(boidList);

	// NOTE these functions should always return a normalised vector
	XMFLOAT3  vSeparation = calculateSeparationVector(&nearBoids);
	XMFLOAT3  vAlignment = calculateAlignmentVector(&nearBoids);
	XMFLOAT3  vCohesion = calculateCohesionVector(&nearBoids);

	// set me
	XMFLOAT3 vVelocity = XMFLOAT3(0, 0, 0);

	if(magnitudeFloat3(vSeparation) > 0.0f)
		vVelocity = addWeightedFloat3(vSeparation, m_seperationMultiplier/1000.0f);

	if (magnitudeFloat3(vAlignment) > 0.0f)
		vVelocity = addWeightedFloat3(vAlignment, m_alignmentMultiplier/100.0f);

	if (magnitudeFloat3(vCohesion) > 0.0f)
		vVelocity = addWeightedFloat3(vCohesion, m_cohesionMultiplier/100.0f);

	// set shark
	if (m_scale == 1) {
		vVelocity = addFloat3(vSeparation, vAlignment);
		vVelocity = addFloat3(vVelocity, vCohesion);
	}
	
	if (magnitudeFloat3(vVelocity) != 0.0f)
	{
		vVelocity = normaliseFloat3(vVelocity);

		
		//multiplyFloat3(vVelocity, m_velocityMultiplier/10.0f);
		//m_direction = addFloat3(m_direction, vVelocity);
		//m_direction = normaliseFloat3(m_direction);

		m_direction = addWeightedFloat3(vVelocity, m_velocityMultiplier);
	}
	
	XMFLOAT3 vDirection = multiplyFloat3(m_direction, t * m_speed);
	m_position = addFloat3(m_position, vDirection);

	DrawableGameObject::update(t);
}

XMFLOAT3 Boid::addWeightedFloat3(XMFLOAT3& source, const float multiplier)
{
	XMFLOAT3 vWeighted = multiplyFloat3(source, multiplier);
	return normaliseFloat3(vWeighted);
}

XMFLOAT3 Boid::calculateSeparationVector(vecBoid* boidList)
{
	XMFLOAT3 nearby = XMFLOAT3(0, 0, 0);
	if (boidList == nullptr)
		return nearby;

	// calculate average position of nearby

	float nearestDistance = NEARBY_DISTANCE;// 9999.0f;
	DrawableGameObject* nearest = nullptr;
	XMFLOAT3 directionNearestStored;

	for (Boid* boid : *boidList) {
		if (boid == this)
			continue;

		XMFLOAT3 mePos = m_position;
		XMFLOAT3 itPos = *boid->getPosition();

		XMFLOAT3 directionNearest = subtractFloat3(mePos, itPos);
		float d = magnitudeFloat3(directionNearest);
		if (d < nearestDistance)
		{
			nearestDistance = d;
			nearest = boid;
			directionNearestStored = directionNearest;
		}
	}

	if (nearest != nullptr) {
		return normaliseFloat3(directionNearestStored);
	}

	return m_direction;
}

XMFLOAT3 Boid::calculateAlignmentVector(vecBoid* boidList)
{
	XMFLOAT3 vDirection = XMFLOAT3(0, 0, 0);
	if (boidList == nullptr)
		return vDirection;

	// your code here
	for (Boid* boid : *boidList)
		vDirection = addFloat3(vDirection, *boid->getDirection());

	vDirection = divideFloat3(vDirection, boidList->size());

	return normaliseFloat3(vDirection); // return the normalised (average) direction of nearby drawables
}

XMFLOAT3 Boid::calculateCohesionVector(vecBoid* boidList)
{
	XMFLOAT3 nearby = XMFLOAT3(0, 0, 0);

	if (boidList == nullptr)
		return nearby;

	// calculate average position of nearby
	for (Boid* boid : *boidList) 
		nearby = addFloat3(nearby, *boid->getPosition());

	nearby = divideFloat3(nearby, boidList->size());
	nearby = subtractFloat3(nearby, m_position);

	return normaliseFloat3(nearby); // nearby is the direction to where the other drawables are
}



// use but don't alter the methods below

XMFLOAT3 Boid::addFloat3(XMFLOAT3& f1, XMFLOAT3& f2)
{
	XMFLOAT3 out;
	out.x = f1.x + f2.x;
	out.y = f1.y + f2.y;
	out.z = f1.z + f2.z;

	return out;
}

XMFLOAT3 Boid::subtractFloat3(XMFLOAT3& f1, XMFLOAT3& f2)
{
	XMFLOAT3 out;
	out.x = f1.x - f2.x;
	out.y = f1.y - f2.y;
	out.z = f1.z - f2.z;

	return out;
}

XMFLOAT3 Boid::multiplyFloat3(XMFLOAT3& f1, const float scalar)
{
	XMFLOAT3 out;
	out.x = f1.x * scalar;
	out.y = f1.y * scalar;
	out.z = f1.z * scalar;

	return out;
}

XMFLOAT3 Boid::divideFloat3(XMFLOAT3& f1, const float scalar)
{
	XMFLOAT3 out;
	out.x = f1.x / scalar;
	out.y = f1.y / scalar;
	out.z = f1.z / scalar;

	return out;
}

float Boid::magnitudeFloat3(XMFLOAT3& f1)
{
	return sqrt((f1.x * f1.x) + (f1.y * f1.y) + (f1.z * f1.z));
}

float Boid::distanceFloat3(XMFLOAT3& f1, XMFLOAT3& f2)
{
	return sqrt(pow(f2.x - f1.x, 2) + pow(f2.y - f1.y, 2) * 1.0);
}

XMFLOAT3 Boid::normaliseFloat3(XMFLOAT3& f1)
{
	float length = sqrt((f1.x * f1.x) + (f1.y * f1.y) + (f1.z * f1.z));

	f1.x /= length;
	f1.y /= length;
	f1.z /= length;

	return f1;
}

vecBoid Boid::nearbyBoids(vecBoid* boidList)
{
	vecBoid nearBoids;
	if (boidList->size() == 0)
		return nearBoids;

	for (Boid* boid : *boidList) {
		// ignore self
		if (boid == this)
			continue;

		// get the distance between the two
		XMFLOAT3 vB = *(boid->getPosition());
		XMFLOAT3 vDiff = subtractFloat3(m_position, vB);
		float l = magnitudeFloat3(vDiff);
		if (l < NEARBY_DISTANCE) {
			nearBoids.push_back(boid);
		}
	}

	return nearBoids;
}

void Boid::checkIsOnScreenAndFix(const XMMATRIX&  view, const XMMATRIX&  proj)
{
	XMFLOAT4 v4;
	v4.x = m_position.x;
	v4.y = m_position.y;
	v4.z = m_position.z;
	v4.w = 1.0f;

	XMVECTOR vScreenSpace = XMLoadFloat4(&v4);
	XMVECTOR vScreenSpace2 = XMVector4Transform(vScreenSpace, view);
	XMVECTOR vScreenSpace3 = XMVector4Transform(vScreenSpace2, proj);

	XMFLOAT4 v;
	XMStoreFloat4(&v, vScreenSpace3);
	v.x /= v.w;
	v.y /= v.w;
	v.z /= v.w;
	v.w /= v.w;

	float fOffset = 10; // a suitable distance to rectify position within clip space
	if (v.x < -1 || v.x > 1 || v.y < -1 || v.y > 1)
	{
		if (v.x < -1 || v.x > 1) {
			v4.x = -v4.x + (fOffset * v.x);
		}
		else if (v.y < -1 || v.y > 1) {
			v4.y = -v4.y + (fOffset * v.y);
		}

		// throw a bit of randomness into the mix
		//createRandomDirection();
	}

	// method 1 - appear on the other side
	m_position.x = v4.x;
	m_position.y = v4.y;
	m_position.z = v4.z;


	// method2 - bounce off sides and head to centre
	/*if (v.x < -1 || v.x > 1 || v.y < -1 || v.y > 1)
	{
		m_direction = multiplyFloat3(m_direction, -1);;
		m_direction = normaliseFloat3(m_direction);
	}*/

	return;
}