//=---------------------------------------------------------------------=
//
// $Id: OMFile.cpp,v 1.149 2004/11/18 12:26:48 phil_tudor Exp $ $Name:  $
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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

// @doc OMEXTERNAL
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMFile

#include "OMFile.h"

#include "OMAssertions.h"
#include "OMStoredObject.h"
#include "OMStoredObjectFactory.h"
#include "OMClassFactory.h"
#include "OMPropertyTable.h"
#include "OMUtilities.h"
#include "OMDictionary.h"
#include "OMRootStorable.h"
#include "OMRawStorage.h"
#include "OMUniqueObjectIdentType.h"
#include "OMSetIterator.h"

#include "OMCachedDiskRawStorage.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

  // @mfunc Destructor.
OMFile::~OMFile(void)
{
  TRACE("OMFile::~OMFile");

  delete _referencedProperties;
  _referencedProperties = 0;
  delete [] _fileName;
  _fileName = 0;

  delete _rawStorage;
  _rawStorage = 0;
}

  // @mfunc Open an existing <c OMFile> for read-only access, the
  //        <c OMFile> is named <p fileName>, use the <c OMClassFactory>
  //        <p factory> to create the objects. The file must already
  //        exist.
  //   @parm The name of the file to open.
  //   @parm The factory to use for creating objects.
  //   @parm Specifies the use of lazy or eager loading.
  //   @rdesc The newly opened <c OMFile>.
OMFile* OMFile::openExistingRead(const wchar_t* fileName,
                                 const OMClassFactory* factory,
                                 void* clientOnRestoreContext,
                                 const OMLoadMode loadMode,
                                 OMDictionary* dictionary)
{
  TRACE("OMFile::openExistingRead");

  PRECONDITION("Valid file name", validWideString(fileName));
  PRECONDITION("Valid class factory", factory != 0);
  PRECONDITION("Valid dictionary", dictionary != 0);

  OMStoredObjectEncoding encoding;
  bool result = isRecognized(fileName, encoding);
  ASSERT("Recognized file", result); // tjb - error
  OMStoredObjectFactory* f = findFactory(encoding);
  ASSERT("Recognized file encoding", f != 0);
  OMStoredObject* store = f->openRead(fileName);
  OMFile* newFile = new OMFile(fileName,
                               clientOnRestoreContext,
                               encoding,
                               readOnlyMode,
                               store,
                               factory,
                               dictionary,
                               loadMode);
  ASSERT("Valid heap pointer", newFile != 0);
  POSTCONDITION("File is open", newFile->isOpen());
  return newFile;
}

  // @mfunc Open an existing <c OMFile> for modify access, the
  //        <c OMFile> is named <p fileName>, use the <c OMClassFactory>
  //        <p factory> to create the objects. The file must already
  //        exist.
  //   @parm The name of the file to open.
  //   @parm The factory to use for creating objects.
  //   @parm Specifies the use of lazy or eager loading.
  //   @rdesc The newly opened <c OMFile>.
OMFile* OMFile::openExistingModify(const wchar_t* fileName,
                                   const OMClassFactory* factory,
                                   void* clientOnRestoreContext,
                                   const OMLoadMode loadMode,
                                   OMDictionary* dictionary)
{
  TRACE("OMFile::openExistingModify");

  PRECONDITION("Valid file name", validWideString(fileName));
  PRECONDITION("Valid class factory", factory != 0);
  PRECONDITION("Valid dictionary", dictionary != 0);

  OMStoredObjectEncoding encoding;
  bool result = isRecognized(fileName, encoding);
  ASSERT("Recognized file", result); // tjb - error
  OMStoredObjectFactory* f = findFactory(encoding);
  ASSERT("Recognized file encoding", f != 0);

  OMFile* newFile = 0;
  if (compatibleNamedFile(modifyMode, encoding)) {
    OMStoredObject* store = f->openModify(fileName);
    newFile = new OMFile(fileName,
                         clientOnRestoreContext,
                         encoding,
                         modifyMode,
                         store,
                         factory,
                         dictionary,
                         loadMode);
    ASSERT("Valid heap pointer", newFile != 0);
  } else {
    OMRawStorage* store = OMCachedDiskRawStorage::openExistingModify(fileName);
    ASSERT("Valid raw storage", store != 0);
    newFile = new OMFile(store,
                         clientOnRestoreContext,
			 nullOMStoredObjectEncoding, // don't care 
                         modifyMode,
                         factory,
                         dictionary,
                         loadMode);
    ASSERT("Valid heap pointer", newFile != 0);
    newFile->open();
  }
  POSTCONDITION("File is open", newFile->isOpen());
  return newFile;
}

  // @mfunc Open a new <c OMFile> for modify access, the
  //        <c OMFile> is named <p fileName>, use the <c OMClassFactory>
  //        <p factory> to create the objects. The file must not already
  //        exist. The byte ordering on the newly created file is given
  //        by <p byteOrder>. The client root <c OMStorable> in the newly
  //        created file is given by <p clientRoot>.
  //   @parm The name of the file to create.
  //   @parm The factory to use for creating objects.
  //   @parm TBS
  //   @parm The byte order to use for the newly created file.
  //   @parm The client root <c OMStorable> in the newly created file.
  //   @parm TBS
  //   @parm TBS
  //   @rdesc The newly created <c OMFile>.
