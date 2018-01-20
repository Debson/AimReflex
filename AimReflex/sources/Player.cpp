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
	pCurrentLevel = 0;
	pCombo = 0;

	for (int i = 0; i < LEVEL_COUNT; ++i)
	{//Disable all levels on start
		levelEnabled[i] = false;
	}
}

void Player::checkScore(std::vector<Target> *target)
{
	if (pScore > 1 * 12000 && !levelEnabled[0])
	{
		Target newTarget;
		target->push_back(newTarget);
		levelEnabled[0] = true;
	}

	if (pScore > 2 * 12000 && !levelEnabled[1])
	{
		Target newTarget;
		target->push_back(newTarget);
		levelEnabled[1] = true;
	}
	if (pScore > 5 * 12000 && !levelEnabled[2])
	{
		Target newTarget;
		target->push_back(newTarget);
		levelEnabled[2] = true;
	}
	if (pScore > 8 * 12000 && !levelEnabled[3])
	{
		Target newTarget;
		target->push_back(newTarget);
		levelEnabled[3] = true;
	}
	if (pScore > 13 * 12000 && !levelEnabled[4])
	{
		Target newTarget;
		target->push_back(newTarget);
		levelEnabled[4] = true;
	}
	if (pScore > 21 * 12000 && !levelEnabled[5])
	{
		Target newTarget;
		target->push_back(newTarget);
		levelEnabled[5] = true;
	}
	if (pScore > 30 * 12000 && !levelEnabled[6])
	{
		Target newTarget;
		target->push_back(newTarget);
		levelEnabled[6] = true;
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
	pCombo++;
	if (pCombo > pMaxCombo)
	{
		pMaxCombo = pCombo;
	}
	pTotalTargets++;
}

void Player::miss()
{
	pScore -= MISS;
	pMisses++;
	pCombo = 0;
	pTotalTargets++;
}

void Player::targetMiss()
{
	pScore -= TARGET_MISS;
	pMissedTargets++;
	pCombo = 0;
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