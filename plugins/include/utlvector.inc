/**
 * vim: set ts=4 sw=4 tw=99 noet :
 * SourceMod CUtlVector Extension
 */

#if defined _cutlvector_included
 #endinput
#endif
#define _cutlvector_included

/**
 * CUtlVector is a wrapper for Source engine's CUtlVector class.
 * It allows manipulation of CUtlVector instances in memory from SourcePawn.
 *
 * Note: This works with raw memory addresses and requires knowledge of the
 * element size in bytes since we cannot use C++ templates at runtime.
 */
methodmap CUtlVector < Handle
{
	/**
	 * Creates a CUtlVector wrapper for an existing CUtlVector in memory.
	 *
	 * @param address       Memory address of the CUtlVector instance.
	 * @param elementSize   Size of each element in bytes.
	 * @return             Handle to the CUtlVector wrapper.
	 * @error              Invalid address or element size.
	 */
	public native CUtlVector(Address address, int elementSize);

	/**
	 * Returns the number of elements in the vector.
	 *
	 * @return             Number of elements.
	 */
	property int Count
	{
		public native get();
	}

	/**
	 * Checks if the vector is empty.
	 *
	 * @return             True if empty, false otherwise.
	 */
	public native bool IsEmpty();

	/**
	 * Checks if an index is valid.
	 *
	 * @param index        Index to check.
	 * @return             True if valid, false otherwise.
	 */
	public native bool IsValidIndex(int index);

	/**
	 * Gets a value from the vector.
	 *
	 * @param index        Index to get from.
	 * @return             Value at the index.
	 * @error              Invalid index.
	 */
	public native any Get(int index);

	/**
	 * Sets a value in the vector.
	 *
	 * @param index        Index to set at.
	 * @param value        Value to set.
	 * @error              Invalid index.
	 */
	public native void Set(int index, any value);

	/**
	 * Adds an element to the tail of the vector.
	 *
	 * @param value        Value to add.
	 * @return             Index of the new element.
	 */
	public native int AddToTail(any value);

	/**
	 * Removes an element, preserving order.
	 *
	 * @param index        Index to remove.
	 * @error              Invalid index.
	 */
	public native void Remove(int index);

	/**
	 * Removes all elements from the vector.
	 */
	public native void RemoveAll();

	/**
	 * Finds an element in the vector.
	 *
	 * @param value        Value to find.
	 * @return             Index of the element, or -1 if not found.
	 */
	public native int Find(any value);

	/**
	 * Gets the base address of the vector's memory.
	 *
	 * @return             Base address of the vector's memory.
	 */
	public native Address GetBase();

	/**
	 * Gets the address of a specific element.
	 *
	 * @param index        Index of the element.
	 * @return             Address of the element.
	 * @error              Invalid index.
	 */
	public native Address GetAddress(int index);
};

public Extension __ext_cutlvector =
{
	name = "CUtlVector",
	file = "cutlvector.ext",
#if defined AUTOLOAD_EXTENSIONS
	autoload = 1,
#else
	autoload = 0,
#endif
#if defined REQUIRE_EXTENSIONS
	required = 1,
#else
	required = 0,
#endif
};

#if !defined REQUIRE_EXTENSIONS
public void __ext_cutlvector_SetNTVOptional()
{
	MarkNativeAsOptional("CUtlVector.CUtlVector");
	MarkNativeAsOptional("CUtlVector.Count.get");
	MarkNativeAsOptional("CUtlVector.IsEmpty");
	MarkNativeAsOptional("CUtlVector.IsValidIndex");
	MarkNativeAsOptional("CUtlVector.Get");
	MarkNativeAsOptional("CUtlVector.Set");
	MarkNativeAsOptional("CUtlVector.AddToTail");
	MarkNativeAsOptional("CUtlVector.Remove");
	MarkNativeAsOptional("CUtlVector.RemoveAll");
	MarkNativeAsOptional("CUtlVector.Find");
	MarkNativeAsOptional("CUtlVector.GetBase");
	MarkNativeAsOptional("CUtlVector.GetAddress");
}
#endif