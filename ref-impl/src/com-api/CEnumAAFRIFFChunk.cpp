//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
//
// $Id: CEnumAAFRIFFChunk.cpp,v 1.1 2006/02/15 23:02:40 jlow Exp $ $Name:  $
//
// The contents of this file are subject to the AAF SDK Public
// Source License Agreement (the "License"); You may not use this file
// except in compliance with the License.  The License is available in
// AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
// Association or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.
//
// The Original Code of this file is Copyright 1998-2005, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=



#include "CEnumAAFRIFFChunk.h"
#include "ImplEnumAAFRIFFChunk.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

#include "OMAssertions.h"
#include "OMExceptions.h"

#include <assert.h>
#include <string.h>



#include "CAAFRIFFChunk.h"
#include "ImplAAFRIFFChunk.h"


// CLSID for EnumAAFRIFFChunk 
// {ac3a788e-7814-43a6-8826-f3491e2489cf}
EXTERN_C const CLSID CLSID_EnumAAFRIFFChunk = { 0xac3a788e, 0x7814, 0x43a6, { 0x88, 0x26, 0xf3, 0x49, 0x1e, 0x24, 0x89, 0xcf } };





CEnumAAFRIFFChunk::CEnumAAFRIFFChunk (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFRoot (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplEnumAAFRIFFChunk * newRep;
      newRep = new ImplEnumAAFRIFFChunk;
      assert (newRep);
      InitRep (newRep);
    }
}


CEnumAAFRIFFChunk::~CEnumAAFRIFFChunk ()
{
}



HRESULT STDMETHODCALLTYPE
    CEnumAAFRIFFChunk::NextOne (IAAFRIFFChunk ** ppRIFFChunk)
{
  HRESULT hr;

  ImplEnumAAFRIFFChunk * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFRIFFChunk*> (pO);
  assert (ptr);

  //
  // set up for ppRIFFChunk
  //
  ImplAAFRIFFChunk * internalppRIFFChunk = NULL;
  ImplAAFRIFFChunk ** pinternalppRIFFChunk = NULL;
  if (ppRIFFChunk)
    {
      pinternalppRIFFChunk = &internalppRIFFChunk;
    }

  try
    {
      hr = ptr->NextOne
       (pinternalppRIFFChunk);
    }
  catch (OMException& e)
    {
      // OMExceptions should be handled by the impl code. However, if an
      // unhandled OMException occurs, control reaches here. We must not
      // allow the unhandled exception to reach the client code, so we
      // turn it into a failure status code.
      //
      // If the OMException contains an HRESULT, it is returned to the
      // client, if not, AAFRESULT_UNEXPECTED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNEXPECTED_EXCEPTION);
    }
  catch (OMAssertionViolation &)
    {
      // Control reaches here if there is a programming error in the
      // impl code that was detected by an assertion violation.
      // We must not allow the assertion to reach the client code so
      // here we turn it into a failure status code.
      //
      hr = AAFRESULT_ASSERTION_VIOLATION;
    }
  catch (...)
    {
      // We CANNOT throw an exception out of a COM interface method!
      // Return a reasonable exception code.
      //
      hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }

  //
  // cleanup for ppRIFFChunk
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppRIFFChunk)
        {
          pUnknown = static_cast<IUnknown *> (internalppRIFFChunk->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFRIFFChunk, (void **)ppRIFFChunk);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppRIFFChunk->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}


HRESULT STDMETHODCALLTYPE
    CEnumAAFRIFFChunk::Next (aafUInt32  count,
        IAAFRIFFChunk ** ppRIFFChunk,
        aafUInt32 *  pFetched)
{
  HRESULT hr;

  ImplEnumAAFRIFFChunk * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFRIFFChunk*> (pO);
  assert (ptr);

  //
  // set up for ppRIFFChunk
  //
  ImplAAFRIFFChunk ** internalppRIFFChunk = NULL;
  assert (count >= 0);
  internalppRIFFChunk = new ImplAAFRIFFChunk*[count];
  assert (internalppRIFFChunk);

  ImplAAFRIFFChunk ** pinternalppRIFFChunk = NULL;
  if (ppRIFFChunk)
    {
      pinternalppRIFFChunk = internalppRIFFChunk;
    }

  try
    {
      hr = ptr->Next
       (count,
        pinternalppRIFFChunk,
        pFetched);
    }
  catch (OMException& e)
    {
      // OMExceptions should be handled by the impl code. However, if an
      // unhandled OMException occurs, control reaches here. We must not
      // allow the unhandled exception to reach the client code, so we
      // turn it into a failure status code.
      //
      // If the OMException contains an HRESULT, it is returned to the
      // client, if not, AAFRESULT_UNEXPECTED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNEXPECTED_EXCEPTION);
    }
  catch (OMAssertionViolation &)
    {
      // Control reaches here if there is a programming error in the
      // impl code that was detected by an assertion violation.
      // We must not allow the assertion to reach the client code so
      // here we turn it into a failure status code.
      //
      hr = AAFRESULT_ASSERTION_VIOLATION;
    }
  catch (...)
    {
      // We CANNOT throw an exception out of a COM interface method!
      // Return a reasonable exception code.
      //
      hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }

  //
  // cleanup for ppRIFFChunk
  //
  if (SUCCEEDED(hr)||hr==AAFRESULT_NO_MORE_OBJECTS)
    {
      IUnknown *pUnknown;
      HRESULT hStat;
      aafUInt32 localIdx;
	  assert (count >= 0);
	  for (localIdx = 0; localIdx < *pFetched; localIdx++)
		{
		  pUnknown = static_cast<IUnknown *> (internalppRIFFChunk[localIdx]->GetContainer());
		  hStat = pUnknown->QueryInterface(IID_IAAFRIFFChunk, (void **)(ppRIFFChunk+localIdx));
		  assert (SUCCEEDED (hStat));
		  //pUnknown->Release();
		  internalppRIFFChunk[localIdx]->ReleaseReference(); // We are through with this pointer.
		}
    }
  delete[] internalppRIFFChunk;
  internalppRIFFChunk = 0;
  return hr;
}


