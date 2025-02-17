#ifndef PART3STATE_H
#define PART3STATE_H

#include <states/GameState.h>
#include <ui/LTexture.h>

class Part3State : public GameState {
public:
	//Static accessor
	static Part3State* get();

	//Transitions
	bool enter() override;
	bool exit() override;

	//Main loop functions
	void handleEvent(SDL_Event& e) override;
	void update() override;
	void render() override;

private:
	//Static instance
	static Part3State sPart3State;

	//Private constructor
	Part3State();

	//Intro background
	LTexture mBackgroundTexture;

	//Intro message
	LTexture mMessageTexture;
};
#endif