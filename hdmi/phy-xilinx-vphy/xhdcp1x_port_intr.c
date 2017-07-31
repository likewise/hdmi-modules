/******************************************************************************
*
 *
 * Copyright (C) 2015, 2016, 2017 Xilinx, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details
*
******************************************************************************/
/*****************************************************************************/
/**
*
* @file xhdcp1x_port_intr.c
* @addtogroup hdcp1x_v4_0
* @{
*
* This contains the interrupt related functions of the Xilinx HDCP port driver
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who    Date     Changes
* ----- ------ -------- --------------------------------------------------
* 1.00  fidus  07/16/15 Initial release.
* </pre>
*
******************************************************************************/

/***************************** Include Files *********************************/

#include "xhdcp1x_port.h"
#include "xil_assert.h"
#include "xil_types.h"


/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Definitions *****************************/

/*****************************************************************************/
/**
* This function installs an asynchronous callback function for the given
* HandlerType:
*
* <pre>
* HandlerType                           Callback Function Type
* ------------------------------------  -------------------------------------
* (XHDCP1X_PORT_HANDLER_AUTHENTICATE)   AuthCallback
* </pre>
*
* @param	InstancePtr is a pointer to the HDCP port instance.
* @param	HandlerType specifies the type of handler.
* @param	CallbackFunc is the address of the callback function.
* @param	CallbackRef is a user data item that will be passed to the
*		callback function when it is invoked.
*
* @return
*		- XST_SUCCESS if callback function installed successfully.
*		- XST_INVALID_PARAM when HandlerType is invalid.
*
* @note		Invoking this function for a handler that already has been
*		installed replaces it with the new handler.
*
******************************************************************************/
int XHdcp1x_PortSetCallback(XHdcp1x *InstancePtr, u32 HandlerType,
                XHdcp1x_Callback CallbackFunc, void *CallbackRef)
{
	u32 Status = XST_SUCCESS;

	/* Verify arguments. */
	Xil_AssertNonvoid(InstancePtr != NULL);
	Xil_AssertNonvoid(HandlerType >= (XHDCP1X_PORT_HANDLER_AUTHENTICATE));
	Xil_AssertNonvoid(CallbackFunc != NULL);
	Xil_AssertNonvoid(CallbackRef != NULL);

	/* Check for handler type */
	switch (HandlerType) {
		/* Authentication Callback */
		case (XHDCP1X_PORT_HANDLER_AUTHENTICATE):
			InstancePtr->Port.AuthCallback = CallbackFunc;
			InstancePtr->Port.AuthRef = CallbackRef;
			InstancePtr->Port.IsAuthCallbackSet = (TRUE);
			break;

		default:
			Status = (XST_INVALID_PARAM);
			break;
	}

	return (Status);
}

/*****************************************************************************/
/**
* This handles an interrupt generated by a HDCP port device.
*
* @param	InstancePtr is the device to write to.
* @param	IntCause is the interrupt cause bit map.
*
* @return	None.
*
* @note		None.
*
******************************************************************************/
void XHdcp1x_PortHandleInterrupt(XHdcp1x *InstancePtr, u32 IntCause)
{
	const XHdcp1x_PortPhyIfAdaptor *Adaptor = NULL;

	/* Verify arguments. */
	Xil_AssertVoid(InstancePtr != NULL);

	/* Determine Adaptor */
	Adaptor = InstancePtr->Port.Adaptor;

	/* Check for adaptor function and invoke if present */
	if ((Adaptor != NULL) && (Adaptor->IntrHandler != NULL)) {
		InstancePtr->Port.Stats.IntCount++;
		(*(Adaptor->IntrHandler))(InstancePtr, IntCause);
	}
}
/** @} */
