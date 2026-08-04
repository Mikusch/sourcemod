#pragma semicolon 1
#pragma newdecls required
#include <testing>

enum struct TestStruct
{
	int intval;
	char strval[32];
}

public void OnPluginStart()
{
	ArrayList list = new ArrayList(sizeof(TestStruct));
	
	// --------------------------------------------------------------------------------

	SetTestContext("EmptyArrayTest");

	AssertEq("test_forward", list.FindString("index3", TestStruct::strval, -1, false), -1);
	AssertEq("test_reverse", list.FindString("index3", TestStruct::strval, -1, true), -1);
	AssertEq("test_forward", list.FindValue(3, TestStruct::intval, -1, false), -1);
	AssertEq("test_reverse", list.FindValue(3, TestStruct::intval, -1, true), -1);

	// --------------------------------------------------------------------------------

	// Fill
	TestStruct ts;
	for (int i = 0; i < 10; i++)
	{
		ts.intval = i;
		Format(ts.strval, sizeof(ts.strval), "index%d", i);
		list.PushArray(ts);
	}

	// --------------------------------------------------------------------------------

	SetTestContext("FindString");

	AssertEq("test_defaults", list.FindString("index3", TestStruct::strval), 3);

	AssertEq("test_forward", list.FindString("index3", TestStruct::strval, -1, false), 3);
	AssertEq("test_forward", list.FindString("index3", TestStruct::strval, 0, false), 3);
	AssertEq("test_forward", list.FindString("index3", TestStruct::strval, 2, false), 3);
	AssertEq("test_forward", list.FindString("index3", TestStruct::strval, 3, false), -1);
	AssertEq("test_forward", list.FindString("index3", TestStruct::strval, 10, false), -1);

	AssertEq("test_reverse", list.FindString("index3", TestStruct::strval, -1, true), 3);
	AssertEq("test_reverse", list.FindString("index3", TestStruct::strval, 0, true), -1);
	AssertEq("test_reverse", list.FindString("index3", TestStruct::strval, 3, true), -1);
	AssertEq("test_reverse", list.FindString("index3", TestStruct::strval, 4, true), 3);
	AssertEq("test_reverse", list.FindString("index3", TestStruct::strval, 10, true), 3);

	AssertEq("test_bottom", list.FindString("index0", TestStruct::strval, -1, false), 0);
	AssertEq("test_bottom", list.FindString("index0", TestStruct::strval, -1, true), 0);
	AssertEq("test_bottom", list.FindString("index0", TestStruct::strval, 1, true), 0);
	AssertEq("test_bottom", list.FindString("index0", TestStruct::strval, 10, false), -1);
	AssertEq("test_bottom", list.FindString("index0", TestStruct::strval, 10, true), 0);

	AssertEq("test_top", list.FindString("index9", TestStruct::strval, -1, false), 9);
	AssertEq("test_top", list.FindString("index9", TestStruct::strval, -1, true), 9);
	AssertEq("test_top", list.FindString("index9", TestStruct::strval, 8, false), 9);
	AssertEq("test_top", list.FindString("index9", TestStruct::strval, 10, false), -1);
	AssertEq("test_top", list.FindString("index9", TestStruct::strval, 10, true), 9);
	
	AssertEq("test_case_sensitive", list.FindString("INDEX0", TestStruct::strval, .caseSensitive = true), -1);
	AssertEq("test_case_sensitive", list.FindString("INDEX0", TestStruct::strval, .caseSensitive = false), 0);

	// --------------------------------------------------------------------------------

	SetTestContext("FindValue");

	AssertEq("test_defaults", list.FindValue(3, TestStruct::intval), 3);

	AssertEq("test_forward", list.FindValue(3, TestStruct::intval, -1, false), 3);
	AssertEq("test_forward", list.FindValue(3, TestStruct::intval, 0, false), 3);
	AssertEq("test_forward", list.FindValue(3, TestStruct::intval, 2, false), 3);
	AssertEq("test_forward", list.FindValue(3, TestStruct::intval, 3, false), -1);
	AssertEq("test_forward", list.FindValue(3, TestStruct::intval, 10, false), -1);

	AssertEq("test_reverse", list.FindValue(3, TestStruct::intval, -1, true), 3);
	AssertEq("test_reverse", list.FindValue(3, TestStruct::intval, 0, true), -1);
	AssertEq("test_reverse", list.FindValue(3, TestStruct::intval, 3, true), -1);
	AssertEq("test_reverse", list.FindValue(3, TestStruct::intval, 4, true), 3);
	AssertEq("test_reverse", list.FindValue(3, TestStruct::intval, 10, true), 3);

	AssertEq("test_bottom", list.FindValue(0, TestStruct::intval, -1, false), 0);
	AssertEq("test_bottom", list.FindValue(0, TestStruct::intval, -1, true), 0);
	AssertEq("test_bottom", list.FindValue(0, TestStruct::intval, 1, true), 0);
	AssertEq("test_bottom", list.FindValue(0, TestStruct::intval, 10, false), -1);
	AssertEq("test_bottom", list.FindValue(0, TestStruct::intval, 10, true), 0);

	AssertEq("test_top", list.FindValue(9, TestStruct::intval, -1, false), 9);
	AssertEq("test_top", list.FindValue(9, TestStruct::intval, -1, true), 9);
	AssertEq("test_top", list.FindValue(9, TestStruct::intval, 8, false), 9);
	AssertEq("test_top", list.FindValue(9, TestStruct::intval, 10, false), -1);
	AssertEq("test_top", list.FindValue(9, TestStruct::intval, 10, true), 9);
	
	// --------------------------------------------------------------------------------

	SetTestContext("IterateOverFindString");
	int found, index;

	// Duplicate last entry
	list.PushArray(ts);

	found = 0; index = -1;
	while ((index = list.FindString("index9", TestStruct::strval, index, false)) != -1)
	{
		found++;
	}
	AssertEq("test_find_all_strings_forward", found, 2);

	found = 0; index = -1;
	while ((index = list.FindString("index9", TestStruct::strval, index, true)) != -1)
	{
		found++;
	}
	AssertEq("test_find_all_strings_reverse", found, 2);
	
	// --------------------------------------------------------------------------------
	
	SetTestContext("IterateOverFindValue");

	found = 0, index = -1;
	while ((index = list.FindValue(9, TestStruct::intval, index, false)) != -1)
	{
		found++;
	}
	AssertEq("test_find_all_values_forward", found, 2);

	found = 0; index = -1;
	while ((index = list.FindValue(9, TestStruct::intval, index, true)) != -1)
	{
		found++;
	}
	AssertEq("test_find_all_values_reverse", found, 2);

	// --------------------------------------------------------------------------------

	SetTestContext("FindArray");

	int needle3[] = {3};
	AssertEq("test_findvalue_equivalent", list.FindArray(needle3, sizeof(needle3), TestStruct::intval), 3);

	ArrayList cells = new ArrayList(4);

	int n1[] = {1};
	AssertEq("test_empty", cells.FindArray(n1, sizeof(n1)), -1);

	// Fill
	int row0[] = {1, 2, 3, 4};
	int row1[] = {5, 5, 5, 5};
	int row2[] = {9, 42, 42, 9};
	int row3[] = {42, 42, 42, 42};
	int row4[] = {1, 2, 1, 2};

	cells.PushArray(row0);
	cells.PushArray(row1);
	cells.PushArray(row2);
	cells.PushArray(row3);
	cells.PushArray(row4);

	int n4[] = {4};
	int n5[] = {5};
	int n7[] = {7};
	int n9[] = {9};
	int n42[] = {42};
	int n12[] = {1, 2};
	int n21[] = {2, 1};
	int n23[] = {2, 3};
	int n942[] = {9, 42};
	int n4242[] = {42, 42};

	AssertEq("test_defaults", cells.FindArray(n1, sizeof(n1)), 0);
	AssertEq("test_defaults", cells.FindArray(n5, sizeof(n5)), 1);
	AssertEq("test_defaults", cells.FindArray(n7, sizeof(n7)), -1);
	AssertEq("test_defaults", cells.FindArray(n12, sizeof(n12)), 0);
	AssertEq("test_defaults", cells.FindArray(row0, sizeof(row0)), 0);
	AssertEq("test_defaults", cells.FindArray(row4, sizeof(row4)), 4);

	AssertEq("test_start", cells.FindArray(n942, sizeof(n942), 0), 2);
	AssertEq("test_start", cells.FindArray(n23, sizeof(n23), 1), 0);
	AssertEq("test_start", cells.FindArray(n4242, sizeof(n4242), 1), 2);
	AssertEq("test_start", cells.FindArray(n4242, sizeof(n4242), 2), 3);
	AssertEq("test_start", cells.FindArray(n12, sizeof(n12), 2), 4);
	AssertEq("test_start", cells.FindArray(n4, sizeof(n4), 3), 0);
	AssertEq("test_start", cells.FindArray(n9, sizeof(n9), 3), 2);

	AssertEq("test_limit", cells.FindArray(n42, sizeof(n42), 1, 2), 2);
	AssertEq("test_limit", cells.FindArray(n42, sizeof(n42), 1, 3), 3);
	AssertEq("test_limit", cells.FindArray(n42, sizeof(n42), 2, 2), 3);
	AssertEq("test_limit", cells.FindArray(n5, sizeof(n5), 0, 4), 1);
	AssertEq("test_limit", cells.FindArray(n12, sizeof(n12), 0, 4), 4);

	AssertEq("test_limit_rest", cells.FindArray(n5, sizeof(n5), 0, -1), 1);
	AssertEq("test_limit_rest", cells.FindArray(n42, sizeof(n42), 0, -1), 3);
	AssertEq("test_limit_rest", cells.FindArray(n42, sizeof(n42), 3, -1), 3);
	AssertEq("test_limit_rest", cells.FindArray(n9, sizeof(n9), 3, .limit = -1), 2);

	AssertEq("test_limit_partial", cells.FindArray(n12, sizeof(n12), 0, 3), 4);
	AssertEq("test_limit_partial", cells.FindArray(n21, sizeof(n21), 1, 3), 4);
	AssertEq("test_limit_partial", cells.FindArray(n12, sizeof(n12), 1, 3), -1);

	// --------------------------------------------------------------------------------

	SetTestContext("FindArrayInArray");

	ArrayList single = new ArrayList();
	single.Push(10);
	single.Push(20);
	single.Push(30);

	int n20[] = {20};
	int n40[] = {40};

	AssertEq("test_native", FindArrayInArray(single, n20, sizeof(n20)), 1);
	AssertEq("test_native", FindArrayInArray(single, n20, sizeof(n20), 0, -1), 1);
	AssertEq("test_native", FindArrayInArray(single, n40, sizeof(n40)), -1);

	ArrayList floats = new ArrayList(2);

	float frow0[] = {1.0, 2.0};
	float frow1[] = {3.0, 4.0};

	floats.PushArray(frow0);
	floats.PushArray(frow1);

	float f2[] = {2.0};

	AssertEq("test_floats", floats.FindArray(frow1, sizeof(frow1)), 1);
	AssertEq("test_floats", floats.FindArray(f2, sizeof(f2), 1), 0);

	delete cells;
	delete single;
	delete floats;

	// --------------------------------------------------------------------------------

	PrintToServer("OK");
}