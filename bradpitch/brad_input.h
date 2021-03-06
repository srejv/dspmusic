
#define BASE_ADR 0x80001000

#define FX1 	0x000
#define FX2 	0x100
#define FX3 	0x200
#define FX4 	0x300
#define FX5 	0x400
#define FX6 	0x500
#define FX7 	0x600
#define FX8 	0x700
#define FX9		0x800
#define FX10	0x900
#define FX11	0xA00
#define FX12	0xB00
#define	FX13	0xC00
#define FX14	0xD00
#define FX15	0xE00
#define FX16 	0xF00

#define PARAM1 0x00
#define PARAM2 0x04
#define PARAM3 0x08
#define PARAM4 0x0C

#define PARAM5 0x10
#define PARAM6 0x14
#define PARAM7 0x18
#define PARAM8 0x1C

#define PARAM9  0x20
#define PARAM10 0x24
#define PARAM11 0x28
#define PARAM12 0x2C

#define PARAM13 0x30
#define PARAM14 0x34
#define PARAM15 0x38
#define PARAM16 0x3C

#define PARAM17 0x40
#define PARAM18 0x44
#define PARAM19 0x48
#define PARAM20 0x4C

#define PARAM21 0x50
#define PARAM22 0x54
#define PARAM23 0x58
#define PARAM24 0x5C

#define PARAM25 0x60
#define PARAM26 0x64
#define PARAM27 0x68
#define PARAM28 0x6C

#define PARAM29 0x70
#define PARAM30 0x74
#define PARAM31 0x78
#define PARAM32 0x7C

#define FX_DELAY			FX1
#define DELAY_ACTIVE 		(BASE_ADR + FX_DELAY + PARAM1)
#define DELAY_MIX			(BASE_ADR + FX_DELAY + PARAM2)
#define DELAY_TIME			(BASE_ADR + FX_DELAY + PARAM3)
#define DELAY_FDB			(BASE_ADR + FX_DELAY + PARAM4)

#define FX_DIST				FX2
#define DISTORTION_ACTIVE	(BASE_ADR + FX_DIST + PARAM1)
#define DISTORTION_MIX		(BASE_ADR + FX_DIST + PARAM2)
#define DISTORTION_GAIN 	(BASE_ADR + FX_DIST + PARAM3) 
#define DISTORTION_LVL1		(BASE_ADR + FX_DIST + PARAM4)
#define DISTORTION_LVL2		(BASE_ADR + FX_DIST + PARAM5)
#define DISTORTION_FDB		(BASE_ADR + FX_DIST + PARAM6)
#define DISTORTION_TYPE		(BASE_ADR + FX_DIST + PARAM7)

#define FX_VIBRATO			FX3
#define VIBRATO_ACTIVE		(BASE_ADR + FX_VIBRATO + PARAM1)
#define VIBRATO_MIX			(BASE_ADR + FX_VIBRATO + PARAM2)
#define	VIBRATO_MOD_FREQ	(BASE_ADR + FX_VIBRATO + PARAM3)
#define VIBRATO_MOD_DEPTH	(BASE_ADR + FX_VIBRATO + PARAM4)
#define VIBRATO_FEEDBACK	(BASE_ADR + FX_VIBRATO + PARAM5)


#define FX_FLANGER			FX4
#define FLANGER_ACTIVE 		(BASE_ADR + FX_FLANGER + PARAM1)
#define FLANGER_MIX			(BASE_ADR + FX_FLANGER + PARAM2)
#define	FLANGER_MOD_FREQ	(BASE_ADR + FX_FLANGER + PARAM3)
#define FLANGER_MOD_DEPTH	(BASE_ADR + FX_FLANGER + PARAM4)
#define FLANGER_FEEDBACK	(BASE_ADR + FX_FLANGER + PARAM5)


#define FX_CHORUS			FX5
#define CHORUS_ACTIVE		(BASE_ADR + FX_CHORUS + PARAM1)
#define CHORUS_MIX			(BASE_ADR + FX_CHORUS + PARAM2)
#define	CHORUS_MOD_FREQ		(BASE_ADR + FX_CHORUS + PARAM3)
#define CHORUS_MOD_DEPTH	(BASE_ADR + FX_CHORUS + PARAM4)
#define CHORUS_FEEDBACK		(BASE_ADR + FX_CHORUS + PARAM5)
#define CHORUS_OFFSET		(BASE_ADR + FX_CHORUS + PARAM6)


#define FX_PITCHT			FX6
#define PITCHT_ACTIVE		(BASE_ADR + FX_PITCHT + PARAM1)
#define PITCHT_MIX			(BASE_ADR + FX_PITCHT + PARAM2)
#define PITCHT_PITCHSTEP	(BASE_ADR + FX_PITCHT + PARAM3)
#define PITCHT_CENT			(BASE_ADR + FX_PITCHT + PARAM4)
#define PITCHT_FEEDBACK		(BASE_ADR + FX_PITCHT + PARAM5)

#define FX_EQ				FX7
#define EQ_ACTIVE			(BASE_ADR + FX_EQ + PARAM1)
#define EQ_MIX				(BASE_ADR + FX_EQ + PARAM2)
#define	EQ_LOW_GAIN			(BASE_ADR + FX_EQ + PARAM3)
#define EQ_MID_GAIN			(BASE_ADR + FX_EQ + PARAM4)
#define EQ_HIGH_GAIN		(BASE_ADR + FX_EQ + PARAM5)
#define	EQ_LOW_FREQ			(BASE_ADR + FX_EQ + PARAM6)	// FREQ SPLIT
#define EQ_MID_FREQ			(BASE_ADR + FX_EQ + PARAM7)
#define EQ_HIGH_FREQ		(BASE_ADR + FX_EQ + PARAM8)

#define FX_TREMOLO			FX8
#define TREMOLO_ACTIVE 		(BASE_ADR + FX_TREMOLO + PARAM1)
#define TREMOLO_MIX			(BASE_ADR + FX_TREMOLO + PARAM2)
#define	TREMOLO_MOD_FREQ	(BASE_ADR + FX_TREMOLO + PARAM3)
#define TREMOLO_MOD_DEPTH	(BASE_ADR + FX_TREMOLO + PARAM4)
#define TREMOLO_FEEDBACK	(BASE_ADR + FX_TREMOLO + PARAM5)

#define FX_PITCHF			FX8
#define PITCHF_ACTIVE		(BASE_ADR + FX_PITCHF + PARAM1)
#define PITCHF_PITCH_STEP	(BASE_ADR + FX_PITCHF + PARAM2)
#define PITCHF_PITCH_CENT	(BASE_ADR + FX_PITCHF + PARAM3)
#define PITCHF_MIX			(BASE_ADR + FX_PITCHF + PARAM4)
#define PITCHF_FEEDBACK		(BASE_ADR + FX_PITCHF + PARAM5)

