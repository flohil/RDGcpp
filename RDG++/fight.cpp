#include "fight.hpp"
#include "chances.hpp"
#include "OutputFormatter.hpp"

Fight::Fight(std::shared_ptr<Player> player_, std::shared_ptr<Monster> enemy_, std::shared_ptr<PrototypeStorage> prototypeStorage_, tgui::ChatBox::Ptr chatbox_)
	: player(player_), enemy(enemy_), prototypeStorage(prototypeStorage_), chatbox(chatbox_), loser(nullptr)
{
	/*
	activeAttack = nullptr;
	activeAttackType = Attacks::UNKNOWN;
	selectedPotion = nullptr;
	enemyAttackHealthDamage = 0;
	enemyAttackAttributeDamage = 0;
	enemyArmorSum = 0;

	changeTabActive = false;
	potionTakingActive = false;*/

	playerAttackType = Attacks::UNKNOWN;
	enemyAttackType = Attacks::UNKNOWN;
	chosenTask1 = Attacks::UNKNOWN;
	chosenTask2 = Attacks::UNKNOWN;
}

Fight::~Fight()
{

}

void Fight::fightRound(Attacks::Enum playerTask, unsigned int stage)
{
	/* MAIN FIGHT LOOP */
	//while (player->hp > 0 && enemy->hp > 0)
	//{

	if (loser != nullptr) // fight is already over
	{
		activeRound = 1u;
		return;
	}

	bool playerLost = false;
	bool enemyLost = false;

	if (stage == 1u)
	{
		std::wcout << "Stage = 1u" << std::endl;
		creature1 = nullptr;
		creature2 = nullptr;

		playerAttackType = playerTask;
		enemyAttackType = Chances::randomAttackType();
#if 1

		chosenTask1 = Attacks::UNKNOWN;
		chosenTask2 = Attacks::UNKNOWN;

		unsigned int firstAttackTemp = determineFirstAttack();

		if (playerTask == Attacks::SET || playerTask == Attacks::POTION) // potions and set changes always come first - DON'T CHANGE THIS!
		{
			firstAttackTemp = 1;
		}

		if (firstAttackTemp == 1)
		{
			creature1 = player;
			creature2 = enemy;
			chosenTask1 = playerTask;
			chosenTask2 = enemyAttackType;
			std::cout << "Player attacks first" << std::endl;
		}
		else if (firstAttackTemp == 2)
		{
			creature1 = enemy;
			creature2 = player;
			chosenTask1 = enemyAttackType;
			chosenTask2 = playerTask;
			std::cout << "Enemy attacks first" << std::endl;
		}

		activeAttackType = chosenTask1;

		//perform Attack in first round
		attackControl(creature1, creature2, chosenTask1);

		// potion effects for a creature are applied after its attack
		potionEffects(creature1);

#endif
		if (player->hp <= 0)
		{
			playerLost = true;
		}
		else if (enemy->hp <= 0)
		{
			enemyLost = true;
		}
		activeRound = 2u;
	}
	else if (stage == 2u)
	{
		std::cout << "Stage = 2u" << std::endl;
		// set to null between attacks of player and enemy to determine if attack was already chosen
		activeAttackType = chosenTask2;
#if 1

		// perform Attack in second round
		//std::cout << "Creature2: " << creature2->getCreatureType() << std::endl;
		//std::cout << "Creature1: " << creature1->getCreatureType() << std::endl;
		//std::cout << "ChosenTask: " << chosenTask2 << std::endl;
		attackControl(creature2, creature1, chosenTask2);

		// Thread.sleep(1000);

		// Potion effects for a creature are applied after its attack
		//std::cout << "Creature2: " << creature2->getCreatureType() << std::endl;
		potionEffects(creature2);

#endif
		
		if (player->hp <= 0)
		{
			playerLost = true;
		}
		else if (enemy->hp <= 0)
		{
			enemyLost = true;
		}

		// reset full round variables (after finishing both round1 and round2)
		activeRound = 1u;
		playerAttackType = Attacks::UNKNOWN;
		enemyAttackType = Attacks::UNKNOWN;
		chosenTask1 = Attacks::UNKNOWN;
		chosenTask2 = Attacks::UNKNOWN;
	}

	if (playerLost)
	{
		std::cout << "Player LOST the fight" << std::endl << std::endl;
		loser = player;
	}
	else if (enemyLost)
	{
		//give attribute bonus to winner of the fight
		// set boostes values as new normal player values
		// empty active potion lists
		player->emptyActivePotions();
		enemy->emptyActivePotions();
		std::cout << "Player WON the fight" << std::endl << std::endl;
		loser = enemy;
	}

	// reset variables that need to be changed each round
	resetRoundVariables();
}

