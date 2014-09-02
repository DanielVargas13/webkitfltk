/*
    This file is part of the WebKit open source project.
    This file has been generated by generate-bindings.pl. DO NOT MODIFY!

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "config.h"
#include "JSTestNondeterministic.h"

#include "ExceptionCode.h"
#include "JSDOMBinding.h"
#include "ScriptExecutionContext.h"
#include "TestNondeterministic.h"
#include "URL.h"
#include <runtime/Error.h>
#include <runtime/JSString.h>
#include <wtf/GetPtr.h>

#if ENABLE(WEB_REPLAY)
#include "MemoizedDOMResult.h"
#include <replay/InputCursor.h>
#include <wtf/NeverDestroyed.h>
#endif

using namespace JSC;

namespace WebCore {

// Functions

JSC::EncodedJSValue JSC_HOST_CALL jsTestNondeterministicPrototypeFunctionNondeterministicZeroArgFunction(JSC::ExecState*);

// Attributes

JSC::EncodedJSValue jsTestNondeterministicNondeterministicReadonlyAttr(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
JSC::EncodedJSValue jsTestNondeterministicNondeterministicWriteableAttr(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
void setJSTestNondeterministicNondeterministicWriteableAttr(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::EncodedJSValue);
JSC::EncodedJSValue jsTestNondeterministicNondeterministicExceptionAttr(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
void setJSTestNondeterministicNondeterministicExceptionAttr(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::EncodedJSValue);
JSC::EncodedJSValue jsTestNondeterministicNondeterministicGetterExceptionAttr(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
void setJSTestNondeterministicNondeterministicGetterExceptionAttr(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::EncodedJSValue);
JSC::EncodedJSValue jsTestNondeterministicNondeterministicSetterExceptionAttr(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
void setJSTestNondeterministicNondeterministicSetterExceptionAttr(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::EncodedJSValue);
JSC::EncodedJSValue jsTestNondeterministicConstructor(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);

class JSTestNondeterministicPrototype : public JSC::JSNonFinalObject {
public:
    typedef JSC::JSNonFinalObject Base;
    static JSTestNondeterministicPrototype* create(JSC::VM& vm, JSC::JSGlobalObject* globalObject, JSC::Structure* structure)
    {
        JSTestNondeterministicPrototype* ptr = new (NotNull, JSC::allocateCell<JSTestNondeterministicPrototype>(vm.heap)) JSTestNondeterministicPrototype(vm, globalObject, structure);
        ptr->finishCreation(vm);
        return ptr;
    }

    DECLARE_INFO;
    static JSC::Structure* createStructure(JSC::VM& vm, JSC::JSGlobalObject* globalObject, JSC::JSValue prototype)
    {
        return JSC::Structure::create(vm, globalObject, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), info());
    }

private:
    JSTestNondeterministicPrototype(JSC::VM& vm, JSC::JSGlobalObject*, JSC::Structure* structure)
        : JSC::JSNonFinalObject(vm, structure)
    {
    }

    void finishCreation(JSC::VM&);
};

class JSTestNondeterministicConstructor : public DOMConstructorObject {
private:
    JSTestNondeterministicConstructor(JSC::Structure*, JSDOMGlobalObject*);
    void finishCreation(JSC::VM&, JSDOMGlobalObject*);

public:
    typedef DOMConstructorObject Base;
    static JSTestNondeterministicConstructor* create(JSC::VM& vm, JSC::Structure* structure, JSDOMGlobalObject* globalObject)
    {
        JSTestNondeterministicConstructor* ptr = new (NotNull, JSC::allocateCell<JSTestNondeterministicConstructor>(vm.heap)) JSTestNondeterministicConstructor(structure, globalObject);
        ptr->finishCreation(vm, globalObject);
        return ptr;
    }

    DECLARE_INFO;
    static JSC::Structure* createStructure(JSC::VM& vm, JSC::JSGlobalObject* globalObject, JSC::JSValue prototype)
    {
        return JSC::Structure::create(vm, globalObject, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), info());
    }
};

const ClassInfo JSTestNondeterministicConstructor::s_info = { "TestNondeterministicConstructor", &Base::s_info, 0, CREATE_METHOD_TABLE(JSTestNondeterministicConstructor) };

JSTestNondeterministicConstructor::JSTestNondeterministicConstructor(Structure* structure, JSDOMGlobalObject* globalObject)
    : DOMConstructorObject(structure, globalObject)
{
}

void JSTestNondeterministicConstructor::finishCreation(VM& vm, JSDOMGlobalObject* globalObject)
{
    Base::finishCreation(vm);
    ASSERT(inherits(info()));
    putDirect(vm, vm.propertyNames->prototype, JSTestNondeterministic::getPrototype(vm, globalObject), DontDelete | ReadOnly);
    putDirect(vm, vm.propertyNames->length, jsNumber(0), ReadOnly | DontDelete | DontEnum);
}

/* Hash table for prototype */

