/*
 * @file mazda_miata_1_6.cpp
 *
 *
 * https://github.com/rusefi/rusefi/wiki/Frankenso_MazdaMiataNA6_pnp
 *
 *
 * Frankenso board
 * set engine_type 41
 *
 * @date Apr 16, 2016
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "custom_engine.h"
#include "mazda_miata_1_6.h"
#include "mazda_miata_base_maps.h"
#include "mre_meta.h"

static const uint8_t hardCodedveLoadBins[16] = {20, 30, 34, 38, 43, 48, 54, 60, 67, 74, 82, 90, 99, 108, 118, 128};

static const uint16_t hardCodedveRpmBins[16] = {650, 950, 1250, 1600, 1950, 2350, 2750, 3200, 3700, 4200, 4700, 5200, 5800, 6400, 7100, 7800};

static const uint8_t hardCodedveTable[16][16] = {
/* Generated by TS2C on Sun Aug 02 10:02:25 EDT 2020*/
{/* 0 20.000	*//* 0 650.0*/40,	/* 1 950.0*/42,	/* 2 1250.0*/44,	/* 3 1600.0*/44,	/* 4 1950.0*/45,	/* 5 2350.0*/45,	/* 6 2750.0*/42,	/* 7 3200.0*/41,	/* 8 3700.0*/39,	/* 9 4200.0*/41,	/* 10 4700.0*/43,	/* 11 5200.0*/47,	/* 12 5800.0*/52,	/* 13 6400.0*/55,	/* 14 7100.0*/58,	/* 15 7800.0*/58,	},
{/* 1 30.000	*//* 0 650.0*/46,	/* 1 950.0*/49,	/* 2 1250.0*/49,	/* 3 1600.0*/49,	/* 4 1950.0*/49,	/* 5 2350.0*/50,	/* 6 2750.0*/51,	/* 7 3200.0*/50,	/* 8 3700.0*/48,	/* 9 4200.0*/49,	/* 10 4700.0*/54,	/* 11 5200.0*/58,	/* 12 5800.0*/64,	/* 13 6400.0*/66,	/* 14 7100.0*/67,	/* 15 7800.0*/68,	},
{/* 2 34.000	*//* 0 650.0*/51,	/* 1 950.0*/55,	/* 2 1250.0*/56,	/* 3 1600.0*/55,	/* 4 1950.0*/56,	/* 5 2350.0*/57,	/* 6 2750.0*/59,	/* 7 3200.0*/58,	/* 8 3700.0*/55,	/* 9 4200.0*/56,	/* 10 4700.0*/59,	/* 11 5200.0*/65,	/* 12 5800.0*/70,	/* 13 6400.0*/71,	/* 14 7100.0*/72,	/* 15 7800.0*/73,	},
{/* 3 38.000	*//* 0 650.0*/55,	/* 1 950.0*/58,	/* 2 1250.0*/60,	/* 3 1600.0*/62,	/* 4 1950.0*/62,	/* 5 2350.0*/63,	/* 6 2750.0*/66,	/* 7 3200.0*/66,	/* 8 3700.0*/63,	/* 9 4200.0*/63,	/* 10 4700.0*/67,	/* 11 5200.0*/72,	/* 12 5800.0*/76,	/* 13 6400.0*/78,	/* 14 7100.0*/78,	/* 15 7800.0*/78,	},
{/* 4 43.000	*//* 0 650.0*/62,	/* 1 950.0*/64,	/* 2 1250.0*/68,	/* 3 1600.0*/70,	/* 4 1950.0*/70,	/* 5 2350.0*/71,	/* 6 2750.0*/74,	/* 7 3200.0*/74,	/* 8 3700.0*/71,	/* 9 4200.0*/70,	/* 10 4700.0*/73,	/* 11 5200.0*/78,	/* 12 5800.0*/82,	/* 13 6400.0*/83,	/* 14 7100.0*/84,	/* 15 7800.0*/84,	},
{/* 5 48.000	*//* 0 650.0*/69,	/* 1 950.0*/72,	/* 2 1250.0*/75,	/* 3 1600.0*/78,	/* 4 1950.0*/78,	/* 5 2350.0*/80,	/* 6 2750.0*/82,	/* 7 3200.0*/81,	/* 8 3700.0*/78,	/* 9 4200.0*/78,	/* 10 4700.0*/78,	/* 11 5200.0*/83,	/* 12 5800.0*/87,	/* 13 6400.0*/90,	/* 14 7100.0*/91,	/* 15 7800.0*/91,	},
{/* 6 54.000	*//* 0 650.0*/75,	/* 1 950.0*/79,	/* 2 1250.0*/82,	/* 3 1600.0*/84,	/* 4 1950.0*/84,	/* 5 2350.0*/85,	/* 6 2750.0*/86,	/* 7 3200.0*/86,	/* 8 3700.0*/84,	/* 9 4200.0*/83,	/* 10 4700.0*/85,	/* 11 5200.0*/89,	/* 12 5800.0*/93,	/* 13 6400.0*/96,	/* 14 7100.0*/97,	/* 15 7800.0*/98,	},
{/* 7 60.000	*//* 0 650.0*/80,	/* 1 950.0*/83,	/* 2 1250.0*/86,	/* 3 1600.0*/90,	/* 4 1950.0*/88,	/* 5 2350.0*/88,	/* 6 2750.0*/90,	/* 7 3200.0*/89,	/* 8 3700.0*/89,	/* 9 4200.0*/89,	/* 10 4700.0*/91,	/* 11 5200.0*/95,	/* 12 5800.0*/98,	/* 13 6400.0*/101,	/* 14 7100.0*/103,	/* 15 7800.0*/104,	},
{/* 8 67.000	*//* 0 650.0*/85,	/* 1 950.0*/87,	/* 2 1250.0*/90,	/* 3 1600.0*/94,	/* 4 1950.0*/93,	/* 5 2350.0*/92,	/* 6 2750.0*/94,	/* 7 3200.0*/93,	/* 8 3700.0*/93,	/* 9 4200.0*/95,	/* 10 4700.0*/97,	/* 11 5200.0*/101,	/* 12 5800.0*/105,	/* 13 6400.0*/107,	/* 14 7100.0*/109,	/* 15 7800.0*/110,	},
{/* 9 74.000	*//* 0 650.0*/90,	/* 1 950.0*/92,	/* 2 1250.0*/94,	/* 3 1600.0*/99,	/* 4 1950.0*/98,	/* 5 2350.0*/98,	/* 6 2750.0*/99,	/* 7 3200.0*/98,	/* 8 3700.0*/99,	/* 9 4200.0*/101,	/* 10 4700.0*/105,	/* 11 5200.0*/108,	/* 12 5800.0*/111,	/* 13 6400.0*/112,	/* 14 7100.0*/114,	/* 15 7800.0*/114,	},
{/* 10 82.000	*//* 0 650.0*/96,	/* 1 950.0*/96,	/* 2 1250.0*/99,	/* 3 1600.0*/102,	/* 4 1950.0*/103,	/* 5 2350.0*/103,	/* 6 2750.0*/103,	/* 7 3200.0*/105,	/* 8 3700.0*/106,	/* 9 4200.0*/109,	/* 10 4700.0*/111,	/* 11 5200.0*/114,	/* 12 5800.0*/116,	/* 13 6400.0*/118,	/* 14 7100.0*/118,	/* 15 7800.0*/118,	},
{/* 11 90.000	*//* 0 650.0*/100,	/* 1 950.0*/101,	/* 2 1250.0*/103,	/* 3 1600.0*/107,	/* 4 1950.0*/107,	/* 5 2350.0*/106,	/* 6 2750.0*/106,	/* 7 3200.0*/108,	/* 8 3700.0*/112,	/* 9 4200.0*/114,	/* 10 4700.0*/117,	/* 11 5200.0*/120,	/* 12 5800.0*/121,	/* 13 6400.0*/122,	/* 14 7100.0*/122,	/* 15 7800.0*/121,	},
{/* 12 99.000	*//* 0 650.0*/104,	/* 1 950.0*/105,	/* 2 1250.0*/107,	/* 3 1600.0*/109,	/* 4 1950.0*/109,	/* 5 2350.0*/108,	/* 6 2750.0*/108,	/* 7 3200.0*/110,	/* 8 3700.0*/114,	/* 9 4200.0*/117,	/* 10 4700.0*/120,	/* 11 5200.0*/123,	/* 12 5800.0*/125,	/* 13 6400.0*/124,	/* 14 7100.0*/125,	/* 15 7800.0*/124,	},
{/* 13 108.000	*//* 0 650.0*/105,	/* 1 950.0*/106,	/* 2 1250.0*/107,	/* 3 1600.0*/108,	/* 4 1950.0*/109,	/* 5 2350.0*/108,	/* 6 2750.0*/109,	/* 7 3200.0*/110,	/* 8 3700.0*/114,	/* 9 4200.0*/117,	/* 10 4700.0*/119,	/* 11 5200.0*/123,	/* 12 5800.0*/124,	/* 13 6400.0*/126,	/* 14 7100.0*/126,	/* 15 7800.0*/125,	},
{/* 14 118.000	*//* 0 650.0*/105,	/* 1 950.0*/106,	/* 2 1250.0*/107,	/* 3 1600.0*/109,	/* 4 1950.0*/110,	/* 5 2350.0*/109,	/* 6 2750.0*/109,	/* 7 3200.0*/111,	/* 8 3700.0*/114,	/* 9 4200.0*/116,	/* 10 4700.0*/119,	/* 11 5200.0*/121,	/* 12 5800.0*/123,	/* 13 6400.0*/125,	/* 14 7100.0*/128,	/* 15 7800.0*/128,	},
{/* 15 128.000	*//* 0 650.0*/104,	/* 1 950.0*/105,	/* 2 1250.0*/107,	/* 3 1600.0*/108,	/* 4 1950.0*/108,	/* 5 2350.0*/110,	/* 6 2750.0*/111,	/* 7 3200.0*/112,	/* 8 3700.0*/114,	/* 9 4200.0*/116,	/* 10 4700.0*/118,	/* 11 5200.0*/119,	/* 12 5800.0*/121,	/* 13 6400.0*/124,	/* 14 7100.0*/127,	/* 15 7800.0*/130,	}
};