void Fight::end()
{
	if (loser->getCreatureType() == CreatureType::PLAYER)
	{
		ResourceManager::getInstance().getSound("humanDies").play();
		OutputFormatter::chat(chatbox, player->getPlayerName() + " lost the fight against " + enemy->getName(), sf::Color::White);
	}
	else
	{
		ResourceManager::getInstance().getSound(enemy->getSoundName()).play();
		OutputFormatter::chat(chatbox, player->getPlayerName() + " won the fight against " + enemy->getName(), sf::Color::White);
	}
	isEnded = true;
}

void Fight::resetRoundVariables()
{
	activeAttackType = Attacks::UNKNOWN;
	selectedPotion = nullptr;
	enemyAttackHealthDamage = 0;
	enemyAttackAttributeDamage = 0;
	changeTabActive = false;
	potionTakingActive = false;
}

std::shared_ptr<Potion> Fight::getSelectedPotion(std::shared_ptr<Creature> creature1)
{
	if (creature1->getCreatureType() == CreatureType::PLAYER)
	{
		return selectedPotion;
	}
	else
	{
		return nullptr;
	}
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

	return chosenAttackType;s
}
*/

void Fight::attackControl(std::shared_ptr<Creature> creature1, std::shared_ptr<Creature> creature2, Attacks::Enum chosenTask)
{
	float activeAttackNumber = 0;
	std::shared_ptr<Attack> activeAttack = nullptr;
	bool parrySucceeded = false; // can only be true if chosenTask is Parry and parrySuccess() calculation returned true
	parryMultiplier = 1.f;

	//attacker chooses what to do
	switch (chosenTask)
	{
	case Attacks::TORSO:
	{
		activeAttack = prototypeStorage->attackFactory->create(EnumMapper::mapAttackName(chosenTask));
		break;
	}
	case Attacks::HEAD:
		activeAttack = prototypeStorage->attackFactory->create(EnumMapper::mapAttackName(chosenTask));
		break;

	case Attacks::ARMS:
		activeAttack = prototypeStorage->attackFactory->create(EnumMapper::mapAttackName(chosenTask));
		break;

	case Attacks::LEGS:
		activeAttack = prototypeStorage->attackFactory->create(EnumMapper::mapAttackName(chosenTask));
		break;

	case Attacks::SET:
		//activeAttackNumber = 5.f;
		OutputFormatter::chat(chatbox, player->getPlayerName() + " changed Equipment Set", sf::Color::White);
		break;

	case Attacks::POTION:
		selectedPotion = getSelectedPotion(creature1);

		std::cout << "selected potion for " << creature1 << ": " << selectedPotion << std::endl;

		if (selectedPotion != nullptr)
		{
			// manage the handling of a used potion
			OutputFormatter::chat(chatbox, player->getPlayerName() + " used " + selectedPotion->getName(), sf::Color::White);
			ResourceManager::getInstance().getSound("drink").play();
			usePotion(creature1, creature2, selectedPotion);

			// reset selected Potion
			selectedPotion = nullptr;
		}
		//activeAttackNumber = 6.f;
		break;

	case Attacks::PARRY:
		if (parrySuccess(creature1, creature2))
		{
			// when creature (player) parries successfully, he deals x times the damage of a normal torso attack
			parryMultiplier = 2.f;
			activeAttack = prototypeStorage->attackFactory->create(EnumMapper::mapAttackName(Attacks::TORSO));
			parrySucceeded = false;
			OutputFormatter::chat(chatbox, player->getPlayerName() + " burst through " + enemy->getName() + "'s attack", sf::Color::White);
		}
		else
		{
			parryMultiplier = 0.f;
			activeAttack = prototypeStorage->attackFactory->create(EnumMapper::mapAttackName(Attacks::TORSO));
			ResourceManager::getInstance().getSound("forceParry").play();
			OutputFormatter::chat(chatbox, enemy->getName() + " parried " + player->getPlayerName() + "'s attack", sf::Color::White);
		}
		//activeAttackNumber = 7.f;
		break;

	default:
		break;
	}


	// actual attack
	if (chosenTask == Attacks::HEAD || chosenTask == Attacks::TORSO || chosenTask == Attacks::ARMS || chosenTask == Attacks::LEGS || (chosenTask == Attacks::PARRY && parrySucceeded)/*activeAttackNumber < 5.f || activeAttackNumber > 6.f*/)
	{
		attack(creature1, creature2, activeAttack, parrySucceeded);
		//std::cout << "attack successful" << std::endl;
	}

	// parryMultiplier is used on every attack and only temporarily increased when parrying -> needs to be resetted
	parryMultiplier = 1.f;
}