static const HashTableValue JSTestNondeterministicPrototypeTableValues[] =
{
    { "constructor", DontEnum | ReadOnly, NoIntrinsic, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsTestNondeterministicConstructor), (intptr_t) static_cast<PutPropertySlot::PutValueFunc>(0) },
    { "nondeterministicReadonlyAttr", DontDelete | ReadOnly | CustomAccessor, NoIntrinsic, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsTestNondeterministicNondeterministicReadonlyAttr), (intptr_t) static_cast<PutPropertySlot::PutValueFunc>(0) },
    { "nondeterministicWriteableAttr", DontDelete | CustomAccessor, NoIntrinsic, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsTestNondeterministicNondeterministicWriteableAttr), (intptr_t) static_cast<PutPropertySlot::PutValueFunc>(setJSTestNondeterministicNondeterministicWriteableAttr) },
    { "nondeterministicExceptionAttr", DontDelete | CustomAccessor, NoIntrinsic, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsTestNondeterministicNondeterministicExceptionAttr), (intptr_t) static_cast<PutPropertySlot::PutValueFunc>(setJSTestNondeterministicNondeterministicExceptionAttr) },
    { "nondeterministicGetterExceptionAttr", DontDelete | CustomAccessor, NoIntrinsic, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsTestNondeterministicNondeterministicGetterExceptionAttr), (intptr_t) static_cast<PutPropertySlot::PutValueFunc>(setJSTestNondeterministicNondeterministicGetterExceptionAttr) },
    { "nondeterministicSetterExceptionAttr", DontDelete | CustomAccessor, NoIntrinsic, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsTestNondeterministicNondeterministicSetterExceptionAttr), (intptr_t) static_cast<PutPropertySlot::PutValueFunc>(setJSTestNondeterministicNondeterministicSetterExceptionAttr) },
    { "nondeterministicZeroArgFunction", JSC::Function, NoIntrinsic, (intptr_t)static_cast<NativeFunction>(jsTestNondeterministicPrototypeFunctionNondeterministicZeroArgFunction), (intptr_t) (0) },
};

WEBCORE_EXPORT const ClassInfo JSTestNondeterministicPrototype::s_info = { "TestNondeterministicPrototype", &Base::s_info, 0, CREATE_METHOD_TABLE(JSTestNondeterministicPrototype) };

void JSTestNondeterministicPrototype::finishCreation(VM& vm)
{
    Base::finishCreation(vm);
    reifyStaticProperties(vm, JSTestNondeterministicPrototypeTableValues, *this);
}

WEBCORE_EXPORT const ClassInfo JSTestNondeterministic::s_info = { "TestNondeterministic", &Base::s_info, 0, CREATE_METHOD_TABLE(JSTestNondeterministic) };

JSTestNondeterministic::JSTestNondeterministic(Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<TestNondeterministic> impl)
    : JSDOMWrapper(structure, globalObject)
    , m_impl(impl.leakRef())
{
}

JSObject* JSTestNondeterministic::createPrototype(VM& vm, JSGlobalObject* globalObject)
{
    return JSTestNondeterministicPrototype::create(vm, globalObject, JSTestNondeterministicPrototype::createStructure(vm, globalObject, globalObject->objectPrototype()));
}

JSObject* JSTestNondeterministic::getPrototype(VM& vm, JSGlobalObject* globalObject)
{
    return getDOMPrototype<JSTestNondeterministic>(vm, globalObject);
}

