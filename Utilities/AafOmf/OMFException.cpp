// @doc INTERNAL
// @com This file implements the OMF exception handling utility. 
//=---------------------------------------------------------------------=
//
// $Id: OMFException.cpp,v 1.6 2009/06/01 11:46:48 stuart_hc Exp $ $Name:  $
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#include "OMFException.h"

/*******************************************************************
Name:
	OMFException
Description:
	Test the given error code for a failure condition. If a failure
	occurred, print the given printf() style message to the log (if 
	applicable) and throw an exception.
Returns:
	None.
********************************************************************/

void OMFException::Check( omfErr_t errCode, const char *fmt, ... ) 
{
	if( errCode != OM_ERR_NONE )
	{
		if( fmt != 0 && iLogger != 0 )
		{
			va_list args;
			va_start( args, fmt );
			VaList valist( args );
			iLogger->Log( 0, fmt, valist );
			va_end( args );
		}
		throw OMFException( errCode );
	}
	return;
}