OMFile* OMFile::openNewModify(const wchar_t* fileName,
                              const OMClassFactory* factory,
                              void* clientOnRestoreContext,
                              const OMByteOrder byteOrder,
                              OMStorable* clientRoot,
                              const OMStoredObjectEncoding& encoding,
                              OMDictionary* dictionary)
{
  TRACE("OMFile::openNewModify");

  PRECONDITION("Valid file name", validWideString(fileName));
  PRECONDITION("Valid class factory", factory != 0);
  PRECONDITION("Valid byte order",
                    ((byteOrder == littleEndian) || (byteOrder == bigEndian)));
  PRECONDITION("Valid client root", clientRoot != 0);
  PRECONDITION("Valid dictionary ", dictionary != 0);

  OMFile* newFile = 0;
  if (compatibleNamedFile(modifyMode, encoding)) {
    OMStoredObjectFactory* f = findFactory(encoding);
    ASSERT("Recognized file encoding", f != 0);
    OMStoredObject* store = f->createModify(fileName, byteOrder);
    OMRootStorable* root = new OMRootStorable(clientRoot, dictionary);
    ASSERT("Valid heap pointer", root != 0);

    newFile = new OMFile(fileName,
                         clientOnRestoreContext,
                         encoding,
                         modifyMode,
                         store,
                         factory,
                         dictionary,
                         root);
    ASSERT("Valid heap pointer", newFile != 0);
  } else {
    OMRootStorable* root = new OMRootStorable(clientRoot, dictionary);
    ASSERT("Valid heap pointer", root != 0);

    OMRawStorage* store = OMCachedDiskRawStorage::openNewModify(fileName);
    ASSERT("Valid raw storage", store != 0);
    newFile = new OMFile(store,
                         clientOnRestoreContext,
                         encoding,
                         modifyMode,
                         factory,
                         dictionary,
                         root,
                         byteOrder);
    ASSERT("Valid heap pointer", newFile != 0);
    newFile->open();
  }
  POSTCONDITION("File is open", newFile->isOpen());
  return newFile;
}

  // @mfunc Is the given <c OMRawStorage> compatible with the given file
  //        access mode and encoding ? Can a file of the specified encoding
  //        be created on <p rawStorage> and then accessed in mode
  //        <p accessMode> ?
  //   @parm The <c OMRawStorage>
  //   @parm The <t OMAccessMode>
  //   @parm The <t OMStoredObjectEncoding>
  //   @rdesc True if <p accessMode> and <p encoding> are compatible,
  //          false otherwise.
bool OMFile::compatibleRawStorage(const OMRawStorage* rawStorage,
                                  const OMAccessMode accessMode,
                                  const OMStoredObjectEncoding& encoding)
{
  TRACE("OMFile::compatibleRawStorage");

  bool result = false;
  OMStoredObjectFactory* factory = findFactory(encoding);
  ASSERT("Recognized file encoding", factory != 0);
  result = factory->compatibleRawStorage(rawStorage, accessMode);
  return result;
}

  // @mfunc Can a file of the encoding specified by <p encoding> be
  //        created successfully as a named file and
  //        accessed successfully in the mode specified by <p accessMode> ?
  //   @parm The <t OMAccessMode>
  //   @parm The <t OMStoredObjectEncoding>
  //   @rdesc True if <p accessMode> and <p encoding> are compatible,
  //          false otherwise.
bool OMFile::compatibleNamedFile(const OMAccessMode accessMode,
                                 const OMStoredObjectEncoding& encoding)
{
  TRACE("OMFile::compatibleNamedFile");

  bool result = false;
  OMStoredObjectFactory* factory = findFactory(encoding);
  ASSERT("Recognized file encoding", factory != 0);
  result = factory->compatibleNamedFile(accessMode);
  return result;
}

  // @mfunc Is <p rawStorage> compatible with <p accesMode> ?
  //   @parm The <c OMRawStorage>
  //   @parm The <t OMAccessMode>
  //   @rdesc True if <p rawStorage> and <p accessMode> are compatible,
  //          false otherwise.
bool OMFile::compatible(const OMRawStorage* rawStorage,
                        const OMAccessMode accessMode)
{
  TRACE("OMFile::compatible");

  bool result = false;
  switch (accessMode) {
    case readOnlyMode:
      if (rawStorage->isReadable()) {
        result = true;
      }
      break;
    case writeOnlyMode:
      if (rawStorage->isWritable()) {
        result = true;
      }
      break;
    case modifyMode:
      if (rawStorage->isReadable() && rawStorage->isWritable()) {
        result = true;
      }
      break;
  }
  return result;
}

OMFile* OMFile::openExistingRead(OMRawStorage* rawStorage,
                                 const OMClassFactory* factory,
                                 void* clientOnRestoreContext,
                                 const OMLoadMode loadMode,
				 const OMStoredObjectEncoding& encoding,
                                 OMDictionary* dictionary)
{
  TRACE("OMFile::openExistingRead");

  PRECONDITION("Valid raw storage", rawStorage != 0);
  PRECONDITION("Compatible access mode", compatible(rawStorage, readOnlyMode));
  PRECONDITION("Valid class factory", factory != 0);
  PRECONDITION("Valid dictionary", dictionary != 0);

  OMFile* newFile = new OMFile(rawStorage,
                               clientOnRestoreContext,
			       encoding,
                               readOnlyMode,
                               factory,
                               dictionary,
                               loadMode);
  ASSERT("Valid heap pointer", newFile != 0);
  return newFile;
}