void JSTestNondeterministic::destroy(JSC::JSCell* cell)
{
    JSTestNondeterministic* thisObject = static_cast<JSTestNondeterministic*>(cell);
    thisObject->JSTestNondeterministic::~JSTestNondeterministic();
}

JSTestNondeterministic::~JSTestNondeterministic()
{
    releaseImplIfNotNull();
}

EncodedJSValue jsTestNondeterministicNondeterministicReadonlyAttr(ExecState* exec, JSObject* slotBase, EncodedJSValue thisValue, PropertyName)
{
    UNUSED_PARAM(exec);
    UNUSED_PARAM(slotBase);
    UNUSED_PARAM(thisValue);
    JSTestNondeterministic* castedThis = jsDynamicCast<JSTestNondeterministic*>(JSValue::decode(thisValue));
    if (UNLIKELY(!castedThis)) {
        if (jsDynamicCast<JSTestNondeterministicPrototype*>(slotBase))
            return reportDeprecatedGetterError(*exec, "TestNondeterministic", "nondeterministicReadonlyAttr");
        return throwGetterTypeError(*exec, "TestNondeterministic", "nondeterministicReadonlyAttr");
    }
#if ENABLE(WEB_REPLAY)
    JSGlobalObject* globalObject = exec->lexicalGlobalObject();
    InputCursor& cursor = globalObject->inputCursor();
    static NeverDestroyed<const AtomicString> bindingName("TestNondeterministic.nondeterministicReadonlyAttr", AtomicString::ConstructFromLiteral);
    if (cursor.isCapturing()) {
        int memoizedResult = castedThis->impl().nondeterministicReadonlyAttr();
        cursor.appendInput<MemoizedDOMResult<int>>(bindingName.get().string(), memoizedResult, 0);
        JSValue result = jsNumber(memoizedResult);
        return JSValue::encode(result);
    }

    if (cursor.isReplaying()) {
        int memoizedResult;
        MemoizedDOMResultBase* input = cursor.fetchInput<MemoizedDOMResultBase>();
        if (input && input->convertTo<int>(memoizedResult)) {
            JSValue result = jsNumber(memoizedResult);
            return JSValue::encode(result);
        }
    }
#endif
    TestNondeterministic& impl = castedThis->impl();
    JSValue result = jsNumber(impl.nondeterministicReadonlyAttr());
    return JSValue::encode(result);
}


EncodedJSValue jsTestNondeterministicNondeterministicWriteableAttr(ExecState* exec, JSObject* slotBase, EncodedJSValue thisValue, PropertyName)
{
    UNUSED_PARAM(exec);
    UNUSED_PARAM(slotBase);
    UNUSED_PARAM(thisValue);
    JSTestNondeterministic* castedThis = jsDynamicCast<JSTestNondeterministic*>(JSValue::decode(thisValue));
    if (UNLIKELY(!castedThis)) {
        if (jsDynamicCast<JSTestNondeterministicPrototype*>(slotBase))
            return reportDeprecatedGetterError(*exec, "TestNondeterministic", "nondeterministicWriteableAttr");
        return throwGetterTypeError(*exec, "TestNondeterministic", "nondeterministicWriteableAttr");
    }
#if ENABLE(WEB_REPLAY)
    JSGlobalObject* globalObject = exec->lexicalGlobalObject();
    InputCursor& cursor = globalObject->inputCursor();
    static NeverDestroyed<const AtomicString> bindingName("TestNondeterministic.nondeterministicWriteableAttr", AtomicString::ConstructFromLiteral);
    if (cursor.isCapturing()) {
        String memoizedResult = castedThis->impl().nondeterministicWriteableAttr();
        cursor.appendInput<MemoizedDOMResult<String>>(bindingName.get().string(), memoizedResult, 0);
        JSValue result = jsStringWithCache(exec, memoizedResult);
        return JSValue::encode(result);
    }

    if (cursor.isReplaying()) {
        String memoizedResult;
        MemoizedDOMResultBase* input = cursor.fetchInput<MemoizedDOMResultBase>();
        if (input && input->convertTo<String>(memoizedResult)) {
            JSValue result = jsStringWithCache(exec, memoizedResult);
            return JSValue::encode(result);
        }
    }
#endif
    TestNondeterministic& impl = castedThis->impl();
    JSValue result = jsStringWithCache(exec, impl.nondeterministicWriteableAttr());
    return JSValue::encode(result);
}


