#include <SFML/Graphics.hpp>
#include <vector>

#include "animationHandler.hpp"

void AnimationHandler::update(const float deltaTime)
{
	if (currentAnim >= (int) animations.size() || currentAnim < 0)
	{
		return;
	}

	float duration = animations[currentAnim].duration;

	/* Check if the animation has progessed to a new frame and if so
	* change to the next frame */
	if (int((ellapsedTime + deltaTime) / duration) > int(ellapsedTime / duration))
	{
		int frame = int((ellapsedTime + deltaTime) / duration); // calculate frame number
		frame %= animations[currentAnim].getLength(); // adjust for looping

		// set the sprite to the new frame
		sf::IntRect rect = frameSize;
		rect.left = rect.width * frame;
		rect.top = rect.height * currentAnim;
		bounds = rect;
	}

	ellapsedTime += deltaTime;
	if (ellapsedTime > duration * animations[currentAnim].getLength()) // adjust for looping
	{
		ellapsedTime = 0.0f;
	}

	return;
}

void AnimationHandler::addAnim(Animation& anim)
{
	this->animations.push_back(anim);

	return;
}

void AnimationHandler::changeAnim(unsigned int animID)
{
	/* Do not change the animation if the animation is currently active or
	* the new animation does not exist */
	if (currentAnim == animID || animID >= animations.size() || animID < 0)
	{
		return;
	}

	currentAnim = animID;

	/* Update the animation bounds */
	sf::IntRect rect = frameSize;
	rect.top = rect.height * animID;
	bounds = rect;
	ellapsedTime = 0.0;

	return;
}