OMFile* OMFile::openExistingModify(OMRawStorage* rawStorage,
                                   const OMClassFactory* factory,
                                   void* clientOnRestoreContext,
                                   const OMLoadMode loadMode,
				   const OMStoredObjectEncoding& encoding,
                                   OMDictionary* dictionary)
{
  TRACE("OMFile::openExistingModify");

  PRECONDITION("Valid raw storage", rawStorage != 0);
  PRECONDITION("Compatible access mode", compatible(rawStorage, modifyMode));
  PRECONDITION("Valid class factory", factory != 0);
  PRECONDITION("Valid dictionary", dictionary != 0);

  OMFile* newFile = new OMFile(rawStorage,
                               clientOnRestoreContext,
 			       encoding,
                               modifyMode,
                               factory,
                               dictionary,
                               loadMode);
  ASSERT("Valid heap pointer", newFile != 0);
  return newFile;
}

OMFile* OMFile::openNewWrite(OMRawStorage* rawStorage,
                             const OMClassFactory* factory,
                             void* clientOnRestoreContext,
                             const OMByteOrder byteOrder,
                             OMStorable* clientRoot,
                             const OMStoredObjectEncoding& encoding,
                             OMDictionary* dictionary)
{
  TRACE("OMFile::openNewWrite");
  PRECONDITION("Valid raw storage", rawStorage != 0);
  PRECONDITION("Compatible access mode",
                compatible(rawStorage, writeOnlyMode));
  PRECONDITION("Creatable",
		compatibleRawStorage(rawStorage, writeOnlyMode, encoding));
  PRECONDITION("Valid class factory", factory != 0);
  PRECONDITION("Valid byte order",
                    ((byteOrder == littleEndian) || (byteOrder == bigEndian)));
  PRECONDITION("Valid client root", clientRoot != 0);
  PRECONDITION("Valid dictionary ", dictionary != 0);
  OMRootStorable* root = new OMRootStorable(clientRoot, dictionary);
  ASSERT("Valid heap pointer", root != 0);
  OMFile* newFile = new OMFile(rawStorage,
                               clientOnRestoreContext,
                               encoding,
                               writeOnlyMode,
                               factory,
                               dictionary,
                               root,
                               byteOrder);
  ASSERT("Valid heap pointer", newFile != 0);
  return newFile;
}

OMFile* OMFile::openNewModify(OMRawStorage* rawStorage,
                              const OMClassFactory* factory,
                              void* clientOnRestoreContext,
                              const OMByteOrder byteOrder,
                              OMStorable* clientRoot,
                              const OMStoredObjectEncoding& encoding,
                              OMDictionary* dictionary)
{
  TRACE("OMFile::openNewModify");

  PRECONDITION("Valid raw storage", rawStorage != 0);
  PRECONDITION("Compatible access mode", compatible(rawStorage, modifyMode));
  PRECONDITION("Creatable",
                       compatibleRawStorage(rawStorage, modifyMode, encoding));
  PRECONDITION("Valid class factory", factory != 0);
  PRECONDITION("Valid byte order",
                    ((byteOrder == littleEndian) || (byteOrder == bigEndian)));
  PRECONDITION("Valid client root", clientRoot != 0);
  PRECONDITION("Valid dictionary ", dictionary != 0);

  OMRootStorable* root = new OMRootStorable(clientRoot, dictionary);
  ASSERT("Valid heap pointer", root != 0);

  OMFile* newFile = new OMFile(rawStorage,
                               clientOnRestoreContext,
                               encoding,
                               modifyMode,
                               factory,
                               dictionary,
                               root,
                               byteOrder);
  ASSERT("Valid heap pointer", newFile != 0);
  return newFile;
}

  // @mfunc Is the file named <p fileName> a recognized file ?
  //        If so, the result is true, and the encoding is returned
  //        in <p encoding>.
  //   @parm The name of the file to check.
  //   @parm If recognized, the file encoding.
  //   @rdesc True if the file is recognized, false otherwise.
bool OMFile::isRecognized(const wchar_t* fileName,
                          OMStoredObjectEncoding& encoding)
{
  TRACE("OMFile::isRecognized");
  bool result = false;

  ASSERT("Valid factory", _factory != 0);
	// first, search for preferred factories
  FactorySetIterator iterator(*_factory, OMBefore);
  while (++iterator) {
    if ( !iterator.value()->better()
			 && iterator.value()->isRecognized(fileName)) {
      result = true;
      encoding = iterator.key();
      break;
    }
  }
	// second, search for other factories
	if( !result ) {
		FactorySetIterator iterator2(*_factory, OMBefore);
		while (++iterator2) {
			if ( iterator2.value()->better()
				 && iterator2.value()->isRecognized(fileName)) {
				result = true;
				encoding = iterator2.key();
				break;
			}
		}
	}

  return result;
}

  // @mfunc Does <p rawStorage> contain a recognized file ?
  //        If so, the result is true, and the encoding is returned
  //        in <p encoding>.
  //   @parm The <c OMRawStorage> to check.
  //   @parm If recognized, the file encoding.
  //   @rdesc True if the <c OMRawStorage> contains a recognized
  //          file, false otherwise.
