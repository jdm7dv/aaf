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
#include "OMIdentitySet.h"

static const OMKLVKey ClosedHeaderPartitionPackKey =
  {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x05, 0x01, 0x01,
   0x0d, 0x01, 0x02, 0x01, 0x01, 0x02, 0x02, 0x00};

static const OMKLVKey ClosedBodyPartitionPackKey =
  {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x05, 0x01, 0x01,
   0x0d, 0x01, 0x02, 0x01, 0x01, 0x03, 0x02, 0x00};

static const OMKLVKey ClosedFooterPartitionPackKey =
  {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x05, 0x01, 0x01,
   0x0d, 0x01, 0x02, 0x01, 0x01, 0x04, 0x02, 0x00};

static const OMKLVKey primerPackKey =
  {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x05, 0x01, 0x01,
   0x0d, 0x01, 0x02, 0x01, 0x01, 0x05, 0x01, 0x00};

static const OMKLVKey fillKey =
  {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x01,
   0x03, 0x01, 0x02, 0x10, 0x01, 0x00, 0x00, 0x00};

static const OMUInt32 bodyPartitionOffset = 0x20000;

static const OMUInt32 KAGSize = 0x100;

static const OMUInt16 currentMajorVersion = 0xffff;
static const OMUInt16 currentMinorVersion = 0xfffb;

static const OMPropertyId PID_InterchangeObject_InstanceUID = 0x3c0a;

static const OMPropertyId PID_Root_ObjectDirectory = 0x003;
static const OMPropertyId PID_Root_FormatVersion   = 0x004;

static const OMUInt32 formatVersion = 0x004;

static const OMKLVKey objectDirectoryKey =
  {0x96, 0x13, 0xb3, 0x8a, 0x87, 0x34, 0x87, 0x46,
   0xf1, 0x02, 0x96, 0xf0, 0x56, 0xe0, 0x4d, 0x2a};


