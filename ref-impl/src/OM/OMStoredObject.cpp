#include "OMStoredObject.h"
#include "OMStoredPropertySetIndex.h"
#include "OMProperty.h"
#include "OMPropertySet.h"
#include "OMStoredVectorIndex.h"

#include "OMAssertions.h"
#include "OMUtilities.h"

#include <iostream.h>

#include <windows.h>

static void convert(wchar_t* wcName, size_t length, const char* name);

static void convert(char* cName, size_t length, wchar_t* name);

static void formatError(DWORD errorCode);

static int check(HRESULT resultCode);

static int checkFile(HRESULT resultCode, const char* fileName);

static int checkStream(HRESULT resultCode, const char* streamName);

static int checkStorage(HRESULT resultCode, const char* storageName);

static void printError(const char* prefix, const char* type);

static void printName(const char* name);


OMStoredObject::OMStoredObject(struct IStorage* s)
: _storage(s), _index(0), _indexStream(0), _propertiesStream(0),
  _offset(0), _classId(0), _open(false)
{
}

OMStoredObject* OMStoredObject::create(const char* fileName)
{
  TRACE("createFile");
  PRECONDITION("Valid file name", validString(fileName));

  wchar_t wcFileName[256];
  convert(wcFileName, 256, fileName);

  HRESULT result;
  IStorage* storage;

  result = StgCreateDocfile(
    wcFileName,
    STGM_DIRECT | STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE,
    0,
    &storage);
  if (!checkFile(result, fileName)) {
    exit(FAILURE);
  }

  OMStoredObject* newStoredObject = new OMStoredObject(storage);
  newStoredObject->create();

  return newStoredObject;
}

OMStoredObject* OMStoredObject::open(const char* fileName)
{
  TRACE("openFile");
  PRECONDITION("Valid file name", validString(fileName));

  wchar_t wcFileName[256];
  convert(wcFileName, 256, fileName);

  HRESULT result;
  IStorage* storage;

  result = StgOpenStorage(
    wcFileName,
    0,
    STGM_DIRECT | STGM_READWRITE | STGM_SHARE_EXCLUSIVE,
    0,
    0,
    &storage);
  if (!checkFile(result, fileName)) {
    exit(FAILURE);
  }

  OMStoredObject* newStoredObject = new OMStoredObject(storage);
  newStoredObject->open();

  return newStoredObject;
}

OMStoredObject* OMStoredObject::openStoragePath(const char* storagePathName)
{
  TRACE("OMFile::openStoragePath");
  PRECONDITION("Valid stream path name", validString(storagePathName));
  PRECONDITION("Path name is absolute", storagePathName[0] == '/');

  char* path = new char[strlen(storagePathName) + 1];
  strcpy(path, storagePathName);
  
  char* element = path;
  element++; // skip first '/'

  OMStoredObject* storage = this;
  OMStoredObject* result = 0;

  char* end = strchr(element, '/');
  
  while (end != 0) {
    *end = 0;
    storage = storage->openSubStorage(element);
    ASSERT("Valid storage pointer", storage != 0);
    element = ++end;
    end = strchr(element, '/');
  }

  if ((element != 0) && (strlen(element) > 0)) {
    result = storage->openSubStorage(element);
  } else {
    result = storage;
  }
  
  return result;
}

void OMStoredObject::save(OMProperty* p)
{
  TRACE("OMStoredObject::save(OMProperty*)");
  PRECONDITION("Already open", _open);
  PRECONDITION("Valid property", p != 0);

  p->saveTo(*this);
}

void OMStoredObject::save(OMStoredPropertySetIndex* index)
{
  TRACE("OMStoredObject::save(OMStoredPropertySetIndex*)");
  PRECONDITION("Already open", _open);
  PRECONDITION("Sorted index", index->isSorted());

  // Write version number.
  //
  int version = 1;
  writeToStream(_indexStream, &version, sizeof(version));
  
  // Write count of entries.
  //
  size_t entries = index->entries();
  writeToStream(_indexStream, &entries, sizeof(entries));
  
  // Write entries.
  //
  int pid;
  int type;
  size_t offset;
  size_t length;
  size_t context = 0;
  for (size_t i = 0; i < entries; i++) {
    index->iterate(context, pid, type, offset, length);
    writeToStream(_indexStream, &pid, sizeof(pid));
    writeToStream(_indexStream, &type, sizeof(type));
    writeToStream(_indexStream, &offset, sizeof(length));
    writeToStream(_indexStream, &length, sizeof(length));
  }
  index->clearDirty();
}

