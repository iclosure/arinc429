### Output file ###
OUTFILE = EZUSB.LIB

### Object Files ###
OBJ_FILES = resume.obj susp.obj discon.obj delayms.obj delay.obj i2c.obj ezregs.obj get_dscr.obj get_infc.obj get_strd.obj get_cnfg.obj usbirqcl.obj i2c_rw.obj

### Tool options ###
CC_OPTS = debug oe code small moddp2 "ot(6,size)"
ASM_OPTS = debug errorprint nomod51
# debug -- Turn on debugging information in the .obj file
# errorprint -- Print errors to screen, not just to the listing
# nomod51 -- Turn off SFR definitions for standard 8051, allows use of reg320.inc

### Implicit Rules for BORLAND ###
.SUFFIXES: .c .a51

.c.obj:
	c51 $< $(CC_OPTS)

.a51.obj:
	a51 $< $(ASM_OPTS)


### Implicit Rules for UNIX ###
#%.obj:%.c
#	c51 $< $(CC_OPTS)

#%.obj:%.a51
#	a51 $< $(ASM_OPTS)

### Dependencies ###
EZ_USB: $(OUTFILE) USBJmpTb.obj

$(OUTFILE): $(OBJ_FILES)
	-del $(OUTFILE)
	lib51 create $(OUTFILE)
	&lib51 add $** to $(OUTFILE)