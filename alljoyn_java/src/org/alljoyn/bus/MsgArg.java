/*
 *  * Copyright (c) Open Connectivity Foundation (OCF) and AllJoyn Open
 *    Source Project (AJOSP) Contributors and others.
 *
 *    SPDX-License-Identifier: Apache-2.0
 *
 *    All rights reserved. This program and the accompanying materials are
 *    made available under the terms of the Apache License, Version 2.0
 *    which accompanies this distribution, and is available at
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Copyright (c) Open Connectivity Foundation and Contributors to AllSeen
 *    Alliance. All rights reserved.
 *
 *    Permission to use, copy, modify, and/or distribute this software for
 *    any purpose with or without fee is hereby granted, provided that the
 *    above copyright notice and this permission notice appear in all
 *    copies.
 *
 *     THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 *     WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 *     WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 *     AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 *     DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 *     PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 *     TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 *     PERFORMANCE OF THIS SOFTWARE.
 */

package org.alljoyn.bus;

import java.lang.reflect.Array;
import java.lang.reflect.Field;
import java.lang.reflect.GenericArrayType;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.lang.reflect.ParameterizedType;
import java.lang.reflect.Type;
import java.util.AbstractMap;
import java.util.HashMap;
import java.util.Map;

/**
 * MsgArg provides methods for marshalling from Java types to native types and
 * unmarshalling from native types to Java types.  The methods use a {@code
 * long} as the handle to the native type (representing a {@code MsgArg} in
 * native code).  No unnecessary Java objects are created.
 */
public final class MsgArg {

    /*
     * The AllJoyn data type IDs.
     */
    public static final int ALLJOYN_INVALID          =  0;
    public static final int ALLJOYN_ARRAY            = 'a';
    public static final int ALLJOYN_BOOLEAN          = 'b';
    public static final int ALLJOYN_DOUBLE           = 'd';
    public static final int ALLJOYN_DICT_ENTRY       = 'e';
    public static final int ALLJOYN_SIGNATURE        = 'g';
    public static final int ALLJOYN_INT32            = 'i';
    public static final int ALLJOYN_INT16            = 'n';
    public static final int ALLJOYN_OBJECT_PATH      = 'o';
    public static final int ALLJOYN_UINT16           = 'q';
    public static final int ALLJOYN_STRUCT           = 'r';
    public static final int ALLJOYN_STRING           = 's';
    public static final int ALLJOYN_UINT64           = 't';
    public static final int ALLJOYN_UINT32           = 'u';
    public static final int ALLJOYN_VARIANT          = 'v';
    public static final int ALLJOYN_INT64            = 'x';
    public static final int ALLJOYN_BYTE             = 'y';
    public static final int ALLJOYN_HANDLE           = 'h';

    public static final int ALLJOYN_STRUCT_OPEN      = '(';
    public static final int ALLJOYN_STRUCT_CLOSE     = ')';
    public static final int ALLJOYN_DICT_ENTRY_OPEN  = '{';
    public static final int ALLJOYN_DICT_ENTRY_CLOSE = '}';

    private static final int ALLJOYN_BOOLEAN_ARRAY    = ('b' << 8) | 'a';
    private static final int ALLJOYN_DOUBLE_ARRAY     = ('d' << 8) | 'a';
    private static final int ALLJOYN_INT32_ARRAY      = ('i' << 8) | 'a';
    private static final int ALLJOYN_INT16_ARRAY      = ('n' << 8) | 'a';
    private static final int ALLJOYN_UINT16_ARRAY     = ('q' << 8) | 'a';
    private static final int ALLJOYN_UINT64_ARRAY     = ('t' << 8) | 'a';
    private static final int ALLJOYN_UINT32_ARRAY     = ('u' << 8) | 'a';
    private static final int ALLJOYN_INT64_ARRAY      = ('x' << 8) | 'a';
    private static final int ALLJOYN_BYTE_ARRAY       = ('y' << 8) | 'a';

    private MsgArg() {}