class OMStorable;
template <typename Key, typename Element>
class OMSet;
template <typename Key, typename Element>
class OMSetIterator;
template <typename Element>
class OMIdentitySetIterator;

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
  virtual void setOperationalPattern(const OMKLVKey& pattern);

    // @cmember Get the operational pattern.
  virtual OMKLVKey operationalPattern(void) const;

  typedef OMIdentitySet<OMKLVKey> LabelSet;
  typedef OMIdentitySetIterator<OMKLVKey> LabelSetIterator;

    // @cmember Add <p label> to the set of essence container labels.
  virtual void addEssenceContainerLabel(const OMKLVKey& label);

    // @cmember Is <p label> present in the set of essence container labels.
  virtual bool containsEssenceContainerLabel(const OMKLVKey& label) const;

    // @cmember Create an iterator over the set of essence container labels.
  virtual LabelSetIterator* essenceContainerLabels(void) const;

    // @cmember Set the file generation.
  virtual void setGeneration(const OMUniqueObjectIdentification& generation);

  virtual void writeHeaderPartition(void);
  virtual void writeBodyPartition(void);
  virtual void writeFooterPartition(void);
  virtual void writePartition(const OMKLVKey& key,
                              OMUInt32 KAGSize,
                              bool reorderBytes);

    // @cmember Write a fill key, a BER encoded length and
    //          <p length> bytes of fill.
  virtual void writeKLVFill(const OMUInt64& length);

    // @cmember Write fill so that the next key is page aligned.
  virtual void fillAlignK(const OMUInt64& currentPosition,
                          const OMUInt32& KAGSize);

    // @cmember Write fill so that the next value is page aligned.
  virtual void fillAlignV(const OMUInt64& currentPosition,
                          const OMUInt32& KAGSize);

  virtual void write(const OMByte* bytes,
                     OMUInt32 byteCount,
                     OMUInt32& bytesWritten);
  virtual void write(const OMUInt8& i);
  virtual void write(const OMUInt16& i, bool reorderBytes);
  virtual void write(const OMUInt32& i, bool reorderBytes);
  virtual void write(const OMUInt64& i, bool reorderBytes);
  virtual void write(const OMUniqueObjectIdentification& id,
                     bool reorderBytes);
  virtual void write(const OMByte* buffer,
                     const OMUInt32& bufferSize);

  virtual void writeKLVKey(const OMKLVKey& key);

  virtual void writeKLVLength(const OMUInt8& length);
  virtual void writeKLVLength(const OMUInt16& length);
  virtual void writeKLVLength(const OMUInt32& length);
  virtual void writeKLVLength(const OMUInt64& length);

  virtual void writeBerLength(OMUInt32 lengthSize, const OMUInt64& length);

    // @cmember The minimum size of <p i> when encoded using <f berEncode>.
    //          The returned size includes the BER length byte.
  static size_t berEncodedSize(const OMUInt64 i);

    // @cmember BER encode <p value> into <p berValueBuffer>.
  static void berEncode(OMByte* berValueBuffer,
                        size_t berValueBufferSize,
                        const OMUInt32& berValueSize,
                        const OMUInt64& value);

  virtual bool readHeaderPartition(void);

  static bool read(const OMRawStorage* store, OMUInt8& i);
  static bool read(const OMRawStorage* store, OMUInt16& i, bool reorderBytes);
  static bool read(const OMRawStorage* store, OMKLVKey& key);
  static bool readKLVLength(const OMRawStorage* store, OMUInt64& length);

  virtual void read(OMUInt8& i) const;
  virtual void read(OMUInt16& i, bool reorderBytes) const;
  virtual void read(OMUInt32& i, bool reorderBytes) const;
  virtual void read(OMUInt64& i, bool reorderBytes) const;
  virtual void read(OMUniqueObjectIdentification& id, bool reorderBytes) const;
  virtual void read(OMByte* buffer, const OMUInt32& bufferSize) const;
  virtual void read(OMByte* bytes,
                    OMUInt32 byteCount,
                    OMUInt32& bytesRead) const;

  virtual void readKLVKey(OMKLVKey& key) const;

  virtual OMUInt64 readKLVLength(void) const;

  virtual void readKLVFill(void) const;

  virtual void skipLV(void) const;

  static OMUInt64 readBerLength(const OMRawStorage* store);

  struct ObjectDirectoryEntry {
    OMStorable* _object;
    OMUInt64 _offset;
    OMUInt8 _flags;
  };

    // Object -> instanceId
  OMUniqueObjectIdentification instanceId(OMStorable* object);

    // instanceId -> object
  OMStorable* object(const OMUniqueObjectIdentification& instanceId);

    // object <-> instanceId
  void associate(OMStorable* object,
                 const OMUniqueObjectIdentification& instanceId);

    // enter object into object directory
  void enterObject(OMStorable& object, OMUInt64 position);

  void removeObject(OMStorable& object);

  bool containsObject(const OMUniqueObjectIdentification& instanceId);

  typedef OMSet<OMUniqueObjectIdentification,
                ObjectDirectoryEntry> ObjectDirectory;
  typedef OMSetIterator<OMUniqueObjectIdentification,
                        ObjectDirectoryEntry> ObjectDirectoryIterator;

  virtual void saveObjectDirectory(void);

  virtual void fixupReference(OMUInt64 patchOffset, OMUInt64 patchValue);

  virtual void restoreObjectDirectory(void);

  virtual void setObjectDirectoryReference(OMUInt64 objectDirectoryReference);

  virtual void setObjectDirectoryOffset(OMUInt64 objectDirectoryOffset);

  ObjectDirectory* instanceIdToObject(void);

  typedef OMSet<OMStorable*, OMUniqueObjectIdentification> ObjectSet;

  ObjectSet* objectToInstanceId(void);

private:
  // @access Private members.

  OMKLVKey _operationalPattern;
  LabelSet _essenceContainerLabels;
  OMUniqueObjectIdentification _generation;
  OMUInt64 _objectDirectoryOffset;    // offset of object directory
  OMUInt64 _objectDirectoryReference; // offset of object directory offset
  ObjectDirectory* _instanceIdToObject;
  ObjectSet* _objectToInstanceId;

};

#endif