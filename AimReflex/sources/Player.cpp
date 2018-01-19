#include "../headers/Player.h"


Player::Player()
{
	pTargetCount = START_TARGET_COUNT;

	pHit = false;
	pMiss = false;

	totalTargetsTimeAiming = 0.f;

	pScore = 0;
	pHits = 0;
	pMisses = 0;
	pMissedTargets = 0;
	pMaxCombo = 0;
	pTotalTargets = START_TARGET_COUNT;
	pAiming = 0.f;

	combo = 0;

	for (int i = 0; i < LEVEL_COUNT; ++i)
	{//Disable all levels on start
		levelEnabled[i] = false;
	}
}

void Player::checkScore(std::vector<Target> *target)
{
	bool createNew = false;

	if (pScore > 6000 && !levelEnabled[0])
	{
		Target newTarget;
		target->push_back(newTarget);
		levelEnabled[0] = true;
	}

	if (pScore > 9000 && !levelEnabled[1])
	{
		Target newTarget;
		target->push_back(newTarget);
		levelEnabled[1] = true;
	}

	if (pScore > 12000 && !levelEnabled[2])
	{
		Target newTarget;
		target->push_back(newTarget);
		levelEnabled[2] = true;
	}
}

void Player::checkTargetLifeTime(std::vector<Target> target)
{
	for (std::vector<Target>::size_type i = 0; i != target.size(); ++i)
	{
		if (target[i].tHit || target[i].targetMiss)
		{
			pAiming += target[i].getLifeTime();
		}
	}
}

int Player::getScore()
{
	return pScore;
}

int Player::getTargetCount()
{
	return pTargetCount;
}

void Player::hit()
{
	pScore += HIT;
	pHits++;
	combo++;
	if (combo > pMaxCombo)
	{
		pMaxCombo = combo;
	}
	pTotalTargets++;
}

void Player::miss()
{
	pScore -= MISS;
	pMisses++;
	combo = 0;
	pTotalTargets++;
}

void Player::targetMiss()
{
	pScore -= TARGET_MISS;
	pMissedTargets++;
	combo = 0;
	pTotalTargets++;
}

void Player::setMaxCombo(int combo)
{
	pMaxCombo = combo;
}

void Player::setAiming(float aiming)
{
	pAiming = aiming;
}

int Player::getHits()
{
	return pHits;
}

int Player::getMisses()
{
	return pMisses;
}

int Player::getMissedTargets()
{
	return pMissedTargets;
}

int Player::getMaxCombo()
{
	return pMaxCombo;
}

int Player::getTotalTargets()
{
	return pTotalTargets;
}

float Player::getAiming()
{
	if (pAiming == 0.f || pHits == 0.f)
	{
		return 0;
	}
	else
	{
		return pAiming / (float)pHits / 1000.f;
	}
}