EncodedJSValue jsTestNondeterministicNondeterministicExceptionAttr(ExecState* exec, JSObject* slotBase, EncodedJSValue thisValue, PropertyName)
{
    UNUSED_PARAM(exec);
    UNUSED_PARAM(slotBase);
    UNUSED_PARAM(thisValue);
    JSTestNondeterministic* castedThis = jsDynamicCast<JSTestNondeterministic*>(JSValue::decode(thisValue));
    if (UNLIKELY(!castedThis)) {
        if (jsDynamicCast<JSTestNondeterministicPrototype*>(slotBase))
            return reportDeprecatedGetterError(*exec, "TestNondeterministic", "nondeterministicExceptionAttr");
        return throwGetterTypeError(*exec, "TestNondeterministic", "nondeterministicExceptionAttr");
    }
#if ENABLE(WEB_REPLAY)
    JSGlobalObject* globalObject = exec->lexicalGlobalObject();
    InputCursor& cursor = globalObject->inputCursor();
    static NeverDestroyed<const AtomicString> bindingName("TestNondeterministic.nondeterministicExceptionAttr", AtomicString::ConstructFromLiteral);
    if (cursor.isCapturing()) {
        String memoizedResult = castedThis->impl().nondeterministicExceptionAttr();
        cursor.appendInput<MemoizedDOMResult<String>>(bindingName.get().string(), memoizedResult, 0);
        JSValue result = jsStringWithCache(exec, memoizedResult);
        return JSValue::encode(result);
    }

    if (cursor.isReplaying()) {
        String memoizedResult;
        MemoizedDOMResultBase* input = cursor.fetchInput<MemoizedDOMResultBase>();
        if (input && input->convertTo<String>(memoizedResult)) {
            JSValue result = jsStringWithCache(exec, memoizedResult);
            return JSValue::encode(result);
        }
    }
#endif
    TestNondeterministic& impl = castedThis->impl();
    JSValue result = jsStringWithCache(exec, impl.nondeterministicExceptionAttr());
    return JSValue::encode(result);
}


EncodedJSValue jsTestNondeterministicNondeterministicGetterExceptionAttr(ExecState* exec, JSObject* slotBase, EncodedJSValue thisValue, PropertyName)
{
    UNUSED_PARAM(exec);
    UNUSED_PARAM(slotBase);
    UNUSED_PARAM(thisValue);
    JSTestNondeterministic* castedThis = jsDynamicCast<JSTestNondeterministic*>(JSValue::decode(thisValue));
    if (UNLIKELY(!castedThis)) {
        if (jsDynamicCast<JSTestNondeterministicPrototype*>(slotBase))
            return reportDeprecatedGetterError(*exec, "TestNondeterministic", "nondeterministicGetterExceptionAttr");
        return throwGetterTypeError(*exec, "TestNondeterministic", "nondeterministicGetterExceptionAttr");
    }
    ExceptionCode ec = 0;
#if ENABLE(WEB_REPLAY)
    JSGlobalObject* globalObject = exec->lexicalGlobalObject();
    InputCursor& cursor = globalObject->inputCursor();
    static NeverDestroyed<const AtomicString> bindingName("TestNondeterministic.nondeterministicGetterExceptionAttr", AtomicString::ConstructFromLiteral);
    if (cursor.isCapturing()) {
        String memoizedResult = castedThis->impl().nondeterministicGetterExceptionAttr(ec);
        cursor.appendInput<MemoizedDOMResult<String>>(bindingName.get().string(), memoizedResult, ec);
        JSValue result = jsStringWithCache(exec, memoizedResult);
        setDOMException(exec, ec);
        return JSValue::encode(result);
    }

    if (cursor.isReplaying()) {
        String memoizedResult;
        MemoizedDOMResultBase* input = cursor.fetchInput<MemoizedDOMResultBase>();
        if (input && input->convertTo<String>(memoizedResult)) {
            JSValue result = jsStringWithCache(exec, memoizedResult);
            setDOMException(exec, input->exceptionCode());
            return JSValue::encode(result);
        }
    }
#endif
    TestNondeterministic& impl = castedThis->impl();
    JSValue result = jsStringWithCache(exec, impl.nondeterministicGetterExceptionAttr(ec));
    setDOMException(exec, ec);
    return JSValue::encode(result);
}


