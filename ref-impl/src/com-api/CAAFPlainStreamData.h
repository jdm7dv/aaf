//@doc
//@class    AAFPlainStreamData | Implementation class for AAFPlainStreamData
#ifndef __CAAFPlainStreamData_h__
#define __CAAFPlainStreamData_h__

//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
//
// $Id: CAAFPlainStreamData.h,v 1.3 2007/03/16 18:07:39 akharkev Exp $ $Name:  $
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



#ifndef __AAF_h__
#include "AAF.h"
#endif






#ifndef __CAAFRoot_h__
#include "CAAFRoot.h"
#endif


class CAAFPlainStreamData
  : public IAAFPlainStreamData,
    public IAAFKLVStreamParameters,
    public CAAFRoot
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFPlainStreamData (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFPlainStreamData ();

public:



  //***********************************************************
  //
  // GetSize()
  //
  // Returns number of bytes contained in the referenced property
  /// value.
  ///
  /// Succeeds if:
  /// - Initialize() has already been called on this object.
  /// - pStreamPropertyValue is a valid pointer.
  /// - pSize is a valid pointer.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - Either pStreamPropertyValue or pSize arg is NULL.
  //
  STDMETHOD (GetSize) (
    // stream property value
    /*[in]*/ IAAFPropertyValue * pStreamPropertyValue,

    // count of bytes in the specified stream property value
    /*[out]*/ aafInt64 *  pSize);

  //***********************************************************
  //
  // SetSize()
  //
  // Set the number of bytes contained in the give stream property value
  /// to newElementCount
  /// Succeeds if:
  /// - Initialize() has already been called on this object.
  /// - pStreamPropertyValue is a valid pointer.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - If pStreamPropertyValue arg is NULL.
  //
  STDMETHOD (SetSize) (
    // stream property value
    /*[in]*/ IAAFPropertyValue * pStreamPropertyValue,

    // new count of bytes in the specified stream property value
    /*[in]*/ aafInt64  newSize);

  //***********************************************************
  //
  // GetPosition()
  //
  // Returns the byte position of the current element in the stream. 
  ///
  /// Succeeds if:
  /// - Initialize() has already been called on this object.
  /// - pStreamPropertyValue is a valid pointer.
  /// - pPosition is a valid pointer.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - Either pStreamPropertyValue or pPosition arg is NULL.
  //
  STDMETHOD (GetPosition) (
    // stream property value
    /*[in]*/ IAAFPropertyValue * pStreamPropertyValue,

    // current byte position in the specified stream property value
    /*[out]*/ aafInt64 *  pPosition);

  //***********************************************************
  //
  // SetPosition()
  //
  // Make the current byte position to the one at newPosition in the stream 
  /// property value. 
  ///
  /// Succeeds if:
  /// - pStreamPropertyValue is a valid pointer.
  /// - the new position is valid
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - If pStreamPropertyValue arg is NULL.
  ///
  /// AAFRESULT_INVALID_PARAM
  ///   - newPosition is outside the bounds of the stream.
  //
  STDMETHOD (SetPosition) (
    // stream property value
    /*[in]*/ IAAFPropertyValue * pStreamPropertyValue,

    // the new position in the specified stream property value
    /*[in]*/ aafInt64  newPosition);


  //***********************************************************
  //
  // Read()
  //
  // Sequential access.
  /// Copies the data at the position of the stream to the given
  /// buffer.
  ///
  /// Succeeds if all of the following are true:
  /// - the pStreamPropertyValue pointer is valid.
  /// - the pData pointer is valid.
  /// - the indicated bytes exist in the stream.
  ///
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - either pStreamPropertyValue or pData arg is NULL.
  //
  STDMETHOD (Read) (
    // stream property value to read
    /*[in]*/ IAAFPropertyValue * pStreamPropertyValue,

    // number of bytes to read
    /*[in]*/ aafUInt32  dataSize,

    // buffer into which one element from the stream should be written
    /*[out, size_is(dataSize), length_is(*bytesRead)]*/ aafMemPtr_t  pData,

    // number of bytes actually read (will be either dataSize or 0 if there 
  /// is in error)
    /*[out,ref]*/ aafUInt32 *  bytesRead);

  //***********************************************************
  //
  // Write()
  //
  // Sequential access.
  /// Copies the data in the given buffer into the stream at the 
  /// current position of the stream..
  ///
  /// Succeeds if all of the following are true:
  /// - the pStreamPropertyValue pointer is valid.
  /// - the pData pointer is valid.
  /// - the indicated bytes could be written to the stream.
  ///
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - Either pStreamPropertyValue or pData arg is NULL.
  //
  STDMETHOD (Write) (
    // stream property value to modify
    /*[in]*/ IAAFPropertyValue * pStreamPropertyValue,

    // number of bytes to write
    /*[in]*/ aafUInt32  dataSize,

    // buffer into which should contain one element to be written to the stream
    /*[in, ref, size_is(dataSize)]*/ aafMemPtr_t  pData);

  //***********************************************************
  //
  // Append()
  //
  // Extending the stream.
  /// Copies the data in the given buffer into the stream at the 
  /// end of the stream.
  ///
  /// Succeeds if all of the following are true:
  /// - the pStreamPropertyValue pointer is valid.
  /// - the pData pointer is valid.
  /// - the indicated bytes could be appended to the stream.
  ///
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - either pStreamPropertyValue or pData arg is NULL.
  //
  STDMETHOD (Append) (
    // stream property value to modify
    /*[in]*/ IAAFPropertyValue * pStreamPropertyValue,

    // number of bytes to write (must be equal to the element type length)
    /*[in]*/ aafUInt32  dataSize,

    // buffer into which should contain one element to be written to the stream
    /*[in, ref, size_is(dataSize)]*/ aafMemPtr_t  pData);


  //***********************************************************
  //
  // HasStoredByteOrder()
  //
  // Returns kAAFTrue if the stream has a stored byte order or 
  /// kAAFFalse otherwise.
  ///
  /// Succeeds if:
  /// - Initialize() has already been called on this object.
  /// - pStreamPropertyValue is a valid pointer.
  /// - pHasByteOrder is a valid pointer.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - Either pStreamPropertyValue or pHasByteOrder arg is NULL.
  //
  STDMETHOD (HasStoredByteOrder) (
    // stream property value
    /*[in]*/ IAAFPropertyValue * pStreamPropertyValue,

    // kAAFTrue if this stream has a stored byte order
    /*[out]*/ aafBoolean_t *  pHasByteOrder);

  //***********************************************************
  //
  // GetStoredByteOrder()
  //
  // Access byte order of the stream.
  ///
  /// Succeeds if:
  /// - Initialize() has already been called on this object.
  /// - pStreamPropertyValue is a valid pointer.
  /// - pByteOrder is a valid pointer.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - Either pStreamPropertyValue or pByteOrder arg is NULL.
  //
  STDMETHOD (GetStoredByteOrder) (
    // stream property value
    /*[in]*/ IAAFPropertyValue * pStreamPropertyValue,

    // Pointer to variable where byte order is to be copied
    /*[out]*/ eAAFByteOrder_t *  pByteOrder);

  //***********************************************************
  //
  // SetStoredByteOrder()
  //
  // Sets the byte order to be associated with this stream. Note: the stream
  /// must be empty.
  ///
  /// Succeeds if:
  /// - Initialize() has already been called on this object.
  /// - pStreamPropertyValue is a valid pointer.
  /// - stream is empty.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - If pStreamPropertyValue arg is NULL.
  //
  STDMETHOD (SetStoredByteOrder) (
    // stream property value
    /*[in]*/ IAAFPropertyValue * pStreamPropertyValue,

    // byte order is to be stored with the stream
    /*[in]*/ eAAFByteOrder_t  byteOrder);

  //***********************************************************
  //
  // ClearStoredByteOrder()
  //
  // Clears the byte order to be associated with this stream. Note: the stream
  /// must be empty.
  ///
  /// Succeeds if:
  /// - Initialize() has already been called on this object.
  /// - pStreamPropertyValue is a valid pointer.
  /// - stream is empty.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - If pStreamPropertyValue arg is NULL.
  //
  STDMETHOD (ClearStoredByteOrder) (
    // stream property value 
    /*[in]*/ IAAFPropertyValue * pStreamPropertyValue);


  //***********************************************************
  //
  // ReadElements()
  //
  // Access in typed chunks of Elements.
  /// Copies the data at the current position of the stream to the given
  /// buffer. Requires that any structures declared within element 
  /// typedef have had their offsets registered with that type.
  ///
  /// Succeeds if all of the following are true:
  /// - the pStreamPropertyValue pointer is valid.
  /// - the pElementType pointer is valid.
  /// - the pData pointer is valid.
  /// - dataSize indicates pData is large enough to hold the data.
  /// - compile-time struct has had its member offests registered.
  /// - the indicated elements exist in this stream type def.
  ///
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - either pStreamPropertyValue or pElementType or pBytesRead or pData arg is NULL.
  ///
  /// AAFRESULT_INVALID_PARAM
  ///   - dataSize indicates pData is too small.
  ///
  /// AAFRESULT_NOT_REGISTERED
  ///  - struct offsets have not yet been registered for element typedef.
  ///
  /// AAFRESULT_INVALID_PARAM
  ///   - there are not dataSize bytes left in the stream.
  //
  STDMETHOD (ReadElements) (
    // stream property value to read
    /*[in]*/ IAAFPropertyValue * pStreamPropertyValue,

    // the type definition of the elements to read
    /*[in]*/ IAAFTypeDef * pElementType,

    // number of bytes to read (must be evenly divisible by the element 
  /// type length)
    /*[in]*/ aafUInt32  dataSize,

    // buffer into which elements from the stream should be written
    /*[out, size_is(dataSize), length_is(*pBytesRead)]*/ aafMemPtr_t  pData,

    // number of bytes actually read (will be either dataSize or 0 if 
  /// there is in error)
    /*[out,ref]*/ aafUInt32 *  pBytesRead);

  //***********************************************************
  //
  // WriteElements()
  //
  // Access in typed chunks of Elements.
  /// Copies the data in the given buffer into the stream at the  
  /// current position of the stream. Requires that any structures 
  /// declared within element 
  /// typedef have had their offsets registered with that type..
  ///
  /// Succeeds if all of the following are true:
  /// - the pStreamPropertyValue pointer is valid.
  /// - the pElementType pointer is valid.
  /// - the pData pointer is valid.
  /// - dataSize indicates pData is large enough to hold the data.
  /// - compile-time struct has had its member offests registered.
  /// - the indicated elements exist in this stream type def.
  ///
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - either pStreamPropertyValue or pElementType or pData arg is NULL.
  ///
  /// AAFRESULT_INVALID_PARAM
  ///   - dataSize indicates pData is an even multiple of the given 
  ///     element type size.
  ///
  /// AAFRESULT_NOT_REGISTERED
  ///  - struct offsets have not yet been registered for the element typedef.
  //
  STDMETHOD (WriteElements) (
    // stream property value to modify
    /*[in]*/ IAAFPropertyValue * pStreamPropertyValue,

    // the type definition of the elements to read
    /*[in]*/ IAAFTypeDef * pElementType,

    // number of bytes to write (must be evenly divisible by the element type 
  /// length)
    /*[in]*/ aafUInt32  dataSize,

    // buffer into which elements from the stream should be written
    /*[in, ref, size_is(dataSize)]*/ aafMemPtr_t  pData);


  /// Extend in chunks of typed Elements 

  //***********************************************************
  //
  // AppendElements()
  //
  // Access in typed chunks of Elements.
  /// Copies the data in the given buffer onto the end of the stream. 
  /// Requires that any structures declared within element 
  /// typedef have had their offsets registered with that type..
  ///
  /// Succeeds if all of the following are true:
  /// - the pStreamPropertyValue pointer is valid.
  /// - the pElementType pointer is valid.
  /// - the pData pointer is valid.
  /// - dataSize indicates pData is large enough to hold the data.
  /// - compile-time struct has had its member offests registered.
  /// - the indicated elements exist in this stream type def.
  ///
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - either pStreamPropertyValue or pElementType or pData arg is NULL.
  ///
  /// AAFRESULT_INVALID_PARAM
  ///   - dataSize indicates pData is an even multiple of the given 
  ///     element type size.
  ///
  /// AAFRESULT_NOT_REGISTERED
  ///  - struct offsets have not yet been registered for the element typedef.
  //
  STDMETHOD (AppendElements) (
    // stream property value to modify
    /*[in]*/ IAAFPropertyValue * pStreamPropertyValue,

    // the type definition of the elements to read
    /*[in]*/ IAAFTypeDef * pElementType,

    // number of bytes to write (must be evenly divisible by the element type 
  /// length)
    /*[in]*/ aafUInt32  dataSize,

    // buffer into which elements from the stream should be written
    /*[in, ref, size_is(dataSize)]*/ aafMemPtr_t  pData);


  //***********************************************************
  // METHOD NAME: GetEssenceElementKey()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFKLVStreamParameters | GetEssenceElementKey |
  // This method returns essence element key associated
  /// with the specified stream.
  ///
  /// Succeeds if all of the following are true:
  /// - pStreamPropertyValue is a valid pointer.
  /// - pEssenceElementKey is a valid pointer.
  /// - the specified stream supports essence element keys.
  ///
  /// If this method fails nothing will be written to *pEssenceElementKey.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - either pStreamPropertyValue or pEssenceElementKey arg is NULL.
  ///
  /// AAFRESULT_INVALID_PARAM
  ///   - the specified stream does not support essence element keys.
  // @end
  // 
  STDMETHOD (GetEssenceElementKey)
   (
    // @parm [in] AAFPropertyValue | pStreamPropertyValue | stream property value
    IAAFPropertyValue * pStreamPropertyValue,

    // @parm [out] aafUID_t * | pEssenceElementKey | Essence element key.
    aafUID_t *  pEssenceElementKey
  );

  //***********************************************************
  // METHOD NAME: SetEssenceElementKey()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFKLVStreamParameters | SetEssenceElementKey |
  // Set the essence element key on the specified stream.
  /// 
  /// Succeeds if all of the following are true:
  /// - pStreamPropertyValue is a valid pointer.
  /// - the specified stream supports essence element keys.
  /// 
  /// If this method fails, the property will not be changed.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - pStreamPropertyValue arg is NULL.
  ///
  /// AAFRESULT_INVALID_PARAM
  ///   - the specified stream does not support essence element keys.
  // @end
  // 
  STDMETHOD (SetEssenceElementKey)
   (
    // @parm [in] AAFPropertyValue | pStreamPropertyValue | stream property value
    IAAFPropertyValue * pStreamPropertyValue,

    // @parm [in, ref] aafUID_constref | key | essence element key
    aafUID_constref  key
  );


protected:
  // 
  // Declare the QI that implements for the interfaces
  // for this module. This will be called by CAAFUnknown::QueryInterface().
  // 
  STDMETHOD(InternalQueryInterface)(REFIID riid, void **ppvObjOut);


public:
  //
  // This class as concrete. All AAF objects can be constructed from
  // a CLSID. This will allow subclassing all "base-classes" by
  // aggreggation.
  // 
  AAF_DECLARE_CONCRETE();
  //
  //********
};

#endif // ! __CAAFPlainStreamData_h__
