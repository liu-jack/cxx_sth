#pragma once
#include "def/TypeDef.h"

class Player;

class PalaceSkill {
public:
    static bool HaveOffice(Player& player, int officeId);

    static void FlyTo(Player& player, int cityId);   //�ʵ�-���
    static void Empty(Player& player, int cityId);   //����-�ճ�
    static void Confuse(Player& player, int cityId); //Ԫ��-����
    static void Maze(Player& player, int cityId);    //̫ʦ-�Թ�
    static void Trap(Player& player, int cityId);    //��-����

	//TODO��ÿ��һ��ɱ����Գǳ�ʹ�ã��ҷ����ɱ����������������30����
	//TODO��ÿ�����ι�Ա�����ѵġ��������CD 30����

private:
    static bool RefreshSkillCD(Player& player, int officeId);
};