bool OMFile::isRecognized(OMRawStorage* rawStorage,
                          OMStoredObjectEncoding& encoding)
{
  TRACE("OMFile::isRecognized");
  bool result = false;

  ASSERT("Valid factory", _factory != 0);
  ASSERT("Positionable raw storage", rawStorage->isPositionable());
  rawStorage->setPosition(0);

	// first, search for preferred factories
  FactorySetIterator iterator(*_factory, OMBefore);
  while (++iterator) {
    ASSERT("Properly positioned raw storage", rawStorage->position() == 0);
    if ( !iterator.value()->better()
			 && iterator.value()->isRecognized(rawStorage)) {
      result = true;
      encoding = iterator.key();
      break;
    }
  }
	// second, search for other factories
	if( !result ) {
		FactorySetIterator iterator2(*_factory, OMBefore);
		while (++iterator2) {
			ASSERT("Properly positioned raw storage", rawStorage->position() == 0);
			if ( iterator2.value()->better()
				 && iterator2.value()->isRecognized(rawStorage)) {
				result = true;
				encoding = iterator2.key();
				break;
			}
		}
	}

  ASSERT("Properly positioned raw storage", rawStorage->position() == 0);
  return result;
}

void OMFile::initialize(void)
{
  TRACE("OMFile::initialize");
  PRECONDITION("No valid factory", _factory == 0);

  _factory = new OMFile::FactorySet();

  POSTCONDITION("Valid factory", _factory != 0);
}

void OMFile::finalize(void)
{
  TRACE("OMFile::finalize");

  delete _factory;
  _factory = 0;
  POSTCONDITION("No valid factory", _factory == 0);
}

void OMFile::registerFactory(const OMStoredObjectEncoding& encoding,
                             OMStoredObjectFactory* factory)
{
  TRACE("OMFile::registerFactory");

  PRECONDITION("Valid factory", factory != 0);
  PRECONDITION("Unique encoding", !hasFactory(encoding));
  PRECONDITION("Unique name", !hasFactory(factory->name()));

  ASSERT("Valid factory", _factory != 0);

	// if _factory already has any registered which do the same signature
	// set factory._better to point to the first one found
	OMStoredObjectFactory* better = 0;
  FactorySetIterator iterator(*_factory, OMBefore);
  while (++iterator) {
    if( iterator.value()->signature() == factory->signature() ){
      better = iterator.value();
			factory->setBetter( better );
      break;
    }
  }

  _factory->insert(encoding, factory);
  factory->initialize();
}

bool OMFile::hasFactory(const OMStoredObjectEncoding& encoding)
{
  TRACE("OMFile::hasFactory");
  bool result = false;

  if (_factory != 0) {
    OMStoredObjectFactory* f = 0;
    _factory->find(encoding, f);
    if (f != 0) {
      result = true;
    }
  }
  return result;
}

bool OMFile::hasFactory(const wchar_t* name)
{
  TRACE("OMFile::hasFactory");
  bool result = false;

  if (_factory != 0) {
    FactorySetIterator iterator(*_factory, OMBefore);
    while (++iterator) {
      if (compareWideString(iterator.value()->name(), name) == 0) {
        result = true;
        break;
      }
    }
  }
  return result;
}

OMStoredObjectFactory* OMFile::findFactory(
                                        const OMStoredObjectEncoding& encoding)
{
  TRACE("OMFile::findFactory");

  OMStoredObjectFactory* result = 0;
  ASSERT("Valid factory", _factory != 0);
  _factory->find(encoding, result);
  ASSERT("Recognized file encoding", result != 0);
  return result;
}

void OMFile::removeFactory(const OMStoredObjectEncoding& encoding)
{
  TRACE("OMFile::removeFactory");
  PRECONDITION("Valid factory", _factory != 0);
  PRECONDITION("Factory present", _factory->contains(encoding));

  OMStoredObjectFactory* factory = 0;
  _factory->find(encoding, factory);

	// if any other _factory thought this one was better, reeducate them
  FactorySetIterator iterator(*_factory, OMBefore);
  while (++iterator) {
    if( iterator.value()->signature() == factory->signature() ){
      iterator.value()->setBetter( 0 );
    }
  }

  _factory->remove(encoding);
  ASSERT("Valid factory", factory != 0);
  factory->finalize();
  delete factory;
}

void OMFile::removeAllFactories(void)
{
  TRACE("OMFile::removeAllFactories");

  ASSERT("Valid factory", _factory != 0);
  FactorySetIterator iterator(*_factory, OMBefore);
  while (++iterator) {
    OMStoredObjectFactory* factory = iterator.value();
    factory->finalize();
    delete factory;
  }
  _factory->clear();
}

OMFile::FactorySetIterator* OMFile::factories(void)
{
  TRACE("OMFile::factories");

  ASSERT("Valid factory", _factory != 0);
  FactorySetIterator* result = new FactorySetIterator(*_factory, OMBefore);
  POSTCONDITION("Valid iterator", result != 0);
  return result;
}

  // @mfunc Save all changes made to the contents of this
  //        <c OMFile>. It is not possible to save
  //        read-only or transient files.
  //   @parm Client context for callbacks.
  //   @precondition <f isOpen()>