OMStoredPropertySetIndex* OMStoredObject::restore(void)
{
  TRACE("OMStoredObject::restore");
  PRECONDITION("Already open", _open);
  PRECONDITION("At start of index stream", streamOffset(_indexStream) == 0);

  // Read version number.
  //
  int version;
  readFromStream(_indexStream, &version, sizeof(version));
  ASSERT("Valid version number", version > 0);
  ASSERT("Recognized version number", version == 1);
  
  // Read count of entries.
  //
  size_t entries;
  readFromStream(_indexStream, &entries, sizeof(size_t));
  OMStoredPropertySetIndex* index = new OMStoredPropertySetIndex(entries);
  
  // Read entries.
  //
  int pid;
  int type;
  size_t offset;
  size_t length;
  for (size_t i = 0; i < entries; i++) {
    readFromStream(_indexStream, &pid, sizeof(pid));
    readFromStream(_indexStream, &type, sizeof(type));
    readFromStream(_indexStream, &offset, sizeof(offset));
    readFromStream(_indexStream, &length, sizeof(length));
    index->insert(pid, type, offset, length);
  }
  index->clearDirty();
  
  POSTCONDITION("Sorted index", index->isSorted());
  return index;
}

void OMStoredObject::restore(OMPropertySet& properties)
{
  TRACE("OMStoredObject::restore");
  PRECONDITION("Already open", _open);
  PRECONDITION("At start of properties stream",
               streamOffset(_propertiesStream) == 0);

  size_t entries = _index->entries();
  
  int pid;
  int type;
  size_t offset;
  size_t length;
  size_t context = 0;
  for (size_t i = 0; i < entries; i++) {
    _index->iterate(context, pid, type, offset, length);
    OMProperty* p = properties.get(pid);
    ASSERT("Valid property", p != 0);
    p->restoreFrom(*this, length);
  }
  
}

void OMStoredObject::create(void)
{
  TRACE("OMStoredObject::create");
  PRECONDITION("Not already open", !_open);

  _index = new OMStoredPropertySetIndex(50);
  _indexStream = createStream(_storage, "property index");
  _propertiesStream = createStream(_storage, "properties");
  _open = true;
}

void OMStoredObject::open(void)
{
  TRACE("OMStoredObject::open");
  PRECONDITION("Not already open", !_open);

  _indexStream = openStream(_storage, "property index");
  _propertiesStream = openStream(_storage, "properties");
  _open = true;
  _index = restore();
}

void OMStoredObject::close(void)
{
  TRACE("OMStoredObject::close");
  PRECONDITION("Already open", _open);

  if (_index->isDirty()) {
    save(_index);
    _index->clearDirty();
  }
  closeStream(_indexStream);
  closeStream(_propertiesStream);
  
  HRESULT resultCode = _storage->Release();
  _storage = 0;
  ASSERT("Reference count is 0.", resultCode == 0);
  _open = false;
}

void OMStoredObject::write(int pid, int type, void* start, size_t size)
{
  TRACE("OMStoredObject::write");

  _index->insert(pid, type, _offset, size);

  // Write property value.
  //
  writeToStream(_propertiesStream, start, size);
  _offset += size;
}

void OMStoredObject::read(int pid, int type, void* start, size_t size)
{
  TRACE("OMStoredObject::read");

  // Read property value.
  //
  readFromStream(_propertiesStream, start, size);
}

void OMStoredObject::saveClassId(const int cid)
{
  TRACE("OMStoredObject::saveClassId");

  _classId = cid;
  setClass(_storage, _classId);
}

int OMStoredObject::restoreClassId(void)
{
  TRACE("OMStoredObject::restoreClassId");

  getClass(_storage, _classId);
  return _classId;
}

