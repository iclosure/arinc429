#include <stdio.h>
#include <fx2.h>

CONFIGDSCR xdata *	EZUSB_GetConfigDscr(BYTE ConfigIdx)
{
	BYTE		index = 0;
	WORD		length;
	CONFIGDSCR	xdata *dscr;

	dscr = (CONFIGDSCR	xdata *) pConfigDscr;

	while(dscr->type == CONFIG_DSCR)
	{
		 if(index++ == ConfigIdx)
			return(dscr);
		length = dscr->config_len;
		SWAP_ENDIAN(length);
		dscr = (CONFIGDSCR xdata *)((WORD)dscr + length);
	}

	return(NULL);
}