void Fight::attack(std::shared_ptr<Creature> attacker, std::shared_ptr<Creature> defender, std::shared_ptr<Attack> activeAttack, bool parrySuccess)
{
	if (activeAttack == nullptr) return;

	float healthDmg = 0.f;
	float attributeDmg = 0.f;
	bool hitSuccess = calcHitSuccess(attacker, defender, activeAttack);

	if (parrySuccess || hitSuccess)
	{
		healthDmg = float(int(calcHealthDamage(attacker, defender, activeAttack)*100))/100;
		attributeDmg = calcAttributeDamage(defender, activeAttack);
		hitSuccess = true;
	}

	//hitSuccess = true; // for testing

	if (hitSuccess)
	{
		if (attacker->getCreatureType() == CreatureType::MONSTER)
		{
			OutputFormatter::chat(chatbox, enemy->getName() + " hit " + player->getPlayerName() + " on the " + EnumMapper::mapAttackName(activeAttackType), sf::Color::White);
			OutputFormatter::chat(chatbox, player->getPlayerName() + " suffers " + OutputFormatter::shortFloat(healthDmg) + " HP damage", sf::Color::White);
			ResourceManager::getInstance().getSound("humanHit").play();
		}
		else
		{
			OutputFormatter::chat(chatbox, player->getPlayerName() + " hit " + enemy->getName() + " on the " + EnumMapper::mapAttackName(activeAttackType), sf::Color::White);
			OutputFormatter::chat(chatbox, enemy->getName() + " suffers " + OutputFormatter::shortFloat(healthDmg) + " HP damage", sf::Color::White);
			ResourceManager::getInstance().getSound(player->getEquipmentSet()->getPrimaryWeapon()->getAttackSoundName()).play();
		}
	}
	else
	{
		if (attacker->getCreatureType() == CreatureType::MONSTER)
		{
			OutputFormatter::chat(chatbox, enemy->getName() + "'s attack missed " + player->getPlayerName(), sf::Color::White);
		}
		else
		{
			OutputFormatter::chat(chatbox, player->getPlayerName() + "'s attack missed " + enemy->getName(), sf::Color::White);
		}
		ResourceManager::getInstance().getSound("miss").play();
	}

	if (healthDmg > 0)
	{
		updateHealth(defender, healthDmg);
	}

	if (attributeDmg > 0)
	{
		updateAttributes(defender, attributeDmg, activeAttack);
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
	playerArmorSpeedMalusSum = player->getEquipmentSet()->getStats(ArmorStatsMode::SUM, ArmorStatsAttributes::SPEED);
	playerWeaponSpeedMalusMax = player->getEquipmentSet()->getStats(ArmorStatsMode::MAX, ArmorStatsAttributes::SPEED);

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

	float scaledAttackerSpeed = 0.f;
	float scaledDefenderSpeed = 0.f;

	// determine creatures' speed
	attackerSpeedTemp = attacker->speed;
	defenderSpeedTemp = defender->speed;

	// include a finishedStages Bonus for Monsters
	if (defender->getCreatureType() == CreatureType::MONSTER)
	{
		defenderSpeedTemp = defenderSpeedTemp * (1 + finishedStages / finishedStagesDivisor);
		if (defenderSpeedTemp > 0)
		{
			scaledDefenderSpeed = (calcCreatureSpeed(defender) / defenderSpeedTemp);
		}
	}
	if (attacker->getCreatureType() == CreatureType::MONSTER)
	{
		attackerSpeedTemp = attackerSpeedTemp * (1 + finishedStages / finishedStagesDivisor);
		if (attackerSpeedTemp > 0)
		{
			scaledAttackerSpeed = (calcCreatureSpeed(attacker) / attackerSpeedTemp);
		}
	}

	// perform caltulations
	attackerSpeedBase = attackerSpeedTemp + scaledAttackerSpeed * speedBase;
	defenderSpeedBase = defenderSpeedTemp + scaledDefenderSpeed * speedBase;
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
	float const accuracyMultiplier = 4.0f;

	// results
	float accuracy;

	// get accuracy of monster
	if (creature->getCreatureType() == CreatureType::MONSTER)
	{
		return creature->accuracy;
	}

	//perform calculations
	accuracy = creature->accuracy / 100 * player->getEquipmentSet()->getStats(ArmorStatsMode::AVG, ArmorStatsAttributes::ACCURACY) * accuracyMultiplier;

	return accuracy;
}

bool Fight::calcHitSuccess(std::shared_ptr<Creature> attacker, std::shared_ptr<Creature> defender, std::shared_ptr<Attack> activeAttack)
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
	else	{

	}

	float scaledAttackerAccuracy = 0.f;

	if (attacker->accuracy > 0.f)
	{
		scaledAttackerAccuracy = calcCreatureAccuracy(attacker) / attacker->accuracy;
	}

	//perform calculations
	attackerAccuracyBase = activeAttack->getHitProbability() * (attacker->accuracy + scaledAttackerAccuracy * accuracyBase);
	attackerAccuracy = MinAvgMax(randAccuracyLow, randAccuracyHigh, attackerAccuracyBase);
	defenderSpeed = speedBasedSuccess(attacker, defender).defender;

	std::cout << "attackerAccuracyBase: " << attackerAccuracyBase << std::endl;
	std::cout << "AttackerAccuracy: min " << attackerAccuracy.min << std::endl;
	std::cout << "AttackerAccuracy: avg " << attackerAccuracy.avg << std::endl;
	std::cout << "AttackerAccuracy: max " << attackerAccuracy.max << std::endl;
	std::cout << "DefenderSpeed: min" << defenderSpeed.min << std::endl;
	std::cout << "DefenderSpeed: avg" << defenderSpeed.avg << std::endl;
	std::cout << "DefenderSpeed: max" << defenderSpeed.max << std::endl;

	// calculate randomized values
	randAttackerAccuracy = Chances::randomFloat(attackerAccuracy.min, attackerAccuracy.max);
	randDefenderSpeed = Chances::randomFloat(defenderSpeed.min, defenderSpeed.max);

	std::cout << "randAttackerAccuracy: " << randAttackerAccuracy << std::endl;
	std::cout << "randDefenderSpeed: " << randDefenderSpeed << std::endl;

	//determine hitSuccess - always hit after successful parry
	if (randAttackerAccuracy >= randDefenderSpeed || parryMultiplier > 1.f)
	{
		hitSuccess = true;
		std::cout << "hitSuccess = true" << std::endl;
	}
	else
	{
		hitSuccess = false;
		std::cout << "hitSuccess = false" << std::endl;
	}
	return hitSuccess;
}