/* Generated by TS2C on Sun Aug 02 10:02:25 EDT 2020*/
static void setMapVeTable() {
	copyArray(config->veLoadBins, hardCodedveLoadBins);
	copyArray(config->veRpmBins, hardCodedveRpmBins);
	copyTable(config->veTable, hardCodedveTable);
}

#if IGN_LOAD_COUNT == DEFAULT_IGN_LOAD_COUNT
static const uint8_t mapBased16IgnitionTable[16][16] = {
		/* Generated by TS2C on Wed Jun 15 21:22:11 EDT 2016*/
		{/* 0 18.000	*//* 0 650.0*/10,	/* 1 1100.0*/11,	/* 2 1550.0*/12,	/* 3 2000.0*/14,	/* 4 2450.0*/15,	/* 5 2900.0*/17,	/* 6 3350.0*/18,	/* 7 3800.0*/20,	/* 8 4250.0*/21,	/* 9 4700.0*/22,	/* 10 5150.0*/24,	/* 11 5650.0*/25,	/* 12 6100.0*/27,	/* 13 6550.0*/28,	/* 14 7000.0*/30,	/* 15 7450.0*/30,	},
		{/* 1 24.000	*//* 0 650.0*/10,	/* 1 1100.0*/11,	/* 2 1550.0*/12,	/* 3 2000.0*/14,	/* 4 2450.0*/15,	/* 5 2900.0*/16,	/* 6 3350.0*/18,	/* 7 3800.0*/19,	/* 8 4250.0*/21,	/* 9 4700.0*/22,	/* 10 5150.0*/23,	/* 11 5650.0*/25,	/* 12 6100.0*/26,	/* 13 6550.0*/28,	/* 14 7000.0*/29,	/* 15 7450.0*/29,	},
		{/* 2 33.000	*//* 0 650.0*/10,	/* 1 1100.0*/11,	/* 2 1550.0*/13,	/* 3 2000.0*/14,	/* 4 2450.0*/15,	/* 5 2900.0*/16,	/* 6 3350.0*/17,	/* 7 3800.0*/19,	/* 8 4250.0*/20,	/* 9 4700.0*/21,	/* 10 5150.0*/22,	/* 11 5650.0*/24,	/* 12 6100.0*/25,	/* 13 6550.0*/26,	/* 14 7000.0*/27,	/* 15 7450.0*/27,	},
		{/* 3 39.000	*//* 0 650.0*/10,	/* 1 1100.0*/11,	/* 2 1550.0*/13,	/* 3 2000.0*/14,	/* 4 2450.0*/15,	/* 5 2900.0*/16,	/* 6 3350.0*/17,	/* 7 3800.0*/18,	/* 8 4250.0*/20,	/* 9 4700.0*/21,	/* 10 5150.0*/22,	/* 11 5650.0*/23,	/* 12 6100.0*/24,	/* 13 6550.0*/25,	/* 14 7000.0*/27,	/* 15 7450.0*/27,	},
		{/* 4 45.000	*//* 0 650.0*/11,	/* 1 1100.0*/12,	/* 2 1550.0*/13,	/* 3 2000.0*/14,	/* 4 2450.0*/15,	/* 5 2900.0*/16,	/* 6 3350.0*/17,	/* 7 3800.0*/18,	/* 8 4250.0*/19,	/* 9 4700.0*/20,	/* 10 5150.0*/21,	/* 11 5650.0*/22,	/* 12 6100.0*/24,	/* 13 6550.0*/25,	/* 14 7000.0*/26,	/* 15 7450.0*/26,	},
		{/* 5 51.000	*//* 0 650.0*/11,	/* 1 1100.0*/11,	/* 2 1550.0*/12,	/* 3 2000.0*/13,	/* 4 2450.0*/14,	/* 5 2900.0*/15,	/* 6 3350.0*/16,	/* 7 3800.0*/17,	/* 8 4250.0*/18,	/* 9 4700.0*/19,	/* 10 5150.0*/21,	/* 11 5650.0*/22,	/* 12 6100.0*/23,	/* 13 6550.0*/24,	/* 14 7000.0*/25,	/* 15 7450.0*/25,	},
		{/* 6 60.000	*//* 0 650.0*/9,	/* 1 1100.0*/10,	/* 2 1550.0*/11,	/* 3 2000.0*/12,	/* 4 2450.0*/13,	/* 5 2900.0*/14,	/* 6 3350.0*/15,	/* 7 3800.0*/16,	/* 8 4250.0*/17,	/* 9 4700.0*/18,	/* 10 5150.0*/19,	/* 11 5650.0*/20,	/* 12 6100.0*/21,	/* 13 6550.0*/22,	/* 14 7000.0*/23,	/* 15 7450.0*/23,	},
		{/* 7 66.000	*//* 0 650.0*/9,	/* 1 1100.0*/10,	/* 2 1550.0*/11,	/* 3 2000.0*/12,	/* 4 2450.0*/13,	/* 5 2900.0*/14,	/* 6 3350.0*/15,	/* 7 3800.0*/16,	/* 8 4250.0*/17,	/* 9 4700.0*/18,	/* 10 5150.0*/19,	/* 11 5650.0*/20,	/* 12 6100.0*/21,	/* 13 6550.0*/22,	/* 14 7000.0*/23,	/* 15 7450.0*/23,	},
		{/* 8 72.000	*//* 0 650.0*/8,	/* 1 1100.0*/9,	/* 2 1550.0*/10,	/* 3 2000.0*/11,	/* 4 2450.0*/12,	/* 5 2900.0*/13,	/* 6 3350.0*/14,	/* 7 3800.0*/15,	/* 8 4250.0*/16,	/* 9 4700.0*/17,	/* 10 5150.0*/18,	/* 11 5650.0*/19,	/* 12 6100.0*/20,	/* 13 6550.0*/21,	/* 14 7000.0*/22,	/* 15 7450.0*/22,	},
		{/* 9 78.000	*//* 0 650.0*/8,	/* 1 1100.0*/9,	/* 2 1550.0*/10,	/* 3 2000.0*/11,	/* 4 2450.0*/12,	/* 5 2900.0*/13,	/* 6 3350.0*/14,	/* 7 3800.0*/14,	/* 8 4250.0*/15,	/* 9 4700.0*/16,	/* 10 5150.0*/17,	/* 11 5650.0*/18,	/* 12 6100.0*/19,	/* 13 6550.0*/20,	/* 14 7000.0*/21,	/* 15 7450.0*/21,	},
		{/* 10 84.000	*//* 0 650.0*/7,	/* 1 1100.0*/8,	/* 2 1550.0*/9,	/* 3 2000.0*/10,	/* 4 2450.0*/11,	/* 5 2900.0*/12,	/* 6 3350.0*/13,	/* 7 3800.0*/14,	/* 8 4250.0*/15,	/* 9 4700.0*/16,	/* 10 5150.0*/17,	/* 11 5650.0*/18,	/* 12 6100.0*/19,	/* 13 6550.0*/19,	/* 14 7000.0*/20,	/* 15 7450.0*/20,	},
		{/* 11 93.000	*//* 0 650.0*/7,	/* 1 1100.0*/8,	/* 2 1550.0*/8,	/* 3 2000.0*/9,	/* 4 2450.0*/10,	/* 5 2900.0*/11,	/* 6 3350.0*/12,	/* 7 3800.0*/13,	/* 8 4250.0*/14,	/* 9 4700.0*/15,	/* 10 5150.0*/16,	/* 11 5650.0*/17,	/* 12 6100.0*/17,	/* 13 6550.0*/18,	/* 14 7000.0*/19,	/* 15 7450.0*/19,	},
		{/* 12 99.000	*//* 0 650.0*/6,	/* 1 1100.0*/6,	/* 2 1550.0*/8,	/* 3 2000.0*/9,	/* 4 2450.0*/9,	/* 5 2900.0*/10,	/* 6 3350.0*/11,	/* 7 3800.0*/12,	/* 8 4250.0*/13,	/* 9 4700.0*/14,	/* 10 5150.0*/15,	/* 11 5650.0*/16,	/* 12 6100.0*/17,	/* 13 6550.0*/17,	/* 14 7000.0*/18,	/* 15 7450.0*/18,	},
		{/* 13 105.000	*//* 0 650.0*/6,	/* 1 1100.0*/7,	/* 2 1550.0*/7,	/* 3 2000.0*/8,	/* 4 2450.0*/9,	/* 5 2900.0*/10,	/* 6 3350.0*/11,	/* 7 3800.0*/12,	/* 8 4250.0*/12,	/* 9 4700.0*/13,	/* 10 5150.0*/14,	/* 11 5650.0*/15,	/* 12 6100.0*/16,	/* 13 6550.0*/17,	/* 14 7000.0*/18,	/* 15 7450.0*/18,	},
		{/* 14 111.000	*//* 0 650.0*/5,	/* 1 1100.0*/6,	/* 2 1550.0*/7,	/* 3 2000.0*/8,	/* 4 2450.0*/8,	/* 5 2900.0*/9,	/* 6 3350.0*/10,	/* 7 3800.0*/11,	/* 8 4250.0*/12,	/* 9 4700.0*/13,	/* 10 5150.0*/13,	/* 11 5650.0*/14,	/* 12 6100.0*/15,	/* 13 6550.0*/16,	/* 14 7000.0*/17,	/* 15 7450.0*/17,	},
		{/* 15 120.000	*//* 0 650.0*/5,	/* 1 1100.0*/5,	/* 2 1550.0*/6,	/* 3 2000.0*/7,	/* 4 2450.0*/8,	/* 5 2900.0*/8,	/* 6 3350.0*/9,	/* 7 3800.0*/10,	/* 8 4250.0*/11,	/* 9 4700.0*/12,	/* 10 5150.0*/12,	/* 11 5650.0*/13,	/* 12 6100.0*/14,	/* 13 6550.0*/15,	/* 14 7000.0*/16,	/* 15 7450.0*/16,	},
};
#endif

