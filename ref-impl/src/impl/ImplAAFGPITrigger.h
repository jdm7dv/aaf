//@doc
//@class    AAFGPITrigger | Implementation class for AAFGPITrigger
#ifndef __ImplAAFGPITrigger_h__
#define __ImplAAFGPITrigger_h__

//=---------------------------------------------------------------------=
//
// $Id: ImplAAFGPITrigger.h,v 1.8 2009/06/01 11:47:06 stuart_hc Exp $ $Name:  $
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

#ifndef __ImplAAFEvent_h__
#include "ImplAAFEvent.h"
#endif


class ImplAAFGPITrigger : public ImplAAFEvent
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFGPITrigger ();

protected:
  virtual ~ImplAAFGPITrigger ();

public:


  //****************
  // GetActiveState()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetActiveState
        // @parm [out, retval] pointer to the result
        (aafBool *  pActiveState);


  //****************
  // SetActiveState()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetActiveState
        // @parm [in] the active state of the trigger
        (aafBool  ActiveState);

protected:
  OMFixedSizeProperty<aafBool> _activeState;
};

#endif // ! __ImplAAFGPITrigger_h__


