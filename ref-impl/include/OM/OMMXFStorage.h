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
#ifndef OMMXFSTORAGE_H
#define OMMXFSTORAGE_H

#include "OMWrappedRawStorage.h"
#include "OMDataTypes.h"

class OMStorable;
template <typename Key, typename Element>
class OMSet;
template <typename Key, typename Element>
class OMSetIterator;

  // @class Class supporting access to the raw bytes of MXF
  //        files supported by the Object Manager.
  //   @base public | <c OMRawStorage>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMMXFStorage : public OMWrappedRawStorage {
public:
  // @access Public members.

    // @cmember Constructor.
  OMMXFStorage(OMRawStorage* store);

    // @cmember Destructor.
  virtual ~OMMXFStorage(void);

    // @cmember Set the operational pattern to <p pattern>.
  virtual void setOperationalPattern(const OMObjectIdentification& pattern);

    // @cmember Get the operational pattern.
  virtual OMObjectIdentification operationalPattern(void) const;

  struct ObjectDirectoryEntry {
    OMStorable* _object;
    OMUInt64 _offset;
    OMUInt8 _flags;
  };

  typedef OMSet<OMUniqueObjectIdentification,
                ObjectDirectoryEntry> ObjectDirectory;
  typedef OMSetIterator<OMUniqueObjectIdentification,
                        ObjectDirectoryEntry> ObjectDirectoryIterator;

  ObjectDirectory* instanceIdToObject(void);

  typedef OMSet<OMStorable*, OMUniqueObjectIdentification> ObjectSet;

  ObjectSet* objectToInstanceId(void);

private:
  // @access Private members.

  OMObjectIdentification _operationalPattern;
  ObjectDirectory* _instanceIdToObject;
  ObjectSet* _objectToInstanceId;

};

#endif
