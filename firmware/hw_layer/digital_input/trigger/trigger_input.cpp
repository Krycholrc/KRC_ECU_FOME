/*
 * @file	trigger_input.cpp
 *
 * @date Nov 11, 2019
 * @author Andrey Belomutskiy, (c) 2012-2021
 */

#include "global.h"
#include "trigger_input.h"

/* TODO:
 * - merge comparator trigger
 */

#if (EFI_SHAFT_POSITION_INPUT) || defined(__DOXYGEN__)

#if (HAL_USE_ICU == TRUE) || (HAL_TRIGGER_USE_PAL == TRUE)

#if (HAL_USE_ICU == TRUE)
	void icuTriggerTurnOnInputPins();
	int  icuTriggerTurnOnInputPin(const char *msg, int index, bool isTriggerShaft);
	void icuTriggerTurnOffInputPin(brain_pin_e brainPin);
#else
	#define icuTriggerTurnOnInputPins() ((void)0)
	int  icuTriggerTurnOnInputPin(const char *msg, int index, bool isTriggerShaft) {
		UNUSED(msg);
		UNUSED(index);
		UNUSED(isTriggerShaft);

		return -2;
	}
	#define icuTriggerTurnOffInputPin(brainPin) ((void)0)
#endif

#if (HAL_TRIGGER_USE_PAL == TRUE)
	void extiTriggerTurnOnInputPins();
	int  extiTriggerTurnOnInputPin(const char *msg, int index, bool isTriggerShaft);
	void extiTriggerTurnOffInputPin(brain_pin_e brainPin);
#else
	#define extiTriggerTurnOnInputPins() ((void)0)
	int  extiTriggerTurnOnInputPin(const char *msg, int index, bool isTriggerShaft) {
		UNUSED(msg);
		UNUSED(index);
		UNUSED(isTriggerShaft);

		return -2;
	}
	#define extiTriggerTurnOffInputPin(brainPin) ((void)0)
#endif

enum triggerType {
	TRIGGER_NONE,
	TRIGGER_ICU,
	TRIGGER_EXTI
};

static triggerType shaftTriggerType[TRIGGER_SUPPORTED_CHANNELS];
static triggerType camTriggerType[CAM_INPUTS_COUNT];

static int turnOnTriggerInputPin(const char *msg, int index, bool isTriggerShaft) {
	brain_pin_e brainPin = isTriggerShaft ?
		CONFIG(triggerInputPins)[index] : engineConfiguration->camInputs[index];

	if (isTriggerShaft) {
		shaftTriggerType[index] = TRIGGER_NONE;
	} else {
		camTriggerType[index] = TRIGGER_NONE;
	}

	if (!isBrainPinValid(brainPin)) {
		return 0;
	}

	/* try ICU first */
#if EFI_ICU_INPUTS
	if (icuTriggerTurnOnInputPin(msg, index, isTriggerShaft) >= 0) {
		if (isTriggerShaft) {
			shaftTriggerType[index] = TRIGGER_ICU;
		} else {
			camTriggerType[index] = TRIGGER_ICU;
		}
		return 0;
	}
#endif

	/* ... then EXTI */
	if (extiTriggerTurnOnInputPin(msg, index, isTriggerShaft) >= 0) {
		if (isTriggerShaft) {
			shaftTriggerType[index] = TRIGGER_EXTI;
		} else {
			camTriggerType[index] = TRIGGER_EXTI;
		}
		return 0;
	}

	firmwareError(CUSTOM_ERR_NOT_INPUT_PIN, "%s: Not input pin %s", msg, hwPortname(brainPin));

	return -1;
}

