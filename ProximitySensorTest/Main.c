/*********************************************************************************
 * Copyright:  
 * File:      ProximitySensorTest
 * Author:    Karlo Delmis
 * Created:   June 29, 2021/8:42 PM 
 *********************************************************************************/ 

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif


void _CYCLIC ProgramCyclic(void)
{
	limit_down1 = gSwitch_Limit_Down1;
	limit_down2 = gSwitch_Limit_Down2;
	
	limit_down1_counter += gSwitch_Limit_Down1;
	limit_down2_counter += gSwitch_Limit_Down2;
	
	
	limit_down1_counter += limit_down1;
	limit_down2_counter += limit_down1;
	
	
	
	limit_up1 = gSwitch_Limit_Up1;
	limit_up2 = gSwitch_Limit_Up2;
	
	limit_up1_counter += gSwitch_Limit_Up1;
	limit_up2_counter += gSwitch_Limit_Up2;
	
	
	limit_up1_counter += limit_up1;
	limit_up2_counter += limit_up2;
	
	
}