    /**
     * Marshals an {@code int} value into a Java {@code Enum}.
     *
     * @param type the type of the {@code Enum}
     * @param value the ordinal value of the enum
     * @return an {@code Enum} object of type {@code type} if the marshalling
     *         succeeds.  Otherwise {@code null} is returned.
     * @throws BusException if {@code type} is an {@code Enum}, but the enum
     *                      object corresponding to the ordinal value cannot be
     *                      determined
     */
    private static Enum<?> getEnumObject(Type type, int value) throws BusException {
        if (type instanceof Class) {
            Class<?> c = (Class<?>) type;
            if (c.isEnum()) {
                try {
                    Object[] values = (Object[]) c.getMethod("values").invoke(null);
                    return (Enum<?>) values[value];
                } catch (ArrayIndexOutOfBoundsException ex) {
                    throw new BusException("failed to get " + c + " for value " + value, ex);
                } catch (NoSuchMethodException ex) {
                    throw new BusException("failed to get " + c + " for value " + value, ex);
                } catch (IllegalAccessException ex) {
                    throw new BusException("failed to get " + c + " for value " + value, ex);
                } catch (InvocationTargetException ex) {
                    throw new BusException("failed to get " + c + " for value " + value, ex);
                }
            }
        }
        return null;
    }

    /**
     * Gets the ordinal value of the {@code Enum} object.
     *
     * @param obj the {@code Enum} object
     * @return the ordinal value as an {@code int}
     * @throws BusException if {@code obj} is an {@code Enum}, but the ordinal
     *                      value cannot be determined
     */
    private static int getEnumValue(Object obj) throws BusException {
        if (obj != null) {
            Class<?> c = obj.getClass();
            if (c.isEnum()) {
                try {
                    Object[] values = (Object[]) c.getMethod("values").invoke(null);
                    for (int i = 0; i < values.length; ++i) {
                        if (values[i].equals(obj)) {
                            return i;
                        }
                    }
                    throw new BusException("failed to get ordinal value for " + obj);
                } catch (NoSuchMethodException ex) {
                    throw new BusException("failed to get ordinal value for " + obj, ex);
                } catch (IllegalAccessException ex) {
                    throw new BusException("failed to get ordinal value for " + obj, ex);
                } catch (InvocationTargetException ex) {
                    throw new BusException("failed to get ordinal value for " + obj, ex);
                }
            }
        }
        return -1;
    }

    /*
     * Accessor functions for talking to the native MsgArgs.  The msgArg
     * parameter is a native (MsgArg *).
     */
    public static native int getTypeId(long msgArg);
    public static native byte getByte(long msgArg);
    public static native short getInt16(long msgArg);
    public static native short getUint16(long msgArg);
    public static native boolean getBool(long msgArg);
    public static native int getUint32(long msgArg);
    public static native int getInt32(long msgArg);
    public static native long getInt64(long msgArg);
    public static native long getUint64(long msgArg);
    public static native double getDouble(long msgArg);
    public static native String getString(long msgArg);
    public static native String getObjPath(long msgArg);
    public static native String getSignature(long msgArg);
    public static native byte[] getByteArray(long msgArg);
    public static native short[] getInt16Array(long msgArg);
    public static native short[] getUint16Array(long msgArg);
    public static native boolean[] getBoolArray(long msgArg);
    public static native int[] getUint32Array(long msgArg);
    public static native int[] getInt32Array(long msgArg);
    public static native long[] getInt64Array(long msgArg);
    public static native long[] getUint64Array(long msgArg);
    public static native double[] getDoubleArray(long msgArg);
    public static native long getKey(long msgArg);
    public static native long getVal(long msgArg);
    public static native int getNumElements(long msgArg);
    public static native long getElement(long msgArg, int index);
    public static native String getElemSig(long msgArg);
    public static native int getNumMembers(long msgArg);
    public static native long getMember(long msgArg, int index);

