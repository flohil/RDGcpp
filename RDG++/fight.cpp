#include "fight.hpp"
#include "chances.hpp"

Fight::Fight(std::shared_ptr<Player> player_, std::shared_ptr<Monster> enemy_, std::shared_ptr<PrototypeStorage> prototypeStorage_)
	: player(player_), enemy(enemy_), prototypeStorage(prototypeStorage_)
{
	/*attackSet = false;
	activeAttack = nullptr;
	activeAttackType = Attacks::UNKNOWN;
	selectedPotion = nullptr;
	enemyAttackHealthDamage = 0;
	enemyAttackAttributeDamage = 0;
	enemyArmorSum = 0;
	attackSet = false;

	changeTabActive = false;
	potionTakingActive = false;*/
}


Fight::~Fight()
{

}

std::shared_ptr<Creature> Fight::fightRound(Attacks::Enum playerTask, unsigned int stage)
{
	/* MAIN FIGHT LOOP */
	//while (player->hp > 0 && enemy->hp > 0)
	//{
	std::cout << "Playertask: " << playerTask << std::endl;
	std::cout << "entered fightRound()" << std::endl;
	if (stage == 1u)
	{
		std::wcout << "stage=1u" << std::endl;
		creature1 = nullptr;
		creature2 = nullptr;
#if 0

		chosenTask1 = Attacks::UNKNOWN;
		chosenTask2 = Attacks::UNKNOWN;

		unsigned int firstAttackTemp = determineFirstAttack();

		if (firstAttackTemp == 1)
		{
			creature1 = player;
			creature2 = enemy;
			chosenTask1 = playerTask;
			chosenTask2 = Chances::randomAttackType();
			// this.attackScreen = AttackScreen.MAIN;
		}
		else if (firstAttackTemp == 2)
		{
			creature1 = enemy;
			creature2 = player;
			chosenTask1 = Chances::randomAttackType();
			chosenTask2 = playerTask;
			// this.attackScreen = AttackScreen.WAITING;
		}

		// thread.sleep(1000);

		//perform Attack of creature in first round
		attackControl(creature1, creature2, chosenTask1);

		// potion effects for a creature are applied after its attack
		potionEffects(creature1);
#endif
	}
	else if (stage == 2u)
	{
		std::cout << "stage=2u" << std::endl;
		// set to null between attacks of player and enemy to determine if attack was already chosen
		activeAttack = nullptr;
		activeAttackType = Attacks::UNKNOWN;
#if 0

		// perform Attack of creature first in round
		//std::cout << "Creature2: " << creature2->getCreatureType() << std::endl;
		//std::cout << "Creature1: " << creature1->getCreatureType() << std::endl;
		//std::cout << "ChosenTask: " << chosenTask2 << std::endl;
		attackControl(creature2, creature1, chosenTask2);
		std::cout << "attackControl success" << std::endl;

		// Thread.sleep(1000);

		// Potion effects for a creature are applied after its attack
		//std::cout << "Creature2: " << creature2->getCreatureType() << std::endl;
		potionEffects(creature2);
		std::cout << "potionEffects success" << std::endl;

		// reset variables that need to be changed each round
		resetRoundVariables();
		std::cout << "resetRoundVariables success" << std::endl;

		//}

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
			if (fightLoser->getCreatureType() == CreatureType::MONSTER)
			{
				attributeBonusForWinner(fightLoser);
			}
			// set boostes values as new normal player values
			player->resetOriginals();
		}

		// empty active potion lists
		player->emptyActivePotions();
		enemy->emptyActivePotions();

		//return loser of the fight
		return fightLoser;
#endif
	}
	return nullptr;
}

void Fight::resetRoundVariables()
{
	attackSet = false;
	activeAttack = nullptr;
	activeAttackType = Attacks::UNKNOWN;
	selectedPotion = nullptr;
	enemyAttackHealthDamage = 0;
	enemyAttackAttributeDamage = 0;
	attackSet = false;
	changeTabActive = false;
	potionTakingActive = false;
}

std::shared_ptr<Potion> Fight::getSelectedPotion(std::shared_ptr<Creature> creature1)
{
	return nullptr;
}