EncodedJSValue jsTestNondeterministicNondeterministicSetterExceptionAttr(ExecState* exec, JSObject* slotBase, EncodedJSValue thisValue, PropertyName)
{
    UNUSED_PARAM(exec);
    UNUSED_PARAM(slotBase);
    UNUSED_PARAM(thisValue);
    JSTestNondeterministic* castedThis = jsDynamicCast<JSTestNondeterministic*>(JSValue::decode(thisValue));
    if (UNLIKELY(!castedThis)) {
        if (jsDynamicCast<JSTestNondeterministicPrototype*>(slotBase))
            return reportDeprecatedGetterError(*exec, "TestNondeterministic", "nondeterministicSetterExceptionAttr");
        return throwGetterTypeError(*exec, "TestNondeterministic", "nondeterministicSetterExceptionAttr");
    }
#if ENABLE(WEB_REPLAY)
    JSGlobalObject* globalObject = exec->lexicalGlobalObject();
    InputCursor& cursor = globalObject->inputCursor();
    static NeverDestroyed<const AtomicString> bindingName("TestNondeterministic.nondeterministicSetterExceptionAttr", AtomicString::ConstructFromLiteral);
    if (cursor.isCapturing()) {
        String memoizedResult = castedThis->impl().nondeterministicSetterExceptionAttr();
        cursor.appendInput<MemoizedDOMResult<String>>(bindingName.get().string(), memoizedResult, 0);
        JSValue result = jsStringWithCache(exec, memoizedResult);
        return JSValue::encode(result);
    }

    if (cursor.isReplaying()) {
        String memoizedResult;
        MemoizedDOMResultBase* input = cursor.fetchInput<MemoizedDOMResultBase>();
        if (input && input->convertTo<String>(memoizedResult)) {
            JSValue result = jsStringWithCache(exec, memoizedResult);
            return JSValue::encode(result);
        }
    }
#endif
    TestNondeterministic& impl = castedThis->impl();
    JSValue result = jsStringWithCache(exec, impl.nondeterministicSetterExceptionAttr());
    return JSValue::encode(result);
}


EncodedJSValue jsTestNondeterministicConstructor(ExecState* exec, JSObject* baseValue, EncodedJSValue, PropertyName)
{
    JSTestNondeterministicPrototype* domObject = jsDynamicCast<JSTestNondeterministicPrototype*>(baseValue);
    if (!domObject)
        return throwVMTypeError(exec);
    return JSValue::encode(JSTestNondeterministic::getConstructor(exec->vm(), domObject->globalObject()));
}

void setJSTestNondeterministicNondeterministicWriteableAttr(ExecState* exec, JSObject* baseObject, EncodedJSValue thisValue, EncodedJSValue encodedValue)
{
    JSValue value = JSValue::decode(encodedValue);
    UNUSED_PARAM(baseObject);
    JSTestNondeterministic* castedThis = jsDynamicCast<JSTestNondeterministic*>(JSValue::decode(thisValue));
    if (UNLIKELY(!castedThis)) {
        if (jsDynamicCast<JSTestNondeterministicPrototype*>(JSValue::decode(thisValue)))
            reportDeprecatedSetterError(*exec, "TestNondeterministic", "nondeterministicWriteableAttr");
        else
            throwSetterTypeError(*exec, "TestNondeterministic", "nondeterministicWriteableAttr");
        return;
    }
    TestNondeterministic& impl = castedThis->impl();
    const String& nativeValue(value.isEmpty() ? String() : value.toString(exec)->value(exec));
    if (UNLIKELY(exec->hadException()))
        return;
    impl.setNondeterministicWriteableAttr(nativeValue);
}


