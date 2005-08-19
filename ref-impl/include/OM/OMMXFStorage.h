//=---------------------------------------------------------------------=
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
// The Original Code of this file is Copyright 1998-2003, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

// @doc OMINTERNAL
#ifndef OMMXFRAWSTORAGE_H
#define OMMXFRAWSTORAGE_H

#include "OMRawStorage.h"
#include "OMDataTypes.h"


  // @class Class supporting access to the raw bytes of MXF
  //        files supported by the Object Manager.
  //   @base public | <c OMRawStorage>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMMXFRawStorage : public OMRawStorage {
public:
  // @access Public members.

    // @cmember Constructor.
  OMMXFRawStorage(OMRawStorage* store);

    // @cmember Destructor.
  virtual ~OMMXFRawStorage(void);

    // @cmember Is it possible to read from this <c OMMXFRawStorage> ?
  virtual bool isReadable(void) const;

    // @cmember Attempt to read the number of bytes given by <p byteCount>
    //          from the current position in this <c OMMXFRawStorage>
    //          into the buffer at address <p bytes>.
    //          The actual number of bytes read is returned in <p bytesRead>.
    //          Reading from positions greater than
    //          <mf OMMXFRawStorage::size> causes <p bytesRead> to be less
    //          than <p byteCount>. Reading bytes that have never been written
    //          returns undefined data in <p bytes>.
    //          @precondition <f isReadable()>
  virtual void read(OMByte* bytes,
                    OMUInt32 byteCount,
                    OMUInt32& bytesRead) const;

    // @cmember Attempt to read the number of bytes given by <p byteCount>
    //          from offset <p position> in this <c OMMXFRawStorage>
    //          into the buffer at address <p bytes>.
    //          The actual number of bytes read is returned in <p bytesRead>.
    //          Reading from positions greater than
    //          <mf OMMXFRawStorage::size> causes <p bytesRead> to be less
    //          than <p byteCount>. Reading bytes that have never been written
    //          returns undefined data in <p bytes>.
    //          @precondition <f isReadable()> && <f isPositionable()>
  virtual void readAt(OMUInt64 position,
                      OMByte* bytes,
                      OMUInt32 byteCount,
                      OMUInt32& bytesRead) const;

    // @cmember Is it possible to write to this <c OMMXFRawStorage> ?
  virtual bool isWritable(void) const;

    // @cmember Attempt to write the number of bytes given by <p byteCount>
    //          to the current position in this <c OMMXFRawStorage>
    //          from the buffer at address <p bytes>.
    //          The actual number of bytes written is returned in
    //          <p bytesWritten>.
    //          Writing to positions greater than
    //          <mf OMMXFRawStorage::size> causes this <c OMMXFRawStorage>
    //          to be extended, however such extension can fail, causing
    //          <p bytesWritten> to be less than <p byteCount>.
    //          @precondition <f isWritable()>
  virtual void write(const OMByte* bytes,
                     OMUInt32 byteCount,
                     OMUInt32& bytesWritten);

    // @cmember Attempt to write the number of bytes given by <p byteCount>
    //          to offset <p position> in this <c OMMXFRawStorage>
    //          from the buffer at address <p bytes>.
    //          The actual number of bytes written is returned in
    //          <p bytesWritten>.
    //          Writing to positions greater than
    //          <mf OMMXFRawStorage::size> causes this <c OMMXFRawStorage>
    //          to be extended, however such extension can fail, causing
    //          <p bytesWritten> to be less than <p byteCount>.
    //          @precondition <f isWritable()> && <f isPositionable()>
  virtual void writeAt(OMUInt64 position,
                       const OMByte* bytes,
                       OMUInt32 byteCount,
                       OMUInt32& bytesWritten);

    // @cmember May this <c OMMXFRawStorage> be changed in size ?
    //          An implementation of <c OMMXFRawStorage> for disk files
    //          would most probably return true. An implemetation
    //          for network streams would return false. An implementation
    //          for fixed size contiguous memory files (avoiding copying)
    //          would return false.
  virtual bool isExtendible(void) const;

    // @cmember The current extent of this <c OMMXFRawStorage> in bytes.
    //          The <f extent()> is the allocated size, while the <f size()>
    //          is the valid size.
    //          @precondition <f isPositionable()>
  virtual OMUInt64 extent(void) const;

    // @cmember Set the size of this <c OMMXFRawStorage> to <p newSize> bytes.
    //          If <p newSize> is greater than <mf OMMXFRawStorage::size>
    //          then this <c OMMXFRawStorage> is extended. If <p newSize>
    //          is less than <mf OMMXFRawStorage::size> then this
    //          <c OMMXFRawStorage> is truncated. Truncation may also result
    //          in the current position for <f read()> and <f write()>
    //          being set to <mf OMMXFRawStorage::size>.
    //          @precondition <f isExtendible()>
  virtual void extend(OMUInt64 newSize);

    // @cmember The current size of this <c OMMXFRawStorage> in bytes.
    //          The <f size()> is the valid size, while the <f extent()>
    //          is the allocated size.
    //          @precondition <f isPositionable()>
  virtual OMUInt64 size(void) const;

    // @cmember May the current position, for <f read()> and <f write()>,
    //          of this <c OMMXFRawStorage> be changed ?
    //          An implementation of <c OMMXFRawStorage> for disk files
    //          would most probably return true. An implemetation
    //          for network streams would return false. An implementation
    //          for memory files would return true.
  virtual bool isPositionable(void) const;

    // @cmember The current position for <f read()> and <f write()>, as an
    //          offset in bytes from the beginning of this
    //          <c OMMXFRawStorage>.
  virtual OMUInt64 position(void) const;

    // @cmember Set the current position for <f read()> and <f write()>, as an
    //          offset in bytes from the beginning of this
    //          <c OMMXFRawStorage>.
    //          precondition - isPositionable()
  virtual void setPosition(OMUInt64 newPosition) const;

    // @cmember Synchronize this <c OMMXFRawStorage> with its external
    //          representation.
  virtual void synchronize(void);

private:
  OMRawStorage* _store;
};

#endif