    /*
     * Accessor functions for setting native MsgArgs.  The msgArg
     * parameter is a native (MsgArg *).
     */
    public static native long set(long msgArg, String signature, byte arg) throws BusException;
    public static native long set(long msgArg, String signature, boolean arg) throws BusException;
    public static native long set(long msgArg, String signature, short arg) throws BusException;
    public static native long set(long msgArg, String signature, int arg) throws BusException;
    public static native long set(long msgArg, String signature, long arg) throws BusException;
    public static native long set(long msgArg, String signature, double arg) throws BusException;
    public static native long set(long msgArg, String signature, String arg) throws BusException;
    public static native long set(long msgArg, String signature, byte[] arg) throws BusException;
    public static native long set(long msgArg, String signature, boolean[] arg) throws BusException;
    public static native long set(long msgArg, String signature, short[] arg) throws BusException;
    public static native long set(long msgArg, String signature, int[] arg) throws BusException;
    public static native long set(long msgArg, String signature, long[] arg) throws BusException;
    public static native long set(long msgArg, String signature, double[] arg) throws BusException;
    public static native long setArray(long msgArg, String elemSig, int numElements) throws BusException;
    public static native long setStruct(long msgArg, int numMembers) throws BusException;
    public static native long setDictEntry(long msgArg) throws BusException;
    public static native long setVariant(long msgArg, String signature, long val) throws BusException;
    public static native long setVariant(long msgArg) throws BusException;

    public static long set(long msgArg, String signature, Boolean[] arg) throws BusException {
        boolean[] ab = new boolean[arg.length];
        for (int i = 0; i < arg.length; ++i) {
            ab[i] = arg[i];
        }
        return set(msgArg, signature, ab);
    }

    /**
     * Returns a string representation of the signature of an array of message
     * args.
     *
     * @param msgArgs an array of message arg values
     * @return the signature string for the message args
     */
    public static native String getSignature(long[] msgArgs);

