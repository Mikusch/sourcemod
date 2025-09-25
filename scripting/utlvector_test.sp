/**
 * CUtlVector Extension Test Plugin
 * Tests basic functionality of the CUtlVector extension
 */

#include <sourcemod>
#include <utlvector>
#include <sourcescramble>

#pragma semicolon 1
#pragma newdecls required

public Plugin myinfo =
{
	name = "CUtlVector Test",
	author = "Community",
	description = "Test plugin for CUtlVector extension",
	version = "1.0.0",
	url = ""
};

public void OnPluginStart()
{
	RegAdminCmd("sm_cutlvector_test", Command_TestVector, ADMFLAG_ROOT, "Test CUtlVector functionality");
	RegAdminCmd("sm_cutlvector_stress", Command_StressTest, ADMFLAG_ROOT, "Stress test CUtlVector");
}

public Action Command_TestVector(int client, int args)
{
	PrintToServer("========================================");
	PrintToServer("Starting CUtlVector Test Suite");
	PrintToServer("========================================");

	// Test 1: Create a simulated CUtlVector structure
	if (!Test_BasicOperations())
	{
		PrintToServer("[FAIL] Basic operations test failed");
		return Plugin_Handled;
	}
	PrintToServer("[PASS] Basic operations test");

	// Test 2: Element access
	if (!Test_ElementAccess())
	{
		PrintToServer("[FAIL] Element access test failed");
		return Plugin_Handled;
	}
	PrintToServer("[PASS] Element access test");

	// Test 3: Find operations
	if (!Test_FindOperations())
	{
		PrintToServer("[FAIL] Find operations test failed");
		return Plugin_Handled;
	}
	PrintToServer("[PASS] Find operations test");

	// Test 4: Remove operations
	if (!Test_RemoveOperations())
	{
		PrintToServer("[FAIL] Remove operations test failed");
		return Plugin_Handled;
	}
	PrintToServer("[PASS] Remove operations test");

	PrintToServer("========================================");
	PrintToServer("All tests passed!");
	PrintToServer("========================================");

	ReplyToCommand(client, "[CUtlVector] All tests passed! Check server console for details.");
	return Plugin_Handled;
}

bool Test_BasicOperations()
{
	// Allocate memory for a simulated CUtlVector structure
	// CUtlVector layout: CUtlMemory (3 ints) + m_Size (1 int) + m_pElements (1 pointer)
	// Total size: approximately 20-24 bytes depending on platform
	MemoryBlock block = new MemoryBlock(32);

	// Initialize as empty vector (all zeros already from MemoryBlock)
	Address vectorAddr = block.Address;

	// Create wrapper for int vector (4 bytes per element)
	CUtlVector vec = new CUtlVector(vectorAddr, 4);

	// Test initial state
	if (vec.Count != 0)
	{
		PrintToServer("Error: Initial count should be 0, got %d", vec.Count);
		delete vec;
		delete block;
		return false;
	}

	if (!vec.IsEmpty())
	{
		PrintToServer("Error: Vector should be empty initially");
		delete vec;
		delete block;
		return false;
	}

	if (vec.IsValidIndex(0))
	{
		PrintToServer("Error: Index 0 should be invalid in empty vector");
		delete vec;
		delete block;
		return false;
	}

	delete vec;
	delete block;
	return true;
}

