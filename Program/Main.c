
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

void _INIT ProgramInit(void)
{
	limit_down = gSwitch_Limit_Down1;
	limit_up = gSwitch_Limit_Up1;
	zbroj = gSwitch_Limit_Up2+gSwitch_Limit_Down2;

}

void _CYCLIC ProgramCyclic(void)
{
	if(!limit_down || !limit_up){
		alarm = 1;
	}
	else{
		alarm = 0;
	}
	if(!alarm){
		alarm = 1;
	}
	counter += alarm;
	counter += zbroj;
	
	
}

void _EXIT ProgramExit(void)
{
	alarm = 0;
}