void miataNAcommonEngineSettings() {
	// Base engine
	engineConfiguration->displacement = 1.6;
	engineConfiguration->cylindersCount = 4;
	engineConfiguration->firingOrder = FO_1_3_4_2;

	engineConfiguration->rpmHardLimit = 7200;

	engineConfiguration->cylinderBore = 78;
	engineConfiguration->knockBandCustom = 6.8;

	strcpy(engineConfiguration->engineMake, ENGINE_MAKE_MAZDA);
	strcpy(engineConfiguration->engineCode, "NA6");
	engineConfiguration->vehicleWeight = 950;
	engineConfiguration->compressionRatio = 9.0;

	engineConfiguration->injectionMode = IM_BATCH;
	engineConfiguration->ignitionMode = IM_WASTED_SPARK;

	// Trigger
	engineConfiguration->trigger.type = trigger_type_e::TT_MAZDA_MIATA_NA;

	// Cranking
	engineConfiguration->primingDelay = 1.0;

	engineConfiguration->cranking.rpm = 400;
	engineConfiguration->crankingTimingAngle = 10;
	engineConfiguration->cranking.baseFuel = 27.5;

	config->crankingFuelCoef[0] = 3.36; // base cranking fuel adjustment coefficient
	config->crankingFuelBins[0] = -20; // temperature in C
	config->crankingFuelCoef[1] = 2.97;
	config->crankingFuelBins[1] = -10;
	config->crankingFuelCoef[2] = 2.69;
	config->crankingFuelBins[2] = 5;
	config->crankingFuelCoef[3] = 2.15;
	config->crankingFuelBins[3] = 30;
	config->crankingFuelCoef[4] = 1.78;
	config->crankingFuelBins[4] = 40;
	config->crankingFuelCoef[5] = 1.47;
	config->crankingFuelBins[5] = 50;
	config->crankingFuelCoef[6] = 1.0;
	config->crankingFuelBins[6] = 65;
	config->crankingFuelCoef[7] = 1.0;
	config->crankingFuelBins[7] = 90;

	// Idle
	engineConfiguration->idle.solenoidFrequency = 500;

	engineConfiguration->idleMode = IM_AUTO;
	engineConfiguration->manIdlePosition = 20;

	// below 20% this valve seems to be opening for fail-safe idle air
	engineConfiguration->idleRpmPid.minValue = 20;
	engineConfiguration->idleRpmPid.pFactor = 0.01;
	engineConfiguration->idleRpmPid.iFactor = 0.00001;
	engineConfiguration->idleRpmPid.dFactor = 0.0001;

	engineConfiguration->useIdleTimingPidControl = true;
	engineConfiguration->idleTimingPid.pFactor = 0.1;
	engineConfiguration->idleTimingPid.dFactor = 0.0001;
	engineConfiguration->idleTimingPid.minValue = -15;
	engineConfiguration->idleTimingPid.maxValue =  15;

	// Idle timing
	static const uint16_t idleAdvanceBins[] = { 500, 650, 800, 950, 1050, 1200, 1350, 1500 };
	copyArray(config->idleAdvanceBins, idleAdvanceBins);
	setArrayValues(config->idleAdvance, 15);

	// Fan
	engineConfiguration->enableFan1WithAc = true;
	engineConfiguration->enableFan2WithAc = true;

	// AC
	engineConfiguration->acDelay = 0.5;

	// Tach
	engineConfiguration->tachPulsePerRev = 2;

	// Tables
	copyTable(config->ignitionTable, mapBased16IgnitionTable);
	miataNA_setCltIdleCorrBins();
	miataNA_setCltIdleRpmBins();
	miataNA_setIacCoastingBins();
	miataNA_setIgnitionTable();
	setMapVeTable();
	setTable(config->injectionPhase, 400);

	static const float cltFuelMultValues[] = { 1.25, 1.23, 1.2, 1.18, 1.16, 1.14, 1.11, 1.09, 1.07, 1.05, 1.02, 1, 1, 1, 1, 1 };
	copyArray(config->cltFuelCorr, cltFuelMultValues);

	/**
	 * http://miataturbo.wikidot.com/fuel-injectors
	 * 90-93 (Blue) - #195500-1970
	 */
	engineConfiguration->injector.flow = 212;

	static const float deadtimeBins[] = { 8, 9.6, 11.2, 12.8, 13.2, 14.4, 15.3, 16 };
	static const float deadtimeValues[] = { 1.97, 1.52, 1.23, 1.04, 0.99, 0.9, 0.85, 0.73 };
	copyArray(engineConfiguration->injector.battLagCorrBins, deadtimeBins);
	copyArray(engineConfiguration->injector.battLagCorr, deadtimeValues);

	// Sensors
	// TPS
	// my car was originally a manual so proper TPS
	engineConfiguration->tpsMin = 92;
	engineConfiguration->tpsMax = 872;

	// CLT/IAT
	engineConfiguration->clt.config = { -20, 40, 80, 16150, 1150, 330, 2700 };
	engineConfiguration->iat.config = { -20, 40, 80, 16150, 1150, 330, 2700 };

	engineConfiguration->map.sensor.type = MT_GM_3_BAR;

	// Vehicle speed/gears
	engineConfiguration->totalGearsCount = 5;
	engineConfiguration->gearRatio[0] = 3.14;
	engineConfiguration->gearRatio[1] = 1.89;
	engineConfiguration->gearRatio[2] = 1.33;
	engineConfiguration->gearRatio[3] = 1.00;
	engineConfiguration->gearRatio[4] = 0.81;

	// These may need to change based on your real car
	engineConfiguration->driveWheelRevPerKm = 551;
	engineConfiguration->finalGearRatio = 4.3;

	// This should be correct for factory matched speedo gears and diffs,
	// but will need to be adjusted for mismatched combinations.

	// - 6 teeth on transmission output shaft
	// - 23 teeth on speedometer sensor
	// - 4.3 rear axle ratio
	// 4.3 * 6 / 23 ~= 1.12
	engineConfiguration->vssGearRatio = 4.3 * 6 / 23;
	engineConfiguration->vssToothCount = 4;

	// Set up closed loop fuel
	engineConfiguration->fuelClosedLoopCorrectionEnabled = true;
	engineConfiguration->stft.minAfr = 10;
	engineConfiguration->stft.cellCfgs[0] = { 5, -5, 5 };
	engineConfiguration->stft.cellCfgs[1] = { 15, -15, 10 };
	engineConfiguration->stft.cellCfgs[2] = { 15, -15, 1 };
	engineConfiguration->stft.cellCfgs[3] = { 5, -5, 30 };
}