    /**
     * Unmarshals a native MsgArg into a Java object.
     *
     * @param msgArg the native MsgArg pointer
     * @param type the Java type to unmarshal into
     * @return the unmarshalled Java object
     * @throws MarshalBusException if the unmarshalling fails
     */
    @SuppressWarnings("unchecked")
    public static Object unmarshal(long msgArg, Type type) throws MarshalBusException {
        // Verify msgArg is valid
        if (msgArg == 0) {
            throw new MarshalBusException("cannot marshal invalid MsgArg into " + type);
        }

        try {
            Object object;
            switch (getTypeId(msgArg)) {
            case ALLJOYN_ARRAY:
                /*
                 * If the type parameter is an instance of Object (i.e. generic/unspecified),
                 * then recompute the actual type based on the msgArg signature.
                 */
                if (type == Object.class) {
                    type = toType(getSignature(new long[]{msgArg}));
                }
                if (getElemSig(msgArg).charAt(0) == ALLJOYN_DICT_ENTRY_OPEN) {
                    Type rawType = ((ParameterizedType) type).getRawType();
                    rawType = (rawType == Map.class) ? HashMap.class : rawType;
                    object = ((Class<?>) rawType).newInstance();
                    for (int i = 0; i < getNumElements(msgArg); ++i) {
                        long element  = getElement(msgArg, i);
                        Type[] typeArgs = ((ParameterizedType) type).getActualTypeArguments();
                        // TODO Can't seem to get it to suppress the warning here...
                        ((Map<Object, Object>) object).put(unmarshal(getKey(element), typeArgs[0]),
                                                           unmarshal(getVal(element), typeArgs[1]));
                    }
                    return object;
                } else {
                    Type componentType = (type instanceof GenericArrayType)
                        ? ((GenericArrayType) type).getGenericComponentType()
                        : ((Class<?>) type).getComponentType();
                    Class<?> componentClass;
                    if (componentType instanceof ParameterizedType) {
                        Type rawType = ((ParameterizedType) componentType).getRawType();
                        rawType = (rawType == Map.class) ? HashMap.class : rawType;
                        componentClass = (Class<?>) rawType;
                    } else {
                        componentClass = (Class<?>) componentType;
                    }
                    object = Array.newInstance(componentClass, getNumElements(msgArg));
                    for (int i = 0; i < getNumElements(msgArg); ++i) {
                        /*
                         * Under Sun the Array.set() is sufficient to check the
                         * type.  Under Android that is not the case.
                         */
                        Object component = unmarshal(getElement(msgArg, i), componentType);
                        if (!componentClass.isInstance(component)) {
                            throw new IllegalArgumentException("argument type mismatch");
                        }
                        Array.set(object, i, component);
                    }
                    return object;
                }
            case ALLJOYN_BOOLEAN:
                return getBool(msgArg);
            case ALLJOYN_BOOLEAN_ARRAY:
                boolean[] b = getBoolArray(msgArg);
                Class<?> componentClass = ((Class<?>) type).getComponentType();
                if (componentClass == Boolean.class) {
                    Boolean[] B = new Boolean[b.length];
                    for (int i = 0; i < b.length; ++i) {
                        B[i] = b[i];
                    }
                    return B;
                }
                return b;
            case ALLJOYN_BYTE:
                object = getEnumObject(type, (int) getByte(msgArg));
                if (object == null) {
                    return getByte(msgArg);
                }
                return object;
            case ALLJOYN_BYTE_ARRAY:
                return getByteArray(msgArg);
            case ALLJOYN_DOUBLE:
                return getDouble(msgArg);
            case ALLJOYN_DOUBLE_ARRAY:
                return getDoubleArray(msgArg);
            case ALLJOYN_INT16:
                object = getEnumObject(type, (int) getInt16(msgArg));
                if (object == null) {
                    return getInt16(msgArg);
                }
                return object;
            case ALLJOYN_INT16_ARRAY:
                return getInt16Array(msgArg);
            case ALLJOYN_INT32:
                object = getEnumObject(type, getInt32(msgArg));
                if (object == null) {
                    return getInt32(msgArg);
                }
                return object;
            case ALLJOYN_INT32_ARRAY:
                return getInt32Array(msgArg);
            case ALLJOYN_INT64:
                object = getEnumObject(type, (int) getInt64(msgArg));
                if (object == null) {
                    return getInt64(msgArg);
                }
                return object;
            case ALLJOYN_INT64_ARRAY:
                return getInt64Array(msgArg);
            case ALLJOYN_OBJECT_PATH:
                return getObjPath(msgArg);
            case ALLJOYN_SIGNATURE:
                return getSignature(msgArg);
            case ALLJOYN_STRING:
                return getString(msgArg);
            case ALLJOYN_STRUCT:
                /*
                 * If the type parameter is an instance of Object or Object[] (i.e. generic/unspecified),
                 * there is no implementation class on which to do reflection, so Type[] and Field[]
                 * values can't be obtained. Instead, determine structure based on the msgArg signature.
                 */
                if (type == Object.class || type == Object[].class) {
                    String signature = getSignature(new long[] {msgArg});
                    String[] sigs = Signature.split(signature.substring(1,signature.length()-1));
                    if (sigs.length != getNumMembers(msgArg)) {
                        throw new MarshalBusException(
                            "cannot marshal '" + signature + "' with "
                            + getNumMembers(msgArg) + " members into " + type + " with "
                            + sigs.length + " fields");
                    }
                    Object[] array = (Object[]) Array.newInstance(Object.class, sigs.length);
                    for (int i = 0; i < getNumMembers(msgArg); ++i) {
                        array[i] = unmarshal(getMember(msgArg, i), toType(sigs[i]));
                    }
                    return array; // returned structure represented as a generic Object[]
                } else {
                    Type[] types = Signature.structTypes((Class<?>) type);
                    if (types.length != getNumMembers(msgArg)) {
                        throw new MarshalBusException(
                            "cannot marshal '" + getSignature(new long[] { msgArg }) + "' with "
                            + getNumMembers(msgArg) + " members into " + type + " with "
                            + types.length + " fields");
                    }
                    object = ((Class<?>) type).newInstance();
                    Field[] fields = Signature.structFields((Class<?>) type);
                    for (int i = 0; i < getNumMembers(msgArg); ++i) {
                        Object value = unmarshal(getMember(msgArg, i), types[i]);
                        fields[i].set(object, value);
                    }
                    return object;
                }
            case ALLJOYN_UINT16:
                object = getEnumObject(type, (int) getUint16(msgArg));
                if (object == null) {
                    return getUint16(msgArg);
                }
                return object;
            case ALLJOYN_UINT16_ARRAY:
                return getUint16Array(msgArg);
            case ALLJOYN_UINT32:
                object = getEnumObject(type, getUint32(msgArg));
                if (object == null) {
                    return getUint32(msgArg);
                }
                return object;
            case ALLJOYN_UINT32_ARRAY:
                return getUint32Array(msgArg);
            case ALLJOYN_UINT64:
                object = getEnumObject(type, (int) getUint64(msgArg));
                if (object == null) {
                    return getUint64(msgArg);
                }
                return object;
            case ALLJOYN_UINT64_ARRAY:
                return getUint64Array(msgArg);
            case ALLJOYN_VARIANT:
                Variant variant = new Variant();
                variant.setMsgArg(msgArg);
                return variant;
            default:
                throw new MarshalBusException("unimplemented '"
                                              + getSignature(new long[] { msgArg }) + "'");
            }
        } catch (Throwable th) {
            throw new MarshalBusException("cannot marshal '" + getSignature(new long[] { msgArg })
                                          + "' into " + type, th);
        }
    }

