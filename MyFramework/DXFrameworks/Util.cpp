#include "DXUT.h"
#include "Util.h"

void Util::SwitchShader()
{
	for (auto& it : keylist)
	{
		it->Checkkey();
	}
}

void Util::AddSwitch(int key, int num)
{
	ShaderKey* k = new ShaderKey(key, num);
	keylist.push_back(k);
}

void Util::SwitchInitialize()
{
	AddSwitch(VK_F1, 1);
	AddSwitch(VK_F2, 2);
	AddSwitch(VK_F3, 3);
	AddSwitch(VK_F4, 4);
}

Util::ShaderKey::ShaderKey(int k, int n) : settednum(n), key(k)
{

}

void Util::ShaderKey::Checkkey()
{
	if (DXUTWasKeyPressed(key))
	{
		Util::GetIns()->num = settednum;
		std::cout << "½¦ÀÌ´õ ³Ñ¹ö : " << Util::GetIns()->num << std::endl;
	}
}