static void turnOffTriggerInputPin(int index, bool isTriggerShaft) {
	brain_pin_e brainPin = isTriggerShaft ?
		activeConfiguration.triggerInputPins[index] : activeConfiguration.camInputs[index];

	if (isTriggerShaft) {
#if EFI_ICU_INPUTS
		if (shaftTriggerType[index] == TRIGGER_ICU) {
			icuTriggerTurnOffInputPin(brainPin);
		}
#endif /* EFI_ICU_INPUTS */
		if (shaftTriggerType[index] == TRIGGER_EXTI) {
			extiTriggerTurnOffInputPin(brainPin);
		}

		shaftTriggerType[index] = TRIGGER_NONE;
	} else {
#if EFI_ICU_INPUTS
		if (camTriggerType[index] == TRIGGER_ICU) {
			icuTriggerTurnOffInputPin(brainPin);
		}
#endif /* EFI_ICU_INPUTS */
		if (camTriggerType[index] == TRIGGER_EXTI) {
			extiTriggerTurnOffInputPin(brainPin);
		}

		camTriggerType[index] = TRIGGER_NONE;
	}
}

/*==========================================================================*/
/* Exported functions.														*/
/*==========================================================================*/

void stopTriggerInputPins(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	for (int i = 0; i < TRIGGER_SUPPORTED_CHANNELS; i++) {
		if (isConfigurationChanged(triggerInputPins[i])) {
			turnOffTriggerInputPin(i, true);
		}
	}
	for (int i = 0; i < CAM_INPUTS_COUNT; i++) {
		if (isConfigurationChanged(camInputs[i])) {
			turnOffTriggerInputPin(i, false);
		}
	}
}

void startTriggerInputPins(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	for (int i = 0; i < TRIGGER_SUPPORTED_CHANNELS; i++) {
		if (isConfigurationChanged(triggerInputPins[i])) {
			const char * msg = (i == 0 ? "Trigger #1" : (i == 1 ? "Trigger #2" : "Trigger #3"));
			turnOnTriggerInputPin(msg, i, true);
		}
	}

	for (int i = 0; i < CAM_INPUTS_COUNT; i++) {
		if (isConfigurationChanged(camInputs[i])) {
			turnOnTriggerInputPin("Cam", i, false);
		}
	}
}

void turnOnTriggerInputPins(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	/* init all trigger HW available */
	icuTriggerTurnOnInputPins();
	extiTriggerTurnOnInputPins();

	applyNewTriggerInputPins(PASS_ENGINE_PARAMETER_SIGNATURE);
}

#endif /* (HAL_USE_ICU == TRUE) || (HAL_TRIGGER_USE_PAL == TRUE) */


void stopTriggerDebugPins(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	for (int i = 0; i < TRIGGER_INPUT_PIN_COUNT; i++) {
		if (isConfigurationChanged(triggerInputDebugPins[i])) {
			efiSetPadUnused(activeConfiguration.triggerInputDebugPins[i] PASS_ENGINE_PARAMETER_SUFFIX);
		}
	}
	for (int i = 0; i < CAM_INPUTS_COUNT; i++) {
		if (isConfigurationChanged(camInputsDebug[i])) {
			efiSetPadUnused(activeConfiguration.camInputsDebug[i] PASS_ENGINE_PARAMETER_SUFFIX);
		}
	}
}

void startTriggerDebugPins(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	for (int i = 0; i < TRIGGER_INPUT_PIN_COUNT; i++) {
		if (isConfigurationChanged(triggerInputDebugPins[i])) {
			efiSetPadMode("trigger debug", CONFIG(triggerInputDebugPins[i]), PAL_MODE_OUTPUT_PUSHPULL PASS_ENGINE_PARAMETER_SUFFIX);
		}
	}
	for (int i = 0; i < CAM_INPUTS_COUNT; i++) {
		if (isConfigurationChanged(camInputsDebug[i])) {
			efiSetPadMode("cam debug", CONFIG(camInputsDebug[i]), PAL_MODE_OUTPUT_PUSHPULL PASS_ENGINE_PARAMETER_SUFFIX);
		}
	}
}

void applyNewTriggerInputPins(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
#if EFI_PROD_CODE
	// first we will turn off all the changed pins
	stopTriggerInputPins(PASS_ENGINE_PARAMETER_SIGNATURE);
	// then we will enable all the changed pins
	startTriggerInputPins(PASS_ENGINE_PARAMETER_SIGNATURE);
#endif /* EFI_PROD_CODE */
}

#endif /* EFI_SHAFT_POSITION_INPUT */