float Fight::calcHealthDamage(std::shared_ptr<Creature> attacker, std::shared_ptr<Creature> defender, std::shared_ptr<Attack> activeAttack)
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
		attackerWeaponDamage = player->getEquipmentSet()->getStats(ArmorStatsMode::SUM, ArmorStatsAttributes::ATTACK);
		finishedStagesMult = 1;
	}

	/*std::cout << "attackerWeaponDamage: " << attackerWeaponDamage << std::endl;
*/
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
	else if (defender->getCreatureType() == CreatureType::PLAYER)
	{
		defenderArmor = player->getEquipmentSet()->getStats(ArmorStatsMode::SUM, ArmorStatsAttributes::ARMOR);
		finishedStagesMult = 1;
		baseDefense = baseDefensePlayer;
	}

	/*std::cout << "defenderArmor: " << defenderArmor << std::endl;
	std::cout << "baseDefense: " << baseDefense << std::endl;*/

	// perform calculations
	attackDamage = MinAvgMax( activeAttack->getAttackStatsLowMultiplier(), activeAttack->getAttackStatsHighMultiplier(), activeAttack->getHpDamageMultiplier() );
	attackerRawDamage = MinAvgMax(
		attackDamage.min * (strengthMult * attacker->strength + weaponMult * attackerWeaponDamage),
		attackDamage.max * (strengthMult * attacker->strength + weaponMult * attackerWeaponDamage)
	);

	defenderDefense = finishedStagesMult * baseDefense + armorMult * defenderArmor; // can never fy 0 because of baseDefense
	healthDamage = MinAvgMax(
		parryMultiplier * attackerRawDamage.min / defenderDefense * damageMult,
		parryMultiplier * attackerRawDamage.max / defenderDefense * damageMult
	);

	/*std::cout << "parryMultiplier: " << parryMultiplier << std::endl;

	std::cout << "Calculated Damage: min:" << attackDamage.min << std::endl;
	std::cout << "Calculated Damage: avg:" << attackDamage.avg << std::endl;
	std::cout << "Calculated Damage: max:" << attackDamage.max << std::endl;
	std::cout << "Calculated Defense: " << defenderDefense << std::endl;
	std::cout << "Calculated HealthDamage: min:" << healthDamage.min << std::endl;
	std::cout << "Calculated HealthDamage: avg:" << healthDamage.avg << std::endl;
	std::cout << "Calculated HealthDamage: max:" << healthDamage.max << std::endl;*/

	// calculate a randomized result
	randHealthDamage = Chances::randomFloat(healthDamage.min, healthDamage.max);

	/*std::cout << "Actual Randomized Damage: " << randHealthDamage << std::endl;*/

	return randHealthDamage;
}