OMStoredObject* OMStoredObject::createSubStorage(const char* name)
{
  TRACE("OMStoredObject::createSubStorage");

  IStorage* newStorage = createStorage(_storage, name);
  OMStoredObject* result = new OMStoredObject(newStorage);
  result->create();
  return result;
}

OMStoredObject* OMStoredObject::openSubStorage(const char* name)
{
  TRACE("OMStoredObject::openSubStorage");

  IStorage* newStorage = openStorage(_storage, name);
  OMStoredObject* result = new OMStoredObject(newStorage);
  result->open();
  return result;
}

void OMStoredObject::save(const OMStoredVectorIndex* index,
                          const char* vectorName)
{
  TRACE("OMStoredObject::save");

  // Calculate the stream name for the index.
  //
  char* vectorIndexName = vectorIndexStreamName(vectorName);

  // Create the stream.
  //
  IStream* vectorIndexStream = createStream(_storage, vectorIndexName);

  // Write the high water mark.
  //
  size_t highWaterMark = index->highWaterMark();
  writeToStream(vectorIndexStream, &highWaterMark, sizeof(highWaterMark));

  // Write the count of elements.
  //
  size_t entries = index->entries();
  writeToStream(vectorIndexStream, &entries, sizeof(entries));

  // Write the element names.
  //
  size_t context = 0;
  size_t name;
  for (size_t i = 0; i < entries; i++) {
    index->iterate(context, name);
    writeToStream(vectorIndexStream, &name, sizeof(name));
  }

  // Close the stream.
  //
  closeStream(vectorIndexStream);
}

OMStoredVectorIndex* OMStoredObject::restore(const char* vectorName)
{
  TRACE("OMStoredObject::restore");
  
  // Calculate the stream name for the index.
  //
  char* vectorIndexName = vectorIndexStreamName(vectorName);

  // Open the stream.
  //
  IStream* vectorIndexStream = openStream(_storage, vectorIndexName);

  // Read the high water mark.
  //
  size_t highWaterMark;
  readFromStream(vectorIndexStream, &highWaterMark, sizeof(highWaterMark));

  // Read the count of elements.
  //
  size_t entries;
  readFromStream(vectorIndexStream, &entries, sizeof(entries));

  // Create an index.
  //
  OMStoredVectorIndex* vectorIndex = new OMStoredVectorIndex(entries);

  // Read the element names, placing them in the index.
  //
  for (size_t i = 0; i < entries; i++) {
    size_t name;
    readFromStream(vectorIndexStream, &name, sizeof(name));
    vectorIndex->insert(i, name);
  }
  return vectorIndex;
}

char* OMStoredObject::vectorIndexStreamName(const char* vectorName)
{
  char* suffix = " index";
  char* vectorIndexName = new char[strlen(vectorName) + strlen(suffix) + 1];
  strcpy(vectorIndexName, vectorName);
  strcat(vectorIndexName, suffix);

  return vectorIndexName;
}

IStream* OMStoredObject::createStream(IStorage* storage,
                                      const char* streamName)
{
  TRACE("OMStoredObject::createStream");
  PRECONDITION("Valid storage", storage != 0);

  IStream* stream;
  wchar_t wcStreamName[256];
  convert(wcStreamName, 256, streamName);

  HRESULT resultCode = storage->CreateStream(
    wcStreamName,
    STGM_DIRECT | STGM_WRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE,
    0,
    0,
    &stream);
  if (!checkStream(resultCode, streamName)) {
    exit(FAILURE);
  }

  return stream;
}

IStream* OMStoredObject::openStream(IStorage* storage, const char* streamName)
{
  TRACE("OMStoredObject::openStream");
  PRECONDITION("Valid storage", storage != 0);
  PRECONDITION("Valid stream name", validString(streamName));
  
  IStream* stream;
  wchar_t wcStreamName[256];
  convert(wcStreamName, 256, streamName);
  
  HRESULT resultCode = storage->OpenStream(
    wcStreamName,
    0,
    STGM_DIRECT | STGM_READ | STGM_SHARE_EXCLUSIVE,
    0,
    &stream);
  if (!checkStream(resultCode, streamName)) {
    exit(FAILURE);
  }
  
  return stream;
  
}