/*
// replace with gui commands for player and random attack for monster
Attacks::Enum Fight::getCommand(std::shared_ptr<Creature> creature)
{
	//check which option was selected, returns enum
	Attacks::Enum chosenAttackType = Attacks::UNKNOWN;

	if (creature->getCreatureType() == CreatureType::PLAYER)
	{
		// wait for the player to chose an attack - also set selected Potion, Set changes and so on...
		while (activeAttackType == Attacks::UNKNOWN)
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
*/

void Fight::attackControl(std::shared_ptr<Creature> creature1, std::shared_ptr<Creature> creature2, Attacks::Enum chosenTask)
{
	float activeAttackNumber = 0;

	//player chooses what to do
	switch (chosenTask)
	{
	case Attacks::TORSO:
	{
		activeAttack = prototypeStorage->attackFactory->create(EnumMapper::mapAttackName(chosenTask));
		//activeAttack = &attacks.at(Attacks::TORSO);
		if (creature1 == player)
		{
			ResourceManager::getInstance().getSound("buttonClick").play();
			//activeAttackNumber = 1.f;
		}
		break;
	}
	case Attacks::HEAD:
		activeAttack = prototypeStorage->attackFactory->create(EnumMapper::mapAttackName(chosenTask));
		if (creature1 == player)
		{
			ResourceManager::getInstance().getSound("buttonClick").play();
			//activeAttackNumber = 1.f;
		}
		break;

	case Attacks::ARMS:
		activeAttack = prototypeStorage->attackFactory->create(EnumMapper::mapAttackName(chosenTask));
		if (creature1 == player)
		{
			ResourceManager::getInstance().getSound("buttonClick").play();
			//activeAttackNumber = 1.f;
		}
		break;

	case Attacks::LEGS:
		activeAttack = prototypeStorage->attackFactory->create(EnumMapper::mapAttackName(chosenTask));
		if (creature1 == player)
		{
			ResourceManager::getInstance().getSound("buttonClick").play();
			//activeAttackNumber = 1.f;
		}
		break;

	case Attacks::SET:
		//activeAttackNumber = 5.f;
		break;

	case Attacks::POTION:
		selectedPotion = getSelectedPotion(creature1);

		if (selectedPotion != nullptr)
		{
			// manage the handling of a used potion
			usePotion(creature1, creature2, selectedPotion);
		}
		//activeAttackNumber = 6.f;
		break;

	case Attacks::PARRY:
		if (parrySuccess(creature1, creature2))
		{
			// when creature (player) parries successful, he deals x times the damage of a normal torso attack
			parryMultiplier = 2.f;
			activeAttack = prototypeStorage->attackFactory->create(EnumMapper::mapAttackName(chosenTask));
		}
		else
		{
			parryMultiplier = 0.f;
			activeAttack = prototypeStorage->attackFactory->create(EnumMapper::mapAttackName(chosenTask));
		}
		//activeAttackNumber = 7.f;
		break;

	default:
		break;
	}


	// actual attack
	if (chosenTask == Attacks::HEAD || chosenTask == Attacks::TORSO || chosenTask == Attacks::ARMS || chosenTask == Attacks::LEGS/*activeAttackNumber < 5.f || activeAttackNumber > 6.f*/)
	{
		attack(creature1, creature2);
		std::cout << "attack successful" << std::endl;
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
			if (attacker->getCreatureType() == CreatureType::MONSTER)
			{
				ResourceManager::getInstance().getSound("humanHit").play();
			}
			else
			{
				ResourceManager::getInstance().getSound(player->getEquipmentSet()->getPrimaryWeapon()->getAttackSound()).play();
			}
		}
		else
		{
			ResourceManager::getInstance().getSound("miss").play();
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
	if (creature->getCreatureType() == CreatureType::MONSTER)
	{
		return creature->speed;
	}

	// get Speed of player
	playerArmorSpeedMalusSum = player->getEquipmentSet()->getStats(ItemType::ARMAMENT, ArmorStatsMode::SUM, ArmorStatsAttributes::SPEED);
	playerWeaponSpeedMalusMax = player->getEquipmentSet()->getStats(ItemType::WEAPON, ArmorStatsMode::MAX, ArmorStatsAttributes::SPEED);

	// perform calculations
	playerArmorSpeedMalus = creature->speed / 100 * playerArmorSpeedMalusSum * armorSpeedMalusMult;
	playerWeaponSpeedMalus = creature->speed / 100 * playerWeaponSpeedMalusMax * weaponSpeedMult;
	speed = creature->speed - playerArmorSpeedMalus - playerWeaponSpeedMalus;
	return speed;
}

AttDefMinAvgMax Fight::speedBasedSuccess(std::shared_ptr<Creature> attacker, std::shared_ptr<Creature> defender)
{
	//constants
	float const speedRandLow = 0.5f;
	float const speedRandHigh = 1.5f;
	float const speedBase = 50.f;
	float const finishedStagesDivisor = 40.f;

	// variables
	float attackerSpeedTemp;
	float defenderSpeedTemp;
	float attackerSpeedBase;
	float defenderSpeedBase;

	// determine creatures' speed
	attackerSpeedTemp = attacker->speed;
	defenderSpeedTemp = defender->speed;

	// include a finishedStages Bonus for Monsters
	if (defender->getCreatureType() == CreatureType::MONSTER)
	{
		defenderSpeedTemp = defenderSpeedTemp * (1 + finishedStages / finishedStagesDivisor);
	}
	if (attacker->getCreatureType() == CreatureType::MONSTER)
	{
		attackerSpeedTemp = attackerSpeedTemp * (1 + finishedStages / finishedStagesDivisor);
	}

	// perform caltulations
	attackerSpeedBase = attackerSpeedTemp + (calcCreatureSpeed(attacker) / attackerSpeedTemp) * speedBase;
	defenderSpeedBase = defenderSpeedTemp + (calcCreatureSpeed(defender) / defenderSpeedTemp) * speedBase;
	MinAvgMax attackerSpeed(speedRandLow, speedRandHigh, attackerSpeedBase);
	MinAvgMax defenderSpeed(speedRandLow, speedRandHigh, defenderSpeedBase);

	return AttDefMinAvgMax(attackerSpeed, defenderSpeed);
} 

unsigned int Fight::determineFirstAttack()
{
	//variables
	float randAttackerSpeed;
	float randDefenderSpeed;

	// perform randomizing alculations
	AttDefMinAvgMax speedBasedSuccessRetVal = speedBasedSuccess(player, enemy);
	randAttackerSpeed = Chances::randomFloat(speedBasedSuccessRetVal.attacker.min, speedBasedSuccessRetVal.attacker.min);
	randDefenderSpeed = Chances::randomFloat(speedBasedSuccessRetVal.defender.min, speedBasedSuccessRetVal.defender.max);

	// determine which player comes first
	if (randAttackerSpeed >= randDefenderSpeed)	return 1u;
	else return 2u;
}

bool Fight::parrySuccess(std::shared_ptr<Creature> attacker, std::shared_ptr<Creature> defender)
{
	// variables
	float randAttackerSpeed;
	float randDefenderSpeed;
	bool parrySuccess = false;

	// perform randomizng calculations
	AttDefMinAvgMax speedBasedSuccessRetVal = speedBasedSuccess(player, enemy);
	randAttackerSpeed = Chances::randomFloat(speedBasedSuccessRetVal.attacker.min, speedBasedSuccessRetVal.attacker.max);
	randDefenderSpeed = Chances::randomFloat(speedBasedSuccessRetVal.defender.min, speedBasedSuccessRetVal.defender.max);

	//determine which player comes first
	if (randAttackerSpeed >= randDefenderSpeed)
	{
		parrySuccess = true;
	}
	else
	{
		parrySuccess = false;
	}

	return parrySuccess;
}

float Fight::calcCreatureAccuracy(std::shared_ptr<Creature> creature)
{
	// constants
	float const accuracyMultiplier = 2.65f;

	// results
	float accuracy;

	// get accuracy of monster
	if (creature->getCreatureType() == CreatureType::MONSTER)
	{
		return creature->accuracy;
	}

	//perform calculations
	accuracy = creature->accuracy / 100 * player->getEquipmentSet()->getStats(ItemType::WEAPON, ArmorStatsMode::AVG, ArmorStatsAttributes::ACCURACY) * accuracyMultiplier;

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
	MinAvgMax attackerAccuracy;
	MinAvgMax defenderSpeed;
	float randAttackerAccuracy;
	float randDefenderSpeed;

	// speed
	bool hitSuccess;

	// set finishedStagesMult Bonus for Monsters
	if (attacker->getCreatureType() == CreatureType::MONSTER)
	{
	
	}
	else
	{

	}

	//perform calculations
	attackerAccuracyBase = activeAttack->getHitProbability() * (attacker->accuracy + calcCreatureAccuracy(attacker) / attacker->accuracy * accuracyBase);
	attackerAccuracy = MinAvgMax(randAccuracyLow, randAccuracyHigh, attackerAccuracyBase);
	defenderSpeed = speedBasedSuccess(attacker, defender).defender;

	// calculate randomized values
	randAttackerAccuracy = Chances::randomFloat(attackerAccuracy.min, attackerAccuracy.max);
	randDefenderSpeed = Chances::randomFloat(defenderSpeed.min, defenderSpeed.max);

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
	float randHealthDamage;

	// set parameters
	if (attacker->getCreatureType() == CreatureType::MONSTER)
	{
		attackerWeaponDamage = 0;		// maybe change because of armed enemies??
		finishedStagesMult = 1 + (finishedStages / finishedStagesDivisor);
	}
	else
	{
		attackerWeaponDamage = player->getEquipmentSet()->getStats(ItemType::WEAPON, ArmorStatsMode::SUM, ArmorStatsAttributes::ATTACK);
		finishedStagesMult = 1;
	}

	if (defender->getCreatureType() == CreatureType::MONSTER)
	{
		defenderArmor = 0;
		finishedStagesMult = 1 + (finishedStages / finishedStagesDivisor);
		if (defender->getLevel() == DifficultyLevel::EASY)
		{
			baseDefense = baseDefenseEasy;
		}
		else if (defender->getLevel() == DifficultyLevel::NORMAL)
		{
			baseDefense = baseDefenseNormal;
		}
		else
		{
			baseDefense = baseDefenseHard;
		}
	}
	else if (defender == player)
	{
		defenderArmor = player->getEquipmentSet()->getStats(ItemType::ARMAMENT, ArmorStatsMode::SUM, ArmorStatsAttributes::ARMOR);
		finishedStagesMult = 1;
		baseDefense = baseDefensePlayer;
	}
	else
	{
		defenderArmor = enemyArmorSum;
		finishedStagesMult = 1;
		baseDefense = baseDefensePlayer;
	}

	// perform calculations
	attackDamage = MinAvgMax( activeAttack->getAttackStatsLowMultiplier(), activeAttack->getAttackStatsHighMultiplier(), activeAttack->getHpDamageMultiplier() );
	attackerRawDamage = MinAvgMax(
		attackDamage.min * (strengthMult * attacker->strength + weaponMult * attackerWeaponDamage),
		attackDamage.max * (strengthMult * attacker->strength + weaponMult * attackerWeaponDamage)
	);

	defenderDefense = finishedStagesMult * baseDefense + armorMult * defenderArmor;
	healthDamage = MinAvgMax(
		parryMultiplier * attackerRawDamage.min / defenderDefense * damageMult,
		parryMultiplier * attackerRawDamage.max / defenderDefense * damageMult
	);

	// calculate a randomized result
	randHealthDamage = Chances::randomFloat(healthDamage.min, healthDamage.max);

	return randHealthDamage;
}

float Fight::calcAttributeDamage(std::shared_ptr<Creature> defender)
{
	// constants
	float const playerAttributeDamageMult = 1.f;
	float const easyAttributeDamageMult = 1.05f;
	float const normalAtributeDamageMult = 1.f;
	float const hardAttributeDamageMult = 0.95f;

	// variables
	float attributeDamageMult;
	float defenderDamageMult;
	float defenderAttribute = 0;

	// results
	float attributeDamage;

	// get enemies curent atribute values
	switch (activeAttack->getEffect())
	{
		case Attribute::HP:
			defenderAttribute = defender->hp;
			break;
		case Attribute::ACCURACY:
			defenderAttribute = defender->accuracy;
			break;
		case Attribute::STRENGTH:
			defenderAttribute = defender->strength;
			break;
		case Attribute::SPEED:
			defenderAttribute = defender->speed;
			break;
		default:
			break;
	}

	// set atributeDamageMult
	if (defender->getCreatureType() == CreatureType::MONSTER)
	{
		if (defender->getLevel() == DifficultyLevel::EASY)
		{
			attributeDamageMult = easyAttributeDamageMult;
		}
		else if (defender->getLevel() == DifficultyLevel::NORMAL)
		{
			attributeDamageMult = normalAtributeDamageMult;
		}
		else
		{
			attributeDamageMult = hardAttributeDamageMult;
		}
	}
	else
	{
		attributeDamageMult = playerAttributeDamageMult;
	}

	// perform calculations
	defenderDamageMult = attributeDamageMult * activeAttack->getAttributeDamageMultiplier();
	attributeDamage = defenderAttribute - defenderAttribute * defenderDamageMult;

	return attributeDamage;
}

void Fight::updateHealth(std::shared_ptr<Creature> defender, float healthDamage)
{
	float hp = defender->hp - healthDamage;
	if (hp < 0) hp = 0;
	defender->hp = hp;
}

void Fight::updateAttributes(std::shared_ptr<Creature> defender, float attributeDamage)
{
	switch (activeAttack->getEffect())
	{
	case Attribute::HP:
	{
		float hp = defender->hp - attributeDamage;
		if (hp < 0) hp = 0;
		defender->hp = hp;
		break;
	}
	case Attribute::ACCURACY:
	{
		float accuracy = defender->accuracy - attributeDamage;
		if (accuracy < 0) accuracy = 0;
		defender->accuracy = accuracy;
		break;
	}
	case Attribute::STRENGTH:
	{
		float strength = defender->strength - attributeDamage;
		if (strength < 0) strength = 0;
		defender->strength = strength;
		break;
	}
	case Attribute::SPEED:
	{
		float speed = defender->speed - attributeDamage;
		if (speed < 0) speed = 0;
		defender->speed = speed;
		break;
	}
	default:
		break;
	}
}

void Fight::usePotion(std::shared_ptr<Creature> potionUser, std::shared_ptr<Creature> opponent, std::shared_ptr<Potion> potion)
{
	// if player uses antidote / removes the first poison in the list
	if (potion->getMode() == Mode::CURE)
	{
		/*for (Potion _potion : potionUser->activePotions)
		{
			if (_potion.getEffect == Attribute::HP && _potion.getMode() == Mode::INCREMENTAL_DECREASE)
			{
				potionUser->removeActivePotions(_potion);
			}
			break;
		}*/
	}
	else
	{
		//store potions to the creature that they affect
		if (potion->getTarget() == Target::SELF)
		{
			potionUser->addActivePotion(potion);
			if (potion->getMode() == Mode::TEMPORARY_INCREASE)
			{
				potionIncrease(potionUser, potion);
			}
		}
		else
		{
			opponent->addActivePotion(potion);
			if (potion->getMode() == Mode::TEMPORARY_DECREASE)
			{
				potionDecrease(opponent, potion);
			}
		}
	}
}

void Fight::potionEffects(std::shared_ptr<Creature> creature)
{
	// apply all non temporary potion effects
	if (creature != nullptr)
	{
		std::cout << "creature exists" << std::endl;
		for (std::shared_ptr<Potion> potion : creature->activePotions)
		{
			std::cout << "Active Potion: " << potion << std::endl;
			potion->setDuration(potion->getDuration() - 1);
			switch (potion->getMode())
			{
			case Mode::TEMPORARY_INCREASE:
				std::cout << "potion mode: TEMPORARY_INCREASE" << std::endl;
				potionIncrease(creature, potion);
				std::cout << "potionIncrease successful" << std::endl;
				break;
			case Mode::TEMPORARY_DECREASE:
				std::cout << "potion mode: TEMPORARY_DECREASE" << std::endl;
				potionDecrease(creature, potion);
				std::cout << "potionDecrease successful" << std::endl;
				break;
			default:
				std::cout << "potion mode: DEFAULT" << std::endl << "ERROR!!" << std::endl;
				break;
			}
			if (potion->getDuration() <= 0)
			{
				std::cout << "potionDuration <= 0" << std::endl;
				revertEffect(creature, potion);
				std::cout << "revertEffect successful" << std::endl;
				creature->removeActivePotions(potion);
				std::cout << "removeActivePotions sucessful" << std::endl;
			}
		}
	}
	else
	{
		std::cout << "creature == nullptr" << std::endl;
	}
}

void Fight::revertEffect(std::shared_ptr<Creature> creature, std::shared_ptr<Potion> potion)
{
	if (potion->getMode() == Mode::TEMPORARY_INCREASE)
	{
		potionDecrease(creature, potion);
	}
	else if (potion->getMode() == Mode::TEMPORARY_DECREASE)
	{
		potionIncrease(creature, potion);
	}
}

void Fight::potionDecrease(std::shared_ptr<Creature> creature, std::shared_ptr<Potion> potion)
{
	// constants
	float const finishedStagesDivisor = 50.f;

	// variables
	float finishedStagesMult = 1 + (finishedStages / finishedStagesDivisor);
	/*
	if (creature == player && (creature->getCreatureType == CreatureType::PLAYER))
	{
		finishedStagesMult = enemyStagesMultiplier; // ????
	}
	*/

	switch (potion->getEffect())
	{
		case Attribute::HP:
		{
			float hp = creature->hp - potion->getStrength() * finishedStagesMult;
			if (hp < 0) hp = 0;
			creature->hp = hp;
			break;
		}
		case Attribute::SPEED:
		{
			float speed = creature->speed - potion->getStrength() * finishedStagesMult;
			if (speed < 0) speed = 0;
			creature->speed = speed;
			break;
		}
		case Attribute::ACCURACY:
		{
			float accuracy = creature->accuracy - potion->getStrength() * finishedStagesMult;
			if (accuracy < 0) accuracy = 0;
			creature->accuracy = accuracy;
			break;
		}
		case Attribute::STRENGTH:
		{
			float strength = creature->strength - potion->getStrength() * finishedStagesMult;
			if (strength < 0) strength = 0;
			creature->strength = strength;
			break;
		}
		default:
			break;
	}
}

void Fight::potionIncrease(std::shared_ptr<Creature> creature, std::shared_ptr<Potion> potion)
{
	// constants
	float const finishedStagesDivisor = 50.f;

	// variables
	float finishedStagesMult = 1 + (finishedStages / finishedStagesDivisor);
	/*
	if (creature == enemy && (creature->getCreatureType == CreatureType::PLAYER))
	{
		finishedStagesMult = enemyStagesMultilier;	// ???
	}
	*/

	switch (potion->getEffect())
	{
		case Attribute::HP:
		{
			float hp = creature->hp + potion->getStrength() * finishedStagesMult;
			if (hp > creature->getOrHP()) hp = creature->getOrHP();
			creature->hp = hp;
			break;
		}
		case Attribute::SPEED:
		{
			float speed = creature->speed + potion->getStrength() * finishedStagesMult;
			if (speed > creature->getOrSpeed()) speed = creature->getOrSpeed();
			creature->speed = speed;
			break;
		}
		case Attribute::ACCURACY:
		{
			float accuracy = creature->accuracy + potion->getStrength() * finishedStagesMult;
			if (accuracy > creature->getOrAccuracy()) accuracy = creature->getOrAccuracy();
			creature->accuracy = accuracy;
			break;
		}
		case Attribute::STRENGTH:
		{
			float strength = creature->strength + potion->getStrength() * finishedStagesMult;
			if (strength > creature->getOrStrength()) strength = creature->getOrStrength();
			creature->strength = strength;
			break;
		}
		default:
			break;
	}
}

void Fight::attributeBonusForWinner(std::shared_ptr<Creature>)
{
	//implement

	return;
}