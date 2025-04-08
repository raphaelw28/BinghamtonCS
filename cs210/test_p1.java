class test_p1
{
	public static void main(String[] args)
	{
		// make sure we have 6 commandline arguments, if not, output what we are looking
		// for and exit
		if( args.length != 6 )
		{
			System.out.println("format test_p1 \"filename\" \"lower bound\" \"upper bound\" \"test value\" \"test value 1\" \"test value 2\"");
			System.exit(0);
		}
		
		try
		{
			// get the commandline arguments and convert them to ints
			int lowerBound = Integer.parseInt(args[1]);
			int upperBound = Integer.parseInt(args[2]);
			int testValue = Integer.parseInt(args[3]);
			int testValue1 = Integer.parseInt(args[4]);
			int testValue2 = Integer.parseInt(args[5]);
			
			// instantiate a BufferedReader to read from the input file
			java.io.BufferedReader input = new java.io.BufferedReader(new java.io.InputStreamReader(new java.io.FileInputStream(args[0])));
			String inn;
			
			// ArrayList<String> to temporarily store the lines from the input file,
			// we will be discussing ArrayList shortly, for now just think of it
			// as a resizable array
			java.util.ArrayList<String> inputLists = new java.util.ArrayList<>();
			
			// read the input file and store the lines in inputLists
			while( (inn = input.readLine()) != null )
			{
				if( (inn.trim().length() > 0) && (!inn.trim().startsWith("#")) )
				{
					inputLists.add(inn.trim());
				}
			}
			input.close();
			
			// allocate the array to sort 
			int[] valuesToSort = new int[inputLists.size()];
			System.out.println("\"" + args[0] + "\"" + " has " + valuesToSort.length + " values to process");
			
			// define the array that will be returned from selection sort
			int[] valuesSorted;
			
			// allocate the array to hold the test values 
			int[] testValues = new int[inputLists.size()];
			
			// populate the array to sort and the test value array
			for( int i = 0; i < valuesToSort.length; i++ )
			{
				try
				{
					testValues[i] = Integer.parseInt(inputLists.get(i));
					valuesToSort[i] = testValues[i];
				}
				catch(Exception e)
				{
					System.err.println("\"" + inputLists.get(i) + "\" is not an integer");
				}
			}

			// output the commandline parameters
			System.out.println("lowerBound = " + lowerBound);
			System.out.println("upperBound = " + upperBound);
			System.out.println("testValue = " + testValue);
			System.out.println("testValue1 = " + testValue1);
			System.out.println("testValue2 = " + testValue2);
			
			// instantiate a copy of the program to test
			wong_p1 testClass = new wong_p1();
			
			// call selection sort
			valuesSorted = testClass.selectionSort(lowerBound, upperBound, valuesToSort);
			
			// output the sorted values
			System.out.println("sorted values");
			for( int i = 0; i < valuesSorted.length; i++ )
			{
				System.out.println(valuesSorted[i]);
			}
			
			// call the for loop test and output the results
			int forLoopTestResult = testClass.forLoopTest(testValues, lowerBound, upperBound, testValue);
			System.out.println("forLoopTestResult = " + forLoopTestResult);
			
			// call the while loop test and output the results
			int whileLoopTestResult = testClass.whileLoopTest(testValues, lowerBound, upperBound, testValue);
			System.out.println("whileLoopTestResult = " + whileLoopTestResult);
			
			// call the do while loop test and output the results
			int doWhileLoopTestResult = testClass.doWhileLoopTest(testValues, lowerBound, upperBound, testValue1, testValue2);
			System.out.println("doWhileLoopTestResult = " + doWhileLoopTestResult);
			
			// call the do while loop test a second time, with testvalue1 and testValue2 swapped, 
			// and output the results
			int doWhileLoopTestResult2 = testClass.doWhileLoopTest(testValues, lowerBound, upperBound, testValue2, testValue1);
			System.out.println("doWhileLoopTestResult2 = " + doWhileLoopTestResult2);
			
			// call the switch test and output the results
			int[] switchTestResult = testClass.switchTest(testValues, lowerBound, upperBound);
			for( int i = 0; i < switchTestResult.length; i++ )
			{
				System.out.println("switchTestResult[" + i + "] = " + switchTestResult[i]);
			}			
		}
		catch(Exception e)
		{
			System.out.println(e.toString());
			System.exit(0);
		}
		
	}
}
