#include "fight.hpp"

Fight::Fight(std::shared_ptr<Player> player_, std::shared_ptr<Creature> enemy_)
	: player(player_)
{
	attackSet = false;
	activeAttack = nullptr;
	activeAttackType = nullptr;
	selectedPotion = nullptr;
	enemyAttackHealthDamage = 0;
	enemyAttackAttributeDamage = 0;
	attackSet = false;

	changeTabActive = false;
	potionTakingActive = false;
}


Fight::~Fight()
{
}

Creature Fight::fight()
{
	/* MAIN FIGHT LOOP */
	while (player->hp > 0 && enemy->accuracy > 0)
	{
		std::shared_ptr<Creature> creature1 = nullptr;
		std::shared_ptr<Creature> creature2 = nullptr;

		float firstAttackTemp = determineFirstAttack();

		if (firstAttackTemp == 1)
		{
			creature1 = player;
			creature2 = enemy;
			// this.attackScreen = AttackScreen.MAIN;
		}
		else if (firstAttackTemp == 2)
		{
			creature1 = enemy;
			creature2 = player;
			// this.attackScreen = AttackScreen.WAITING;
		}

		// thread.sleep(1000);

		//perform Attack of creature in first round
		attackControl(creature1, creature2);

		// potion effects for a creature are applied after its attack
		potionEffects(creature1);

		if (player->hp <= 0 || enemy->hp <= 0)
		{
			break;
		}

		// set to null between attacks of player and enemy to determine if attack was already chosen
		activeAttack = nullptr;
		activeAttackType = nullptr;


		/*

		//change attack screen
		if (firstAttackTemp == 1)
		{
			this.attackScreen = AttackScreens.WAITING;
		}
		else if
		{
			this.attackScreen = AttackScreens.MAIN
		}

		*/

		// Thread.sleep(1000);

		// perform Attack of creature first in round
		attackControl(creature2, creature1);

		// Thread.sleep(1000);

		// Potion effects for a creature are applied after its attack
		potionEffects(creature2);

		// reset variables that need to be changed each round
		resetRoundVariables();
	}

	// determine who lost the fight
	std::shared_ptr<Creature> fightLoser = nullptr;

	if (player->hp <= 0)
	{
		player->resetOriginals();
		enemy->resetOriginals();
		fightLoser = player;
	}
	else
	{
		fightLoser = enemy;
		//give attribute bonus to winner of the fight
		if (fightLoser instanceof Monster)
		{
			attributeBonusForWinner(fightLoser);
		}
		// set boostes values as new normal player values
		player->resetOriginals();
	}

	// empty active potion lists
	// player->emptyActivePotions();
	// enemy->emptyActivePotions();

	//return loser of the fight
	return *fightLoser;
}

void Fight::resetRoundVariables()
{
	attackSet = false;
	activeAttack = nullptr;
	activeAttackType = nullptr;
	selectedPotion = nullptr;
	enemyAttackHealthDamage = 0;
	enemyAttackAttributeDamage = 0;
	attackSet = false;
	changeTabActive = false;
	potionTakingActive = false;
}

std::shared_ptr<Potion> Fight::getSelectedPotion(std::shared_ptr<Creature> creature1)
{
	
}

Attacks::Enum Fight::getCommand(std::shared_ptr<Creature> creature)
{
	//check which option was selected, returns enum
	Attacks::Enum chosenAttackType = Attacks::UNKNOWN;

	if (creature instanceof Player)
	{
		// wait for the player to chose an attack - also set selected Potion, Set changes and so on...
		while (this->activeAttackType == nullptr)
		{
			// Thread.sleep(100);
		}

		chosenAttackType = activeAttackType;
	}
	else
	{
		//chosenAttackType = Chances.randomAttackType();	// actually need chances class
	}

	return chosenAttackType;
}

