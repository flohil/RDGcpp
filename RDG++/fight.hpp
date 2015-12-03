#ifndef FIGHT_INCLUDE
#define FIGHT_INCLUDE

#include "gameObjects.hpp"
#include "enums.hpp"
#include "prototypes.hpp"


struct MinAvgMax
{
	MinAvgMax(float min_, float max_, float base_) : min(min_ * base_), max(max_ * base_), avg((min_ + (max_ - min_)) * base_ / 2) {};
	MinAvgMax(float min_, float max_) : min(min_), max(max_), avg(min_ + (max_ - min_) / 2) {};
	MinAvgMax() : min(0), max(0), avg(0) {};

	float min;
	float avg;
	float max;
};

struct AttDefMinAvgMax
{
	AttDefMinAvgMax(MinAvgMax attacker_, MinAvgMax defender_) : attacker(attacker_), defender(defender_) {};

	MinAvgMax attacker;
	MinAvgMax defender;
};

class Fight
{
public:
	Fight(std::shared_ptr<Player> player_, std::shared_ptr<Monster> monster_, std::shared_ptr<PrototypeStorage> prototypeStorage, tgui::ChatBox::Ptr chatbox_);
	~Fight();

	void fightRound(Attacks::Enum, unsigned int stage);
	void addPotionToBeUsed(std::shared_ptr<Potion> potion) { 
		std::cout << (potion->getMode() == Mode::INCREMENTAL_INCREASE) << std::endl;
		selectedPotion = potion; 
	};
	unsigned int getActiveRound() const { return activeRound; };
	void setActiveRound(unsigned int activeRound_) { activeRound = activeRound_; };
	Attacks::Enum getPlayerAttackType() { return playerAttackType; }
	std::shared_ptr<Creature> getLoser() const { return loser; };
	std::shared_ptr<Monster> getEnemy() const { return enemy; };
	void Fight::end();
	bool isFightEnded() const { return isEnded; };
	void attributeBonusForWinner();

private:

	// VARIABLES
	bool isEnded = false;
	tgui::ChatBox::Ptr chatbox;
	std::shared_ptr<Creature> loser;
	bool activeFight;
	std::map<Attacks::Enum, Attack> attacks; // ?
	float parryMultiplier;
	std::shared_ptr<Monster> enemy;
	std::shared_ptr<Player> player;
	bool changeTabActive;
	bool potionTakingActive;
	float finishedStages;
	Attacks::Enum activeAttackType;
	Attacks::Enum playerAttackType;
	Attacks::Enum enemyAttackType;
	std::shared_ptr<Potion> selectedPotion;
	float enemyAttackHealthDamage;
	float enemyAttackAttributeDamage;
	float enemyArmorSum;
	bool enemyFinished;
	unsigned int activeRound = 0u;
	std::shared_ptr<PrototypeStorage> prototypeStorage;
	float minBaseMult = 0.35f;

	std::shared_ptr<Creature> creature1;
	std::shared_ptr<Creature> creature2;

	std::shared_ptr<Monster> pendingAttacker = nullptr;
	std::shared_ptr<Monster> pendingDefender = nullptr;

	Attacks::Enum chosenTask1 = Attacks::UNKNOWN;
	Attacks::Enum chosenTask2 = Attacks::UNKNOWN;

	// METHODS
	void resetRoundVariables();
	std::shared_ptr<Potion> getSelectedPotion(std::shared_ptr<Creature> creature1);
	Attacks::Enum getCommand(std::shared_ptr<Creature> creature);
	void attackControl(std::shared_ptr<Creature> creature1, std::shared_ptr<Creature> creature2, Attacks::Enum);
	void attack(std::shared_ptr<Creature> attacker, std::shared_ptr<Creature> defender, std::shared_ptr<Attack> activeAttack, bool parrySuccess);
	float calcCreatureSpeed(std::shared_ptr<Creature> creature);
	AttDefMinAvgMax speedBasedSuccess(std::shared_ptr<Creature> attacker, std::shared_ptr<Creature> defender);
	unsigned int determineFirstAttack();
	bool parrySuccess(std::shared_ptr<Creature> attacker, std::shared_ptr<Creature> defender);
	float calcCreatureAccuracy(std::shared_ptr<Creature> creature);
	bool calcHitSuccess(std::shared_ptr<Creature> attacker, std::shared_ptr<Creature> defender, std::shared_ptr<Attack> activeAttack);
	float calcHealthDamage(std::shared_ptr<Creature> attacker, std::shared_ptr<Creature> defender, std::shared_ptr<Attack> activeAttack);
	float calcAttributeDamage(std::shared_ptr<Creature> defender, std::shared_ptr<Attack> activeAttack);
	void updateHealth(std::shared_ptr<Creature> defender, float attributeDamage);
	void updateAttributes(std::shared_ptr<Creature> defender, float attributeDamage, std::shared_ptr<Attack> activeAttack);
	void usePotion(std::shared_ptr<Creature> potionUser, std::shared_ptr<Creature> opponent, std::shared_ptr<Potion> potion);
	void potionEffects(std::shared_ptr<Creature> creature);
	void revertEffect(std::shared_ptr<Creature>, std::shared_ptr<Potion> potion);
	void potionDecrease(std::shared_ptr<Creature> creature, std::shared_ptr<Potion> potion);
	void potionIncrease(std::shared_ptr<Creature> creature, std::shared_ptr<Potion> potion);
};

#endif //FIGHT_INCLUDE