HRESULT STDMETHODCALLTYPE
    CEnumAAFRIFFChunk::Skip (aafUInt32  count)
{
  HRESULT hr;

  ImplEnumAAFRIFFChunk * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFRIFFChunk*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->Skip
       (count);
    }
  catch (OMException& e)
    {
      // OMExceptions should be handled by the impl code. However, if an
      // unhandled OMException occurs, control reaches here. We must not
      // allow the unhandled exception to reach the client code, so we
      // turn it into a failure status code.
      //
      // If the OMException contains an HRESULT, it is returned to the
      // client, if not, AAFRESULT_UNEXPECTED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNEXPECTED_EXCEPTION);
    }
  catch (OMAssertionViolation &)
    {
      // Control reaches here if there is a programming error in the
      // impl code that was detected by an assertion violation.
      // We must not allow the assertion to reach the client code so
      // here we turn it into a failure status code.
      //
      hr = AAFRESULT_ASSERTION_VIOLATION;
    }
  catch (...)
    {
      // We CANNOT throw an exception out of a COM interface method!
      // Return a reasonable exception code.
      //
      hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }

  return hr;
}


HRESULT STDMETHODCALLTYPE
    CEnumAAFRIFFChunk::Reset ()
{
  ImplEnumAAFRIFFChunk * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFRIFFChunk*> (pO);
  assert (ptr);
  HRESULT hr;

  try
    {
      hr = ptr->Reset();
    }
  catch (OMException& e)
    {
      // OMExceptions should be handled by the impl code. However, if an
      // unhandled OMException occurs, control reaches here. We must not
      // allow the unhandled exception to reach the client code, so we
      // turn it into a failure status code.
      //
      // If the OMException contains an HRESULT, it is returned to the
      // client, if not, AAFRESULT_UNEXPECTED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNEXPECTED_EXCEPTION);
    }
  catch (OMAssertionViolation &)
    {
      // Control reaches here if there is a programming error in the
      // impl code that was detected by an assertion violation.
      // We must not allow the assertion to reach the client code so
      // here we turn it into a failure status code.
      //
      hr = AAFRESULT_ASSERTION_VIOLATION;
    }
  catch (...)
    {
      // We CANNOT throw an exception out of a COM interface method!
      // Return a reasonable exception code.
      //
      hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }

  return hr;
}


HRESULT STDMETHODCALLTYPE
    CEnumAAFRIFFChunk::Clone (IEnumAAFRIFFChunk ** ppEnum)
{
  HRESULT hr;

  ImplEnumAAFRIFFChunk * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFRIFFChunk*> (pO);
  assert (ptr);

  //
  // set up for ppEnum
  //
  ImplEnumAAFRIFFChunk * internalppEnum = NULL;
  ImplEnumAAFRIFFChunk ** pinternalppEnum = NULL;
  if (ppEnum)
    {
      pinternalppEnum = &internalppEnum;
    }

  try
    {
      hr = ptr->Clone
       (pinternalppEnum);
    }
  catch (OMException& e)
    {
      // OMExceptions should be handled by the impl code. However, if an
      // unhandled OMException occurs, control reaches here. We must not
      // allow the unhandled exception to reach the client code, so we
      // turn it into a failure status code.
      //
      // If the OMException contains an HRESULT, it is returned to the
      // client, if not, AAFRESULT_UNEXPECTED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNEXPECTED_EXCEPTION);
    }
  catch (OMAssertionViolation &)
    {
      // Control reaches here if there is a programming error in the
      // impl code that was detected by an assertion violation.
      // We must not allow the assertion to reach the client code so
      // here we turn it into a failure status code.
      //
      hr = AAFRESULT_ASSERTION_VIOLATION;
    }
  catch (...)
    {
      // We CANNOT throw an exception out of a COM interface method!
      // Return a reasonable exception code.
      //
      hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }

  //
  // cleanup for ppEnum
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppEnum)
        {
          pUnknown = static_cast<IUnknown *> (internalppEnum->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IEnumAAFRIFFChunk, (void **)ppEnum);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppEnum->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}


//
// 
// 
inline int EQUAL_UID(const GUID & a, const GUID & b)
{
  return (0 == memcmp((&a), (&b), sizeof (aafUID_t)));
}
HRESULT CEnumAAFRIFFChunk::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IEnumAAFRIFFChunk)) 
    { 
        *ppvObj = (IEnumAAFRIFFChunk *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFRoot::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(EnumAAFRIFFChunk)