void OMFile::saveFile(void* clientOnSaveContext)
{
  TRACE("OMFile::saveFile");
  PRECONDITION("File is open", isOpen());

  _clientOnSaveContext = clientOnSaveContext;

  _isValid = false; // failing save() leaves the file invalid
  if (isWritable()) {
    _rootStore->save(*this);
  }
  _isValid = true; // successful save() leaves the file valid
}

  // @mfunc Save the entire contents of this <c OMFile> as well as
  //        any unsaved changes in the new file <p destFile>. <p destFile>
  //        must be open, writeable and not yet contain any objects.
  //        <mf OMFile::saveAsFile> may be called
  //        for files opened in modify mode and for files opened in
  //        read-only and transient modes.
  //   @parm OMFile* | destFile | The destination file.
  //   @this const
void OMFile::saveAsFile(OMFile* ANAME(destFile)) const
{
  TRACE("OMFile::saveAsFile");

  PRECONDITION("Valid file", destFile != 0);

  ASSERT("Unimplemented code not reached", false);
}

void OMFile::saveCopyAs(OMFile* ANAME(destFile)) const
{
  TRACE("OMFile::saveCopyAs");

  PRECONDITION("Valid file", destFile != 0);

  ASSERT("Unimplemented code not reached", false);
}

  // @mfunc Discard all changes made to this <c OMFile> since the
  //        last <mf OMFile::save> or <mf OMFile::open>.
void OMFile::revert(void)
{
  TRACE("OMFile::revert");

  ASSERT("Unimplemented code not reached", false);
}

  // @mfunc Restore the client root <c OMStorable> object from this <c OMFile>.
  //   @rdesc The newly restored root <c OMStorable>.
    //   @precondition <f isOpen()>
OMStorable* OMFile::restore(void)
{
  TRACE("OMFile::restore");
  PRECONDITION("File is open", isOpen());
  PRECONDITION("Valid root", _root != 0);

  return _root->clientRoot();
}

  // @mfunc Open this <c OMFile>.
  //   @precondition <f !isOpen()>
  //   @precondition <f !isClosed()>
  //   @postcondition <f isOpen()>
void OMFile::open(void)
{
  TRACE("OMFile::open");
  PRECONDITION("Not already open", !isOpen());
  PRECONDITION("Never been opened", !isClosed());
  PRECONDITION("Valid mode", (_mode == readOnlyMode) ||
                             (_mode == writeOnlyMode) ||
                             (_mode == modifyMode));

  if (_isNew) { // new file - create
    ASSERT("Correct mode for new file", _mode != readOnlyMode);
    if (_mode == modifyMode) {
      createModify();
    } else { // _mode == writeOnly
      createWrite();
    }
  } else {      // existing file - open
    ASSERT("Correct mode for existing file", (_mode == readOnlyMode) ||
                                             (_mode == modifyMode));
    if (_mode == readOnlyMode) {
      openRead();
    } else { // _mode == modifyMode
      openModify();
    }
    ASSERT("No root object", _root == 0);
    _root = restoreRoot();
  }

  _isOpen = true;
  POSTCONDITION("Open", isOpen());
}

  // @mfunc Close this <c OMFile>, any unsaved changes are discarded.
  //   @precondition <f isOpen()>
  //   @postcondition <f !isOpen()>
  //   @postcondition <f isClosed()>
void OMFile::close(void)
{
  TRACE("OMFile::close");
  PRECONDITION("Open", isOpen());
  PRECONDITION("Valid root", _root != 0);

  _root->close();
  _rootStore->close();

  if (isValid()) {
    OMStoredObjectFactory* factory = findFactory(_encoding);
    ASSERT("Recognized file encoding", factory != 0);
    OMRawStorage* store = rawStorage();
    if (store != 0) {
      factory->close(store, isWritable());
      store->synchronize();
    } else {
      const wchar_t* name = fileName();
      factory->close(name, isWritable());
    }
  }

  // TBS - tjb delete _rootStore; _rootStore = 0;

  _root->detach();
  delete _root;
  _root = 0;
  _isOpen = false;
  _isClosed = true;
  POSTCONDITION("Closed", isClosed());
  POSTCONDITION("Closed", !isOpen());
}

  // @mfunc Is this <c OMFile> open ?
bool OMFile::isOpen(void) const
{
  TRACE("OMFile::isOpen");
  return _isOpen;
}

  // @mfunc Is this <c OMFile> closed ? Note that <f isClosed()> is not the
  //        same as !<f isOpen()> since before open() is called,
  //        <f isClosed()> is false. That is, <f isClosed()> means
  //        "was once open and is now closed".
bool OMFile::isClosed(void) const
{
  TRACE("OMFile::isClosed");
  return _isClosed;
}

bool OMFile::isValid(void) const
{
  TRACE("OMFile::isValid");
  return _isValid;
}

  // @mfunc Retrieve the client root <c OMStorable> from this <c OMFile>.
  //   @rdesc The root <c OMStorable>.
