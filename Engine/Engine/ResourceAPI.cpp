//
// Urho3D Engine
// Copyright (c) 2008-2011 Lasse ��rni
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#include "Precompiled.h"
#include "APITemplates.h"
#include "ResourceCache.h"

void ArrayToVariantVector(CScriptArray* arr, VariantVector& dest);

void RegisterResource(asIScriptEngine* engine)
{
    RegisterResource<Resource>(engine, "Resource");
}

static Resource* ResourceCacheGetResource(const String& type, const String& name, ResourceCache* ptr)
{
    return ptr->GetResource(ShortStringHash(type), name);
}

static File* ResourceCacheGetFile(const String& name, ResourceCache* ptr)
{
    SharedPtr<File> file = ptr->GetFile(name);
    // The shared pointer will go out of scope, so have to increment the reference count
    // (here an auto handle can not be used)
    if (file)
        file->AddRef();
    return file.Ptr();
}

static void ResourceCacheReleaseResource(const String& type, const String& name, bool force, ResourceCache* ptr)
{
    ptr->ReleaseResource(ShortStringHash(type), name, force);
}

static void ResourceCacheReleaseResources(const String& type, bool force, ResourceCache* ptr)
{
    ptr->ReleaseResources(ShortStringHash(type), force);
}

static void ResourceCacheReleaseResourcesPartial(const String& type, const String& partialName, bool force, ResourceCache* ptr)
{
    ptr->ReleaseResources(ShortStringHash(type), partialName, force);
}

static void ResourceCacheSetMemoryBudget(const String& type, unsigned budget, ResourceCache* ptr)
{
    ptr->SetMemoryBudget(ShortStringHash(type), budget);
}

static unsigned ResourceCacheGetMemoryBudget(const String& type, ResourceCache* ptr)
{
    return ptr->GetMemoryBudget(ShortStringHash(type));
}

static unsigned ResourceCacheGetMemoryUse(const String& type, ResourceCache* ptr)
{
    return ptr->GetMemoryUse(ShortStringHash(type));
}

static ResourceCache* GetResourceCache()
{
    return GetScriptContext()->GetSubsystem<ResourceCache>();
}

