#pragma once
#include "def/TypeDef.h"

class Player;

class PalaceSkill {
public:
    static bool HaveOffice(Player& player, int officeId);

    static void FlyTo(Player& player, int cityId);   //皇帝-奇兵
    static void Empty(Player& player, int cityId);   //储君-空城
    static void Confuse(Player& player, int cityId); //元老-混乱
    static void Maze(Player& player, int cityId);    //太师-迷宫
    static void Trap(Player& player, int cityId);    //大将-陷阱

	//TODO：每天一次杀敌令，对城池使用，我方玩家杀敌数翻倍……持续30分钟
	//TODO：每天三次官员令，即免费的“征召令”，CD 30分钟

private:
    static bool RefreshSkillCD(Player& player, int officeId);
};
