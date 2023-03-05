#include "Main.h"

bool AKeyPressed = false;
bool DKeyPressed = false;
bool Awaslastkey = false;
bool LetMeResetFFS = false;

double oldX = 0;

bool isCube(gd::PlayerObject* player) {
	return (!player->m_isShip && !player->m_isBall && !player->m_isBird && !player->m_isDart && !player->m_isRobot && !player->m_isSpider);
}

void __fastcall PlayerObject::ringJumpHook(gd::PlayerObject* self, int, gd::GameObject* ring) {
	PlayerObject::ringJump(self, ring);
	ring->m_bHasBeenActivated = false;
	ring->m_bHasBeenActivatedP2 = false;
}

void __fastcall GameObject::activateObjectHook(gd::GameObject* self, int, gd::PlayerObject* player) {
	GameObject::activateObject(self, player);
	self->m_bHasBeenActivated = false;
	self->m_bHasBeenActivatedP2 = false;
}

void __fastcall GJBaseGameLayer::bumpPlayerHook(gd::GJBaseGameLayer* self, int, gd::PlayerObject* player, gd::GameObject* object) {
	GJBaseGameLayer::bumpPlayer(self, player, object);
	object->m_bHasBeenActivated = false;
	object->m_bHasBeenActivatedP2 = false;
}

bool __fastcall PlayLayer::initHook(gd::PlayLayer* self, int edx, gd::GJGameLevel* level) {
	bool ret = PlayLayer::init(self, level);
	if (!ret) return ret;

	// Fix dashing backwards crash (by cos8o)
	WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x1EEB92), "\xE9\xA7\x00", 3, NULL);

	return ret;
}

void __fastcall PlayLayer::updateHook(gd::PlayLayer* self, int edx, float deltaTime) {
	PlayLayer::update(self, deltaTime);

	auto p1 = self->m_pPlayer1;
	auto p2 = self->m_pPlayer2;

	if (isCube(p1) || p1->m_isShip || p1->m_isDart) p1->setRotation(0);
	if (isCube(p2) || p2->m_isShip || p2->m_isDart) p2->setRotation(0);

	if (AKeyPressed) {
		Awaslastkey = true;
		if (!LetMeResetFFS) {
			p1->m_position.x = oldX - (p1->m_position.x - oldX);
			p2->m_position.x = oldX - (p1->m_position.x - oldX);
		}
		p1->setScaleX(-p1->m_vehicleSize);
	}
	else {
		if (DKeyPressed) {
			Awaslastkey = false;
			p1->setScaleX(p1->m_vehicleSize);
		}
		else {
			if (!LetMeResetFFS) {
				p1->m_position.x = oldX;
				p2->m_position.x = oldX;
			}
			if (Awaslastkey) p1->setScaleX(-p1->m_vehicleSize); else p1->setScaleX(p1->m_vehicleSize);
		}
	}

	oldX = p1->m_position.x;
	LetMeResetFFS = false;
}

void __fastcall PlayLayer::resetLevelHook(gd::PlayLayer* self) {
	PlayLayer::resetLevel(self);
	LetMeResetFFS = true;
}

bool __fastcall PlayLayer::pushButtonHook(gd::PlayLayer* self, uintptr_t, int state, bool player) {
	bool ret = PlayLayer::pushButton(self, state, player);
	if (!ret) return ret;

	return ret;
}

bool __fastcall PlayLayer::releaseButtonHook(gd::PlayLayer* self, uintptr_t, int state, bool player) {
	bool ret = PlayLayer::releaseButton(self, state, player);
	if (!ret) return ret;

	return ret;
}

void __fastcall Extra::dispatchKeyboardMSGHook(cocos2d::CCKeypadDispatcher* self, void*, int key, bool down) {
	dispatchKeyboardMSG(self, key, down);

	if (key == cocos2d::KEY_A) AKeyPressed = (key == cocos2d::KEY_A && !AKeyPressed);
	if (key == cocos2d::KEY_D) DKeyPressed = (key == cocos2d::KEY_D && !DKeyPressed);
}

void Platformer::Run_Mod() {
	auto base = gd::base;

	MH_CreateHook(
		(PVOID)(base + 0x01FB780),
		PlayLayer::initHook,
		(LPVOID*)&PlayLayer::init
	);

	MH_CreateHook(
		(PVOID)(base + 0x2029C0),
		PlayLayer::updateHook,
		(LPVOID*)&PlayLayer::update
	);

	MH_CreateHook(
		(PVOID)(base + 0x20BF00),
		PlayLayer::resetLevelHook,
		(LPVOID*)&PlayLayer::resetLevel
	);

	MH_CreateHook(
		(PVOID)(base + 0x111500),
		PlayLayer::pushButtonHook,
		(LPVOID*)&PlayLayer::pushButton
	);

	MH_CreateHook(
		(PVOID)(base + 0x111660),
		PlayLayer::releaseButtonHook,
		(LPVOID*)&PlayLayer::releaseButton
	);

	MH_CreateHook(
		(PVOID)(base + 0x1F4FF0),
		PlayerObject::ringJumpHook,
		(LPVOID*)&PlayerObject::ringJump
	);

	MH_CreateHook(
		(PVOID)(base + 0xEF0E0),
		GameObject::activateObjectHook,
		(LPVOID*)&GameObject::activateObject
	);

	MH_CreateHook(
		(PVOID)(base + 0x10ed50),
		GJBaseGameLayer::bumpPlayerHook,
		(LPVOID*)&GJBaseGameLayer::bumpPlayer
	);

	auto cocos = GetModuleHandleA("libcocos2d.dll");
	auto addr = GetProcAddress(cocos, "?dispatchKeyboardMSG@CCKeyboardDispatcher@cocos2d@@QAE_NW4enumKeyCodes@2@_N@Z");
	MH_CreateHook(
		addr,
		Extra::dispatchKeyboardMSGHook,
		reinterpret_cast<void**>(&Extra::dispatchKeyboardMSG)
	);
}