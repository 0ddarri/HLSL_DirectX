#pragma once
#include "Singleton.h"

class Util : public Singleton<Util>
{
private:
	class ShaderKey
	{
	public:
		ShaderKey(int k, int n);
		int key;
		int settednum;

		void Checkkey();
	};

	std::list<ShaderKey*> keylist;
public:
	int num = 1;
	void SwitchShader();

	void AddSwitch(int key, int num);

	void SwitchInitialize();
};