void Fight::attackControl(std::shared_ptr<Creature> creature1, std::shared_ptr<Creature> creature2)
{
	float activeAttackNumber = 0;

	//player chooses what to do
	switch (getCommand(creature1))
	{
	case Attacks::TORSO:
		

		activeAttackNumber = 1.f;
		break;

	case Attacks::HEAD:


		activeAttackNumber = 2.f;
		break;

	case Attacks::ARMS:


		activeAttackNumber = 3.f;
		break;

	case Attacks::LEGS:


		activeAttackNumber = 4.f;
		break;

	case Attacks::SET:

		if (creature1 == player)	// only switch if it's players turn
		{
			// switch Set
		}
		activeAttackNumber = 5.f;
		break;

	case Attacks::POTION:
		selectedPotion = getSelectedPotion(creature1);

		if (selectedPotion != nullptr)
		{
			// manage the handling of a used potion
			usePotion(creature1, creature2, selectedPotion);
		}
		activeAttackNumber = 6.f;
		break;

	case Attacks::PARRY:
		if (parrySuccess(creature1, creature2))
		{
			// when creature (player) parries successful, he deals x times the damage of a normal torso attack
			parryMultiplier = 2.f;
			*activeAttack = Attacks::TORSO;
		}
		else
		{
			parryMultiplier = 0.f;
			*activeAttack = Attacks::TORSO;
		}
		activeAttackNumber = 7.f;
		break;

	default:
		break;
	}

	// actual attack
	if (activeAttackNumber < 5.f || activeAttackNumber > 6.f)
	{
		attack(creature1, creature2);
	}

	// parryMultiplier is used on every attack and only temporarily increased when parrying -> needs to be resetted
	parryMultiplier = 1.f;

	// reset selected Potion
	selectedPotion = nullptr;

}

void Fight::attack(std::shared_ptr<Creature> attacker, std::shared_ptr<Creature> defender)
{
	if (activeAttack == nullptr) return;

	float healthDmg = 0.f;
	float attributeDmg = 0.f;

	if (attacker == player)
	{
		// now calculate actual attack
		if (calcHitSuccess(attacker, defender))
		{
			healthDmg = calcHealthDamage(attacker, defender);
			attributeDmg = calcAttributeDamage(attacker);
		}
	}
	else
	{
		healthDmg = enemyAttackHealthDamage;
		attributeDmg = enemyAttackAttributeDamage;
	}

	if (healthDmg > 0)
	{
		updateHealth(defender, healthDmg);
	}

	if (attributeDmg > 0)
	{
		updateAttributes(defender, attributeDmg);
	}

	// reset values
	enemyAttackHealthDamage = 0.f;
	enemyAttackAttributeDamage = 0.f;
}

float Fight::calcCreatureSpeed(std::shared_ptr<Creature> creature)
{
	// CONSTANTS
	float const armorSpeedMalusMult = 0.3f;
	float const weaponSpeedMult = 0.3f;

	// VARIABLES
	float playerArmorSpeedMalusSum;		// Sum of all equiped armaments' speed maluses
	float playerWeaponSpeedMalusMax;	// 100 - min. Speed of equipped Weapons
	float playerArmorSpeedMalus;
	float playerWeaponSpeedMalus;

	// results
	float speed = 0.f;

	// get monster's speed value
	if (creature == Monster)
	{
		return creature->speed;
	}

	// get Speed of player
	// playerArmorSpeedMalusSum = armorView.getStats(ArmorStatsTypes.ARMAMENT, ArmorStatsMode.SUM, ArmorStatsAttributes.SPEED);
	// playerWeaponSpeedMalusMax = armorView.getStats(ArmorStatsTypes.WEAPONS, ArmorStatsMode.MAX, ArmorStatsAttributes.SPEED);


	// perform calculations
	playerArmorSpeedMalus = creature->speed / 100 * playerArmorSpeedMalusSum * armorSpeedMalusMult;
	playerWeaponSpeedMalus = creature->speed / 100 * playerWeaponSpeedMalusMax * weaponSpeedMult;
	speed = creature->speed - playerArmorSpeedMalus - playerWeaponSpeedMalus;
	return speed;
}

//AttDefMinAvgMax Fight::speedBasedSuccess(Creature attacker, Creature defender)
//{
//
//} 

float Fight::determineFirstAttack()
{

}

bool Fight::parrySuccess(std::shared_ptr<Creature> attacker, std::shared_ptr<Creature> defender)
{

}