/**
 * MIATA_NA6_MAP
 */
void setMiataNA6_MAP_Frankenso() {
	setFrankensoConfiguration();
	miataNAcommonEngineSettings();

	// Frankenso middle plug 2J, W32 top <> W47 bottom "#5 Green" jumper, not OEM
	engineConfiguration->map.sensor.hwChannel = EFI_ADC_4;

	engineConfiguration->mafAdcChannel = EFI_ADC_0;

	// Wide band oxygen (from middle plug) to W52
	engineConfiguration->afr.hwChannel = EFI_ADC_13; // PA3

	engineConfiguration->vbattDividerCoeff = 9.75;// ((float) (8.2 + 33)) / 8.2 * 2;

	engineConfiguration->isSdCardEnabled = true;

	engineConfiguration->injectionPins[0] = Gpio::D3; // #1&3 pin 3U
	engineConfiguration->injectionPins[1] = Gpio::E2; // #2&4 pin 3V
	engineConfiguration->injectionPins[2] = Gpio::Unassigned;
	engineConfiguration->injectionPins[3] = Gpio::Unassigned;

	// white wire from 1E - TOP of W4 to BOTTOM W62
	engineConfiguration->malfunctionIndicatorPin = Gpio::D5;

	// yellow wire from 1V/W22 to bottom of W48
	engineConfiguration->clutchDownPin = Gpio::A3;
	engineConfiguration->clutchDownPinMode = PI_PULLUP;

	// green wire from 1Q/W17 to bottom of W46
	engineConfiguration->acSwitch = Gpio::A6;

#if ! EFI_UNIT_TEST
	// W57 PE3 A/C compressor relay out
	engineConfiguration->acRelayPin = Gpio::E3;
	// W58 PE4 A/C fan relay out
#endif /* EFI_UNIT_TEST */

	engineConfiguration->idle.solenoidPin = Gpio::B9; // this W61 <> W61 jumper, pin 3W

	engineConfiguration->ignitionPins[0] = Gpio::E14; // Frankenso high side - pin 1G
	engineConfiguration->ignitionPins[1] = Gpio::Unassigned;
	engineConfiguration->ignitionPins[2] = Gpio::C7; // Frankenso high side - pin 1H
	engineConfiguration->ignitionPins[3] = Gpio::Unassigned;
}

