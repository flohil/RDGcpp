#ifndef FIGHT_INCLUDE
#define FIGHT_INCLUDE

#include "gameObjects.hpp"
#include "enums.hpp"

class Fight
{
public:
	Fight(std::shared_ptr<Player> player_, std::shared_ptr<Creature> creature_);
	~Fight();

	std::shared_ptr<Creature> fight();

	void handleFightOptions();

private:
	// VARIABLES
	bool activeFight;
	std::map<Attacks::Enum, Attack> attacks;
	Attack* activeAttack;
	float parryMultiplier;
	//GameEnvironment gameEnvironment;
	//Chat chat;
	//ResourceManager resource;
	std::shared_ptr<Creature> enemy;
	std::shared_ptr<Player> player;
	bool changeTabActive;
	bool potionTakingActive;
	//ArmorView armorview;
	//AttackScreens attackScreen;
	float finishedStages;
	bool attackSet;
	Attacks::Enum activeAttackType;
	std::shared_ptr<Potion> selectedPotion;
	float enemyAttackHealthDamage;
	float enemyAttackAttributeDamage;
	float enemyArmorSum;
	bool enemyFinished;

	// METHODS
	void resetRoundVariables();
	std::shared_ptr<Potion> getSelectedPotion(std::shared_ptr<Creature> creature1);
	Attacks::Enum getCommand(std::shared_ptr<Creature> creature);
	void attackControl(std::shared_ptr<Creature> creature1, std::shared_ptr<Creature> creature2);
	void attack(std::shared_ptr<Creature> attacker, std::shared_ptr<Creature> defender);
	float calcCreatureSpeed(std::shared_ptr<Creature> creature);
	AttDefMinAvgMax speedBasedSuccess(std::shared_ptr<Creature> attacker, std::shared_ptr<Creature> defender);
	float determineFirstAttack();
	bool parrySuccess(std::shared_ptr<Creature> attacker, std::shared_ptr<Creature> defender);
	float calcCreatureAccuracy(std::shared_ptr<Creature> creature);
	float calcHitSuccess(std::shared_ptr<Creature> attacker, std::shared_ptr<Creature> defender);
	float calcHealthDamage(std::shared_ptr<Creature> attacker, std::shared_ptr<Creature> defender);
	float calcAttributeDamage(std::shared_ptr<Creature> defender);
	void updateHealth(std::shared_ptr<Creature> defender, float attributeDamage);
	void updateAttributes(std::shared_ptr<Creature> defender, float attributeDamage);
	void usePotion(std::shared_ptr<Creature> potionUser, std::shared_ptr<Creature> opponent, std::shared_ptr<Potion> potion);
	void potionEffects(std::shared_ptr<Creature> creature);
	void revertEffect(std::shared_ptr<Creature>, Potion potion);
	void potionDecrease(std::shared_ptr<Creature> creature, Potion potion);
	void potionIncrease(std::shared_ptr<Creature> creature, Potion potion);
	void attributeBonusForWinner(std::shared_ptr<Creature>);

};

#endif //FIGHT_INCLUDE