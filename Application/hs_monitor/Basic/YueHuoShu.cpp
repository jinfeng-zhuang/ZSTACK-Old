#include "../main.h"

int YueHuoShu(struct hs_target *target)
{
	target->blood -= 1;

	return 0;
}

int JiHuoA(struct hs_target *target)
{
	user.crystal += 1;
}