class wong_p1
{
        public wong_p1()
        {
        }
        
        public int[] selectionSort(int lowerBound, int upperBound, int[] testValue)
        {
                for (int i = lowerBound; i <= upperBound ; i++)
                {
                        int minimum = i;
                        for (int j = i + 1; j < upperBound + 1; j++)
                                if (testValue[j] < testValue[minimum])
                                        minimum = j;
                        int t = testValue[i];
                        testValue[i] = testValue[minimum];
                        testValue[minimum] = t;
                }
                return testValue;

        }

        public int forLoopTest(int[] values, int lowerBound, int upperBound, int[] testValue)
        {
                int counter = 0;
                for(int i = lowerBound; i <= upperBound; i++)
                        {
                        if(values[i] > testValue[i - lowerBound])
                                counter++;
                        }
                        return counter;
        }
        
        public int whileLoopTest(int[] values, int lowerBound, int upperBound, int testValue)
        {
                int counter = 0;
                int i = 0;
                while(i < lowerBound)
                {
                        if(values[i] <= testValue)
                        {
                                counter++;
                        }
                        i++;
                }
                i = upperBound + 1;
                while(i < values.length)
                {
                        if(values[i] <= testValue)
                        {
                                counter++;
                        }
                        i++;
                }
                return counter;
        }

        public int doWhileLoopTest(int[] values, int lowerBound, int upperBound, int testValue1, int testValue2)
        {
                int counter = 0;
                do
                {
                        int test = values[lowerBound];
                        if(testValue1 < testValue2)
                        {
                                if(test >= testValue1 && test <= testValue2)
                                        counter++;
                                lowerBound++;
                        }
                else
                        {
                                if(test >= testValue1 && test <= testValue2)
                                        counter++;
                                 lowerBound++;
                        }
                }
                while(lowerBound <= upperBound);
                return counter;
        }

        public int[] switchTest(int[] values, int lowerBound, int upperBound)
        {
                int[] results = new int[11];
                for (int i = lowerBound; i <= upperBound; i++)
                {
                        int value = values[i];
                        switch (value) 
                        {
                                case 0:
                                        results[0]++;
                                        break;
                                case 3:
                                        results[1]++;
                                case 6:
                                        results[2]++;
                                        break;
                                case 9:
                                        results[3]++;
                                case 12:
                                        results[4]++;
                                        break;
                                case 15:
                                        results[5]++;
                                case 18:
                                        results[6]++;
                                        break;
                                case 21:
                                        results[7]++;
                                case 24:
                                        results[8]++;
                                        break;
                                case 27:
                                        results[9]++;
                                default:
                                        results[10]++;
                                        break;
                        }
                }
                return results;
        }

        
}
