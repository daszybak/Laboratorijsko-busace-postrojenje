/* Automation Studio generated header file */
/* Do not edit ! */

#ifndef _ASCONT_
#define _ASCONT_

#include <bur/plctypes.h>

#ifndef _IEC_CONST
#define _IEC_CONST _WEAK const
#endif

/* Datatypes and datatypes of function blocks */
typedef struct AO_4_20
{
	/* VAR_INPUT (analog) */
	signed short IN;
	/* VAR_OUTPUT (analog) */
	signed short OUT;
	/* VAR_INPUT (digital) */
	plcbit enable;
} AO_4_20_typ;

typedef struct AI_4_20
{
	/* VAR_INPUT (analog) */
	signed short IN;
	/* VAR_OUTPUT (analog) */
	signed short OUT;
	/* VAR_INPUT (digital) */
	plcbit enable;
} AI_4_20_typ;



/* Prototyping of functions and function blocks */
void AO_4_20(struct AO_4_20* inst);
void AI_4_20(struct AI_4_20* inst);



#endif /* _ASCONT_ */