void setHellenNA94() {
	miataNAcommonEngineSettings();
	strcpy(engineConfiguration->engineCode, "94");
	/**
	 * http://miataturbo.wikidot.com/fuel-injectors
	 * 94-97 Tan - #195500-2180
	 */
	engineConfiguration->injector.flow = 254;

	engineConfiguration->displacement = 1.8;
	engineConfiguration->injectionMode = IM_SEQUENTIAL;
	engineConfiguration->map.sensor.type = MT_MPX4250;

	engineConfiguration->fanOnTemperature = 100;
	engineConfiguration->fanOffTemperature = 96;
	engineConfiguration->fan2OnTemperature = 95;
	engineConfiguration->fan2OffTemperature = 91;

	engineConfiguration->fan2Pin = Gpio::D9; // 3S - A/C Fan 94-95
}

void setHellenNA6() {
	miataNAcommonEngineSettings();
	engineConfiguration->map.sensor.type = MT_MPX4250;
}

#if HW_PROTEUS
#include "proteus_meta.h"

void setMiataNa6_Polygonus() {
	miataNAcommonEngineSettings();

	// Triggers
	engineConfiguration->triggerInputPins[0] = PROTEUS_DIGITAL_1;
	engineConfiguration->triggerInputPins[1] = PROTEUS_VR_1;
	engineConfiguration->vehicleSpeedSensorInputPin = PROTEUS_DIGITAL_3;

	// Sensors
	engineConfiguration->map.sensor.type = MT_MPXH6400;
	engineConfiguration->map.sensor.hwChannel = engineConfiguration->clt.adcChannel = PROTEUS_IN_ANALOG_VOLT_1;

	engineConfiguration->clt.adcChannel = PROTEUS_IN_ANALOG_TEMP_3;
	engineConfiguration->iat.adcChannel = PROTEUS_IN_ANALOG_TEMP_2;

	engineConfiguration->tps1_1AdcChannel = PROTEUS_IN_ANALOG_VOLT_2;

#if EFI_PROD_CODE
	engineConfiguration->clutchDownPin = getAdcChannelBrainPin("", PROTEUS_IN_ANALOG_VOLT_5);
#endif // EFI_PROD_CODE
	engineConfiguration->clutchDownPinInverted = true;

	// Fuel pressure
	engineConfiguration->lowPressureFuel.hwChannel = PROTEUS_IN_ANALOG_VOLT_6;
	engineConfiguration->lowPressureFuel.v1 = 0.5;
	engineConfiguration->lowPressureFuel.value1 = 0;
	engineConfiguration->lowPressureFuel.v2 = 4.5;
	engineConfiguration->lowPressureFuel.value2 = 689.5;

	// Built in wideband controller on bus 2
	engineConfiguration->enableAemXSeries = true;
	engineConfiguration->widebandOnSecondBus = true;
	engineConfiguration->enableVerboseCanTx = true;

	// Outputs
	engineConfiguration->fuelPumpPin = PROTEUS_LS_9;

	engineConfiguration->idle.solenoidPin = PROTEUS_LS_7;

	// TODO: which is fan 1 or 2?
	engineConfiguration->fanPin = PROTEUS_LS_8;
	engineConfiguration->fanOnTemperature = 90;
	engineConfiguration->fanOffTemperature = 84;

	engineConfiguration->acSwitch = PROTEUS_DIGITAL_4;
	engineConfiguration->acRelayPin = PROTEUS_LS_10;

	engineConfiguration->malfunctionIndicatorPin = PROTEUS_LS_11;

	// Disable ETBs
	engineConfiguration->etbFunctions[0] = dc_function_e::DC_None;
	engineConfiguration->etbFunctions[1] = dc_function_e::DC_None;
}
#endif