    /**
     * Unmarshals an array of native MsgArgs into an array of Java objects.  The
     * MsgArgs are unmarshalled into the parameter types of the specified Java
     * method.
     *
     * @param method the method specifying the parameter types to unmarshal into
     * @param msgArgs an ALLJOYN_STRUCT containing the native MsgArgs as members
     * @return the unmarshalled Java objects
     * @throws MarshalBusException if the unmarshalling fails
     */
    public static Object[] unmarshal(Method method, long msgArgs) throws MarshalBusException {
        Type[] types = method.getGenericParameterTypes();
        int numArgs = getNumMembers(msgArgs);

        /*
         * Support Methods that have a purely variable parameter list; single parameter of "Object...".
         * This is a Java varargs parameter and is equivalent to an Object[], but will allow
         * a Method to accept any number of Object parameters (i.e. generic/unspecified).
         * This requires the types array to be reset to an array of Object elements with size numArgs.
         */
        boolean isVariable = method.isVarArgs() && (types.length == 1) && (types[0] == Object[].class);
        if (isVariable) {
            types = new Type[numArgs];
            for (int i = 0; i < numArgs; i++) {
                types[i] = Object.class;
            }
        }

        if (types.length != numArgs) {
            throw new MarshalBusException(
                "cannot marshal " + numArgs + " args into " + types.length + " parameters");
        }

        Object[] objects = new Object[numArgs];
        for (int i = 0; i < numArgs; ++i) {
            objects[i] = unmarshal(getMember(msgArgs, i), types[i]);
        }

        return isVariable ? new Object[]{objects}: objects;
    }