float Fight::calcCreatureAccuracy(std::shared_ptr<Creature> creature)
{
	// constants
	float const accuracyMultiplier = 2.65f;

	// results
	float accuracy;

	// get accuracy of monster
	if (creature instanceof Monster)
	{
		return creature->accuracy;
	}

	//perform calculations
	accuracy = creature->accuracy / 100 * armorView.getStats(ArmorStatsTypes.WEAPON, ArmorStatsMode.AVERAGE, ArmorStatsAttributes.ACCURACY) * accuracyMultiplier;

	return accuracy;
}

float Fight::calcHitSuccess(std::shared_ptr<Creature> attacker, std::shared_ptr<Creature> defender)
{
	//constants
	float const randAccuracyLow = 0.5f;
	float const randAccuracyHigh = 1.5f;
	float const accuracyBase = 50.f;
	float const finishedStagesDivisor = 40.f;

	// variables
	float attackerAccuracyBase;
	//MinAvgMax atackerAccuracy;
	//MinAvgMax defenderSpeed;
	float randAttackerAccuracy;
	float randDefenderSpeed;

	// speed
	bool hitSuccess;

	// set finishedStagesMult Bonus for Monsters
	if (attacker instanceof Monster)
	{
	
	}
	else
	{
	}

	//perform calculations
	attackerAccuracyBase = activeAttack.hitPropability * (attacker->accuracy + calcCreatureAccuracy(attacker) / attacker->accuracy * accuracyBase));
	//attackerAccuracy = new MinAvgMax(randAccuracyLow, randAccuracyHigh, attackerAccuracyBase);
	//defenderSpeed = speedBasedSuccess(attacker, defender).defender;

	// calculate randomized values
	//randAttackerAccuracy = Chances.randomFloat(attackerAccuracy.min, attackerAccuracy.max);
	//randDefenderSpeed = Chances.randomFloat(defenderSpeed.min, defenderSpeed.max);

	//determine hitSuccess - always hit after successful parry
	if (randAttackerAccuracy >= randDefenderSpeed || parryMultiplier > 1.f)
	{
		hitSuccess = true;
	}
	else
	{
		hitSuccess = false;
	}
	return hitSuccess;
}

float Fight::calcHealthDamage(std::shared_ptr<Creature> attacker, std::shared_ptr<Creature> defender)
{
	//constants
	float const baseDefensePlayer = 100.f;
	float const baseDefenseEasy = 100.f;
	float const baseDefenseNormal = 150.f;
	float const baseDefenseHard = 200.f;
	float const finishedStagesDivisor = 100.f;
	float const damageMult = 45.f;
	float const weaponMult = 1.f;
	float const strengthMult = 1.5f;
	float const armorMult = 0.5f;

	// parameters
	float attackerWeaponDamage;
	float defenderArmor;
	float baseDefense;

	//variables
	MinAvgMax attackDamage;
	MinAvgMax attackerRawDamage;
	float defenderDefense;
	MinAvgMax healthDamage;
	float finishedStagesMult;

	//results
	float randHealthDamage

	// set parameters
	if (attaker instaneof Monster)
	{
		attackerWeaponDamage = 0;		// maybe change because of armed enemies??
		finishedStagesMult = 1 + (finishedStages / finishedStagesDivisor);
	}
	else
	{
		attackerWeaponDamage = armorView.getStats(ArmorStatsTypes.Weapons, ArmorStatsMode.SUM, ArmorStatsAttributes.ATTACK);
		finishedStagesMult = 1;
	}

	if (defender instanceof Monster)
	{
		defenderArmor = 0;
		finishedStagesMult = 1 + (finishedStages / finishedStagesDivisor);
		if (((Monster) defender).level == DifficultyLevel::EASY 
	}

}

float Fight::calcAttributeDamage(std::shared_ptr<Creature> defender)
{

}

void Fight::updateHealth(std::shared_ptr<Creature> defender, float attributeDamage)
{

}

void Fight::usePotion(std::shared_ptr<Creature> potionUser, std::shared_ptr<Creature> opponent, std::shared_ptr<Potion> potion)
{

}

void Fight::potionEffects(std::shared_ptr<Creature> creature)
{

}