OMStorable* OMFile::clientRoot(void)
{
  TRACE("OMFile::clientRoot");
  PRECONDITION("Valid root", _root != 0);
  OMStorable* result;
  result = _root->clientRoot();
  return result;
}

OMStorable* OMFile::root(void)
{
  TRACE("OMFile::root");

  return _root;
}

OMStoredObject* OMFile::rootStore(void)
{
  TRACE("OMFile::rootStore");

  return _rootStore;
}

OMDictionary* OMFile::dictionary(void) const
{
  TRACE("OMFile::dictionary");
  // tjb - temporary
  //PRECONDITION("Valid root", _root != 0);
  //return _root->dictionary();
  return _dictionary;
}

const OMClassFactory* OMFile::classFactory(void) const
{
  TRACE("OMFile::classFactory");

  return _classFactory;
}

  // @mfunc Retrieve the <c OMPropertyTable> from this <c OMFile>.
  //   @rdesc The table of referenced properties.
OMPropertyTable* OMFile::referencedProperties(void)
{
  TRACE("OMFile::referencedProperties");

  if (_referencedProperties == 0) {
    _referencedProperties = new OMPropertyTable();
    ASSERT("Valid heap pointer", _referencedProperties != 0);
  }
  return _referencedProperties;
}

void OMFile::setReferencedProperties(OMPropertyTable* table)
{
  TRACE("OMFile::setReferencedProperties");

  PRECONDITION("Valid table", table != 0);
  PRECONDITION("No previous table", _referencedProperties == 0);

  _referencedProperties = table;
}

bool OMFile::propertyTableExists(void) const
{
  TRACE("OMFile::propertyTableExists");

  bool result;
  if (_referencedProperties != 0) {
    result = true;
  } else {
    result = false;
  }
  return result;
}

  // @mfunc The byte order of this <c OMFile>.
  //   @rdesc The byte order of this <c OMFile>.
  //   @this const
OMByteOrder OMFile::byteOrder(void) const
{
  TRACE("OMFile::byteOrder");

  ASSERT("Valid root", _rootStore != 0);
  return _rootStore->byteOrder();
}

  // @mfunc The loading mode (eager or lazy) of this <c OMFile>.
  //   @rdesc The loading mode (eager or lazy) of this <c OMFile>.
  //   @this const
OMFile::OMLoadMode OMFile::loadMode(void) const
{
  TRACE("OMFile::loadMode");

  return _loadMode;
}

void OMFile::setLoadMode(OMLoadMode mode)
{
  TRACE("OMFile::setLoadMode");

  _loadMode = mode;
}

  // @mfunc The access mode of this <c OMFile>.
  //   @rdesc The access mode of this <c OMFile>.
  //   @this const
OMFile::OMAccessMode OMFile::accessMode(void) const
{
  TRACE("OMFile::acessMode");

  return _mode;
}

  // @cmember Is it possible to read from this <c OMFile> ?
  //  @rdesc True if this <c OMFile> is readable, false otherwise.
  //   @this const
bool OMFile::isReadable(void) const
{
  TRACE("OMFile::isReadable");

  bool result;
  if ((accessMode() == readOnlyMode) || (accessMode() == modifyMode)) {
    result = true;
  } else {
    result = false;
  }
  return result;
}

  // @cmember Is it possible to write to this <c OMFile> ?
  //  @rdesc True if this <c OMFile> is writable, false otherwise.
  //   @this const
bool OMFile::isWritable(void) const
{
  TRACE("OMFile::isWritable");

  bool result;
  if ((accessMode() == writeOnlyMode) || (accessMode() == modifyMode)) {
    result = true;
  } else {
    result = false;
  }
  return result;
}

  // @mfunc The name of this <c OMFile>.
  //   @rdesc The name of this <c OMFile>.
const wchar_t* OMFile::fileName(void) const
{
  return _fileName;
}

  // @mfunc The encoding of this <c OMFile>.
  //   @rdesc The encoding of this <c OMFile>.
OMStoredObjectEncoding OMFile::encoding(void) const
{
  return _encoding;
}

  // @mfunc The raw storage on which this <c OMFile> is stored.
  //   @rdesc The raw storage on which the raw bytes of
  //          this <c OMFile> reside.
OMRawStorage* OMFile::rawStorage(void) const
{
  TRACE("OMFile::rawStorage");

  return _rawStorage;
}

OMPropertyId* OMFile::path(const wchar_t* propertyPathName) const
{
  TRACE("OMFile::path");

  wchar_t delimiter = L'/';
  PRECONDITION("Valid property path name", validWideString(propertyPathName));
  PRECONDITION("Path name is absolute", propertyPathName[0] == delimiter);
  PRECONDITION("Valid root", _root != 0);

  // Allocate result
  //
  size_t count = countWideCharacter(propertyPathName, delimiter);
  OMPropertyId* result = new OMPropertyId[count + 1];
  ASSERT("Valid heap pointer", result != 0);

  // Parse path name
  //
  wchar_t* path = saveWideString(propertyPathName);
  wchar_t* element = path;
  element++; // skip first '/'
  const OMStorable* storable = _root;
  OMProperty* property = 0;

  size_t index = 0;
  wchar_t* end = findWideCharacter(element, delimiter);

  while (end != 0) {
    *end = 0;
    property = storable->findProperty(element);
    result[index] = property->propertyId();
    index = index + 1;
    storable = storable->find(element);
    ASSERT("Valid storable pointer", storable != 0);
    element = ++end;
    end = findWideCharacter(element, delimiter);
  }

  if ((element != 0) && (lengthOfWideString(element) > 0)) {
    property = storable->findProperty(element);
    result[index] = property->propertyId();
    index = index + 1;
  }

  result[index] = 0;

  delete [] path;
  return result;
}