static void RegisterResourceCache(asIScriptEngine* engine)
{
    RegisterObject<ResourceCache>(engine, "ResourceCache");
    engine->RegisterObjectMethod("ResourceCache", "bool AddResourcePath(const String&in)", asMETHOD(ResourceCache, AddResourcePath), asCALL_THISCALL);
    engine->RegisterObjectMethod("ResourceCache", "void AddPackageFile(PackageFile@+)", asMETHOD(ResourceCache, AddPackageFile), asCALL_THISCALL);
    engine->RegisterObjectMethod("ResourceCache", "bool AddManualResource(Resource@+)", asMETHOD(ResourceCache, AddManualResource), asCALL_THISCALL);
    engine->RegisterObjectMethod("ResourceCache", "void RemoveResourcePath(const String&in)", asMETHOD(ResourceCache, RemoveResourcePath), asCALL_THISCALL);
    engine->RegisterObjectMethod("ResourceCache", "void RemovePackageFile(PackageFile@+, bool, bool)", asMETHODPR(ResourceCache, RemovePackageFile, (PackageFile*, bool, bool), void), asCALL_THISCALL);
    engine->RegisterObjectMethod("ResourceCache", "void RemovePackageFile(const String&in, bool, bool)", asMETHODPR(ResourceCache, RemovePackageFile, (const String&, bool, bool), void), asCALL_THISCALL);
    engine->RegisterObjectMethod("ResourceCache", "void ReleaseResource(const String&in, const String&in, bool)", asFUNCTION(ResourceCacheReleaseResource), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("ResourceCache", "void ReleaseResources(const String&in, bool)", asFUNCTION(ResourceCacheReleaseResources), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("ResourceCache", "void ReleaseResources(const String&in, const String&in, bool)", asFUNCTION(ResourceCacheReleaseResourcesPartial), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("ResourceCache", "void ReleaseAllResources(bool)", asMETHOD(ResourceCache, ReleaseAllResources), asCALL_THISCALL);
    engine->RegisterObjectMethod("ResourceCache", "bool ReloadResource(Resource@+)", asMETHOD(ResourceCache, ReloadResource), asCALL_THISCALL);
    engine->RegisterObjectMethod("ResourceCache", "bool Exists(const String&in) const", asMETHODPR(ResourceCache, Exists, (const String&) const, bool), asCALL_THISCALL);
    engine->RegisterObjectMethod("ResourceCache", "File@ GetFile(const String&in)", asFUNCTION(ResourceCacheGetFile), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("ResourceCache", "String GetPreferredResourcePath(const String&in)", asMETHOD(ResourceCache, GetPreferredResourcePath), asCALL_THISCALL);
    engine->RegisterObjectMethod("ResourceCache", "Resource@+ GetResource(const String&in, const String&in)", asFUNCTION(ResourceCacheGetResource), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("ResourceCache", "void set_memoryBudget(const String&in, uint)", asFUNCTION(ResourceCacheSetMemoryBudget), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("ResourceCache", "uint get_memoryBudget(const String&in) const", asFUNCTION(ResourceCacheGetMemoryBudget), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("ResourceCache", "uint get_memoryUse(const String&in) const", asFUNCTION(ResourceCacheGetMemoryUse), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("ResourceCache", "uint get_totalMemoryUse() const", asMETHOD(ResourceCache, GetTotalMemoryUse), asCALL_THISCALL);
    engine->RegisterGlobalFunction("ResourceCache@+ get_resourceCache()", asFUNCTION(GetResourceCache), asCALL_CDECL);
    engine->RegisterGlobalFunction("ResourceCache@+ get_cache()", asFUNCTION(GetResourceCache), asCALL_CDECL);
}

static void RegisterImage(asIScriptEngine* engine)
{
    RegisterResource<Image>(engine, "Image");
    engine->RegisterObjectMethod("Image", "void SaveBMP(const String&in)", asMETHOD(Image, SaveBMP), asCALL_THISCALL);
    engine->RegisterObjectMethod("Image", "void SaveTGA(const String&in)", asMETHOD(Image, SaveTGA), asCALL_THISCALL);
    engine->RegisterObjectMethod("Image", "int get_width() const", asMETHOD(Image, GetWidth), asCALL_THISCALL);
    engine->RegisterObjectMethod("Image", "int get_height() const", asMETHOD(Image, GetHeight), asCALL_THISCALL);
    engine->RegisterObjectMethod("Image", "uint get_components() const", asMETHOD(Image, GetComponents), asCALL_THISCALL);
    engine->RegisterObjectMethod("Image", "bool get_compressed() const", asMETHOD(Image, IsCompressed), asCALL_THISCALL);
}

static void ConstructXMLElement(XMLElement* ptr)
{
    new(ptr) XMLElement();
}

static void ConstructXMLElementCopy(const XMLElement& element, XMLElement* ptr)
{
    new(ptr) XMLElement(element);
}

static void DestructXMLElement(XMLElement* ptr)
{
    ptr->~XMLElement();
}

static void XMLElementRemoveChildDefault(XMLElement* ptr)
{
    ptr->RemoveChild();
}

static void XMLElementRemoveChildrenDefault(XMLElement* ptr)
{
    ptr->RemoveChildren();
}

static void XMLElementSetVariantVector(CScriptArray* value, XMLElement* ptr)
{
    VariantVector src;
    ArrayToVariantVector(value, src);
    ptr->SetVariantVector(src);
}

static XMLElement XMLElementGetChildDefault(XMLElement* ptr)
{
    return ptr->GetChild();
}

static XMLElement XMLElementGetNextDefault(XMLElement* ptr)
{
    return ptr->GetNext();
}

static CScriptArray* XMLElementGetAttributeNames(XMLElement* ptr)
{
    return VectorToArray<String>(ptr->GetAttributeNames(), "Array<String>");
}

static CScriptArray* XMLElementGetVariantVector(XMLElement* ptr)
{
    return VectorToArray<Variant>(ptr->GetVariantVector(), "Array<Variant>");
}

static void RegisterXMLElement(asIScriptEngine* engine)
{
    RegisterResource<XMLFile>(engine, "XMLFile");
    engine->RegisterObjectType("XMLElement", sizeof(XMLElement), asOBJ_VALUE | asOBJ_APP_CLASS_CDA);
    engine->RegisterObjectBehaviour("XMLElement", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(ConstructXMLElement), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectBehaviour("XMLElement", asBEHAVE_CONSTRUCT, "void f(const XMLElement&in)", asFUNCTION(ConstructXMLElementCopy), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectBehaviour("XMLElement", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(DestructXMLElement), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("XMLElement", "XMLElement &opAssign(const XMLElement&in)", asMETHOD(XMLElement, operator =), asCALL_THISCALL);
    engine->RegisterObjectMethod("XMLElement", "XMLElement CreateChild(const String&in)", asMETHOD(XMLElement, CreateChild), asCALL_THISCALL);
    engine->RegisterObjectMethod("XMLElement", "bool RemoveChild()", asFUNCTION(XMLElementRemoveChildDefault), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("XMLElement", "bool RemoveChild(const String&in, bool)", asMETHOD(XMLElement, RemoveChild), asCALL_THISCALL);
    engine->RegisterObjectMethod("XMLElement", "bool RemoveChildren()", asFUNCTION(XMLElementRemoveChildrenDefault), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("XMLElement", "bool RemoveChildren(const String&in)", asMETHOD(XMLElement,RemoveChildren), asCALL_THISCALL);
    engine->RegisterObjectMethod("XMLElement", "bool SetAttribute(const String&in, const String&in)", asMETHOD(XMLElement, SetAttribute), asCALL_THISCALL);
    engine->RegisterObjectMethod("XMLElement", "bool SetBool(const String&in, bool)", asMETHOD(XMLElement, SetBool), asCALL_THISCALL);
    engine->RegisterObjectMethod("XMLElement", "bool SetBoundingBox(const BoundingBox&in)", asMETHOD(XMLElement, SetBoundingBox), asCALL_THISCALL);
    engine->RegisterObjectMethod("XMLElement", "bool SetColor(const String&in, const Color&in)", asMETHOD(XMLElement, SetColor), asCALL_THISCALL);
    engine->RegisterObjectMethod("XMLElement", "bool SetFloat(const String&in, float)", asMETHOD(XMLElement, SetFloat), asCALL_THISCALL);
    engine->RegisterObjectMethod("XMLElement", "bool SetInt(const String&in, int)", asMETHOD(XMLElement, SetInt), asCALL_THISCALL);
    engine->RegisterObjectMethod("XMLElement", "bool SetQuaternion(const String&in, const Quaternion&in)", asMETHOD(XMLElement, SetQuaternion), asCALL_THISCALL);
    engine->RegisterObjectMethod("XMLElement", "bool SetString(const String&in, const String&in)", asMETHOD(XMLElement, SetString), asCALL_THISCALL);
    engine->RegisterObjectMethod("XMLElement", "bool SetVariant(const Variant&in)", asMETHOD(XMLElement, SetVariant), asCALL_THISCALL);
    engine->RegisterObjectMethod("XMLElement", "bool SetResourceRef(const String&in, const ResourceRef&in)", asMETHOD(XMLElement, SetResourceRef), asCALL_THISCALL);
    engine->RegisterObjectMethod("XMLElement", "bool SetResourceRefList(const String&in, const ResourceRefList&in)", asMETHOD(XMLElement, SetResourceRefList), asCALL_THISCALL);
    engine->RegisterObjectMethod("XMLElement", "bool SetVariantVector(Array<Variant>@+)", asFUNCTION(XMLElementSetVariantVector), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("XMLElement", "bool SetVariantMap(const VariantMap&in)", asMETHOD(XMLElement, SetVariantMap), asCALL_THISCALL);
    engine->RegisterObjectMethod("XMLElement", "bool SetVector2(const String&in, const Vector2&in)", asMETHOD(XMLElement, SetVector2), asCALL_THISCALL);
    engine->RegisterObjectMethod("XMLElement", "bool SetVector3(const String&in, const Vector3&in)", asMETHOD(XMLElement, SetVector3), asCALL_THISCALL);
    engine->RegisterObjectMethod("XMLElement", "bool SetVector4(const String&in, const Vector3&in)", asMETHOD(XMLElement, SetVector4), asCALL_THISCALL);
    engine->RegisterObjectMethod("XMLElement", "uint GetNumAttributes() const", asMETHOD(XMLElement, GetNumAttributes), asCALL_THISCALL);
    engine->RegisterObjectMethod("XMLElement", "bool HasAttribute(const String&in) const", asMETHOD(XMLElement, HasAttribute), asCALL_THISCALL);
    engine->RegisterObjectMethod("XMLElement", "String GetAttributes(const String&in) const", asMETHOD(XMLElement, GetAttribute), asCALL_THISCALL);
    engine->RegisterObjectMethod("XMLElement", "Array<String>@ GetAttributeNames() const", asFUNCTION(XMLElementGetAttributeNames), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("XMLElement", "bool HasChild(const String&in) const", asMETHOD(XMLElement, HasChild), asCALL_THISCALL);
    engine->RegisterObjectMethod("XMLElement", "XMLElement GetChild() const", asFUNCTION(XMLElementGetChildDefault), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("XMLElement", "XMLElement GetChild(const String&in) const", asMETHOD(XMLElement, GetChild), asCALL_THISCALL);
    engine->RegisterObjectMethod("XMLElement", "XMLElement GetNext() const", asFUNCTION(XMLElementGetNextDefault), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("XMLElement", "XMLElement GetNext(const String&in) const", asMETHOD(XMLElement, GetNext), asCALL_THISCALL);
    engine->RegisterObjectMethod("XMLElement", "bool GetBool(const String&in) const", asMETHOD(XMLElement, GetBool), asCALL_THISCALL);
    engine->RegisterObjectMethod("XMLElement", "BoundingBox GetBoundingBox() const", asMETHOD(XMLElement, GetBoundingBox), asCALL_THISCALL);
    engine->RegisterObjectMethod("XMLElement", "Color GetColor(const String&in) const", asMETHOD(XMLElement, GetColor), asCALL_THISCALL);
    engine->RegisterObjectMethod("XMLElement", "float GetFloat(const String&in) const", asMETHOD(XMLElement, GetFloat), asCALL_THISCALL);
    engine->RegisterObjectMethod("XMLElement", "int GetInt(const String&in) const", asMETHOD(XMLElement, GetInt), asCALL_THISCALL);
    engine->RegisterObjectMethod("XMLElement", "Quaternion GetQuaternion(const String&in) const", asMETHOD(XMLElement, GetQuaternion), asCALL_THISCALL);
    engine->RegisterObjectMethod("XMLElement", "String GetString(const String&in) const", asMETHOD(XMLElement, GetString), asCALL_THISCALL);
    engine->RegisterObjectMethod("XMLElement", "String GetStringLower(const String&in) const", asMETHOD(XMLElement, GetStringLower), asCALL_THISCALL);
    engine->RegisterObjectMethod("XMLElement", "String GetStringUpper(const String&in) const", asMETHOD(XMLElement, GetStringUpper), asCALL_THISCALL);
    engine->RegisterObjectMethod("XMLElement", "Variant GetVariant() const", asMETHOD(XMLElement, GetVariant), asCALL_THISCALL);
    engine->RegisterObjectMethod("XMLElement", "ResourceRef GetResourceRef() const", asMETHOD(XMLElement, GetResourceRef), asCALL_THISCALL);
    engine->RegisterObjectMethod("XMLElement", "ResourceRefList GetResourceRefList() const", asMETHOD(XMLElement, GetResourceRefList), asCALL_THISCALL);
    engine->RegisterObjectMethod("XMLElement", "Array<Variant>@ GetVariantVector() const", asFUNCTION(XMLElementGetVariantVector), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("XMLElement", "VariantMap GetVariantMap() const", asMETHOD(XMLElement, GetVariantMap), asCALL_THISCALL);
    engine->RegisterObjectMethod("XMLElement", "Vector2 GetVector2(const String&in) const", asMETHOD(XMLElement, GetVector2), asCALL_THISCALL);
    engine->RegisterObjectMethod("XMLElement", "Vector3 GetVector3(const String&in) const", asMETHOD(XMLElement, GetVector3), asCALL_THISCALL);
    engine->RegisterObjectMethod("XMLElement", "Vector4 GetVector4(const String&in) const", asMETHOD(XMLElement, GetVector4), asCALL_THISCALL);
    engine->RegisterObjectMethod("XMLElement", "String get_name() const", asMETHOD(XMLElement, GetName), asCALL_THISCALL);
    engine->RegisterObjectMethod("XMLElement", "String get_text() const", asMETHOD(XMLElement, GetText), asCALL_THISCALL);
    engine->RegisterObjectMethod("XMLElement", "bool get_null() const", asMETHOD(XMLElement, IsNull), asCALL_THISCALL);
    engine->RegisterObjectMethod("XMLElement", "bool get_notNull() const", asMETHOD(XMLElement, NotNull), asCALL_THISCALL);
    engine->RegisterObjectMethod("XMLElement", "XMLElement get_parent() const", asMETHOD(XMLElement, GetParent), asCALL_THISCALL);
    engine->RegisterObjectMethod("XMLElement", "XMLFile@+ get_file() const", asMETHOD(XMLElement, GetFile), asCALL_THISCALL);
}

static XMLElement XMLFileGetRootDefault(XMLFile* ptr)
{
    return ptr->GetRoot();
}

static void RegisterXMLFile(asIScriptEngine* engine)
{
    engine->RegisterObjectMethod("XMLFile", "XMLElement CreateRoot(const String&in)", asMETHOD(XMLFile, CreateRoot), asCALL_THISCALL);
    engine->RegisterObjectMethod("XMLFile", "XMLElement GetRoot(const String&in)", asMETHOD(XMLFile, GetRoot), asCALL_THISCALL);
    engine->RegisterObjectMethod("XMLFile", "XMLElement get_root()", asFUNCTION(XMLFileGetRootDefault), asCALL_CDECL_OBJLAST);
}

void RegisterResourceAPI(asIScriptEngine* engine)
{
    RegisterResource(engine);
    RegisterResourceCache(engine);
    RegisterImage(engine);
    RegisterXMLElement(engine);
    RegisterXMLFile(engine);
}
