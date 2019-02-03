#ifndef ANIMATION_HANDLER_HPP
#define ANIMATION_HANDLER_HPP

#include <SFML/Graphics.hpp>
#include <vector>

class Animation
{
public:

	unsigned int startFrame;
	unsigned int endFrame;

	float duration; // amount of time one frame should last for

	Animation(unsigned int startFrame_, unsigned int endFrame_, float duration_) :
		startFrame(startFrame_), endFrame(endFrame_), duration(duration) {};

	unsigned int getLength() const { return endFrame - startFrame + 1; }
};

class AnimationHandler
{
public:

	AnimationHandler() : ellapsedTime(0.0f), currentAnim(-1) {};
	AnimationHandler(const sf::IntRect& frameSize_) : frameSize(frameSize_), ellapsedTime(0.0f), currentAnim(-1) {};

	void addAnim(Animation& anim);
	void update(const float deltaTime);
	void changeAnim(unsigned int animNum); // do not forget to reset ellapsedTime

	sf::IntRect bounds; // current section of animation textureset that should be displayed
	sf::IntRect frameSize; // pixel dimensions of a frame

private:

	std::vector<Animation> animations;
	float ellapsedTime; // Current time since the animation loop started
	int currentAnim;
};

#endif /* ANIMATION_HANDLER_HPP */