void setJSTestNondeterministicNondeterministicExceptionAttr(ExecState* exec, JSObject* baseObject, EncodedJSValue thisValue, EncodedJSValue encodedValue)
{
    JSValue value = JSValue::decode(encodedValue);
    UNUSED_PARAM(baseObject);
    JSTestNondeterministic* castedThis = jsDynamicCast<JSTestNondeterministic*>(JSValue::decode(thisValue));
    if (UNLIKELY(!castedThis)) {
        if (jsDynamicCast<JSTestNondeterministicPrototype*>(JSValue::decode(thisValue)))
            reportDeprecatedSetterError(*exec, "TestNondeterministic", "nondeterministicExceptionAttr");
        else
            throwSetterTypeError(*exec, "TestNondeterministic", "nondeterministicExceptionAttr");
        return;
    }
    TestNondeterministic& impl = castedThis->impl();
    const String& nativeValue(value.isEmpty() ? String() : value.toString(exec)->value(exec));
    if (UNLIKELY(exec->hadException()))
        return;
    impl.setNondeterministicExceptionAttr(nativeValue);
}


void setJSTestNondeterministicNondeterministicGetterExceptionAttr(ExecState* exec, JSObject* baseObject, EncodedJSValue thisValue, EncodedJSValue encodedValue)
{
    JSValue value = JSValue::decode(encodedValue);
    UNUSED_PARAM(baseObject);
    JSTestNondeterministic* castedThis = jsDynamicCast<JSTestNondeterministic*>(JSValue::decode(thisValue));
    if (UNLIKELY(!castedThis)) {
        if (jsDynamicCast<JSTestNondeterministicPrototype*>(JSValue::decode(thisValue)))
            reportDeprecatedSetterError(*exec, "TestNondeterministic", "nondeterministicGetterExceptionAttr");
        else
            throwSetterTypeError(*exec, "TestNondeterministic", "nondeterministicGetterExceptionAttr");
        return;
    }
    TestNondeterministic& impl = castedThis->impl();
    const String& nativeValue(value.isEmpty() ? String() : value.toString(exec)->value(exec));
    if (UNLIKELY(exec->hadException()))
        return;
    impl.setNondeterministicGetterExceptionAttr(nativeValue);
}


void setJSTestNondeterministicNondeterministicSetterExceptionAttr(ExecState* exec, JSObject* baseObject, EncodedJSValue thisValue, EncodedJSValue encodedValue)
{
    JSValue value = JSValue::decode(encodedValue);
    UNUSED_PARAM(baseObject);
    JSTestNondeterministic* castedThis = jsDynamicCast<JSTestNondeterministic*>(JSValue::decode(thisValue));
    if (UNLIKELY(!castedThis)) {
        if (jsDynamicCast<JSTestNondeterministicPrototype*>(JSValue::decode(thisValue)))
            reportDeprecatedSetterError(*exec, "TestNondeterministic", "nondeterministicSetterExceptionAttr");
        else
            throwSetterTypeError(*exec, "TestNondeterministic", "nondeterministicSetterExceptionAttr");
        return;
    }
    TestNondeterministic& impl = castedThis->impl();
    ExceptionCode ec = 0;
    const String& nativeValue(value.isEmpty() ? String() : value.toString(exec)->value(exec));
    if (UNLIKELY(exec->hadException()))
        return;
    impl.setNondeterministicSetterExceptionAttr(nativeValue, ec);
    setDOMException(exec, ec);
}


JSValue JSTestNondeterministic::getConstructor(VM& vm, JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSTestNondeterministicConstructor>(vm, jsCast<JSDOMGlobalObject*>(globalObject));
}