    /**
     * Marshals a Java object into a native MsgArg.
     *
     * @param msgArg the MsgArg to marshal the Java object into.
     * @param sig the desired signature of the marshalled MsgArg
     * @param arg the Java object
     * @throws MarshalBusException if the marshalling fails
     */
    public static void marshal(long msgArg, String sig, Object arg) throws BusException {
        try {
            int value = getEnumValue(arg);
            switch (sig.charAt(0)) {
            case ALLJOYN_BYTE:
                if (value != -1) {
                    set(msgArg, sig, (byte) value);
                } else {
                    set(msgArg, sig, ((Number) arg).byteValue());
                }
                break;
            case ALLJOYN_BOOLEAN:
                set(msgArg, sig, ((Boolean) arg).booleanValue());
                break;
            case ALLJOYN_INT16:
            case ALLJOYN_UINT16:
                if (value != -1) {
                    set(msgArg, sig, (short) value);
                } else {
                    set(msgArg, sig, ((Number) arg).shortValue());
                }
                break;
            case ALLJOYN_INT32:
            case ALLJOYN_UINT32:
                if (value != -1) {
                    set(msgArg, sig, value);
                } else {
                    set(msgArg, sig, ((Number) arg).intValue());
                }
                break;
            case ALLJOYN_INT64:
            case ALLJOYN_UINT64:
                if (value != -1) {
                    set(msgArg, sig, (long) value);
                } else {
                    set(msgArg, sig, ((Number) arg).longValue());
                }
                break;
            case ALLJOYN_DOUBLE:
                set(msgArg, sig, ((Number) arg).doubleValue());
                break;
            case ALLJOYN_STRING:
            case ALLJOYN_SIGNATURE:
            case ALLJOYN_OBJECT_PATH:
                if (arg == null) {
                    throw new MarshalBusException("cannot marshal null into '" + sig + "'");
                }
                set(msgArg, sig, (String) arg);
                break;
            case ALLJOYN_ARRAY:
                if (arg == null) {
                    throw new MarshalBusException("cannot marshal null into '" + sig + "'");
                }
                char elementTypeId = sig.charAt(1);
                if (ALLJOYN_DICT_ENTRY_OPEN == elementTypeId) {
                    arg = ((Map<?, ?>) arg).entrySet().toArray();
                }
                switch (elementTypeId) {
                case ALLJOYN_BYTE:
                    set(msgArg, sig, (byte[]) arg);
                    break;
                case ALLJOYN_BOOLEAN:
                    if (arg instanceof boolean[]) {
                        set(msgArg, sig, (boolean[]) arg);
                    } else {
                        set(msgArg, sig, (Boolean[]) arg);
                    }
                    break;
                case ALLJOYN_INT16:
                case ALLJOYN_UINT16:
                    set(msgArg, sig, (short[]) arg);
                    break;
                case ALLJOYN_INT32:
                case ALLJOYN_UINT32:
                    set(msgArg, sig, (int[]) arg);
                    break;
                case ALLJOYN_INT64:
                case ALLJOYN_UINT64:
                    set(msgArg, sig, (long[]) arg);
                    break;
                case ALLJOYN_DOUBLE:
                    set(msgArg, sig, (double[]) arg);
                    break;
                default:
                    String elemSig = sig.substring(1);
                    Object[] args = (Object[]) arg;
                    setArray(msgArg, elemSig, args.length);
                    for (int i = 0; i < getNumElements(msgArg); ++i) {
                        marshal(getElement(msgArg, i), elemSig, args[i]);
                    }
                    break;
                }
                break;
            case ALLJOYN_STRUCT_OPEN:
                Object[] args = Signature.structArgs(arg);
                String[] memberSigs = Signature.split(sig.substring(1, sig.length() - 1));
                if (memberSigs == null) {
                    throw new MarshalBusException("cannot marshal " + arg.getClass() + " into '"
                                                  + sig + "'");
                }
                setStruct(msgArg, memberSigs.length);
                for (int i = 0; i < getNumMembers(msgArg); ++i) {
                    marshal(getMember(msgArg, i), memberSigs[i], args[i]);
                }
                break;
            case ALLJOYN_VARIANT:
                Variant variant = (Variant) arg;
                if (variant.getMsgArg() != 0) {
                    setVariant(msgArg, sig, variant.getMsgArg());
                } else {
                    setVariant(msgArg);
                    marshal(getVal(msgArg), variant.getSignature(), variant.getValue());
                }
                break;
            case ALLJOYN_DICT_ENTRY_OPEN:
                Map.Entry<?, ?> entry = (Map.Entry<?, ?>) arg;
                String[] sigs = Signature.split(sig.substring(1, sig.length() - 1));
                if (sigs == null) {
                    throw new MarshalBusException("cannot marshal " + arg.getClass() + " into '"
                                                  + sig + "'");
                }
                setDictEntry(msgArg);
                marshal(getKey(msgArg), sigs[0], entry.getKey());
                marshal(getVal(msgArg), sigs[1], entry.getValue());
                break;
            default:
                throw new MarshalBusException("unimplemented '" + sig + "'");
            }
        } catch (Throwable th) {
            throw new MarshalBusException("cannot marshal " +
                                          ((arg == null) ? "null" : arg.getClass()) +
                                          " into '" + sig + "'", th);
        }
    }