void OMStoredObject::closeStream(IStream*& stream)
{
  TRACE("OMStoredObject::closeStream");
  PRECONDITION("Valid stream", stream != 0);

  HRESULT resultCode = stream->Release();
  stream = 0;
  ASSERT("Reference count is 0.", resultCode == 0);
}

IStorage* OMStoredObject::createStorage(IStorage* storage,
                                        const char* storageName)
{
  TRACE("createStorage");
  PRECONDITION("Valid storage", storage != 0);
  PRECONDITION("Valid storage name", validString(storageName));

  IStorage* newStorage;
  wchar_t wcStorageName[256];
  convert(wcStorageName, 256, storageName);

  HRESULT resultCode = storage->CreateStorage(
    wcStorageName,
    // STGM_DIRECT | STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE,
    STGM_DIRECT | STGM_READWRITE | STGM_SHARE_EXCLUSIVE,
    0,
    0,
    &newStorage);
  if (!checkStorage(resultCode, storageName)) {
    newStorage = 0;
  }

  return newStorage;
}

IStorage* OMStoredObject::openStorage(IStorage* storage,
                                      const char* storageName)
{
  TRACE("openStorage");
  PRECONDITION("Valid storage", storage != 0);
  PRECONDITION("Valid storage name", validString(storageName));

  IStorage* newStorage;
  wchar_t wcStorageName[256];
  convert(wcStorageName, 256, storageName);

  HRESULT resultCode = storage->OpenStorage(
    wcStorageName,
    0,
    STGM_DIRECT | STGM_READWRITE | STGM_SHARE_EXCLUSIVE,
    0,
    0,
    &newStorage);
  if (!checkStorage(resultCode, storageName)) {
    exit(FAILURE);
  }

  return newStorage;
}

void OMStoredObject::closeStorage(IStorage*& storage)
{
  TRACE("closeStorage");
  PRECONDITION("Valid storage", storage != 0);

  HRESULT resultCode = storage->Release();
  storage = 0;
  ASSERT("Reference count is 0.", resultCode == 0);
}

void OMStoredObject::writeToStream(IStream* stream, void* data, size_t size)
{
  TRACE("OMStoredObject::writeToStream");
  PRECONDITION("Valid stream", stream != 0);
  PRECONDITION("Valid data", data != 0);
  PRECONDITION("Valid size", size > 0);

  HRESULT resultCode = stream->Write(data, size, NULL);
  if (!check(resultCode)) {
    exit(FAILURE);
  }
}

void OMStoredObject::readFromStream(IStream* stream, void* data, size_t size)
{
  TRACE("OMStoredObject::readFromStream");

  unsigned long bytesRead;
  HRESULT result = stream->Read(data, size, &bytesRead);
  if (!check(result)) {
    exit(FAILURE);
  }
  ASSERT("Successful read", bytesRead == size);
}

void OMStoredObject::setClass(IStorage* storage, int cid)
{
  TRACE("OMStoredObject::setClass");
  PRECONDITION("Valid storage", storage != 0);

  struct _GUID g = {0,0,0, {0, 0, 0, 0, 0, 0, 0, 0}};
  g.Data4[7] = cid;
  HRESULT resultCode = storage->SetClass(g);
  if (!check(resultCode)) {
    exit(FAILURE);
  }

}

void OMStoredObject::getClass(IStorage* storage, int& cid)
{
  TRACE("OMStoredObject::getClass");

  STATSTG statstg;
  HRESULT result = storage->Stat(&statstg, STATFLAG_NONAME);
  if (!check(result)) {
    exit(FAILURE);
  }
  cid = statstg.clsid.Data4[7];
}

size_t OMStoredObject::streamOffset(IStream* stream)
{
  TRACE("OMStoredObject::streamOffset");

  size_t result;
  LARGE_INTEGER zero = {0, 0};
  ULARGE_INTEGER position;
  HRESULT status = stream->Seek(zero, STREAM_SEEK_CUR, &position);
  if (!check(status)) {
  }
#if defined(__sgi)
  ASSERT("Small stream", position.u.HighPart == 0);
  result = position.u.LowPart;
#else
  ASSERT("Small stream", position.HighPart == 0);
  result = position.LowPart;
#endif
  return result;
}