OMProperty* OMFile::findProperty(const OMPropertyId* path) const
{
  TRACE("OMFile::findProperty");

  PRECONDITION("Valid root", _root != 0);

  size_t count = lengthOfPropertyPath(path);
  const OMStorable* storable = _root;

  for (size_t index = 0; index < count - 1; index++) {
    OMPropertyId pid = path[index];
    storable = storable->find(pid);
    ASSERT("Valid storable pointer", storable != 0);
  }

  OMProperty* result = storable->findProperty(path[count - 1]);

  return result;
}

  // @mfunc The number of objects in this <c OMFile>.
  //   @rdesc The number of objects.
OMUInt64 OMFile::objectCount(void) const
{
  TRACE("OMFile::objectCount");
  PRECONDITION("Open", isOpen());
  PRECONDITION("Valid root", _root != 0);

  return _root->objectCount();
}

void* OMFile::clientOnSaveContext(void)
{
  return _clientOnSaveContext;
}

void* OMFile::clientOnRestoreContext(void)
{
  return _clientOnRestoreContext;
}

  // @mfunc Constructor. Create an <c OMFile> object representing
  //        an existing named external file.
  //   @parm The name of this <c OMFile>.
  //   @parm The access mode of this <c OMFile>.
  //   @parm The <c OMStoredObject> containing the root
  //         <c OMStorable> object.
  //   @parm The <c OMClassFactory> to use to restore objects from
  //         this <c OMFile>.
  //   @parm The <e OMFile::OMLoadMode> for this <c OMFile>.
OMFile::OMFile(const wchar_t* fileName,
               void* clientOnRestoreContext,
               OMStoredObjectEncoding encoding,
               const OMAccessMode mode,
               OMStoredObject* store,
               const OMClassFactory* factory,
               OMDictionary* dictionary,
               const OMLoadMode loadMode)
: _root(0),
  _rootStore(store),
  _dictionary(dictionary),
  _classFactory(factory),
  _referencedProperties(0),
  _mode(mode),
  _loadMode(loadMode),
  _fileName(0),
  _encoding(encoding),
  _clientOnSaveContext(0),
  _clientOnRestoreContext(clientOnRestoreContext),
  _rawStorage(0),
  _isOpen(false),
  _isClosed(false),
  _isNew(false),
  _isValid(true),
  _byteOrder(unspecified)
{
  TRACE("OMFile::OMFile");

  PRECONDITION("Valid file name", validWideString(fileName));
  PRECONDITION("Valid dictionary", _dictionary != 0);
  _fileName = saveWideString(fileName);
  ASSERT("No root object", _root == 0);
  _root = restoreRoot();
  _isOpen = true;
  POSTCONDITION("File is open", _isOpen);
}

  // @mfunc Constructor. Create an <c OMFile> object representing
  //        a new named external file.
  //   @parm The name of this <c OMFile>.
  //   @parm The encoding of this <c OMFile>.
  //   @parm The access mode of this <c OMFile>.
  //   @parm The <c OMStoredObject> in which to store the root
  //         <c OMStorable> object.
  //   @parm The <c OMClassFactory> to use to restore objects from
  //         this <c OMFile>.
  //   @parm The root <c OMStorable> object to save in this file.
OMFile::OMFile(const wchar_t* fileName,
               void* clientOnRestoreContext,
               OMStoredObjectEncoding encoding,
               const OMAccessMode mode,
               OMStoredObject* store,
               const OMClassFactory* factory,
               OMDictionary* dictionary,
               OMRootStorable* root)
: _root(root),
  _rootStore(store),
  _dictionary(dictionary),
  _classFactory(factory),
  _referencedProperties(0),
  _mode(mode),
  _loadMode(lazyLoad),
  _fileName(0),
  _encoding(encoding),
  _clientOnSaveContext(0),
  _clientOnRestoreContext(clientOnRestoreContext),
  _rawStorage(0),
  _isOpen(false),
  _isClosed(false),
  _isNew(true),
  _isValid(true),
  _byteOrder(unspecified)
{
  TRACE("OMFile::OMFile");

  PRECONDITION("Valid file name", validWideString(fileName));
  PRECONDITION("Valid root", _root != 0);
  PRECONDITION("Valid dictionary", _dictionary != 0);
  _fileName = saveWideString(fileName);
  _root->attach(this);
  _root->setStore(_rootStore);
  _isOpen = true;
  POSTCONDITION("File is open", _isOpen);
}

  // @mfunc Constructor. Create an <c OMFile> object representing
  //        an existing external file on the given <c OMRawStorage>.
OMFile::OMFile(OMRawStorage* rawStorage,
               void* clientOnRestoreContext,
	       OMStoredObjectEncoding encoding,
               const OMAccessMode mode,
               const OMClassFactory* factory,
               OMDictionary* dictionary,
               const OMLoadMode loadMode)