EncodedJSValue JSC_HOST_CALL jsTestNondeterministicPrototypeFunctionNondeterministicZeroArgFunction(ExecState* exec)
{
    JSValue thisValue = exec->thisValue();
    JSTestNondeterministic* castedThis = jsDynamicCast<JSTestNondeterministic*>(thisValue);
    if (UNLIKELY(!castedThis))
        return throwThisTypeError(*exec, "TestNondeterministic", "nondeterministicZeroArgFunction");
    ASSERT_GC_OBJECT_INHERITS(castedThis, JSTestNondeterministic::info());
    TestNondeterministic& impl = castedThis->impl();
    JSValue result;
#if ENABLE(WEB_REPLAY)
    InputCursor& cursor = exec->lexicalGlobalObject()->inputCursor();
    static NeverDestroyed<const AtomicString> bindingName("TestNondeterministic.nondeterministicZeroArgFunction", AtomicString::ConstructFromLiteral);
    if (cursor.isCapturing()) {
        bool memoizedResult = impl.nondeterministicZeroArgFunction();
        cursor.appendInput<MemoizedDOMResult<bool>>(bindingName.get().string(), memoizedResult, 0);
        result = jsBoolean(memoizedResult);
    } else if (cursor.isReplaying()) {
        MemoizedDOMResultBase* input = cursor.fetchInput<MemoizedDOMResultBase>();
        bool memoizedResult;
        if (input && input->convertTo<bool>(memoizedResult)) {
            result = jsBoolean(memoizedResult);
        } else
            result = jsBoolean(impl.nondeterministicZeroArgFunction());
    } else
        result = jsBoolean(impl.nondeterministicZeroArgFunction());
#else
    result = jsBoolean(impl.nondeterministicZeroArgFunction());
#endif
    return JSValue::encode(result);
}

bool JSTestNondeterministicOwner::isReachableFromOpaqueRoots(JSC::Handle<JSC::Unknown> handle, void*, SlotVisitor& visitor)
{
    UNUSED_PARAM(handle);
    UNUSED_PARAM(visitor);
    return false;
}

void JSTestNondeterministicOwner::finalize(JSC::Handle<JSC::Unknown> handle, void* context)
{
    JSTestNondeterministic* jsTestNondeterministic = jsCast<JSTestNondeterministic*>(handle.slot()->asCell());
    DOMWrapperWorld& world = *static_cast<DOMWrapperWorld*>(context);
    uncacheWrapper(world, &jsTestNondeterministic->impl(), jsTestNondeterministic);
    jsTestNondeterministic->releaseImpl();
}

#if ENABLE(BINDING_INTEGRITY)
#if PLATFORM(WIN)
#pragma warning(disable: 4483)
extern "C" { extern void (*const __identifier("??_7TestNondeterministic@WebCore@@6B@")[])(); }
#else
extern "C" { extern void* _ZTVN7WebCore20TestNondeterministicE[]; }
#endif
#endif
JSC::JSValue toJS(JSC::ExecState*, JSDOMGlobalObject* globalObject, TestNondeterministic* impl)
{
    if (!impl)
        return jsNull();
    if (JSValue result = getExistingWrapper<JSTestNondeterministic>(globalObject, impl))
        return result;

#if ENABLE(BINDING_INTEGRITY)
    void* actualVTablePointer = *(reinterpret_cast<void**>(impl));
#if PLATFORM(WIN)
    void* expectedVTablePointer = reinterpret_cast<void*>(__identifier("??_7TestNondeterministic@WebCore@@6B@"));
#else
    void* expectedVTablePointer = &_ZTVN7WebCore20TestNondeterministicE[2];
#if COMPILER(CLANG)
    // If this fails TestNondeterministic does not have a vtable, so you need to add the
    // ImplementationLacksVTable attribute to the interface definition
    COMPILE_ASSERT(__is_polymorphic(TestNondeterministic), TestNondeterministic_is_not_polymorphic);
#endif
#endif
    // If you hit this assertion you either have a use after free bug, or
    // TestNondeterministic has subclasses. If TestNondeterministic has subclasses that get passed
    // to toJS() we currently require TestNondeterministic you to opt out of binding hardening
    // by adding the SkipVTableValidation attribute to the interface IDL definition
    RELEASE_ASSERT(actualVTablePointer == expectedVTablePointer);
#endif
    return createNewWrapper<JSTestNondeterministic>(globalObject, impl);
}

TestNondeterministic* toTestNondeterministic(JSC::JSValue value)
{
    if (auto* wrapper = jsDynamicCast<JSTestNondeterministic*>(value))
        return &wrapper->impl();
    return nullptr;
}

}