void OMStoredObject::streamSeek(IStream* stream, size_t offset)
{
  TRACE("OMStoredObject::streamSeek");

  LARGE_INTEGER newPosition = {0, offset};
  ULARGE_INTEGER oldPosition;
  HRESULT status = stream->Seek(newPosition, STREAM_SEEK_SET, &oldPosition);
  if (!check(status)) {
  }
#if defined(__sgi)
  ASSERT("Small stream", oldPosition.u.HighPart == 0);
#else
  ASSERT("Small stream", oldPosition.HighPart == 0);
#endif
}

static void convert(wchar_t* wcName, size_t length, const char* name)
{
  TRACE("convert");
  PRECONDITION("Valid input name", validString(name));
  PRECONDITION("Valid output buffer", wcName != 0);
  PRECONDITION("Valid output buffer size", length > 0);
  
  ASSERT("Valid program name", validString(getProgramName()));

  size_t status = mbstowcs(wcName, name, length);
  if (status == (size_t)-1) {
    cerr << getProgramName()
      << "Error : Failed to convert \""
      << name
      << "\" to a wide character string."
      << endl;
    exit(FAILURE);  
  }
}

static void convert(char* cName, size_t length, wchar_t* name)
{
  ASSERT("Valid program name", validString(getProgramName()));

  size_t status = wcstombs(cName, name, length);
  if (status == (size_t)-1) {
    cerr << getProgramName()
      << ": Error : Conversion failed."
      << endl;
    exit(FAILURE);  
  }
}

static void formatError(DWORD errorCode)
{
  wchar_t buffer[256];

  int status = FormatMessage(
    FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
    NULL,
    errorCode,
    LANG_SYSTEM_DEFAULT,
    buffer, 256,
    NULL);

  char message[256];

  convert(message, 256, buffer);

  if (status != 0) {
    int length = strlen(message);
    // zap cr/lf
    if (length >= 2) {
      message[length - 2] = '\0';
    }
    cerr << message << endl;
  } else {
    cerr << "Error code = " << hex << errorCode << dec << endl;
  }

}

static int check(HRESULT resultCode)
{
  TRACE("check");

  ASSERT("Valid program name", validString(getProgramName()));

  if (FAILED(resultCode)) {
    printError(getProgramName(), "Error");
    formatError(resultCode);
    return FALSE;
  } else {
    return TRUE;
  }
}

static int checkFile(HRESULT resultCode, const char* fileName)
{
  TRACE("checkFile");
  PRECONDITION("Valid file name", validString(fileName));

  ASSERT("Valid program name", validString(getProgramName()));

  if (FAILED(resultCode)) {
    printError(getProgramName(), "File error");
    printName(fileName);
    formatError(resultCode);
    return FALSE;
  } else {
    return TRUE;
  }
}

static int checkStream(HRESULT resultCode, const char* streamName)
{
  TRACE("checkStream");
  PRECONDITION("Valid stream name", validString(streamName));

  ASSERT("Valid program name", validString(getProgramName()));

  if (FAILED(resultCode)) {
    printError(getProgramName(), "Stream error");
    printName(streamName);
    formatError(resultCode);
    return FALSE;
  } else {
    return TRUE;
  }

}

static int checkStorage(HRESULT resultCode, const char* storageName)
{
  TRACE("checkStorage");
  PRECONDITION("Valid storage name", validString(storageName));

  ASSERT("Valid program name", validString(getProgramName()));

  if (FAILED(resultCode)) {
    printError(getProgramName(), "Storage error");
    printName(storageName);
    formatError(resultCode);
    return FALSE;
  } else {
    return TRUE;
  }

}

static void printError(const char* prefix, const char* type)
{
  cerr << prefix << ": " << type << ": ";
}

static void printName(const char* name)
{
  cerr << "\"" << name << "\": ";
}