bool Test_ElementAccess()
{
	// Create a larger block to simulate vector with pre-allocated space
	MemoryBlock block = new MemoryBlock(256);
	Address vectorAddr = block.Address;

	// Manually set up a vector with some elements
	// We'll simulate a vector that already has allocated memory and elements

	// Allocate data buffer separately
	MemoryBlock dataBlock = new MemoryBlock(64); // Space for 16 integers

	// Set up CUtlMemory structure
	// m_pMemory pointer
	block.StoreToOffset(0, view_as<int>(dataBlock.Address), NumberType_Int32);
	// m_nAllocationCount = 16
	block.StoreToOffset(4, 16, NumberType_Int32);
	// m_nGrowSize = 0
	block.StoreToOffset(8, 0, NumberType_Int32);
	// m_Size = 5 (5 elements)
	block.StoreToOffset(12, 5, NumberType_Int32);

	// Store test data in the data buffer
	dataBlock.StoreToOffset(0, 100, NumberType_Int32);  // [0] = 100
	dataBlock.StoreToOffset(4, 200, NumberType_Int32);  // [1] = 200
	dataBlock.StoreToOffset(8, 300, NumberType_Int32);  // [2] = 300
	dataBlock.StoreToOffset(12, 400, NumberType_Int32); // [3] = 400
	dataBlock.StoreToOffset(16, 500, NumberType_Int32); // [4] = 500

	CUtlVector vec = new CUtlVector(vectorAddr, 4);

	// Test count
	if (vec.Count != 5)
	{
		PrintToServer("Error: Count should be 5, got %d", vec.Count);
		delete vec;
		delete dataBlock;
		delete block;
		return false;
	}

	// Test element access
	if (vec.Get(0) != 100)
	{
		PrintToServer("Error: Element 0 should be 100, got %d", vec.Get(0));
		delete vec;
		delete dataBlock;
		delete block;
		return false;
	}

	if (vec.Get(4) != 500)
	{
		PrintToServer("Error: Element 4 should be 500, got %d", vec.Get(4));
		delete vec;
		delete dataBlock;
		delete block;
		return false;
	}

	// Test set operation
	vec.Set(2, 999);
	int value = dataBlock.LoadFromOffset(8, NumberType_Int32);
	if (value != 999)
	{
		PrintToServer("Error: After Set(2, 999), element should be 999, got %d", value);
		delete vec;
		delete dataBlock;
		delete block;
		return false;
	}

	// Test IsValidIndex
	if (!vec.IsValidIndex(0) || !vec.IsValidIndex(4))
	{
		PrintToServer("Error: Indices 0-4 should be valid");
		delete vec;
		delete dataBlock;
		delete block;
		return false;
	}

	if (vec.IsValidIndex(5) || vec.IsValidIndex(-1))
	{
		PrintToServer("Error: Index 5 and -1 should be invalid");
		delete vec;
		delete dataBlock;
		delete block;
		return false;
	}

	delete vec;
	delete dataBlock;
	delete block;
	return true;
}

bool Test_FindOperations()
{
	// Set up vector with test data
	MemoryBlock block = new MemoryBlock(256);
	MemoryBlock dataBlock = new MemoryBlock(64);

	// Set up CUtlMemory
	block.StoreToOffset(0, view_as<int>(dataBlock.Address), NumberType_Int32);
	block.StoreToOffset(4, 16, NumberType_Int32); // allocation count
	block.StoreToOffset(8, 0, NumberType_Int32);  // grow size
	block.StoreToOffset(12, 4, NumberType_Int32); // size = 4

	// Store test data
	dataBlock.StoreToOffset(0, 10, NumberType_Int32);
	dataBlock.StoreToOffset(4, 20, NumberType_Int32);
	dataBlock.StoreToOffset(8, 30, NumberType_Int32);
	dataBlock.StoreToOffset(12, 20, NumberType_Int32); // Duplicate value

	CUtlVector vec = new CUtlVector(block.Address, 4);

	// Test Find
	int index = vec.Find(20);
	if (index != 1)
	{
		PrintToServer("Error: Find(20) should return 1, got %d", index);
		delete vec;
		delete dataBlock;
		delete block;
		return false;
	}

	index = vec.Find(99);
	if (index != -1)
	{
		PrintToServer("Error: Find(99) should return -1, got %d", index);
		delete vec;
		delete dataBlock;
		delete block;
		return false;
	}

	delete vec;
	delete dataBlock;
	delete block;
	return true;
}