: _root(0),
  _rootStore(0),
  _dictionary(dictionary),
  _classFactory(factory),
  _referencedProperties(0),
  _mode(mode),
  _loadMode(loadMode),
  _fileName(0),
  _encoding(encoding),
  _clientOnSaveContext(0),
  _clientOnRestoreContext(clientOnRestoreContext),
  _rawStorage(rawStorage),
  _isOpen(false),
  _isClosed(false),
  _isNew(false),
  _isValid(true),
  _byteOrder(unspecified)
{
  TRACE("OMFile::OMFile");

  PRECONDITION("Valid raw storage", _rawStorage != 0);
  PRECONDITION("Consistent access modes",
                     IMPLIES(((mode == modifyMode) || (mode == writeOnlyMode)),
                     rawStorage->isWritable()));
  PRECONDITION("Valid dictionary", _dictionary != 0);

  POSTCONDITION("File not yet open", !_isOpen);
}

  // @mfunc Constructor. Create an <c OMFile> object representing
  //          a new external file on the given <c OMRawStorage>.
OMFile::OMFile(OMRawStorage* rawStorage,
               void* clientOnRestoreContext,
               OMStoredObjectEncoding encoding,
               const OMAccessMode mode,
               const OMClassFactory* factory,
               OMDictionary* dictionary,
               OMRootStorable* root,
               const OMByteOrder byteOrder)
: _root(root),
  _rootStore(0),
  _dictionary(dictionary),
  _classFactory(factory),
  _referencedProperties(0),
  _mode(mode),
  _loadMode(lazyLoad),
  _fileName(0),
  _encoding(encoding),
  _clientOnSaveContext(0),
  _clientOnRestoreContext(clientOnRestoreContext),
  _rawStorage(rawStorage),
  _isOpen(false),
  _isClosed(false),
  _isNew(true),
  _isValid(true),
  _byteOrder(byteOrder)
{
  TRACE("OMFile::OMFile");

  PRECONDITION("Valid dictionary", _dictionary != 0);
  PRECONDITION("Valid raw storage", rawStorage != 0);
  PRECONDITION("Valid root", _root != 0);
  PRECONDITION("Consistent access modes",
                     IMPLIES(((mode == modifyMode) || (mode == writeOnlyMode)),
                     rawStorage->isWritable()));

  _root->attach(this);
  POSTCONDITION("File not yet open", !_isOpen);
}

void OMFile::openRead(void)
{
  TRACE("OMFile::openRead");

	OMStoredObjectFactory* factory = 0;
	// if client said DontCare, get from isRecognized(_rawStorage, _encoding)
	if( nullOMStoredObjectEncoding == _encoding )
	{
		bool result = isRecognized(_rawStorage, _encoding);
		ASSERT("Recognized file", result);
		factory = findFactory(_encoding);
		ASSERT("Recognized file encoding", factory != 0);
	}
	else
	{
		// else get exact:  findfactory->isRecognized(_rawStorage)
		factory = findFactory(_encoding);
		ASSERT("Recognized file encoding", factory != 0);
		bool result = factory->isRecognized(_rawStorage);
		ASSERT("Recognized file", result);
	}

  _rootStore = factory->openRead(_rawStorage);
  POSTCONDITION("Valid store", _rootStore != 0);
}

void OMFile::openModify(void)
{
  TRACE("OMFile::openModify");

	OMStoredObjectFactory* factory = 0;
	// if client said DontCare, get from isRecognized(_rawStorage, _encoding)
	if( nullOMStoredObjectEncoding == _encoding )
	{
		bool result = isRecognized(_rawStorage, _encoding);
		ASSERT("Recognized file", result);
		factory = findFactory(_encoding);
		ASSERT("Recognized file encoding", factory != 0);
	}
	else
	{
		// else get exact:  findfactory->isRecognized(_rawStorage)
		factory = findFactory(_encoding);
		ASSERT("Recognized file encoding", factory != 0);
		bool result = factory->isRecognized(_rawStorage);
		ASSERT("Recognized file", result);
	}

  _rootStore = factory->openModify(_rawStorage);
  ASSERT("Valid store", _rootStore != 0);
}

void OMFile::createModify(void)
{
  TRACE("OMFile::createModify");

  OMStoredObjectFactory* factory = findFactory(_encoding);
  ASSERT("Recognized file encoding", factory != 0);
  _rootStore = factory->createModify(_rawStorage, _byteOrder);
  ASSERT("Valid root", _root != 0);
  ASSERT("Valid store", _rootStore != 0);
  _root->setStore(_rootStore);
}

void OMFile::createWrite(void)
{
  TRACE("OMFile::createWrite");

  OMStoredObjectFactory* factory = findFactory(_encoding);
  ASSERT("Recognized file encoding", factory != 0);
  _rootStore = factory->createWrite(_rawStorage, _byteOrder);
  ASSERT("Valid root", _root != 0);
  ASSERT("Valid store", _rootStore != 0);
  _root->setStore(_rootStore);
}

OMRootStorable* OMFile::restoreRoot(void)
{
  TRACE("OMFile::restoreRoot");

  OMRootStorable* root = _rootStore->restore(*this);
  return root;
}

OMFile::FactorySet* OMFile::_factory;
