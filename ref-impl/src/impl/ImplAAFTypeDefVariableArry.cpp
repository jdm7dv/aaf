/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/


#ifndef __ImplAAFPropValData_h__
#include "ImplAAFPropValData.h"
#endif

#ifndef __ImplAAFTypeDefVariableArray_h__
#include "ImplAAFTypeDefVariableArray.h"
#endif

#ifndef __ImplAAFTypeDefStrongObjRef_h__
#include "ImplAAFTypeDefStrongObjRef.h"
#endif

#ifndef __ImplAAFTypeDefWeakObjRef_h__
#include "ImplAAFTypeDefWeakObjRef.h"
#endif

#ifndef __ImplAAFHeader_h_
#include "ImplAAFHeader.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"
#include "ImplAAFObjectCreation.h"

#include <assert.h>
#include <string.h>

extern "C" const aafClassID_t CLSID_AAFPropertyValue;


ImplAAFTypeDefVariableArray::ImplAAFTypeDefVariableArray ()
  : _ElementType  ( PID_TypeDefinitionVariableArray_ElementType,  "Element Type")
{
  _persistentProperties.put(_ElementType.address());
}



ImplAAFTypeDefVariableArray::~ImplAAFTypeDefVariableArray ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefVariableArray::GetType (
      ImplAAFTypeDef ** ppTypeDef) const
{
  if (! ppTypeDef) return AAFRESULT_NULL_PARAM;

  if (!_cachedElemType)
	{
	  ImplAAFDictionarySP pDict;

	  AAFRESULT hr;
	  hr = GetDictionary(&pDict);
	  if (AAFRESULT_FAILED(hr))
		return hr;
	  assert (pDict);

	  ImplAAFTypeDefVariableArray * pNonConstThis =
		  (ImplAAFTypeDefVariableArray*) this;
	  aafUID_t id = _ElementType;
	  hr = pDict->LookupType (&id, &pNonConstThis->_cachedElemType);
	  if (AAFRESULT_FAILED(hr))
		return hr;
	  assert (_cachedElemType);
	}
  assert (ppTypeDef);
  *ppTypeDef = _cachedElemType;
  assert (*ppTypeDef);
  (*ppTypeDef)->AcquireReference ();

  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
   ImplAAFTypeDefVariableArray::Initialize (
      const aafUID_t *  pID,
      ImplAAFTypeDef * pTypeDef,
      wchar_t *  pTypeName)
{
  if (! pTypeName) return AAFRESULT_NULL_PARAM;
  if (! pTypeDef)  return AAFRESULT_NULL_PARAM;
  if (! pID)       return AAFRESULT_NULL_PARAM;

  HRESULT hr;
  hr = SetName (pTypeName);
  if (! AAFRESULT_SUCCEEDED (hr)) return hr;

  hr = SetAUID (pID);
  if (! AAFRESULT_SUCCEEDED (hr)) return hr;

  aafUID_t id;
  assert (pTypeDef);
  hr = pTypeDef->GetAUID(&id);
  if (! AAFRESULT_SUCCEEDED (hr)) return hr;
  _ElementType = id;

  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefVariableArray::GetCount (
      ImplAAFPropertyValue * pPropVal,
      aafUInt32 *  pCount) const
{
  ImplAAFTypeDefSP ptd;
  AAFRESULT hr;

  if (! pPropVal) return AAFRESULT_NULL_PARAM;
  if (! pCount) return AAFRESULT_NULL_PARAM;
  // Bobt semi-hack: need non-const this in order to call
  // non-const GetType. We know we aren't mangling it, so it
  // technically is OK...
  ImplAAFTypeDefVariableArray * pNonConstThis =
	(ImplAAFTypeDefVariableArray *) this;
  hr = pNonConstThis->GetType (&ptd);
  if (AAFRESULT_FAILED(hr)) return hr;
  assert (ptd);
  assert (ptd->IsFixedSize());
  aafUInt32 elemSize = ptd->PropValSize();
  aafUInt32 propSize;
  assert (pPropVal);

  ImplAAFPropValDataSP pvd;
  pvd = dynamic_cast<ImplAAFPropValData *>(pPropVal);

  assert (pvd);
  hr = pvd->GetBitsSize (&propSize);
  if (AAFRESULT_FAILED(hr)) return hr;
  assert (pCount);
  *pCount = propSize / elemSize;

  return AAFRESULT_SUCCESS;
}


// Override from AAFTypeDef
AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefVariableArray::GetTypeCategory (eAAFTypeCategory_t *  pTid)
{
  if (!pTid) return AAFRESULT_NULL_PARAM;
  *pTid = kAAFTypeCatVariableArray;
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefVariableArray::AppendElement
(
 ImplAAFPropertyValue * /*pInPropVal*/,
 ImplAAFPropertyValue * /*pMemberPropVal*/
)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


ImplAAFTypeDefSP ImplAAFTypeDefVariableArray::BaseType() const
{
  ImplAAFTypeDefSP result;
  AAFRESULT hr = GetType (&result);
  assert (AAFRESULT_SUCCEEDED (hr));
  assert (result);
  return result;
}


void ImplAAFTypeDefVariableArray::reorder(OMByte* externalBytes,
										  size_t externalBytesSize) const
{
  ImplAAFTypeDefSP ptd = BaseType ();
  assert (ptd);

  aafUInt32 extElemSize = PropValSize ();
  aafUInt32 numElems = externalBytesSize / extElemSize;
  aafInt32 numBytesLeft = externalBytesSize;
  aafUInt32 elem = 0;

  for (elem = 0; elem < numElems; elem++)
	{
	  ptd->reorder (externalBytes, extElemSize);
	  externalBytes += extElemSize;
	  numBytesLeft -= extElemSize;
	  assert (numBytesLeft >= 0);
	}
}


size_t ImplAAFTypeDefVariableArray::externalSize(OMByte* internalBytes,
												 size_t internalBytesSize) const
{
  ImplAAFTypeDefSP ptd = BaseType ();
  assert (ptd);

  // aafUInt32 extElemSize = ptd->PropValSize ();
  // aafUInt32 intElemSize = ptd->NativeSize ();
  aafUInt32 extElemSize = ptd->externalSize (0, 0);
  aafUInt32 intElemSize = ptd->internalSize (0, 0);
  assert (intElemSize);
  aafUInt32 numElems = internalBytesSize / intElemSize;
  return numElems * extElemSize;
}


void ImplAAFTypeDefVariableArray::externalize(OMByte* internalBytes,
											  size_t internalBytesSize,
											  OMByte* externalBytes,
											  size_t externalBytesSize,
											  OMByteOrder byteOrder) const
{
  ImplAAFTypeDefSP ptd = BaseType ();
  assert (ptd);

  aafUInt32 intElemSize = ptd->internalSize (0, 0);
  aafUInt32 extElemSize = ptd->externalSize (0, 0);
  aafUInt32 numElems = internalBytesSize / intElemSize;
  aafInt32 intNumBytesLeft = externalBytesSize;
  aafInt32 extNumBytesLeft = internalBytesSize;
  aafUInt32 elem = 0;

  for (elem = 0; elem < numElems; elem++)
	{
	  ptd->externalize (internalBytes,
						intElemSize,
						externalBytes,
						extElemSize,
						byteOrder);
	  internalBytes += intElemSize;
	  externalBytes += extElemSize;
	  intNumBytesLeft -= intElemSize;
	  extNumBytesLeft -= extElemSize;
	  assert (intNumBytesLeft >= 0);
	  assert (extNumBytesLeft >= 0);
	}
}


size_t ImplAAFTypeDefVariableArray::internalSize(OMByte* externalBytes,
												 size_t externalBytesSize) const
{
  ImplAAFTypeDefSP ptd = BaseType ();
  assert (ptd);

  // aafUInt32 extElemSize = ptd->PropValSize ();
  // aafUInt32 intElemSize = ptd->NativeSize ();
  aafUInt32 extElemSize = ptd->externalSize (0, 0);
  aafUInt32 intElemSize = ptd->internalSize (0, 0);
  assert (intElemSize);
  aafUInt32 numElems = externalBytesSize / extElemSize;
  return numElems * intElemSize;
}


void ImplAAFTypeDefVariableArray::internalize(OMByte* externalBytes,
											  size_t externalBytesSize,
											  OMByte* internalBytes,
											  size_t internalBytesSize,
											  OMByteOrder byteOrder) const
{
  ImplAAFTypeDefSP ptd = BaseType ();
  assert (ptd);

  aafUInt32 intElemSize = ptd->internalSize (0, 0);
  aafUInt32 extElemSize = ptd->externalSize (0, 0);
  aafUInt32 numElems = externalBytesSize / extElemSize;
  aafInt32 intNumBytesLeft = externalBytesSize;
  aafInt32 extNumBytesLeft = internalBytesSize;
  aafUInt32 elem = 0;

  for (elem = 0; elem < numElems; elem++)
	{
	  ptd->internalize (externalBytes,
						extElemSize,
						internalBytes,
						intElemSize,
						byteOrder);
	  internalBytes += intElemSize;
	  externalBytes += extElemSize;
	  intNumBytesLeft -= intElemSize;
	  extNumBytesLeft -= extElemSize;
	  assert (intNumBytesLeft >= 0);
	  assert (extNumBytesLeft >= 0);
	}
}


aafUInt32 ImplAAFTypeDefVariableArray::pvtCount
(
 ImplAAFPropertyValue * pInPropVal
) const
{
  assert (pInPropVal);
  AAFRESULT hr;
  aafUInt32 retval;
  hr = GetCount (pInPropVal, &retval);
  assert (AAFRESULT_SUCCEEDED(hr));
  return retval;
}


aafBool ImplAAFTypeDefVariableArray::IsFixedSize (void) const
{
  return AAFFalse;
}


size_t ImplAAFTypeDefVariableArray::PropValSize (void) const
{
  assert (0);
  return 0; // not reached!
}


aafBool ImplAAFTypeDefVariableArray::IsRegistered (void) const
{
  // Only depends on registration of basic type.
  return BaseType()->IsRegistered ();
}


size_t ImplAAFTypeDefVariableArray::NativeSize (void) const
{
  assert (0);
  return 0; // not reached!
}


OMProperty * ImplAAFTypeDefVariableArray::pvtCreateOMPropertyMBS
  (OMPropertyId pid,
   const char * name) const
{
  assert (name);

  ImplAAFTypeDefSP ptd = BaseType ();
  assert (ptd);

  OMProperty * result = 0;

  if (dynamic_cast<ImplAAFTypeDefStrongObjRef*>((ImplAAFTypeDef*) ptd))
	{
	  // element is strong ref
	  result = new OMStrongReferenceVectorProperty<ImplAAFObject> (pid, name);
	}
  else if (dynamic_cast<ImplAAFTypeDefWeakObjRef*>((ImplAAFTypeDef*) ptd))
	{
	  // element is weak ref, hence implemented as AUID array.
	  // Specify a size of one element.
	  result = new OMSimpleProperty (pid, name, sizeof (aafUID_t));
	}

  else
	{
	  // We don't support variable arrays of variably-sized properties.
	  assert (ptd->IsFixedSize());
	  aafUInt32 elemSize = ptd->NativeSize ();

	  // But even though elems are fixed size, the variable array is
	  // of variable size.  Specify a size of one element.
	  result = new OMSimpleProperty (pid, name, elemSize);
	}

  assert (result);
  return result;
}


OMDEFINE_STORABLE(ImplAAFTypeDefVariableArray, AUID_AAFTypeDefVariableArray);