bool Test_RemoveOperations()
{
	// Set up vector with test data
	MemoryBlock block = new MemoryBlock(256);
	MemoryBlock dataBlock = new MemoryBlock(64);

	// Set up CUtlMemory
	block.StoreToOffset(0, view_as<int>(dataBlock.Address), NumberType_Int32);
	block.StoreToOffset(4, 16, NumberType_Int32); // allocation count
	block.StoreToOffset(8, 0, NumberType_Int32);  // grow size
	block.StoreToOffset(12, 3, NumberType_Int32); // size = 3

	// Store test data [10, 20, 30]
	dataBlock.StoreToOffset(0, 10, NumberType_Int32);
	dataBlock.StoreToOffset(4, 20, NumberType_Int32);
	dataBlock.StoreToOffset(8, 30, NumberType_Int32);

	CUtlVector vec = new CUtlVector(block.Address, 4);

	// Test RemoveAll
	vec.RemoveAll();
	if (vec.Count != 0)
	{
		PrintToServer("Error: After RemoveAll, count should be 0, got %d", vec.Count);
		delete vec;
		delete dataBlock;
		delete block;
		return false;
	}

	if (!vec.IsEmpty())
	{
		PrintToServer("Error: After RemoveAll, vector should be empty");
		delete vec;
		delete dataBlock;
		delete block;
		return false;
	}

	delete vec;
	delete dataBlock;
	delete block;
	return true;
}

public Action Command_StressTest(int client, int args)
{
	PrintToServer("========================================");
	PrintToServer("Starting CUtlVector Stress Test");
	PrintToServer("========================================");

	// Allocate larger blocks for stress testing
	MemoryBlock block = new MemoryBlock(1024);
	MemoryBlock dataBlock = new MemoryBlock(4096); // 1024 integers

	// Set up CUtlMemory
	block.StoreToOffset(0, view_as<int>(dataBlock.Address), NumberType_Int32);
	block.StoreToOffset(4, 1024, NumberType_Int32); // allocation count
	block.StoreToOffset(8, 0, NumberType_Int32);   // grow size
	block.StoreToOffset(12, 0, NumberType_Int32);   // size = 0

	CUtlVector vec = new CUtlVector(block.Address, 4);

	// Add many elements
	PrintToServer("Adding 100 elements...");
	for (int i = 0; i < 100; i++)
	{
		// Simulate AddToTail by incrementing size and writing data
		int currentSize = block.LoadFromOffset(12, NumberType_Int32);
		dataBlock.StoreToOffset(currentSize * 4, i * 10, NumberType_Int32);
		block.StoreToOffset(12, currentSize + 1, NumberType_Int32);
	}

	if (vec.Count != 100)
	{
		PrintToServer("Error: Expected 100 elements, got %d", vec.Count);
		delete vec;
		delete dataBlock;
		delete block;
		ReplyToCommand(client, "[CUtlVector] Stress test failed!");
		return Plugin_Handled;
	}

	// Verify some elements
	PrintToServer("Verifying elements...");
	bool success = true;
	for (int i = 0; i < 10; i++)
	{
		int value = vec.Get(i * 10);
		if (value != i * 100)
		{
			PrintToServer("Error: Element %d should be %d, got %d", i * 10, i * 100, value);
			success = false;
		}
	}

	// Find operations on large dataset
	PrintToServer("Testing find operations...");
	int index = vec.Find(500); // Should be at index 50
	if (index != 50)
	{
		PrintToServer("Error: Find(500) should return 50, got %d", index);
		success = false;
	}

	// Test GetBase and GetAddress
	Address baseAddr = vec.GetBase();
	Address elemAddr = vec.GetAddress(10);

	if (view_as<int>(elemAddr) != view_as<int>(baseAddr) + 40) // 10 * 4 bytes
	{
		PrintToServer("Error: GetAddress(10) calculation incorrect");
		success = false;
	}

	delete vec;
	delete dataBlock;
	delete block;

	if (success)
	{
		PrintToServer("========================================");
		PrintToServer("Stress test completed successfully!");
		PrintToServer("========================================");
		ReplyToCommand(client, "[CUtlVector] Stress test passed!");
	}
	else
	{
		PrintToServer("========================================");
		PrintToServer("Stress test failed!");
		PrintToServer("========================================");
		ReplyToCommand(client, "[CUtlVector] Stress test failed! Check server console.");
	}

	return Plugin_Handled;
}