    /**
     * Marshals an array of Java objects into a ALLJOYN_STRUCT containing the Java
     * objects as members.
     *
     * @param msgArg the MsgArg to marshal the Java object into.
     * @param sig the desired signature of the marshalled MsgArgs
     * @param args the Java objects
     * @throws MarshalBusException if the marshalling fails
     */
    public static void marshal(long msgArg, String sig, Object[] args) throws BusException {
        String[] sigs = Signature.split(sig);
        if (sigs == null) {
            throw new MarshalBusException("cannot marshal args into '" + sig + "', bad signature");
        }
        setStruct(msgArg, (args == null) ? 0 : args.length);
        for (int i = 0; i < getNumMembers(msgArg); ++i) {
            marshal(getMember(msgArg, i), sigs[i], args[i]);
        }
    }

    /**
     * Return the Type that corresponds to the given signature. For example,
     * 's' is String, and 'as' is String[]. A null or empty signature is invalid.
     *
     * @param sig the signature that describes an object's underlying data type
     * @return the Type of the signature
     * @throws BusException if the signature is invalid or its Java Type cannot be determined.
     */
    public static Type toType(String sig) throws BusException {
        if (sig == null || sig.isEmpty()) {
            throw new BusException("null or empty signature");
        }

        // Check if sig describes loose fields not in a container (then need to treat it as a struct)
        String[] sigArray = Signature.split(sig);
        if (sigArray != null && sigArray.length > 1) {
            sig = "(" + sig + ")";
        }

        Type result = null;
        try {
            switch (sig.charAt(0)) {
                case ALLJOYN_ARRAY: {
                    final Type componentType = toType(sig.substring(1));
                    // Check if signature is actually an array of dict-entries (a map)
                    if (sig.charAt(1) == ALLJOYN_DICT_ENTRY_OPEN) {
                        result = new ParameterizedType() {
                            public Type getRawType() { return HashMap.class; }
                            public Type getOwnerType() { return null; }
                            public Type[] getActualTypeArguments() {
                                return ((ParameterizedType)componentType).getActualTypeArguments();
                            }
                        };
                    } else {
                        Class<?> componentClass;
                        if (componentType instanceof ParameterizedType) {
                            Type rawType = ((ParameterizedType) componentType).getRawType();
                            rawType = (rawType == Map.class) ? HashMap.class : rawType;
                            componentClass = (Class<?>) rawType;
                        } else {
                            componentClass = (Class<?>) componentType;
                        }
                        result = Array.newInstance(componentClass,0).getClass();
                    }
                    break;
                }
                case ALLJOYN_DICT_ENTRY_OPEN: {
                    String[] sigs = Signature.split(sig.substring(1, sig.length()-1));
                    final Type keyType = toType(sigs[0]);
                    final Type valueType = toType(sigs[1]);
                    result = new ParameterizedType() {
                        public Type getRawType() { return AbstractMap.SimpleEntry.class; }
                        public Type getOwnerType() { return HashMap.class; }
                        public Type[] getActualTypeArguments() {
                            return new Type[] { keyType, valueType };
                        }
                    };
                    break;
                }
                case ALLJOYN_BOOLEAN:
                    result = Boolean.class;
                    break;
                case ALLJOYN_BYTE:
                    result = Byte.class;
                    break;
                case ALLJOYN_INT16:
                case ALLJOYN_UINT16:
                    result = Short.class;
                    break;
                case ALLJOYN_INT32:
                case ALLJOYN_UINT32:
                    result = Integer.class;
                    break;
                case ALLJOYN_INT64:
                case ALLJOYN_UINT64:
                    result = Long.class;
                    break;
                case ALLJOYN_DOUBLE:
                    result = Double.class;
                    break;
                case ALLJOYN_STRING:
                case ALLJOYN_SIGNATURE:
                case ALLJOYN_OBJECT_PATH:
                    result = String.class;
                    break;
                case ALLJOYN_STRUCT_OPEN:
                    result = Object[].class;
                    break;
                case ALLJOYN_VARIANT:
                    result = Variant.class;
                    break;
                case ALLJOYN_HANDLE:
                    result = Long.class;
                    break;
                default:
                    throw new BusException("unimplemented signature '" + sig + "'");
            }
        } catch (Throwable th) {
            throw new BusException("cannot handle signature '" + sig + "'", th);
        }

        return result;
    }

}