float Fight::calcAttributeDamage(std::shared_ptr<Creature> defender, std::shared_ptr<Attack> activeAttack)
{
	// constants
	float const generalAttributeDamageMult = 1.f;
	float const playerAttributeDamageMult = 1.f * generalAttributeDamageMult;
	float const easyAttributeDamageMult = 1.05f * generalAttributeDamageMult;
	float const normalAtributeDamageMult = 1.f * generalAttributeDamageMult;
	float const hardAttributeDamageMult = 0.95f * generalAttributeDamageMult;

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
		/*std::cout << "defender is monster" << std::endl;*/
	}
	else
	{
		attributeDamageMult = playerAttributeDamageMult;
		/*std::cout << "defender is player" << std::endl;*/
	}

	/*std::cout << "attributeDamageMult: " << attributeDamageMult << std::endl;
	std::cout << "defenderAttribute: " << defenderAttribute << std::endl;*/

	// perform calculations
	defenderDamageMult = attributeDamageMult * activeAttack->getAttributeDamageMultiplier();
	attributeDamage = defenderAttribute - defenderAttribute * defenderDamageMult;

	/*std::cout << "attributeDamage before: " << attributeDamage << std::endl;*/

	attributeDamage = Chances::randomFloat(attributeDamage * activeAttack->getAttackStatsLowMultiplier(), attributeDamage * activeAttack->getAttackStatsHighMultiplier());

	/*std::cout << "defenderDamageMult: " << defenderDamageMult << std::endl;
	std::cout << "activeAttack->getAttributeDamageMultiplier(): " << activeAttack->getAttributeDamageMultiplier() << std::endl;
	std::cout << "attributeDamage: " << attributeDamage << std::endl;*/

	return attributeDamage;
}

void Fight::updateHealth(std::shared_ptr<Creature> defender, float healthDamage)
{
	float hp = defender->hp - healthDamage;
	if (hp < 0) hp = 0;
	defender->hp = hp;
}

void Fight::updateAttributes(std::shared_ptr<Creature> defender, float attributeDamage, std::shared_ptr<Attack> activeAttack)
{

	std::cout << "updating Attribute damage with " << attributeDamage << std::endl;

	if (activeAttack->getEffect() == Attribute::HP)
	{
		return;
	}

	if (defender->getCreatureType() == CreatureType::PLAYER)
	{
		OutputFormatter::chat(chatbox, player->getPlayerName() + " suffers " + OutputFormatter::shortFloat(attributeDamage) + " " + EnumMapper::mapAttributeName(activeAttack->getEffect()) + " damage", sf::Color::White);
	}
	else
	{
		OutputFormatter::chat(chatbox, enemy->getName() + " suffers " + OutputFormatter::shortFloat(attributeDamage) + " " + EnumMapper::mapAttributeName(activeAttack->getEffect()) + " damage", sf::Color::White);
	}

	switch (activeAttack->getEffect())
	{
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

void Fight::attributeBonusForWinner()
{
	switch (enemy->getKillBonusType())
	{
	case Attribute::ACCURACY:
		player->addToOrAccuracy(enemy->getKillBonus());
		break;
	case Attribute::SPEED:
		player->addToOrSpeed(enemy->getKillBonus());
		break;
	case Attribute::STRENGTH:
		player->addToOrStrength(enemy->getKillBonus());
		break;
	default:
		break;
	}

	return;
}