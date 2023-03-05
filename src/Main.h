#pragma once
#include <cocos2d.h>
#include "MinHook.h"
#include "gd.h"
#include <conio.h>

namespace PlayLayer {
	inline bool(__thiscall* init)(gd::PlayLayer* self, gd::GJGameLevel* GJGameLevel);
	bool __fastcall initHook(gd::PlayLayer* self, int edx, gd::GJGameLevel* GJGameLevel);

	inline void(__thiscall* update)(gd::PlayLayer* self, float deltatime);
	void __fastcall updateHook(gd::PlayLayer* self, int edx, float deltatime);

	inline void(__thiscall* resetLevel)(gd::PlayLayer* self);
	void __fastcall resetLevelHook(gd::PlayLayer* self);

	inline bool(__thiscall* pushButton)(gd::PlayLayer* self, int state, bool player);
	bool __fastcall pushButtonHook(gd::PlayLayer* self, uintptr_t, int state, bool player);

	inline bool(__thiscall* releaseButton)(gd::PlayLayer* self, int state, bool player);
	bool __fastcall releaseButtonHook(gd::PlayLayer* self, uintptr_t, int state, bool player);
}

namespace PlayerObject {
	inline void(__thiscall* ringJump)(gd::PlayerObject* self, gd::GameObject* ring);
	void __fastcall ringJumpHook(gd::PlayerObject* self, int, gd::GameObject* ring);
}

namespace GameObject {
	inline void(__thiscall* activateObject)(gd::GameObject* self, gd::PlayerObject* player);
	void __fastcall activateObjectHook(gd::GameObject* self, int, gd::PlayerObject* player);
}

namespace GJBaseGameLayer {
	inline void(__thiscall* bumpPlayer)(gd::GJBaseGameLayer* self, gd::PlayerObject* player, gd::GameObject* object);
	void __fastcall bumpPlayerHook(gd::GJBaseGameLayer* self, int, gd::PlayerObject* player, gd::GameObject* object);
}

namespace Extra {
	inline void(__thiscall* dispatchKeyboardMSG)(cocos2d::CCKeypadDispatcher* self, int key, bool down);
	void __fastcall dispatchKeyboardMSGHook(cocos2d::CCKeypadDispatcher* self, void*, int key, bool down);
}

namespace Platformer {
	void Run_Mod();
}